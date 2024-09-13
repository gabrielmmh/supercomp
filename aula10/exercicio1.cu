
#include <thrust/device_vector.h>   // Biblioteca para vetores na GPU
#include <thrust/host_vector.h>     // Biblioteca para vetores na CPU
#include <iostream>                 // Biblioteca padrão para entrada e saída de dados
#include <fstream>                  // Biblioteca para manipulação de arquivos
#include <chrono>                   // Biblioteca para medir o tempo de execução

int main() {
    std::ifstream file("stocks-google.txt");  // Abre o arquivo
    if (!file) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return 1;
    }

    // Contagem do número de linhas no arquivo
    int n = 0;
    std::string line;
    while (std::getline(file, line)) {
        n++;
    }

    // Reinicia o ponteiro de leitura para o início do arquivo
    file.clear();
    file.seekg(0, std::ios::beg);

    // Variável para armazenar o valor lido
    double value = 0.0;

    // Medir o tempo de alocação e leitura do host_vector
    auto start_alloc = std::chrono::steady_clock::now();

    // Criação do host_vector para armazenar os dados na CPU
    thrust::host_vector<double> host(n, 0);

    // Leitura dos valores do arquivo e armazenamento no host_vector
    for (int i = 0; i < n; i++) {
        file >> value;  // Lê o valor do arquivo
        host[i] = value;    // Armazena no host_vector
    }

    // Fecha o arquivo
    file.close();

    // Tempo de alocação e leitura concluído
    auto end_alloc = std::chrono::steady_clock::now();
    std::chrono::duration<double> alloc_time = end_alloc - start_alloc;
    std::cout << "Tempo de alocação e leitura (CPU): " << alloc_time.count() << " segundos" << std::endl;

    // Medir o tempo de transferência para a GPU
    auto start_copy = std::chrono::steady_clock::now();

    // Criação do device_vector e cópia dos dados da CPU (host_vector) para a GPU
    thrust::device_vector<double> device(host);

    // Tempo de transferência concluído
    auto end_copy = std::chrono::steady_clock::now();
    std::chrono::duration<double> copy_time = end_copy - start_copy;
    std::cout << "Tempo de transferência (CPU -> GPU): " << copy_time.count() << " segundos" << std::endl;

    return 0;
}
