#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

// Estrutura para armazenar itens com peso e valor
struct Item {
    int peso;
    int valor;
};

// Função de busca exaustiva para encontrar o valor máximo
int knapsackExaustiva(const std::vector<Item>& itens, int capacidade, int n) {
    if (n == 0 || capacidade == 0)
        return 0;

    // Se o peso do item é maior que a capacidade da mochila, não pode ser incluído
    if (itens[n-1].peso > capacidade)
        return knapsackExaustiva(itens, capacidade, n-1);

    // Retorna o valor máximo ao incluir ou excluir o item atual
    else
        return std::max(
            itens[n-1].valor + knapsackExaustiva(itens, capacidade - itens[n-1].peso, n-1),
            knapsackExaustiva(itens, capacidade, n-1)
        );
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo de entrada>" << std::endl;
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile) {
        std::cerr << "Erro ao abrir o arquivo de entrada: " << argv[1] << std::endl;
        return 1;
    }

    int n, capacidade;
    infile >> n >> capacidade;
    std::vector<Item> itens(n);

    for (int i = 0; i < n; ++i) {
        infile >> itens[i].peso >> itens[i].valor;
    }

    int maxValor = knapsackExaustiva(itens, capacidade, n);
    std::cout << "O valor máximo que pode ser colocado na mochila é: " << maxValor << std::endl;

    return 0;
}
