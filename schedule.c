#define _GNU_SOURCE
#define _USE_GNU
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sched.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<time.h>

#define MAX_NAME_LENGTH ((int)64)
#define MAX_PROCESS_NUM ((int)64)
#define TIME_QUANTUM ((int)500)
#define TIME_UNIT ((int)100)
#define PRIORITY_HIGH (int)99
#define PRIORITY_LOW (int)1

enum{FIFO, RR, SJF, PSJF};
char plcy_names[4][10]={"FIFO","RR","SJF","PSJF"};

#define swap(x, y) do { typeof(x) SWAP=(x); (x)=(y); (y)=SWAP; } while (0)

long long int cur_time(){
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec * 1000000000ll + t.tv_nsec;
}

typedef struct process{
    char name[MAX_NAME_LENGTH];
    int rd_time;
    int ex_time;
    int ID;
}process;

process pool[MAX_PROCESS_NUM];

process *waiting[MAX_PROCESS_NUM];
int waiting_top,proc_fin;

int proc_cmp(const void *a,const void *b){
    process *p1=(process *)a;
    process *p2=(process *)b;
    if(p1->rd_time<p2->rd_time)
        return -1;
    if(p1->rd_time>p2->rd_time)
        return 1;
    if(p1->ID<p2->ID)
        return -1;
    if(p1->ID>p2->ID)
        return 1;
    return 0;
}

void proc_insert(int policy, process *P){
    if(policy==FIFO||policy==RR)
        waiting[waiting_top]=P;
    if(policy==PSJF||policy==SJF){
        waiting[waiting_top]=P;
        for(int i=waiting_top;i>0;i--){
            if(waiting[i]->ex_time<waiting[i-1]->ex_time)
                swap(waiting[i],waiting[i-1]);
            else break;
        }
    }
    waiting_top++;
}

void proc_swap_end(){
    for(int i=1;i<waiting_top;i++)
        swap(waiting[i-1],waiting[i]);
    if(waiting[waiting_top-1]==NULL)
        waiting_top--,proc_fin++;
}

int proc_exec(int policy){
    int length;
    switch(policy){
        case FIFO:
        case SJF:
            length=waiting[0]->ex_time;
            waiting[0]->ex_time=0;
            waiting[0]=NULL;
            proc_swap_end();
            break;
        case RR:
            if(waiting[0]->ex_time>TIME_QUANTUM){
                length=TIME_QUANTUM;
                waiting[0]->ex_time-=TIME_QUANTUM;
            }
            else{
                length=waiting[0]->ex_time;
                waiting[0]->ex_time=0;
                waiting[0]=NULL;
            }
            proc_swap_end();
            break;
        case PSJF:
            length=TIME_UNIT;
            if(length>waiting[0]->ex_time)
                length=waiting[0]->ex_time;
            waiting[0]->ex_time-=length;
            if(waiting[0]->ex_time==0){
                waiting[0]=NULL;
                proc_swap_end();
            }
            break;
        default:
            length=0;
    }
    return length;
}

void set_affinity(int affinity){
    cpu_set_t cpu_mask;
    CPU_ZERO(&cpu_mask);
    CPU_SET(affinity,&cpu_mask);
    if(sched_setaffinity(0,sizeof(cpu_set_t),&cpu_mask)!=0){
        perror("sched_setaffinity error");
        exit(EXIT_FAILURE);
    }
}

void set_scheduler(pid_t pid,int priority){
    struct sched_param param;
    param.sched_priority=priority;
    if(sched_setscheduler(pid,SCHED_FIFO,&param)!=0){
        perror("sched_setscheduler error");
        exit(EXIT_FAILURE);
    }
}

int main(){
    int proc_num;
    char plcy_name[MAX_NAME_LENGTH];
    scanf("%s",plcy_name);
    scanf("%d",&proc_num);

    if(proc_num>MAX_PROCESS_NUM){
        perror("proc_num>MAX_PROCESS_NUM");
        exit(EXIT_FAILURE);
    }

    for(int i=0;i<proc_num;i++){
        scanf("%s%d%d",pool[i].name,&pool[i].rd_time,&pool[i].ex_time);
        pool[i].ID=i;
    }
    qsort(pool,proc_num,sizeof(process),proc_cmp);

    int policy=0;
    for(int i=0;i<4;i++)
        if(!strcmp(plcy_name,plcy_names[i]))
            policy=i;

    for(int i=0;i<MAX_PROCESS_NUM;i++)
        waiting[i]=NULL;

    set_affinity(0);
    set_scheduler(getpid(),PRIORITY_HIGH);

    int now=0;
    int pnum=0;
    pid_t pids[MAX_PROCESS_NUM];
    int ex_length=0;
    int running_id=-1;
    int tot_length=0;
    while(proc_fin!=proc_num||ex_length){
        while(pnum<proc_num&&pool[pnum].rd_time<=now){
            pid_t pid=fork();
            if(pid<0){
                perror("fork error");
                exit(EXIT_FAILURE);
            }
            else if(pid==0){
                char ex_time[10];
                sprintf(ex_time,"%d",pool[pnum].ex_time);
                set_affinity(0);
                set_scheduler(getpid(),PRIORITY_LOW);
                if(execlp("./process","process",pool[pnum].name,ex_time,plcy_name,NULL)<0){
                    perror("execlp error");
                    exit(EXIT_FAILURE);
                }
            }
            sched_yield();
            pids[pool[pnum].ID]=pid;
            proc_insert(policy,&pool[pnum]);
            pnum++;
        }
        if(waiting[0]==NULL&&!ex_length){
            volatile unsigned long i;
            for(i=0;i<1000000UL;i++);
            now++;
        }
        else{
            if(ex_length==0){
                if(running_id!=waiting[0]->ID)tot_length=0;
                else fprintf(stderr,"\e[1A\e[0K\r");
                running_id=waiting[0]->ID;
                ex_length=proc_exec(policy);
                tot_length+=ex_length;
                fprintf(stderr,"Run: %s %d unit\n",pool[running_id].name,tot_length);
            }
            set_scheduler(pids[running_id],PRIORITY_HIGH);
            sched_yield();
            set_scheduler(pids[running_id],PRIORITY_LOW);
            if(ex_length>=TIME_UNIT){
                ex_length-=TIME_UNIT;
                now+=TIME_UNIT;
            }
            else{
                now-=ex_length;
                ex_length=0;
            }
            if(ex_length==0&&pool[running_id].ex_time==0){
                int status;
                waitpid(pids[running_id],&status,0);
           }
        }
    }
    return 0;
}
