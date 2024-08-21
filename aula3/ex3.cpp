
#include <iostream>
#include <chrono>
#include <vector>

int somaMatriz(int** matriz, int N) {
    int soma = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            soma += matriz[i][j];
        }
    }
    return soma;
}

int somaMatriz(const std::vector<std::vector<int>>& matriz) {
    int soma = 0;
    for (const auto& linha : matriz) {
        for (int valor : linha) {
            soma += valor;
        }
    }
    return soma;
}

int main() {
    int N = 1000;

    // Alocação dinâmica manual
    int** matrizDinamica = new int*[N];
    for (int i = 0; i < N; ++i) {
        matrizDinamica[i] = new int[N];
    }

    // Inicialização da matriz
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrizDinamica[i][j] = i + j;
        }
    }

    // Medição de tempo para soma da matriz alocada dinamicamente
    auto inicio = std::chrono::high_resolution_clock::now();
    int soma = somaMatriz(matrizDinamica, N);
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracaoDinamica = fim - inicio;
    std::cout << "Soma (matriz dinâmica): " << soma << std::endl;
    std::cout << "Tempo (matriz dinâmica): " << duracaoDinamica.count() << " segundos" << std::endl;

    // Liberação da memória alocada dinamicamente
    for (int i = 0; i < N; ++i) {
        delete[] matrizDinamica[i];
    }
    delete[] matrizDinamica;

    // Usando std::vector
    std::vector<std::vector<int>> matrizVector(N, std::vector<int>(N));

    // Inicialização da matriz
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrizVector[i][j] = i + j;
        }
    }

    // Medição de tempo para soma da matriz usando std::vector
    inicio = std::chrono::high_resolution_clock::now();
    soma = somaMatriz(matrizVector);
    fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracaoVector = fim - inicio;
    std::cout << "Soma (std::vector): " << soma << std::endl;
    std::cout << "Tempo (std::vector): " << duracaoVector.count() << " segundos" << std::endl;

    return 0;
}
