/*============================================================================*/
/* CSF13 - 2022-2 - TRABALHO 3                                                */
/*----------------------------------------------------------------------------*/
/* Bogdan T. Nassu - btnassu@utfpr.edu.br                                     */
/* Leyza E. B. Dorini - leyza@utfpr.edu.br                                    */
/*============================================================================*/
/** Teste para o trabalho 3 (Temperos). */
/*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "gerador_de_testes.h"
#include "trabalho3.h"

/*============================================================================*/
/* Um modificador para a semente usada pelo gerador de números aleatórios na
 * hora de criar imagens de teste. Todos os trabalhos usarão as mesmas
 * imagens, então o offset precisa ser igual para todos. Entretanto, usaremos
 * um valor diferente na hora de testar os trabalhos de fato. */

#define RANDOM_SEED_OFFSET 1

#define N_TESTES 1

#define SALVA_INTERMEDIARIOS 1 /* Flag que diz se devemos salvar as imagens de teste. Desative se for rodar muitos testes! */
#define MOSTRA_INTERMEDIARIOS 1 /* Flag que diz se devemos mostrar os resultados de cada. Desative se for rodar muitos testes! */

/*============================================================================*/

int main ()
{
	int i;
	Imagem1C* img; /* Imagem de teste. */
	int n_circulos_real, n_retangulos_real, n_circulos_medido, n_retangulos_medido, n_circulos_total = 0, n_retangulos_total = 0;
	clock_t tempo_inicio, tempo_total = 0;

    int erro_conta, erro_classifica;
	int erro_total_conta = 0, erro_total_classifica = 0;
	int pior_teste_conta = -1, pior_erro_conta = 0;
	int pior_teste_classifica = -1, pior_erro_classifica = 0;

    /* Realiza N_TESTES testes. */
	for (i = 0; i < N_TESTES; i++)
	{
	    if (MOSTRA_INTERMEDIARIOS)
        {
            printf ("----------------------------------------\n");
            printf ("Teste %d\n", i);
        }

		/* Gera um caso de teste. */
		img = geraCasoDeTeste (i+RANDOM_SEED_OFFSET, &n_circulos_real, &n_retangulos_real);
		n_circulos_total += n_circulos_real;
		n_retangulos_total += n_retangulos_real;
		if (!img)
		{
			printf ("Erro gerando caso de teste (avisar o professor!!!).\n");
			return (1);
		}

		/* Salva (se necessário). */
		if (SALVA_INTERMEDIARIOS)
		{
			char foostring [64];
			sprintf (foostring, "teste%d.bmp", i);
			salvaImagem1C (img, foostring);
		}

		/* Invoca o testador. */
		tempo_inicio = clock ();
		contaIngredientes (img, &n_circulos_medido, &n_retangulos_medido);
		tempo_total += clock () - tempo_inicio;

		/* Compara os resultados com o que foi gerado. */
		erro_conta = abs (n_circulos_real + n_retangulos_real - (n_circulos_medido + n_retangulos_medido));
		erro_classifica = abs (n_circulos_real - n_circulos_medido) + abs (n_retangulos_real - n_retangulos_medido);
		erro_total_conta += erro_conta;
		erro_total_classifica += erro_classifica;
		if (MOSTRA_INTERMEDIARIOS)
            printf ("Real: %d circulos e %d retangulos (%d total)\nDetectado: %d circulos e %d retangulos (%d total)\nErros (conta): %d\nErros (classifica): %d\n", n_circulos_real, n_retangulos_real, n_circulos_real+n_retangulos_real, n_circulos_medido, n_retangulos_medido, n_circulos_medido+n_retangulos_medido, erro_conta, erro_classifica);

        /* Guarda o pior teste. */
        if (erro_conta > pior_erro_conta)
        {
            pior_erro_conta = erro_conta;
            pior_teste_conta = i;
        }

        if (erro_classifica > pior_erro_classifica)
        {
            pior_erro_classifica = erro_classifica;
            pior_teste_classifica = i;
        }

		/* Joga fora as imagens geradas. */
		destroiImagem1C (img);
	}

    /* Mostra os resultados finais. */
    printf ("----------------------------------------\n");
    printf ("%d circulos e %d retangulos (%d total)\n", n_circulos_total, n_retangulos_total, n_circulos_total+n_retangulos_total);
	printf ("Erro total (conta): %d (%.2f%%)\n", erro_total_conta, 100.0*(erro_total_conta / ((float) n_circulos_total+n_retangulos_total)));
	printf ("Erro total (classifica): %d (%.2f%%)\n", erro_total_classifica, 100.0*(erro_total_classifica / ((float) n_circulos_total+n_retangulos_total)));
	printf ("Pior teste (conta): %d (%d erros)\n", pior_teste_conta, pior_erro_conta);
	printf ("Pior teste (classifica): %d (%d erros)\n", pior_teste_classifica, pior_erro_classifica);
	printf ("Tempo total: %d\n", (int) tempo_total);

	return (0);
}

/*============================================================================*/
