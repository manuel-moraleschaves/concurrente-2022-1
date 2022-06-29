// Copyright 2022 Jose Mena Arias <jose.menaarias@ucr.ac.cr>

#include <iostream>
#include <iomanip>
#include <omp.h>

int main(int argc, char** args) {
    
    int threadsNum = omp_get_max_threads();
    int n = threadsNum;
    if (argc >= 2) {
        n = atoi(args[1]);
    }

    double sum = 0.0;

    #pragma omp parallel for num_threads(threadsNum) default(none) \
        shared(n) reduction(+:sum)
    for ( int k = 0; k < n; ++k) {
        double factor = (k%2 == 0) ? 1.0 : -1.0;
        sum += factor/(2*k+1);
    }

    double pi = 4.0 * sum;
    std::cout << "pi is approximately " << std::fixed <<
        std::setprecision(16) << pi << "\n";

}