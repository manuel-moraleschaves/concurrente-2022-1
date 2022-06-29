#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <vector>

void print_mapping(const char* const type, const std::vector<int>& mapping) {
  std::cout << std::endl << type;
  for (size_t index = 0; index < mapping.size(); ++index) {
    std::cout << " " << mapping[index];
  }
}

int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  int iteration_count = thread_count;
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }

  int block_size = 1;
  if (argc >= 4) {
    block_size = atoi(argv[3]);
  }

  std::vector<int> mapping(iteration_count);

  std::cout << "         ";
  for (size_t index = 0; index < mapping.size(); ++index) {
    std::cout << " " << index;
  }

  #pragma omp parallel for num_threads(thread_count) default(none) \
    shared(iteration_count) shared(mapping) shared(block_size) schedule(static, block_size)
  for (int iteration = 0; iteration < iteration_count; ++iteration) {
    mapping[iteration] = omp_get_thread_num();
  }
  print_mapping("static   ", mapping);

  #pragma omp parallel for num_threads(thread_count) default(none) \
    shared(iteration_count) shared(mapping) shared(block_size) schedule(dynamic,block_size)
  for (int iteration = 0; iteration < iteration_count; ++iteration) {
    mapping[iteration] = omp_get_thread_num();
  }
  print_mapping("dynamic  ", mapping);

  #pragma omp parallel for num_threads(thread_count) default(none) \
    shared(iteration_count) shared(mapping) shared(block_size) schedule(guided,block_size)
  for (int iteration = 0; iteration < iteration_count; ++iteration) {
    mapping[iteration] = omp_get_thread_num();
  }
  print_mapping("guided   ", mapping);

  #pragma omp parallel for num_threads(thread_count) default(none) \
    shared(iteration_count) shared(mapping) schedule(runtime)
  for (int iteration = 0; iteration < iteration_count; ++iteration) {
    mapping[iteration] = omp_get_thread_num();
  }
  print_mapping("runtime  ", mapping);

  std::cout << std::endl;
}
