#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/labirinto.h"

void liberarLabirinto(Labirinto *lab){    //Verifica se o ponteiro recebido é válido
    if (lab == NULL){
        return;
    }

    if (lab -> mapa){              //Libera cada linha matriz que representa o mapa
        for (int i = 0; i < lab -> linhas; i++){
            free(lab -> mapa[i]);
        }
        free(lab -> mapa);
    }

    if (lab->visitado) {            //Libera cada linha da matriz de salas visitadas
        for (int i = 0; i < lab->linhas; i++){ 
            free(lab->visitado[i]);
        }
        free(lab->visitado);
    }
    free(lab -> caminho);
    memset(lab, 0, sizeof(*lab));        //Reinicializa os campos da estrutura.
}

int carregarLabirinto(Labirinto *lab, const char *nomeArquivo){     //Carrega o labirinto a partir de um arquivo e verifica se as dimensões, os caracteres e os elementos são válidos. Retorna 1 em caso de sucesso e 0 em caso de fracasso
    memset(lab, 0, sizeof(*lab)); 

    
    FILE *f = fopen(nomeArquivo, "r");         //Abre o arquivo para leitura
    if (f == NULL){
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", nomeArquivo);
        return 0;
    }

    int linhas, colunas, chaves;
    
    if (fscanf(f, "%d %d", &linhas, &colunas) != 2 || linhas <= 0 || colunas <= 0){     //Vê as dimensões e verifica se são positivas
        fprintf(stderr, "Erro, dimensoes invalidas na primeira linha\n");
        fclose(f);
        return 0;
    }

    if (fscanf(f, "%d", &chaves) != 1 || chaves < 0){            //Le a quantidade de chaves necessárias
        fprintf(stderr, "Erro, numero de chaves invalido na segunda linha\n");
        fclose(f);
        return 0;
    }

    lab -> linhas = linhas;       //Armazena as informações iniciais da estrutura
    lab -> colunas = colunas;
    lab -> chavesNecessarias = chaves;
    lab -> tamanhoCaminho = 0;

    lab -> mapa = calloc(linhas, sizeof(char*));        //Aloca memória para as matrizes e para o caminho
    lab -> visitado = calloc(linhas, sizeof(bool*));
    lab -> caminho = malloc(sizeof(*lab -> caminho) * linhas * colunas);

    if (!lab -> mapa || !lab -> visitado || !lab -> caminho){
        fprintf(stderr, "Erro ao alocar memoria\n");
        fclose(f);
        liberarLabirinto(lab);
        return 0;
    }

    for (int i = 0; i < linhas; i++){
        lab -> mapa[i] = malloc(colunas);
        lab -> visitado[i] = calloc(colunas, sizeof(bool));
        if (!lab -> mapa[i] || !lab -> visitado[i]){
            fprintf(stderr, "Erro ao alocar memoria\n");
            fclose(f);
            liberarLabirinto(lab);
            return 0;
        }
    }

    int qtdA = 0,  qtdX = 0, qtdC = 0;        //Contabiliza a quantidade de elementos do mapa
    for (int i = 0; i < linhas; i++){
        for (int j = 0; j < colunas; j++){
            char ch;

            if (fscanf(f, " %c", &ch) != 1){  //Verifica se o arquivo possui todos os caracteres necessários
                fprintf(stderr, "Erro: arquivo terminou antes do esperado (linhas: %d, colunas: %d)\n", i, j);
                fclose(f);
                liberarLabirinto(lab);
                return 0;
            }

            if (ch != '0' && ch != '1' && ch != 'C' && ch != 'X' && ch != 'A'){        //Aceita apenas os caracteres válidos
                fprintf(stderr, "Erro: caractere invalido '%c' na posicao (%d, %d)\n", ch, i, j);
                fclose(f);
                liberarLabirinto(lab);
                return 0;
            }
            lab -> mapa[i][j] = ch;    //Armazena o caractere na posicao correspondente.
            if (ch == 'A'){          //Registra as posicoes inicial e final e conta as chaves.
                qtdA++;
                lab -> inicioL = i;
                lab -> inicioC = j;
            } else if (ch == 'X'){
                qtdX++;
            } else if (ch == 'C'){
                qtdC++;
            }
        }
    }  

    fclose(f);       //Fecha o arquivo apos concluir a leitura.

    if (qtdA != 1 || qtdX != 1){         //Exige exatamente uma posicao inicial e uma saida.
        fprintf(stderr, "Erro: o mapa deve ter exatamente um 'A' e um 'X'\n");
        liberarLabirinto(lab);
        return 0;
    }

    if (qtdC != chaves){      //Confere se a quantidade de chaves corresponde ao valor informado.
        fprintf(stderr, "Erro: o mapa tem %d chaves, mas sao necessarias %d\n", qtdC, chaves);
        liberarLabirinto(lab);
        return 0;
    }

    return 1;
}

void sincronizarMatrizVisitado(Labirinto *lab) {    //Sincroniza a matriz de visitacao com o caminho armazenado. Primeiro desmarca todas as posicoes e depois marca as do caminho.
    if (!lab || !lab->visitado) return;
    
    for (int i = 0; i < lab->linhas; i++) {
        for (int j = 0; j < lab->colunas; j++) {
            lab->visitado[i][j] = false;
        }
    }
    
    for (int i = 0; i < lab->tamanhoCaminho; i++) {
        int r = lab->caminho[i][0];
        int c = lab->caminho[i][1];
        if (r >= 0 && r < lab->linhas && c >= 0 && c < lab->colunas) {
            lab->visitado[r][c] = true;
        }
    }
}