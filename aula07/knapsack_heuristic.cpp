#include <iostream>
#include <vector>
#include <algorithm>  // Para std::shuffle
#include <random>     // Para std::random_device e std::mt19937
#include <fstream>

// Estrutura para armazenar itens com peso e valor
struct Item {
    int peso;
    int valor;
};

// Função para a heurística de embaralhar e preencher a mochila
int mochilaEmbaralhada(std::vector<Item>& itens, int capacidade) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(itens.begin(), itens.end(), g);

    int pesoAtual = 0, valorAtual = 0;
    for (const auto& item : itens) {
        if (pesoAtual + item.peso <= capacidade) {
            pesoAtual += item.peso;
            valorAtual += item.valor;
        }
    }
    return valorAtual;
}

// Função para a heurística de seleção probabilística
int mochilaProbabilistica(const std::vector<Item>& itens, int capacidade, double limiar) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    int pesoAtual = 0, valorAtual = 0;
    for (const auto& item : itens) {
        double prob = dis(gen);
        if (prob > limiar && pesoAtual + item.peso <= capacidade) {
            pesoAtual += item.peso;
            valorAtual += item.valor;
        }
    }
    return valorAtual;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <método> <arquivo de entrada>" << std::endl;
        return 1;
    }

    int metodo = std::stoi(argv[1]);
    std::ifstream infile(argv[2]);
    if (!infile) {
        std::cerr << "Erro ao abrir o arquivo de entrada: " << argv[2] << std::endl;
        return 1;
    }

    int n, capacidade;
    infile >> n >> capacidade;
    std::vector<Item> itens(n);

    for (int i = 0; i < n; ++i) {
        infile >> itens[i].peso >> itens[i].valor;
    }

    int maxValor = 0;
    if (metodo == 1) {
        maxValor = mochilaEmbaralhada(itens, capacidade);
    } else if (metodo == 2) {
        maxValor = mochilaProbabilistica(itens, capacidade, 0.5);  // Limiar de 0.5
    } else {
        std::cerr << "Método inválido. Use 1 para embaralhada ou 2 para probabilística." << std::endl;
        return 1;
    }

    std::cout << "O valor máximo encontrado pela heurística é: " << maxValor << std::endl;

    return 0;
}
