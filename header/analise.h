#ifndef ANALISE_H
#define ANALISE_H

#include <stdbool.h>
#include "../header/labirinto.h"

/*
 * MODO ANALISE
 *
 * Instrumenta as funcoes recursivas (explorador_labirinto e
 * buscar_todos_os_caminhos) sem alterar a logica do backtracking.
 * Quando o modo esta DESLIGADO, todas as funcoes abaixo retornam
 * imediatamente (custo desprezivel).
 *
 * Convencao: a chamada inicial da recursao esta no nivel 1.
 *
 * Uso tipico (ver main.c):
 *     analise_iniciar(&lab, "titulo");
 *     ... busca recursiva (que chama analise_entrar / analise_sair) ...
 *     analise_finalizar();
 *     analise_imprimir_resumo();
 */

// Como cada chamada recursiva terminou
typedef enum {
    ANALISE_SOLUCAO,        // achou o bau com todas as chaves
    ANALISE_SUCESSO,        // devolve o sucesso para o nivel anterior (busca do 1o caminho)
    ANALISE_RETROCESSO,     // esgotou as opcoes: desfaz a escolha e volta (backtracking)
    ANALISE_PODA_PAREDE,    // chamada imediatamente descartada: parede
    ANALISE_PODA_VISITADA,  // chamada imediatamente descartada: sala ja visitada
    ANALISE_PODA_FORA       // chamada imediatamente descartada: fora do mapa
} AnaliseResultado;

// Liga/desliga o modo analise e define quantas chamadas recursivas serao
// exibidas no rastreio (0 = so contadores, -1 = todas).
void analise_configurar(bool ativo, long long limiteRastreio);
bool analise_ativa(void);
long long analise_limite_rastreio(void);

// Ciclo de uma execucao
void analise_iniciar(const Labirinto *lab, const char *titulo);
void analise_finalizar(void);
void analise_imprimir_resumo(void);

// Ganchos chamados de dentro das funcoes recursivas
void analise_entrar(int linha, int coluna, int chavesColetadas);
void analise_sair(AnaliseResultado resultado);

#endif
