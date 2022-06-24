/// @copyright 2022 ECCI, Universidad de Costa Rica. All rights reserved

#include <unistd.h>
#include <iostream>
#include <vector>

#include "FactWorkUnit.hpp"
#include "FactConsumer.hpp"
#include "FactProducer.hpp"
#include "prodcons/Queue.hpp"

#define NUM_UNITS 10

int main() {
    int numConsumers = sysconf(_SC_NPROCESSORS_ONLN);
    int numUnits = NUM_UNITS;

    std::cout << "Starting simulation ..." << std::endl;

    std::vector<FactWorkUnit*> units;
    double results[NUM_UNITS];

    for (int unit = 0; unit < numUnits; ++unit) {
        results[unit] = -1.0;
        units.push_back(new FactWorkUnit(unit, &results[unit]));
    }

    Queue<FactWorkUnit>* queue = new Queue<FactWorkUnit>();

    FactProducer* producer = new FactProducer(units, numConsumers);
    producer->setProducingQueue(queue);

    std::vector<FactConsumer*> consumers;

    for (int con = 0; con < numConsumers; ++con) {
        FactConsumer* consumer = new FactConsumer();
        consumer->setConsumingQueue(queue);
        consumers.push_back(consumer);
        consumer->startThread();
    }

    producer->startThread();

    std::cout << "Waiting producer to finish" << std::endl;

    producer->waitToFinish();

    std::cout << "Waiting consumers to finish" << std::endl;
    for (int con = 0; con < numConsumers; ++con) {
        consumers[con]->waitToFinish();
        delete consumers[con];
    }

    std::cout << "\nResult: " << std::endl;
    for (int unit = 0; unit < numUnits; ++unit) {
        std::cout << "Fact of " << units[unit]->number << " is "
                                        << *units[unit]->result << std::endl;
        delete units[unit];
    }
    delete queue;
    delete producer;
}
