#include <thrust/device_vector.h>   // Biblioteca para vetores na GPU
#include <thrust/host_vector.h>     // Biblioteca para vetores na CPU
#include <thrust/transform.h>       // Biblioteca para transformações entre vetores
#include <thrust/reduce.h>          // Biblioteca para redução (soma)
#include <iostream>                 // Biblioteca padrão para entrada e saída de dados
#include <fstream>                  // Biblioteca para manipulação de arquivos
#include <sstream>                  // Biblioteca para processamento de strings

int main() {
    std::ifstream file("stocks2.txt");  // Abre o arquivo
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

    // Criação de host_vectors para armazenar os preços das ações da Apple e Microsoft
    thrust::host_vector<double> AAPL_host(n);
    thrust::host_vector<double> MSFT_host(n);

    // Leitura dos valores do arquivo e armazenamento nos host_vectors
    int i = 0;
    double aapl_price, msft_price;
    std::string token;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::getline(ss, token, ',');  // Lê o preço da Apple (AAPL)
        aapl_price = std::stod(token);
        std::getline(ss, token, ',');  // Lê o preço da Microsoft (MSFT)
        msft_price = std::stod(token);

        AAPL_host[i] = aapl_price;
        MSFT_host[i] = msft_price;
        i++;
    }

    // Fecha o arquivo
    file.close();

    // Transferir dados para a GPU (device_vectors)
    thrust::device_vector<double> AAPL_device = AAPL_host;
    thrust::device_vector<double> MSFT_device = MSFT_host;

    // Vetor para armazenar as diferenças entre os preços
    thrust::device_vector<double> diff_device(n);

    // Calcula a diferença ponto a ponto entre AAPL e MSFT e armazena no vetor diff_device
    thrust::transform(AAPL_device.begin(), AAPL_device.end(), MSFT_device.begin(), diff_device.begin(), thrust::minus<double>());

    // Calcula a soma das diferenças
    double soma_diferencas = thrust::reduce(diff_device.begin(), diff_device.end(), 0.0, thrust::plus<double>());

    // Calcula a média das diferenças
    double media_diferencas = soma_diferencas / n;

    std::cout << "Diferença média entre os preços da Apple e Microsoft: " << media_diferencas << std::endl;

    return 0;
}
