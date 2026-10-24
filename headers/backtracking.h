#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include <stdio.h>

#include "labirinto.h"

#define Cima -1
#define baixo 1
#define esquerda -1
#define direita 1


//Y = Linha | X = Coluna
//Retonar 1 = achou o báu
//Retornar 0 = não tem caminho
int explorador_labirinto(Labirinto* _labirinto, int YposAtual, int XposAtual, int chavesColetadas); 

//Verifica se o caminho foi visitado
bool caminho_visitado(Labirinto* _labirinto, int YposNova, int XposNova);

#endif
