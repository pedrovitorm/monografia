#ifndef UNIVERSIDADE_H
#define UNIVERSIDADE_H

#include <vector>
#include "Pesquisador.h"

class Universidade {
private:
    std::vector<Pesquisador> pesquisadores;  // Pesquisadores da universidade

public:
    Universidade();

    // Forma trabalhadores qualificados
    void formarTrabalhador(int qualificacao);

    // Realiza projetos de pesquisa e vende para empresas
    void realizarPesquisa();

    // Métodos de acesso
    int getNumeroPesquisadores() const;
};

#endif
