#ifndef TRABALHADOR_H
#define TRABALHADOR_H

#include "Consumidor.h"
#include <iostream>

class Trabalhador : public Consumidor {
private:
    int produtividade;
    bool quer_sair_emprego;
    int periodo_na_empresa;
    //int nivel_qualificacao;

public:
    Trabalhador(double _salario, int _produtividade, double _riqueza_inicial, double _disposicao_produto)
        : Consumidor(_salario,_riqueza_inicial, _disposicao_produto), produtividade(_produtividade) {quer_sair_emprego = false;
        periodo_na_empresa = 0;}

    int get_produtividade(){return produtividade;} // Implementar a lógica de produção
    //int getNivelQualificacao() const { return nivel_qualificacao; }
    //int getProdutividade() const { return produtividade; }
    //void setProdutividade(int nova_produtividade) { produtividade = nova_produtividade; }

    // Métodos adicionais específicos do Trabalhador
    void imprime();

    void adiciona_ano_empresa(){periodo_na_empresa++;}
    void reinicia_ano_empresa(){periodo_na_empresa = 0;}

    bool get_quer_se_demitir(){return quer_sair_emprego;}
    bool quer_se_demitir();
    void contratado(){quer_sair_emprego = false;}


    // Operador de igualdade dentro da classe
    bool igual(std::shared_ptr<Trabalhador> &outro) {
        return get_salario() == outro->get_salario() &&
               get_disposicao_salario() == outro->get_disposicao_salario() &&
               get_poupanca() == outro->get_poupanca() &&
               get_disposicao_produto() == outro->get_disposicao_produto() &&
               get_produtod_consumidos() == outro->get_produtod_consumidos() &&
               produtividade == outro->produtividade &&
               quer_sair_emprego == outro->quer_sair_emprego &&
               periodo_na_empresa == outro->periodo_na_empresa;
    }
};

bool Trabalhador::quer_se_demitir(){
    if(periodo_na_empresa>=12){
        quer_sair_emprego = true;
    }
    return quer_sair_emprego;
}

void Trabalhador::imprime(){
    std::cout << "RIQUEZA:" << get_salario() << "|DISPOSICAO_SALARIO:"<< get_disposicao_salario()
    << "|POUPANCA:" << get_poupanca() << "|PRODUTIVIDADE:" << produtividade << 
    "|DISPOSICAO_PRODUTO:" << get_disposicao_produto() << "|PRODUTOS_CONSUMIDOS:" << get_produtod_consumidos()
    << "|QUER_SE_DEMITIR:" << quer_se_demitir() << "|PERIODOS_EMPRESA:" << periodo_na_empresa << std::endl;
    set_produtos_consumidos(0);
}



#endif