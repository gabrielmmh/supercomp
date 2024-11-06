#include <omp.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <ctime>
#include <iomanip>

int main()
{
    auto start_time = std::chrono::system_clock::now();
    std::time_t start_time_t = std::chrono::system_clock::to_time_t(start_time);
    std::cout << "Data e Hora de Início: " << std::put_time(std::localtime(&start_time_t), "%Y-%m-%d %H:%M:%S") << std::endl;

    auto begin = std::chrono::high_resolution_clock::now();

    std::unordered_map<char, int> global_counts = {{'A', 0}, {'T', 0}, {'C', 0}, {'G', 0}};

    // Processar cada arquivo de chr1.subst.fa a chr22.subst.fa
    for (int file_num = 1; file_num <= 22; ++file_num) {
        // Nome do arquivo atual
        std::string filename = "../dados/chr" + std::to_string(file_num) + ".subst.fa";
        std::cout << "Processando arquivo: " << filename << std::endl;

        std::ifstream file(filename);
        std::string sequence;

        if (file.is_open()) {
            std::string line;
            std::getline(file, line); // Pular o header
            while (std::getline(file, line)) {
                sequence += line;
            }
            file.close();
        }

        std::unordered_map<char, int> local_counts = {{'A', 0}, {'T', 0}, {'C', 0}, {'G', 0}};

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

            #pragma omp critical
            for (const auto &count : thread_counts) {
                local_counts[count.first] += count.second;
            }
        }

        for (const auto &count : local_counts) {
            global_counts[count.first] += count.second;
        }
    }

    std::cout << "Contagem total de bases:" << std::endl;
    for (const auto &count : global_counts) {
        std::cout << count.first << ": " << count.second << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - begin;
    std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;

    return 0;
}
