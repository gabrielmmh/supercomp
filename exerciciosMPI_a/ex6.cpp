#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Exibe a indicação do exercício uma vez
    if (rank == 0) {
        std::cout << "[Exercício 6] Alternância de Mensagens" << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);  // Sincronização

    char msg[30] = "Mensagem de teste";

    if (rank == 0) {
        for (int i = 1; i < size; ++i) {
            // Envia mensagem para o processo i
            MPI_Send(msg, 30, MPI_CHAR, i, 0, MPI_COMM_WORLD);

            // Recebe resposta do processo i
            MPI_Recv(msg, 30, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Processo 0 recebeu resposta de " << i << ": " << msg << std::endl;
        }
    } else {
        // Recebe mensagem do processo 0
        MPI_Recv(msg, 30, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Envia resposta para o processo 0
        MPI_Send(msg, 30, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
