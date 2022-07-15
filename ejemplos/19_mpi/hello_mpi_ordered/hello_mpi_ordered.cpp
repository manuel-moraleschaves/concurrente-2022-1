// Copyright 2022 Jose Mena Arias <jose.menaarias@ucr.ac.cr>

#include <mpi.h>
#include <iostream>
#include <iomanip>

int main(int argc, char* args[]) {
    if (MPI_Init(&argc, &args) == MPI_SUCCESS) {
        int rank = -1;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        int processCount = -1;
        MPI_Comm_size(MPI_COMM_WORLD, &processCount);

        char hostname[MPI_MAX_PROCESSOR_NAME];
        int hostnameLength = -1;
        MPI_Get_processor_name(hostname, &hostnameLength);

        bool canPrint = false;

        if (rank != 0) {
            MPI_Recv(&canPrint, 1, MPI_C_BOOL, rank - 1, 0,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        const double startTime = MPI_Wtime();
        std::cout << hostname << ":" << rank << "/" << processCount
                << ": Hello MPI!!" << std::endl;

        double elapsedTime = MPI_Wtime() - startTime;
        std::cout << "Process " << rank << ": time elapsed "
            << std::fixed << std::setprecision(16) << elapsedTime << "s"
            << std::endl;

        int nextRank = (rank + 1) % processCount;

        MPI_Send(&canPrint, 1, MPI_C_BOOL, nextRank, /*tag*/ 0, MPI_COMM_WORLD);

        MPI_Finalize();
    }

    return 0;
}
