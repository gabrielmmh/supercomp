
#include <iostream>
#include <fstream>
#include <vector>

// Função que lê números de um arquivo e os armazena em um vetor.
void lerArquivo(const std::string& nomeArquivo, std::vector<int>& numeros) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return;
    }
    
    int numero;
    while (arquivo >> numero) {
        numeros.push_back(numero);
    }
}

// Função que dobra os valores do vetor usando passagem por referência.
void dobrarValoresReferencia(const std::vector<int>& numerosOriginais, std::vector<int>& numerosDobrados) {
    numerosDobrados.clear();  // Limpa o vetor para garantir que está vazio
    for (int numero : numerosOriginais) {
        numerosDobrados.push_back(numero * 2);
    }
}

// Função que dobra os valores do vetor usando ponteiros.
void dobrarValoresPonteiro(const std::vector<int>* numerosOriginais, std::vector<int>* numerosDobrados) {
    if (numerosOriginais == nullptr || numerosDobrados == nullptr) {
        std::cerr << "Ponteiro nulo!" << std::endl;
        return;
    }
    numerosDobrados->clear();  // Limpa o vetor para garantir que está vazio
    for (int numero : *numerosOriginais) {
        numerosDobrados->push_back(numero * 2);
    }
}

// Função que escreve os números de um vetor em um arquivo.
void escreverArquivo(const std::string& nomeArquivo, const std::vector<int>& numeros) {
    std::ofstream arquivo(nomeArquivo);
    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo para escrita!" << std::endl;
        return;
    }
    
    for (int numero : numeros) {
        arquivo << numero << std::endl;
    }
}

int main() {
    std::vector<int> numeros, numerosDobrados;

    lerArquivo("entrada.txt", numeros);

    dobrarValoresReferencia(numeros, numerosDobrados);
    escreverArquivo("saida_referencia.txt", numerosDobrados);

    dobrarValoresPonteiro(&numeros, &numerosDobrados);
    escreverArquivo("saida_ponteiro.txt", numerosDobrados);

    return 0;
}
