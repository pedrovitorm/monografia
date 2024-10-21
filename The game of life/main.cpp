//UNIVERSIDADE FEDERAL DE VICOSA - DEPARTAMENTO DE ECONOMIA
//Pedro Vitor Mourao Ribeiro Peres | Matricula: 102182
//CONWAY'S GAME OF LIFE

//BIBLIOTECAS
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

//NECESSARIO TER SFML INSTALADO
//codigo de compilacao:
//g++ main.cpp -lsfml-graphics -lsfml-window -lsfml-system

//___________________________________________________________________________________________________________
//CONSTANTES ADAPTAVEIS (5,140,273)
const int QUADRADO = 2; //tamanho da celula 
const int ALTURA = 350; //altura matriz //350
const int LARGURA = 682; //largura matriz 682
const int LARGURA_DISPLAY = 1365; //largura da tela em pixels
const int ALTURA_DISPLAY = 700; //altura da tela em pixels
const int ALEATORIEDADE = 50; //chance em percentual  de uma celula ser gerada viva no mapa inicial

//Funcoes auxiliares__________________________________________________________________________________________
//retorna o numero de vizinhos vivos
int vizinhos_vivos(int i, int j, bool matriz[ALTURA][LARGURA]) {
    // Definindo os deslocamentos para as posicoes vizinhas
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    int contador = 0; // Contador de vizinhos vivos

    // Iterando sobre todas as posicoes vizinhas
    //#####PARA UM MAPA FECHADO#####
    /* for (int k = 0; k < 8; ++k) {
        int novo_i = i + dx[k];
        int novo_j = j + dy[k];

        // Verificando se as novas posicoes estão dentro dos limites da matriz
        if (novo_i >= 0 && novo_i < ALTURA && novo_j >= 0 && novo_j < LARGURA) {
            if (matriz[novo_i][novo_j]) { // Se o vizinho for igual a 1
                contador++;
            }
        }
    } */
    //#####PARA UM MAPA CONTINUO#####
    for (int k = 0; k < 8; ++k) {
        // Calcular as novas coordenadas com wrap-around
        int novo_i = (i + dx[k] + ALTURA) % ALTURA; 
        int novo_j = (j + dy[k] + LARGURA) % LARGURA; 

        if (matriz[novo_i][novo_j]) { // Se o vizinho for igual a 1
            contador++;
        }
    }
    return contador; // Retorna o numero de vizinhos vivos
}

int main(){//______________________________________________________________________________________________
    //CRIA MATRIZ
    bool matriz[ALTURA][LARGURA];
    //matriz aleatoria
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for(int i=0;i<ALTURA;i++){
        for(int j=0;j<LARGURA;j++){
            int random = std::rand()%100;
            if(random > ALEATORIEDADE) matriz[i][j] = 0;
            else{
                matriz[i][j] = 1;
            }
        }
    }

    //JANELA E QUADRADOS
    sf::RenderWindow window(sf::VideoMode(LARGURA_DISPLAY,ALTURA_DISPLAY),"The Game of Life");
    sf::RectangleShape quadrado(sf::Vector2f(QUADRADO, QUADRADO));
    quadrado.setFillColor(sf::Color::White);

    //RELOGIO
    sf::Clock clock;

    //RENDERIZACAO
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
        for(int i=0;i<ALTURA;i++){
            for(int j=0;j<LARGURA;j++){
                if(matriz[i][j]==1){
                    quadrado.setPosition(j*QUADRADO,i*QUADRADO);
                        window.draw(quadrado);
                }
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

        //PROXIMA GERACAO
        //guarda matriz anterior
        bool anterior[ALTURA][LARGURA];
        for(int i=0;i<ALTURA;i++){
            for(int j=0;j<LARGURA;j++){
                anterior[i][j] = matriz[i][j];
            }
        }
        //calcula nova geracao
        for(int i=0;i<ALTURA;i++){
            for(int j=0;j<LARGURA;j++){
                //numero de vizinho vivos
                int num_vizinhos = vizinhos_vivos(i,j,anterior);
                if(matriz[i][j]==0&&num_vizinhos==3) matriz[i][j] = 1;
                else if(matriz[i][j]==1&&num_vizinhos<2) matriz[i][j] = 0;
                else if(matriz[i][j]==1&&num_vizinhos>3) matriz[i][j] = 0;
                else if(matriz[i][j]==1&&num_vizinhos<=3&&num_vizinhos>=2) matriz[i][j] = 1;
            }
        }
    }
    return 0;
}