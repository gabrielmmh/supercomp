#!/bin/bash
#SBATCH --job-name=knapsack_full               # Nome do job
#SBATCH --output=knapsack_full_%j.txt          # Arquivo de saída com o ID do job
#SBATCH --ntasks=1                             # Número de tarefas (apenas 1 processo)
#SBATCH --cpus-per-task=1                      # Número de CPUs por tarefa
#SBATCH --mem=4096MB                           # Memória máxima alocada
#SBATCH --time=03:00:00                        # Tempo máximo de execução (3 horas)
#SBATCH --partition=espec                      # Fila do cluster a ser utilizada

EXECUTABLE="./knapsack"
HEURISTIC_EXEC="./knapsack_heuristic"
INPUT_FILES=("entrada4.txt")

# Função para executar a busca exaustiva
function run_exaustiva {
    local input_file=$1
    echo "[$(date)] Executando busca exaustiva com entrada: $input_file"
    # Usando o comando time embutido do shell
    time $EXECUTABLE $input_file
    echo -e "\n[$(date)] Execução exaustiva com entrada $input_file finalizada"
}

# Função para executar as heurísticas

# Executar a busca exaustiva para cada entrada
for input_file in "${INPUT_FILES[@]}"; do
    run_exaustiva $input_file
done

echo "[$(date)] Job knapsack finalizado"
