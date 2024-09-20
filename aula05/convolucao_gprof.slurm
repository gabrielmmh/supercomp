#!/bin/bash
#SBATCH --job-name=analysis_gprof          # Nome do job
#SBATCH --output=analysis_gprof_%j.txt     # Arquivo de saída com o ID do job
#SBATCH --ntasks=1                         # Número de tarefas (1 processo)
#SBATCH --cpus-per-task=1                  # Número de threads por tarefa (1 thread por processo)
#SBATCH --mem=512MB                        # Memória total alocada por nó (512 MB)
#SBATCH --time=00:05:00                    # Tempo máximo de execução (5 minutos)
#SBATCH --partition=espec                  # Fila do cluster a ser utilizada

# Executar o programa com suporte ao Gprof
srun --ntasks=1 ./convolucao_gprof 100

# Após a execução, o arquivo gmon.out será gerado no mesmo diretório
