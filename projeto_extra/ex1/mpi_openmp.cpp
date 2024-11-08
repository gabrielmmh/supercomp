#include <mpi.h>
#include <omp.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <ctime>
#include <iomanip>

int main(int argc, char **argv) {
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

    // Contador local de bases para o processo
    std::unordered_map<char, int> local_counts = {{'A', 0}, {'T', 0}, {'C', 0}, {'G', 0}};

    // Processar apenas a parte dos arquivos destinada a este processo
    for (int file_num = world_rank + 1; file_num <= 22; file_num += world_size) {
        // Nome do arquivo atual
        std::string filename = "../dados/preprocessed/chr" + std::to_string(file_num) + "_upper.subst.fa";
        std::cout << "Processando arquivo: " << filename << " pelo processo " << world_rank << std::endl;

        std::ifstream file(filename);
        std::string sequence;

        if (file.is_open()) {
            std::string line;
            std::getline(file, line); // Pular o header
            while (std::getline(file, line)) {
                sequence += line; // Carregar a sequência completa
            }
            file.close();
        } else {
            std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
            continue;
        }

        // Paralelizar a contagem de bases usando OpenMP
        #pragma omp parallel
        {
            std::unordered_map<char, int> thread_counts = {{'A', 0}, {'T', 0}, {'C', 0}, {'G', 0}};

            #pragma omp for
            for (size_t i = 0; i < sequence.size(); i++) {
                char base = sequence[i];
                if (base == 'A' || base == 'T' || base == 'C' || base == 'G') {
                    thread_counts[base]++;
                }
            }

            // Somar contagens locais de cada thread no contador local do processo
            #pragma omp critical
            for (const auto &count : thread_counts) {
                local_counts[count.first] += count.second;
            }
        }
    }

    // Reduzir as contagens locais para o contador global no processo mestre
    std::unordered_map<char, int> global_counts = {{'A', 0}, {'T', 0}, {'C', 0}, {'G', 0}};
    MPI_Reduce(&local_counts['A'], &global_counts['A'], 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_counts['T'], &global_counts['T'], 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_counts['C'], &global_counts['C'], 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_counts['G'], &global_counts['G'], 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

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
