#!/bin/bash
#SBATCH --job-name=ex1
#SBATCH --output=output_%x.txt
#SBATCH --nodes=2
#SBATCH --ntasks=4
#SBATCH --time=00:05:00
#SBATCH --mem=12000M

echo "[Exercício 1]"
mpirun -np 4 ./mpi_openmp
echo -e ""