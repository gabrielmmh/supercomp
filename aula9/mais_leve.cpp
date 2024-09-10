#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

struct Item {
    int peso;
    int valor;
};

// Função de comparação para ordenar por peso (menor peso primeiro)
bool compararPeso(const Item& a, const Item& b) {
    return a.peso < b.peso;
}

int maisLevePrimeiro(const std::vector<Item>& itens, int capacidade) {
    std::vector<Item> itensOrdenados = itens;
    std::sort(itensOrdenados.begin(), itensOrdenados.end(), compararPeso); // Ordena itens por peso

    int pesoAtual = 0, valorAtual = 0;

    for (const auto& item : itensOrdenados) {
        if (pesoAtual + item.peso <= capacidade) {
            pesoAtual += item.peso;
            valorAtual += item.valor;
        }
    }

    return valorAtual;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo de entrada>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Erro ao abrir arquivo de entrada: " << argv[1] << std::endl;
        return 1;
    }

    int capacidade, n;
    inputFile >> capacidade >> n;

    std::vector<Item> itens(n);
    for (int i = 0; i < n; ++i) {
        inputFile >> itens[i].peso >> itens[i].valor;
    }

    int valorMaximo = maisLevePrimeiro(itens, capacidade);
    std::cout << "O valor máximo que pode ser colocado na mochila (Mais Leve Primeiro) é: " << valorMaximo << std::endl;

    return 0;
}
