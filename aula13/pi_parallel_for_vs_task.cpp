#include <omp.h>
#include <iostream>
#include <iomanip>
#include <vector>

// Função para o cálculo de Pi usando recursão e parallel for
void pi_r_parallel_for(long Nstart, long Nfinish, double step, double& sum, long MIN_BLK) {
    long i, iblk;
    if (Nfinish - Nstart < MIN_BLK) {
        #pragma omp parallel for reduction(+:sum)
        for (i = Nstart; i < Nfinish; i++) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
    } else {
        iblk = Nfinish - Nstart;
        pi_r_parallel_for(Nstart, Nfinish - iblk / 2, step, sum, MIN_BLK);
        pi_r_parallel_for(Nfinish - iblk / 2, Nfinish, step, sum, MIN_BLK);
    }
}

// Função para o cálculo de Pi usando tasks com redução manual
void pi_r_tasks(long Nstart, long Nfinish, double step, double& sum, long MIN_BLK) {
    long i, iblk;
    double local_sum = 0.0;  // Variável local para evitar atomic

    if (Nfinish - Nstart < MIN_BLK) {
        // Para blocos menores que MIN_BLK, faz o cálculo normalmente
        for (i = Nstart; i < Nfinish; i++) {
            double x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }
        #pragma omp atomic
        sum += local_sum;  // Soma local para evitar concorrência
    } else {
        iblk = Nfinish - Nstart;
        // Dividimos o intervalo em duas tasks
        #pragma omp task shared(sum) firstprivate(Nstart, Nfinish, step, MIN_BLK)
        pi_r_tasks(Nstart, Nfinish - iblk / 2, step, sum, MIN_BLK);
        #pragma omp task shared(sum) firstprivate(Nstart, Nfinish, step, MIN_BLK)
        pi_r_tasks(Nfinish - iblk / 2, Nfinish, step, sum, MIN_BLK);
        #pragma omp taskwait  // Espera por todas as tasks filhas antes de prosseguir
    }
}

int main() {
    long num_steps = 1024l * 1024 * 1024 * 2;
    double step = 1.0 / (double)num_steps;
    
    std::vector<long> MIN_BLK_values = {1024 * 1024 * 64, 1024 * 1024 * 256, 1024 * 1024 * 512};
    
    for (long MIN_BLK : MIN_BLK_values) {
        double sum_parallel_for = 0.0;
        double sum_tasks = 0.0;

        // Cálculo usando parallel for
        double init_time_for = omp_get_wtime();
        pi_r_parallel_for(0, num_steps, step, sum_parallel_for, MIN_BLK);
        double pi_parallel_for = step * sum_parallel_for;
        double time_parallel_for = omp_get_wtime() - init_time_for;

        // Cálculo usando tasks
        double init_time_task = omp_get_wtime();
        #pragma omp parallel
        {
            #pragma omp single
            pi_r_tasks(0, num_steps, step, sum_tasks, MIN_BLK);
        }
        double pi_tasks = step * sum_tasks;
        double time_tasks = omp_get_wtime() - init_time_task;

        // Resultados
        std::cout << "MIN_BLK: " << MIN_BLK << std::endl;
        std::cout << "for parallel for " << num_steps << " steps pi = " << std::setprecision(15) << pi_parallel_for << " in " << time_parallel_for << " secs\n";
        std::cout << "for tasks " << num_steps << " steps pi = " << std::setprecision(15) << pi_tasks << " in " << time_tasks << " secs\n";
        std::cout << "----------------------------------------" << std::endl;
    }

    return 0;
}
