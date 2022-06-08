// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4
// Simulates a producer and a consumer that share a unbounded buffer

#include <stdlib.h>
#include <stdio.h>

#include "simulation.h"

int main(int argc, char* argv[]) {
  simulation_t* simulation = simulation_create();
  if (simulation) {
    int result = simulation_run(simulation, argc, argv);
    simulation_destroy(simulation);
    return result;
  } else {
    fprintf(stderr, "error: could not allocate simulation\n");
    return EXIT_FAILURE;
  }
}
