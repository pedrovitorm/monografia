#ifndef CONSUMIDOR_H
#define CONSUMIDOR_H

#include <iostream>

class Consumidor {
private:
    double salario;        // Salário do consumidor
    double poupanca;       // Poupança acumulada
    double disposicao_salario;
    double disposicao_produto;
    int produtos_consumidos;
    //int utilidade;      // Utilidade obtida do consumo
    //int taxa_juros;     // Taxa de juros oferecida pelo banco

public:
    Consumidor(double _salario, double _riqueza_inicial, double _disposicao_produto): 
    disposicao_salario(_salario) {produtos_consumidos = 0; poupanca = 0;salario=0;salario = _riqueza_inicial;disposicao_produto = _disposicao_produto;};

    void set_salario(double valor){
        if(valor<0.0001){
            salario = 0;
            return;
        }  
        salario = valor;
    }
    void set_disposicao_salario_incremento(double novo_percent){disposicao_salario += disposicao_salario*novo_percent;}
    void set_disposicao_salario_decremento(double novo_percent){disposicao_salario -= disposicao_salario*novo_percent; if(disposicao_salario<0) disposicao_salario = 0;}
    double get_salario(){return salario;}
    double get_disposicao_salario(){return disposicao_salario;}
    double get_poupanca(){return poupanca;}
    double get_disposicao_produto(){return disposicao_produto;}
    void set_disposicao_produto(double valor) {
        if(valor<0.0001){
            disposicao_produto = 0;
            return;
        }  
        disposicao_produto = valor;
    }
    int get_produtod_consumidos(){return produtos_consumidos;}
    void set_produtos_consumidos(int valor){produtos_consumidos = valor;}

    // Decide se vai consumir bens com base na utilidade e preços
    void consumir(double preco_bem);
    void reinicia_consumidos() {produtos_consumidos = 0;}

    //Recebe salario
    void pagar_salario(double valor){salario += valor;}

    // Se não consumir, guarda o dinheiro no banco
    void poupar();
    void despoupar();
};

//IMPLEMENTACOES DAS FUNCOES___________________________________________________________________________________
void Consumidor::consumir(double preco_bem) {
    if(preco_bem > salario){ //se preco for maior que o salario restante ele nao compra
        poupar();
    }
    else if(preco_bem <= salario){
        salario -= preco_bem;
        produtos_consumidos++;
    }
    else if(preco_bem < salario+poupanca){
        despoupar();
        salario -= preco_bem;
    }
}

void Consumidor::poupar(){
    poupanca += salario;
    salario = 0;
}

void Consumidor::despoupar(){
    salario += poupanca;
    poupanca = 0;
}

#endif