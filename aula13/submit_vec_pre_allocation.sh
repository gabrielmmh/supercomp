#!/bin/bash
#SBATCH --job-name=vec_pre_allocation       # Nome do job
#SBATCH --output=output_pre_allocation.txt  # Nome do arquivo de saída
#SBATCH --ntasks=1                          # Número de tarefas (1 processo)
#SBATCH --cpus-per-task=4                   # Número de CPUs por tarefa
#SBATCH --time=00:05:00                     # Tempo máximo de execução (5 minutos)
#SBATCH --mem=12000M

# Executar o código compilado 3x
./vec_pre_allocation
./vec_pre_allocation
./vec_pre_allocation
