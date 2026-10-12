#ifndef LABIRINTO_H
#define LABIRINTO_H

#include <stbool.h>

typedef struct Labirinto{
    int linhas, colunas;
    int chavesNecessarias;
    char **mapa;
    bool **visitado;
    int (*caminho)[2];          //caminho[i] = [linha,coluna]
    int tamanho;
    int inicioL, inicioC;
} Labirinto;


//return 0 = erro, 1 = sucesso

void liberarLabirinto(Labirinto *lab);

int carregarLabirinto(Labirinto *lab, const char *nomeArquivo);

