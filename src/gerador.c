#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mapa.h"

static const int dL[4] = {-1, 1, 0, 0};
static const int dC[4] = {0, 0, -1, 1};

Mapa* AlocarMapa(int linhas, int colunas, int chaves_necessarias) {
    if (linhas <= 0 || colunas <= 0) return NULL;

    Mapa *m = (Mapa*) malloc(sizeof(Mapa));
    if (!m) return NULL;

    m->linhas = linhas;
    m->colunas = colunas;
    m->chaves_necessarias = chaves_necessarias;
    m->inicio_linha = 0;
    m->inicio_coluna = 0;
    m->bau_linha = linhas - 1;
    m->bau_coluna = colunas - 1;

    m->matriz = (char**) malloc(linhas * sizeof(char*));
    if (!m->matriz) {
        free(m);
        return NULL;
    }

    for (int i = 0; i < linhas; i++) {
        m->matriz[i] = (char*) malloc((colunas + 1) * sizeof(char));
        if (!m->matriz[i]) {
            for (int k = 0; k < i; k++) free(m->matriz[k]);
            free(m->matriz);
            free(m);
            return NULL;
        }
        for (int j = 0; j < colunas; j++) {
            m->matriz[i][j] = SALA_VAZIA;
        }
        m->matriz[i][colunas] = '\0';
    }

    return m;
}

void LiberarMapa(Mapa *m) {
    if (!m) return;
    if (m->matriz) {
        for (int i = 0; i < m->linhas; i++) {
            free(m->matriz[i]);
        }
        free(m->matriz);
    }
    free(m);
}

Mapa* CarregarMapaArquivo(const char *nome_arquivo) {
    FILE *fp = fopen(nome_arquivo, "r");
    if (!fp) {
        perror("Erro ao abrir o ficheiro do mapa");
        return NULL;
    }

    int linhas, colunas, chaves;
    if (fscanf(fp, "%d %d", &linhas, &colunas) != 2) {
        fclose(fp);
        return NULL;
    }
    if (fscanf(fp, "%d", &chaves) != 1) {
        fclose(fp);
        return NULL;
    }

    Mapa *m = AlocarMapa(linhas, colunas, chaves);
    if (!m) {
        fclose(fp);
        return NULL;
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            char c;
            if (fscanf(fp, " %c", &c) == 1) {
                m->matriz[i][j] = c;
                if (c == INICIO) {
                    m->inicio_linha = i;
                    m->inicio_coluna = j;
                } else if (c == BAU) {
                    m->bau_linha = i;
                    m->bau_coluna = j;
                }
            }
        }
    }

    fclose(fp);
    return m;
}

int SalvarMapaArquivo(const Mapa *m, const char *nome_arquivo) {
    if (!m) return 0;

    FILE *fp = fopen(nome_arquivo, "w");
    if (!fp) {
        perror("Erro ao criar ficheiro para guardar o mapa");
        return 0;
    }

    fprintf(fp, "%d %d\n", m->linhas, m->colunas);
    fprintf(fp, "%d\n", m->chaves_necessarias);

    for (int i = 0; i < m->linhas; i++) {
        fprintf(fp, "%s\n", m->matriz[i]);
    }

    fclose(fp);
    return 1;
}

static void BaralharDirecoes(int dirs[4]) {
    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = dirs[i];
        dirs[i] = dirs[j];
        dirs[j] = temp;
    }
}

static int ConstruirCaminhoSimples(int l, int c, int passo_atual, int meta,
                                   int linhas, int colunas, int **visitado,
                                   int *cam_l, int *cam_c) {
    visitado[l][c] = 1;
    cam_l[passo_atual] = l;
    cam_c[passo_atual] = c;

    if (passo_atual + 1 == meta) {
        return 1;
    }

    int dirs[4] = {0, 1, 2, 3};
    BaralharDirecoes(dirs);

    for (int i = 0; i < 4; i++) {
        int nl = l + dL[dirs[i]];
        int nc = c + dC[dirs[i]];

        if (nl >= 0 && nl < linhas && nc >= 0 && nc < colunas && !visitado[nl][nc]) {
            if (ConstruirCaminhoSimples(nl, nc, passo_atual + 1, meta,
                                        linhas, colunas, visitado, cam_l, cam_c)) {
                return 1;
            }
        }
    }
    visitado[l][c] = 0;
    return 0;
}

Mapa* GerarMapaParametrizado(ParametrosGeracao params) {
    if (params.linhas < 3) params.linhas = 3;
    if (params.colunas < 3) params.colunas = 3;
    int total_celulas = params.linhas * params.colunas;

    if (params.num_chaves < 0) params.num_chaves = 0;
    if (params.num_chaves > total_celulas - 2) {
        params.num_chaves = total_celulas - 2;
    }
    if (params.densidade_paredes < 0) params.densidade_paredes = 0;
    if (params.densidade_paredes > 100) params.densidade_paredes = 100;

    if (params.semente != 0) {
        srand(params.semente);
    } else {
        srand((unsigned int)time(NULL));
    }

    Mapa *m = AlocarMapa(params.linhas, params.colunas, params.num_chaves);
    if (!m) return NULL;

    int **protegido = (int**) malloc(params.linhas * sizeof(int*));
    for (int i = 0; i < params.linhas; i++) {
        protegido[i] = (int*) calloc(params.colunas, sizeof(int));
    }

    int caminho_criado = 0;

    if (params.garantir_caminho) {
        int min_passos = params.num_chaves + 2;
        int max_passos = (total_celulas > min_passos * 2) ? (total_celulas / 2) : total_celulas;
        if (max_passos < min_passos) max_passos = min_passos;

        int tamanho_caminho = min_passos + (rand() % (max_passos - min_passos + 1));

        int *cam_l = (int*) malloc(tamanho_caminho * sizeof(int));
        int *cam_c = (int*) malloc(tamanho_caminho * sizeof(int));

        int ini_l = rand() % (params.linhas / 2 + 1);
        int ini_c = rand() % params.colunas;

        if (ConstruirCaminhoSimples(ini_l, ini_c, 0, tamanho_caminho,
                                    params.linhas, params.colunas, protegido, cam_l, cam_c)) {
            caminho_criado = 1;

            m->inicio_linha = cam_l[0];
            m->inicio_coluna = cam_c[0];
            m->matriz[m->inicio_linha][m->inicio_coluna] = INICIO;

            m->bau_linha = cam_l[tamanho_caminho - 1];
            m->bau_coluna = cam_c[tamanho_caminho - 1];
            m->matriz[m->bau_linha][m->bau_coluna] = BAU;

            int chaves_colocadas = 0;
            while (chaves_colocadas < params.num_chaves) {
                int idx = 1 + (rand() % (tamanho_caminho - 2));
                int cl = cam_l[idx];
                int cc = cam_c[idx];
                if (m->matriz[cl][cc] == SALA_VAZIA) {
                    m->matriz[cl][cc] = CHAVE;
                    chaves_colocadas++;
                }
            }
        }

        free(cam_l);
        free(cam_c);
    }

    if (!caminho_criado) {
        m->inicio_linha = 0;
        m->inicio_coluna = 0;
        m->matriz[0][0] = INICIO;
        protegido[0][0] = 1;

        m->bau_linha = params.linhas - 1;
        m->bau_coluna = params.colunas - 1;
        m->matriz[m->bau_linha][m->bau_coluna] = BAU;
        protegido[m->bau_linha][m->bau_coluna] = 1;

        int chaves_colocadas = 0;
        while (chaves_colocadas < params.num_chaves) {
            int r = rand() % params.linhas;
            int c = rand() % params.colunas;
            if (!protegido[r][c]) {
                m->matriz[r][c] = CHAVE;
                protegido[r][c] = 1;
                chaves_colocadas++;
            }
        }
    }

    for (int i = 0; i < params.linhas; i++) {
        for (int j = 0; j < params.colunas; j++) {
            if (!protegido[i][j]) {
                if ((rand() % 100) < params.densidade_paredes) {
                    m->matriz[i][j] = PAREDE;
                } else {
                    m->matriz[i][j] = SALA_VAZIA;
                }
            }
        }
        free(protegido[i]);
    }
    free(protegido);

    return m;
}

int GerarESalvarMapa(ParametrosGeracao params, const char *nome_arquivo) {
    Mapa *m = GerarMapaParametrizado(params);
    if (!m) return 0;

    int sucesso = SalvarMapaArquivo(m, nome_arquivo);
    LiberarMapa(m);
    return sucesso;
}
