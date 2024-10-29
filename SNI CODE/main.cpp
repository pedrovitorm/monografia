#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <ctime>
#include "Firma.h"
#include "Trabalhador.h"
#include "Mercado.h"
#include "Consumidor.h" //ja incluso em Trabalhador e Pesquisador

//Agentes em implementacao
//#include "Universidade.h"
//#include "Pesquisador.h"
//#include "Governo.h"
//#include "Banco.h"

//PARAMETROS GERAIS
const int NUM_ITERACOES = 500;

//FUNCOES AUXILIARES___________________________________________________________________________________________
// Função para criar as firmas com valores aleatórios com distribuição normal
void criar_firmas(std::vector<std::shared_ptr<Firma>>& firmas) {

    //PARAMETROS FIRMAS
    const int NUM_FIRMAS = 2;
    const double MEDIA_CAPITAL = 400; // Média para o capital
    const double DESVIO_PADRAO_CAPITAL = 30.0; // Desvio padrão para o capital
    const double MEDIA_SALARIO = 30.0; // Média para o salário
    const double DESVIO_PADRAO_SALARIO = 15.0; // Desvio padrão para o salário
    const int MEDIA_ESTOQUE_INICIAL = 10;
    const int DESVIO_PADRAO_ESTOQUE_INICIAL = 3;

    // Inicializa o gerador de números aleatórios
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> capital_dist(MEDIA_CAPITAL, DESVIO_PADRAO_CAPITAL);
    std::normal_distribution<> salario_dist(MEDIA_SALARIO, DESVIO_PADRAO_SALARIO);
    std::normal_distribution<> estoque_dist(MEDIA_ESTOQUE_INICIAL, DESVIO_PADRAO_ESTOQUE_INICIAL);

    // Cria as firmas com valores aleatórios e as adiciona ao vetor
    for (int i = 0; i < NUM_FIRMAS; i++) {
        double capital_inicial = capital_dist(gen);
        double disposicao_salario = salario_dist(gen);
        int estoque_inicial = std::round(estoque_dist(gen));


        // Limita os valores para permanecerem dentro dos intervalos definidos
        if (disposicao_salario < 0) disposicao_salario = 0;
        if (capital_inicial < 0) capital_inicial = 0;

        // Cria uma nova firma com os valores sorteados
        auto nova_firma = std::make_shared<Firma>(capital_inicial, disposicao_salario, estoque_inicial);
        firmas.push_back(nova_firma);
    }
}

void cria_trabalhadores(std::vector<std::shared_ptr<Trabalhador>>& trabalhadores) {
    // Parâmetros para a criação dos trabalhadores
    const int NUM_TRABALHADORES = 5;
    const double MEDIA_PRODUTIVIDADE = 5.0; // Média para a produtividade
    const double DESVIO_PADRAO_PRODUTIVIDADE = 2.0; // Desvio padrão para a produtividade
    const double MEDIA_SALARIO = 30.0; // Média para o salário
    const double DESVIO_PADRAO_SALARIO = 10.0; // Desvio padrão para o salário
    const double MEDIA_RIQUEZA_INICIAL = 100.0;
    const double DESVIO_PADRAO_RIQUEZA_INICIAL = 30.0;
    const double MEDIA_DISPOSICAO_PRODUTO = 40;
    const double DESVIO_PADRAO_PRODUTO = 10;

    // Gerador de números aleatórios
    std::random_device rd;  // Obtém um gerador de números aleatórios
    std::mt19937 gen(rd()); // Semente com o gerador
    std::normal_distribution<> prod_dist(MEDIA_PRODUTIVIDADE, DESVIO_PADRAO_PRODUTIVIDADE); // Distribuição normal para produtividade
    std::normal_distribution<> salario_dist(MEDIA_SALARIO, DESVIO_PADRAO_SALARIO); // Distribuição normal para salário
    std::normal_distribution<> riqueza_inicial_dist(MEDIA_RIQUEZA_INICIAL,DESVIO_PADRAO_RIQUEZA_INICIAL);
    std::normal_distribution<> disposicao_produto_dist(MEDIA_DISPOSICAO_PRODUTO,DESVIO_PADRAO_PRODUTO);

    // Criação dos trabalhadores
    for (int i = 0; i < NUM_TRABALHADORES; ++i) {
        double salario = salario_dist(gen);
        int produtividade = static_cast<int>(prod_dist(gen)); // Converter para inteiro
        double riqueza_inicial = riqueza_inicial_dist(gen);
        double disposicao_produto = disposicao_produto_dist(gen);

        // Garantir que a produtividade e o salário sejam positivos
        if (salario < 0) salario = 0;
        if (produtividade < 0) produtividade = 0;
        if (riqueza_inicial < 0) riqueza_inicial = 0;
        if (disposicao_produto < 0) disposicao_produto = 0;

        auto novo_trabalhador = std::make_shared<Trabalhador>(salario, produtividade, riqueza_inicial, disposicao_produto);
        trabalhadores.push_back(novo_trabalhador);
    }
}

int main() {//________________________________________________________________________________________________
    // Cria vetores para armazenar Firmas e Trabalhadores
    std::vector<std::shared_ptr<Firma>> firmas;
    std::vector<std::shared_ptr<Trabalhador>> trabalhadores;

    cria_trabalhadores(trabalhadores);
    criar_firmas(firmas);
    
    // Inicializa o mercado
    Mercado mercado;

    mercado.adicionarFirmas(firmas);
    mercado.adicionarTrabalhadores(trabalhadores);

    //ITERACOES______________________________________________________________________________________________
    // Simulação de algumas iterações
    for (int iteracao = 0; iteracao < NUM_ITERACOES; iteracao++) {
        std::cout << "Iteracao " << iteracao + 1 << " ------------------------------------------" << std::endl;

        mercado.imprime_mercado();

        std::cout << "FIRMAS DEMITEM_____________________________________________________" << std::endl;
        mercado.firmas_demitem();

        std::cout << "FIRMAS CONTRATAM_____________________________________________________" << std::endl;
        // Firmas contratam trabalhadores
        mercado.firmas_contratam();

        std::cout << "TRABALHADORES CONSOMEM_____________________________________________________" << std::endl;
        // Trabalhadores consomem bens no mercado
        mercado.trabalhadores_consomem();

        std::cout << "FIRMAS PRODUZEM_____________________________________________________" << std::endl;
        // Firmas produzem
        mercado.firmas_produzem();

        std::cout << "FIRMAS PAGAM SALARIOS_____________________________________________________" << std::endl;
       //Firmas pagam salario
        mercado.firmas_pagam_salario();

        // Atualizar o mercado (por exemplo, ajustar os preços com base no estoque e demanda)
        //mercado atualiza os parametros dos agentes com base em suas interacoes

        //comportamentos esperados
        //trabalhadores com maior produtividade recebem salarios mais altos
        //empresas com trabalhadores produtivos vendem produtos mais baratos (homogeneos)
        
        std::cout << "Fim da iteracao " << iteracao + 1 << " -----------------------------------" << "\n" << std::endl;
    }

    return 0;
}