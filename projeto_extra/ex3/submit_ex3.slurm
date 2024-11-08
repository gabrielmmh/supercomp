#!/bin/bash
#SBATCH --job-name=ex3
#SBATCH --output=output_%x.txt
#SBATCH --nodes=2
#SBATCH --ntasks=4
#SBATCH --time=00:05:00
#SBATCH --mem=12000M

echo "[Exercício 3]"
mpirun -np 4 ./mpi_openmp
