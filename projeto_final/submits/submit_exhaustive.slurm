#!/bin/bash
#SBATCH --job-name=exhaustive               # Nome do job
#SBATCH --output=output_exhaustive.txt      # Arquivo de saída com ID do job
#SBATCH --nodes=2                           # Número de nós
#SBATCH --ntasks=4                          # Número total de tarefas MPI
#SBATCH --time=00:05:00                     # Tempo limite
#SBATCH --mem=12000M                        # Memória total por nó

# Executa o programa uma vez para processar todos os arquivos de RNA
mpirun -np 4 ../src/exhaustive

echo -e "Execução concluída para todos os arquivos.\n"
