#include <mpi.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <chrono>
#include <vector>

// Mapeamento de códons para aminoácidos representados por números
std::unordered_map<std::string, int> codon_to_amino = {
    {"CCA", 1}, {"CCG", 1}, {"CCU", 1}, {"CCC", 1},  // Prolina
    {"UCU", 2}, {"UCA", 2}, {"UCG", 2}, {"UCC", 2},  // Serina
    {"CAG", 3}, {"CAA", 3},                          // Glutamina
    {"ACA", 4}, {"ACC", 4}, {"ACU", 4}, {"ACG", 4},  // Treonina
    {"AUG", 5},                                      // Metionina (Início)
    {"UGA", -1},                                     // Códon STOP
    {"UGC", 6}, {"UGU", 6},                          // Cisteína
    {"GUG", 7}, {"GUA", 7}, {"GUC", 7}, {"GUU", 7}   // Valina
};

// Função para traduzir uma sequência de RNA em uma sequência de aminoácidos (representados por números)
std::vector<int> translate_rna_to_protein_sequence(const std::string &rna_sequence) {
    bool translating = false;
    std::vector<int> protein_sequence;

    for (size_t i = 0; i + 2 < rna_sequence.size(); i += 3) {
        std::string codon = rna_sequence.substr(i, 3);

        if (codon == "AUG") {  // Início de uma nova proteína
            translating = true;
            protein_sequence.push_back(codon_to_amino[codon]); // Adiciona o códon de início
        } else if (translating) {
            if (codon == "UGA") {  // Códon de parada
                translating = false;
                break;  // Fim da proteína atual
            } else if (codon_to_amino.count(codon)) {
                protein_sequence.push_back(codon_to_amino[codon]);
            }
        }
    }

    return protein_sequence;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank == 0) {
        std::cout << "[Exercício 4: Tradução de RNA para Aminoácidos]" << std::endl;
    }

    // Caminho para a pasta de saída dos arquivos RNA
    std::string base_path = "../ex2/output_mpi/";

    for (int file_num = 1; file_num <= 22; ++file_num) {
        // Definir o nome do arquivo de entrada
        std::string filename = base_path + "transcribed_chr" + std::to_string(file_num) + "_upper.subst.fa";

        // Ler a sequência de RNA do arquivo
        std::ifstream file(filename);
        std::string line, rna_sequence;

        if (file.is_open()) {
            std::getline(file, line);  // Ignorar o cabeçalho

            // Carregar a sequência de RNA
            while (std::getline(file, line)) {
                rna_sequence += line;
            }
            file.close();
        } else {
            if (world_rank == 0) {
                std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
            }
            continue;
        }

        // Dividir a sequência entre os processos
        int sequence_size = rna_sequence.size();
        int chunk_size = sequence_size / world_size;
        int start = world_rank * chunk_size;
        int end = (world_rank == world_size - 1) ? sequence_size : start + chunk_size;

        // Começar a contagem do tempo
        auto start_time = std::chrono::high_resolution_clock::now();

        // Traduzir a porção local da sequência
        std::vector<int> local_protein_sequence = translate_rna_to_protein_sequence(rna_sequence.substr(start, end - start));
        int local_protein_count = local_protein_sequence.size();

        // Preparar para o MPI_Gatherv
        std::vector<int> recv_counts(world_size);
        MPI_Gather(&local_protein_count, 1, MPI_INT, recv_counts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Calcular os deslocamentos para o MPI_Gatherv
        std::vector<int> displs(world_size, 0);
        int total_size = 0;
        if (world_rank == 0) {
            for (int i = 0; i < world_size; ++i) {
                displs[i] = total_size;
                total_size += recv_counts[i];
            }
        }

        // Buffer de recebimento para o processo mestre
        std::vector<int> total_protein_sequence;
        if (world_rank == 0) {
            total_protein_sequence.resize(total_size);
        }

        // Reunir as sequências de proteínas usando MPI_Gatherv
        MPI_Gatherv(local_protein_sequence.data(), local_protein_count, MPI_INT,
                    total_protein_sequence.data(), recv_counts.data(), displs.data(), MPI_INT,
                    0, MPI_COMM_WORLD);

        // Finalizar a contagem do tempo
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end_time - start_time;

        // No processo mestre, exibir o tempo de execução e a sequência traduzida
        if (world_rank == 0) {
            std::cout << "Tradução completa para " << filename << ". Tempo de execução: " << duration.count() << " segundos." << std::endl;
            std::cout << "Número total de aminoácidos na proteína sintetizada: " << total_protein_sequence.size() << std::endl;
            std::cout << "Sequência de aminoácidos (representada por números): ";
            for (int amino_acid : total_protein_sequence) {
                std::cout << amino_acid << " ";
            }
            std::cout << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
