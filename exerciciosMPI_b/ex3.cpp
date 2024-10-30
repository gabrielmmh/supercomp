#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>  // Para rand()
#include <algorithm>  // Para std::max_element
#include <iomanip>  // Para configurar precisão decimal

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 16;
    std::vector<int> array(N);
    int max_value;

    if (rank == 0) {
        std::cout << "[Exercício 3] Normalização de um Array em Paralelo" << std::endl;

        // Inicializa o array com valores aleatórios
        for (int i = 0; i < N; ++i) {
            array[i] = rand() % 100;
        }

        // Exibe os valores iniciais
        std::cout << "Array inicial: ";
        for (int val : array) {
            std::cout << val << " ";
        }
        std::cout << std::endl;

        // Encontra o valor máximo
        max_value = *std::max_element(array.begin(), array.end());
        std::cout << "Valor máximo: " << max_value << std::endl;
    }

    // Envia o valor máximo para todos os processos
    MPI_Bcast(&max_value, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<double> local_array(N / size);
    std::vector<int> local_int_array(N / size);  // Para receber os inteiros na distribuição inicial

    // Distribui o array para os processos
    MPI_Scatter(array.data(), N / size, MPI_INT, local_int_array.data(), N / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Converte e normaliza a parte local do array
    for (size_t i = 0; i < local_int_array.size(); ++i) {
        local_array[i] = static_cast<double>(local_int_array[i]) / max_value;
    }

    // Coleta os arrays normalizados no processo raiz
    std::vector<double> normalized_array(N);
    MPI_Gather(local_array.data(), N / size, MPI_DOUBLE, normalized_array.data(), N / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Exibe o array normalizado com precisão
        std::cout << std::fixed << std::setprecision(2) << "Array normalizado: ";
        for (double val : normalized_array) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
