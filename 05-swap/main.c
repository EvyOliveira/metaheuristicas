#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0

/* 1º Definição de uma estrutura representando matriz */
struct matriz {
    int numero_elementos;
    int** elementos;
};

/* 2º Definição de uma estrutura representando o nodo para construção aleatória */
struct nodo {
    int indice;
    int valor; /* Custo da aresta */
};

void ler_arquivo(struct matriz*, char*);
int calcular_custo(struct matriz, int*);
/* 3º Relacionado à construção gulosa */
void construir_caminho(struct matriz, int*);
/* 4º Relacionado à construção do caminho aleatório */
void construir_caminho_aleatorio(struct matriz, int*);

void realizar_movimento_troca(struct matriz, int*, int*);
void copiar_caminho(struct matriz, int*, int*);
/* 5º Explicação Imprimir Caminho */
void imprimir_caminho(int, int*);

void imprimir_matriz(struct matriz);
void linha();

int main(int argc, char *argv[]) {
    struct matriz m;
    /* 6º Leitura de arquivo com 10 elementos e o valor conhecido será 25 */
    ler_arquivo(&m, "/Users/gleissonassis/Dropbox/CH/instancias/10_01_25.txt");
    imprimir_matriz(m);

    int *solucao_inicial = malloc((m.numero_elementos + 1) * sizeof(int));
    construir_caminho(m, solucao_inicial);
    printf("Solucao inicial gulosa: ");
    imprimir_caminho(m.numero_elementos + 1, solucao_inicial);
    int custo_solucao_inicial = calcular_custo(m, solucao_inicial);
    printf("Custo solução inicial gulosa: %d\n", custo_solucao_inicial);

    /* 11º Estratégica gulosa do vizinho mais próximo */
    int* melhor_vizinho = malloc((m.numero_elementos + 1) * sizeof(int));
    realizar_movimento_troca(m, solucao_inicial, melhor_vizinho);
    printf("Solucao melhor vizinho: ");
    imprimir_caminho(m.numero_elementos + 1, melhor_vizinho);
    int custo_solucao_melhor_vizinho = calcular_custo(m, melhor_vizinho);
    printf("Custo solução melhor vizinho: %d\n", custo_solucao_melhor_vizinho);
}

//-----------------------------------------------------------------------------

void copiar_caminho(struct matriz m, int* origem, int* destino) {
    int i;

    for(i = 0; i <= m.numero_elementos; i++) {
        destino[i] = origem[i];
    }
}

//-----------------------------------------------------------------------------

void realizar_movimento_troca(struct matriz m, int* solucao_inicial, int* melhor_vizinho) {
    int* solucao_tmp = malloc((m.numero_elementos + 1) * sizeof(int));
    int custo_referencia = calcular_custo(m, solucao_inicial);

    copiar_caminho(m, solucao_inicial, melhor_vizinho);

    for(int i = 1; i < m.numero_elementos; i++) {
        copiar_caminho(m, solucao_inicial, solucao_tmp);

        for(int j = i + 1; j < m.numero_elementos; j++) {

            int tmp = solucao_tmp[i];
            solucao_tmp[i] = solucao_tmp[j];
            solucao_tmp[j] = tmp;

            int custo_solucao_tmp = calcular_custo(m, solucao_tmp);

            if(custo_solucao_tmp < custo_referencia) {
                custo_referencia = custo_solucao_tmp;
                copiar_caminho(m, solucao_tmp, melhor_vizinho);
            }
        }
    }
}

//-----------------------------------------------------------------------------
/* 7º Através de ponteiros, lerá a quantidade de elementos da matriz */

void ler_arquivo(struct matriz* m, char* arquivo) {
    FILE* fp = fopen(arquivo, "r");

    fscanf(fp, "%d\n", &m->numero_elementos);

    m->elementos = malloc(m->numero_elementos * sizeof(int*));

    for(int i = 0; i < m->numero_elementos; i++) {
        m->elementos[i] = malloc(m->numero_elementos * sizeof(int));
        for(int j = 0; j < m->numero_elementos; j++) {
            fscanf(fp, "%d ", &m->elementos[i][j]);
        }
    }


    fclose(fp);
}

//-----------------------------------------------------------------------------
/* 9º Calcularaá custo da aresta */

int calcular_custo(struct matriz m, int* caminho) {
    int custo = 0;

    for(int i = 0; i < m.numero_elementos; i++) {
        custo = custo + m.elementos[caminho[i]][caminho[i + 1]];
    }

    return custo;
}

//-----------------------------------------------------------------------------
/* 8º Definirá o caminho usando o menor valor da aresta do dodecaedro */

void construir_caminho(struct matriz m, int* caminho) {
    int *inseridos = malloc(m.numero_elementos * sizeof(int));

    for(int i = 0; i < m.numero_elementos; i++) {
        inseridos[i] = FALSE;
    }

    caminho[0] = 0;
    inseridos[0] = TRUE;

    for(int i = 0; i < m.numero_elementos; i++) {
        int valor_referencia = INT_MAX;
        int vizinho_selecionado = 0;

        for(int j = 0; j < m.numero_elementos; j++) {
            if(!inseridos[j] && valor_referencia > m.elementos[i][j]) {
                vizinho_selecionado = j;
                valor_referencia = m.elementos[i][j];
            }
        }

        caminho[i + 1] = vizinho_selecionado;
        inseridos[vizinho_selecionado] = TRUE;
    }

    caminho[m.numero_elementos] = 0;

    free(inseridos);
}



//-----------------------------------------------------------------------------
/* 10º Definirá o caminho randomicamente dentro de opções possíveis */

void construir_caminho_aleatorio(struct matriz m, int* caminho) {
    int *inseridos = malloc(m.numero_elementos * sizeof(int));
    struct nodo *vizinhos = malloc(m.numero_elementos * sizeof(struct nodo));

    for(int i = 0; i < m.numero_elementos; i++) {
        inseridos[i] = FALSE;
    }

    caminho[0] = 0;
    inseridos[0] = TRUE;

    for(int i = 0; i < m.numero_elementos; i++) {
        int iv = 0;

        for(int j = 0; j < m.numero_elementos; j++) {
            if(!inseridos[j]) {
                vizinhos[iv].indice = j;
                vizinhos[iv].valor = m.elementos[i][j];
                iv++;
            }
        }

        if(iv == 0) {
            caminho[i + 1] = 0;
        } else {
            int vizinho_selecionado = rand() % iv;
            caminho[i + 1] = vizinhos[vizinho_selecionado].indice;
            inseridos[vizinhos[vizinho_selecionado].indice] = TRUE;
        }
    }

    free(inseridos);
    free(vizinhos);
}

//-----------------------------------------------------------------------------
/* Impressão da matriz para analisar as possibilidades */
void imprimir_matriz(struct matriz m) {
    linha();
    printf("Matriz\n\n");

    for(int i = 0; i < m.numero_elementos; i++) {
        for(int j = 0; j < m.numero_elementos; j++) {
            printf("%d ", m.elementos[i][j]);
        }

        printf("\n");
    }

    linha();
}

//-----------------------------------------------------------------------------
/* Impressão do caminho escolhido */
void imprimir_caminho(int n, int* caminho) {
    int i;

    for(i = 0; i < n; i++) {
        printf("%d ", caminho[i]);
    }
    printf("\n");
}

//-----------------------------------------------------------------------------
/* Separação de caracteres */
void linha() {
    int i;
    printf("\n");
    for(i = 0; i < 80; i++) printf("_");
    printf("\n");
}
