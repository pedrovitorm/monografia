#ifndef FIRMA_H
#define FIRMA_H

#include <vector>
#include "Trabalhador.h"

class Firma {
private:
    double capital; // Capital da firma
    int estoque; // Quantidade de bens estocados
    std::vector<std::shared_ptr<Trabalhador>> trabalhadores; // Trabalhadores contratados
    std::vector<std::shared_ptr<Trabalhador>> trabalhadores_produtivo; //Trabalhadores produtivos
    double disposicao_salario;
    double disposicao_salario_produtivo;
    double preco_produto;
    int quantidade_vendida;

public:
    Firma(double capital_inicial, double _disposicao_salario, double _estoque_inicial, double _preco_produto) {
        capital = capital_inicial;
        estoque = _estoque_inicial;
        disposicao_salario = _disposicao_salario;
        disposicao_salario_produtivo = _disposicao_salario;
        preco_produto = _preco_produto;
        quantidade_vendida = 0;
    }

    //Pagar salarios
    void pagar_salarios();

    //Funcao para produzir
    int produzir();

    // Armazena bens não vendidos
    void armazenarProduto(int quantidade) {estoque += quantidade;}
    void vender_produto(double valor) {estoque--;capital+=valor;quantidade_vendida++;}

    //metodos para serem usados pelo MERCADO_H
    // Contrata trabalhadores
    void contratar(std::shared_ptr<Trabalhador> trabalhador) {
        trabalhadores.push_back(trabalhador);
    }
    //Funcao para contratar trabalhador diferenciado (recebe um salario maior)
    void contratar_produtivo(std::shared_ptr<Trabalhador> trabalhador) {
        trabalhadores_produtivo.push_back(trabalhador);
    }

    //GETS E SETS
    double get_disposicao_salario() {return disposicao_salario;}
    double get_disposicao_salario_produtivo() {return disposicao_salario_produtivo;}
    int get_quandidade_vendida(){return quantidade_vendida;}
    int get_quantidade_trabalhadores(){return (trabalhadores.size()+trabalhadores_produtivo.size());}
    void set_preco_produto(double valor){
        if(valor<0.0001){
            preco_produto = 0;
            return;
        }  
        preco_produto = valor;
    }
    double get_preco_produto(){return preco_produto;}
    void set_quantidade_vendida(int quantidade){quantidade_vendida = quantidade;}
    void set_disposicao_salario_incremento(double novo_percent){disposicao_salario += disposicao_salario*novo_percent; if(disposicao_salario<0) disposicao_salario = 0;}
    void set_disposicao_salario_decremento(double novo_percent){disposicao_salario -= disposicao_salario*novo_percent; if(disposicao_salario<0) disposicao_salario = 0;}
    void set_disposicao_salario_incremento_produtivo(double novo_percent){disposicao_salario_produtivo += disposicao_salario_produtivo*novo_percent; if(disposicao_salario_produtivo<0) disposicao_salario_produtivo = 0;}
    void set_disposicao_salario_decremento_produtivo(double novo_percent){disposicao_salario_produtivo -= disposicao_salario_produtivo*novo_percent; if(disposicao_salario_produtivo<0) disposicao_salario_produtivo = 0;}
    int get_estoque(){return estoque;}
    double get_capital(){return capital;}
    double get_salario_produtivo_medio(){
        double soma = 0;
        if(trabalhadores_produtivo.empty()) return 0;

        for(auto &trabalhador : trabalhadores_produtivo){
            soma+=trabalhador->get_salario_efetivo();
        }
        return soma/trabalhadores_produtivo.size();
    }
    double get_salario_improdutivo_medio(){
        double soma = 0;
        if(trabalhadores.empty()) return 0;

        for(auto &trabalhador : trabalhadores){
            soma+=trabalhador->get_salario_efetivo();
        }
        return soma/trabalhadores.size();
    }

    //Funcao para imprimir informacoes no terminal
    void imprime();

    //Funcao para demitir os funcionarios
    std::shared_ptr<Trabalhador> demite_menos_produtivo();
};

//IMPLEMENTACAO____________________________________________________________________________________
void Firma::pagar_salarios(){
    for(auto &trabalhador : trabalhadores){
        trabalhador->pagar_salario(trabalhador->get_salario_efetivo());
        capital -= trabalhador->get_salario_efetivo();
    }
    for(auto &trabalhador : trabalhadores_produtivo){
        trabalhador->pagar_salario(trabalhador->get_salario_efetivo());
        capital -= trabalhador->get_salario_efetivo();
    }
}

std::shared_ptr<Trabalhador> Firma::demite_menos_produtivo() {
    if (trabalhadores.empty()) {
        if(trabalhadores_produtivo.empty()){
            //Nenhum funcionario para ser demitido (retorna nulo)
            return NULL;
        }

        //DEMITE UM PRODUTIVO
        // Inicializa o índice do trabalhador menos produtivo
        size_t indice_menos_produtivo = 0;
        double produtividade_minima = trabalhadores_produtivo[0]->get_produtividade();

        // Itera sobre os trabalhadores para encontrar o menos produtivo
        for (size_t i = 1; i < trabalhadores_produtivo.size(); ++i) {
            double produtividade_atual = trabalhadores_produtivo[i]->get_produtividade();
            if (produtividade_atual < produtividade_minima) {
                indice_menos_produtivo = i; // Atualiza o índice do trabalhador menos produtivo
                produtividade_minima = produtividade_atual;
            }
        }

        // Armazena o trabalhador a ser demitido
        auto trabalhador_demidido = trabalhadores_produtivo[indice_menos_produtivo];

        // Demite o trabalhador menos produtivo
        trabalhadores_produtivo.erase(trabalhadores_produtivo.begin() + indice_menos_produtivo); // Remove o trabalhador da lista

        // Retorna o trabalhador demitido
        trabalhador_demidido->set_salario_efetivo(0);
        return trabalhador_demidido;
    }
    else{
        //DEMITE UM TRABALHADOR MENOS PRODUTIVO
        // Inicializa o índice do trabalhador menos produtivo
        size_t indice_menos_produtivo = 0;
        double produtividade_minima = trabalhadores[0]->get_produtividade();

        // Itera sobre os trabalhadores para encontrar o menos produtivo
        for (size_t i = 1; i < trabalhadores.size(); ++i) {
            double produtividade_atual = trabalhadores[i]->get_produtividade();
            if (produtividade_atual < produtividade_minima) {
                indice_menos_produtivo = i; // Atualiza o índice do trabalhador menos produtivo
                produtividade_minima = produtividade_atual;
            }
        }

        // Armazena o trabalhador a ser demitido
        auto trabalhador_demidido = trabalhadores[indice_menos_produtivo];

        // Demite o trabalhador menos produtivo
        trabalhadores.erase(trabalhadores.begin() + indice_menos_produtivo); // Remove o trabalhador da lista

        // Retorna o trabalhador demitido
        return trabalhador_demidido;
    }
}

int Firma::produzir(){
    int produtividade_total = 0;
    for(auto &trabalhador : trabalhadores){
        produtividade_total+= trabalhador->get_produtividade();
    }
    for(auto &trabalhador : trabalhadores_produtivo){
        produtividade_total+= trabalhador->get_produtividade();
    }
    armazenarProduto(produtividade_total);
    return produtividade_total;
}

void Firma::imprime(){
    std::cout << "CAPITAL:" << capital << "|ESTOQUE:" << estoque << "|DISPOSICAO_SALARIO:" << disposicao_salario
    << "|PRECO_PRODUTO:" << preco_produto << "|QUANTIDADE_VENDIDA:" << quantidade_vendida << 
    "|NUM_TRABALHADORES:" << trabalhadores.size() << std::endl;
}

#endif