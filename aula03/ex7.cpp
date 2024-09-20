#include <iostream>

int main() {
    int vetor[10];

    std::cout << "Insira 10 números:" << std::endl;
    for(int i = 0; i < 10; i++) {
        std::cin >> vetor[i];
    }

    // Bubble Sort
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9 - i; j++) {
            if(vetor[j] > vetor[j + 1]) {
                int temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }

    std::cout << "Números ordenados: ";
    for(int i = 0; i < 10; i++) {
        std::cout << vetor[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}
