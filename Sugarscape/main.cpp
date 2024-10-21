#include "sugarseeker.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <SFML/Graphics.hpp>

//falta: crescer nos lugares onde vira 0, sortear direção que vai se todas sao iguais.

//variaveis globais
const int QUADRADO = 14; //tamanho da celula em pixels x pixels 
const int LARGURA_DISPLAY = 700; //largura da tela em pixels
const int ALTURA_DISPLAY = 700; //altura da tela em pixels
const int ALTURA = 50;
const int LARGURA = 50;
const int NUM_SEEKERS = 100;
const int TX_CRESC_SUGAR = 2;
const int NUM_SUGAR_PEAKS = 3;
const int NUM_ITERACOES = 3;
const int RAIO_SUGAR_PEAKS = 10;

void preencher_mapa_com_valores(std::vector<std::vector<int>>& mapa, int ALTURA, int LARGURA, int x_centro, int y_centro) {
    // Verifica se o ponto central está dentro dos limites do mapa
    if (x_centro < 0 || x_centro >= LARGURA || y_centro < 0 || y_centro >= ALTURA) {
        std::cout << "Ponto central fora dos limites do mapa!" << std::endl;
        return;
    }

    // Define a intensidade máxima (valor inicial) no ponto central
    int valor_maximo = 4;

    // Percorrer as camadas de valores ao redor do ponto central
    for (int valor = valor_maximo; valor >= 0; --valor) {
        // Define o raio de alcance de cada camada, com base no valor atual
        int raio = valor_maximo - valor;

        // Preenche o mapa ao redor do ponto central, respeitando os limites
        for (int i = -raio; i <= raio; i++) {
            for (int j = -raio; j <= raio; j++) {
                int novo_x = x_centro + j;
                int novo_y = y_centro + i;

                // Verifica se a posição está dentro dos limites do mapa
                if (novo_x >= 0 && novo_x < LARGURA && novo_y >= 0 && novo_y < ALTURA) {
                    // Atualiza o valor no mapa, somente se for maior do que o valor já presente
                    mapa[novo_y][novo_x] = std::max(mapa[novo_y][novo_x], valor);
                    //mapa[novo_y][novo_x] += valor;
                }
            }
        }
    }
}

void preencher_mapa_com_valores2(std::vector<std::vector<int>>& mapa, int ALTURA, int LARGURA, int num_pontos, int raio) {
    // Sorteia pontos aleatórios no mapa
    for (int n = 0; n < num_pontos; n++) {
        // Gera coordenadas aleatórias para o ponto central
        int x_centro = std::rand() % LARGURA;
        int y_centro = std::rand() % ALTURA;

        // Percorre as células dentro do raio ao redor do ponto central
        for (int i = -raio; i <= raio; i++) {
            for (int j = -raio; j <= raio; j++) {
                int novo_x = x_centro + j;
                int novo_y = y_centro + i;

                // Verifica se a posição está dentro dos limites do mapa
                if (novo_x >= 0 && novo_x < LARGURA && novo_y >= 0 && novo_y < ALTURA) {
                    // Incrementa o valor da célula no mapa
                    mapa[novo_y][novo_x] += 1;
                }
            }
        }
    }
}

// Função para verificar se uma posição já está ocupada
bool posicao_ocupada(std::vector<posicao>& ocupados, int x, int y) {
    if(ocupados.empty()) return false;
    for (const auto& p : ocupados) {
        if (p._x == x && p._y == y) {
            return true; // A posição já está ocupada
        }
    }
    return false; // A posição está livre
}

// Função para criar e posicionar os seekers
void criar_sugarseekers(std::vector<std::vector<int>>& mapa, const int ALTURA, const int LARGURA, const int NUM_SEEKERS, std::vector<sugarseeker>& seekers) {
    std::vector<posicao> ocupados; // Para controlar as posições já ocupadas

    //srand(static_cast<unsigned int>(time(nullptr))); // Semente para rand

    for (int i = 0; i < NUM_SEEKERS;i++) {
        sugarseeker seeker(i, ALTURA, LARGURA);
        
        do {
            // Sorteia uma posição aleatória dentro do mapa
            seeker.nasce();
            //std::cout << "x: " << seeker.posicao_seeker()._x << " y: " << seeker.posicao_seeker()._y << std::endl;
        } while (posicao_ocupada(ocupados, seeker.posicao_seeker()._x,seeker.posicao_seeker()._y)); // Gera nova posição se já estiver ocupada
        // Adiciona a posição à lista de ocupadas
        posicao pos(seeker.posicao_seeker()._x,seeker.posicao_seeker()._y);
        
        ocupados.push_back(pos);
        //std::cout << "i: " << i << std::endl;
        // Cria o seeker e o coloca na posição sorteada
        seekers.push_back(seeker);
    }
}

// Função para verificar se há um seeker na posição (x, y)
bool existe_seeker_na_posicao(std::vector<sugarseeker>& seekers, int x, int y) {
    for (std::vector<sugarseeker>::iterator it = seekers.begin();it!=seekers.end();it++) {
        sugarseeker seeker = *it;
        posicao p = seeker.posicao_seeker();
        if (p._x == x && p._y == y) {
            return true;
        }
    }
    return false;
}

// Função para imprimir o mapa, substituindo por -1 onde há seekers
void imprimir_mapa(std::vector<std::vector<int>>& mapa, const int ALTURA, const int LARGURA,std::vector<sugarseeker>& seekers) {
    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < LARGURA; j++) {
            if (existe_seeker_na_posicao(seekers, j, i)) {
                std::cout << "-1 "; // Imprime -1 onde há um seeker
            } else {
                std::cout << mapa[i][j] << " "; // Imprime o valor do mapa
            }
        }
        std::cout << std::endl;
    }
}

void atualiza_mapa(std::vector<std::vector<int>>& mapa, std::vector<sugarseeker>& seekers, const int TX_CRESC_SUGAR) {
    // Crescimento de açúcar primeiro
    for (int i = 0; i < mapa.size(); ++i) {
        for (int j = 0; j < mapa[i].size(); ++j) {
            // Crescimento de açúcar APENAS em células que já possuem algum açúcar
            if (existe_seeker_na_posicao(seekers, j, i)){
                if (mapa[i][j] > 0) {
                mapa[i][j] += TX_CRESC_SUGAR;
            }
            }
        }
    }

    // Agora os seekers consomem o açúcar após o crescimento
    for (auto& seeker : seekers) {
        // Obter a posição atual do seeker
        int x = seeker.posicao_seeker()._x;
        int y = seeker.posicao_seeker()._y;

        // Obter o metabolismo do seeker
        int metabolismo = seeker.get_metabolismo();

        // Referência ao açúcar disponível na célula atual
        int& acucar_disponivel = mapa[y][x];

        // Se o açúcar disponível for maior ou igual ao metabolismo, o seeker consome o necessário
        if (acucar_disponivel >= metabolismo) {
            acucar_disponivel -= metabolismo;
        }
        // Se não houver açúcar suficiente, o seeker consome o que resta
        else {
            acucar_disponivel = 0;
        }
    }
}


int main(){
    //GERACAO DO MAPA
    std::vector<std::vector<int>> mapa(ALTURA, std::vector<int>(LARGURA, 0));

    // Define a seed para a função rand() usando o tempo atual
    srand(static_cast<unsigned int>(time(nullptr)));  // Usa o tempo atual como seed

    //COLOCA PICOS EM LUGARES ALEATORIOS DO MAPA COM BASE NO NUMERO DE PICOS
    for(int i=0;i<NUM_SUGAR_PEAKS;i++){
        preencher_mapa_com_valores2(mapa, ALTURA, LARGURA,NUM_SUGAR_PEAKS,RAIO_SUGAR_PEAKS);
    }
    
    // Vetor para armazenar os seekers
    std::vector<sugarseeker> seekers;

    // Cria os seekers e os posiciona no mapa
    criar_sugarseekers(mapa, ALTURA, LARGURA, NUM_SEEKERS, seekers);

    //JANELA E QUADRADOS
    sf::RenderWindow window(sf::VideoMode(LARGURA_DISPLAY,ALTURA_DISPLAY),"SUGARSCAPE");
    sf::RectangleShape quadrado(sf::Vector2f(QUADRADO, QUADRADO));
    quadrado.setFillColor(sf::Color::White);

    //RELOGIO
    sf::Clock clock;

    //RENDERIZACAO____________________________________________________________________________________________
    while(window.isOpen()){
        //FECHAR JANELA SE CLICAR EM X
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
        }

        //LIMPAR
        window.clear(sf::Color::Black);

        //DESENHA QUADRADOS
        for(int i = 0; i < ALTURA; i++) {
            for(int j = 0; j < LARGURA; j++) {
                if(existe_seeker_na_posicao(seekers, j, i)) {
                    // Se houver um seeker na posição, ele será preto
                    quadrado.setFillColor(sf::Color::Black);
                } else {
                    // Determina a cor do quadrado baseado na quantidade de açúcar
                    int acucar = mapa[i][j];
                    sf::Color cor_acucar;

                    if (acucar == 0) {
                        cor_acucar = sf::Color::White;  // Sem açúcar, a célula é branca
                    } else {
                        // Interpolação entre branco e amarelo
                        // A cor amarela em SFML é sf::Color(255, 255, 0)
                        int intensidade = 255 * acucar / 10;  // Ajusta a intensidade entre 0 e 255
                        cor_acucar = sf::Color(255, 255 - intensidade, 0);  // De branco para amarelo
                    }

                    quadrado.setFillColor(cor_acucar);
                }

                quadrado.setPosition(j * QUADRADO, i * QUADRADO);
                window.draw(quadrado);
            }
        }
        
        //EXIBE
        window.display();

        //TEMPO DE ESPERA
        while(true){
            if(clock.getElapsedTime() > sf::seconds(0.1f)){
                break;
            }
        }
        clock.restart(); //reinicia o relogio

        //ATUALIZACAO DO MAPA
        atualiza_mapa(mapa,seekers,TX_CRESC_SUGAR);

        //ACAO DOS SEEKERS
        for(auto &seeker:seekers){
            seeker.seek(mapa,seekers);
        }
    }

    //std::cout << "MAPA INICIAL:" << std::endl;
    //imprimir_mapa(mapa, ALTURA, LARGURA, seekers);

    /* for(int i=0;i<NUM_ITERACOES;i++){
        std::cout << i << " itecacao:" << std::endl;
        for(auto &seeker:seekers){
            seeker.seek(mapa);
        }
        imprimir_mapa(mapa,ALTURA,LARGURA,seekers);
    } */

    return 0;
}