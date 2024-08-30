#!/bin/bash
#SBATCH --job-name=knapsack_exaustiva          # Nome do job
#SBATCH --output=knapsack_exaustiva_%j.txt     # Arquivo de saída com o ID do job
#SBATCH --ntasks=1                             # Número de tarefas (apenas 1 processo)
#SBATCH --cpus-per-task=1                      # Número de CPUs por tarefa
#SBATCH --mem=1024MB                           # Memória máxima alocada
#SBATCH --time=03:00:00                        # Tempo máximo de execução (3 horas)
#SBATCH --partition=espec                      # Fila do cluster a ser utilizada

EXECUTABLE="./knapsack"
INPUT_FILES=("entrada_1.txt" "entrada_2.txt" "entrada_3.txt" "entrada_4.txt")

# Função para executar a busca exaustiva
function run_exaustiva {
    local input_file=$1
    echo "[$(date)] Executando busca exaustiva com entrada: $input_file"
    # Usando o comando time embutido do shell
    time $EXECUTABLE $input_file
    echo -e "\n[$(date)] Execução exaustiva com entrada $input_file finalizada"
}

# Executar a busca exaustiva para cada entrada
for input_file in "${INPUT_FILES[@]}"; do
    run_exaustiva $input_file
done

echo "[$(date)] Job knapsack_exaustiva finalizado"
