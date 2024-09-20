#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

struct Item {
    int peso;
    int valor;
};

// Função de comparação para ordenar por valor (maior valor primeiro)
bool compararValor(const Item& a, const Item& b) {
    return a.valor > b.valor;
}

int maisCaroPrimeiro(const std::vector<Item>& itens, int capacidade) {
    std::vector<Item> itensOrdenados = itens;
    std::sort(itensOrdenados.begin(), itensOrdenados.end(), compararValor); // Ordena itens por valor

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

    int valorMaximo = maisCaroPrimeiro(itens, capacidade);
    std::cout << "O valor máximo que pode ser colocado na mochila (Mais Caro Primeiro) é: " << valorMaximo << std::endl;

    return 0;
}
