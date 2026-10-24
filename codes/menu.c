#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#include "../headers/labirinto.h"
#include "../headers/saida.h"
#include "../headers/todos_os_caminhos.h"
#include "../headers/gerador.h"
#include "../headers/analise.h"
#include "../headers/menu.h"

#define TAMANHO_MAXIMO_GERADO 50
#define ARQUIVO_GERADO_PADRAO "labirinto_gerado.txt"
#define PASTA_MAPAS "testes/"    // pasta onde os mapas gerados sao guardados

#ifdef _WIN32
    #include <windows.h>
#endif

int lerLinha(char *buf, size_t tam) {
    if (fgets(buf, (int)tam, stdin) == NULL) {
        return 0;
    }
    buf[strcspn(buf, "\r\n")] = '\0';
    return 1;
}

static long int lerInteiro(const char *pergunta, long int min, long int max, long int padrao) {
    char buf[64];

    while (1) {
        printf("%s [%ld]: ", pergunta, padrao);
        if (!lerLinha(buf, sizeof(buf))) {
            return padrao;
        }
        if (buf[0] == '\0') {
            return padrao;
        }

        char *fim;
        long int v = strtol(buf, &fim, 10);
        if (*fim != '\0') {
            printf("  Valor invalido. Digite um numero inteiro.\n");
        } else if (v < min || v > max) {
            printf("  Valor fora do intervalo permitido (%ld a %ld).\n", min, max);
        } else {
            return v;
        }
    }
}

static int lerSimNao(const char *pergunta, int padrao) {
    char buf[16];

    while (1) {
        printf("%s (s/n) [%c]: ", pergunta, padrao ? 's' : 'n');
        if (!lerLinha(buf, sizeof(buf)) || buf[0] == '\0') {
            return padrao;
        }
        if (buf[0] == 's' || buf[0] == 'S') return 1;
        if (buf[0] == 'n' || buf[0] == 'N') return 0;
        printf("  Responda com 's' ou 'n'.\n");
    }
}


void exibirMenu(Analise *A) {
    printf("\n=========================================\n");
    printf("   EXPLORADOR DE LABIRINTO   \n");
    printf("=========================================\n");

    printf("1. Gerar mapa de labirinto\n");
    printf("2. Carregar labirinto de arquivo\n");
    printf("3. Resolver (Buscar PRIMEIRO caminho valido)\n");
    printf("4. Resolver (Buscar TODOS os caminhos validos)\n");
    printf("5. Exibir caminho formatado/criativo (apos a opcao 3)\n");
    printf("6. Exibir TODOS os caminhos de forma criativa (apos a opcao 4)\n");
    printf("7. Modo analise: %s\n", analise_ativa(A) ? "LIGADO (desligar)" : "DESLIGADO (ligar)");
    printf("0. Sair\n");

    printf("=========================================\n");

    printf("Escolha uma opcao: ");
}

//libera o labirinto atual e os caminhos associados a ele
void descartarLabirinto(Labirinto *lab, TodosOsCaminhos **lista, int *carregado) {
    todos_caminhos_liberar(*lista);
    *lista = NULL;

    if (*carregado) {
        liberarLabirinto(lab);
        *carregado = 0;
    }
}

void mostrarResumoLabirinto(const Labirinto *lab) {
    printf("Dimensoes: %dx%d | Chaves Necessarias: %d | Posicao Inicial: (%d, %d)\n",
           lab->linhas, lab->colunas, lab->chavesNecessarias, lab->inicioL, lab->inicioC);
}

int gerarLabirinto(Labirinto *lab) {
    ParametrosGeracao p;
    char arquivo[256];

    printf("\n--- GERADOR DE LABIRINTOS ---\n");
    printf("(Enter aceita o valor entre colchetes)\n\n");

    p.linhas  = (int)lerInteiro("Numero de linhas", 3, TAMANHO_MAXIMO_GERADO, 10);
    p.colunas = (int)lerInteiro("Numero de colunas", 3, TAMANHO_MAXIMO_GERADO, 10);

    int maxChaves = p.linhas * p.colunas - 2;
    long int chavesPadrao = (maxChaves < 2) ? maxChaves : 2;
    p.num_chaves = (int)lerInteiro("Chaves necessarias para abrir o bau", 0, maxChaves, chavesPadrao);

    p.densidade_paredes = (int)lerInteiro("Densidade de paredes (%)", 0, 100, 30);
    p.garantir_caminho  = lerSimNao("Garantir que exista um caminho valido?", 1);

    long int semente = lerInteiro("Semente aleatoria (0 = sortear)", 0, INT_MAX, 0);
    if (semente == 0) {
        semente = (long int)time(NULL) % INT_MAX;
    }
    p.semente = (unsigned int)semente;

    char nome[200];
    printf("Nome do arquivo de saida (salvo em %s) [%s]: ", PASTA_MAPAS, ARQUIVO_GERADO_PADRAO);
    if (!lerLinha(nome, sizeof(nome)) || nome[0] == '\0') {
        snprintf(nome, sizeof(nome), "%s", ARQUIVO_GERADO_PADRAO);
    }
 
    if (strchr(nome, '/') != NULL || strchr(nome, '\\') != NULL) {
        snprintf(arquivo, sizeof(arquivo), "%s", nome);
    } else {
        snprintf(arquivo, sizeof(arquivo), "%s%s", PASTA_MAPAS, nome);
    }

    Mapa *mapa = GerarMapaParametrizado(p);
    if (mapa == NULL) {
        printf("Erro: nao foi possivel gerar o mapa.\n");
        return 0;
    }

    int garantido = mapa->caminho_garantido;
    int salvo = SalvarMapaArquivo(mapa, arquivo);
    LiberarMapa(mapa);

    if (!salvo) {
        printf("Erro: nao foi possivel salvar o mapa em '%s'.\n", arquivo);
        return 0;
    }

    if (!carregarLabirinto(lab, arquivo)) {
        printf("Erro: o mapa gerado nao pode ser carregado.\n");
        return 0;
    }

    printf("\nMapa gerado e salvo em '%s' (semente: %u).\n", arquivo, p.semente);
    if (p.garantir_caminho && !garantido) {
        printf("Aviso: nao foi possivel embutir um caminho garantido com esses parametros;\n");
        printf("       o mapa foi gerado de forma aleatoria e pode nao ter solucao.\n");
    } else if (garantido) {
        printf("Existe ao menos um caminho valido neste mapa.\n");
    }
    mostrarResumoLabirinto(lab);

    if (p.linhas * p.colunas > 100 && p.densidade_paredes < 40) {
        printf("Aviso: mapa grande com poucas paredes. A busca por backtracking pode demorar muito\n");
        printf("       (principalmente a opcao 4). Para testes rapidos use ate ~10x10 ou mais paredes.\n");
    }

    imprimirLabirinto(lab);

    return 1;
}

void alternarModoAnalise(Analise *A) {
    analise_configurar(A, !analise_ativa(A));
    printf("\nModo analise %s.\n", analise_ativa(A) ? "LIGADO" : "DESLIGADO");
}
