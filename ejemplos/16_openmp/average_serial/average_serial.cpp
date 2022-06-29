// Copyright 2022 Jose Mena Arias <jose.menaarias@ucr.ac.cr>

#include <iostream>
#include <vector>

int main() {
    
    std::vector<double> values;
    double current_value;
    while (std::cin >> current_value) {
        values.push_back(current_value);
    }

    double sum = 0.0;

    for (size_t i = 0; i < values.size(); ++i) {
        sum += values[i];
    }

    double average = sum / values.size();

    std::cout << "Average: " << average << std::endl;
}
