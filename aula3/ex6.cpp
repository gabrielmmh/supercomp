#include <iostream>

int main() {
    int vetor[10];
    int maior;

    std::cout << "Insira 10 números:" << std::endl;
    for(int i = 0; i < 10; i++) {
        std::cin >> vetor[i];
        if (i == 0 || vetor[i] > maior) {
            maior = vetor[i];
        }
    }

    std::cout << "O maior número é: " << maior << std::endl;
    return 0;
}
