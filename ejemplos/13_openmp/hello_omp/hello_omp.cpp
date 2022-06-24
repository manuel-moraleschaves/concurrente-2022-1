// Copyright 2022 Jose Mena Arias <jose.menaarias@ucr.ac.cr>

#include <omp.h>
#include <iostream>

int main(int argc, char** args) {
    int threadsNum = omp_get_max_threads();
    int threadsCreated = threadsNum;

    if (argc == 2) {
        threadsNum = atoi(args[1]);
    }

    std::cout << "Hello from main thread!" << std::endl;

    double start_time = omp_get_wtime();

    #pragma omp parallel num_threads(threadsNum) private(threadsCreated) \
        shared(threadsNum, std::cout) default(none)
        {
            threadsCreated = omp_get_num_threads();

            #pragma omp critical
            {
                std::cout << omp_get_thread_num() << "/" << threadsCreated
                    << ": Hello from secondary thread!" << std::endl;
            }
        }
    
    double end_time = omp_get_wtime();

    std::cout << "Time elapsed: " << (end_time - start_time) << std::endl;
}
