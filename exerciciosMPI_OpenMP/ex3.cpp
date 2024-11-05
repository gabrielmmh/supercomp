#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 16;  
    std::vector<int> array(N);

    if (rank == 0) {
        std::cout << "[Exercício 3] Cálculo Distribuído da Média de um Array" << std::endl;
        srand(time(0));
        for (int& val : array) {
            val = rand() % 100;
        }
        std::cout << "Array gerado: ";
        for (int val : array) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    int local_size = N / size;
    std::vector<int> local_array(local_size);
    MPI_Scatter(array.data(), local_size, MPI_INT, local_array.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);

    int local_sum = 0;
    #pragma omp parallel for reduction(+:local_sum)
    for (int val : local_array) {
        local_sum += val;
    }

    int global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double global_mean = static_cast<double>(global_sum) / N;
        std::cout << "Média Global: " << global_mean << std::endl;
    }

    MPI_Finalize();
    return 0;
}
