/// @copyright 2022 ECCI, Universidad de Costa Rica. All rights reserved

#include "FactConsumer.hpp"
#include <iostream>

int FactConsumer::run() {
    this->consumeForever();

    return EXIT_SUCCESS;
}

void FactConsumer::consume(FactWorkUnit product) {
    if (product.number > 0) {
        *product.result = 1;

        for (int i = 1; i <= product.number; ++i) {
            *product.result *= i;
        }
    }

    std::cout << "      Consumed" << product.number << std::endl;
}
