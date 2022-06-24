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

    #pragma omp parallel for num_threads(threadsNum)
    for (int i = 0; i < iterationsCount; ++i) {
        #pragma omp critical
        {
            std::cout << omp_get_thread_num() << "/" << omp_get_num_threads()
                    << ": Iteration " << i << std::endl;
        }
    }
    double end_time = omp_get_wtime();

    std::cout << "Time elapsed: " << (end_time - start_time) << std::endl;
}
