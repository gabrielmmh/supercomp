#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int num_iteracoes;

    if (rank == 0) {
        std::cout << "[Exercício 2] Distribuição de Configuração para Cálculos Paralelos" << std::endl;
        num_iteracoes = 100;  // Configuração inicial
    }

    // Envia a configuração para todos os processos
    MPI_Bcast(&num_iteracoes, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo executa o cálculo com base na configuração recebida
    std::cout << "Processo " << rank << " executando " << num_iteracoes << " iterações." << std::endl;

    MPI_Finalize();
    return 0;
}
