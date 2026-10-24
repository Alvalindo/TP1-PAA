#ifndef ANALISE_H
#define ANALISE_H

#include <stdbool.h>

typedef struct {
    bool ativo;
    long long chamadas;
    int nivelAtual;
    int nivelMaximo;
} Analise;

Analise *analise_estado(void);

void analise_configurar(Analise *A, bool ativo);

bool analise_ativa(Analise *A);

void analise_iniciar(Analise *A);

void analise_entrar(Analise *A);

void analise_sair(Analise *A);

void analise_imprimir(Analise *A);

#endif
