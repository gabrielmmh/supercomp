#include <mpi.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sys/stat.h>
#include <sys/types.h>

void transcribe_dna_to_rna(std::string &dna_sequence) {
    for (char &base : dna_sequence) {
        if (base == 'T') {
            base = 'U';
        }
    }
}

void write_rna_sequence(const std::string &rna_sequence, const std::string &filename) {
    std::ofstream output(filename);
    output << "> RNA transcribed from DNA\n";

    // Escrever a sequência em linhas de 50 caracteres
    for (size_t i = 0; i < rna_sequence.size(); i += 50) {
        output << rna_sequence.substr(i, 50) << '\n';
    }
    output.close();
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank == 0) {
        auto start_time = std::chrono::system_clock::now();
        std::time_t start_time_t = std::chrono::system_clock::to_time_t(start_time);
        std::cout << "Data e Hora de Início: " << std::put_time(std::localtime(&start_time_t), "%Y-%m-%d %H:%M:%S") << std::endl;

        // Criar a pasta de saída "output_mpi" se não existir
        mkdir("output_mpi", 0777);
    }

    auto begin = std::chrono::high_resolution_clock::now();

    // Processar cada arquivo de chr1.subst.fa a chr22.subst.fa
    for (int file_num = 1; file_num <= 22; ++file_num) {
        std::string filename = "../dados/chr" + std::to_string(file_num) + ".subst.fa";
        if (world_rank == 0) {
            std::cout << "Processando arquivo: " << filename << std::endl;
        }

        std::ifstream file(filename);
        std::string dna_sequence;

        if (file.is_open()) {
            std::string line;
            if (world_rank == 0) {
                std::getline(file, line); // Pular o header
            }

            while (std::getline(file, line)) {
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

        // Processo mestre reúne partes transcritas dos processos
        std::string rna_sequence;
        if (world_rank == 0) {
            rna_sequence = local_dna;

            for (int i = 1; i < world_size; i++) {
                int recv_size;
                MPI_Recv(&recv_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                std::string temp(recv_size, '\0');
                MPI_Recv(&temp[0], recv_size, MPI_CHAR, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                rna_sequence += temp;
            }

            // Salvar o RNA convertido na pasta "output_mpi" com formatação de 50 caracteres por linha
            write_rna_sequence(rna_sequence, "output_mpi/transcribed_chr" + std::to_string(file_num) + ".fa");
        } else {
            int local_size = local_dna.size();
            MPI_Send(&local_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(local_dna.c_str(), local_dna.size(), MPI_CHAR, 0, 1, MPI_COMM_WORLD);
        }
    }

    if (world_rank == 0) {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - begin;
        std::cout << "Tempo de execução total: " << duration.count() << " segundos" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
