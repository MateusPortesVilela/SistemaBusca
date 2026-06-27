/* ============================================================
 * leitor.h  —  Leitura e tokenização de arquivos .txt
 *
 * - Lê tokens separados por espaço/pontuação
 * - Normaliza: minúsculas, remove acentos (Latin-1 e UTF-8)
 * - Consulta lista de exclusão antes de inserir
 * ============================================================ */

#ifndef LEITOR_H
#define LEITOR_H

#include "exclusao.h"

#define MAX_PALAVRA  128   /* Tamanho máximo de uma palavra indexada */

/* Ponteiro para função de inserção no repositório (AVL ou Hash).
   O campo 'repositorio' é passado opaco (void *) para desacoplar. */
typedef void (*FnInserir)(void *repositorio, const char *palavra);

/* Normaliza a string no lugar:
   - Remove caracteres não-alfa (pontuação, dígitos, etc.)
   - Converte para minúsculas
   - Remove acentos (suporte a Latin-1 e UTF-8 português) */
void leitor_normalizar(char *palavra);

/* Lê o arquivo em 'caminho', tokeniza e insere cada palavra válida
   no repositório usando fn_inserir.
   Retorna a quantidade de palavras válidas inseridas, ou -1 em erro. */
int leitor_processar(const char *caminho,
                     const ListaExclusao *exclusao,
                     FnInserir fn_inserir,
                     void *repositorio);

#endif /* LEITOR_H */
