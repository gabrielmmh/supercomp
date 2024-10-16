#include <iostream>
#include <iomanip>
#include <cmath>
#include <omp.h>

int main() {
    const long N = 100000000;  // Número de pontos
    long count = 0;            // Pontos dentro do círculo

    double start = omp_get_wtime();  // Início da contagem de tempo

    for (long i = 0; i < N; ++i) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            ++count;
        }
    }

    double pi = 4.0 * count / N;
    double end = omp_get_wtime();  // Fim da contagem de tempo

    std::cout << std::fixed << std::setprecision(15);
    std::cout << "Valor estimado de Pi: " << pi << std::endl;
    std::cout << "Tempo de execução: " << end - start << " segundos" << std::endl;

    return 0;
}
