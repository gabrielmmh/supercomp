#include <omp.h>
#include <fstream>
#include <iostream>
#include <string>

int count_codons(const std::string &rna_sequence, const std::string &codon)
{
    int count = 0;
    for (size_t i = 0; i <= rna_sequence.size() - 3; i += 3)
    {
        if (rna_sequence.substr(i, 3) == codon)
        {
            count++;
        }
    }
    return count;
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

    int global_count = 0;

// Contagem paralelizada com OpenMP
#pragma omp parallel for reduction(+ : global_count)
    for (size_t i = 0; i <= rna_sequence.size() - 3; i += 3)
    {
        if (rna_sequence.substr(i, 3) == "AUG")
        {
            global_count++;
        }
    }

    std::cout << "Total de proteínas inicializadas (AUG encontrados): " << global_count << std::endl;
    return 0;
}
