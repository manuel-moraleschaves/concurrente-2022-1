/// @copyright 2022 ECCI, Universidad de Costa Rica. All rights reserved

#ifndef FACT_WORK_UNIT_H
#define FACT_WORK_UNIT_H


class FactWorkUnit {
 public:
    int number;
    double* result;

 public:
    explicit FactWorkUnit(int number = -1, double* result = NULL):
        number(number), result(result) {}

    inline bool operator==(const FactWorkUnit& other) const {
        return this->number == other.number && this->result == other.result;
    }
};

#endif
