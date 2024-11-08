#!/bin/bash
#SBATCH --job-name=ex4_translation
#SBATCH --output=output_ex4_translation_%j.txt  # Saída com ID do job
#SBATCH --nodes=2               # Número de nós
#SBATCH --ntasks=4              # Número total de tarefas MPI
#SBATCH --time=00:05:00         # Tempo limite
#SBATCH --mem=12000M            # Memória total por nó

echo "[Exercício 4: Tradução de RNA para Aminoácidos]"

# Loop para processar os arquivos de transcrição de chr1 a chr22
for i in {1..22}; do
    RNA_FILE="../ex2/output_mpi/transcribed_chr${i}_upper.subst.fa"
    echo "Processando $RNA_FILE"
    mpirun -np 4 ./mpi_openmp "$RNA_FILE"
    echo -e ""
done

echo -e "Execução concluída para todos os arquivos.\n"
