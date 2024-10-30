#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Indicação do exercício impressa uma única vez
    if (rank == 0) {
        std::cout << "[Exercício 5] Somas Paralelas" << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);  // Sincronização

    int local_value = rank + 1;  // Valor exclusivo para cada processo
    int total_sum = 0;

    if (rank == 0) {
        MPI_Send(&local_value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }

    if (rank > 0) {
        int received_value;
        MPI_Recv(&received_value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        local_value += received_value;

        if (rank < size - 1) {
            MPI_Send(&local_value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        } else {
            total_sum = local_value;
            std::cout << "Soma total: " << total_sum << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
