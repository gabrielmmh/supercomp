#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

void multiply_matrices(const std::vector<std::vector<int>> &A, const std::vector<std::vector<int>> &B, std::vector<std::vector<int>> &result)
{
#pragma omp parallel for
    for (size_t i = 0; i < A.size(); ++i)
    {
        for (size_t j = 0; j < B[0].size(); ++j)
        {
            result[i][j] = 0;
            for (size_t k = 0; k < B.size(); ++k)
            {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main()
{
    std::vector<size_t> sizes = {300, 900, 1300};

    for (size_t N : sizes)
    {
        std::vector<std::vector<int>> A(N, std::vector<int>(N));
        std::vector<std::vector<int>> B(N, std::vector<int>(N));
        std::vector<std::vector<int>> result(N, std::vector<int>(N));

        for (size_t i = 0; i < N; ++i)
        {
            for (size_t j = 0; j < N; ++j)
            {
                A[i][j] = i + j;
                B[i][j] = i * j;
            }
        }

        auto start = std::chrono::high_resolution_clock::now();
        multiply_matrices(A, B, result);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "Tamanho da matriz: " << N << "x" << N << ", Tempo de execução em C++ com OpenMP: " << duration.count() << " segundos" << std::endl;
    }

    return 0;
}
