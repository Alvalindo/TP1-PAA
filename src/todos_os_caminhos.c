#include <stdio.h>
#include <stdlib.h>
#include "../header/todos_os_caminhos.h"
#include "../header/caminho.h"
#include "../header/analise.h"

TodosOsCaminhos* todos_caminhos_criar(void) {
    TodosOsCaminhos *lista = (TodosOsCaminhos*) malloc(sizeof(TodosOsCaminhos));
    if (lista == NULL) {
        return NULL;
    }
    lista->caminhos = NULL;
    lista->quantidade = 0;
    lista->capacidade = 0;
    return lista;
}

int todos_caminhos_adicionar(TodosOsCaminhos *lista, const Labirinto *lab) {
    if (lista == NULL || lab == NULL || lab->caminho == NULL) {
        return 0;
    }

    // Redimensiona o vetor de caminhos se a capacidade máxima for atingida
    if (lista->quantidade >= lista->capacidade) {
        int novaCapacidade = (lista->capacidade == 0) ? 10 : lista->capacidade * 2;
        CaminhoValido *temp = (CaminhoValido*) realloc(lista->caminhos, sizeof(CaminhoValido) * novaCapacidade);
        if (temp == NULL) {
            return 0;
        }
        lista->caminhos = temp;
        lista->capacidade = novaCapacidade;
    }

    // Aloca memória para armazenar as posições do novo caminho
    CaminhoValido *novoCaminho = &lista->caminhos[lista->quantidade];
    novoCaminho->tamanho = lab->tamanhoCaminho;
    novoCaminho->posicoes = (Posicao*) malloc(sizeof(Posicao) * lab->tamanhoCaminho);

    if (novoCaminho->posicoes == NULL) {
        return 0;
    }

    // Copia as posições gravadas no vetor do labirinto
    for (int i = 0; i < lab->tamanhoCaminho; i++) {
        novoCaminho->posicoes[i].linha = lab->caminho[i][0];
        novoCaminho->posicoes[i].coluna = lab->caminho[i][1];
    }

    lista->quantidade++;
    return 1;
}

void todos_caminhos_imprimir(const TodosOsCaminhos *lista) {
    if (lista == NULL || lista->quantidade == 0) {
        printf("Não há um caminho possível para a entrada de dados fornecida\n");
        return;
    }

    printf("=== Total de caminhos encontrados: %d ===\n\n", lista->quantidade);
    for (int i = 0; i < lista->quantidade; i++) {
        printf("Caminho %d:\n", i + 1);
        for (int j = 0; j < lista->caminhos[i].tamanho; j++) {
            printf("[%d,%d]", lista->caminhos[i].posicoes[j].linha, lista->caminhos[i].posicoes[j].coluna);
            if (j < lista->caminhos[i].tamanho - 1) {
                printf(",");
            }
        }
        printf("\n\n");
    }
}

void todos_caminhos_liberar(TodosOsCaminhos *lista) {
    if (lista == NULL) {
        return;
    }

    if (lista->caminhos != NULL) {
        for (int i = 0; i < lista->quantidade; i++) {
            free(lista->caminhos[i].posicoes);
        }
        free(lista->caminhos);
    }
    free(lista);
}

void buscar_todos_os_caminhos(Labirinto *lab, int linha, int coluna, int chavesColetadas, TodosOsCaminhos *lista) {
    //Modo analise: registra a entrada nesta chamada recursiva
    analise_entrar(linha, coluna, chavesColetadas);

    // Validação de limites da matriz, paredes ('1') e posições já visitadas
    if (linha < 0 || linha >= lab->linhas || coluna < 0 || coluna >= lab->colunas) {
        analise_sair(ANALISE_PODA_FORA);
        return;
    }
    if (lab->mapa[linha][coluna] == '1') {
        analise_sair(ANALISE_PODA_PAREDE);
        return;
    }
    if (lab->visitado[linha][coluna]) {
        analise_sair(ANALISE_PODA_VISITADA);
        return;
    }

    char celulaAtual = lab->mapa[linha][coluna];
    AnaliseResultado resultado = ANALISE_RETROCESSO;

    // Se for uma chave 'C', contabiliza a coleta
    if (celulaAtual == 'C') {
        chavesColetadas++;
    }

    // Marca a posição atual como visitada e adiciona ao caminho
    lab->visitado[linha][coluna] = true;
    caminho_adicionar(lab, linha, coluna);

    // Caso de parada: encontrou o baú 'X' com todas as chaves
    if (celulaAtual == 'X' && chavesColetadas == lab->chavesNecessarias) {
        todos_caminhos_adicionar(lista, lab);
        resultado = ANALISE_SOLUCAO;
    } else {
        // Direções de movimentação: Cima, Baixo, Esquerda, Direita
        int dirL[] = {-1, 1, 0, 0};
        int dirC[] = {0, 0, -1, 1};

        for (int d = 0; d < 4; d++) {
            buscar_todos_os_caminhos(lab, linha + dirL[d], coluna + dirC[d], chavesColetadas, lista);
        }
    }

    // Backtracking: desfaz as escolhas para explorar novos caminhos
    caminho_remover(lab);
    lab->visitado[linha][coluna] = false;

    analise_sair(resultado);
}
