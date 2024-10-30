#include <mpi.h>
#include <iostream>
#include <cstring>  // Necessário para strcpy

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Exibe a indicação apenas uma vez (pelo processo de rank 0)
    if (rank == 0) {
        std::cout << "[Exercício 1] Comunicação entre dois processos" << std::endl;
    }

    // Sincroniza todos os processos para evitar problemas de ordenação no output
    MPI_Barrier(MPI_COMM_WORLD);

    const int msg_size = 5;  // Tamanho ajustado para incluir o caractere nulo '\0'
    char msg[msg_size];

    if (rank == 0) {
        // Processo 0 envia "Olá" e recebe "Oi"
        strcpy(msg, "Olá");
        MPI_Send(msg, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(msg, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo 0 recebeu: " << msg << std::endl;
    } else if (rank == 1) {
        // Processo 1 recebe "Olá" e envia "Oi"
        MPI_Recv(msg, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo 1 recebeu: " << msg << std::endl;

        strcpy(msg, "Oi");
        MPI_Send(msg, 3, MPI_CHAR, 0, 0, MPI_COMM_WORLD);  // Envia apenas "Oi" (3 caracteres)
    }

    MPI_Finalize();
    return 0;
}
