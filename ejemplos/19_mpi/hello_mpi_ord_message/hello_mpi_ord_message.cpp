// Copyright 2022 Jose Mena Arias <jose.menaarias@ucr.ac.cr>

#include <mpi.h>
#include <iostream>
#include <iomanip>

#define MAX_MESSAGE_SIZE 256

int main(int argc, char* args[]) {
    if (MPI_Init(&argc, &args) == MPI_SUCCESS) {
        int rank = -1;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        int processCount = -1;
        MPI_Comm_size(MPI_COMM_WORLD, &processCount);

        char hostname[MPI_MAX_PROCESSOR_NAME];
        int hostnameLength = -1;
        MPI_Get_processor_name(hostname, &hostnameLength);

        std::string message = std::to_string(rank) + "/"
            + std::to_string(processCount) + ": Hello MPI!!";

        if (rank != 0) {
            MPI_Send(&message[0], message.length(), MPI_CHAR, 0, 0,
                MPI_COMM_WORLD);
        } else {
            const double startTime = MPI_Wtime();
            std::cout << message << std::endl;

            for (int source = 1; source < processCount; ++source) {
                MPI_Recv(&message[0], MAX_MESSAGE_SIZE, MPI_CHAR, source, 0,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                std::cout << message << std::endl;
            }

            double elapsedTime = MPI_Wtime() - startTime;
            std::cout << "Process " << rank << ": time elapsed " << std::fixed
                << std::setprecision(16) << elapsedTime << "s" << std::endl;
        }


        MPI_Finalize();
    }

    return 0;
}
