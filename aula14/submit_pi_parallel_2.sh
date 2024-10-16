#!/bin/bash
#SBATCH --job-name=pi_parallel_2
#SBATCH --output=output_pi_parallel_2.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --time=00:05:00
#SBATCH --mem=12000M

./pi_parallel_2
