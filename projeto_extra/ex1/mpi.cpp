#include <mpi.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <ctime>
#include <iomanip>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank == 0) {
        auto start_time = std::chrono::system_clock::now();
        std::time_t start_time_t = std::chrono::system_clock::to_time_t(start_time);
        std::cout << "Data e Hora de Início: " << std::put_time(std::localtime(&start_time_t), "%Y-%m-%d %H:%M:%S") << std::endl;
    }

    auto begin = std::chrono::high_resolution_clock::now();

    // Contador global de bases, inicializado apenas no processo mestre
    std::unordered_map<char, int> global_counts = {{'A', 0}, {'T', 0}, {'C', 0}, {'G', 0}};

    // Processar cada arquivo de chr1.subst.fa a chr22.subst.fa
    for (int file_num = 1; file_num <= 22; ++file_num) {
        // Nome do arquivo atual
        std::string filename = "../dados/chr" + std::to_string(file_num) + ".subst.fa";
        if (world_rank == 0) {
            std::cout << "Processando arquivo: " << filename << std::endl;
        }

        // Contador local para o arquivo atual
        std::unordered_map<char, int> local_counts = {{'A', 0}, {'T', 0}, {'C', 0}, {'G', 0}};
        std::ifstream file(filename);

        if (file.is_open()) {
            std::string line;
            if (world_rank == 0) {
                std::getline(file, line); // Pular o header
            }

            std::vector<char> local_data;
            char base;
            int base_count = 0;

            while (file.get(base)) {
                if (base == 'A' || base == 'T' || base == 'C' || base == 'G') {
                    if (base_count % world_size == world_rank) {
                        local_data.push_back(base);
                    }
                    base_count++;
                }
            }

            // Contar bases no processo local
            for (char b : local_data) {
                local_counts[b]++;
            }
        }
        file.close();

        // Variáveis temporárias para armazenar contagens globais do arquivo atual
        int global_count_A = 0, global_count_T = 0, global_count_C = 0, global_count_G = 0;

        // Reduzir as contagens locais para contagens globais do arquivo atual
        MPI_Reduce(&local_counts['A'], &global_count_A, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&local_counts['T'], &global_count_T, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&local_counts['C'], &global_count_C, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&local_counts['G'], &global_count_G, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        // Processo mestre acumula as contagens de cada arquivo no contador global
        if (world_rank == 0) {
            global_counts['A'] += global_count_A;
            global_counts['T'] += global_count_T;
            global_counts['C'] += global_count_C;
            global_counts['G'] += global_count_G;
        }
    }

    // Processo mestre imprime a contagem total após processar todos os arquivos
    if (world_rank == 0) {
        std::cout << "Contagem total de bases:" << std::endl;
        for (const auto &count : global_counts) {
            std::cout << count.first << ": " << count.second << std::endl;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - begin;
        std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
