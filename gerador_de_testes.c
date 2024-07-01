/*============================================================================*/
/* GERADOR DE CASOS DE TESTE PARA O PROBLEMA DOS TEMPEROS                     */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu - nassu@dainf.ct.utfpr.edu.br                       */
/*============================================================================*/
/** Um gerador de casos de teste para o problema dos temperos. Geramos uma
 * imagem preta contendo círculos e retângulos, com ruído e suavizações. */
/*============================================================================*/

#include <stdlib.h>
#include <math.h>

#include "gerador_de_testes.h"

/*============================================================================*/

#define N_TAMANHOS 3
int LARGURAS [] = {768,1280,1920};
int ALTURAS [] = {432,720,1080};

#define MIN(a,b) ((a<b)? a : b)
#define MAX(a,b) ((a>b)? a : b)

/* Desenhando. */
#define LARGURA_GRID_MIN 30
#define LARGURA_GRID_MAX 100
#define CHANCE_OBJ_MIN 10
#define CHANCE_OBJ_MAX 50
#define RAIO_CIRCULO_MIN 10
#define ALTURA_RETANGULO_MIN 5
#define LARGURA_RETANGULO_MIN 10
#define MARGEM_OBJ 4

/* Ruído e suavização. */
#define MAX_PROB_RUIDO_BINARIO 1000 /* Probabilidade máxima de ruído binário, multiplicada por 100 (i.e. 100 = 1%) */
#define PARAMETRO_SUAVIZACAO 4 /* Quanto menor, mais borradas as imagens tendem a ficar. */
#define SIGMA_RUIDO 25 /* Quanto maior, mais ruído. */

/*============================================================================*/
/* Funções internas do módulo. */

void desenhaObjetos (Imagem1C* img, int* n_circulos, int* n_retangulos);
void desenhaCirculo (Imagem1C* img, int x, int y, int raio);
void desenhaRetangulo (Imagem1C* img, int x, int y, int w, int h);
void ruidoBinario (Imagem1C* img);
void suavizaImagem (Imagem1C* in, Imagem1C* out);
void adicionaRuido (Imagem1C* img);
double rand_normal (double mean, double stddev);

/*============================================================================*/
/** Função central do gerador, cria um caso de testes.
 *
 * Parâmetros: int seed: a semente para o gerador de números aleatórios.
 *             int* n_circulos: parâmetro de saída, contém o número de
 *               círculos.
 *             int* n_retangulos: parâmetro de saída, contém o número de
 *               retângulos.
 *
 * Valor de retorno: uma imagem contendo o caso de teste. Lembre-se de destruí-la! */

Imagem1C* geraCasoDeTeste (int seed, int* n_circulos, int* n_retangulos)
{
    Imagem1C *img, *suavizada;
    int i, j, tam;

    srand (seed);

    /* Escolhe um tamanho e cria a imagem. */
    tam = rand () % N_TAMANHOS;
    img = criaImagem1C (LARGURAS [tam], ALTURAS [tam]);

    /* Pinta a imagem de preto. */
    for (i = 0; i < img->altura; i++)
        for (j = 0; j < img->largura; j++)
            img->dados [i][j] = 0;

    /* Desenha. */
    desenhaObjetos (img, n_circulos, n_retangulos);

    /* Para complicar um pouco mais as coisas... */
    ruidoBinario (img);
    suavizada = criaImagem1C (img->largura, img->altura);
    suavizaImagem (img, suavizada);
    adicionaRuido (suavizada);

    destroiImagem1C (img);

    return (suavizada);
}

/*----------------------------------------------------------------------------*/
/** Desenha objetos dos dois tipos na imagem dada. O tamanho e quantidade de
 * objetos é definido de forma mais ou menos aleatória. Implementação simples
 * usando a ideia de uma quadtree: uma região pode ser dividida em 4, ignorada
 * ou preenchida com um objeto.
 *
 * Parâmetros: Imagem1C* img: desenha aqui.
 *             int* n_circulos: parâmetro de saída, contém o número de
 *               círculos.
 *             int* n_retangulos: parâmetro de saída, contém o número de
 *               retângulos.
 *
 * Valor de retorno: nenhum. */

 /* Sub-função da desenhaObjetos: é uma função recursiva que pode desenhar ou não um objeto e/ou repartir a região em 4.*/
void _desenhaObjetosBase (Imagem1C* img, int* n_circulos, int* n_retangulos, int x, int y, int w, int h, int chance_circulo, int chance_retangulo)
{
    int pode_dividir;

    /* Caso-base: tamanho é pequeno demais -> não faz nada. */
    if (w < LARGURA_GRID_MIN || h < LARGURA_GRID_MIN)
        return;

    /* Verifica se tem condições de dividir em 4... */
    pode_dividir = (w/2 >= LARGURA_GRID_MIN) && (h/2 >= LARGURA_GRID_MIN);

    /* Sorteia para ver se divide a região em 4 (50% de chance). Se ainda estiver muito grande, SEMPRE faz isso. */
    if ((w > LARGURA_GRID_MAX || h > LARGURA_GRID_MAX) ||
        (pode_dividir && (rand ()%2)))
    {
        _desenhaObjetosBase (img, n_circulos, n_retangulos, x, y, w/2, h/2, chance_circulo, chance_retangulo);
        _desenhaObjetosBase (img, n_circulos, n_retangulos, x+w/2, y, w-w/2, h/2, chance_circulo, chance_retangulo);
        _desenhaObjetosBase (img, n_circulos, n_retangulos, x, y+h/2, w/2, h-h/2, chance_circulo, chance_retangulo);
        _desenhaObjetosBase (img, n_circulos, n_retangulos, x+w/2, y+h/2, w-w/2, h-h/2, chance_circulo, chance_retangulo);
    }
    else
    {
        /* Finalizou esta região. Decide se desenha algo aqui. */
        int sorteio = rand () % 100;
        if (sorteio < chance_circulo)
        {
            int raio = RAIO_CIRCULO_MIN + rand () % (MIN(w,h)/2-MARGEM_OBJ-RAIO_CIRCULO_MIN);
            int cx = x + raio + MARGEM_OBJ + rand () % (w-raio*2-MARGEM_OBJ*2);
            int cy = y + raio + MARGEM_OBJ + rand () % (h-raio*2-MARGEM_OBJ*2);
            desenhaCirculo (img, cx, cy, raio);
            (*n_circulos)++;
        }
        else if (sorteio < chance_retangulo)
        {
            int w_retangulo = LARGURA_RETANGULO_MIN + rand () % (w-MARGEM_OBJ*2-LARGURA_RETANGULO_MIN);
            int h_retangulo = ALTURA_RETANGULO_MIN + rand () % (h-MARGEM_OBJ*2-ALTURA_RETANGULO_MIN);
            int x_retangulo = x + MARGEM_OBJ + rand () % (w - MARGEM_OBJ - w_retangulo);
            int y_retangulo = y + MARGEM_OBJ + rand () % (h - MARGEM_OBJ - h_retangulo);
            desenhaRetangulo (img, x_retangulo, y_retangulo, w_retangulo, h_retangulo);
            (*n_retangulos)++;
        }
    }
}

void desenhaObjetos (Imagem1C* img, int* n_circulos, int* n_retangulos)
{
    *n_circulos = 0;
    *n_retangulos = 0;

    /* Define as chances de ter um objeto de cada tipo em cada posição, para este caso de teste. */
    int chance_circulo, chance_retangulo;
    chance_circulo = CHANCE_OBJ_MIN + rand () % (CHANCE_OBJ_MAX - CHANCE_OBJ_MIN + 1);
    chance_retangulo = chance_circulo + CHANCE_OBJ_MIN + rand () % (CHANCE_OBJ_MAX - CHANCE_OBJ_MIN + 1);

    /* Preenche com a sub-função recursiva, iniciando com a área total da imagem. */
    _desenhaObjetosBase (img, n_circulos, n_retangulos, 0, 0, img->largura, img->altura, chance_circulo, chance_retangulo);
}

/*----------------------------------------------------------------------------*/
/** Desenha um círculo branco na imagem dada. O raio não inclui o pixel
 * central, então o círculo ocupará uma região que vai de (x-raio,y-raio) a
 * (x+raio,y+raio), inclusive.
 *
 * Parâmetros: Imagem1C* img: desenha aqui.
 *             int x, int y: centro do círculo.
 *             int raio: raio do círculo.
 *
 * Valor de retorno: nenhum */

void desenhaCirculo (Imagem1C* img, int x, int y, int raio)
{
    int i, j, lim;

    for (i = -raio; i <= raio; i++)
    {
        lim = sqrt (raio*raio - i*i);
        for (j = -lim; j <= lim; j++)
            img->dados [y + i][x + j] = 255;
    }
}

/*----------------------------------------------------------------------------*/
/** Desenha um retângulo branco na imagem dada.
 *
 * Parâmetros: Imagem1C* img: desenha aqui.
 *             int x, int y: coordenadas do canto superior esquerdo.
 *             int w, int h: largura e altura.
 *
 * Valor de retorno: nenhum */

void desenhaRetangulo (Imagem1C* img, int x, int y, int w, int h)
{
    int i, j;
    for (i = y; i < y+h; i++)
        for (j = x; j < x+w; j++)
            img->dados [i][j] = 255;
}

/*----------------------------------------------------------------------------*/
/** Adiciona ruído binário a uma imagem. Inverte cada byte que for sorteado.
 *
 * Parâmetros: Imagem1C* img: imagem a alterar.
 *
 * Valor de retorno: NENHUM */

void ruidoBinario (Imagem1C* img)
{
    int i, j, prob;

    /* Define a probabilidade de ruído binário. */
    prob = rand () % (MAX_PROB_RUIDO_BINARIO+1);

    for (i = 0; i < img->altura; i++)
        for (j = 0; j < img->largura; j++)
            if (rand () % 10000 < prob)
                img->dados [i][j] = ~img->dados [i][j];
}

/*----------------------------------------------------------------------------*/
/** Suaviza a imagem com um filtro 3x3. O peso do pixel central é escolhido
 * aleatoriamente, ou seja, a imagem pode ficar mais ou menos borrada.
 *
 * Parâmetros: Imagem1C* in: imagem a suavizar.
 *             Imagem1C* out: imagem de saída, já que não dá para fazer a
 *               suavização in-place.
 *
 * Valor de retorno: nenhum */

void suavizaImagem (Imagem1C* in, Imagem1C* out)
{
	unsigned long row, col;
	int peso_centro;

    /* Faz uma borda preta. */
    for (row = 0; row < in->altura; row++)
    {
        out->dados [row][0] = 0;
        out->dados [row][out->largura-1] = 0;
    }

    for (col = 0; col < in->largura; col++)
    {
        out->dados [0][col] = 0;
        out->dados [out->altura-1][0] = 0;
    }

    /* Agora borra. */
	peso_centro = rand () % PARAMETRO_SUAVIZACAO + 4;
    for (row = 1; row < in->altura-1; row++)
		for (col = 1; col < in->largura-1; col++)
            out->dados [row][col] = (in->dados [row-1][col-1] +
                                    in->dados [row-1][col]*2 +
                                    in->dados [row-1][col+1] +
			                        in->dados [row][col-1]*2 +
									in->dados [row][col]*peso_centro +
									in->dados [row][col+1]*2 +
									in->dados [row+1][col-1] +
									in->dados [row+1][col]*2 +
									in->dados [row+1][col+1])/(peso_centro + 12);
}

/*----------------------------------------------------------------------------*/
/** Adiciona ruído a uma imagem.
 *
 * Parâmetros: Imagem1C* img: imagem à qual adicionaremos ruído.
 *
 * Valor de retorno: nenhum. */

void adicionaRuido (Imagem1C* img)
{
	unsigned long row, col;

    for (row = 0; row < img->altura; row++)
        for (col = 0; col < img->largura; col++)
        {
            double ruido = rand_normal (0, SIGMA_RUIDO);
            img->dados [row][col] = (unsigned char) (MIN (255, MAX (0, img->dados [row][col] + ruido)));
        }
}

/*----------------------------------------------------------------------------*/
/** Código copiado de outro lugar para gerar números aleatórios seguindo uma
 * distribuição normal. Original em:
 * en.literateprograms.org/Box-Muller_transform_%28C%29#chunk%20use:complete%20Box-Muller%20function */

double rand_normal (double mean, double stddev)
{
	static double n2 = 0.0;
	static int n2_cached = 0;
	if (!n2_cached) {
		double x, y, r;
		do {
			x = 2.0*rand()/RAND_MAX - 1;
			y = 2.0*rand()/RAND_MAX - 1;
			r = x*x + y*y;
		} while (r == 0.0 || r > 1.0);
		{
			double result;
			double d = sqrt(-2.0*log(r)/r);
			double n1 = x*d;
			n2 = y*d;
			result = n1*stddev + mean;
			n2_cached = 1;
			return result;
		}
	}
	else {
		n2_cached = 0;
		return n2*stddev + mean;
	}
}

/*============================================================================*/
