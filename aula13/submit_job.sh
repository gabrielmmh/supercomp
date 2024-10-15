#!/bin/bash
#SBATCH --job-name=omp_scheduler_test
#SBATCH --output=output_omp_schedulers.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem=1024MB  # Ajuste de memória
#SBATCH --time=00:05:00

./omp_schedulers 10