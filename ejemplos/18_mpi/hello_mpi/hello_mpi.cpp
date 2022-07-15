// Copyright 2022 Jose Mena Arias <jose.menaarias@ucr.ac.cr>

#include <mpi.h>
#include <iostream>

int main(int argc, char* args[]) {
    if (MPI_Init(&argc, &args) == MPI_SUCCESS) {
        int rank = -1;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        int processCount = -1;
        MPI_Comm_size(MPI_COMM_WORLD, &processCount);

        char hostname[MPI_MAX_PROCESSOR_NAME];
        int hostnameLength = -1;
        MPI_Get_processor_name(hostname, &hostnameLength);

        std::cout << hostname << ":" << rank << "/" << processCount
                << ": Hello MPI!!" << std::endl;

        MPI_Finalize();
    }

    return 0;
}
