#ifndef GERADOR_H
#define GERADOR_H

// Elementos do labirinto
#define SALA_VAZIA '0'
#define PAREDE     '1'
#define INICIO     'A'
#define CHAVE      'C'
#define BAU        'X'

// Estrutura do Mapa
typedef struct {
    int linhas;
    int colunas;
    int chaves_necessarias;
    int inicio_linha;
    int inicio_coluna;
    int bau_linha;
    int bau_coluna;
    char **matriz;
    int caminho_garantido;   // 1 = o gerador conseguiu embutir um caminho valido no mapa
} Mapa;

// Estrutura de parâmetros para o Gerador de Mapas
typedef struct {
    int linhas;              // Número de linhas do labirinto
    int colunas;             // Número de colunas do labirinto
    int num_chaves;          // Quantidade de chaves 'C'
    int densidade_paredes;   // Percentagem de paredes
    int garantir_caminho;    // 1 = caminho válido sem repetir salas; 0 = aleatório
    unsigned int semente;    // Semente para rand()
} ParametrosGeracao;


Mapa* AlocarMapa(int linhas, int colunas, int chaves_necessarias);
void LiberarMapa(Mapa *m);
Mapa* CarregarMapaArquivo(const char *nome_arquivo);
int SalvarMapaArquivo(const Mapa *m, const char *nome_arquivo);
Mapa* GerarMapaParametrizado(ParametrosGeracao params);
int GerarESalvarMapa(ParametrosGeracao params, const char *nome_arquivo);

#endif