#!/bin/bash
#SBATCH --job-name=tetris_solver_mpi
#SBATCH --output=result.txt
#SBATCH --partition=nu-wide
#SBATCH --ntasks=4
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=1
#SBATCH --time=00:15:00

module load gcc/7.2.0
module load mpich/3.2.1-gcc-7.2.0

mpiexec -n 4 ./bin/tetris_solver_mpi ./test/Test4.txt 8