#include <iostream>
#include <fstream>
#include <cmath>  // Para gerar alguns dados de exemplo
#include <chrono> // Para usar o sleep
#include <thread> // Para usar o sleep_for

int main() {
    // Abrir o arquivo CSV para escrita
    std::ofstream outFile("test.csv");

    // Verificar se o arquivo foi aberto corretamente
    if (!outFile) {
        std::cerr << "Não foi possível abrir o arquivo!" << std::endl;
        return 1;
    }

    // Gerar alguns dados de exemplo (e.g., uma função seno)
    for (double x = 0; x <= 1000; x += 0.1) {  // Número reduzido de iterações para teste
        double y = std::sin(x); // Exemplo de uma função, pode ser qualquer outro cálculo
        
        // Escrever os dados no formato CSV (X, Y)
        outFile << x << "," << y << "\n";  
        outFile.flush();  // Força a escrita no arquivo

        // Pausar a execução
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // Imprimir no console para acompanhar o progresso
        std::cout << "Escrevendo dados: " << x << "," << y << std::endl;
    }

    // Fechar o arquivo
    outFile.close();

    std::cout << "Dados escritos no arquivo 'test.csv' com sucesso!" << std::endl;
    return 0;
}
