#ifndef TODOS_OS_CAMINHOS_H
#define TODOS_OS_CAMINHOS_H

#include <stdbool.h>
#include "labirinto.h"

// Estrutura para representar uma posição (linha, coluna) no labirinto
typedef struct {
    int linha;
    int coluna;
} Posicao;

// Estrutura para armazenar uma solução/caminho individual
typedef struct {
    Posicao *posicoes;
    int tamanho;
} CaminhoValido;

// Estrutura dinamicamente redimensionável para armazenar todos os caminhos válidos encontrados
typedef struct {
    CaminhoValido *caminhos;
    int quantidade;
    int capacidade;
} TodosOsCaminhos;


// Cria e inicializa a estrutura para guardar múltiplos caminhos
TodosOsCaminhos* todos_caminhos_criar(void);

// Adiciona o caminho atual contido na estrutura Labirinto para a lista de soluções
int todos_caminhos_adicionar(TodosOsCaminhos *lista, const Labirinto *lab);

// Imprime todos os caminhos no formato especificado ou a mensagem de erro caso nenhum seja encontrado
void todos_caminhos_imprimir(const TodosOsCaminhos *lista);

// Libera toda a memória alocada para os caminhos armazenados
void todos_caminhos_liberar(TodosOsCaminhos *lista);

// Executa a busca por backtracking para enumerar e armazenar TODOS os caminhos válidos
void buscar_todos_os_caminhos(Labirinto *lab, int linha, int coluna, int chavesColetadas, TodosOsCaminhos *lista);

#endif