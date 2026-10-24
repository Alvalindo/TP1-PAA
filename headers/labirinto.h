#ifndef LABIRINTO_H
#define LABIRINTO_H

#include <stdbool.h>

typedef struct Labirinto{
    int linhas, colunas;    //Dimensões do labirinto
    int chavesNecessarias;   //Quantidade de chaves necessárias para sair do labirinto
    char **mapa;    //Matriz que representa o mapa do labirinto
    bool **visitado;    //Matriz que indica se a sala foi visitada ou não
    int (*caminho)[2];          //caminho[i] = [linha,coluna]
    int tamanhoCaminho;      //Quantidade de posições do caminho
    int inicioL, inicioC;     //Coordenadas iniciais do personagem
} Labirinto;


//return 0 = erro, 1 = sucesso

void liberarLabirinto(Labirinto *lab);       //Libera a memoria utilizada pela estrutura.

int carregarLabirinto(Labirinto *lab, const char *nomeArquivo);      //Carrega e valida o labirinto. Retorna 1 se der certo e 0 se houver erro.

void sincronizarMatrizVisitado(Labirinto *lab);         //Atualiza a matriz visitado com base no caminho armazenado.

#endif 