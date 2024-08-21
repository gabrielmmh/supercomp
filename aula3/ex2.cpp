
#include <iostream>
#include <vector>

// Classe Matriz para representar uma matriz quadrada
class Matriz {
public:
    Matriz(int N) : data(N, std::vector<int>(N, 0)) {}

    int get(int i, int j) const {
        return data[i][j];
    }

    void set(int i, int j, int valor) {
        data[i][j] = valor;
    }

    void imprime() const {
        for (const auto& linha : data) {
            for (int valor : linha) {
                std::cout << valor << " ";
            }
            std::cout << std::endl;
        }
    }

    int tamanho() const {
        return data.size();
    }

private:
    std::vector<std::vector<int>> data;
};

// Função que multiplica duas matrizes A e B, armazenando o resultado em C
void multiplicaMatriz(const Matriz& A, const Matriz& B, Matriz& C) {
    int N = A.tamanho();

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int soma = 0;
            for (int k = 0; k < N; k++) {
                soma += A.get(i, k) * B.get(k, j);
            }
            C.set(i, j, soma);
        }
    }
}

int main() {
    int N = 3;

    Matriz A(N), B(N), C(N);

    A.set(0, 0, 1); A.set(0, 1, 2); A.set(0, 2, 3);
    A.set(1, 0, 4); A.set(1, 1, 5); A.set(1, 2, 6);
    A.set(2, 0, 7); A.set(2, 1, 8); A.set(2, 2, 9);

    B.set(0, 0, 9); B.set(0, 1, 8); B.set(0, 2, 7);
    B.set(1, 0, 6); B.set(1, 1, 5); B.set(1, 2, 4);
    B.set(2, 0, 3); B.set(2, 1, 2); B.set(2, 2, 1);

    multiplicaMatriz(A, B, C);

    C.imprime();

    return 0;
}
