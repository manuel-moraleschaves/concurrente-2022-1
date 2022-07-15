// Copyright 2022 Jose Mena Arias <jose.menaarias@ucr.ac.cr>
#include <omp.h>
#include <iostream>
#include <string>
#include <iomanip>

int main(int argc, char *argv[]) {
    int n = -1;

    if (argc > 1) {
        n = std::stoi(argv[1]);
    } else {
        std::cerr << "Invalid number of parameters." << std::endl;
        return 1;
    }

    double factor = 1.0;
    double sum = 0.0;

    #pragma omp parallel for default(none) private(factor) shared(n) \
        reduction(+: sum)
    for (int k = 0; k < n; k++) {
        if (k % 2 == 0) {
            factor = 1.0;
        } else {
            factor = -1.0;
        }
        sum += factor/(2*k+1);
    }

    double pi = 4 * sum;

    std::cout << "pi is approximately "
                << std::fixed << std::setprecision(16) << pi << std::endl;

    return 0;
}
