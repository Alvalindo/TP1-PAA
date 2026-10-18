#ifndef SAIDA_H
#define SAIDA_H

#include "../header/labirinto.h"
#include "../header/todos_os_caminhos.h"

void imprimirCaminhoPadrao(Labirinto *lab);   // Saída padrão 

void imprimirSemCaminho(void);

void imprimirCaminhoCriativo(Labirinto *lab); //Extra, print criativo

void imprimirTodosCaminhosCriativo(const Labirinto *lab, const TodosOsCaminhos *lista, int limite);   //Extra, desenha cada caminho da lista (opcao 3).

#endif

