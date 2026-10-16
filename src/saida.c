#include <stdio.h>
#include "../header/saida.h"

void imprimirCaminhoPadrao(Labirinto *lab){          //Saída padrão
    for (int i = 0; i < lab -> tamanhoCaminho; i++){
        if (i > 0){
            printf(",");
        }
        printf("[%d, %d]", lab -> caminho[i][0], lab -> caminho[i][1]); // Imprime as coordenadas do caminho encontrado     
    }
    printf("\n");   
}

void imprimirSemCaminho(void){          
    printf("Sem caminho\n");
}

static void desenharMapa(Labirinto *lab, int pl, int pc){    //pl = posição linha, pc = posição coluna
    for (int i = 0; i < lab -> linhas; i++){
        for (int j = 0; j < lab -> colunas; j++){
            char ch = lab -> mapa[i][j];
            if (i == pl && j == pc){
                printf("\033[1;35m@ \033[0m");

            }else if (ch == '1'){
                printf("\033[90m██\033[0m"); 
            }else if (ch == 'A'){
                printf("\033[1;32mA \033[0m"); 
            }else if (ch == 'X'){
                printf("\033[1;31mX \033[0m");
            }else if (ch == 'C'){
                if (lab -> visitado[i][j]) printf("\033[1;33mC \033[0m");  //Chave coletada
                else printf("\033[33mc \033[0m");    //Chave fora do caminho
            }else if (lab -> visitado[i][j]){
                printf("\033[32m● \033[0m");  //Caminho percorrido
            }else{
                printf("  ");   //Sala vazia
            }
        }
        printf("\n");
    }
}

void imprimirCaminhoCriativo(Labirinto *lab){         //Extra, print criativo
    printf("\nCaminho encontrado (%d salas):\n", lab -> tamanhoCaminho);
    desenharMapa(lab, -1, -1);  //Desenha o mapa sem o personagem
    printf("\n");
}