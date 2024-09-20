#include <iostream>

int main() {
    int matriz[3][3];
    int somaDiagonal = 0;

    std::cout << "Insira os elementos da matriz 3x3:" << std::endl;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            std::cin >> matriz[i][j];
            if(i == j) {
                somaDiagonal += matriz[i][j];
            }
        }
    }

    std::cout << "A soma dos elementos na diagonal principal é: " << somaDiagonal << std::endl;
    return 0;
}
