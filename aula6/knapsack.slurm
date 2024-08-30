#!/bin/bash
#SBATCH --job-name=knapsack                 # Nome do job
#SBATCH --output=knapsack%j.txt             # Arquivo de saída com o ID do job
#SBATCH --ntasks=1                          # Número de tarefas (apenas 1 processo)
#SBATCH --cpus-per-task=1                   # Número de CPUs por tarefa
#SBATCH --mem=512MB                         # Memória máxima alocada
#SBATCH --time=00:30:00                     # Tempo máximo de execução (30 minutos)
#SBATCH --partition=espec                   # Fila do cluster a ser utilizada

# Função para executar o programa e medir o tempo
function run_knapsack {
    local input_file=$1

    echo "[$(date)] Executando mochila com entrada: $input_file"
    # Usando o comando time sem o caminho absoluto
    time ./knapsack $input_file
    echo -e "\n[$(date)] Execução com entrada $input_file finalizada"
}

# Caminho para os arquivos de entrada
INPUT_FILES=("entrada1.txt" "entrada2.txt" "entrada3.txt" "entrada4.txt")

# Loop através dos arquivos de entrada e executar o programa para cada um
for input_file in "${INPUT_FILES[@]}"; do
    run_knapsack $input_file
done

echo "[$(date)] Job knapsack finalizado"
