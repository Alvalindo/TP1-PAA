#include "../header/backtracking.h"

int explorador_labirinto(Labirinto* _labirinto, int YposAtual, int XposAtual, int chavesColetadas){
    

    if(_labirinto->mapa[YposAtual][XposAtual] == 'X' && _labirinto->chavesNecessarias == chavesColetadas){

        // Registra o baú no caminho final
        _labirinto->caminho[_labirinto->tamanhoCaminho][0] = YposAtual;
        _labirinto->caminho[_labirinto->tamanhoCaminho][1] = XposAtual;
        _labirinto->tamanhoCaminho++;

        printf("BAU ENCONTRADO em (%d,%d)!\n", YposAtual, XposAtual);

        //Báu aberto e caminho econtrado
        return 1;
    }

    //Armazenando posição atual
        _labirinto->caminho[_labirinto->tamanhoCaminho][0] = YposAtual;
        _labirinto->caminho[_labirinto->tamanhoCaminho][1] = XposAtual;
        _labirinto->tamanhoCaminho++;

    //Verifica se encontrou uma chave
    if(_labirinto->mapa[YposAtual][XposAtual] == 'C' && chavesColetadas < _labirinto->chavesNecessarias){
        chavesColetadas++;
    }

    //Posição nova
    int YposNova;
    int XposNova;

    //Percorrer para cima;
    YposNova = YposAtual + Cima;
    XposNova = XposAtual;
    if(!(YposNova < 0 || YposNova >= _labirinto->linhas || _labirinto->mapa[YposNova][XposNova] == '1' 
        || caminho_visitado(_labirinto, YposNova, XposNova))){

           //printf("Moveu para cima: (%d,%d) -> (%d,%d)\n",
           //YposAtual, XposAtual, YposNova, XposNova);
            
        if(explorador_labirinto(_labirinto, YposNova , XposNova, chavesColetadas)){
            return 1;
        }
    }

    //Percorrer para direita;
    YposNova = YposAtual;
    XposNova = XposAtual + direita;
    if(!(XposNova < 0 || XposNova >= _labirinto->colunas || _labirinto->mapa[YposNova][XposNova] == '1' 
        || caminho_visitado(_labirinto, YposNova, XposNova))){

           //printf("Moveu para direita: (%d,%d) -> (%d,%d)\n",
           //YposAtual, XposAtual, YposNova, XposNova);

        if(explorador_labirinto(_labirinto, YposNova , XposNova, chavesColetadas)){
            return 1;
        }
    }

    //Percorrer para baixo;
    YposNova = YposAtual + baixo;
    XposNova = XposAtual;
    if(!(YposNova < 0 || YposNova >= _labirinto->linhas || _labirinto->mapa[YposNova][XposNova] == '1' 
        || caminho_visitado(_labirinto, YposNova, XposNova))){

           //printf("Moveu para baixo: (%d,%d) -> (%d,%d)\n",
           //YposAtual, XposAtual, YposNova, XposNova);

        if(explorador_labirinto(_labirinto, YposNova , XposNova, chavesColetadas)){
            return 1;
        }
    }

    //Percorrer para esquerda;
    YposNova = YposAtual;
    XposNova = XposAtual + esquerda;
    if(!(XposNova < 0 || XposNova >= _labirinto->colunas || _labirinto->mapa[YposNova][XposNova] == '1' 
        || caminho_visitado(_labirinto, YposNova, XposNova))){

           //printf("Moveu para esquerda: (%d,%d) -> (%d,%d)\n",
           //YposAtual, XposAtual, YposNova, XposNova);

        if(explorador_labirinto(_labirinto, YposNova , XposNova, chavesColetadas)){
            return 1;
        }
    }


    //Se não encotrou nenhum caminho
    _labirinto->tamanhoCaminho--;
    return 0;

}

bool caminho_visitado(Labirinto* _labirinto, int YposNova, int XposNova){

    for(int i = 0; i < _labirinto->tamanhoCaminho; i++){
            if(_labirinto->caminho[i][0] == YposNova && _labirinto->caminho[i][1] == XposNova){
                return true;
            }
        }
        return false;
}
