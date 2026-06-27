/* ============================================================
 * hash.c  —  Tabela Hash com Listas Encadeadas
 *
 * Função hash: djb2  →  hash = hash * 33 + c
 * Colisões:    encadeamento separado (listas ligadas por bucket)
 * Inserção:    O(1) amortizado | Busca: O(1) amortizado
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

/* ============================================================ */
/*  Função hash djb2                                             */
/* ============================================================ */
unsigned int hash_funcao(const char *palavra) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*palavra++) != '\0')
        hash = ((hash << 5) + hash) + (unsigned long)c;  /* hash * 33 + c */
    return (unsigned int)(hash % TAM_TABELA);
}

/* ============================================================ */
/*  Interface pública                                            */
/* ============================================================ */

void hash_inicializar(TabelaHash *t) {
    for (int i = 0; i < TAM_TABELA; i++)
        t->tabela[i] = NULL;
    t->total_unicos      = 0;
    t->total_ocorrencias = 0;
    t->colisoes          = 0;
    t->buckets_usados    = 0;
}

/* ------------------------------------------------------------ */
void hash_inserir(TabelaHash *t, const char *palavra) {
    unsigned int idx = hash_funcao(palavra);
    NoHash *atual    = t->tabela[idx];

    /* Percorre a lista do bucket procurando a palavra */
    while (atual) {
        if (strcmp(atual->palavra, palavra) == 0) {
            /* Já existe: apenas incrementa o contador */
            atual->ocorrencias++;
            t->total_ocorrencias++;
            return;
        }
        atual = atual->proximo;
    }

    /* Palavra nova: cria nó e insere no início da lista */
    NoHash *novo = (NoHash *)malloc(sizeof(NoHash));
    if (!novo) {
        fprintf(stderr, "[ERRO] Memoria insuficiente!\n");
        exit(EXIT_FAILURE);
    }
    strncpy(novo->palavra, palavra, MAX_PAL_HASH - 1);
    novo->palavra[MAX_PAL_HASH - 1] = '\0';
    novo->ocorrencias = 1;
    novo->proximo     = t->tabela[idx];

    /* Contabiliza colisão se o bucket já tinha elemento */
    if (t->tabela[idx] != NULL)
        t->colisoes++;
    else
        t->buckets_usados++;

    t->tabela[idx] = novo;
    t->total_unicos++;
    t->total_ocorrencias++;
}

/* ------------------------------------------------------------ */
int hash_buscar(const TabelaHash *t, const char *palavra) {
    unsigned int idx = hash_funcao(palavra);
    const NoHash *atual = t->tabela[idx];

    while (atual) {
        if (strcmp(atual->palavra, palavra) == 0)
            return atual->ocorrencias;
        atual = atual->proximo;
    }
    return 0;   /* não encontrada */
}

/* ------------------------------------------------------------ */
void hash_listar(const TabelaHash *t) {
    if (t->total_unicos == 0) {
        printf("  Repositorio vazio.\n");
        return;
    }
    printf("  %-35s %6s\n", "PALAVRA", "OCORR.");
    printf("  %-35s %6s\n",
           "-----------------------------------",
           "------");

    for (int i = 0; i < TAM_TABELA; i++) {
        const NoHash *atual = t->tabela[i];
        while (atual) {
            printf("  %-35s %6d\n", atual->palavra, atual->ocorrencias);
            atual = atual->proximo;
        }
    }
}

/* ------------------------------------------------------------ */
void hash_liberar(TabelaHash *t) {
    for (int i = 0; i < TAM_TABELA; i++) {
        NoHash *atual = t->tabela[i];
        while (atual) {
            NoHash *prox = atual->proximo;
            free(atual);
            atual = prox;
        }
        t->tabela[i] = NULL;
    }
    t->total_unicos      = 0;
    t->total_ocorrencias = 0;
    t->colisoes          = 0;
    t->buckets_usados    = 0;
}
