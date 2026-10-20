#include <stdio.h>
#include <stdlib.h>
#include "../header/saida.h"

void imprimirCaminhoPadrao(Labirinto *lab){          //Saída padrão
    for (int i = 0; i < lab -> tamanhoCaminho; i++){
        if (i > 0){
            printf(",");
        }
        printf("[%d,%d]", lab -> caminho[i][0], lab -> caminho[i][1]); // Imprime as coordenadas do caminho encontrado     
    }
    printf("\n");   
}

void imprimirSemCaminho(void){          
    printf("Não há um caminho possível para a entrada de dados fornecida\n");   //Mensagem exigida pela especificação
}

static void desenharMapa(const Labirinto *lab, bool **marcado, int pl, int pc){    //pl = posição linha, pc = posição coluna
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
                if (marcado[i][j]) printf("\033[1;33mC \033[0m");  //Chave coletada
                else printf("\033[33mc \033[0m");    //Chave fora do caminho
            }else if (marcado[i][j]){
                printf("\033[32m● \033[0m");  //Caminho percorrido
            }else{
                printf("  ");   //Sala vazia
            }
        }
        printf("\n");
    }
}

void imprimirLabirinto(const Labirinto *lab){      //Desenha o labirinto puro, sem caminho
    printf("\nLabirinto %dx%d | chaves necessarias: %d\n", lab -> linhas, lab -> colunas, lab -> chavesNecessarias);
    desenharMapa(lab, lab -> visitado, -1, -1);
    printf("\n");
}

void imprimirCaminhoCriativo(Labirinto *lab){         //Extra, print criativo
    printf("\nCaminho encontrado (%d salas):\n", lab -> tamanhoCaminho);
    desenharMapa(lab, lab -> visitado, -1, -1);  //Desenha o mapa sem o personagem
    printf("\n");
}

 
void imprimirTodosCaminhosCriativo(const Labirinto *lab, const TodosOsCaminhos *lista, int limite){   //Extra, print criativo de todos os caminhos
    if (lista == NULL || lista -> quantidade == 0){
        printf("Nenhum caminho para exibir.\n");
        return;
    }
 
    // Matriz auxiliar para marcar as salas de cada caminho (assim o labirinto nao e alterado)
    bool **marcado = calloc(lab -> linhas, sizeof(bool*));
    if (marcado == NULL){
        fprintf(stderr, "Erro ao alocar memoria\n");
        return;
    }
 
    for (int i = 0; i < lab -> linhas; i++){
        marcado[i] = calloc(lab -> colunas, sizeof(bool));
        if (marcado[i] == NULL){
            fprintf(stderr, "Erro ao alocar memoria\n");
            for (int k = 0; k < lab -> linhas; k++){
                free(marcado[k]);      // as linhas ainda nao alocadas estao em NULL
            }
            free(marcado);
            return;
        }
    }
 
    int total = lista -> quantidade;
    int exibir = total;
    if (limite > 0 && limite < total){
        exibir = limite;
    }
 
    for (int k = 0; k < exibir; k++){
        const CaminhoValido *cam = &lista -> caminhos[k];
 
        for (int p = 0; p < cam -> tamanho; p++){
            marcado[cam -> posicoes[p].linha][cam -> posicoes[p].coluna] = true;
        }
 
        printf("\nCaminho %d de %d (%d salas):\n", k + 1, total, cam -> tamanho);
        desenharMapa(lab, marcado, -1, -1);
 
        for (int p = 0; p < cam -> tamanho; p++){      //Desmarca para o proximo caminho
            marcado[cam -> posicoes[p].linha][cam -> posicoes[p].coluna] = false;
        }
    }
 
    if (exibir < total){
        printf("\n... e mais %d caminho(s) nao exibido(s).\n", total - exibir);
    }
    printf("\n");
 
    for (int i = 0; i < lab -> linhas; i++){
        free(marcado[i]);
    }
    free(marcado);
}
