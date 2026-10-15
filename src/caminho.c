#include <stdio.h>
#include "../header/caminho.h"

void caminho_inicializar(Labirinto *lab) {
    if (lab != NULL) {
        lab->tamanhoCaminho = 0;
    }
}

int caminho_adicionar(Labirinto *lab, int linha, int coluna) {
    if (lab == NULL || lab->caminho == NULL) {
        return 0;
    }

    int capacidade = lab->linhas * lab->colunas;

    if (lab->tamanhoCaminho >= capacidade) {
        return 0;
    }

    lab->caminho[lab->tamanhoCaminho][0] = linha;
    lab->caminho[lab->tamanhoCaminho][1] = coluna;
    lab->tamanhoCaminho++;

    return 1;
}

void caminho_remover(Labirinto *lab) {
    if (lab != NULL && lab->tamanhoCaminho > 0) {
        lab->tamanhoCaminho--;
    }
}

void caminho_imprimir(const Labirinto *lab) {
    if (lab == NULL) {
        return;
    }

    for (int i = 0; i < lab->tamanhoCaminho; i++) {
        printf("[%d,%d]",
               lab->caminho[i][0],
               lab->caminho[i][1]);

        if (i < lab->tamanhoCaminho - 1) {
            printf(",");
        }
    }

    printf("\n");
}