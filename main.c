#include <stdio.h>
#include <string.h>

#include "headers/labirinto.h"
#include "headers/backtracking.h"
#include "headers/caminho.h"
#include "headers/saida.h"
#include "headers/todos_os_caminhos.h"
#include "headers/analise.h"

#include "headers/menu.h"

#ifdef _WIN32
    #include <windows.h>
#endif


int main(void) {

    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    Labirinto lab = {0};
    TodosOsCaminhos *listaCaminhos = NULL;
    Analise *analise = analise_estado();

    int arquivoCarregado = 0;
    int opcao = -1;
    char entrada[64];
    char caminhoArquivo[256];

    while (opcao != 0) {
        exibirMenu(analise);

        if (!lerLinha(entrada, sizeof(entrada))) {   // EOF (entrada redirecionada acabou)
            opcao = 0;
            printf("\n");
        } else if (sscanf(entrada, "%d", &opcao) != 1) {
            printf("\nOpcao invalida. Digite um numero.\n");
            opcao = -1;
            continue;
        }

        switch (opcao) {
            case 1: {
                descartarLabirinto(&lab, &listaCaminhos, &arquivoCarregado);

                if (gerarLabirinto(&lab)) {
                    arquivoCarregado = 1;
                } else {
                    memset(&lab, 0, sizeof(lab));
                }
                break;
            }

            case 2: {
                printf("\nDigite o caminho do arquivo de entrada (ex: testes/labirinto.txt): ");
                if (!lerLinha(caminhoArquivo, sizeof(caminhoArquivo)) || caminhoArquivo[0] == '\0') {
                    printf("Nenhum arquivo informado.\n");
                    break;
                }

                descartarLabirinto(&lab, &listaCaminhos, &arquivoCarregado);

                if (carregarLabirinto(&lab, caminhoArquivo)) {
                    arquivoCarregado = 1;
                    printf("Labirinto carregado com sucesso!\n");
                    mostrarResumoLabirinto(&lab);
                }

                else {
                    printf("Falha ao carregar o arquivo especificado.\n");
                }
                break;
            }

            case 3: {
                if (!arquivoCarregado) {
                    printf("\nErro: Nenhum labirinto disponivel ainda. Use a Opcao 1 (gerar) ou a Opcao 2 (carregar).\n");
                    break;
                }

                caminho_inicializar(&lab);

                printf("\nBuscando primeiro caminho\n");
                analise_iniciar(analise);
                int encontrou = explorador_labirinto(&lab, lab.inicioL, lab.inicioC, 0);

                if (encontrou) {
                    printf("\nCaminho encontrado:\n");
                    imprimirCaminhoPadrao(&lab);
                }

                else {
                    printf("\n");
                    imprimirSemCaminho();
                }

                analise_imprimir(analise);
                break;
            }

            case 4: {
                if (!arquivoCarregado) {
                    printf("\nErro: Nenhum labirinto disponivel ainda. Use a Opcao 1 (gerar) ou a Opcao 2 (carregar).\n");
                    break;
                }

                caminho_inicializar(&lab);
                sincronizarMatrizVisitado(&lab);
                todos_caminhos_liberar(listaCaminhos);
                listaCaminhos = todos_caminhos_criar();

                if (!listaCaminhos) {
                    printf("Erro ao alocar memoria para a busca de todos os caminhos.\n");
                    break;
                }

                printf("\nBuscando todos os caminhos\n");
                analise_iniciar(analise);
                buscar_todos_os_caminhos(&lab, lab.inicioL, lab.inicioC, 0, listaCaminhos);

                todos_caminhos_imprimir(listaCaminhos);
                analise_imprimir(analise);

                break;
            }

            case 5: {
                if (!arquivoCarregado) {
                    printf("\nErro: Nenhum labirinto disponivel ainda. Use a Opcao 1 (gerar) ou a Opcao 2 (carregar).\n");
                    break;
                }

                if (lab.tamanhoCaminho == 0) {
                    printf("\nExecute primeiro a busca do primeiro caminho (Opcao 3) para gerar o caminho a ser desenhado.\n");
                    break;
                }

                sincronizarMatrizVisitado(&lab);
                imprimirCaminhoCriativo(&lab);
                break;
            }

            case 6: {
                if (!arquivoCarregado) {
                    printf("\nErro: Nenhum labirinto disponivel ainda. Use a Opcao 1 (gerar) ou a Opcao 2 (carregar).\n");
                    break;
                }

                if (listaCaminhos == NULL) {
                    printf("\nExecute primeiro a busca de todos os caminhos (Opcao 4).\n");
                    break;
                }

                imprimirTodosCaminhosCriativo(&lab, listaCaminhos, LIMITE_CRIATIVO);
                break;
            }

            case 7:
                alternarModoAnalise(analise);
                break;

            case 0:
                printf("\nEncerrando o programa...\n");
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }
    }

    todos_caminhos_liberar(listaCaminhos);

    if (arquivoCarregado) {
        liberarLabirinto(&lab);
    }

    return 0;
}
