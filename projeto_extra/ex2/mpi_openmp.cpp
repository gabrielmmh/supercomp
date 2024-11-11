#include <mpi.h>
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
        switch (sequence[i]) {
            case 'A':  // Adenina no DNA se torna Uracila no RNA
                sequence[i] = 'U';
                break;
            case 'T':  // Timina no DNA se torna Adenina no RNA
                sequence[i] = 'A';
                break;
            case 'C':  // Citosina no DNA se torna Guanina no RNA
                sequence[i] = 'G';
                break;
            case 'G':  // Guanina no DNA se torna Citosina no RNA
                sequence[i] = 'C';
                break;
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

    if (argc < 2) {
        if (world_rank == 0) {
            std::cerr << "Uso: mpirun -np <num_processos> ./mpi_programa <caminho_arquivo_dna>" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    std::string filename = argv[1];  // Nome do arquivo fornecido como argumento
    if (world_rank == 0) {
        auto start_time = std::chrono::system_clock::now();
        std::time_t start_time_t = std::chrono::system_clock::to_time_t(start_time);
        std::cout << "Data e Hora de Início: " << std::put_time(std::localtime(&start_time_t), "%Y-%m-%d %H:%M:%S") << std::endl;
        std::cout << "Processando arquivo: " << filename << " pelo processo mestre." << std::endl;

        // Criar a pasta de saída "output_mpi" se não existir
        mkdir("output_mpi", 0777);
    }

    auto begin = std::chrono::high_resolution_clock::now();

    std::ifstream file(filename);
    std::string dna_sequence;

    if (file.is_open()) {
        std::string line;
        std::getline(file, line); // Pular o header
        while (std::getline(file, line)) {
            dna_sequence += line;
        }
        file.close();
    } else {
        if (world_rank == 0) {
            std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    // Dividir a sequência entre os processos MPI
    int sequence_size = dna_sequence.size();
    int chunk_size = sequence_size / world_size;
    int remainder = sequence_size % world_size;

    // Ajustar para que o último processo pegue a parte restante, se houver
    int start = world_rank * chunk_size + std::min(world_rank, remainder);
    int end = start + chunk_size + (world_rank < remainder ? 1 : 0);

    // Obter a subsequência de DNA deste processo
    std::string local_dna = dna_sequence.substr(start, end - start);
    transcribe_dna_to_rna(local_dna);

    // Reunir as partes transcritas no processo mestre
    if (world_rank == 0) {
        std::string rna_sequence(sequence_size, '\0'); // Alocar o tamanho total necessário

        // Copiar a parte transcrita do processo mestre
        rna_sequence.replace(start, local_dna.size(), local_dna);

        // Receber as partes dos outros processos
        for (int i = 1; i < world_size; i++) {
            int recv_size;
            MPI_Recv(&recv_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            std::string temp(recv_size, '\0');
            MPI_Recv(&temp[0], recv_size, MPI_CHAR, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Local de inserção calculado para cada processo
            int recv_start = i * chunk_size + std::min(i, remainder);
            rna_sequence.replace(recv_start, temp.size(), temp);
        }

        // Salvar o RNA convertido na pasta "output_mpi" com formatação de 50 caracteres por linha
        std::string output_filename = "output_mpi/transcribed_" + filename.substr(filename.find_last_of("/") + 1);
        write_rna_sequence(rna_sequence, output_filename);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - begin;
        std::cout << "Tempo de execução total: " << duration.count() << " segundos" << std::endl;
    } else {
        int local_size = local_dna.size();
        MPI_Send(&local_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(local_dna.c_str(), local_dna.size(), MPI_CHAR, 0, 1, MPI_COMM_WORLD);

        std::cout << "Processo " << world_rank << " enviou tamanho " << local_size << " e dados ao processo mestre." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
