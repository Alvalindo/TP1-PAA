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
#include "../headers/utils.h"

#define TAMANHO_MAXIMO_GERADO 50          // evita recursao profunda demais (pilha) em mapas gigantes
#define ARQUIVO_GERADO_PADRAO "labirinto_gerado.txt"
#define PASTA_MAPAS "testes/"    // pasta onde os mapas gerados sao guardados
#define RASTREIO_PADRAO 100               // chamadas exibidas no modo analise, por padrao

#ifdef _WIN32
    #include <windows.h>
#endif


// ---------------------------------------------------------------------------
// Leitura de entrada (fgets em vez de scanf, para nao deixar '\n' no buffer)
// ---------------------------------------------------------------------------

// Le uma linha sem o '\n'. Retorna 0 em EOF.
int lerLinha(char *buf, size_t tam) {
    if (fgets(buf, (int)tam, stdin) == NULL) {
        return 0;
    }
    buf[strcspn(buf, "\r\n")] = '\0';
    return 1;
}

// Le um inteiro em [min, max]. Linha vazia = padrao. Repete ate ser valido.
static long long lerInteiro(const char *pergunta, long long min, long long max, long long padrao) {
    char buf[64];

    while (1) {
        printf("%s [%lld]: ", pergunta, padrao);
        if (!lerLinha(buf, sizeof(buf))) {
            return padrao;
        }
        if (buf[0] == '\0') {
            return padrao;
        }

        char *fim;
        long long v = strtoll(buf, &fim, 10);
        if (*fim != '\0') {
            printf("  Valor invalido. Digite um numero inteiro.\n");
        } else if (v < min || v > max) {
            printf("  Valor fora do intervalo permitido (%lld a %lld).\n", min, max);
        } else {
            return v;
        }
    }
}

// Pergunta s/n. Linha vazia = padrao.
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


// ---------------------------------------------------------------------------
// Menu e utilitarios
// ---------------------------------------------------------------------------

void exibirMenu(void) {
    printf("\n=========================================\n");
    printf("   EXPLORADOR DE LABIRINTO   \n");
    printf("=========================================\n");

    printf("1. Gerar mapa de labirinto\n");
    printf("2. Carregar labirinto de arquivo\n");
    printf("3. Resolver (Buscar PRIMEIRO caminho valido)\n");
    printf("4. Resolver (Buscar TODOS os caminhos validos)\n");
    printf("5. Exibir caminho formatado/criativo (apos a opcao 3)\n");
    printf("6. Exibir TODOS os caminhos de forma criativa (apos a opcao 4)\n");
    printf("7. Modo analise: %s\n", analise_ativa() ? "LIGADO (desligar)" : "DESLIGADO (ligar)");
    printf("0. Sair\n");

    printf("=========================================\n");

    printf("Escolha uma opcao: ");
}

// Libera o labirinto atual e os caminhos associados a ele.
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

// Opcao 1: pergunta os parametros, gera o mapa, salva em arquivo e carrega em `lab`
// (assim as demais opcoes do menu usam exatamente o mesmo labirinto gerado).
int gerarLabirinto(Labirinto *lab) {
    ParametrosGeracao p;
    char arquivo[256];

    printf("\n--- GERADOR DE LABIRINTOS ---\n");
    printf("(Enter aceita o valor entre colchetes)\n\n");

    p.linhas  = (int)lerInteiro("Numero de linhas", 3, TAMANHO_MAXIMO_GERADO, 10);
    p.colunas = (int)lerInteiro("Numero de colunas", 3, TAMANHO_MAXIMO_GERADO, 10);

    int maxChaves = p.linhas * p.colunas - 2;     // 'A' e 'X' ocupam duas salas
    long long chavesPadrao = (maxChaves < 2) ? maxChaves : 2;
    p.num_chaves = (int)lerInteiro("Chaves necessarias para abrir o bau", 0, maxChaves, chavesPadrao);

    p.densidade_paredes = (int)lerInteiro("Densidade de paredes (%)", 0, 100, 30);
    p.garantir_caminho  = lerSimNao("Garantir que exista um caminho valido?", 1);

    long long semente = lerInteiro("Semente aleatoria (0 = sortear)", 0, INT_MAX, 0);
    if (semente == 0) {
        semente = (long long)time(NULL) % INT_MAX;   // sorteada aqui para poder ser mostrada e reproduzida
    }
    p.semente = (unsigned int)semente;

    char nome[200];
    printf("Nome do arquivo de saida (salvo em %s) [%s]: ", PASTA_MAPAS, ARQUIVO_GERADO_PADRAO);
    if (!lerLinha(nome, sizeof(nome)) || nome[0] == '\0') {
        snprintf(nome, sizeof(nome), "%s", ARQUIVO_GERADO_PADRAO);
    }
 
    if (strchr(nome, '/') != NULL || strchr(nome, '\\') != NULL) {           // Se o usuario digitou um caminho (com / ou \), usa como esta; senao guarda na pasta de mapas
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

    // O backtracking tem custo exponencial: mapas grandes e abertos podem levar muito tempo
    if (p.linhas * p.colunas > 100 && p.densidade_paredes < 40) {
        printf("Aviso: mapa grande com poucas paredes. A busca por backtracking pode demorar muito\n");
        printf("       (principalmente a opcao 4). Para testes rapidos use ate ~10x10 ou mais paredes.\n");
    }

    imprimirLabirinto(lab);

    return 1;
}

// Opcao 7: liga/desliga o modo analise
void alternarModoAnalise(void) {
    if (analise_ativa()) {
        analise_configurar(false, analise_limite_rastreio());
        printf("\nModo analise DESLIGADO.\n");
        return;
    }

    printf("\nModo analise LIGADO: ao resolver (opcoes 3 e 4) serao exibidos o rastreio das\n");
    printf("chamadas recursivas e o resumo (total de chamadas, nivel maximo, retrocessos, ...).\n");
    printf("Atencao: a busca de TODOS os caminhos faz muitas chamadas; limite o rastreio.\n\n");

    long long limite = lerInteiro("Chamadas recursivas a exibir no rastreio (0 = so resumo, -1 = todas)",
                                  -1, INT_MAX, RASTREIO_PADRAO);
    analise_configurar(true, limite);
}
