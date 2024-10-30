#include <mpi.h>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Indicação do exercício impressa uma única vez
    if (rank == 0) {
        std::cout << "[Exercício 4] Comunicação Múltipla" << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);  // Sincronização inicial

    const int BUFFER_SIZE = 100;  // Tamanho suficiente para mensagens maiores
    char recv_msg[BUFFER_SIZE] = {0};  // Inicializa o buffer para evitar lixo

    if (rank == 0) {
        // Processo 0 envia uma mensagem personalizada para cada processo
        for (int i = 1; i < size; ++i) {
            std::string msg = "Mensagem para o processo " + std::to_string(i);
            MPI_Send(msg.c_str(), msg.size() + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    } else {
        // Demais processos recebem suas mensagens do processo 0
        MPI_Recv(recv_msg, BUFFER_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu: " << recv_msg << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);  // Sincronização final

    MPI_Finalize();
    return 0;
}
