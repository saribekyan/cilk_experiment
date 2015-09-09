cores=(1 2 4 8 16   32      64      80)
nodes=(0 0 0 0  0  0,1 0,1,2,3 0,1,2,3)

echo "" > result.txt

for ((i=0;i<8;++i));
do
    echo Starting ${cores[i]} cores | tee result.txt
    for j in 0 1 2 3 4;
    do
        #numactl --cpunodebind=${nodes[i]} ./Main --n_worker ${cores[i]} --mat_size 200 --n_mat 1000 --experiment $j --separate_jobs
        numactl --cpunodebind=${nodes[i]} ./Main --n_worker ${cores[i]} --mat_size 200 --n_mat 1000 --experiment $j
    done
done

