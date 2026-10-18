#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header/labirinto.h"
#include "header/backtracking.h"
#include "header/caminho.h"
#include "header/saida.h"
#include "header/todos_os_caminhos.h"

#define LIMITE_CRIATIVO 10

#ifdef _WIN32
    #include <windows.h>
#endif


void exibirMenu(void) {
    printf("\n=========================================\n");
    printf("   EXPLORADOR DE LABIRINTO   \n");
    printf("=========================================\n");

    printf("1. Carregar labirinto de arquivo\n");
    printf("2. Resolver (Buscar PRIMEIRO caminho valido)\n");
    printf("3. Resolver (Buscar TODOS os caminhos validos)\n");
    printf("4. Exibir caminho formatado/criativo\n");
    printf("5. Exibir TODOS os caminhos de forma criativa (apos a opcao 3)\n");
    printf("0. Sair\n");

    printf("=========================================\n");

    printf("Escolha uma opcao: ");
}

int main(void) {

    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    Labirinto lab = {0};
    TodosOsCaminhos *listaCaminhos = NULL;
    
    int arquivoCarregado = 0;
    int opcao = -1;
    char caminhoArquivo[256];

    while (opcao != 0) {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            printf("\nOpcao invalida. Digite um numero.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (opcao) {
            case 1: {
                printf("\nDigite o caminho do arquivo de entrada (ex: entrada.txt): ");
                scanf("%255s", caminhoArquivo);

                if (arquivoCarregado) {
                    todos_caminhos_liberar(listaCaminhos);
                     listaCaminhos = NULL;
                    liberarLabirinto(&lab);
                    arquivoCarregado = 0;
                }

                if (carregarLabirinto(&lab, caminhoArquivo)) {
                    arquivoCarregado = 1;
                    printf("Labirinto carregado com sucesso!\n");
                    printf("Dimensoes: %dx%d | Chaves Necessarias: %d | Posicao Inicial: (%d, %d)\n", lab.linhas, lab.colunas, lab.chavesNecessarias, lab.inicioL, lab.inicioC);
                } 
                
                else {
                    printf("Falha ao carregar o arquivo especificado.\n");
                }
                break;
            }

            case 2: {
                if (!arquivoCarregado) {
                    printf("\nErro: Nenhum labirinto foi carregado ainda. Use a Opcao 1.\n");
                    break;
                }

                caminho_inicializar(&lab);

                printf("\nBuscando primeiro caminho\n");
                int encontrou = explorador_labirinto(&lab, lab.inicioL, lab.inicioC, 0);

                if (encontrou) {
                    printf("\nCaminho encontrado:\n");
                    imprimirCaminhoPadrao(&lab);
                } 
                
                else {
                    printf("\nnao ha um caminho possivel para a entrada de dados fornecida\n");
                }

                break;
            }

            case 3: {
                if (!arquivoCarregado) {
                    printf("\nErro: Nenhum labirinto foi carregado ainda. Use a Opcao 1.\n");
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
                buscar_todos_os_caminhos(&lab, lab.inicioL, lab.inicioC, 0, listaCaminhos);
                todos_caminhos_imprimir(listaCaminhos);

                
                break;
            }

            case 4: {
                if (!arquivoCarregado) {
                    printf("\nErro: Nenhum labirinto foi carregado ainda. Use a Opcao 1.\n");
                    break;
                }

                if (lab.tamanhoCaminho == 0) {
                    printf("\nExecute primeiro a busca (Opcao 2) para gerar o caminho a ser desenhado.\n");
                    break;
                }

                sincronizarMatrizVisitado(&lab);
                imprimirCaminhoCriativo(&lab);
                break;
            }

            case 5: {
                if (!arquivoCarregado) {
                    printf("\nErro: Nenhum labirinto foi carregado ainda. Use a Opcao 1.\n");
                    break;
                }
 
                if (listaCaminhos == NULL) {
                    printf("\nExecute primeiro a busca de todos os caminhos (Opcao 3).\n");
                    break;
                }
 
                imprimirTodosCaminhosCriativo(&lab, listaCaminhos, LIMITE_CRIATIVO);
                break;
            }

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