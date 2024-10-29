#ifndef PESQUISADOR_H
#define PESQUISADOR_H

#include "Consumidor.h"

class Pesquisador : public Consumidor {
private:
    int nivel_qualificacao;
    bool em_projeto_pesquisa;
    
public:
    Pesquisador(double salario, int nivel_qualificacao, bool em_projeto_pesquisa = false)
        : Consumidor(salario), nivel_qualificacao(nivel_qualificacao), em_projeto_pesquisa(em_projeto_pesquisa) {}

    void realizarPesquisa();  // Implementar a lógica de pesquisa
    int getNivelQualificacao() const { return nivel_qualificacao; }
    bool isEmProjetoPesquisa() const { return em_projeto_pesquisa; }
    void setEmProjetoPesquisa(bool status) { em_projeto_pesquisa = status; }

    // Métodos adicionais específicos do Pesquisador
};

#endif
