#include <mpi.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <sys/stat.h>

// Mapeamento de códons para aminoácidos representados por números
std::unordered_map<std::string, int> codon_to_amino = {
    {"CCA", 1}, {"CCG", 1}, {"CCU", 1}, {"CCC", 1},  // Prolina
    {"UCU", 2}, {"UCA", 2}, {"UCG", 2}, {"UCC", 2},  // Serina
    {"CAG", 3}, {"CAA", 3},                          // Glutamina
    {"ACA", 4}, {"ACC", 4}, {"ACU", 4}, {"ACG", 4},  // Treonina
    {"AUG", 5},                                      // Metionina (Início)
    {"UGA", -1}, {"UAA", -1}, {"UAG", -1},           // Códons STOP
    {"UGC", 6}, {"UGU", 6},                          // Cisteína
    {"GUG", 7}, {"GUA", 7}, {"GUC", 7}, {"GUU", 7}   // Valina
};

// Função para traduzir proteínas dentro de uma subsequência de RNA
std::vector<std::vector<int>> translate_proteins_in_sequence(const std::string &rna_sequence) {
    std::vector<std::vector<int>> all_proteins;
    std::vector<int> current_protein;
    bool translating = false;

    for (size_t i = 0; i + 2 < rna_sequence.size(); i += 3) {
        std::string codon = rna_sequence.substr(i, 3);

        if (codon == "AUG" && !translating) {  // Início de uma nova proteína
            translating = true;
            current_protein.push_back(codon_to_amino[codon]);
        } else if (translating) {
            if (codon_to_amino[codon] == -1) {  // Códon de parada
                translating = false;
                all_proteins.push_back(current_protein);  // Salva a proteína atual
                current_protein.clear();  // Limpa para a próxima proteína
            } else if (codon_to_amino.count(codon)) {
                current_protein.push_back(codon_to_amino[codon]);
            }
        }
    }

    // Adiciona a última proteína se não terminar em códon de parada
    if (!current_protein.empty()) {
        all_proteins.push_back(current_protein);
    }

    return all_proteins;
}

// Função para salvar proteínas traduzidas em arquivo fasta
void save_proteins_to_fasta(const std::vector<std::vector<int>> &proteins, const std::string &output_filename) {
    std::ofstream output_file(output_filename);
    if (!output_file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de saída: " << output_filename << std::endl;
        return;
    }

    int protein_number = 1;
    for (const auto &protein : proteins) {
        output_file << ">Proteína " << protein_number++ << "\n";
        for (int amino_acid : protein) {
            output_file << amino_acid << " ";
        }
        output_file << "\n";
    }

    output_file.close();
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank == 0) {
        std::cout << "[Exercício 4: Tradução de RNA para Aminoácidos]\n" << std::endl;
    }

    // Criação de diretório de saída
    std::string output_dir = "output_ex4/";
    mkdir(output_dir.c_str(), 0777);

    // Caminho para a pasta de saída dos arquivos RNA
    std::string base_path = "../ex2/output_mpi/";

    for (int file_num = world_rank + 1; file_num <= 22; file_num += world_size) {
        std::string filename = base_path + "transcribed_chr" + std::to_string(file_num) + "_upper.subst.fa";

        // Lê a sequência de RNA do arquivo
        std::ifstream file(filename);
        std::string line, rna_sequence;

        if (file.is_open()) {
            std::getline(file, line);  // Ignora o cabeçalho
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

        // Cronometragem do processo
        auto start_time = std::chrono::high_resolution_clock::now();

        // Traduzir proteínas na sequência de RNA
        std::vector<std::vector<int>> proteins = translate_proteins_in_sequence(rna_sequence);

        // Salva o arquivo de saída com base no processo e arquivo de entrada
        std::string output_filename = output_dir + "proteins_chr" + std::to_string(file_num) + "_rank" + std::to_string(world_rank) + ".fasta";
        save_proteins_to_fasta(proteins, output_filename);

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end_time - start_time;

        std::cout << "Arquivo " << filename << " processado pelo processo " << world_rank 
                  << ". Tempo de execução: " << duration.count() << " segundos." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
