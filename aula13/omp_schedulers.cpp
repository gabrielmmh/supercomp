#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <cstdlib>  // Para usar atoi
#include <numeric>  // Para std::accumulate

void print_iterations(const std::string& description,
                      const std::vector< std::vector<int> >& vectors,
                      const int n)
{
    std::vector< std::string > strings(4, std::string());
    for (int i = 0; i != n; i++)
    {
        for (int j = 0; j != 4; j++)
        {
            const auto& vector = vectors[j];
            auto it = std::find(vector.begin(), vector.end(), i);
            if (it != vector.end())
            {
                strings[j] += "*";
            }
            else 
            { 
                strings[j] += " ";
            }
        }
    }
    std::cout << description << std::endl;
    for (auto& s : strings)
    {
        std::cout << s << "\n";
    }
    std::cout << std::endl;
}

template <typename T>
double schedule(T function, const std::string& description, const int n)
{
    std::vector< std::vector<int> > vectors(4, std::vector<int>());

    double start_time = omp_get_wtime();
    function(vectors, n);
    double end_time = omp_get_wtime();

    print_iterations(description, vectors, n);
    std::cout << "Tempo de execução para " << description << ": " << (end_time - start_time) << " segundos\n";
    
    return (end_time - start_time);
}

void scheduleDefault(std::vector< std::vector<int> >& vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {    
#pragma omp for 
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleStatic(std::vector< std::vector<int> >& vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {    
        #pragma omp for schedule(static)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleStatic4(std::vector< std::vector<int> >& vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {    
#pragma omp for schedule(static, 4)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleStatic8(std::vector< std::vector<int> >& vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {    
#pragma omp for schedule(static, 8)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleDynamic(std::vector< std::vector<int> >& vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {    
#pragma omp for schedule(dynamic)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleDynamic1(std::vector< std::vector<int> >& vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {    
#pragma omp for schedule(dynamic, 1)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleDynamic4(std::vector< std::vector<int> >& vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {    
#pragma omp for schedule(dynamic, 4)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleDynamic8(std::vector< std::vector<int> >& vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {    
#pragma omp for schedule(dynamic, 8)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleGuided(std::vector< std::vector<int> >& vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {    
#pragma omp for schedule(guided)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleGuided2(std::vector< std::vector<int> >& vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {    
#pragma omp for schedule(guided, 2)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleGuided4(std::vector< std::vector<int> >& vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {    
#pragma omp for schedule(guided, 4)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleGuided8(std::vector< std::vector<int> >& vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {    
#pragma omp for schedule(guided, 8)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleAuto(std::vector< std::vector<int> >& vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {    
#pragma omp for schedule(auto)
        for (int i = 0; n > i; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleRuntime(std::vector< std::vector<int> >& vectors, int n)
{
#pragma omp parallel num_threads(4), shared(vectors, n)
    {    
#pragma omp for schedule(runtime)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <numero_de_execucoes>" << std::endl;
        return 1;
    }

    int execucoes = std::atoi(argv[1]); // Pega o número de execuções a partir do argumento de linha de comando
    const int n = 64;

    std::vector<double> tempos_default, tempos_static;

    for (int i = 0; i < execucoes; ++i) {
        tempos_default.push_back(schedule(scheduleDefault, "default:               ", n));
        tempos_static.push_back(schedule(scheduleStatic, "schedule(static):      ", n));
    }

    // Cálculo da média dos tempos
    double media_default = std::accumulate(tempos_default.begin(), tempos_default.end(), 0.0) / execucoes;
    double media_static = std::accumulate(tempos_static.begin(), tempos_static.end(), 0.0) / execucoes;

    std::cout << "Tempo médio de execução para default: " << media_default << " segundos\n";
    std::cout << "Tempo médio de execução para schedule(static): " << media_static << " segundos\n";

    return 0;
}
