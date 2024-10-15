#include <iostream>
#include <vector>
#include <omp.h>

double conta_complexa(int i) {
    return 2 * i;
}

int main() {
    int N = 1000000;  // Tamanho do vetor
    std::vector<double> vec;

    double init_time = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        double result = conta_complexa(i);
        #pragma omp critical
        {
            vec.push_back(result);  // Acesso à região crítica para evitar condições de corrida
        }
    }
    double exec_time = omp_get_wtime() - init_time;

    std::cout << "Tempo de execução com omp critical: " << exec_time << " segundos" << std::endl;

    return 0;
}
