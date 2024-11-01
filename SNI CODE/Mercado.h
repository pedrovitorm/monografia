#ifndef MERCADO_H
#define MERCADO_H

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <random>
#include "Firma.h"
#include "Trabalhador.h"

class Mercado {
private:
    std::vector<std::shared_ptr<Trabalhador>> trabalhadores_no_mercado; // Trabalhadores disponíveis
    std::vector<std::shared_ptr<Trabalhador>> trabalhadores_empregados; // Trabalhadores empregados
    std::vector<std::shared_ptr<Firma>> firmas;           // Lista de Firmas

    //PARAMETROS DO MERCADO
    const double AJUSTE_FIXO = 0.2; //PARAMETRO DE AJUSTE DE SALARIOS
    const double AJUSTE_FIXO_PRECO = 0.05; //PARAMETRO AJUSTE PRECOS
    const int ESTOQUE_MAX = 100; //QUANTIDADE DE ESTOQUE MAXIMO ATE A FIRMA DEMITIR UM TRABALHADOR
    const int TENTATIVAS_COMPRA_MAX = 1000; //QUANTIDADE DE TENTATIVAS DE COMPRA DE UM CONSUMIDOR POR ITERACAO

    //DADOS DO MERCADO EM CADA ITERACAO
    int contratados;
    int demitidos;
    int produzidos;
    int consumidos;

public:
    Mercado() {contratados = demitidos = produzidos = consumidos = 0;} // Inicializa com valores padrão

    void reinicia_dados(){contratados = demitidos = produzidos = consumidos = 0;}

    // Adicionar trabalhadores e Firmas ao mercado
    void adicionarTrabalhadores(std::vector<std::shared_ptr<Trabalhador>>& trabalhador) {
        trabalhadores_no_mercado = trabalhador;
    }
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
    void firmas_demitem();

    void trabalhadores_consomem();

    void trabalhadores_se_demitem();

    void imprime_mercado();


    std::vector<std::shared_ptr<Firma>>& firmas_mercado() { return firmas; }
    std::vector<std::shared_ptr<Trabalhador>>& trabalhadores_mercado() { return trabalhadores_no_mercado; }
};

void Mercado::firmas_contratam() {
    std::random_device rd; //definir seed para manter aleatoriedade
    std::mt19937 gen(rd());

    int recusados = 0;

    // Iterar sobre todas as firmas
    for (auto& firma : firmas) {
        // Verificar se há trabalhadores disponíveis
        if (trabalhadores_no_mercado.empty()) {
            //std::cout << "Nenhum trabalhador disponivel no mercado." << std::endl;
            return;
        }

        // Selecionar um trabalhador aleatório
        std::uniform_int_distribution<> distrib(0, trabalhadores_no_mercado.size() - 1);
        int indiceTrabalhador = distrib(gen);

        //std::cout << "Tamanho antes da contratacao/recusa: " << trabalhadores_no_mercado.size() << std::endl;

        // Obter o trabalhador aleatório
        auto& trabalhador = trabalhadores_no_mercado[indiceTrabalhador];

        // Comparar a disposição do trabalhador com o salário oferecido pela firma
        if (trabalhador->get_disposicao_salario() <= firma->get_disposicao_salario() && firma->get_capital()>0) {
            // O trabalhador aceita o salário e é contratado
            firma->contratar(trabalhador);
            trabalhadores_empregados.push_back(trabalhador);
            contratados++;
            //std::cout << "Trabalhador contratado pela firma " << std::endl;

            // Ajustar a disposição do trabalhador (aumenta após ser contratado)
            trabalhador->set_disposicao_salario_incremento(AJUSTE_FIXO);
            trabalhador->reinicia_ano_empresa();

            // Ajustar a disposição da firma (diminui após contratar)
            firma->set_disposicao_salario_decremento(AJUSTE_FIXO);

            // Remover o trabalhador da lista de disponíveis
            trabalhadores_no_mercado.erase(trabalhadores_no_mercado.begin() + indiceTrabalhador);

        } else if(trabalhador->get_disposicao_salario() <= firma->get_disposicao_salario() && firma->get_capital()<=0){
            // Ajustar a disposição da firma (diminui após nao ter dinheiro para contratar)
            firma->set_disposicao_salario_decremento(AJUSTE_FIXO);
        } else if(trabalhador->get_disposicao_salario() > firma->get_disposicao_salario()){
            // Trabalhador rejeita o salário
            //std::cout << "Trabalhador recusou a oferta da firma " << std::endl;

            // Ajustar a disposição do trabalhador (diminui após rejeitar oferta)
            trabalhador->set_disposicao_salario_decremento(AJUSTE_FIXO);

            // Ajustar a disposição da firma (aumenta após fracasso em contratação)
            firma->set_disposicao_salario_incremento(AJUSTE_FIXO);

            recusados++;
        }
    }
    //std::cout << "CONTRATADOS: " << contratados << " | RECUSADOS: " << recusados << std::endl;
}

void Mercado::firmas_demitem(){
    for(auto &firma : firmas){

        //se nao tiver trabalhadores nao faz nada
        if(firma->get_quantidade_trabalhadores()==0){
            return;
        }

        //se o capital for menor que zero ela demite um trabalhador e diminui a disposicao salarial
        if(firma->get_capital()<0/* ||firma->get_estoque()>ESTOQUE_MAX */){
            //demite o trabalhador menos produtivo
            auto trabalhador = firma->demite_menos_produtivo();
            demitidos++;
            trabalhador->set_disposicao_salario_decremento(AJUSTE_FIXO);
            trabalhadores_no_mercado.push_back(trabalhador);
            
            trabalhadores_empregados.erase(
                std::remove_if(trabalhadores_empregados.begin(), trabalhadores_empregados.end(),
                            [&trabalhador](const std::shared_ptr<Trabalhador>& emp) {
                                return emp == trabalhador; // Compara os shared_ptr
                            }),
                trabalhadores_empregados.end()
            );

            //diminui disposicao salarial
            firma->set_disposicao_salario_decremento(AJUSTE_FIXO);
        }
    }
}

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
        while ((trabalhador->get_produtod_consumidos() < 10) && (num_tentativas < TENTATIVAS_COMPRA_MAX)) { //PARAMETRO
            // Seleciona uma firma aleatória
            auto &firma = firmas[distrib_firma(gen)];

            /* std::cout << "Trabalhador empregado tentando comprar: " 
            << "Salario: " << trabalhador->get_salario() 
            << ", Disposicao: " << trabalhador->get_disposicao_produto() 
            << ", Preco: " << firma->get_preco_produto() 
            << ", Estoque: " << firma->get_estoque() << std::endl; */

            // Verifica se a firma tem estoque
            if (firma->get_estoque() > 0) {
                double preco_produto = firma->get_preco_produto();
                double disposicao_produto = trabalhador->get_disposicao_produto();

                // Verifica se o trabalhador tem disposição a pagar e se pode pagar o produto
                if (disposicao_produto >= preco_produto && trabalhador->get_salario() >= preco_produto) {
                    // O trabalhador compra o produto

                    std::cout << "Trabalhador empregado comprou: " 
                    << "Salario: " << trabalhador->get_salario() 
                    << ", Disposicao: " << trabalhador->get_disposicao_produto() 
                    << ", Preco: " << firma->get_preco_produto() 
                    << ", Estoque: " << firma->get_estoque() << std::endl;

                    trabalhador->consumir(preco_produto);
                    consumidos++;
                    firma->vender_produto(preco_produto);

                    // Ajusta o preço do produto e a disposição a pagar
                    firma->set_preco_produto(firma->get_preco_produto()+firma->get_preco_produto()*AJUSTE_FIXO_PRECO);
                    trabalhador->set_disposicao_produto(trabalhador->get_disposicao_produto()-firma->get_preco_produto()*AJUSTE_FIXO_PRECO);
                } else if(disposicao_produto < preco_produto && trabalhador->get_salario() >= preco_produto){
                    firma->set_preco_produto(firma->get_preco_produto()-firma->get_preco_produto()*AJUSTE_FIXO_PRECO*(firma->get_estoque()/100));
                    trabalhador->set_disposicao_produto(trabalhador->get_disposicao_produto()+firma->get_preco_produto()*AJUSTE_FIXO_PRECO);
                }
                else{
                    // Se não pode comprar, o trabalhador para de tentar consumir
                    //break;
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

        while ((trabalhador->get_salario() > 0) && (num_tentativas < TENTATIVAS_COMPRA_MAX)) { //PARAMETRO
            // Seleciona uma firma aleatória
            auto &firma = firmas[distrib_firma(gen)];

            /* std::cout << "Trabalhador desempregado tentando comprar: " 
            << "Salario: " << trabalhador->get_salario() 
            << ", Disposicao: " << trabalhador->get_disposicao_produto() 
            << ", Preco: " << firma->get_preco_produto() 
            << ", Estoque: " << firma->get_estoque() << std::endl; */

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
                    firma->set_preco_produto(firma->get_preco_produto()-firma->get_preco_produto()*AJUSTE_FIXO_PRECO);
                    trabalhador->set_disposicao_produto(trabalhador->get_disposicao_produto()+firma->get_preco_produto()*AJUSTE_FIXO_PRECO);
                }
                else{
                    // Se não pode comprar, o trabalhador para de tentar consumir
                    //break;
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

void Mercado::trabalhadores_se_demitem(){
    for (size_t i = 0; i < trabalhadores_empregados.size(); i++) {
        auto& trabalhador = trabalhadores_empregados[i];

        // Verifica se o trabalhador quer se demitir
        if (trabalhador->get_quer_se_demitir()) {
            // Remove o trabalhador da firma
            for (auto& firma : firmas) {
                std::cout << "entrei" << std::endl;
                firma->demite(trabalhador);
                demitidos++;
            }

            // Adiciona o trabalhador ao vetor `trabalhadores_no_mercado`
            trabalhadores_no_mercado.push_back(trabalhador);

            // Remove o trabalhador de `trabalhadores_empregados` e ajusta o índice
            trabalhadores_empregados.erase(trabalhadores_empregados.begin() + i);
        }
    }
}

void Mercado::imprime_mercado(){
    //imprime trabalhadores e firmas
    std::cout << "TRABALHADORES EMPREGADOS_____________________________________________________" << std::endl;
    //agora todos os trabalhadores estao no vector todos_trabalhadores
    for(auto &trabalhador : trabalhadores_empregados){
        trabalhador->imprime();
        trabalhador->reinicia_consumidos();//ARRUMAR!!!
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

    reinicia_dados();
}

#endif // MERCADO_H