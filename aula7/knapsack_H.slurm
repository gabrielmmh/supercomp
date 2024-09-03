#!/bin/bash
#SBATCH --job-name=knapsack_H                  # Nome do job
#SBATCH --output=knapsack_H_%j.txt             # Arquivo de saída com o ID do job
#SBATCH --ntasks=1                             # Número de tarefas (apenas 1 processo)
#SBATCH --cpus-per-task=1                      # Número de CPUs por tarefa
#SBATCH --mem=4096MB                           # Memória máxima alocada
#SBATCH --time=00:10:00                        # Tempo máximo de execução (3 horas)
#SBATCH --partition=espec                      # Fila do cluster a ser utilizada

HEURISTIC_EXEC="./knapsack_heuristic"
INPUT_FILES=("entrada1.txt" "entrada2.txt" "entrada3.txt" "entrada4.txt")

# Função para executar as heurísticas
function run_heuristics {
    local input_file=$1
    for i in {1..5}; do
        echo "[$(date)] Executando heurística embaralhada ($i) com entrada: $input_file"
        time $HEURISTIC_EXEC 1 $input_file
        echo "[$(date)] Execução da heurística embaralhada ($i) com entrada $input_file finalizada"

        echo "[$(date)] Executando heurística probabilística ($i) com entrada: $input_file"
        time $HEURISTIC_EXEC 2 $input_file
        echo "[$(date)] Execução da heurística probabilística ($i) com entrada $input_file finalizada"
    done
}

# Executar as heurísticas para cada entrada
for input_file in "${INPUT_FILES[@]}"; do
    run_heuristics $input_file
done

echo "[$(date)] Job knapsack finalizado"
