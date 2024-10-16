#!/bin/bash
#SBATCH --job-name=pi_seq
#SBATCH --output=output_pi_seq.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=00:05:00
#SBATCH --mem=12000M

./pi_seq
