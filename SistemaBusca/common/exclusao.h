/* ============================================================
 * exclusao.h  —  Lista de exclusão de palavras (stopwords)
 *
 * Implementação: array de strings ordenado + busca binária O(log n)
 * ============================================================ */

#ifndef EXCLUSAO_H
#define EXCLUSAO_H

#define MAX_EXCLUSAO   1000   /* Máximo de palavras na lista */
#define MAX_PAL_EXCL     64   /* Tamanho máximo de cada palavra */

/* ------------------------------------------------------------ */
typedef struct {
    char palavras[MAX_EXCLUSAO][MAX_PAL_EXCL];
    int  tamanho;
} ListaExclusao;

/* Carrega palavras do arquivo, ordena e prepara para busca binária.
   Retorna 1 em sucesso, 0 em falha (continua com lista vazia). */
int exclusao_carregar(ListaExclusao *lista, const char *caminho);

/* Busca binária — retorna 1 se 'palavra' está na lista, 0 caso contrário. */
int exclusao_contem(const ListaExclusao *lista, const char *palavra);

#endif /* EXCLUSAO_H */
