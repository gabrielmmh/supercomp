#include <iostream>
#include <iomanip>
#include <cmath>
#include <omp.h>
#include <random>

int main() {
    const long N = 100000000;  // Número de pontos
    long count = 0;

    double start = omp_get_wtime();  // Início da contagem de tempo

    #pragma omp parallel
    {
        std::mt19937 rng(omp_get_thread_num());  // Gerador por thread
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        long local_count = 0;

        #pragma omp for
        for (long i = 0; i < N; ++i) {
            double x = dist(rng);
            double y = dist(rng);
            if (x * x + y * y <= 1.0) {
                ++local_count;
            }
        }

        #pragma omp atomic
        count += local_count;
    }

    double pi = 4.0 * count / N;
    double end = omp_get_wtime();  // Fim da contagem de tempo

    std::cout << std::fixed << std::setprecision(15);
    std::cout << "Valor estimado de Pi: " << pi << std::endl;
    std::cout << "Tempo de execução: " << end - start << " segundos" << std::endl;

    return 0;
}
