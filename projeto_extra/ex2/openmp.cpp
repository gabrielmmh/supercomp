#include <omp.h>
#include <fstream>
#include <iostream>
#include <string>

void transcribe_dna_to_rna(std::string &sequence)
{
#pragma omp parallel for
    for (size_t i = 0; i < sequence.size(); i++)
    {
        if (sequence[i] == 'T')
        {
            sequence[i] = 'U';
        }
    }
}

int main()
{
    std::string filename = "chrX.subst.fa"; // Substituir pelo nome correto
    std::ifstream file(filename);
    std::string dna_sequence;

    if (file.is_open())
    {
        std::string line;
        std::getline(file, line); // Pular o header
        while (std::getline(file, line))
        {
            dna_sequence += line;
        }
        file.close();
    }

    // Transcrição usando OpenMP
    transcribe_dna_to_rna(dna_sequence);

    // Salvar o RNA transcrito
    std::ofstream output("transcribed_rna.fa");
    output << "> RNA transcribed from DNA\n"
           << dna_sequence;
    output.close();

    std::cout << "Transcrição concluída e salva em transcribed_rna.fa." << std::endl;
    return 0;
}
