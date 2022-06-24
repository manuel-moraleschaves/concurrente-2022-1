// Copyright 2022 Jose Mena Arias <jose.menaarias@ucr.ac.cr>

#include <omp.h>
#include <iostream>

int main(int argc, char** args) {
    int threadsNum = omp_get_max_threads();
    if (argc >= 2) {
        threadsNum = atoi(args[1]);
    }

    int iterationsCount = threadsNum;

    if (argc >= 3) {
        iterationsCount = atoi(args[2]);
    }

    std::cout << "Hello from main thread!" << std::endl;

    double start_time = omp_get_wtime();

    #pragma omp parallel num_threads(threadsNum)
    {
        #pragma omp for
        for (int i = 0; i < 2; ++i) {
            #pragma omp critical
            {
                std::cout << "1: " << omp_get_thread_num() << "/" << omp_get_num_threads()
                        << ": Iteration " << i << std::endl;
            }
        }

        #pragma omp single
        std::cout << std::endl;

        #pragma omp for
        for (int i = 0; i < iterationsCount; ++i) {
            #pragma omp critical
            {
                std::cout << "2: " << omp_get_thread_num() << "/" << omp_get_num_threads()
                        << ": Iteration " << i << std::endl;
            }
        }

        #pragma omp single
        std::cout << std::endl;

        #pragma omp for
        for (int i = 0; i < iterationsCount; ++i) {
            #pragma omp critical
            {
                std::cout << "3: " << omp_get_thread_num() << "/" << omp_get_num_threads()
                        << ": Iteration " << i << std::endl;
            }
        }
    }

    double end_time = omp_get_wtime();

    std::cout << "Time elapsed: " << (end_time - start_time) << std::endl;
}
