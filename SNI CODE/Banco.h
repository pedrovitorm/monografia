#ifndef BANCO_H
#define BANCO_H

class Banco {
private:
    int taxa_juros;  // Taxa de juros definida pelo governo
    int capital;     // Capital do banco

public:
    Banco(int taxa_juros_inicial);

    // Faz empréstimos para firmas e consumidores com base no score de crédito
    void emprestarParaFirma(int valor, int score_firma);
    void emprestarParaConsumidor(int valor, int score_consumidor);

    // Métodos de acesso
    int getTaxaJuros() const;
    int getCapital() const;
};

#endif
