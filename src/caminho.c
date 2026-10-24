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

