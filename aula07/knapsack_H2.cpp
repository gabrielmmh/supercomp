#include <iostream>
#include <vector>
#include <random> // Para std::random_device, std::mt19937 e std::uniform_real_distribution

struct Item {
    int peso;
    int valor;
};

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
