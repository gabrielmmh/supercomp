#!/bin/bash
#SBATCH --job-name=ex2
#SBATCH --output=output_%x.txt
#SBATCH --nodes=2
#SBATCH --ntasks=4
#SBATCH --time=00:05:00
#SBATCH --mem=12000M

echo "[Exercício 2]"
for i in {1..22}; do
    echo "Processando chr${i}_upper.subst.fa"
    mpirun -np 4 ./mpi_openmp ../dados/preprocessed/chr${i}_upper.subst.fa
    echo -e ""
done
