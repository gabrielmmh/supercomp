#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <omp.h>    // Biblioteca OpenMP
#include <chrono>   // Para medir o tempo de execução

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

    vector<vector<int>> grafo(numVertices, vector<int>(numVertices, 0));

    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;
    }

    arquivo.close();
    return grafo;
}

// Função recursiva para encontrar a clique máxima
void EncontrarCliqueMaxima(
    const vector<vector<int>>& grafo,
    vector<int>& cliqueAtual,
    vector<int>& melhorClique,
    int verticeAtual,
    int numVertices) {

    if (cliqueAtual.size() > melhorClique.size()) {
        melhorClique = cliqueAtual;
    }

    for (int i = verticeAtual; i < numVertices; ++i) {
        bool podeAdicionar = true;

        for (int v : cliqueAtual) {
            if (grafo[v][i] == 0) {
                podeAdicionar = false;
                break;
            }
        }

        if (podeAdicionar) {
            cliqueAtual.push_back(i);
            EncontrarCliqueMaxima(grafo, cliqueAtual, melhorClique, i + 1, numVertices);
            cliqueAtual.pop_back();
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

    vector<int> melhorCliqueGlobal;

    auto inicioExecucao = high_resolution_clock::now();

    #pragma omp parallel
    {
        vector<int> melhorCliqueLocal;
        vector<int> cliqueAtual;

        #pragma omp for schedule(dynamic)
        for (int i = 0; i < numVertices; ++i) {
            cliqueAtual.clear();
            cliqueAtual.push_back(i);
            EncontrarCliqueMaxima(grafo, cliqueAtual, melhorCliqueLocal, i + 1, numVertices);
        }

        #pragma omp critical
        {
            if (melhorCliqueLocal.size() > melhorCliqueGlobal.size()) {
                melhorCliqueGlobal = melhorCliqueLocal;
            }
        }
    }

    auto fimExecucao = high_resolution_clock::now();
    auto duracao = duration_cast<milliseconds>(fimExecucao - inicioExecucao);

    // Exibir a clique máxima encontrada
    cout << "Clique Máxima Encontrada (Tamanho: " << melhorCliqueGlobal.size() << "): ";
    for (int v : melhorCliqueGlobal) {
        cout << (v + 1) << " ";
    }
    cout << endl;

    // Exibir o tempo de execução
    cout << "Tempo de execução (OpenMP): " << duracao.count() << " ms" << endl;

    return 0;
}
