#include "header/backtracking.h"
#include "header/saida.h"

int main(){

    Labirinto lab;
    
    carregarLabirinto(&lab, "labirinto.txt");

    if(explorador_labirinto(&lab, 0, 0, 0)){
        imprimirCaminhoPadrao(&lab);
    }else{
        printf("TIRA QUE EU VOU CAGAR, n tem caminho do meu cuzinho \n");
    }
    

}
