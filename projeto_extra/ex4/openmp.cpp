#include <omp.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Tabela de códons para aminoácidos
std::unordered_map<std::string, int> codon_to_aminoacid = {
    {"AUG", 1}, // Início (Metionina)
    {"UUU", 2},
    {"UUC", 2}, // Fenilalanina
    {"UUA", 3},
    {"UUG", 3}, // Leucina
    {"UAA", -1},
    {"UAG", -1},
    {"UGA", -1} // Códons de parada
    // Adicione outros códons conforme necessário
};

std::vector<int> translate_rna_to_protein(const std::string &rna_sequence)
{
    std::vector<int> protein_sequence;
#pragma omp parallel for
    for (size_t i = 0; i <= rna_sequence.size() - 3; i += 3)
    {
        std::string codon = rna_sequence.substr(i, 3);
        if (codon_to_aminoacid.find(codon) != codon_to_aminoacid.end())
        {
            int amino_acid = codon_to_aminoacid[codon];
            if (amino_acid == -1)
                break; // Parar ao encontrar códon de parada

#pragma omp critical
            protein_sequence.push_back(amino_acid);
        }
    }
    return protein_sequence;
}

int main()
{
    std::string filename = "transcribed_rna.fa"; // Arquivo RNA transcrito
    std::ifstream file(filename);
    std::string rna_sequence;

    if (file.is_open())
    {
        std::string line;
        std::getline(file, line); // Pular o header
        while (std::getline(file, line))
        {
            rna_sequence += line;
        }
        file.close();
    }

    // Traduzir RNA para sequência de aminoácidos
    std::vector<int> protein_sequence = translate_rna_to_protein(rna_sequence);

    // Exibir a sequência traduzida
    std::cout << "Sequência de proteína (aminoácidos): ";
    for (int amino_acid : protein_sequence)
    {
        std::cout << amino_acid << " ";
    }
    std::cout << std::endl;

    return 0;
}
