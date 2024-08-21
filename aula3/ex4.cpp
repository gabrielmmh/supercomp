#include <iostream>

class Vector {
public:
    Vector(int tamanho) : tam(tamanho), capacidade(tamanho), dados(new int[tamanho]) {}

    ~Vector() {
        delete[] dados;
    }

    void inicializa(int valor) {
        for (int i = 0; i < tam; ++i) {
            dados[i] = valor;
        }
    }

    int get(int index) const {
        if (index >= 0 && index < tam) {
            return dados[index];
        } else {
            throw std::out_of_range("Índice fora do intervalo.");
        }
    }

    void set(int index, int valor) {
        if (index >= 0 && index < tam) {
            dados[index] = valor;
        } else {
            throw std::out_of_range("Índice fora do intervalo.");
        }
    }

    void inserir(int index, int valor) {
        if (index < 0 || index > tam) {
            throw std::out_of_range("Índice fora do intervalo.");
        }
        if (tam == capacidade) {
            redimensiona(capacidade * 2);
        }
        for (int i = tam; i > index; --i) {
            dados[i] = dados[i - 1];
        }
        dados[index] = valor;
        ++tam;
    }

    void remover(int index) {
        if (index < 0 || index >= tam) {
            throw std::out_of_range("Índice fora do intervalo.");
        }
        for (int i = index; i < tam - 1; ++i) {
            dados[i] = dados[i + 1];
        }
        --tam;
    }

    void imprime() const {
        for (int i = 0; i < tam; ++i) {
            std::cout << dados[i] << " ";
        }
        std::cout << std::endl;
    }

private:
    int* dados;
    int tam;
    int capacidade;

    void redimensiona(int novaCapacidade) {
        int* novoDados = new int[novaCapacidade];
        for (int i = 0; i < tam; ++i) {
            novoDados[i] = dados[i];
        }
        delete[] dados;
        dados = novoDados;
        capacidade = novaCapacidade;
    }
};

int main() {
    Vector vec(5);
    vec.inicializa(0);
    vec.imprime();

    vec.set(2, 10);
    vec.inserir(1, 5);
    vec.imprime();

    vec.remover(3);
    vec.imprime();

    return 0;
}

