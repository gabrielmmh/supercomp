#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono> // Para medir o tempo de execução

using namespace std;
using namespace chrono;

// Função para ler o grafo a partir do arquivo
vector<vector<int>> LerGrafo(const string& nomeArquivo, int& numVertices) {
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        exit(1);
    }

    int numArestas;
    arquivo >> numVertices >> numArestas;

    // Criar a matriz de adjacência
    vector<vector<int>> grafo(numVertices, vector<int>(numVertices, 0));

    // Preencher a matriz com base nas arestas do arquivo
    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;  // Grafo não direcionado
        grafo[v - 1][u - 1] = 1;  // Simetria
    }

    arquivo.close();
    return grafo;
}

// Função recursiva para encontrar a clique máxima (Abordagem Exaustiva)
void EncontrarCliqueMaxima(
    const vector<vector<int>>& grafo,
    vector<int>& cliqueAtual,
    vector<int>& melhorClique,
    int verticeAtual,
    int numVertices) {
    
    // Verificar se a clique atual é maior que a melhor até o momento
    if (cliqueAtual.size() > melhorClique.size()) {
        melhorClique = cliqueAtual;
    }

    // Explorar vértices restantes
    for (int i = verticeAtual; i < numVertices; ++i) {
        bool podeAdicionar = true;

        // Verificar se o vértice é adjacente a todos na clique atual
        for (int v : cliqueAtual) {
            if (grafo[v][i] == 0) {
                podeAdicionar = false;
                break;
            }
        }

        // Adicionar o vértice à clique e continuar a busca
        if (podeAdicionar) {
            cliqueAtual.push_back(i);
            EncontrarCliqueMaxima(grafo, cliqueAtual, melhorClique, i + 1, numVertices);
            cliqueAtual.pop_back();  // Backtracking
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <arquivo_grafo>" << endl;
        return 1;
    }

    string nomeArquivo = argv[1];
    int numVertices;

    // Ler o grafo do arquivo
    vector<vector<int>> grafo = LerGrafo(nomeArquivo, numVertices);

    // Inicializar estruturas para encontrar a clique máxima
    vector<int> melhorClique;
    vector<int> cliqueAtual;

    // Medir o tempo de execução
    auto inicio = high_resolution_clock::now();
    
    // Encontrar a clique máxima
    EncontrarCliqueMaxima(grafo, cliqueAtual, melhorClique, 0, numVertices);

    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<milliseconds>(fim - inicio);

    // Exibir a clique máxima encontrada
    cout << "Clique Máxima Encontrada (Tamanho: " << melhorClique.size() << "): ";
    for (int v : melhorClique) {
        cout << (v + 1) << " ";  // +1 para ajustar índice ao formato do arquivo
    }
    cout << endl;

    // Exibir o tempo de execução
    cout << "Tempo de execução: " << duracao.count() << " ms" << endl;

    return 0;
}
