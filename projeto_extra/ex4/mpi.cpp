#include <mpi.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Tabela de códons para aminoácidos representados por números
std::unordered_map<std::string, int> codon_to_aminoacid = {
    {"AUG", 1}, // Início (Metionina)
    {"UUU", 2},
    {"UUC", 2}, // Fenilalanina
    {"UUA", 3},
    {"UUG", 3}, // Leucina
    {"UAA", -1},
    {"UAG", -1},
    {"UGA", -1} // Códons de parada
    // Adicione outros códons conforme necessário
};

std::vector<int> translate_rna_to_protein(const std::string &rna_sequence)
{
    std::vector<int> protein_sequence;
    for (size_t i = 0; i <= rna_sequence.size() - 3; i += 3)
    {
        std::string codon = rna_sequence.substr(i, 3);
        if (codon_to_aminoacid.find(codon) != codon_to_aminoacid.end())
        {
            int amino_acid = codon_to_aminoacid[codon];
            if (amino_acid == -1)
                break; // Parar na síntese se encontrar códon de parada
            protein_sequence.push_back(amino_acid);
        }
    }
    return protein_sequence;
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
    int chunk_size = (sequence_size / 3) / world_size * 3;
    int start = world_rank * chunk_size;
    int end = (world_rank == world_size - 1) ? sequence_size : start + chunk_size;

    std::string local_rna = rna_sequence.substr(start, end - start);

    // Traduzir a sequência local para aminoácidos
    std::vector<int> local_protein_sequence = translate_rna_to_protein(local_rna);

    if (world_rank == 0)
    {
        // Processo mestre reúne as sequências dos outros processos
        std::vector<int> global_protein_sequence = local_protein_sequence;

        for (int i = 1; i < world_size; i++)
        {
            int local_size;
            MPI_Recv(&local_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            std::vector<int> temp(local_size);
            MPI_Recv(temp.data(), local_size, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            global_protein_sequence.insert(global_protein_sequence.end(), temp.begin(), temp.end());
        }

        // Exibir a sequência traduzida
        std::cout << "Sequência de proteína (aminoácidos): ";
        for (int amino_acid : global_protein_sequence)
        {
            std::cout << amino_acid << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        int local_size = local_protein_sequence.size();
        MPI_Send(&local_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(local_protein_sequence.data(), local_size, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
