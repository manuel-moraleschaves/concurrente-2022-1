#!/bin/bash
#SBATCH --job-name=mpi_unordered
#SBATCH --output=result.txt
#SBATCH --partition=nu-wide
#SBATCH --ntasks=12
#SBATCH --nodes=12
#SBATCH --ntasks-per-node=1
#SBATCH --time=00:05:00

mpiexec -np 12 ./bin/hello_mpi_unordered