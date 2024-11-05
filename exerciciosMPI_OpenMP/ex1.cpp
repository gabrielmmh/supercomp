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

    const int rows = 4;  // Defina o número de linhas
    const int cols = 4;  // Defina o número de colunas
    std::vector<int> matrix(rows * cols);  // Matriz linearizada para facilitar o broadcast

    if (rank == 0) {
        std::cout << "[Exercício 1] Cálculo do Quadrado de cada Elemento da Matriz" << std::endl;
        srand(time(0));

        // Inicializa a matriz com valores aleatórios e exibe
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

    // Distribui a matriz para todos os processos
    MPI_Bcast(matrix.data(), rows * cols, MPI_INT, 0, MPI_COMM_WORLD);

    // Paraleliza o cálculo do quadrado de cada elemento
    #pragma omp parallel for
    for (int i = 0; i < rows * cols; ++i) {
        matrix[i] *= matrix[i];
    }

    // Apenas o processo 0 exibe a matriz final
    if (rank == 0) {
        std::cout << "Matriz ao quadrado:" << std::endl;
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
