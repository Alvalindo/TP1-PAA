#ifndef SAIDA_H
#define SAIDA_H

#include "labirinto.h"
#include "todos_os_caminhos.h"

void imprimirCaminhoPadrao(Labirinto *lab);   // Saída padrão 

void imprimirSemCaminho(void);                //Mensagem de "sem solução" da especificação

void imprimirLabirinto(const Labirinto *lab); //Desenha o labirinto (usado após gerar/carregar um mapa)

void imprimirCaminhoCriativo(Labirinto *lab); //Extra, print criativo

void imprimirTodosCaminhosCriativo(const Labirinto *lab, const TodosOsCaminhos *lista, int limite);   //Extra, desenha cada caminho da lista (opcao 3).

#endif
