#include <mpi.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <chrono>

// Função para contar ocorrências de "AUG" na sequência de RNA
int count_protein_starts(const std::string &rna_sequence) {
    int count = 0;
    for (size_t i = 0; i + 2 < rna_sequence.size(); i += 3) {
        if (rna_sequence.substr(i, 3) == "AUG") {
            count++;
        }
    }
    return count;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank == 0) {
        std::cout << "Contagem de proteínas inicializadas (códons AUG) usando MPI e OpenMP:" << std::endl;
    }

    // Iniciar cronômetro
    auto start_time = std::chrono::high_resolution_clock::now();

    // Caminho para a pasta de saída dos arquivos RNA do exercício 2
    std::string base_path = "../ex2/output_mpi/";

    // Contagem total de proteínas inicializadas
    int total_protein_starts = 0;

    // Processar cada arquivo de transcrição de RNA (chr1 a chr22)
    for (int file_num = 1; file_num <= 22; ++file_num) {
        // Definir o nome do arquivo de entrada
        std::string filename = base_path + "transcribed_chr" + std::to_string(file_num) + "_upper.subst.fa";

        // Ler a sequência de RNA do arquivo
        std::ifstream file(filename);
        std::string line, rna_sequence;

        if (file.is_open()) {
            // Ignorar a linha de cabeçalho
            std::getline(file, line);

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

        // Contar "AUG" na porção local da sequência
        int local_count = count_protein_starts(rna_sequence.substr(start, end - start));

        // Reduzir e somar as contagens locais no processo mestre
        int global_count = 0;
        MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        // Somar o total para o arquivo atual no processo mestre
        if (world_rank == 0) {
            total_protein_starts += global_count;
            std::cout << "Arquivo " << filename << " - Contagem de proteínas inicializadas: " << global_count << std::endl;
        }
    }

    // Exibir o total de proteínas inicializadas após processar todos os arquivos e o tempo de execução
    if (world_rank == 0) {
        std::cout << "Contagem total de proteínas inicializadas em todos os arquivos: " << total_protein_starts << std::endl;

        // Finalizar cronômetro e calcular a duração
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end_time - start_time;
        std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
