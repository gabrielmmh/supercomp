#!/bin/bash
#SBATCH --job-name=convolucao_mixed      # Nome do job
#SBATCH --output=convolucao_mixed_%j.txt # Arquivo de saída com o ID do job
#SBATCH --ntasks=1                       # Número de tarefas
#SBATCH --cpus-per-task=4                # Número máximo de CPUs por tarefa
#SBATCH --mem=4096MB                     # Memória máxima alocada por nó (4 GB)
#SBATCH --time=01:00:00                  # Tempo máximo de execução (1 hora)
#SBATCH --partition=espec                # Fila do cluster a ser utilizada

# Exporta a variável de ambiente para configurar o número de threads OpenMP
export OMP_NUM_THREADS=1

# Função para executar o programa com diferentes configurações
function run_convolucao {
    local cpus=$1
    local mem=$2
    local iterations=$3

    export OMP_NUM_THREADS=$cpus

    echo "[$(date)] Executando convolução com $cpus CPU(s), $mem MB de memória e $iterations iterações"
    srun --ntasks=1 --cpus-per-task=$cpus --mem=${mem}MB ./convolucao $iterations
    echo "[$(date)] Convolução com $iterations iterações ($cpus CPU(s), $mem MB) finalizada"
}

# Executa diferentes combinações de CPU, memória e iterações
# Parte 1: Configuração com 1 CPU
run_convolucao 1 512 30
run_convolucao 1 512 50
run_convolucao 1 512 100

run_convolucao 1 1024 30
run_convolucao 1 1024 50
run_convolucao 1 1024 100

run_convolucao 1 2048 30
run_convolucao 1 2048 50
run_convolucao 1 2048 100

# Parte 2: Configuração com 2 CPUs
run_convolucao 2 512 30
run_convolucao 2 512 50
run_convolucao 2 512 100

run_convolucao 2 1024 30
run_convolucao 2 1024 50
run_convolucao 2 1024 100

run_convolucao 2 2048 30
run_convolucao 2 2048 50
run_convolucao 2 2048 100

# Parte 3: Configuração com 3 CPUs
run_convolucao 3 512 30
run_convolucao 3 512 50
run_convolucao 3 512 100

run_convolucao 3 1024 30
run_convolucao 3 1024 50
run_convolucao 3 1024 100

run_convolucao 3 2048 30
run_convolucao 3 2048 50
run_convolucao 3 2048 100

# Parte 4: Configuração com 4 CPUs
run_convolucao 4 512 30
run_convolucao 4 512 50
run_convolucao 4 512 100

run_convolucao 4 1024 30
run_convolucao 4 1024 50
run_convolucao 4 1024 100

run_convolucao 4 2048 30
run_convolucao 4 2048 50
run_convolucao 4 2048 100

echo "[$(date)] Job convolucao_mixed finalizado"
