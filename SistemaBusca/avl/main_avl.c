/* ============================================================
 * main_avl.c  —  Programa principal: versão com Árvore AVL
 *
 * Uso: busca_avl.exe
 *      Os arquivos a carregar são informados interativamente.
 *
 * Dependências:
 *   common/exclusao.h + exclusao.c
 *   common/leitor.h   + leitor.c
 *   avl/avl.h         + avl.c
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "avl.h"
#include "../common/exclusao.h"
#include "../common/leitor.h"

/* ============================================================ */
/*  Wrapper: adapta avl_inserir para a assinatura FnInserir     */
/* ============================================================ */
static void fn_inserir_avl(void *repo, const char *palavra) {
    avl_inserir((ArvoreAVL *)repo, palavra);
}

/* ============================================================ */
/*  Utilitários de interface                                     */
/* ============================================================ */


static void imprimir_separador(void) {
    printf("------------------------------------------------------------\n");
}

static void imprimir_cabecalho(void) {
    printf("\n");
    printf("============================================================\n");
    printf("      SISTEMA DE BUSCA DE PALAVRAS  |  Versao: AVL Tree     \n");
    printf("============================================================\n");
}

static void imprimir_menu(void) {
    printf("\n");
    imprimir_separador();
    printf("  MENU PRINCIPAL\n");
    imprimir_separador();
    printf("  1. Carregar arquivo(s) .txt\n");
    printf("  2. Buscar palavra\n");
    printf("  3. Listar todas as palavras (ordem alfabetica)\n");
    printf("  4. Exibir estatisticas\n");
    printf("  5. Sair\n");
    imprimir_separador();
    printf("  Opcao: ");
}

/* ============================================================ */
/*  Opção 1 — Carregar arquivo(s)                               */
/* ============================================================ */
static void menu_carregar(ArvoreAVL *arvore, const ListaExclusao *excl) {
    char caminho[512];

    printf("\n  Digite o caminho do arquivo .txt (ou aperte Enter para voltar):\n");
    printf("  > ");
    if (!fgets(caminho, sizeof(caminho), stdin)) return;

    /* Remove \n */
    int len = (int)strlen(caminho);
    while (len > 0 && (caminho[len-1] == '\n' || caminho[len-1] == '\r'))
        caminho[--len] = '\0';

    if (caminho[0] == '\0') return;

    printf("  Carregando: %s\n", caminho);
    int inseridas = leitor_processar(caminho, excl, fn_inserir_avl, arvore);
    if (inseridas >= 0) {
        printf("  Carregamento concluido! Total inserido: %d\n", inseridas);
        printf("  Palavras unicas no repositorio: %d\n", arvore->total_unicos);
    }
}

/* ============================================================ */
/*  Opção 2 — Buscar palavra                                    */
/* ============================================================ */
static void menu_buscar(const ArvoreAVL *arvore) {
    char palavra[128];

    printf("\n  Digite a palavra a buscar: ");
    if (!fgets(palavra, sizeof(palavra), stdin)) return;
    int len = (int)strlen(palavra);
    while (len > 0 && (palavra[len-1] == '\n' || palavra[len-1] == '\r'))
        palavra[--len] = '\0';

    /* Normaliza a consulta da mesma forma que as palavras indexadas */
    leitor_normalizar(palavra);

    if (strlen(palavra) == 0) {
        printf("  Palavra invalida.\n");
        return;
    }

    int ocorr = avl_buscar(arvore, palavra);
    imprimir_separador();
    if (ocorr > 0)
        printf("  Resultado: \"%s\" encontrada %d vez(es).\n", palavra, ocorr);
    else
        printf("  Resultado: \"%s\" NAO encontrada no repositorio.\n", palavra);
    imprimir_separador();
}

/* ============================================================ */
/*  Opção 3 — Listar palavras                                   */
/* ============================================================ */
static void menu_listar(const ArvoreAVL *arvore) {
    printf("\n");
    imprimir_separador();
    printf("  PALAVRAS INDEXADAS (ordem alfabetica) - total: %d\n",
           arvore->total_unicos);
    imprimir_separador();
    avl_listar(arvore);
    imprimir_separador();
}

/* ============================================================ */
/*  Opção 4 — Estatísticas                                      */
/* ============================================================ */
static void menu_estatisticas(const ArvoreAVL *arvore) {
    int altura = avl_altura_no(arvore->raiz);
    double altura_ideal = (arvore->total_unicos > 0)
                        ? log2((double)arvore->total_unicos + 1.0)
                        : 0.0;

    printf("\n");
    imprimir_separador();
    printf("  ESTATISTICAS - Arvore AVL\n");
    imprimir_separador();
    printf("  Palavras unicas         : %d\n",   arvore->total_unicos);
    printf("  Total de ocorrencias    : %d\n",   arvore->total_ocorrencias);
    printf("  Altura da arvore        : %d\n",   altura);
    printf("  Altura ideal (log2 n+1) : %.1f\n", altura_ideal);
    printf("  Fator de balanceamento  : %.2fx acima do ideal\n",
           altura_ideal > 0 ? (double)altura / altura_ideal : 0.0);
    imprimir_separador();
}

/* ============================================================ */
/*  main                                                         */
/* ============================================================ */
int main(void) {
    ListaExclusao exclusao;
    ArvoreAVL     arvore;

    imprimir_cabecalho();

    /* Carrega lista de exclusão */
    printf("\n");
    exclusao_carregar(&exclusao, "dados/exclusao.txt");

    /* Inicializa a árvore AVL */
    avl_inicializar(&arvore);

    /* Loop principal do menu */
    int rodando = 1;
    while (rodando) {
        imprimir_menu();

        char entrada[8];
        if (!fgets(entrada, sizeof(entrada), stdin)) break;
        int opcao = atoi(entrada);

        switch (opcao) {
            case 1: menu_carregar(&arvore, &exclusao);    break;
            case 2: menu_buscar(&arvore);                  break;
            case 3: menu_listar(&arvore);                  break;
            case 4: menu_estatisticas(&arvore);            break;
            case 5: rodando = 0;                           break;
            default:
                printf("  Opcao invalida. Tente novamente.\n");
        }
    }

    /* Libera memória */
    avl_liberar(&arvore);
    printf("\n  Encerrando. Memoria liberada. Ate logo!\n\n");
    return 0;
}
