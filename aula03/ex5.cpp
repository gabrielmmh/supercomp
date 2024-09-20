#include <iostream>

int main() {
    int vetor[5];
    int soma = 0;

    std::cout << "Insira 5 números:" << std::endl;
    for(int i = 0; i < 5; i++) {
        std::cin >> vetor[i];
        soma += vetor[i];
    }

    std::cout << "A soma dos números é: " << soma << std::endl;
    return 0;
}
