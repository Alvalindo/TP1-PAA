#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

#include "labirinto.h"
#include "todos_os_caminhos.h"

#define LIMITE_CRIATIVO 10

int lerLinha(char *buf, size_t tam);
void exibirMenu(void);
void descartarLabirinto(Labirinto *lab, TodosOsCaminhos **lista, int *carregado);
void mostrarResumoLabirinto(const Labirinto *lab);
int gerarLabirinto(Labirinto *lab);
void alternarModoAnalise(void);

#endif
