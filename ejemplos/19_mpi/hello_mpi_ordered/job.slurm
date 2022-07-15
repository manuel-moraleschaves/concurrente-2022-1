#!/bin/bash
#SBATCH --job-name=mpi_ordered
#SBATCH --output=result.txt
#SBATCH --partition=nu-wide
#SBATCH --ntasks=6
#SBATCH --nodes=6
#SBATCH --ntasks-per-node=1
#SBATCH --time=00:05:00

mpiexec -np 6 ./bin/hello_mpi_ordered