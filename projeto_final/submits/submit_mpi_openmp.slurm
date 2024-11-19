#!/bin/bash
#SBATCH --job-name=mpi_openmp               # Nome do job
#SBATCH --output=../outputs/output_mpi_openmp.txt      # Arquivo de saída
#SBATCH --nodes=2                           # Número de nós
#SBATCH --ntasks=4                          # Número total de tarefas MPI
#SBATCH --time=00:05:00                     # Tempo limite
#SBATCH --mem=12000M                        # Memória total por nó

# Função para executar um grafo várias vezes e calcular o tempo médio
executar_varias_vezes() {
    local arquivo=$1
    local repeticoes=4
    local soma=0

    echo "Executando ${arquivo} ${repeticoes} vezes:"
    for i in $(seq 1 $repeticoes); do
        # Redireciona a saída do programa para capturar o tempo de execução
        tempo=$(mpirun -np 4 ../src/exec/mpi_openmp $arquivo | grep "Tempo de execução (MPI + OpenMP)" | awk -F': ' '{print $2}' | awk '{print $1}')
        soma=$((soma + tempo))
        echo "Execução $i: ${tempo} ms"
    done

    media=$((soma / repeticoes))
    echo "Tempo médio para ${arquivo}: ${media} ms"
    echo -e "\n"
}

# Executa cada grafo
executar_varias_vezes "../input/grafo_10.txt"
executar_varias_vezes "../input/grafo_50.txt"
executar_varias_vezes "../input/grafo_100.txt"

echo "Execução concluída."
