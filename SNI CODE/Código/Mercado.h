#ifndef MERCADO_H
#define MERCADO_H

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <cmath>
#include "Firma.h"
#include "Trabalhador.h"

//Funcao auxiliar para calculo do desvio padrao da multiplicacao de duas variaveis com distr normal
double calculaDesvioPadraoProduto(double mediaX, double mediaY, double desvioPadraoX, double desvioPadraoY) {
    // Variância é o quadrado do desvio padrão
    double varianciaX = desvioPadraoX * desvioPadraoX;
    double varianciaY = desvioPadraoY * desvioPadraoY;

    // Variância do produto
    double varianciaProduto = (mediaX * mediaX * varianciaY) + 
                              (mediaY * mediaY * varianciaX) + 
                              (varianciaX * varianciaY);

    // Desvio padrão é a raiz quadrada da variância
    return std::sqrt(varianciaProduto);
}

class Mercado {
private:
    //VETORES DE TRABALHADORES EMPREGADOS/DESEMPREGADOS E FIRMAS
    std::vector<std::shared_ptr<Trabalhador>> trabalhadores_no_mercado; // Trabalhadores disponíveis
    std::vector<std::shared_ptr<Trabalhador>> trabalhadores_empregados; // Trabalhadores empregados
    std::vector<std::shared_ptr<Firma>> firmas;           // Lista de Firmas

    //PARAMETROS DO MERCADO
    double AJUSTE_FIXO; //PARAMETRO DE AJUSTE DE SALARIOS
    double AJUSTE_FIXO_TRABALHADOR; //PARAMETRO DE AJUSTE DISPOSICAO SALARIO DO TRABALHADOR (INCREMENTO)
    double AJUSTE_FIXO_PRECO; //PARAMETRO AJUSTE PRECOS
    int TENTATIVAS_COMPRA_MAX; //QUANTIDADE DE TENTATIVAS DE COMPRA DE UM CONSUMIDOR POR ITERACAO
    int TENTATIVAS_CONTRATACAO_MAX; //QUANTIDADE DE TENTATIVAS DE CONTRATACAO DA EMPREDA NO MERCADO POR ITERACAO
    double AJUSTE_FIXO_ESTOQUES;

    //PARAMETROS DE PERCEPCAO DA FIRMA
    double MEDIA_PERCEPCAO;
    double DESVIO_PADRAO_PERCEPCAO;
    double VALOR_PERCEPCAO;

    //DADOS DO MERCADO EM CADA ITERACAO
    int contratados;
    int demitidos;
    int produzidos;
    int consumidos;
public:
    //FUNCOES GERAIS__________________________________________________________________________________
    Mercado(double _AJUSTE_FIXO, double _AJUSTE_FIXO_TRABALHADOR, double _AJUSTE_FIXO_PRECO,
    int _TENTATIVAS_COMPRA_MAX, int _TENTATIVAS_CONTRATACAO_MAX, double _AJUSTE_FIXO_ESTOQUES,
    double _MEDIA_PERCEPCAO, double _DESVIO_PADRAO_PERCEPCAO, double _VALOR_PERCEPCAO) { // Inicializa com valores padrão
        contratados = demitidos = produzidos = consumidos = 0;

        AJUSTE_FIXO = _AJUSTE_FIXO;
        AJUSTE_FIXO_TRABALHADOR = _AJUSTE_FIXO_TRABALHADOR;
        AJUSTE_FIXO_PRECO = _AJUSTE_FIXO_PRECO;
        TENTATIVAS_COMPRA_MAX = _TENTATIVAS_COMPRA_MAX;
        TENTATIVAS_CONTRATACAO_MAX = _TENTATIVAS_CONTRATACAO_MAX;
        AJUSTE_FIXO_ESTOQUES = _AJUSTE_FIXO_ESTOQUES;
        MEDIA_PERCEPCAO = _MEDIA_PERCEPCAO;
        DESVIO_PADRAO_PERCEPCAO = _DESVIO_PADRAO_PERCEPCAO;
        VALOR_PERCEPCAO = _VALOR_PERCEPCAO;
    }

    void reinicia_dados(){contratados = demitidos = produzidos = consumidos = 0;}

    //FIRMAS_________________________________________________________________________________________
    void adicionarFirmas(std::vector<std::shared_ptr<Firma>>& firma) {
        firmas = firma;
    }
    void firmas_produzem(){
        for(auto &firma : firmas){
            produzidos += firma->produzir();
        }
    }
    void firmas_pagam_salario(){
        for(auto &firma : firmas){
            firma->pagar_salarios();
        }
    }
    void firmas_contratam();
    void firmas_demitem_todos();

    //TRABALHADORES__________________________________________________________________________________________
    // Adicionar trabalhadores
    void adicionarTrabalhadores(std::vector<std::shared_ptr<Trabalhador>>& trabalhador) {
        trabalhadores_no_mercado = trabalhador;
    }
    void trabalhadores_consomem();

    //FUNCOES PARA FORNECER DADOS DO MERCADO__________________________________________________________________
    void imprime_mercado();
    std::vector<std::shared_ptr<Firma>>& firmas_mercado() { return firmas; }
    std::vector<std::shared_ptr<Trabalhador>>& trabalhadores_mercado() { return trabalhadores_no_mercado; }
    void imprime_mercado_csv(const std::string& nome_arquivo, int iteracao);

    //GETS
    int get_contratados() {return contratados;}
    int get_demitidos() {return demitidos;}
    int get_produzidos() {return produzidos;}
    int get_consumidos() {return consumidos;}
    double get_salario_medio(){
        double soma_salarios = 0.0;
        if (trabalhadores_empregados.empty()) return 0.0; // Evitar divisão por zero

        for (auto& trabalhador : trabalhadores_empregados) {
            soma_salarios += trabalhador->get_disposicao_salario(); // Supondo que Trabalhador tenha getSalario()
        }

        if(soma_salarios<0.0001) soma_salarios = 0;

        return soma_salarios / trabalhadores_empregados.size();
    }
    double get_salario_produtivo(){
        double soma_salarios = 0.0;
        for(auto &firma : firmas){
            soma_salarios += firma->get_salario_produtivo_medio();
        }

        if(soma_salarios<0.0001) soma_salarios = 0;

        return soma_salarios / firmas.size();
    }
    double get_salario_improdutivo(){
        double soma_salarios = 0.0;
        for(auto &firma : firmas){
            soma_salarios += firma->get_salario_improdutivo_medio();
        }

        if(soma_salarios<0.0001) soma_salarios = 0;

        return soma_salarios / firmas.size();
    }
    double get_preco_medio(){
        double soma_precos = 0.0;
        if (firmas.empty()) return 0.0; // Evitar divisão por zero

        for (auto& firma : firmas) {
            soma_precos += firma->get_preco_produto(); // Supondo que Bem tenha getPreco()
        }

        if(soma_precos<0.0001) soma_precos = 0;

        return soma_precos / firmas.size();
    }
    double get_capital_medio(){
        double soma_capitais = 0.0;
        if (firmas.empty()) return 0.0; // Evitar divisão por zero

        for (auto& firma : firmas) {
            soma_capitais += firma->get_capital(); // Supondo que Firma tenha getCapitalInicial()
        }

        if(soma_capitais<0.0001) soma_capitais = 0;

        return soma_capitais / firmas.size();
    }
    double get_riqueza_media(){
        double soma_riquezas = 0.0;

        for (auto& trabalhador : trabalhadores_empregados) {
            soma_riquezas += trabalhador->get_salario(); // Supondo que Trabalhador tenha getRiquezaInicial()
        }
        for (auto& trabalhador : trabalhadores_no_mercado) {
            soma_riquezas += trabalhador->get_salario(); // Supondo que Trabalhador tenha getRiquezaInicial()
        }

        if(soma_riquezas<0.0001) soma_riquezas = 0;

        return soma_riquezas / (trabalhadores_empregados.size()+trabalhadores_no_mercado.size());
    }
    int get_estoques(){
        int soma_estoques = 0;
        for (auto& firma : firmas) {
            soma_estoques += firma->get_estoque();
        }

        return soma_estoques;
    }
    double get_disposicao_produto_trabalhador_medio(){
        double soma_disposicao = 0.0;

        for (auto& trabalhador : trabalhadores_empregados) {
            soma_disposicao += trabalhador->get_disposicao_produto();
        }
        for (auto& trabalhador : trabalhadores_no_mercado) {
            soma_disposicao += trabalhador->get_disposicao_produto();
        }

        if(soma_disposicao<0.0001) soma_disposicao = 0;

        return soma_disposicao / (trabalhadores_empregados.size()+trabalhadores_no_mercado.size());
    }
    double get_percent_desempregados(){
        return double(trabalhadores_no_mercado.size())/(trabalhadores_empregados.size() + trabalhadores_no_mercado.size());
    }
    int get_empresas_ativas(){
        int ativas = 0;
        for(auto &firma : firmas){
            if(firma->get_capital()>0){
                ativas++;
            }
        }
        return ativas;
    }
    double get_base_monetaria(){
        double quantidade = 0;
        for(auto &firma : firmas){
            quantidade += firma->get_capital();
        }
        for(auto &trabalhador : trabalhadores_empregados){
            quantidade += trabalhador->get_salario();
        }
        for(auto &trabalhador : trabalhadores_no_mercado){
            quantidade += trabalhador->get_salario();
        }
        return quantidade;
    }
    int get_produtividade_total(){
        int produtividade = 0;
        for(auto &trabalhador : trabalhadores_empregados){
            produtividade += trabalhador->get_produtividade();
        }
        for(auto &trabalhador : trabalhadores_no_mercado){
            produtividade += trabalhador->get_produtividade();
        }
        return produtividade;
    }
};

//IMPLEMENTACOES________________________________________________________________________________________________
//FUNCAO PARA FIRMAS CONTRATAREM
void Mercado::firmas_contratam() {
    std::random_device rd; //definir seed para manter aleatoriedade
    std::mt19937 gen(rd());
    std::normal_distribution<> percepcao_dist(MEDIA_PERCEPCAO, DESVIO_PADRAO_PERCEPCAO); //variavel aleatoria de percepcao

    // Iterar sobre todas as firmas
    for (auto& firma : firmas) {
        for(int i=0;i<TENTATIVAS_CONTRATACAO_MAX;i++){
            // Verificar se há trabalhadores disponíveis
            if (trabalhadores_no_mercado.empty()) {
                //std::cout << "Nenhum trabalhador disponivel no mercado." << std::endl;
                return;
            }

            // Selecionar um trabalhador aleatório
            std::uniform_int_distribution<> distrib(0, trabalhadores_no_mercado.size() - 1);
            int indiceTrabalhador = distrib(gen);

            // Obter o trabalhador aleatório
            auto& trabalhador = trabalhadores_no_mercado[indiceTrabalhador];

            //apos a selecao do trabalhador no mercado a firma define se ele é ou nao produtivo
            double produtividade = percepcao_dist(gen)*trabalhador->get_produtividade();

            double ajuste_salario_produtivo = ((produtividade-VALOR_PERCEPCAO)/calculaDesvioPadraoProduto(50,MEDIA_PERCEPCAO,10,DESVIO_PADRAO_PERCEPCAO)); //O QUANTO O TRABALHADOR É ACIMA DA MEDIA
            
            if(ajuste_salario_produtivo>=0){ajuste_salario_produtivo = (ajuste_salario_produtivo+4)/4;}
            else{ajuste_salario_produtivo = (4+ajuste_salario_produtivo)/4;}

            if(produtividade >= VALOR_PERCEPCAO){
                //PRODUTIVO
                //POSSUEM AJUSTE NO SALARIO COM BASE NA PERCEPCAO DE PRODUTIVIDADE
                // Comparar a disposição do trabalhador com o salário oferecido pela firma
                if (trabalhador->get_disposicao_salario() <= firma->get_disposicao_salario_produtivo()*ajuste_salario_produtivo && firma->get_capital()>0) {
                    // O trabalhador aceita o salário e é contratado
                    firma->contratar_produtivo(trabalhador);
                    trabalhadores_empregados.push_back(trabalhador);
                    contratados++;
                    
                    trabalhador->set_salario_efetivo(firma->get_disposicao_salario_produtivo()*ajuste_salario_produtivo);

                    // Ajustar a disposição do trabalhador (aumenta após ser contratado)
                    trabalhador->set_disposicao_salario_incremento(AJUSTE_FIXO_TRABALHADOR);

                    // Ajustar a disposição da firma (diminui após contratar)
                    firma->set_disposicao_salario_decremento_produtivo(AJUSTE_FIXO);

                    // Remover o trabalhador da lista de disponíveis
                    trabalhadores_no_mercado.erase(trabalhadores_no_mercado.begin() + indiceTrabalhador);

                } else if(trabalhador->get_disposicao_salario() <= firma->get_disposicao_salario_produtivo()*ajuste_salario_produtivo && firma->get_capital()<=0){
                    // Ajustar a disposição da firma (diminui após nao ter dinheiro para contratar)
                    firma->set_disposicao_salario_decremento_produtivo(AJUSTE_FIXO);
                } else if(trabalhador->get_disposicao_salario() > firma->get_disposicao_salario_produtivo()*ajuste_salario_produtivo){
                    // Trabalhador rejeita o salário
                    // Ajustar a disposição do trabalhador (diminui após rejeitar oferta)
                    trabalhador->set_disposicao_salario_decremento(AJUSTE_FIXO);

                    // Ajustar a disposição da firma (aumenta após fracasso em contratação)
                    firma->set_disposicao_salario_incremento_produtivo(AJUSTE_FIXO);
                }
            }
            else{
                //NAO PRODUTIVO
                //NAO POSSUEM AJUSTE NO SALARIO, FIRMA APENAS OFERECE O SALARIO BASE
                // Comparar a disposição do trabalhador com o salário oferecido pela firma
                if (trabalhador->get_disposicao_salario() <= firma->get_disposicao_salario() && firma->get_capital()>0) {
                    // O trabalhador aceita o salário e é contratado
                    firma->contratar(trabalhador);
                    trabalhadores_empregados.push_back(trabalhador);
                    contratados++;
                    trabalhador->set_salario_efetivo(firma->get_disposicao_salario());

                    // Ajustar a disposição do trabalhador (aumenta após ser contratado)
                    trabalhador->set_disposicao_salario_incremento(AJUSTE_FIXO_TRABALHADOR);

                    // Ajustar a disposição da firma (diminui após contratar)
                    firma->set_disposicao_salario_decremento(AJUSTE_FIXO);

                    // Remover o trabalhador da lista de disponíveis
                    trabalhadores_no_mercado.erase(trabalhadores_no_mercado.begin() + indiceTrabalhador);

                } else if(trabalhador->get_disposicao_salario() <= firma->get_disposicao_salario() && firma->get_capital()<=0){
                    // Ajustar a disposição da firma (diminui após nao ter dinheiro para contratar)
                    firma->set_disposicao_salario_decremento(AJUSTE_FIXO);
                } else if(trabalhador->get_disposicao_salario() > firma->get_disposicao_salario()){
                    // Trabalhador rejeita o salário
                    // Ajustar a disposição do trabalhador (diminui após rejeitar oferta)
                    trabalhador->set_disposicao_salario_decremento(AJUSTE_FIXO);

                    // Ajustar a disposição da firma (aumenta após fracasso em contratação)
                    firma->set_disposicao_salario_incremento(AJUSTE_FIXO);
                }
            }
        }
    }
}

//FUNCAO PARA FIRMAS DEMITIREM TODOS OS FUNCIONARIOS EM CADA ITERACAO
void Mercado::firmas_demitem_todos(){
    for(auto &firma : firmas){
        while(firma->get_quantidade_trabalhadores()>0){
            //demite o trabalhador menos produtivo
            auto trabalhador = firma->demite_menos_produtivo();
            demitidos++;
            trabalhadores_no_mercado.push_back(trabalhador);
            
            trabalhadores_empregados.erase(
                std::remove_if(trabalhadores_empregados.begin(), trabalhadores_empregados.end(),
                            [&trabalhador](const std::shared_ptr<Trabalhador>& emp) {
                                return emp == trabalhador; // Compara os shared_ptr
                            }),
                trabalhadores_empregados.end()
            );
        }
    }
}

//FUNCAO PARA TRABALHADORES CONSUMIREM
void Mercado::trabalhadores_consomem(){
    // Gerador de números aleatórios para selecionar firmas aleatoriamente
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib_firma(0, firmas.size() - 1);

    int num_tentativas = 0; //numero de vezes que o agente tenta comprar mesmo que sua renda seja maior que zero
    //preco pode ser muito caro para a renda que sobrou ou nao existe estoque no mercado

    // Para cada trabalhador, eles tentam consumir até que sua renda acabe
    for (auto &trabalhador : trabalhadores_empregados) {
        num_tentativas = 0;
        while ((trabalhador->get_salario() > 0) && (num_tentativas < TENTATIVAS_COMPRA_MAX)) { //PARAMETRO
            // Seleciona uma firma aleatória
            auto &firma = firmas[distrib_firma(gen)];

            // Verifica se a firma tem estoque
            if (firma->get_estoque() > 0) {
                double preco_produto = firma->get_preco_produto();
                double disposicao_produto = trabalhador->get_disposicao_produto();

                // Verifica se o trabalhador tem disposição a pagar e se pode pagar o produto
                if (disposicao_produto >= preco_produto && trabalhador->get_salario() >= preco_produto) {
                    // O trabalhador compra o produto
                    trabalhador->consumir(preco_produto);
                    consumidos++;
                    firma->vender_produto(preco_produto);

                    // Ajusta o preço do produto e a disposição a pagar
                    // Firma aumenta a preco devido a demanda
                    firma->set_preco_produto(firma->get_preco_produto()+firma->get_preco_produto()*AJUSTE_FIXO_PRECO);
                    trabalhador->set_disposicao_produto(trabalhador->get_disposicao_produto()-firma->get_preco_produto()*AJUSTE_FIXO_PRECO);
                } else if(disposicao_produto < preco_produto && trabalhador->get_salario() >= preco_produto){
                    firma->set_preco_produto(firma->get_preco_produto()-firma->get_preco_produto()*AJUSTE_FIXO_ESTOQUES*(std::log(firma->get_estoque()>3?firma->get_estoque():3)));
                    trabalhador->set_disposicao_produto(trabalhador->get_disposicao_produto()+firma->get_preco_produto()*AJUSTE_FIXO_PRECO);
                }
                else{
                    // Se não pode comprar, o trabalhador tenta novamente até acabarem as tentativas
                    num_tentativas++;
                }
            } else {
                // Se a firma não tem estoque, tenta outra firma
                num_tentativas++;
                continue;
            }
        }
    }
    //agora para os trabalhadores desempregados
    for (auto &trabalhador : trabalhadores_no_mercado) {
        num_tentativas = 0;

        while ((trabalhador->get_salario() > 0) && (num_tentativas < TENTATIVAS_COMPRA_MAX)) {
            // Seleciona uma firma aleatória
            auto &firma = firmas[distrib_firma(gen)];

            // Verifica se a firma tem estoque
            if (firma->get_estoque() > 0) {
                double preco_produto = firma->get_preco_produto();
                double disposicao_produto = trabalhador->get_disposicao_produto();

                // Verifica se o trabalhador tem disposição a pagar e se pode pagar o produto
                if (disposicao_produto >= preco_produto && trabalhador->get_salario() >= preco_produto) {
                    // O trabalhador compra o produto
                    trabalhador->consumir(preco_produto);
                    consumidos++;
                    firma->vender_produto(preco_produto);

                    // Ajusta o preço do produto e a disposição a pagar
                    firma->set_preco_produto(firma->get_preco_produto()+firma->get_preco_produto()*AJUSTE_FIXO_PRECO);
                    trabalhador->set_disposicao_produto(trabalhador->get_disposicao_produto()-firma->get_preco_produto()*AJUSTE_FIXO_PRECO);
                } else if(disposicao_produto < preco_produto && trabalhador->get_salario() >= preco_produto){
                    firma->set_preco_produto(firma->get_preco_produto()-firma->get_preco_produto()*AJUSTE_FIXO_ESTOQUES*(std::log(firma->get_estoque()>3?firma->get_estoque():3)));
                    trabalhador->set_disposicao_produto(trabalhador->get_disposicao_produto()+firma->get_preco_produto()*AJUSTE_FIXO_PRECO);
                }
                else{
                    // Se não pode comprar, o trabalhador tenta novamente
                    num_tentativas++;
                }
            } else {
                // Se a firma não tem estoque, tenta outra firma
                num_tentativas++;
                continue;
            }
        }
    }
}

//FUNCAO PARA IMPRIMIR OS DADOS NO TERMINAL
void Mercado::imprime_mercado(){
    //imprime trabalhadores e firmas
    std::cout << "TRABALHADORES EMPREGADOS_____________________________________________________" << std::endl;
    //agora todos os trabalhadores estao no vector todos_trabalhadores
    for(auto &trabalhador : trabalhadores_empregados){
        trabalhador->imprime();
    }
    std::cout << "TRABALHADORES DESEMPREGADOS_____________________________________________________" << std::endl;
    for(auto &trabalhador : trabalhadores_no_mercado){
        trabalhador->imprime();
    }
    std::cout << "FIRMAS_____________________________________________________" << std::endl;
    for(auto &firma : firmas){
        firma->imprime();
    }
    std::cout << "DADOS_______________________________________________________" << std::endl;
    std::cout << "contratados: " << contratados << std::endl;
    std::cout << "demitidos: " << demitidos << std::endl;
    std::cout << "produzidos: " << produzidos << std::endl;
    std::cout << "consumidos: " << consumidos << std::endl;
}

void Mercado::imprime_mercado_csv(const std::string& nome_arquivo, int iteracao) {
    std::ofstream arquivo(nome_arquivo, std::ios::app);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para escrita.\n";
        return;
    }
    // Escrever salário médio e preço médio
    arquivo << iteracao  << ',' << get_salario_medio() << ',' << get_preco_medio() << ',' << get_produzidos() << ',' <<
    get_consumidos() << ',' << get_contratados() << ',' << get_demitidos() << ',' << get_estoques() << ',' <<
    get_capital_medio() << ',' << get_riqueza_media() << ',' << get_disposicao_produto_trabalhador_medio() << ',' 
    << get_percent_desempregados() << ',' << get_base_monetaria() << ',' << get_empresas_ativas() <<
    ',' << get_produtividade_total() << ',' << get_salario_improdutivo() << ',' << get_salario_produtivo() << '\n';

    arquivo.flush(); //forca a escrita no arquivo???
    arquivo.close();
}

#endif