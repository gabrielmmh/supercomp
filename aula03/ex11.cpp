#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>

int main() {
    std::ifstream arquivoEntrada("entrada.txt");
    if(!arquivoEntrada) {
        std::cerr << "Erro ao abrir o arquivo de entrada!" << std::endl;
        return 1;
    }

    std::ofstream arquivoSaida("estatisticas.txt");
    if(!arquivoSaida) {
        std::cerr << "Erro ao abrir o arquivo de saída!" << std::endl;
        return 1;
    }

    std::string linha;
    int totalLinhas = 0;
    int totalPalavras = 0;
    std::map<std::string, int> frequenciaPalavras;

    while(std::getline(arquivoEntrada, linha)) {
        totalLinhas++;
        std::istringstream stream(linha);
        std::string palavra;
        while(stream >> palavra) {
            totalPalavras++;
            frequenciaPalavras[palavra]++;
        }
    }

    int maiorFrequencia = 0;
    std::string palavraMaisFrequente;
    for(const auto& par : frequenciaPalavras) {
        if(par.second > maiorFrequencia) {
            maiorFrequencia = par.second;
            palavraMaisFrequente = par.first;
        }
    }

    arquivoSaida << "Número total de linhas: " << totalLinhas << std::endl;
    arquivoSaida << "Número total de palavras: " << totalPalavras << std::endl;
    arquivoSaida << "Número médio de palavras por linha: " << static_cast<double>(totalPalavras) / totalLinhas << std::endl;
    arquivoSaida << "Palavra mais frequente: " << palavraMaisFrequente << " (aparece " << maiorFrequencia << " vezes)" << std::endl;

    arquivoEntrada.close();
    arquivoSaida.close();

    std::cout << "Estatísticas gravadas em 'estatisticas.txt'." << std::endl;
    return 0;
}
