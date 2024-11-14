#!/bin/bash
#SBATCH --job-name=ex4_translation          # Nome do job
#SBATCH --output=output_ex4_translation_%j.txt  # Arquivo de saída com ID do job
#SBATCH --nodes=2                           # Número de nós
#SBATCH --ntasks=4                          # Número total de tarefas MPI
#SBATCH --time=00:05:00                     # Tempo limite
#SBATCH --mem=12000M                        # Memória total por nó

# Executa o programa uma vez para processar todos os arquivos de RNA
mpirun -np 4 ./mpi_openmp

echo -e "Execução concluída para todos os arquivos.\n"
