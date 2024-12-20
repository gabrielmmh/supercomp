#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <mpi.h>    // Biblioteca MPI
#include <omp.h>    // Biblioteca OpenMP
#include <chrono>   // Para medir o tempo de execução

using namespace std;
using namespace chrono;

// Função para ler o grafo a partir do arquivo
vector<vector<int>> LerGrafo(const string& nomeArquivo, int& numVertices) {
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
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
    MPI_Init(&argc, &argv);

    if (argc != 2) {
        if (MPI_COMM_WORLD == 0) {
            cerr << "Uso: " << argv[0] << " <arquivo_grafo>" << endl;
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    string nomeArquivo = argv[1];
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int numVertices;
    vector<vector<int>> grafo;

    if (rank == 0) {
        grafo = LerGrafo(nomeArquivo, numVertices);
    }

    MPI_Bcast(&numVertices, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        grafo.resize(numVertices, vector<int>(numVertices, 0));
    }

    for (int i = 0; i < numVertices; ++i) {
        MPI_Bcast(grafo[i].data(), numVertices, MPI_INT, 0, MPI_COMM_WORLD);
    }

    vector<int> melhorCliqueGlobal;

    auto inicioExecucao = high_resolution_clock::now();

    #pragma omp parallel
    {
        vector<int> melhorCliqueLocal;
        vector<int> cliqueAtual;

        #pragma omp for schedule(dynamic)
        for (int i = rank; i < numVertices; i += size) {
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

    int tamanhoCliqueLocal = melhorCliqueGlobal.size();
    int tamanhoCliqueGlobal;

    MPI_Reduce(&tamanhoCliqueLocal, &tamanhoCliqueGlobal, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    auto fimExecucao = high_resolution_clock::now();
    auto duracao = duration_cast<milliseconds>(fimExecucao - inicioExecucao);

    if (rank == 0) {
        cout << "Tamanho da Clique Máxima: " << tamanhoCliqueGlobal << endl;
        cout << "Tempo de execução (MPI + OpenMP): " << duracao.count() << " ms" << endl;
    }

    MPI_Finalize();
    return 0;
}
