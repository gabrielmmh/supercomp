#!/bin/bash
#SBATCH --job-name=convolucao_optimization # Nome do job
#SBATCH --output=convolucao_optimization_%j.txt # Arquivo de saída com o ID do job
#SBATCH --ntasks=1                               # Número de tarefas (apenas 1 processos)
#SBATCH --cpus-per-task=1                        # Número de threads por tarefa (apenas 1 thread por processo)
#SBATCH --mem=512MB                             # Memória total alocada por nó (512 MB)
#SBATCH --time=00:10:00                          # Tempo máximo de execução (até 10 minutos)
#SBATCH --partition=espec                       # Fila do cluster a ser utilizada

# Função para executar o programa com diferentes configurações de iterações e diferentes versões do executável
function run_convolucao {
    local iterations=$1
    local exec_name=$2

    echo "[$(date)] Executando $exec_name com $iterations iterações"
    srun --ntasks=1 --cpus-per-task=1 --mem=512MB ./$exec_name $iterations
    echo "[$(date)] Convolução com $iterations iterações ($exec_name) finalizada"
}

# Executar as diferentes versões do programa com diferentes números de iterações
run_convolucao 30 "convolucao_basicao"
run_convolucao 50 "convolucao_basicao"
run_convolucao 100 "convolucao_basicao"

run_convolucao 30 "convolucao_O1"
run_convolucao 50 "convolucao_O1"
run_convolucao 100 "convolucao_O1"

run_convolucao 30 "convolucao_O2"
run_convolucao 50 "convolucao_O2"
run_convolucao 100 "convolucao_O2"

run_convolucao 30 "convolucao_O3"
run_convolucao 50 "convolucao_O3"
run_convolucao 100 "convolucao_O3"

echo "[$(date)] Job convolucao_optimization finalizado"
