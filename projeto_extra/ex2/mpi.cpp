#include <mpi.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

void transcribe_dna_to_rna(std::string &dna_sequence)
{
    for (char &base : dna_sequence)
    {
        if (base == 'T')
        {
            base = 'U';
        }
    }
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    std::string filename = "chrX.subst.fa"; // Substituir pelo nome correto
    std::ifstream file(filename);
    std::string dna_sequence;

    if (file.is_open())
    {
        std::string line;
        if (world_rank == 0)
        {
            std::getline(file, line); // Pular o header
        }

        while (std::getline(file, line))
        {
            dna_sequence += line;
        }
        file.close();
    }

    // Dividir a sequência entre os processos
    int sequence_size = dna_sequence.size();
    int chunk_size = sequence_size / world_size;
    int start = world_rank * chunk_size;
    int end = (world_rank == world_size - 1) ? sequence_size : start + chunk_size;

    std::string local_dna = dna_sequence.substr(start, end - start);
    transcribe_dna_to_rna(local_dna);

    // Reunir partes convertidas no processo mestre
    if (world_rank == 0)
    {
        std::string rna_sequence = local_dna;

        for (int i = 1; i < world_size; i++)
        {
            int recv_size = (i == world_size - 1) ? sequence_size - i * chunk_size : chunk_size;
            std::string temp(recv_size, '\0');
            MPI_Recv(&temp[0], recv_size, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            rna_sequence += temp;
        }

        // Salvar o RNA convertido
        std::ofstream output("transcribed_rna.fa");
        output << "> RNA transcribed from DNA\n"
               << rna_sequence;
        output.close();
    }
    else
    {
        MPI_Send(local_dna.c_str(), local_dna.size(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
