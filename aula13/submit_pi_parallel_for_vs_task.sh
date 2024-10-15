#!/bin/bash
#SBATCH --job-name=pi_parallel_for_vs_task    # Nome do job
#SBATCH --output=output_pi_%j.txt          # Arquivo de saída (com ID do job)
#SBATCH --ntasks=1                            # Número de tarefas (apenas 1 processo)
#SBATCH --cpus-per-task=4                     # Número de CPUs por tarefa
#SBATCH --time=00:05:00  # Tempo máximo de execução ajustado para 30 minutos
#SBATCH --mem=12000M

# Executar o programa paralelo
./pi_parallel_for_vs_task
