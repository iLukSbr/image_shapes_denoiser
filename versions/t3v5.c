/* Aluno: Diogo Cademartori Levandoski - Matricula:                    */
/* Aluno: Lucas Yukio Fukuda Matsumoto - Matricula: 2516977                   */
/*============================================================================*/
/* CSF13 - 2022-2 - TRABALHO 3                                                */
/*----------------------------------------------------------------------------*/
/* Bogdan T. Nassu - btnassu@utfpr.edu.br                                     */
/* Leyza E. B. Dorini - leyza@utfpr.edu.br                                    */
/*============================================================================*/
/* Funcao pedida para o 3o trabalho da disciplina Fundamentos de Programacao  */
/* 1, 2o semestre de 2022, prof. Bogdan T. Nassu e Leyza E. B. Dorini,        */
/* Universidade Tecnologica Federal do Parana.                                */
/*============================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "trabalho3.h"
#include "imagem.h"

#ifndef max
    #define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
    #define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#define PRETO 0 // preto rgb
#define BRANCO 255 // branco rgb
#define BRANCO_LIM 80 // intensidade minima do branco
#define BRANCOS_REDOR 4 // quantos pixels brancos ao redor para considerar branco
#define REPETICOES 2 // quantas vezes passar o filtro de ruido

#define RETANG_COMP_MIN 10 // comprimento>=10 eh retangulo
#define RETANG_LARG_MIN 5 // largura>=5 eh retangulo
#define DIAMETRO_MIN 10 // raio>=5, diametro>=10 eh circulo
#define FORMA_LIM 4 // diferenca maxima entre os diametros pra considerar circulo
#define AREA_LIM 10 // diferenca maxima entre as areas pra considerar circulo
#define PERIMETRO_LIM 10 // diferenca maxima entre os perimetros pra considerar circulo

#define ROTULO_INICIAL 256 // rotulo inicial dos ingredientes

typedef struct// guarda as posicoes de cada ponto a ser rotulado
{
    int pos_i;
    int pos_j;
}GuardaPos;

void removeRuido (Imagem1C* ruido);

unsigned int** copiaDados (Imagem1C* copiar);

unsigned int rotulaIngredientes (unsigned int **copia, unsigned int alt, unsigned int larg);

void contaIngredientes (Imagem1C* img, int* n_circulos, int* n_retangulos)
{
    unsigned int i, j, k, rotulo, ingredientes, larg, alt, diagonal2, pixels, **copia;
    double perimetro, area;
    int horizontal, vertical, diagonal;
    *n_circulos = *n_retangulos = 0;

    removeRuido(&(*img));

    copia = copiaDados(&(*img));
    larg = img->largura;
    alt = img->altura;

    //TESTE
    for(i=0; i<alt; i++)
        for(j=0;j<larg;j++)
            img->dados[i][j]=copia[i][j];
    salvaImagem1C(img,"brancoepreto.bmp");
    //TESTE

    ingredientes = rotulaIngredientes(copia, alt, larg);
    printf("passou");
    printf("\n");


    /* verificacao das dimensoes de cada ingrediente */

    for(rotulo = ROTULO_INICIAL; rotulo <= ingredientes; rotulo++)
    {
        pixels = 0;
        horizontal = vertical = diagonal = diagonal2 = 1;
        for(i = 0; i < alt; i++)
        {
            for(j = 0; j < larg; j++)
            {
                if(copia[i][j] == rotulo)
                {
                    pixels++;
                    if(i < alt - 1 && j < larg - 1)
                    {
                        for(k = 1; copia[i][j+k] == rotulo; k++)// mede a maior dimensao horizontal
                        {
                            if(k > horizontal - 1)
                                horizontal++;
                            if(j + k + 1 > larg-1)
                                break;
                        }
                        for(k = 1; copia[i+k][j] == rotulo; k++)// mede a maior dimensao vertical
                        {
                            if(k > vertical - 1)
                                vertical++;
                            if(i + k + 1 > alt - 1)
                                break;
                        }
                        for(k = 1; copia[i+k][j+k] == rotulo; k++)/* mede a maior dimensao diagonal \ */
                        {
                            if(k > diagonal - 1)
                                diagonal++;
                            if(i + k + 1 > alt - 1 || j + k + 1 > larg - 1)
                                break;
                        }
                        if(j > 0)
                        {
                            for(k = 1; copia[i+k][j-k] == rotulo; k++)/* mede a maior dimensao diagonal / */
                            {
                                if(k > diagonal2 - 1)
                                    diagonal2++;
                                if(i + k + 1 > alt - 1 || j - k - 1 < 0)
                                    break;
                            }
                        }
                    }
                }
            }
        }

        printf("\nf%u h%u v%u d%u di%u", rotulo, horizontal, vertical, diagonal, diagonal2);


        /* contagem dos ingredientes por forma */

//        if(max(horizontal, max(vertical, max(diagonal, diagonal2))) - min(horizontal, min(vertical, min(diagonal, diagonal2)) <= FORMA_LIM))
        if((diagonal + diagonal2)/2.0 - (horizontal + vertical)/2.0 >= -FORMA_LIM && (diagonal + diagonal2)/2.0 - (horizontal + vertical)/2.0 <= FORMA_LIM)
        {
            if(horizontal >= DIAMETRO_MIN || vertical >= DIAMETRO_MIN || diagonal >= DIAMETRO_MIN || diagonal2 >= DIAMETRO_MIN)
            {
                (*n_circulos)++;
                printf(" circulo");
            }
        }
        else
        {
            perimetro = pi()*(horizontal + vertical + diagonal + diagonal2)/4;
            area = perimetro*(horizontal + vertical + diagonal + diagonal2)/16;
            if(area - pixels <= AREA_LIM && area - pixels >= -AREA_LIM)
                if(horizontal >= DIAMETRO_MIN || vertical >= DIAMETRO_MIN || diagonal >= DIAMETRO_MIN || diagonal2 >= DIAMETRO_MIN)
                {
                    (*n_circulos)++;
                    printf(" circulo");
                }
            else if(perimetro - pixels_perimetro <= PERIMETRO_LIM && perimetro - pixels_perimetro >= -PERIMETRO_lim)
                if(horizontal >= DIAMETRO_MIN || vertical >= DIAMETRO_MIN || diagonal >= DIAMETRO_MIN || diagonal2 >= DIAMETRO_MIN)
                {
                    (*n_circulos)++;
                    printf(" circulo");
                }
            else if((horizontal >= RETANG_COMP_MIN && vertical >= RETANG_LARG_MIN) || (vertical >= RETANG_COMP_MIN && horizontal >= RETANG_LARG_MIN))
            {
                (*n_retangulos)++;
                printf(" retangulo");
            }
        }
    }

    printf("\n\n");
}

void removeRuido (Imagem1C* matriz)
{
    unsigned int i, j, k;

    for(i = 0; i < matriz->altura; i++)// se aproxima de branco, vira branco, senao vira preto
        for(j = 0; j < matriz->largura; j++)
        {
            if(matriz->dados[i][j] > BRANCO_LIM)
                matriz->dados[i][j] = BRANCO;
            else
                matriz->dados[i][j] = PRETO;
        }

    for(k = 0; k < REPETICOES; k++)// se ao redor eh branco, vira branco
        for(i = 1;i < matriz->altura - 1; i++)
            for(j = 1;j < matriz->largura - 1; j++)
                if((matriz->dados[i-1][j] == BRANCO && matriz->dados[i+1][j] == BRANCO) || (matriz->dados[i][j-1] == BRANCO && matriz->dados[i][j+1] == BRANCO))
                    matriz->dados[i][j] = BRANCO;

    for(k = 0; k < REPETICOES;k++)// se ao redor tem poucos brancos, vira preto
        for(i = 1; i < matriz->altura - 1; i++)
            for(j = 1; j < matriz->largura - 1; j++)
                if(matriz->dados[i-1][j-1] + matriz->dados[i-1][j] + matriz->dados[i-1][j+1] + matriz->dados[i][j-1] + matriz->dados[i][j+1] + matriz->dados[i+1][j-1] + matriz->dados[i+1][j] + matriz->dados[i+1][j+1] < BRANCOS_REDOR*BRANCO)
                    matriz->dados[i][j] = PRETO;

    for(i = 1; i < matriz->altura - 1; i++)// se ao redor tem tantos brancos, vira branco
        for(j = 1; j < matriz->largura - 1; j++)
            if(matriz->dados[i-1][j-1] + matriz->dados[i-1][j] + matriz->dados[i-1][j+1] + matriz->dados[i][j-1] + matriz->dados[i][j+1] + matriz->dados[i+1][j-1] + matriz->dados[i+1][j] + matriz->dados[i+1][j+1] >= BRANCOS_REDOR*BRANCO)
                matriz->dados[i][j] = BRANCO;
}

unsigned int** copiaDados (Imagem1C* matriz)// cria copia da matriz de dados da img
{
    unsigned int i, j, **copia;
    copia = (unsigned int**) malloc(matriz->altura*sizeof(unsigned int*));
    for(i = 0; i < matriz->altura; i++)
        copia[i] = (unsigned int*) malloc(matriz->largura*sizeof(unsigned int));
    for(i = 0; i < matriz->altura; i++)
        for(j = 0; j < matriz->largura; j++)
            copia[i][j] = matriz->dados[i][j];
    return copia;
}

unsigned int rotulaIngredientes (unsigned int** copia, unsigned int alt, unsigned int larg)
{
    unsigned int i, j, k, rotulo=ROTULO_INICIAL, tem_branco, cont=0, a=0, contador;
    GuardaPos pos[5000];
    for(i = 0; i < alt; i++)// percorre a matriz até achar um branco
    {
        for(j = 0; j < larg; j++)
        {
            if(copia[i][j] == BRANCO)
            {
                copia[i][j] = rotulo;
                pos[0].pos_i = i;
                pos[0].pos_j = j;
                tem_branco = 1;
                while(tem_branco)
                {
                    tem_branco = 0;
                    contador = cont;
                    for(k = a; k <= contador; k++)// pra cada posicao no vetor de struct ele acessa e colore
                    {
                         if((pos[k].pos_i-1>=0) && copia[pos[k].pos_i-1][pos[k].pos_j]==BRANCO)// pixel superior
                        {
                            copia[pos[k].pos_i-1][pos[k].pos_j]=rotulo;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i-1;
                            pos[cont].pos_j=pos[k].pos_j;
                            tem_branco=1;
                        }
                        if((pos[k].pos_i+1<alt&&pos[k].pos_j+1<larg) && copia[pos[k].pos_i+1][pos[k].pos_j+1]==BRANCO)// pixel diagonal direito inferior
                        {
                            copia[pos[k].pos_i+1][pos[k].pos_j+1]=rotulo;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i+1;
                            pos[cont].pos_j=pos[k].pos_j+1;
                            tem_branco=1;
                        }
                        if((pos[k].pos_j-1>=0) && copia[pos[k].pos_i][pos[k].pos_j-1]==BRANCO)// pixel esquerdo
                        {
                            copia[pos[k].pos_i][pos[k].pos_j-1]=rotulo;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i;
                            pos[cont].pos_j=pos[k].pos_j-1;
                            tem_branco=1;
                        }
                        if((pos[k].pos_i-1>=0&&pos[k].pos_j+1<larg) && (copia[pos[k].pos_i-1][pos[k].pos_j+1]==BRANCO))// pixel diagonal direito superior
                        {
                            copia[pos[k].pos_i-1][pos[k].pos_j+1]=rotulo;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i-1;
                            pos[cont].pos_j=pos[k].pos_j+1;
                            tem_branco=1;
                        }
                       if(pos[k].pos_i+1<larg && copia[pos[k].pos_i+1][pos[k].pos_j]==BRANCO)// pixel inferior
                        {
                            copia[pos[k].pos_i+1][pos[k].pos_j]=rotulo;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i+1;
                            pos[cont].pos_j=pos[k].pos_j;
                            tem_branco=1;
                        }
                        if((pos[k].pos_i-1>=0&&pos[k].pos_j>=0) && copia[pos[k].pos_i-1][pos[k].pos_j-1]==BRANCO)// pixel diagonal esquerdo superior
                        {
                            copia[pos[k].pos_i-1][pos[k].pos_j-1]=rotulo;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i-1;
                            pos[cont].pos_j=pos[k].pos_j-1;
                            tem_branco=1;
                        }

                        if(j+1<alt && copia[pos[k].pos_i][pos[k].pos_j+1]==BRANCO)// pixel direito
                        {
                            copia[pos[k].pos_i][pos[k].pos_j+1]=rotulo;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i;
                            pos[cont].pos_j=pos[k].pos_j+1;
                            tem_branco=1;
                        }
                        if((pos[k].pos_i+1<=alt&&pos[k].pos_j>=0) && copia[pos[k].pos_i+1][pos[k].pos_j-1]==BRANCO)// pixel diagonal esquerdo inferior
                        {
                            copia[pos[k].pos_i-1][pos[k].pos_j-1]=rotulo;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i-1;
                            pos[cont].pos_j=pos[k].pos_j-1;
                            tem_branco=1;
                        }
                    }
                    a = k + 1;
                }
                printf("Ingrediente %u", rotulo);
                rotulo++;
                a = 0;
                cont = 0;
            }
        }
    }
    rotulo--;
    return rotulo;
}
