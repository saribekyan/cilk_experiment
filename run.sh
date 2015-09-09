cores=(1 2 4 8 16   32      64      80)
nodes=(0 0 0 0  0  0,1 0,1,2,3 0,1,2,3)

for ((i=0;i<8;++i));
do
    echo Starting ${cores[i]} cores
    for j in 0 1 2;
    do
        numactl --cpunodebind=${nodes[i]} ./build/Main --n_worker ${cores[i]} --mat_size 500 --n_mat 500 --experiment $j --separate_jobs
        numactl --cpunodebind=${nodes[i]} ./build/Main --n_worker ${cores[i]} --mat_size 500 --n_mat 500 --experiment $j
    done
done

