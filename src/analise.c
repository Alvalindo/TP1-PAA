#ifdef __MINGW32__
    #define __USE_MINGW_ANSI_STDIO 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../header/analise.h"

#define LARGURA_MAX_INDENTACAO 40

static struct {
    bool ativo;
    long long limite;              

    bool executando;
    const Labirinto *lab;
    char titulo[96];

    unsigned long long chamadas;
    unsigned long long retrocessos;
    unsigned long long solucoes;
    unsigned long long podaParede;
    unsigned long long podaVisitada;
    unsigned long long podaFora;
    int nivelAtual;
    int nivelMaximo;

    unsigned long long *numeroNoNivel;
    int capacidade;
    bool avisouLimite;

    clock_t inicio;
    double tempoMs;
    bool temResultado;
} A;

void analise_configurar(bool ativo, long long limiteRastreio) {
    A.ativo = ativo;
    A.limite = limiteRastreio;
}

bool analise_ativa(void) {
    return A.ativo;
}

long long analise_limite_rastreio(void) {
    return A.limite;
}

void analise_iniciar(const Labirinto *lab, const char *titulo) {
    free(A.numeroNoNivel);
    A.numeroNoNivel = NULL;
    A.executando = false;
    A.temResultado = false;

    if (!A.ativo || lab == NULL) {
        return;
    }

    A.lab = lab;
    snprintf(A.titulo, sizeof(A.titulo), "%s", titulo ? titulo : "");

    A.chamadas = A.retrocessos = A.solucoes = 0;
    A.podaParede = A.podaVisitada = A.podaFora = 0;
    A.nivelAtual = 0;
    A.nivelMaximo = 0;
    A.avisouLimite = false;
    A.tempoMs = 0.0;

    A.capacidade = lab->linhas * lab->colunas + 4;
    A.numeroNoNivel = calloc(A.capacidade, sizeof(unsigned long long));

    if (A.limite != 0) {
        printf("\nRASTREIO DAS CHAMADAS RECURSIVAS\n");
        printf("(-> entrada na chamada | <- retorno | formato: #n = numero da chamada, nivel = profundidade)\n");
        if (A.limite > 0) {
            printf("(exibindo as primeiras %lld chamadas)\n", A.limite);
        }
    }

    A.executando = true;
    A.inicio = clock();
}

void analise_finalizar(void) {
    if (!A.executando) {
        return;
    }
    A.tempoMs = (double)(clock() - A.inicio) * 1000.0 / CLOCKS_PER_SEC;
    A.executando = false;
    A.temResultado = true;

    free(A.numeroNoNivel);
    A.numeroNoNivel = NULL;
}

static const char* descricaoCelula(int l, int c) {
    if (A.lab == NULL || l < 0 || l >= A.lab->linhas || c < 0 || c >= A.lab->colunas) {
        return "fora do mapa";
    }
    switch (A.lab->mapa[l][c]) {
        case '1': return "parede";
        case 'A': return "inicio";
        case 'C': return "chave";
        case 'X': return "bau";
        default:  return "sala vazia";
    }
}

static void imprimirIndentacao(int nivel) {
    int ind = 2 * (nivel - 1);
    if (ind > LARGURA_MAX_INDENTACAO) {
        ind = LARGURA_MAX_INDENTACAO;
    }
    printf("%*s", ind, "");
}

void analise_entrar(int linha, int coluna, int chavesColetadas) {
    if (!A.executando) {
        return;
    }

    A.chamadas++;
    A.nivelAtual++;
    if (A.nivelAtual > A.nivelMaximo) {
        A.nivelMaximo = A.nivelAtual;
    }

    bool exibir = (A.limite < 0) || (A.chamadas <= (unsigned long long)A.limite);

    if (A.nivelAtual < A.capacidade) {
        A.numeroNoNivel[A.nivelAtual] = exibir ? A.chamadas : 0;
    }

    if (exibir) {
        imprimirIndentacao(A.nivelAtual);
        printf("-> #%llu nivel %d: (%d,%d) %s | chaves ao entrar: %d\n",
               A.chamadas, A.nivelAtual, linha, coluna,
               descricaoCelula(linha, coluna), chavesColetadas);
    } else if (!A.avisouLimite) {
        A.avisouLimite = true;
        printf("  ... rastreio suspenso apos %lld chamadas (os contadores continuam ativos)\n", A.limite);
    }
}

void analise_sair(AnaliseResultado resultado) {
    if (!A.executando) {
        return;
    }

    const char *texto = "";

    switch (resultado) {
        case ANALISE_SOLUCAO:
            A.solucoes++;
            texto = "BAU ABERTO: caminho valido encontrado";
            break;
        case ANALISE_SUCESSO:
            texto = "sucesso propagado para o nivel anterior";
            break;
        case ANALISE_RETROCESSO:
            A.retrocessos++;
            texto = "retrocesso: desfaz a escolha e volta";
            break;
        case ANALISE_PODA_PAREDE:
            A.podaParede++;
            texto = "descartada: parede";
            break;
        case ANALISE_PODA_VISITADA:
            A.podaVisitada++;
            texto = "descartada: sala ja visitada";
            break;
        case ANALISE_PODA_FORA:
            A.podaFora++;
            texto = "descartada: fora do mapa";
            break;
    }

    unsigned long long numero = (A.nivelAtual < A.capacidade) ? A.numeroNoNivel[A.nivelAtual] : 0;
    if (numero != 0) {
        imprimirIndentacao(A.nivelAtual);
        printf("<- #%llu nivel %d: %s\n", numero, A.nivelAtual, texto);
    }

    A.nivelAtual--;
}

void analise_imprimir_resumo(void) {
    if (!A.temResultado) {
        return;
    }

    unsigned long long podas = A.podaParede + A.podaVisitada + A.podaFora;

    printf("\nMODO ANALISE\n");
    printf("Execucao: %s\n", A.titulo);
    printf("\n");
    printf("Chamadas recursivas (total, incluindo a inicial): %llu\n", A.chamadas);
    printf("Nivel maximo de recursao (inicial = nivel 1):     %d\n", A.nivelMaximo);
    printf("Retrocessos (backtracking):                       %llu\n", A.retrocessos);
    printf("Chamadas descartadas de imediato:                 %llu\n", podas);
    if (podas > 0) {
        printf("    parede: %llu | sala ja visitada: %llu | fora do mapa: %llu\n",
               A.podaParede, A.podaVisitada, A.podaFora);
    }
    printf("Vezes que o bau foi aberto (solucoes):            %llu\n", A.solucoes);
    printf("Tempo de execucao:                                %.3f ms\n", A.tempoMs);
    if (A.limite != 0) {
        printf("(o tempo inclui a impressao do rastreio)\n");
    }
}
