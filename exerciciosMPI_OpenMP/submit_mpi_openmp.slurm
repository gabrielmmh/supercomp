#!/bin/bash
#SBATCH --job-name=mpi_openmp_exercises
#SBATCH --output=output_%x.txt
#SBATCH --nodes=2
#SBATCH --ntasks=4
#SBATCH --time=00:05:00
#SBATCH --mem=12000M

mpirun -np 2 ./ex1
echo -e ""

mpirun -np 4 ./ex2
echo -e ""

mpirun -np 4 ./ex3
echo -e ""

mpirun -np 4 ./ex4
echo -e ""
