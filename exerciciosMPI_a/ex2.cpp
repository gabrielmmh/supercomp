#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Indicação do exercício impressa apenas uma vez (por rank 0)
    if (rank == 0) {
        std::cout << "[Exercício 2] Anel de processos" << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);  // Sincroniza antes de começar o anel

    const char* msg = "Mensagem do anel";
    char recv_msg[20];

    // Cada processo envia uma mensagem para o próximo no anel
    MPI_Send(msg, 20, MPI_CHAR, (rank + 1) % size, 0, MPI_COMM_WORLD);

    // Cada processo recebe uma mensagem do anterior
    MPI_Recv(recv_msg, 20, MPI_CHAR, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Imprime as mensagens na ordem correta
    for (int i = 0; i < size; i++) {
        MPI_Barrier(MPI_COMM_WORLD);  // Sincroniza antes de cada impressão
        if (rank == i) {
            std::cout << "Processo " << rank << " recebeu: " << recv_msg << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
