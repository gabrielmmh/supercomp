#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric>  // Para std::accumulate
#include <cmath>    // Para std::sqrt
#include <cstdlib>  // Para rand()

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 16;
    std::vector<int> array(N);

    if (rank == 0) {
        std::cout << "[Exercício 4] Cálculo Distribuído de Desvio Padrão" << std::endl;

        // Inicializa o array com valores aleatórios
        for (int i = 0; i < N; ++i) {
            array[i] = rand() % 100;
        }

        // Exibe o array inicial
        std::cout << "Array inicial: ";
        for (int val : array) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // Distribui o array entre os processos
    std::vector<int> local_array(N / size);
    MPI_Scatter(array.data(), N / size, MPI_INT, local_array.data(), N / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcula a soma local
    double local_sum = std::accumulate(local_array.begin(), local_array.end(), 0.0);

    // Coleta as somas locais para calcular a média global
    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double global_mean = 0.0;
    if (rank == 0) {
        global_mean = global_sum / N;
        std::cout << "Média Global: " << global_mean << std::endl;
    }

    // Envia a média global para todos os processos
    MPI_Bcast(&global_mean, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Calcula a variação local com base na média global
    double local_variance = 0.0;
    for (int val : local_array) {
        local_variance += (val - global_mean) * (val - global_mean);
    }

    // Coleta as variações locais para calcular a variância global
    double global_variance;
    MPI_Reduce(&local_variance, &global_variance, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        global_variance /= N;
        double stddev = std::sqrt(global_variance);
        std::cout << "Desvio Padrão Global: " << stddev << std::endl;
    }

    MPI_Finalize();
    return 0;
}
