#include <iostream>
#include <string>

int main() {
    std::string texto;
    std::cout << "Insira uma string: ";
    std::getline(std::cin, texto);

    std::cout << "O número de caracteres na string é: " << texto.length() << std::endl;
    return 0;
}
