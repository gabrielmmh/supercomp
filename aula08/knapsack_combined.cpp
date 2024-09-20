#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <fstream>
#include <sstream>

struct Item {
    int peso;
    int valor;
};

int mochilaCheia(const std::vector<Item>& itens, int capacidade) {
    std::vector<int> solucao(itens.size(), 0); // Inicia com uma solução vazia
    int pesoAtual = 0, valorAtual = 0;

    // Embaralha a solução inicial
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(solucao.begin(), solucao.end(), g);

    // Tenta incluir itens na mochila
    for (size_t i = 0; i < itens.size(); ++i) {
        if (pesoAtual + itens[i].peso <= capacidade) {
            solucao[i] = 1; // Inclui o item
            pesoAtual += itens[i].peso;
            valorAtual += itens[i].valor;
        }
    }

    return valorAtual;
}

int substituicaoDeObjeto(const std::vector<Item>& itens, int capacidade) {
    std::vector<int> solucao(itens.size(), 0); // Inicia com uma solução vazia
    int melhorValor = mochilaCheia(itens, capacidade);
    int pesoAtual = 0;

    // Calcula o peso inicial da solução
    for (size_t i = 0; i < itens.size(); ++i) {
        if (solucao[i] == 1) {
            pesoAtual += itens[i].peso;
        }
    }

    bool encontrouMelhoria;
    do {
        encontrouMelhoria = false;
        // Tenta substituir cada item na mochila
        for (size_t i = 0; i < itens.size(); ++i) {
            if (solucao[i] == 1) { // Se o item i está na mochila
                for (size_t j = 0; j < itens.size(); ++j) {
                    if (i != j && solucao[j] == 0) { // Item j não está na mochila
                        int novoPeso = pesoAtual - itens[i].peso + itens[j].peso;
                        int novoValor = melhorValor - itens[i].valor + itens[j].valor;

                        // Verifica se a troca melhora o valor sem exceder a capacidade
                        if (novoPeso <= capacidade && novoValor > melhorValor) {
                            // Realiza a troca
                            solucao[i] = 0;
                            solucao[j] = 1;
                            melhorValor = novoValor;
                            pesoAtual = novoPeso;
                            encontrouMelhoria = true;
                        }
                    }
                }
            }
        }
    } while (encontrouMelhoria);

    return melhorValor;
}

int hillClimbing(const std::vector<Item>& itens, int capacidade) {
    std::vector<int> solucao(itens.size(), 0); // Inicia com uma solução vazia
    int pesoAtual = 0, valorAtual = 0;

    // Gera uma solução inicial aleatória
    std::random_device rd;
    std::mt19937 g(rd());
    for (size_t i = 0; i < itens.size(); ++i) {
        if (pesoAtual + itens[i].peso <= capacidade) {
            solucao[i] = 1;
            pesoAtual += itens[i].peso;
            valorAtual += itens[i].valor;
        }
    }

    bool encontrouMelhoria;
    do {
        encontrouMelhoria = false;
        int melhorValor = valorAtual;
        int melhorPeso = pesoAtual;

        // Testa vizinhos gerados pela mudança de um item
        for (size_t i = 0; i < itens.size(); ++i) {
            if (solucao[i] == 0 && pesoAtual + itens[i].peso <= capacidade) {
                solucao[i] = 1;
                int novoValor = valorAtual + itens[i].valor;
                int novoPeso = pesoAtual + itens[i].peso;

                if (novoValor > melhorValor) {
                    melhorValor = novoValor;
                    melhorPeso = novoPeso;
                    encontrouMelhoria = true;
                }

                solucao[i] = 0; // Desfazer mudança para testar o próximo vizinho
            } else if (solucao[i] == 1) {
                solucao[i] = 0;
                int novoValor = valorAtual - itens[i].valor;
                int novoPeso = pesoAtual - itens[i].peso;

                if (novoValor > melhorValor) {
                    melhorValor = novoValor;
                    melhorPeso = novoPeso;
                    encontrouMelhoria = true;
                }

                solucao[i] = 1; // Desfazer mudança para testar o próximo vizinho
            }
        }

        if (encontrouMelhoria) {
            valorAtual = melhorValor;
            pesoAtual = melhorPeso;
        }
    } while (encontrouMelhoria);

    return valorAtual;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <algoritmo> <arquivo_de_entrada> (full, substituition, hillclimbing)" << std::endl;
        return 1;
    }

    std::string algoritmo = argv[1];
    std::string nome_arquivo = argv[2];

    // Ler o arquivo de entrada
    std::ifstream arquivo(nome_arquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << nome_arquivo << std::endl;
        return 1;
    }

    int capacidade;
    arquivo >> capacidade;

    std::vector<Item> itens;
    int peso, valor;
    while (arquivo >> peso >> valor) {
        itens.push_back({peso, valor});
    }
    arquivo.close();

    int valorMaximo = 0;

    if (algoritmo == "full") {
        valorMaximo = mochilaCheia(itens, capacidade);
    } else if (algoritmo == "substituition") {
        valorMaximo = substituicaoDeObjeto(itens, capacidade);
    } else if (algoritmo == "hillclimbing") {
        valorMaximo = hillClimbing(itens, capacidade);
    } else {
        std::cerr << "Algoritmo desconhecido: " << algoritmo << std::endl;
        return 1;
    }

    std::cout << "O valor máximo que pode ser colocado na mochila usando " << algoritmo << " é: " << valorMaximo << std::endl;
    return 0;
}
