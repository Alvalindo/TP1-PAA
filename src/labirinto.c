#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/labirinto.h"

void liberarLabirinto(Labirinto *lab){
    if (lab == NULL){
        return;
    }

    if (lab -> mapa){
        for (int i = 0; i < lab -> linhas; i++){
            free(lab -> mapa[i]);
        }
        free(lab -> mapa);
    }

    if (lab->visitado) {
        for (int i = 0; i < lab->linhas; i++){ 
            free(lab->visitado[i]);
        }
        free(lab->visitado);
    }
    free(lab -> caminho);
    memset(lab, 0, sizeof(*lab));
}

int carregarLabirinto(Labirinto *lab, const char *nomeArquivo){
    memset(lab, 0, sizeof(*lab));

    
    FILE *f = fopen(nomeArquivo, "r");
    if (f == NULL){
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", nomeArquivo);
        return 0;
    }

    int linhas, colunas, chaves;
    
    if (fscanf(f, "%d %d", &linhas, &colunas) != 2 || linhas <= 0 || colunas <= 0){
        fprintf(stderr, "Erro, dimensoes invalidas na primeira linha\n");
        fclose(f);
        return 0;
    }

    if (fscanf(f, "%d", &chaves) != 1 || chaves < 0){
        fprintf(stderr, "Erro, numero de chaves invalido na segunda linha\n");
        fclose(f);
        return 0;
    }

    lab -> linhas = linhas;
    lab -> colunas = colunas;
    lab -> chavesNecessarias = chaves;
    lab -> tamanhoCaminho = 0;

    lab -> mapa = calloc(linhas, sizeof(char*));
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

    int qtdA = 0,  qtdX = 0, qtdC = 0;
    for (int i = 0; i < linhas; i++){
        for (int j = 0; j < colunas; j++){
            char ch;

            if (fscanf(f, " %c", &ch) != 1){
                fprintf(stderr, "Erro: arquivo terminou antes do esperado (linhas: %d, colunas: %d)\n", i, j);
                fclose(f);
                liberarLabirinto(lab);
                return 0;
            }

            if (ch != '0' && ch != '1' && ch != 'C' && ch != 'X' && ch != 'A'){
                fprintf(stderr, "Erro: caractere invalido '%c' na posicao (%d, %d)\n", ch, i, j);
                fclose(f);
                liberarLabirinto(lab);
                return 0;
            }
            lab -> mapa[i][j] = ch;
            if (ch == 'A'){
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

    fclose(f);

    if (qtdA != 1 || qtdX != 1){
        fprintf(stderr, "Erro: o mapa deve ter exatamente um 'A' e um 'X'\n");
        liberarLabirinto(lab);
        return 0;
    }

    if (qtdC != chaves){
        fprintf(stderr, "Erro: o mapa tem %d chaves, mas sao necessarias %d\n", qtdC, chaves);
        liberarLabirinto(lab);
        return 0;
    }

    return 1;
}

void sincronizarMatrizVisitado(Labirinto *lab) {
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