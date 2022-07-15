#!/bin/bash
#SBATCH --job-name=mpi_hybrid
#SBATCH --output=result.txt
#SBATCH --partition=nu-wide
#SBATCH --ntasks=4
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=1
#SBATCH --time=00:05:00

mpiexec -np 4 ./bin/hello_mpi_hybrid