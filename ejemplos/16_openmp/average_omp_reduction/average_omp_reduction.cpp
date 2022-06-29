// Copyright 2022 Jose Mena Arias <jose.menaarias@ucr.ac.cr>

#include <iostream>
#include <vector>
#include <omp.h>

int main(int argc, char** args) {
    
    int threadsNum = omp_get_max_threads();
    if (argc >= 2) {
        threadsNum = atoi(args[1]);
    }

    std::vector<double> values;
    double current_value;
    while (std::cin >> current_value) {
        values.push_back(current_value);
    }

    double sum = 0.0;

    #pragma omp parallel for num_threads(threadsNum) default(none) \
        shared(values) reduction(+:sum)
    for (size_t i = 0; i < values.size(); ++i) {
        sum += values[i];
    }

    double average = sum / values.size();

    std::cout << "Average: " << average << std::endl;
}
