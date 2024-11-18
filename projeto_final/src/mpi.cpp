#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <mpi.h> // Biblioteca MPI

using namespace std;

// Função para ler o grafo a partir do arquivo
vector<vector<int>> LerGrafo(const string& nomeArquivo, int& numVertices) {
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
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
            cliqueAtual.pop_back(); // Backtracking
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int numVertices;
    vector<vector<int>> grafo;

    if (rank == 0) {
        // Apenas o processo mestre lê o grafo
        string nomeArquivo = "../input/grafo.txt";
        grafo = LerGrafo(nomeArquivo, numVertices);
    }

    // Broadcast do número de vértices para todos os processos
    MPI_Bcast(&numVertices, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        // Outros processos inicializam a matriz de adjacência
        grafo.resize(numVertices, vector<int>(numVertices, 0));
    }

    // Broadcast do grafo para todos os processos
    for (int i = 0; i < numVertices; ++i) {
        MPI_Bcast(grafo[i].data(), numVertices, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Divisão de tarefas entre os processos
    vector<int> melhorCliqueLocal;
    vector<int> cliqueAtual;

    int inicio = (rank * numVertices) / size;
    int fim = ((rank + 1) * numVertices) / size;

    for (int i = inicio; i < fim; ++i) {
        cliqueAtual.clear();
        cliqueAtual.push_back(i);
        EncontrarCliqueMaxima(grafo, cliqueAtual, melhorCliqueLocal, i + 1, numVertices);
    }

    // Reduzir as melhores cliques locais para o processo mestre
    vector<int> melhorCliqueGlobal;
    int tamanhoCliqueLocal = melhorCliqueLocal.size();
    int tamanhoCliqueGlobal;

    MPI_Reduce(&tamanhoCliqueLocal, &tamanhoCliqueGlobal, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Tamanho da Clique Máxima: " << tamanhoCliqueGlobal << endl;
    }

    MPI_Finalize();
    return 0;
}
