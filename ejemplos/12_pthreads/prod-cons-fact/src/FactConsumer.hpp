/// @copyright 2022 ECCI, Universidad de Costa Rica. All rights reserved

#ifndef FACT_CONSUMER_H
#define FACT_CONSUMER_H

#include "prodcons/Consumer.hpp"
#include "FactConsumer.hpp"
#include "FactWorkUnit.hpp"

class FactConsumer : public Consumer<FactWorkUnit> {
 public:
        FactConsumer() {}

        void consume(FactWorkUnit product) override;

        int run() override;
};

#endif

