
#ifndef CAMINHO_H
#define CAMINHO_H

#include "labirinto.h"

// Inicializa o tamanho do caminho
void caminho_inicializar(Labirinto *lab);

// Adiciona uma posição ao final do caminho
int caminho_adicionar(Labirinto *lab, int linha, int coluna);

// Remove a última posição do caminho
void caminho_remover(Labirinto *lab);



#endif
