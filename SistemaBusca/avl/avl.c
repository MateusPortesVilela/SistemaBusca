/* ============================================================
 * avl.c  —  Árvore AVL para indexação de palavras
 *
 * Operações:
 *   - Inserção com balanceamento automático (rotações simples e duplas)
 *   - Busca iterativa O(log n)
 *   - Listagem em ordem alfabética (percurso in-order)
 *   - Liberação de memória (pós-ordem)
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

/* ============================================================ */
/*  Funções auxiliares internas                                  */
/* ============================================================ */

static int maximo(int a, int b) { return a > b ? a : b; }

/* Retorna a altura do nó (0 se NULL). */
int avl_altura_no(const NoAVL *no) {
    return no ? no->altura : 0;
}

/* Fator de balanceamento: altura(esq) - altura(dir). */
static int fator_bal(const NoAVL *no) {
    return avl_altura_no(no->esq) - avl_altura_no(no->dir);
}

/* Recalcula a altura de um nó a partir de seus filhos. */
static void atualizar_altura(NoAVL *no) {
    no->altura = 1 + maximo(avl_altura_no(no->esq),
                            avl_altura_no(no->dir));
}

/* ------------------------------------------------------------ */
/*  Alocação de nó                                              */
/* ------------------------------------------------------------ */
static NoAVL *criar_no(const char *palavra) {
    NoAVL *no = (NoAVL *)malloc(sizeof(NoAVL));
    if (!no) {
        fprintf(stderr, "[ERRO] Memoria insuficiente!\n");
        exit(EXIT_FAILURE);
    }
    strncpy(no->palavra, palavra, MAX_PAL_AVL - 1);
    no->palavra[MAX_PAL_AVL - 1] = '\0';
    no->ocorrencias = 1;
    no->altura = 1;
    no->esq = no->dir = NULL;
    return no;
}

/* ============================================================ */
/*  Rotações AVL                                                 */
/* ============================================================ */

/*
 * Rotação simples à direita (caso Left-Left):
 *
 *       y                x
 *      / \              / \
 *     x   T3   --->   T1   y
 *    / \                  / \
 *   T1  T2              T2  T3
 */
static NoAVL *rotacao_direita(NoAVL *y) {
    NoAVL *x  = y->esq;
    NoAVL *T2 = x->dir;
    x->dir = y;
    y->esq = T2;
    atualizar_altura(y);
    atualizar_altura(x);
    return x;   /* nova raiz */
}

/*
 * Rotação simples à esquerda (caso Right-Right):
 *
 *     x                  y
 *    / \                / \
 *   T1   y    --->    x   T3
 *       / \          / \
 *      T2  T3       T1  T2
 */
static NoAVL *rotacao_esquerda(NoAVL *x) {
    NoAVL *y  = x->dir;
    NoAVL *T2 = y->esq;
    y->esq = x;
    x->dir = T2;
    atualizar_altura(x);
    atualizar_altura(y);
    return y;   /* nova raiz */
}

/*
 * Aplica as rotações necessárias para restaurar o balanceamento.
 * Assume que a altura do nó já está desatualizada após uma inserção.
 */
static NoAVL *balancear(NoAVL *no) {
    atualizar_altura(no);
    int fb = fator_bal(no);

    /* Left-Left → rotação simples à direita */
    if (fb > 1 && fator_bal(no->esq) >= 0)
        return rotacao_direita(no);

    /* Left-Right → rotação dupla (esquerda + direita) */
    if (fb > 1 && fator_bal(no->esq) < 0) {
        no->esq = rotacao_esquerda(no->esq);
        return rotacao_direita(no);
    }

    /* Right-Right → rotação simples à esquerda */
    if (fb < -1 && fator_bal(no->dir) <= 0)
        return rotacao_esquerda(no);

    /* Right-Left → rotação dupla (direita + esquerda) */
    if (fb < -1 && fator_bal(no->dir) > 0) {
        no->dir = rotacao_direita(no->dir);
        return rotacao_esquerda(no);
    }

    return no;  /* já balanceado */
}

/* ============================================================ */
/*  Inserção recursiva                                           */
/* ============================================================ */
static NoAVL *inserir_rec(NoAVL *no, const char *palavra, int *e_novo) {
    /* Caso base: posição vazia — cria novo nó */
    if (!no) {
        *e_novo = 1;
        return criar_no(palavra);
    }

    int cmp = strcmp(palavra, no->palavra);

    if (cmp < 0) {
        no->esq = inserir_rec(no->esq, palavra, e_novo);
    } else if (cmp > 0) {
        no->dir = inserir_rec(no->dir, palavra, e_novo);
    } else {
        /* Palavra já existe: incrementa contador, sem rebalanceamento */
        no->ocorrencias++;
        return no;
    }

    return balancear(no);
}

/* ============================================================ */
/*  Interface pública                                            */
/* ============================================================ */

void avl_inicializar(ArvoreAVL *t) {
    t->raiz              = NULL;
    t->total_unicos      = 0;
    t->total_ocorrencias = 0;
}

void avl_inserir(ArvoreAVL *t, const char *palavra) {
    int e_novo = 0;
    t->raiz = inserir_rec(t->raiz, palavra, &e_novo);
    if (e_novo) t->total_unicos++;
    t->total_ocorrencias++;
}

int avl_buscar(const ArvoreAVL *t, const char *palavra) {
    const NoAVL *atual = t->raiz;
    while (atual) {
        int cmp = strcmp(palavra, atual->palavra);
        if (cmp == 0) return atual->ocorrencias;
        atual = (cmp < 0) ? atual->esq : atual->dir;
    }
    return 0;   /* não encontrada */
}

/* Percurso in-order: esquerda → raiz → direita (ordem alfabética). */
static void inorder(const NoAVL *no) {
    if (!no) return;
    inorder(no->esq);
    printf("  %-35s %6d\n", no->palavra, no->ocorrencias);
    inorder(no->dir);
}

void avl_listar(const ArvoreAVL *t) {
    if (!t->raiz) {
        printf("  Repositorio vazio.\n");
        return;
    }
    printf("  %-35s %6s\n", "PALAVRA", "OCORR.");
    printf("  %-35s %6s\n",
           "-----------------------------------",
           "------");
    inorder(t->raiz);
}

/* Pós-ordem para liberação segura (filhos antes do pai). */
static void liberar_rec(NoAVL *no) {
    if (!no) return;
    liberar_rec(no->esq);
    liberar_rec(no->dir);
    free(no);
}

void avl_liberar(ArvoreAVL *t) {
    liberar_rec(t->raiz);
    t->raiz              = NULL;
    t->total_unicos      = 0;
    t->total_ocorrencias = 0;
}
