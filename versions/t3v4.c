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

//https://pyimagesearch.com/2016/02/08/opencv-shape-detection/
//https://hal.inria.fr/hal-01656212/document

#include <stdlib.h>
#include <stdio.h>

#include "trabalho3.h"
#include "imagem.h"

#ifndef max
    #define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
    #define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#define PRETO 0
#define BRANCO_LIM 80 // intensidade minima do branco
#define BRANCO 255

#define RETANG_LIM_COMP 10 // comprimento>=10 e retangulo
#define RETANG_LIM_LARG 5 // largura>=5 e retangulo
#define DIAMETRO_LIM 10 // raio>=5, diametro>=10 e circulo
#define FORMATO_LIM 10 // limite da diferenca entre os diametros pra considerar circulo

#define COR_INICIAL 256 // rotulo inicial das figuras

#define CANAL_R 0 // constante usada para se referir ao canal vermelho
#define CANAL_G 1 // constante usada para se referir ao canal verde
#define CANAL_B 2 // constante usada para se referir ao canal azul

/* retangulos azuis */
#define R_RET 0
#define G_RET 64
#define B_RET 255

/* circulos amarelos */
#define R_CIRC 250
#define G_CIRC 253
#define B_CIRC 15

typedef struct// struct pra guardar as posições de cada ponto a ser colorido
{
    int pos_i;
    int pos_j;

}GuardaPos;

void contaIngredientes (Imagem1C* img, int* n_circulos, int* n_retangulos)
{
    unsigned int i, j, k, cor, tem_branco = 1, cont = 0, a = 0,contador, figuras, m, n, larg, alt, diagonal2, **copia;
    double media1, media2;
    int horizontal, vertical, diagonal;
    *n_circulos = *n_retangulos = 0;
    cor = COR_INICIAL;
    GuardaPos pos[5000];
    Imagem3C *img1, *img2;

    copia = (unsigned int**) malloc(img->altura*sizeof(unsigned int*));// cria copia da matriz de dados da img
    for(i = 0; i < img->altura; i++)
        copia[i] = (unsigned int*) malloc(img->largura*sizeof(unsigned int));
    for(i = 0; i < img->altura; i++)
        for(j = 0; j < img->largura; j++)
            copia[i][j] = img->dados[i][j];


    /* remocao de ruido */

    for(i = 0; i < img->altura; i++)// se aproxima de branco, vira branco, senao vira preto
        for(j = 0; j<img->largura; j++)
        {
            if(copia[i][j] > BRANCO_LIM)
                copia[i][j] = BRANCO;
            else
                copia[i][j] = PRETO;
        }

    for(k=0; k < 2; k++)// se ao redor e branco, vira branco
        for(i=1;i<img->altura-1;i++)
            for(j=1;j<img->largura-1;j++)
                if((copia[i-1][j] == BRANCO && copia[i+1][j] == BRANCO) || (copia[i][j-1] == BRANCO && copia[i][j+1] == BRANCO))
                    copia[i][j] = BRANCO;

    for(k=0; k<2;k++)// se ao redor tem pouco branco, vira preto
        for(i=1;i<img->altura-1;i++)
            for(j=1;j<img->largura-1;j++)
                if((copia[i-1][j-1]+copia[i-1][j]+copia[i-1][j+1]+copia[i][j-1]+copia[i][j+1]+copia[i+1][j-1]+copia[i+1][j]+copia[i+1][j+1])<4*BRANCO)
                    copia[i][j] = PRETO;

    for(i=1;i<img->altura-1;i++)// se ao redor tem bastante branco, vira branco
        for(j=1;j<img->largura-1;j++)
            if(!((copia[i-1][j-1]+copia[i-1][j]+copia[i-1][j+1]+copia[i][j-1]+copia[i][j+1]+copia[i+1][j-1]+copia[i+1][j]+copia[i+1][j+1])<4*BRANCO))
                copia[i][j] = BRANCO;

    for(i=1;i<img->altura-1;i++)// atualiza dados
        for(j=1;j<img->largura-1;j++)
            img->dados[i][j] = copia[i][j];

    salvaImagem1C(img, "brancoepreto.bmp");


    /* rotulacao das figuras */

    for(i=0;i<img->altura;i++)// percorro a matriz ate achar um branco
    {
        for(j=0;j<img->largura;j++)
        {
            if(copia[i][j]==BRANCO)
            {
                copia[i][j]=cor;
                pos[0].pos_i=i;
                pos[0].pos_j=j;
                tem_branco=1;

                while(tem_branco)
                {
                    tem_branco=0;
                    contador=cont;
                    for(k=a;k<=contador;k++)// pra cada posicao no vetor de struct ele acessa e colore
                    {

                         if( (pos[k].pos_i-1>=0) && copia[pos[k].pos_i-1][pos[k].pos_j]==BRANCO)
                        {
                            copia[pos[k].pos_i-1][pos[k].pos_j]=cor;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i-1;
                            pos[cont].pos_j=pos[k].pos_j;
                            tem_branco=1;
                        }

                        if((pos[k].pos_i+1<img->altura&&pos[k].pos_j+1<img->largura)&&copia[pos[k].pos_i+1][pos[k].pos_j+1]==BRANCO)//diagonal direita superior
                        {
                            copia[pos[k].pos_i+1][pos[k].pos_j+1]=cor;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i+1;
                            pos[cont].pos_j=pos[k].pos_j+1;
                            tem_branco=1;
                        }
                        if((pos[k].pos_j-1>=0) && copia[pos[k].pos_i][pos[k].pos_j-1]==BRANCO)
                        {
                            copia[pos[k].pos_i][pos[k].pos_j-1]=cor;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i;
                            pos[cont].pos_j=pos[k].pos_j-1;
                            tem_branco=1;
                        }
                        if((pos[k].pos_i-1>=0&&pos[k].pos_j+1<img->largura)&&(copia[pos[k].pos_i-1][pos[k].pos_j+1]==BRANCO))// diagonal direita inferior
                        {
                            copia[pos[k].pos_i-1][pos[k].pos_j+1]=cor;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i-1;
                            pos[cont].pos_j=pos[k].pos_j+1;
                            tem_branco=1;
                        }
                       if(pos[k].pos_i+1<img->largura && copia[pos[k].pos_i+1][pos[k].pos_j]==BRANCO)
                        {
                            copia[pos[k].pos_i+1][pos[k].pos_j]=cor;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i+1;
                            pos[cont].pos_j=pos[k].pos_j;
                            tem_branco=1;
                        }
                        if((pos[k].pos_i-1>=0&&pos[k].pos_j>=0)&&copia[pos[k].pos_i-1][pos[k].pos_j-1]==BRANCO)
                        {
                            copia[pos[k].pos_i-1][pos[k].pos_j-1]=cor;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i-1;
                            pos[cont].pos_j=pos[k].pos_j-1;
                            tem_branco=1;
                        }

                        if(j+1<img->altura && copia[pos[k].pos_i][pos[k].pos_j+1]==BRANCO)
                        {
                            copia[pos[k].pos_i][pos[k].pos_j+1]=cor;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i;
                            pos[cont].pos_j=pos[k].pos_j+1;
                            tem_branco=1;
                        }
                        if((pos[k].pos_i+1<=img->altura&&pos[k].pos_j>=0)&&copia[pos[k].pos_i+1][pos[k].pos_j-1]==BRANCO)
                        {
                            copia[pos[k].pos_i-1][pos[k].pos_j-1]=cor;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i-1;
                            pos[cont].pos_j=pos[k].pos_j-1;
                            tem_branco=1;
                        }
                    }

                   a=k+1;

                }
                printf("\nCor = %u", cor);
                cor++;
                a=0;
                cont=0;
            }
        }
    }

    printf("\n");
    figuras = cor-1;
    larg = img->largura;
    alt = img->altura;

    img1 = criaImagem3C(larg, alt);
    for(m=0;m<alt;m++)
        for(n=0;n<larg;n++)
        {
            img1->dados[CANAL_R][m][n] = 0;
            img1->dados[CANAL_G][m][n] = 0;
            img1->dados[CANAL_B][m][n] = 0;
        }
    i = j = k = 30;
    for(cor = COR_INICIAL; cor <= figuras; cor++)
    {
        for(m=0;m<alt;m++)
        {
            for(n=0;n<larg;n++)
            {
                if(copia[m][n] == cor)
                {
                    img1->dados[CANAL_R][m][n] = i;
                    img1->dados[CANAL_G][m][n] = j;
                    img1->dados[CANAL_B][m][n] = k;
                }
            }
        }
        if(i < BRANCO - 6)
            i += 6;
        else
        {
            //i = 1;
            if(j < BRANCO - 6)
                j += 6;
            else
            {
                //j = 1;
                if(k < BRANCO - 6)
                    k += 6;
                else
                    i = 1;
            }
        }
    }
    salvaImagem3C(img1, "gradiente.bmp");


//    for(i=1;i<img->altura-1;i++)
//        for(j=1;j<img->largura-1;j++)
//            img->dados[i][j] = copia[i][j]-(BRANCO*(copia[i][j]/BRANCO));
//    salvaImagem1C(img, "tonsdecinza.bmp");
//    destroiImagem1C(img);

    img2 = criaImagem3C(larg, alt);
    for(m=0;m<alt;m++)
        for(n=0;n<larg;n++)
        {
            img2->dados[CANAL_R][m][n] = 0;
            img2->dados[CANAL_G][m][n] = 0;
            img2->dados[CANAL_B][m][n] = 0;
        }


    /* verificacao das dimensoes de cada figura */

    for(cor = COR_INICIAL; cor<=figuras; cor++)// mede a maior dimensao horizontal, vertical e diagonal de cada figura
    {
        horizontal = vertical = diagonal = diagonal2 = 1;

        for(i=0;i<alt;i++)
        {
            for(j=0;j<larg;j++)
            {
                if(copia[i][j]==cor && i<alt-1 && j<larg-1)
                {
                    for(k=1; copia[i][j+k]==cor; k++)
                    {
                        if(k>horizontal - 1)
                            horizontal++;
                        if(j+k+1 > larg-1)
                            break;
                    }
                    for(k=1; copia[i+k][j]==cor; k++)
                    {
                        if(k>vertical - 1)
                            vertical++;
                        if(i+k+1 > alt-1)
                            break;
                    }
                    for(k=1; copia[i+k][j+k]==cor; k++)
                    {
                        if(k>diagonal - 1)
                            diagonal++;
                        if(i+k+1 > alt-1 || j+k+1 > larg-1)
                            break;
                    }
                    if(j > 0)
                    {
                        for(k=1; copia[i+k][j-k]==cor; k++)
                        {
                            if(k>diagonal2 - 1)
                                diagonal2++;
                            if(i+k+1 > alt-1 || j-k-1 < 0)
                                break;
                        }
                    }
                }
            }
        }

        printf("\nf%u h%u v%u d%u di%u", cor, horizontal, vertical, diagonal, diagonal2);


        /* contagem das figuras por tipo */

//        if(max(horizontal, max(vertical, max(diagonal, diagonal2))) - min(horizontal, min(vertical, min(diagonal, diagonal2)) <= FORMATO_LIM))
        if((diagonal + diagonal2)/2.0 - (horizontal + vertical)/2.0 >= -FORMATO_LIM && (diagonal + diagonal2)/2.0 - (horizontal + vertical)/2.0 <= FORMATO_LIM)
        {
            if(horizontal >= DIAMETRO_LIM || vertical >= DIAMETRO_LIM || diagonal >= DIAMETRO_LIM || diagonal2 >= DIAMETRO_LIM)
            {
                for(m=0;m<alt;m++)
                    for(n=0;n<larg;n++)
                    {
                        if(copia[m][n] == cor)
                        {
                            img2->dados[CANAL_R][m][n] = R_CIRC;
                            img2->dados[CANAL_G][m][n] = G_CIRC;
                            img2->dados[CANAL_B][m][n] = B_CIRC;
                        }
                    }
                (*n_circulos)++;
                printf(" circulo");
            }
        }
        else
        {
            if((horizontal >= RETANG_LIM_COMP && vertical >= RETANG_LIM_LARG) || (vertical >= RETANG_LIM_COMP && horizontal >= RETANG_LIM_LARG))
            {
                for(m=0;m<alt;m++)
                    for(n=0;n<larg;n++)
                    {
                        if(copia[m][n] == cor)
                        {
                            img2->dados [CANAL_R][m][n] = R_RET;
                            img2->dados [CANAL_G][m][n] = G_RET;
                            img2->dados [CANAL_B][m][n] = B_RET;
                        }
                    }
                (*n_retangulos)++;
                printf(" retangulo");
            }
        }
    }

    printf("\n\n");
    salvaImagem3C(img2, "colorido.bmp");

//    system("colorido.bmp");

    destroiImagem3C(img2);
}
