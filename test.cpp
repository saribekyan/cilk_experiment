#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <chrono>

#include <boost/program_options.hpp>
#include <cilk/cilk_api.h>

namespace po = boost::program_options;

int main(int argc, char** argv) {
    po::options_description desc("Allowed options");

    int workers, m, n, exp;
    desc.add_options()
        ("help", "produce help message")
        ("n_workers", po::value< int >(&workers)->default_value(1), "number of workers")
        ("n_mat",  po::value< int >(&n)->default_value(100), "total number of matrices")
        ("mat_size", po::value< int >(&m)->default_value(100), "size of each matrix (multiplying matrices of this size)")
        ("separate_jobs", "if set, each matrix is a separate job; otherwise creates n_workers jobs each with approximately equal number of matrices")
        ("experiment", po::value< int >(&exp), "the number of experiment")
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << '\n';
        exit(0);
    }

    bool separate;
    if (vm.count("separate_jobs")) {
        separate = true;
    } else {
        separate = false;
    }

    if ( __cilkrts_set_param("nworkers", std::to_string(workers).c_str()) != 0)
    {
       printf("Failed to set worker count\n");
       return 1;
    }

    auto start = std::chrono::system_clock::now();

    int jobs = workers;
    if (separate) {
       jobs = n; 
    }
    cilk_for (int job = 0; job < jobs; ++job) {
        int matPerJob = n / jobs;
        if (job < n % workers) {
            ++matPerJob;
        }

        // create some stupid matrix
        int** mat = new int*[m], **res = new int*[m];
        for (int i = 0; i < m; ++i) {
            mat[i] = new int[m]();
            res[i] = new int[m]();
        }

        while (matPerJob--) {
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < m; ++j) {
                    mat[i][j] = (i * m + matPerJob) + j;
                }
            }

            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < m; ++j)
                    for (int k = 0; k < m; ++k) {
                        res[i][j] += mat[i][k] * mat[k][i];
                }
            }
        }
    }
    auto end = std::chrono::system_clock::now();

    printf("(%d, %d, %d) : %lf\n", workers, (int)separate, exp, (end - start).count() / 1000000.0);

    FILE* f = fopen("result.txt", "a");
    fprintf(f, "(%d, %d, %d) : %lf\n", workers, (int)separate, exp, (end - start).count() / 1000000.0);
    return 0;
}

