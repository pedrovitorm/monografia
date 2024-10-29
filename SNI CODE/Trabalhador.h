#ifndef TRABALHADOR_H
#define TRABALHADOR_H

#include "Consumidor.h"
#include <iostream>

class Trabalhador : public Consumidor {
private:
    int produtividade;
    //int nivel_qualificacao;

public:
    Trabalhador(double _salario, int _produtividade, double _riqueza_inicial, double _disposicao_produto)
        : Consumidor(_salario,_riqueza_inicial, _disposicao_produto), produtividade(_produtividade) {}

    int get_produtividade(){return produtividade;} // Implementar a lógica de produção
    //int getNivelQualificacao() const { return nivel_qualificacao; }
    //int getProdutividade() const { return produtividade; }
    //void setProdutividade(int nova_produtividade) { produtividade = nova_produtividade; }

    // Métodos adicionais específicos do Trabalhador
    void imprime();
};

void Trabalhador::imprime(){
    std::cout << "RIQUEZA:" << get_salario() << "|DISPOSICAO_SALARIO:"<< get_disposicao_salario()
    << "|POUPANCA:" << get_poupanca() << "|PRODUTIVIDADE:" << produtividade << 
    "|DISPOSICAO_PRODUTO:" << get_disposicao_produto() << "|PRODUTOS_CONSUMIDOS:" << get_produtod_consumidos()
    << std::endl;
}

#endif