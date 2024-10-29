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
    const double AJUSTE_FIXO = 0.3; //PARAMETRO DE AJUSTE DE SALARIOS
    const double AJUSTE_FIXO_PRECO = 0.05;

public:
    Mercado() {} // Inicializa com valores padrão

    // Adicionar trabalhadores e Firmas ao mercado
    void adicionarTrabalhadores(std::vector<std::shared_ptr<Trabalhador>>& trabalhador) {
        trabalhadores_no_mercado = trabalhador;
    }
    void adicionarFirmas(std::vector<std::shared_ptr<Firma>>& firma) {
        firmas = firma;
    }

    void firmas_produzem(){
        for(auto &firma : firmas){
            firma->produzir();
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

    void imprime_mercado();

    std::vector<std::shared_ptr<Firma>>& firmas_mercado() { return firmas; }
    std::vector<std::shared_ptr<Trabalhador>>& trabalhadores_mercado() { return trabalhadores_no_mercado; }
};

void Mercado::firmas_contratam() {
    std::random_device rd; //definir seed para manter aleatoriedade
    std::mt19937 gen(rd());

    int contratados = 0;
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
            //std::cout << "Trabalhador contratado pela firma " << std::endl;

            // Ajustar a disposição do trabalhador (aumenta após ser contratado)
            trabalhador->set_disposicao_salario_incremento(AJUSTE_FIXO);

            // Ajustar a disposição da firma (diminui após contratar)
            firma->set_disposicao_salario_decremento(AJUSTE_FIXO);

            // Remover o trabalhador da lista de disponíveis
            trabalhadores_no_mercado.erase(trabalhadores_no_mercado.begin() + indiceTrabalhador);

            contratados++;
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
        if(firma->get_capital()<0){
            //demite o trabalhador menos produtivo
            auto trabalhador = firma->demite_menos_produtivo();
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
        while ((trabalhador->get_salario() > 0) && (num_tentativas < 1000)) { //PARAMETRO
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
                    trabalhador->consumir(preco_produto);
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
    //agora para os trabalhadores desempregados
    for (auto &trabalhador : trabalhadores_no_mercado) {
        num_tentativas = 0;

        while ((trabalhador->get_salario() > 0) && (num_tentativas < 5)) { //PARAMETRO
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
}

#endif // MERCADO_H