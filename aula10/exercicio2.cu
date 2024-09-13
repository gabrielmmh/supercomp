#include <thrust/device_vector.h>   // Biblioteca para vetores na GPU
#include <thrust/host_vector.h>     // Biblioteca para vetores na CPU
#include <thrust/reduce.h>          // Biblioteca para operações de redução
#include <thrust/extrema.h>         // Biblioteca para encontrar o máximo e o mínimo
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

    // Criação do host_vector para armazenar os dados na CPU
    thrust::host_vector<double> host(n, 0);

    // Leitura dos valores do arquivo e armazenamento no host_vector
    for (int i = 0; i < n; i++) {
        file >> value;  // Lê o valor do arquivo
        host[i] = value;    // Armazena no host_vector
    }

    // Fecha o arquivo
    file.close();

    // Transferir dados para a GPU
    thrust::device_vector<double> device(host);

    // Cálculo do preço médio nos últimos 10 anos (período total)
    double soma_total = thrust::reduce(device.begin(), device.end(), 0.0, thrust::plus<double>());
    double media_total = soma_total / n;
    std::cout << "Preço médio das ações nos últimos 10 anos: " << media_total << std::endl;

    // Cálculo do preço médio nos últimos 365 dias
    int ultimo_ano = std::min(365, n);  // Caso o número de dias seja menor que 365
    double soma_ultimo_ano = thrust::reduce(device.end() - ultimo_ano, device.end(), 0.0, thrust::plus<double>());
    double media_ultimo_ano = soma_ultimo_ano / ultimo_ano;
    std::cout << "Preço médio das ações nos últimos 365 dias: " << media_ultimo_ano << std::endl;

    // Encontrar o maior e menor preço no período total
    auto minmax_total = thrust::minmax_element(device.begin(), device.end());
    double menor_total = *minmax_total.first;
    double maior_total = *minmax_total.second;
    std::cout << "Menor preço nos últimos 10 anos: " << menor_total << std::endl;
    std::cout << "Maior preço nos últimos 10 anos: " << maior_total << std::endl;

    // Encontrar o maior e menor preço nos últimos 365 dias
    auto minmax_ultimo_ano = thrust::minmax_element(device.end() - ultimo_ano, device.end());
    double menor_ultimo_ano = *minmax_ultimo_ano.first;
    double maior_ultimo_ano = *minmax_ultimo_ano.second;
    std::cout << "Menor preço nos últimos 365 dias: " << menor_ultimo_ano << std::endl;
    std::cout << "Maior preço nos últimos 365 dias: " << maior_ultimo_ano << std::endl;

    return 0;
}
