#include <stdio.h>
#include "../headers/analise.h"

Analise A;

Analise *analise_estado(void) { return &A; }

void analise_configurar(Analise *A, bool ativo) {
    A->ativo = ativo;

}
bool analise_ativa(Analise *A) {
    return A->ativo;
}

void analise_iniciar(Analise *A) {
    A->chamadas = 0;
    A->nivelAtual = 0;
    A->nivelMaximo = 0;
}

void analise_entrar(Analise *A) {
    A->chamadas++;
    A->nivelAtual++;
    if (A->nivelAtual > A->nivelMaximo) A->nivelMaximo = A->nivelAtual;
}

void analise_sair(Analise *A) {
    A->nivelAtual--;
}

void analise_imprimir(Analise *A) {
    if (!A->ativo)
        return;

    printf("\n--- Modo análise ---\n");
    printf("Chamadas recursivas: %lld\n", A->chamadas);
    printf("Nível máximo de recursão: %d\n", A->nivelMaximo);
}
