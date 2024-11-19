#!/bin/bash
#SBATCH --job-name=exhaustive               # Nome do job
#SBATCH --output=../outputs/output_exhaustive.txt      # Arquivo de saída
#SBATCH --nodes=1                           # Número de nós (apenas 1 necessário para execução sequencial)
#SBATCH --ntasks=1                          # Número total de tarefas
#SBATCH --cpus-per-task=1                   # Apenas 1 CPU necessária
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
        tempo=$(../src/exec/exhaustive $arquivo | grep "Tempo de execução" | awk '{print $4}')
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
