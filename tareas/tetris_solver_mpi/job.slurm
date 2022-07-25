#!/bin/bash
#SBATCH --job-name=tetris_solver_mpi
#SBATCH --output=result.txt
#SBATCH --partition=nu-wide
#SBATCH --ntasks=4
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=1
#SBATCH --time=00:30:00

mpiexec -n 4 ./bin/tetris_solver_mpi ./test/Test4.txt 8