#!/bin/bash
#SBATCH --job-name=knapsack_combined              # Nome do job
#SBATCH --output=return/knapsack_combined_%j.txt         # Arquivo de saída com o ID do job
#SBATCH --ntasks=1                                # Número de tarefas (apenas 1 processo)
#SBATCH --cpus-per-task=1                         # Número de CPUs por tarefa
#SBATCH --mem=1024MB                              # Memória máxima alocada
#SBATCH --time=03:00:00                           # Tempo máximo de execução (3 horas)
#SBATCH --partition=espec                         # Fila do cluster a ser utilizada

EXECUTABLE="./knapsack_combined"
INPUT_FILES=("entradas/entrada1.txt" "entradas/entrada2.txt" "entradas/entrada3.txt")

# Função para executar um algoritmo específico
function run_algorithm {
    local algorithm=$1
    local input_file=$2

    echo "[$(date)] Executando $algorithm com entrada: $input_file"
    # Usando o comando time embutido do shell para medir o tempo
    time $EXECUTABLE $algorithm $input_file
    echo -e "\n[$(date)] Execução de $algorithm com entrada $input_file finalizada"
}

# Executar os três algoritmos para cada entrada
for input_file in "${INPUT_FILES[@]}"; do
    run_algorithm "full" $input_file
    run_algorithm "substituition" $input_file
    run_algorithm "hillclimbing" $input_file
done

echo "[$(date)] Job knapsack_combined finalizado"