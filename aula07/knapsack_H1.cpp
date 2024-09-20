#include <iostream>
#include <vector>
#include <algorithm> // Para std::shuffle
#include <random>    // Para std::random_device e std::mt19937

struct Item {
    int peso;
    int valor;
};

int mochilaEmbaralhada(std::vector<Item>& itens, int capacidade) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(itens.begin(), itens.end(), g);  // Embaralha os itens

    int pesoAtual = 0, valorAtual = 0;

    for (const auto& item : itens) {
        if (pesoAtual + item.peso <= capacidade) {
            pesoAtual += item.peso;
            valorAtual += item.valor;
        }
    }

    return valorAtual;
}
