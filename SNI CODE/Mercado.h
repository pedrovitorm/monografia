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

    //DADOS DO MERCADO EM CADA ITERACAO
    int contratados;
    int demitidos;
    int produzidos;
    int consumidos;

public:

    //FUNCOES GERAIS__________________________________________________________________________________
    Mercado(double _AJUSTE_FIXO, double _AJUSTE_FIXO_TRABALHADOR, double _AJUSTE_FIXO_PRECO,
    int _TENTATIVAS_COMPRA_MAX, int _TENTATIVAS_CONTRATACAO_MAX, double _AJUSTE_FIXO_ESTOQUES) { // Inicializa com valores padrão
        contratados = demitidos = produzidos = consumidos = 0;

        AJUSTE_FIXO = _AJUSTE_FIXO;
        AJUSTE_FIXO_TRABALHADOR = _AJUSTE_FIXO_TRABALHADOR;
        AJUSTE_FIXO_PRECO = _AJUSTE_FIXO_PRECO;
        TENTATIVAS_COMPRA_MAX = _TENTATIVAS_COMPRA_MAX;
        TENTATIVAS_CONTRATACAO_MAX = _TENTATIVAS_CONTRATACAO_MAX;
        AJUSTE_FIXO_ESTOQUES = _AJUSTE_FIXO_ESTOQUES;
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
    void firmas_demitem();
    void firmas_demitem_todos();

    //TRABALHADORES__________________________________________________________________________________________
    // Adicionar trabalhadores
    void adicionarTrabalhadores(std::vector<std::shared_ptr<Trabalhador>>& trabalhador) {
        trabalhadores_no_mercado = trabalhador;
    }

    void trabalhadores_consomem();

    void trabalhadores_se_demitem();

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
};

//IMPLEMENTACOES________________________________________________________________________________________________

//FUNCAO PARA FIRMAS CONTRATAREM
void Mercado::firmas_contratam() {
    std::random_device rd; //definir seed para manter aleatoriedade
    std::mt19937 gen(rd());

    int recusados = 0;

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
                trabalhador->set_disposicao_salario_incremento(AJUSTE_FIXO_TRABALHADOR);
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
    }
    //std::cout << "CONTRATADOS: " << contratados << " | RECUSADOS: " << recusados << std::endl;
}

//FUNCAO PARA FIRMAS DEMITIREM
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
            //trabalhador->set_disposicao_salario_decremento(AJUSTE_FIXO_TRABALHADOR);
            trabalhadores_no_mercado.push_back(trabalhador);
            
            trabalhadores_empregados.erase(
                std::remove_if(trabalhadores_empregados.begin(), trabalhadores_empregados.end(),
                            [&trabalhador](const std::shared_ptr<Trabalhador>& emp) {
                                return emp == trabalhador; // Compara os shared_ptr
                            }),
                trabalhadores_empregados.end()
            );

            //diminui disposicao salarial
            //firma->set_disposicao_salario_decremento(AJUSTE_FIXO);
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
            //trabalhador->set_disposicao_salario_decremento(AJUSTE_FIXO_TRABALHADOR);
            trabalhadores_no_mercado.push_back(trabalhador);
            
            trabalhadores_empregados.erase(
                std::remove_if(trabalhadores_empregados.begin(), trabalhadores_empregados.end(),
                            [&trabalhador](const std::shared_ptr<Trabalhador>& emp) {
                                return emp == trabalhador; // Compara os shared_ptr
                            }),
                trabalhadores_empregados.end()
            );

            //diminui disposicao salarial
            //firma->set_disposicao_salario_decremento(AJUSTE_FIXO);
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

                    /* std::cout << "Trabalhador empregado comprou: " 
                    << "Salario: " << trabalhador->get_salario() 
                    << ", Disposicao: " << trabalhador->get_disposicao_produto() 
                    << ", Preco: " << firma->get_preco_produto() 
                    << ", Estoque: " << firma->get_estoque() << std::endl; */

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

        while ((trabalhador->get_salario() > 0) && (num_tentativas < TENTATIVAS_COMPRA_MAX)) {
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
                    firma->set_preco_produto(firma->get_preco_produto()-firma->get_preco_produto()*AJUSTE_FIXO_ESTOQUES*(std::log(firma->get_estoque()>3?firma->get_estoque():3)));
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

//FUNCAO PARA TRABALHADORES SE DEMITIREM
void Mercado::trabalhadores_se_demitem(){
    for (size_t i = 0; i < trabalhadores_empregados.size(); i++) {
        auto& trabalhador = trabalhadores_empregados[i];

        // Verifica se o trabalhador quer se demitir
        if (trabalhador->get_quer_se_demitir()) {
            // Remove o trabalhador da firma
            for (auto& firma : firmas) {
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

//FUNCAO PARA IMPRIMIR OS DADOS NO TERMINAL
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
    << get_percent_desempregados() <<'\n';

    arquivo.flush(); //forca a escrita no arquivo???

    arquivo.close();
    //std::cout << "Dados do mercado exportados para " << nome_arquivo << " com sucesso.\n";
}

#endif // MERCADO_H