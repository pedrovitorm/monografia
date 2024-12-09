#ifndef FIRMA_H
#define FIRMA_H

#include <vector>
#include "Trabalhador.h"
//#include "Pesquisador.h"

class Firma {
private:
    double capital;               // Capital da firma
    int estoque;               // Quantidade de bens estocados
    std::vector<std::shared_ptr<Trabalhador>> trabalhadores; // Trabalhadores contratados
    std::vector<std::shared_ptr<Trabalhador>> trabalhadores_produtivo; //Trabalhadores produtivos
    double disposicao_salario;
    double disposicao_salario_produtivo;
    double preco_produto;
    int quantidade_vendida;
    //int complexidade_produto;  // Complexidade do bem produzido
    //int trabalhadores_necessarios; // Quantidade de trabalhadores necessários
    //std::vector<Pesquisador> pesquisadores; // Pesquisadores contratados
    //bool financiamento;        // Se a firma está financiando suas operações

public:
    Firma(double capital_inicial, double _disposicao_salario, double _estoque_inicial, double _preco_produto) {
        capital = capital_inicial;
        estoque = _estoque_inicial;
        disposicao_salario = _disposicao_salario;
        disposicao_salario_produtivo = _disposicao_salario;
        preco_produto = _preco_produto;
        quantidade_vendida = 0;
    }

    // Contrata pesquisadores para projetos de pesquisa
    //void contratarPesquisadores(int numero_pesquisadores);

    // Realiza pesquisa com uma probabilidade de sucesso
    //void realizarProjetoPesquisa();

    // Aumenta a produtividade da firma ao adquirir conhecimento
    //void aumentarProdutividade(int aumento);

    //Pagar salarios
    void pagar_salarios();

    int produzir();

    // Calcula o lucro da firma
    double calcularLucro();

    // Armazena bens não vendidos
    void armazenarProduto(int quantidade) {estoque += quantidade;}

    void vender_produto(double valor) {estoque--;capital+=valor;quantidade_vendida++;}

    //metodos para serem usados pelo MERCADO
    // Contrata trabalhadores de acordo com a complexidade do produto
    void contratar(std::shared_ptr<Trabalhador> trabalhador) {
        trabalhadores.push_back(trabalhador);
    }
    void contratar_produtivo(std::shared_ptr<Trabalhador> trabalhador) {
        trabalhadores_produtivo.push_back(trabalhador);
    }

    void demite(std::shared_ptr<Trabalhador> trabalhador);
    std::shared_ptr<Trabalhador> querem_ser_demitidos();

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

    void imprime();

    std::shared_ptr<Trabalhador> demite_menos_produtivo();
};

//IMPLEMENTACAO____________________________________________________________________________________
void Firma::pagar_salarios(){
    for(auto &trabalhador : trabalhadores){
        //trabalhador->imprime();
        trabalhador->pagar_salario(disposicao_salario);
        capital -= disposicao_salario;
        //trabalhador->imprime();
    }

    for(auto &trabalhador : trabalhadores_produtivo){
        //trabalhador->imprime();
        trabalhador->pagar_salario(disposicao_salario_produtivo);
        capital -= disposicao_salario_produtivo;
        //trabalhador->imprime();
    }
}

std::shared_ptr<Trabalhador> Firma::demite_menos_produtivo() {
    if (trabalhadores.empty()) {
        if(trabalhadores_produtivo.empty()){
            //std::cout << "Nenhum trabalhador para demitir." << std::endl;
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
        //std::cout << "Demissão do trabalhador com produtividade " << produtividade_minima << std::endl;
        trabalhadores_produtivo.erase(trabalhadores_produtivo.begin() + indice_menos_produtivo); // Remove o trabalhador da lista

        // Retorna o trabalhador demitido
        return trabalhador_demidido;
    }
    else{
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
        //std::cout << "Demissão do trabalhador com produtividade " << produtividade_minima << std::endl;
        trabalhadores.erase(trabalhadores.begin() + indice_menos_produtivo); // Remove o trabalhador da lista

        // Retorna o trabalhador demitido
        return trabalhador_demidido;
    }
}

void Firma::demite(std::shared_ptr<Trabalhador> trabalhador){
    int indice = -1;
    for (int i = 0; i < trabalhadores.size(); ++i){
        if(trabalhadores[i]->igual(trabalhador)){
            indice = i;
            std::cout << "entrei2" << std::endl;
            break;
        }
    }
    if(indice = -1) return;
    trabalhadores.erase(trabalhadores.begin() + indice);   
}

std::shared_ptr<Trabalhador> Firma::querem_ser_demitidos(){
    for(auto &trabalhador : trabalhadores){
        if(trabalhador->get_quer_se_demitir()){
            demite(trabalhador);
            return trabalhador;
        }
    }
    return nullptr;
}

int Firma::produzir(){
    int produtividade_total = 0;
    for(auto &trabalhador : trabalhadores){
        produtividade_total+= trabalhador->get_produtividade();
        trabalhador->adiciona_ano_empresa();
    }

    for(auto &trabalhador : trabalhadores_produtivo){
        produtividade_total+= trabalhador->get_produtividade();
        trabalhador->adiciona_ano_empresa();
    }

    armazenarProduto(produtividade_total);

    return produtividade_total;
    //lembrar de atualizar a quantidade vendida em cada iteracao
    /* if(quantidade_vendida<produtividade_total){
        armazenarProduto(produtividade_total-quantidade_vendida);
    }
    else{ //tira do estoque vou vende o estoque todo
        if(quantidade_vendida > produtividade_total+estoque){
            quantidade_vendida = produtividade_total+estoque;
        }
        else{
            estoque -= quantidade_vendida-produtividade_total;
        }
    }

    capital += produtividade_total*preco_produto*quantidade_vendida; */
}

double Firma::calcularLucro(){
    produzir();
    //vender();
    pagar_salarios();
    return capital;
}

void Firma::imprime(){
    std::cout << "CAPITAL:" << capital << "|ESTOQUE:" << estoque << "|DISPOSICAO_SALARIO:" << disposicao_salario
    << "|PRECO_PRODUTO:" << preco_produto << "|QUANTIDADE_VENDIDA:" << quantidade_vendida << 
    "|NUM_TRABALHADORES:" << trabalhadores.size() << std::endl;
}

#endif