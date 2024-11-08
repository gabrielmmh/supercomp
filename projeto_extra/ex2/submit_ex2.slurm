#!/bin/bash
#SBATCH --job-name=ex2
#SBATCH --output=output_%x.txt
#SBATCH --nodes=2
#SBATCH --ntasks=4
#SBATCH --time=00:05:00
#SBATCH --mem=12000M

echo "[Exercício 2 - MPI]"
mpirun -np 4 ./mpi
echo -e ""

echo "[Exercício 2 - OpenMP]"
./openmp
echo -e ""
