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

    const int N = 16;  
    const int target = 50;  
    std::vector<int> array(N);

    if (rank == 0) {
        std::cout << "[Exercício 4] Busca em Vetor Distribuído" << std::endl;
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

    std::vector<int> local_positions;
    #pragma omp parallel for
    for (int i = 0; i < local_size; ++i) {
        if (local_array[i] == target) {
            #pragma omp critical
            local_positions.push_back(i + rank * local_size);
        }
    }

    int local_count = local_positions.size();
    int total_count;
    MPI_Reduce(&local_count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Total de ocorrências do valor " << target << ": " << total_count << std::endl;
    }

    MPI_Finalize();
    return 0;
}
