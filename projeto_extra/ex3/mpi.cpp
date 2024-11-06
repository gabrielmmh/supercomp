#include <mpi.h>
#include <fstream>
#include <iostream>
#include <string>

int count_codons(const std::string &rna_sequence, const std::string &codon)
{
    int count = 0;
    for (size_t i = 0; i <= rna_sequence.size() - 3; i += 3)
    {
        if (rna_sequence.substr(i, 3) == codon)
        {
            count++;
        }
    }
    return count;
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    std::string filename = "transcribed_rna.fa"; // Arquivo RNA transcrito
    std::ifstream file(filename);
    std::string rna_sequence;

    if (file.is_open())
    {
        std::string line;
        std::getline(file, line); // Pular o header
        while (std::getline(file, line))
        {
            rna_sequence += line;
        }
        file.close();
    }

    // Dividir a sequência entre os processos
    int sequence_size = rna_sequence.size();
    int chunk_size = (sequence_size / 3) / world_size * 3; // Dividir por múltiplos de 3
    int start = world_rank * chunk_size;
    int end = (world_rank == world_size - 1) ? sequence_size : start + chunk_size;

    std::string local_rna = rna_sequence.substr(start, end - start);

    // Contar o número de códon de início AUG
    int local_count = count_codons(local_rna, "AUG");
    int global_count = 0;

    // Somar contagens locais no processo mestre
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        std::cout << "Total de proteínas inicializadas (AUG encontrados): " << global_count << std::endl;
    }

    MPI_Finalize();
    return 0;
}
