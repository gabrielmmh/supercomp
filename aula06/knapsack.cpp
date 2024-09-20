#include <iostream>
#include <vector>
#include <fstream>

// Função recursiva para encontrar o valor máximo que pode ser obtido.
int knapsack(int W, const std::vector<int>& weights, const std::vector<int>& values, int n) {
    // Caso base: não há itens ou a capacidade é zero.
    if (n == 0 || W == 0) {
        return 0;
    }

    // Se o peso do n-ésimo item é mais do que a capacidade atual da mochila, não podemos incluí-lo.
    if (weights[n - 1] > W) {
        return knapsack(W, weights, values, n - 1);
    } else {
        // Caso contrário, retornamos o máximo de duas opções:
        // 1. Valor obtido ao incluir o n-ésimo item.
        // 2. Valor obtido ao excluir o n-ésimo item.
        int include_item = values[n - 1] + knapsack(W - weights[n - 1], weights, values, n - 1);
        int exclude_item = knapsack(W, weights, values, n - 1);
        return std::max(include_item, exclude_item);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo de entrada>" << std::endl;
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile) {
        std::cerr << "Erro ao abrir o arquivo de entrada." << std::endl;
        return 1;
    }

    int N, W;
    infile >> N >> W;

    std::vector<int> weights(N);
    std::vector<int> values(N);

    for (int i = 0; i < N; i++) {
        infile >> weights[i] >> values[i];
    }

    int max_value = knapsack(W, weights, values, N);

    std::cout << "O valor máximo que pode ser colocado na mochila é: " << max_value << std::endl;

    return 0;
}
