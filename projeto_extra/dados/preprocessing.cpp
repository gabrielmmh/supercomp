#include <omp.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cctype>  // Para toupper
#include <sys/stat.h>
#include <sys/types.h>

// Função para pré-processar uma linha: filtrar bases e converter para maiúsculas
void preprocess_sequence(const std::string &input_line, std::string &output_sequence) {
    std::string filtered_sequence;

    for (char base : input_line) {
        // Verifica se o caractere é uma base válida (A, C, T, G, U ou suas versões minúsculas)
        if (base == 'A' || base == 'C' || base == 'T' || base == 'G' || base == 'U' ||
            base == 'a' || base == 'c' || base == 't' || base == 'g' || base == 'u') {

            // Converte para maiúsculas e adiciona à sequência filtrada
            filtered_sequence += std::toupper(base);
        }
    }

    // Adiciona a sequência filtrada à output_sequence
    output_sequence += filtered_sequence;
}

int main() {
    // Diretórios de entrada e saída
    std::string input_dir = "dados/raw/";
    std::string output_dir = "dados/preprocessed/";

    // Cria o diretório de saída se não existir
    mkdir(output_dir.c_str(), 0777);

    // Processar arquivos de chr1.subst.fa a chr22.subst.fa
    #pragma omp parallel for
    for (int file_num = 1; file_num <= 22; ++file_num) {
        // Nomes dos arquivos de entrada e saída
        std::string input_filename = input_dir + "chr" + std::to_string(file_num) + ".subst.fa";
        std::string output_filename = output_dir + "chr" + std::to_string(file_num) + "_upper.subst.fa";

        // Abrir o arquivo de entrada para leitura
        std::ifstream input_file(input_filename);
        if (!input_file.is_open()) {
            #pragma omp critical
            {
                std::cerr << "Erro ao abrir o arquivo: " << input_filename << std::endl;
            }
            continue;
        }

        // Abrir o arquivo de saída para escrita
        std::ofstream output_file(output_filename);
        if (!output_file.is_open()) {
            #pragma omp critical
            {
                std::cerr << "Erro ao criar o arquivo: " << output_filename << std::endl;
            }
            input_file.close();
            continue;
        }

        std::string line;
        bool first_line = true;
        std::string processed_sequence;

        // Ler o arquivo de entrada linha por linha
        while (std::getline(input_file, line)) {
            if (first_line) {
                // Escrever a linha de cabeçalho sem alterações
                output_file << line << '\n';
                first_line = false;
                continue;
            }

            // Pré-processar a linha: filtrar e converter para maiúsculas
            preprocess_sequence(line, processed_sequence);
        }

        // Escrever a sequência processada em linhas de 50 caracteres
        size_t sequence_length = processed_sequence.size();
        for (size_t i = 0; i < sequence_length; i += 50) {
            std::string sequence_line = processed_sequence.substr(i, 50);
            output_file << sequence_line << '\n';
        }

        input_file.close();
        output_file.close();

        #pragma omp critical
        {
            std::cout << "Arquivo processado e salvo: " << output_filename << std::endl;
        }
    }

    return 0;
}
