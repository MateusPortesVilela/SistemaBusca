/* ============================================================
 * exclusao.c  —  Lista de exclusão (stopwords) em C
 *
 * Estrutura: array de strings fixo
 * Ordenação: qsort() após carregamento
 * Busca:     binária — O(log n)
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "exclusao.h"

/* Comparador para qsort (recebe ponteiros para char[MAX_PAL_EXCL]). */
static int cmp_palavras(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

/* ------------------------------------------------------------ */
int exclusao_carregar(ListaExclusao *lista, const char *caminho) {
    lista->tamanho = 0;

    FILE *f = fopen(caminho, "r");
    if (!f) {
        printf("[AVISO] Lista de exclusao nao encontrada: %s\n", caminho);
        printf("        O sistema continuara sem filtrar stopwords.\n");
        return 0;
    }

    char linha[MAX_PAL_EXCL * 2];

    while (fgets(linha, sizeof(linha), f)) {
        if (lista->tamanho >= MAX_EXCLUSAO) break;

        /* Remove \n, \r, espaços no final */
        int len = (int)strlen(linha);
        while (len > 0 && (linha[len-1] == '\n' || linha[len-1] == '\r'
                           || linha[len-1] == ' ' || linha[len-1] == '\t'))
            linha[--len] = '\0';

        /* Ignora linhas vazias e comentários */
        if (len == 0 || linha[0] == '#') continue;

        /* Converte para minúsculas */
        for (int i = 0; linha[i]; i++)
            linha[i] = (char)tolower((unsigned char)linha[i]);

        snprintf(lista->palavras[lista->tamanho], MAX_PAL_EXCL, "%s", linha);
        lista->tamanho++;
    }

    fclose(f);

    /* Ordena para habilitar busca binária */
    qsort(lista->palavras,
          (size_t)lista->tamanho,
          MAX_PAL_EXCL,
          cmp_palavras);

    printf("[OK] Lista de exclusao: %d palavras carregadas e ordenadas.\n",
           lista->tamanho);
    return 1;
}

/* ------------------------------------------------------------ */
int exclusao_contem(const ListaExclusao *lista, const char *palavra) {
    int inicio = 0;
    int fim    = lista->tamanho - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        int cmp  = strcmp(lista->palavras[meio], palavra);

        if (cmp == 0) return 1;   /* encontrou */
        if (cmp < 0)  inicio = meio + 1;
        else          fim    = meio - 1;
    }
    return 0;   /* não encontrou */
}
