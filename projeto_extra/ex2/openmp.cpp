#include <omp.h>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sys/stat.h>
#include <sys/types.h>

void transcribe_dna_to_rna(std::string &sequence) {
    #pragma omp parallel for
    for (size_t i = 0; i < sequence.size(); i++) {
        if (sequence[i] == 'T') {
            sequence[i] = 'U';
        }
    }
}

int main() {
    auto start_time = std::chrono::system_clock::now();
    std::time_t start_time_t = std::chrono::system_clock::to_time_t(start_time);
    std::cout << "Data e Hora de Início: " << std::put_time(std::localtime(&start_time_t), "%Y-%m-%d %H:%M:%S") << std::endl;

    // Criar a pasta de saída "output_openmp" se não existir
    mkdir("output_openmp", 0777);

    auto begin = std::chrono::high_resolution_clock::now();

    // Processar cada arquivo de chr1.subst.fa a chr22.subst.fa
    for (int file_num = 1; file_num <= 22; ++file_num) {
        std::string filename = "../dados/chr" + std::to_string(file_num) + ".subst.fa";
        std::cout << "Processando arquivo: " << filename << std::endl;

        std::ifstream file(filename);
        std::string dna_sequence;

        if (file.is_open()) {
            std::string line;
            std::getline(file, line); // Pular o header
            while (std::getline(file, line)) {
                dna_sequence += line;
            }
            file.close();
        }

        // Transcrição usando OpenMP
        transcribe_dna_to_rna(dna_sequence);

        // Salvar o RNA transcrito na pasta "output_openmp"
        std::ofstream output("output_openmp/transcribed_chr" + std::to_string(file_num) + ".fa");
        output << "> RNA transcribed from DNA\n" << dna_sequence;
        output.close();

        std::cout << "Transcrição concluída para " << filename << " e salva em output_openmp/transcribed_chr" << file_num << ".fa." << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - begin;
    std::cout << "Tempo de execução total: " << duration.count() << " segundos" << std::endl;

    return 0;
}
