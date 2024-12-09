#include <iostream>
#include <vector>
#include <memory> //smart pointers
#include <random>
#include <ctime> //tempo do pc para funcao random
#include <fstream> //escrito no .csv
#include <string>
#include <sstream> //uso de strings
#include <chrono> // Para usar o sleep
#include <thread> // Para usar o sleep_for
#include <unordered_map>
#include "Firma.h"
#include "Trabalhador.h"
#include "Mercado.h"
#include "Consumidor.h" //ja incluso em Trabalhador e Pesquisador

//Agentes ainda em implementacao
//#include "Universidade.h"
//#include "Pesquisador.h"
//#include "Governo.h"
//#include "Banco.h"

//PARAMETROS GERAIS
bool IMPRIME_TERMINAL;
bool IMPRIME_CSV;
int NUM_ITERACOES;
int TEMPO_ATUALIZACAO; //tempo de atualizacao da simulacao em microsegundos

//PARAMETROS FIRMAS
int NUM_FIRMAS;
double MEDIA_CAPITAL; // Média para o capital
double DESVIO_PADRAO_CAPITAL; // Desvio padrão para o capital
double MEDIA_SALARIO_FIRMAS; // Média para o salário
double DESVIO_PADRAO_SALARIO_FIRMAS; // Desvio padrão para o salário
int MEDIA_ESTOQUE_INICIAL;
int DESVIO_PADRAO_ESTOQUE_INICIAL;
double MEDIA_PRECO_INICIAL;
double DESVIO_PADRAO_PRECO_INICIAL;

// Parâmetros para a criação dos trabalhadores
int NUM_TRABALHADORES;
double MEDIA_PRODUTIVIDADE; // Média para a produtividade
double DESVIO_PADRAO_PRODUTIVIDADE; // Desvio padrão para a produtividade
double MEDIA_SALARIO_TRABALHADOR; // Média para o salário
double DESVIO_PADRAO_SALARIO_TRABALHADOR; // Desvio padrão para o salário
double MEDIA_RIQUEZA_INICIAL;
double DESVIO_PADRAO_RIQUEZA_INICIAL;
double MEDIA_DISPOSICAO_PRODUTO;
double DESVIO_PADRAO_PRODUTO;

//PARAMETROS DO MERCADO
double AJUSTE_FIXO; //PARAMETRO DE AJUSTE DE SALARIOS
double AJUSTE_FIXO_TRABALHADOR; //PARAMETRO DE AJUSTE DISPOSICAO SALARIO DO TRABALHADOR (INCREMENTO)
double AJUSTE_FIXO_PRECO; //PARAMETRO AJUSTE PRECOS
int TENTATIVAS_COMPRA_MAX; //QUANTIDADE DE TENTATIVAS DE COMPRA DE UM CONSUMIDOR POR ITERACAO
int TENTATIVAS_CONTRATACAO_MAX; //QUANTIDADE DE TENTATIVAS DE CONTRATACAO DA EMPREDA NO MERCADO POR ITERACAO
double AJUSTE_FIXO_ESTOQUES; //SENSIBILIDADE DO AJUSTE DE PRECOS DA FIRMA EM RELACAO A QUANTIDADE ESTOCADA

//FUNCOES AUXILIARES___________________________________________________________________________________________
//Funcao para imprimir parametros no terminal
void imprimirParametros() {
    // Imprime os parâmetros gerais
    std::cout << "// PARAMETROS GERAIS\n";
    std::cout << "IMPRIME_TERMINAL: " << (IMPRIME_TERMINAL ? "true" : "false") << "\n";
    std::cout << "IMPRIME_CSV: " << (IMPRIME_CSV ? "true" : "false") << "\n";
    std::cout << "NUM_ITERACOES: " << NUM_ITERACOES << "\n";
    std::cout << "TEMPO_ATUALIZACAO: " << TEMPO_ATUALIZACAO << " ms\n";

    // Imprime os parâmetros das firmas
    std::cout << "\n// PARAMETROS FIRMAS\n";
    std::cout << "NUM_FIRMAS: " << NUM_FIRMAS << "\n";
    std::cout << "MEDIA_CAPITAL: " << MEDIA_CAPITAL << "\n";
    std::cout << "DESVIO_PADRAO_CAPITAL: " << DESVIO_PADRAO_CAPITAL << "\n";
    std::cout << "MEDIA_SALARIO_FIRMAS: " << MEDIA_SALARIO_FIRMAS << "\n";
    std::cout << "DESVIO_PADRAO_SALARIO_FIRMAS: " << DESVIO_PADRAO_SALARIO_FIRMAS << "\n";
    std::cout << "MEDIA_ESTOQUE_INICIAL: " << MEDIA_ESTOQUE_INICIAL << "\n";
    std::cout << "DESVIO_PADRAO_ESTOQUE_INICIAL: " << DESVIO_PADRAO_ESTOQUE_INICIAL << "\n";

    // Imprime os parâmetros dos trabalhadores
    std::cout << "\n// PARAMETROS TRABALHADORES\n";
    std::cout << "NUM_TRABALHADORES: " << NUM_TRABALHADORES << "\n";
    std::cout << "MEDIA_PRODUTIVIDADE: " << MEDIA_PRODUTIVIDADE << "\n";
    std::cout << "DESVIO_PADRAO_PRODUTIVIDADE: " << DESVIO_PADRAO_PRODUTIVIDADE << "\n";
    std::cout << "MEDIA_SALARIO_TRABALHADOR: " << MEDIA_SALARIO_TRABALHADOR << "\n";
    std::cout << "DESVIO_PADRAO_SALARIO_TRABALHADOR: " << DESVIO_PADRAO_SALARIO_TRABALHADOR << "\n";
    std::cout << "MEDIA_RIQUEZA_INICIAL: " << MEDIA_RIQUEZA_INICIAL << "\n";
    std::cout << "DESVIO_PADRAO_RIQUEZA_INICIAL: " << DESVIO_PADRAO_RIQUEZA_INICIAL << "\n";
    std::cout << "MEDIA_DISPOSICAO_PRODUTO: " << MEDIA_DISPOSICAO_PRODUTO << "\n";
    std::cout << "DESVIO_PADRAO_PRODUTO: " << DESVIO_PADRAO_PRODUTO << "\n";
}

// Função para ler o arquivo de configuração e atribuir os valores às variáveis globais
void lerConfiguracoes(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de configurações: " << nomeArquivo << std::endl;
        return;
    }

    std::unordered_map<std::string, std::string> configuracoes;
    std::string linha;

    // Lê linha por linha e armazena os pares chave-valor no mapa
    while (std::getline(arquivo, linha)) {
        std::istringstream iss(linha);
        std::string chave, valor;

        // Pula linhas vazias
        if (linha.empty()) continue;

        // Lê a chave e o valor
        iss >> chave >> valor;

        // Adiciona ao mapa
        configuracoes[chave] = valor;
    }

    arquivo.close();

    // Atribui os valores do mapa às variáveis globais
    if (configuracoes.find("IMPRIME_TERMINAL") != configuracoes.end())
        IMPRIME_TERMINAL = std::stoi(configuracoes["IMPRIME_TERMINAL"]);
    if (configuracoes.find("IMPRIME_CSV") != configuracoes.end())
        IMPRIME_CSV = std::stoi(configuracoes["IMPRIME_CSV"]);

    if (configuracoes.find("NUM_ITERACOES") != configuracoes.end())
        NUM_ITERACOES = std::stoi(configuracoes["NUM_ITERACOES"]);
    if (configuracoes.find("TEMPO_ATUALIZACAO") != configuracoes.end())
        TEMPO_ATUALIZACAO = std::stoi(configuracoes["TEMPO_ATUALIZACAO"]);

    if (configuracoes.find("NUM_FIRMAS") != configuracoes.end())
        NUM_FIRMAS = std::stoi(configuracoes["NUM_FIRMAS"]);
    if (configuracoes.find("MEDIA_CAPITAL") != configuracoes.end())
        MEDIA_CAPITAL = std::stod(configuracoes["MEDIA_CAPITAL"]);
    if (configuracoes.find("DESVIO_PADRAO_CAPITAL") != configuracoes.end())
        DESVIO_PADRAO_CAPITAL = std::stod(configuracoes["DESVIO_PADRAO_CAPITAL"]);
    if (configuracoes.find("MEDIA_SALARIO_FIRMAS") != configuracoes.end())
        MEDIA_SALARIO_FIRMAS = std::stod(configuracoes["MEDIA_SALARIO_FIRMAS"]);
    if (configuracoes.find("DESVIO_PADRAO_SALARIO_FIRMAS") != configuracoes.end())
        DESVIO_PADRAO_SALARIO_FIRMAS = std::stod(configuracoes["DESVIO_PADRAO_SALARIO_FIRMAS"]);
    if (configuracoes.find("MEDIA_ESTOQUE_INICIAL") != configuracoes.end())
        MEDIA_ESTOQUE_INICIAL = std::stoi(configuracoes["MEDIA_ESTOQUE_INICIAL"]);
    if (configuracoes.find("DESVIO_PADRAO_ESTOQUE_INICIAL") != configuracoes.end())
        DESVIO_PADRAO_ESTOQUE_INICIAL = std::stoi(configuracoes["DESVIO_PADRAO_ESTOQUE_INICIAL"]);
    if (configuracoes.find("MEDIA_PRECO_INICIAL") != configuracoes.end())
        MEDIA_PRECO_INICIAL = std::stoi(configuracoes["MEDIA_PRECO_INICIAL"]);
    if (configuracoes.find("DESVIO_PADRAO_PRECO_INICIAL") != configuracoes.end())
        DESVIO_PADRAO_PRECO_INICIAL = std::stoi(configuracoes["DESVIO_PADRAO_PRECO_INICIAL"]);

    if (configuracoes.find("NUM_TRABALHADORES") != configuracoes.end())
        NUM_TRABALHADORES = std::stoi(configuracoes["NUM_TRABALHADORES"]);
    if (configuracoes.find("MEDIA_PRODUTIVIDADE") != configuracoes.end())
        MEDIA_PRODUTIVIDADE = std::stod(configuracoes["MEDIA_PRODUTIVIDADE"]);
    if (configuracoes.find("DESVIO_PADRAO_PRODUTIVIDADE") != configuracoes.end())
        DESVIO_PADRAO_PRODUTIVIDADE = std::stod(configuracoes["DESVIO_PADRAO_PRODUTIVIDADE"]);
    if (configuracoes.find("MEDIA_SALARIO_TRABALHADOR") != configuracoes.end())
        MEDIA_SALARIO_TRABALHADOR = std::stod(configuracoes["MEDIA_SALARIO_TRABALHADOR"]);
    if (configuracoes.find("DESVIO_PADRAO_SALARIO_TRABALHADOR") != configuracoes.end())
        DESVIO_PADRAO_SALARIO_TRABALHADOR = std::stod(configuracoes["DESVIO_PADRAO_SALARIO_TRABALHADOR"]);
    if (configuracoes.find("MEDIA_RIQUEZA_INICIAL") != configuracoes.end())
        MEDIA_RIQUEZA_INICIAL = std::stod(configuracoes["MEDIA_RIQUEZA_INICIAL"]);
    if (configuracoes.find("DESVIO_PADRAO_RIQUEZA_INICIAL") != configuracoes.end())
        DESVIO_PADRAO_RIQUEZA_INICIAL = std::stod(configuracoes["DESVIO_PADRAO_RIQUEZA_INICIAL"]);
    if (configuracoes.find("MEDIA_DISPOSICAO_PRODUTO") != configuracoes.end())
        MEDIA_DISPOSICAO_PRODUTO = std::stod(configuracoes["MEDIA_DISPOSICAO_PRODUTO"]);
    if (configuracoes.find("DESVIO_PADRAO_PRODUTO") != configuracoes.end())
        DESVIO_PADRAO_PRODUTO = std::stod(configuracoes["DESVIO_PADRAO_PRODUTO"]);

    if (configuracoes.find("AJUSTE_FIXO") != configuracoes.end())
        AJUSTE_FIXO = std::stod(configuracoes["AJUSTE_FIXO"]);
    if (configuracoes.find("AJUSTE_FIXO_TRABALHADOR") != configuracoes.end())
        AJUSTE_FIXO_TRABALHADOR = std::stod(configuracoes["AJUSTE_FIXO_TRABALHADOR"]);
    if (configuracoes.find("AJUSTE_FIXO_PRECO") != configuracoes.end())
        AJUSTE_FIXO_PRECO = std::stod(configuracoes["AJUSTE_FIXO_PRECO"]);
    if (configuracoes.find("TENTATIVAS_COMPRA_MAX") != configuracoes.end())
        TENTATIVAS_COMPRA_MAX = std::stod(configuracoes["TENTATIVAS_COMPRA_MAX"]);
    if (configuracoes.find("TENTATIVAS_CONTRATACAO_MAX") != configuracoes.end())
        TENTATIVAS_CONTRATACAO_MAX = std::stod(configuracoes["TENTATIVAS_CONTRATACAO_MAX"]);
    if (configuracoes.find("AJUSTE_FIXO_ESTOQUES") != configuracoes.end())
        AJUSTE_FIXO_ESTOQUES = std::stod(configuracoes["AJUSTE_FIXO_ESTOQUES"]);
}

// Função para criar as firmas com valores aleatórios com distribuição normal
void criar_firmas(std::vector<std::shared_ptr<Firma>>& firmas) {

    // Inicializa o gerador de números aleatórios
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> capital_dist(MEDIA_CAPITAL, DESVIO_PADRAO_CAPITAL);
    std::normal_distribution<> salario_dist(MEDIA_SALARIO_FIRMAS, DESVIO_PADRAO_SALARIO_FIRMAS);
    std::normal_distribution<> estoque_dist(MEDIA_ESTOQUE_INICIAL, DESVIO_PADRAO_ESTOQUE_INICIAL);
    std::normal_distribution<> preco_dist(MEDIA_PRECO_INICIAL, DESVIO_PADRAO_PRECO_INICIAL);


    // Cria as firmas com valores aleatórios e as adiciona ao vetor
    for (int i = 0; i < NUM_FIRMAS; i++) {
        double capital_inicial = capital_dist(gen);
        double disposicao_salario = salario_dist(gen);
        int estoque_inicial = std::round(estoque_dist(gen));
        double preco_inicial = preco_dist(gen);


        // Limita os valores para permanecerem dentro dos intervalos definidos
        if (disposicao_salario < 0) disposicao_salario = 0;
        if (capital_inicial < 0) capital_inicial = 0;

        // Cria uma nova firma com os valores sorteados
        auto nova_firma = std::make_shared<Firma>(capital_inicial, disposicao_salario, estoque_inicial, preco_inicial);
        firmas.push_back(nova_firma);
    }
}

void cria_trabalhadores(std::vector<std::shared_ptr<Trabalhador>>& trabalhadores) {

    // Gerador de números aleatórios
    std::random_device rd;  // Obtém um gerador de números aleatórios
    std::mt19937 gen(rd()); // Semente com o gerador
    std::normal_distribution<> prod_dist(MEDIA_PRODUTIVIDADE, DESVIO_PADRAO_PRODUTIVIDADE); // Distribuição normal para produtividade
    std::normal_distribution<> salario_dist(MEDIA_SALARIO_TRABALHADOR, DESVIO_PADRAO_SALARIO_TRABALHADOR); // Distribuição normal para salário
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

// Função para adicionar o cabeçalho após a simulação
void adicionarCabecalho(const std::string& nome_arquivo) {
    // Abrir o arquivo em modo de leitura
    std::ifstream arquivoEntrada(nome_arquivo);
    if (!arquivoEntrada.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para leitura.\n";
        return;
    }

    // Carregar o conteúdo do arquivo em uma string temporária
    std::ostringstream conteudo;
    conteudo << arquivoEntrada.rdbuf();  // Lê o conteúdo completo para a string
    arquivoEntrada.close();

    // Abrir o arquivo novamente em modo de escrita para sobrescrever
    std::ofstream arquivoSaida(nome_arquivo);
    if (!arquivoSaida.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para escrita.\n";
        return;
    }

    // Escrever o cabeçalho
    arquivoSaida << "iteracao,salario_medio,preco_medio,produzidos,consumidos,contratados,demitidos,estoques,capital medio,riqueza media,media_disp_prod_trab,desemprego_percent,base_monetaria,empresas_ativas,produtividade_total,salario_improdutivo,salario_produtivo\n";

    // Escrever o conteúdo original de volta ao arquivo
    arquivoSaida << conteudo.str();
    arquivoSaida.close();
    std::cout << "Cabecalho adicionado com sucesso no arquivo " << nome_arquivo << ".\n";
}

int main() {//________________________________________________________________________________________________
    //Le parametros de config.txt
    lerConfiguracoes("config.txt");

    //imprime parametros no terminal
    if(IMPRIME_TERMINAL) imprimirParametros();
    
    // Cria vetores para armazenar Firmas e Trabalhadores
    std::vector<std::shared_ptr<Firma>> firmas;
    std::vector<std::shared_ptr<Trabalhador>> trabalhadores;

    cria_trabalhadores(trabalhadores);
    criar_firmas(firmas);
    
    // Inicializa o mercado
    Mercado mercado(AJUSTE_FIXO, AJUSTE_FIXO_TRABALHADOR, AJUSTE_FIXO_PRECO, TENTATIVAS_COMPRA_MAX,
    TENTATIVAS_CONTRATACAO_MAX, AJUSTE_FIXO_ESTOQUES);

    mercado.adicionarFirmas(firmas);
    mercado.adicionarTrabalhadores(trabalhadores);

    //Arquivo de saida
    // Abrir o arquivo CSV para escrita
    std::ofstream outFile("mercado.csv");

    // Verificar se o arquivo foi aberto corretamente
    if (!outFile) {
        std::cerr << "Não foi possível abrir o arquivo!" << std::endl;
        return 1;
    }

    //ITERACOES______________________________________________________________________________________________
    // Simulação de algumas iterações
    for (int iteracao = 0; iteracao < NUM_ITERACOES; iteracao++) {
        if(IMPRIME_TERMINAL) std::cout << "Iteracao " << iteracao + 1 << " ------------------------------------------" << std::endl;

        if(IMPRIME_TERMINAL) std::cout << "FIRMAS DEMITEM_____________________________________________________" << std::endl;
        mercado.firmas_demitem_todos();

        if(IMPRIME_TERMINAL) std::cout << "FIRMAS CONTRATAM_____________________________________________________" << std::endl;
        // Firmas contratam trabalhadores
        mercado.firmas_contratam();

        if(IMPRIME_TERMINAL) std::cout << "TRABALHADORES CONSOMEM_____________________________________________________" << std::endl;
        // Trabalhadores consomem bens no mercado
        mercado.trabalhadores_consomem();

        //std::cout << "TRABALHADORES SE DEMITEM_____________________________________________________" << std::endl;
        // Trabalhadores saem das firmas se seu consumo for 0
        //mercado.trabalhadores_se_demitem();

        if(IMPRIME_TERMINAL) std::cout << "FIRMAS PRODUZEM_____________________________________________________" << std::endl;
        // Firmas produzem
        mercado.firmas_produzem();

        if(IMPRIME_TERMINAL) std::cout << "FIRMAS PAGAM SALARIOS_____________________________________________________" << std::endl;
        //Firmas pagam salario
        mercado.firmas_pagam_salario();

        if(IMPRIME_CSV) mercado.imprime_mercado_csv("mercado.csv",iteracao);
        //mercado.imprime_mercado();
        mercado.reinicia_dados(); //nao tire essa funcao para nao acumular os dados
        
        std::cout << "iteracao: " << iteracao + 1 << "\n";
    
        // Pausar a execução
        std::this_thread::sleep_for(std::chrono::microseconds(TEMPO_ATUALIZACAO));

        if(IMPRIME_TERMINAL) mercado.imprime_mercado();

        if(IMPRIME_TERMINAL) std::cout << "FIM ITERACAO_____________________________________________________" << std::endl;
    }

    // Adiciona o cabeçalho ao final de todas as iterações
    if(IMPRIME_CSV) adicionarCabecalho("mercado.csv");

    return 0;
}