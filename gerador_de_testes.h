/*============================================================================*/
/* GERADOR DE CASOS DE TESTE PARA O PROBLEMA DOS TEMPEROS                     */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu - nassu@dainf.ct.utfpr.edu.br                       */
/*============================================================================*/
/** Um gerador de casos de teste para o problema dos temperos. Geramos uma
 * imagem preta contendo círculos e retângulos, com ruído e suavizações. */
/*============================================================================*/

#ifndef __GERADOR_DE_TESTES_H
#define __GERADOR_DE_TESTES_H

/*============================================================================*/

#include "imagem.h"

/*============================================================================*/

Imagem1C* geraCasoDeTeste (int seed, int* n_circulos, int* n_retangulos);

/*============================================================================*/
#endif /* __GERADOR_DE_TESTES_H */
