#define _GNU_SOURCE
#define _USE_GNU
#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>
#define TIME_UNIT 100
long long int cur_time(){
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec * 1000000000ll + t.tv_nsec;
}
int main(int argc, char* argv[]){
    int yield=0;
    //if(!strcmp(argv[3],"RR"))yield=1;
    //else if(!strcmp(argv[3],"PSJF"))yield=1;
    yield=1;
    char* name = argv[1];
    pid_t pid = getpid();
    int exec_time = strtol(argv[2], NULL, 10);
    long long start_time = cur_time();

    int i;
    for(i = 0; i < exec_time; i++){
        if(i&&i%TIME_UNIT==0&&yield)
            sched_yield();
        volatile unsigned long j;
        for(j = 0; j < 1000000UL; j++); 
    }

    long long end_time=cur_time();
    long long d_time=end_time-start_time;

    fprintf(stderr,"[%s] pid: %d ",name,pid);
    fprintf(stderr,"%lld.%09lld ",start_time/1000000000ll, start_time%1000000000ll);
    fprintf(stderr,"%lld.%09lld ",end_time/1000000000ll, end_time%1000000000ll);
    fprintf(stderr,"%lld.%09lld ",d_time/1000000000ll, d_time%1000000000ll);
    fprintf(stderr,"\n");

    FILE* f=fopen("/dev/kmsg", "a");
    fprintf(f,"[Project1] %d %lld.%09lld %lld.%09lld\n",pid,start_time/1000000000ll, start_time%1000000000ll,end_time/1000000000ll, end_time%1000000000ll);
    fclose(f);
    return 0;
}
