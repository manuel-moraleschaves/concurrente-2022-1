/// @copyright 2022 ECCI, Universidad de Costa Rica. All rights reserved

#ifndef FACT_PRODUCER_H
#define FACT_PRODUCER_H

#include <vector>

#include "prodcons/Producer.hpp"
#include "FactWorkUnit.hpp"

class FactProducer : public Producer<FactWorkUnit> {
 private:
        std::vector<FactWorkUnit*> products;
        int numConsumers;

 public:
        FactProducer(std::vector<FactWorkUnit*> products, int numConsumers):
            products(products), numConsumers(numConsumers) {}

        int run() override;
};

#endif
