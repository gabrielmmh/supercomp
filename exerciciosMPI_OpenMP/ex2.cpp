#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int rows = 4;  
    const int cols = 4;  
    std::vector<int> matrix(rows * cols);

    if (rank == 0) {
        std::cout << "[Exercício 2] Cálculo do Quadrado de cada Elemento da Matriz (MPI)" << std::endl;
        srand(time(0));
        for (int i = 0; i < rows * cols; ++i) {
            matrix[i] = rand() % 10;
        }
        std::cout << "Matriz original:" << std::endl;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                std::cout << matrix[i * cols + j] << " ";
            }
            std::cout << std::endl;
        }
    }

    int local_size = (rows * cols) / size;
    std::vector<int> local_matrix(local_size);
    MPI_Scatter(matrix.data(), local_size, MPI_INT, local_matrix.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);

    #pragma omp parallel for
    for (int i = 0; i < local_size; ++i) {
        local_matrix[i] *= local_matrix[i];
    }

    MPI_Gather(local_matrix.data(), local_size, MPI_INT, matrix.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Matriz ao quadrado (MPI):" << std::endl;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                std::cout << matrix[i * cols + j] << " ";
            }
            std::cout << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
