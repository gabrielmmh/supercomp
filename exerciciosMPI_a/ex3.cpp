#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Verificação de número mínimo de processos
    if (size < 2) {
        if (rank == 0) {
            std::cerr << "Erro: O número de processos deve ser pelo menos 2." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    // Indicação do exercício (impressa uma vez)
    if (rank == 0) {
        std::cout << "[Exercício 3] Difusão Linear" << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);  // Sincroniza todos os processos

    const char* msg = "Mensagem de difusão";
    char recv_msg[30] = {0};  // Inicializa para evitar lixo na memória

    if (rank == 0) {
        std::cout << "Processo 0 enviando mensagem para processo 1" << std::endl;
        MPI_Send(msg, 30, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    } else {
        // Recebe do processo anterior
        MPI_Recv(recv_msg, 30, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu: " << recv_msg << std::endl;

        // Envia para o próximo processo (se não for o último)
        if (rank < size - 1) {
            std::cout << "Processo " << rank << " enviando mensagem para processo " << rank + 1 << std::endl;
            MPI_Send(recv_msg, 30, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);  // Sincroniza antes de finalizar
    std::cout << "Processo " << rank << " finalizando" << std::endl;

    MPI_Finalize();
    return 0;
}
