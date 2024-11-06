#!/bin/bash
#SBATCH --job-name=mpi_openmp_exercises
#SBATCH --output=output_%x.txt
#SBATCH --nodes=2
#SBATCH --ntasks=4
#SBATCH --time=00:05:00
#SBATCH --mem=12000M

echo "[Exercício 1 - MPI]"
mpirun -np 4 ./mpi
echo -e ""

echo "[Exercício 1 - OpenMP]"
./openmp
echo -e ""
