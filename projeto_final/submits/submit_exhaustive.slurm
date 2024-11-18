#!/bin/bash
#SBATCH --job-name=exhaustive               # Nome do job
#SBATCH --output=../outputs/output_exhaustive.txt      # Arquivo de saída
#SBATCH --nodes=1                           # Número de nós (apenas 1 necessário para execução sequencial)
#SBATCH --ntasks=1                          # Número total de tarefas
#SBATCH --cpus-per-task=1                   # Apenas 1 CPU necessária
#SBATCH --time=00:05:00                     # Tempo limite
#SBATCH --mem=12000M                        # Memória total por nó

# Executa o programa diretamente
../src/exhaustive

echo -e "Execução concluída.\n"
