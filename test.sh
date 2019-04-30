inp=$(basename -s .txt -a $(ls OS_PJ1_Test/*.txt 2>/dev/null))
for i in $inp ; do
    echo "$i"
    (stdbuf -o 0 ./schedule <OS_PJ1_Test/$i.txt | tee OS_PJ1_Test/$i.out) 3>&1 1>&2 2>&3 | tee OS_PJ1_Test/$i.err
done
