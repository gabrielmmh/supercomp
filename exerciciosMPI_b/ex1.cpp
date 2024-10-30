#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric>  // Para std::accumulate
#include <cstdlib>  // Para rand()
#include <ctime>    // Para time()

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 16;  // Tamanho total do array
    std::vector<int> array(N);

    if (rank == 0) {
        std::cout << "[Exercício 1] Cálculo Distribuído da Média de um Array" << std::endl;

        // Define uma seed baseada no tempo atual para garantir aleatoriedade
        srand(time(0));  

        // Inicializa o array com valores aleatórios
        std::cout << "Array gerado: ";
        for (int i = 0; i < N; ++i) {
            array[i] = rand() % 100;  // Valores aleatórios entre 0 e 99
            std::cout << array[i] << " ";  // Print dos valores gerados
        }
        std::cout << std::endl;
    }

    // Cada processo receberá N/size elementos
    std::vector<int> local_array(N / size);
    MPI_Scatter(array.data(), N / size, MPI_INT, local_array.data(), N / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcula a média local
    double local_sum = std::accumulate(local_array.begin(), local_array.end(), 0.0);
    double local_mean = local_sum / local_array.size();

    // Coleta as médias locais no processo raiz
    std::vector<double> local_means(size);
    MPI_Gather(&local_mean, 1, MPI_DOUBLE, local_means.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Calcula a média global das médias locais
        double global_mean = std::accumulate(local_means.begin(), local_means.end(), 0.0) / size;
        std::cout << "Média Global: " << global_mean << std::endl;
    }

    MPI_Finalize();
    return 0;
}
