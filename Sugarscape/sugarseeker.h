#ifndef SUGARSEEKER_H
#define SUGARSEEKER_H

#include <ctime>
#include <cstdlib>
#include <vector>

struct posicao
{
    int _x;
    int _y;
    posicao(int x = 0, int y = 0) : _x(x), _y(y) {}
};

class sugarseeker
{
private:
    int _id;
    posicao _pos;
    int _visao;
    int _metabolismo;
    int _idade;
    int _idade_max;
    int _riqueza;
    int ALTURA;
    int LARGURA;

    void move(posicao melhor);
    void metabolize();
public:
    sugarseeker(int id, int _ALTURA, int _LARGURA):
        _id(id), ALTURA(_ALTURA), LARGURA(_LARGURA) {_pos = posicao(0,0);}
    ~sugarseeker() {}

    int get_metabolismo() {return _metabolismo;}
    void seek(std::vector<std::vector<int>>& mapa,std::vector<sugarseeker>& seekers);
    posicao posicao_seeker();
    void nasce();
    int get_id() {return _id;}

    // Construtor de cópia
    sugarseeker(const sugarseeker &other){
        *this = other;
    }
        

    // Operador de atribuição
    sugarseeker& operator=(const sugarseeker &other) {
        if (this != &other) {
            _id = other._id;
            _pos = other._pos;
            _visao = other._visao;
            _metabolismo = other._metabolismo;
            _idade = other._idade;
            _idade_max = other._idade_max;
            _riqueza = other._riqueza;
            ALTURA = other.ALTURA;
            LARGURA = other.LARGURA;
        }
        return *this;
    }
};

void sugarseeker::seek(std::vector<std::vector<int>>& mapa, std::vector<sugarseeker>& seekers) {
    posicao melhor(_pos._x, _pos._y);
    int melhor_sugar = mapa[_pos._y][_pos._x];
    bool encontrou_melhor = false;

    std::vector<posicao> opcoes_validas;  // Armazena todas as posições com açúcar igual ou maior

    // Procura a melhor posição dentro da visão do seeker
    for (int i = -_visao; i <= _visao; i++) {
        for (int j = -_visao; j <= _visao; j++) {
            int novo_x = _pos._x + j;
            int novo_y = _pos._y + i;

            // Verificar se a nova posição está dentro dos limites do mapa
            if (novo_x >= 0 && novo_x < LARGURA && novo_y >= 0 && novo_y < ALTURA) {
                // Verificar se a posição já está ocupada por outro seeker
                bool ocupado = false;
                for (auto &other : seekers) {
                    if (other.posicao_seeker()._x == novo_x && other.posicao_seeker()._y == novo_y) {
                        ocupado = true;
                        break;  // Se encontrar um seeker, não precisa continuar verificando
                    }
                }

                // Se não estiver ocupada, considerar a posição
                if (!ocupado) {
                    if (mapa[novo_y][novo_x] > melhor_sugar) {
                        // Encontrou uma posição com mais açúcar
                        melhor._x = novo_x;
                        melhor._y = novo_y;
                        melhor_sugar = mapa[novo_y][novo_x];
                        encontrou_melhor = true;
                        opcoes_validas.clear();  // Limpa as opções anteriores, pois essa é a nova melhor
                    } else if (mapa[novo_y][novo_x] >= melhor_sugar) {
                        // Se o açúcar for igual ou melhor, adiciona à lista de opções válidas
                        opcoes_validas.push_back(posicao(novo_x, novo_y));
                    }
                }
            }
        }
    }

    // Se não encontrou uma posição melhor ou se existem opções igualmente boas, sorteia uma posição aleatória
    if (!encontrou_melhor && !opcoes_validas.empty()) {
        int indice_aleatorio = rand() % opcoes_validas.size();
        melhor = opcoes_validas[indice_aleatorio];
    }

    // O seeker coleta açúcar da posição atual
    _riqueza += mapa[_pos._y][_pos._x];

    // Mover para a melhor posição (ou sorteada)
    move(melhor);

    _idade++;
    // Metabolizar o açúcar consumido
    metabolize();
}

void sugarseeker::move(posicao melhor) {
    // Verificar se o agente precisa se mover no eixo X
    if (_pos._x < melhor._x) {
        if (_pos._x + 1 < LARGURA) {  // Garantir que não sai dos limites do mapa no eixo X
            _pos._x++;
        }
    } else if (_pos._x > melhor._x) {
        if (_pos._x - 1 >= 0) {  // Garantir que não sai dos limites do mapa no eixo X
            _pos._x--;
        }
    }

    // Verificar se o agente precisa se mover no eixo Y
    if (_pos._y < melhor._y) {
        if (_pos._y + 1 < ALTURA) {  // Garantir que não sai dos limites do mapa no eixo Y
            _pos._y++;
        }
    } else if (_pos._y > melhor._y) {
        if (_pos._y - 1 >= 0) {  // Garantir que não sai dos limites do mapa no eixo Y
            _pos._y--;
        }
    }
}

posicao sugarseeker::posicao_seeker(){
    return posicao(_pos._x, _pos._y);
}

void sugarseeker::nasce(){
    //srand(static_cast<unsigned int>(time(nullptr)));
    _idade = 0;
    
    // Gerar valores aleatórios para os atributos usando rand()
    _pos._x = rand() % LARGURA;  // Supondo que o mapa tenha LARGURA definida
    _pos._y = rand() % ALTURA;   // Supondo que o mapa tenha ALTURA definida

    _visao = (rand() % 6) + 1;       // Visão varia de 1 a 6
    _metabolismo = (rand() % 4) + 1; // Metabolismo varia de 1 a 4
    _idade_max = (rand() % 41) + 60; // Idade máxima varia de 60 a 100
    _riqueza = (rand() % 21) + 5;    // Riqueza varia de 5 a 25
}

void sugarseeker::metabolize(){
    if(_idade>_idade_max){
        nasce();
        return;
    }
    _riqueza -= _metabolismo;
    if(_riqueza<0){
        nasce();
        return;
    }
}

#endif