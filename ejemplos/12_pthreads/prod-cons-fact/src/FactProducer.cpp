/// @copyright 2022 ECCI, Universidad de Costa Rica. All rights reserved

#include "FactProducer.hpp"
#include <iostream>

int FactProducer::run() {
    for (size_t i = 0; i < products.size(); ++i) {
        FactWorkUnit unit = *products[i];
        this->produce(unit);
        std::cout << "Produced: " << unit.number << std::endl;
    }

    for (int c = 0; c < numConsumers; ++c) {
        this->produce(FactWorkUnit());
    }

    return EXIT_SUCCESS;
}
