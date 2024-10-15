#include <iostream>
#include <vector>
#include <omp.h>

double conta_complexa(int i) {
    return 2 * i;
}

int main() {
    int N = 1000000;  // Tamanho do vetor
    std::vector<double> vec(N);  // Pré-aloca o vetor com o tamanho necessário

    double init_time = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        vec[i] = conta_complexa(i);  // Não há necessidade de região crítica
    }
    double exec_time = omp_get_wtime() - init_time;

    std::cout << "Tempo de execução com pré-alocação de memória: " << exec_time << " segundos" << std::endl;

    return 0;
}
