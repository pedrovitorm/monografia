#ifndef GOVERNO_H
#define GOVERNO_H

class Governo {
private:
    int taxa_juros;   // Taxa de juros definida pelo governo
    int orcamento;    // Orçamento do governo

public:
    Governo(int orcamento_inicial);

    // Define a taxa de juros
    void definirTaxaJuros(int nova_taxa);

    // Financia universidades e empresas
    void financiarUniversidade(int valor);
    void financiarEmpresa(int valor);

    // Métodos de acesso
    int getTaxaJuros() const;
};

#endif
