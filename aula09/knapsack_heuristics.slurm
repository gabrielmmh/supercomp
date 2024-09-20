#!/bin/bash
#SBATCH --job-name=knapsack_heuristics            # Nome do job
#SBATCH --output=return/knapsack_heuristics_%j.txt # Arquivo de saída com o ID do job
#SBATCH --ntasks=1                                # Número de tarefas (apenas 1 processo)
#SBATCH --cpus-per-task=1                         # Número de CPUs por tarefa
#SBATCH --mem=1024MB                              # Memória máxima alocada
#SBATCH --time=00:30:00                           # Tempo máximo de execução (30 minutos)
#SBATCH --partition=espec                         # Fila do cluster a ser utilizada

# Diretório de entradas
INPUT_FILES=("entradas/entrada1.txt" "entradas/entrada2.txt" "entradas/entrada3.txt")

# Função para executar a heurística mais caro primeiro
function run_mais_caro {
    local input_file=$1
    echo "[$(date)] Executando heurística Mais Caro Primeiro com entrada: $input_file"
    ./mais_caro $input_file
    echo "[$(date)] Execução da heurística Mais Caro Primeiro com entrada $input_file finalizada"
}

# Função para executar a heurística mais leve primeiro
function run_mais_leve {
    local input_file=$1
    echo "[$(date)] Executando heurística Mais Leve Primeiro com entrada: $input_file"
    ./mais_leve $input_file
    echo "[$(date)] Execução da heurística Mais Leve Primeiro com entrada $input_file finalizada"
}

# Executar ambas as heurísticas para cada arquivo de entrada
for input_file in "${INPUT_FILES[@]}"; do
    run_mais_caro $input_file
    run_mais_leve $input_file
done

echo "[$(date)] Job knapsack_heuristics finalizado"
