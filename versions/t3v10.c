/* Aluno: Diogo Cademartori Levandoski - Matricula: 2242044                   */
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
#include "imagem.h"//TESTE

#ifndef max
    #define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
    #define min(a,b) ((a) < (b) ? (a) : (b))
#endif

/* cor */
#define PRETO 0 // preto rgb
#define BRANCO 255 // branco rgb

/* filtro de ruido */
#define BRANCO_LIM 120 // intensidade minima do branco
#define PRETOS_REDOR 5 // quantos pixels pretos ao redor para considerar preto
#define BRANCOS_REDOR 5 // quantos pixels brancos ao redor para considerar branco
#define N_FILTRAR 3 // quantas vezes passar o filtro de ruido

/* rotulacao */
#define FIGURAS 9000 // estimativa da quantidade de figuras
#define ROTULO_INICIAL 256 // rotulo inicial dos ingredientes

/* classificacao da forma */
#define RETANG_LARG_MIN 10 // largura>=10 eh retangulo
#define RETANG_ALT_MIN 5 // altura>=5 eh retangulo
#define DIAMETRO_MIN 20 // raio>=10, diametro>=20 eh circulo
#define FORMA_LIM 5 // limite da diferenca entre os diametros pra considerar circulo
#define PERIMETRO_LIM 16 // limite da diferenca entre os perimetros calculado e da imagem
#define AREA_LIM 16 // limite da diferenca entre as areas calculada e da imagem
#define DIAGONAL_LIM 3 // limite da diferenca entre as diagonais calculada e da imagem
#define GRANDE_HORI 5 // quao maior a largura deve ser do que a altura para considerar retangulo imediatamente

/* retornos da funcao classifica forma */
#define CIRCULO 1
#define RETANGULO 2

#define CANAL_R 0 /* Constante usada para se referir ao canal vermelho. */
#define CANAL_G 1 /* Constante usada para se referir ao canal verde. *///TESTE
#define CANAL_B 2 /* Constante usada para se referir ao canal azul. */

/* retangulos azuis */
#define R_RET 0
#define G_RET 64//TESTE
#define B_RET 255

/* circulos amarelos */
#define R_CIRC 250
#define G_CIRC 253//TESTE
#define B_CIRC 15

typedef struct{// guarda as posições de cada ponto a ser colorido
    int pos_i;
    int pos_j;
}GuardaPos;

typedef struct{// guarda as dimensoes e rotulo do ingrediente
    unsigned int horizontal;
    unsigned int vertical;
    unsigned int diagonal;
    unsigned int diagonal_inv;
    unsigned int diagonal_bar;
    unsigned int area_pix;
    unsigned int perimetro_pix;
    unsigned int rotulo;
    unsigned int y_0[FIGURAS];
}Dim;

void removeRuido (Imagem1C* img);

unsigned int** copiaDados (Imagem1C* img);

unsigned int rotulaIngredientes (unsigned int **copia, Imagem1C* img, Dim fig);

int classificaForma(Dim fig);

void destroiCopia (unsigned int **copia, Imagem1C* img);

void contaIngredientes (Imagem1C* img, int* n_circulos, int* n_retangulos)
{
    unsigned int i, j, k, ingredientes, **copia, tem_rotulo, m, n, x=0, y=0;//TESTE
    int forma;
    Dim fig;
    Imagem3C* img2;//TESTE

    *n_circulos = *n_retangulos = 0;

    for(k = 0; k < N_FILTRAR; k++)
        removeRuido(img);

    copia = copiaDados(img);

    for(i=0; i<img->altura; i++)//TESTE
        for(j=0;j<img->largura;j++)
            img->dados[i][j]=copia[i][j];
    salvaImagem1C(img,"brancoepreto.bmp");//TESTE

    ingredientes = rotulaIngredientes(copia, img, fig);

    img2 = criaImagem3C(img->largura, img->altura);//TESTE
    for(m=0;m<img->altura;m++)
        for(n=0;n<img->largura;n++)
        {
            img2->dados[CANAL_R][m][n] = 0;
            img2->dados[CANAL_G][m][n] = 0;//TESTE
            img2->dados[CANAL_B][m][n] = 0;
        }//TESTE


    /* verificacao das dimensoes de cada figura */

    for(fig.rotulo = ROTULO_INICIAL; fig.rotulo <= ingredientes; fig.rotulo++)
    {
        fig.horizontal = fig.vertical = fig.diagonal_bar = fig.diagonal_inv = 1;
        fig.area_pix = 0;// area do ingrediente em pixels
        fig.perimetro_pix = 0;// perimetro do ingrediente em pixels
        for(i = fig.y_0[fig.rotulo-ROTULO_INICIAL]; i < img->altura; i++)
        {
            tem_rotulo = 0;
            for(j = 0; j < img->largura; j++)
            {
                if(copia[i][j] == fig.rotulo)
                {
                    tem_rotulo = 1;
                    fig.area_pix++;
                    if(i==0 || j==0 || i==img->altura-1 || j==img->largura-1)
                        fig.perimetro_pix++;
                    else if(copia[i-1][j]==PRETO || copia[i][j+1]==PRETO || copia[i+1][j]==PRETO || copia[i][j-1]==PRETO)
                        fig.perimetro_pix++;
                    if(i < img->altura-1 && j < img->largura-1)
                    {
                        for(k = 1; copia[i][j+k] == fig.rotulo; k++)// mede a maior dimensao horizontal
                        {
                            if(k > fig.horizontal-1)
                                fig.horizontal++;
                            if(j+k+1 > img->largura-1)
                                break;
                        }
                        for(k = 1; copia[i+k][j] == fig.rotulo; k++)// mede a maior dimensao vertical
                        {
                            if(k > fig.vertical-1)
                                fig.vertical++;
                            if(i+k+1 > img->altura-1)
                                break;
                        }
                        for(k = 1; copia[i+k][j+k] == fig.rotulo; k++)/* mede a maior dimensao diagonal \ barra invertida */
                        {
                            if(k > fig.diagonal_inv-1)
                                fig.diagonal_inv++;
                            if(i+k+1 > img->altura-1 || j+k+1 > img->largura-1)
                                break;
                        }
                        if(j > 0)
                        {
                            for(k = 1; copia[i+k][j-k] == fig.rotulo; k++)/* mede a maior dimensao diagonal / barra */
                            {
                                if(k > fig.diagonal_bar-1)
                                    fig.diagonal_bar++;
                                if(i+k+1 > img->altura-1 || j-k-1 < 0)
                                    break;
                            }
                        }
                    }
                }
            }
            if(fig.area_pix && !tem_rotulo)
                break;
        }
        forma = classificaForma(fig);
        if(forma == CIRCULO)
        {
            (*n_circulos)++;
            printf(" circulo%d", *n_circulos);//TESTE
            for(m=0;m<img->altura;m++)
                for(n=0;n<img->largura;n++)
                {
                    if(copia[m][n] == fig.rotulo)
                    {
                        img2->dados[CANAL_R][m][n] = R_CIRC;
                        img2->dados[CANAL_G][m][n] = G_CIRC+x;//TESTE
                        img2->dados[CANAL_B][m][n] = B_CIRC;
                    }
                }
            x+=1;//TESTE
        }
        else if(forma == RETANGULO)
        {
            (*n_retangulos)++;
            printf(" retangulo%d", *n_retangulos);//TESTE
            for(m=0;m<img->altura;m++)
                for(n=0;n<img->largura;n++)
                {
                    if(copia[m][n] == fig.rotulo)
                    {
                        img2->dados [CANAL_R][m][n] = R_RET;
                        img2->dados [CANAL_G][m][n] = G_RET;//TESTE
                        img2->dados [CANAL_B][m][n] = B_RET+y;
                    }
                }
            y+=1;//TESTE
        }
    }

    destroiCopia(copia, img);

    printf("\n\n");//TESTE
    salvaImagem3C(img2, "colorido.bmp");//TESTE
    destroiImagem3C(img2);//TESTE
}

/* cria (unsigned int) copia a partir de (unsigned char) img */
unsigned int** copiaDados (Imagem1C* img)
{
    unsigned int i, j, **copia;
    copia = (unsigned int**) malloc(img->altura*sizeof(unsigned int*));
    for(i = 0; i < img->altura; i++)
        copia[i] = (unsigned int*) malloc(img->largura*sizeof(unsigned int));
    for(i = 0; i < img->altura; i++)
        for(j = 0; j < img->largura; j++)
            copia[i][j] = img->dados[i][j];
    return copia;
}

/* filtro removedor de ruido */
void removeRuido (Imagem1C* img)
{
    unsigned int i, j, k;

    for(i = 0; i < img->altura; i++)// se aproxima de branco, vira branco, senao vira preto
    {
        for(j = 0; j < img->largura; j++)
        {
            if(img->dados[i][j] > BRANCO_LIM)
                img->dados[i][j] = BRANCO;
            else
                img->dados[i][j] = PRETO;
        }
    }
    for(i = 1; i < img->altura-1; i++)
    {
        for(j = 1; j < img->largura-1; j++)
        {
            if((img->dados[i-1][j] == BRANCO && img->dados[i+1][j] == BRANCO) ||
               (img->dados[i][j-1] == BRANCO && img->dados[i][j+1] == BRANCO))
                img->dados[i][j] = BRANCO;// se acima, abaixo, direita e esquerda sao brancos, vira branco
            k = img->dados[i-1][j-1] + img->dados[i-1][j] + img->dados[i-1][j+1] +
               img->dados[i][j-1] + img->dados[i][j+1] + img->dados[i+1][j-1] +
               img->dados[i+1][j] + img->dados[i+1][j+1];
            if(k <= (8-PRETOS_REDOR)*BRANCO)
                img->dados[i][j] = PRETO;// se ao redor tem poucos brancos, vira preto
            if(k >= BRANCOS_REDOR*BRANCO)
                img->dados[i][j] = BRANCO;// se ao redor tem muitos brancos, vira branco
        }
    }
}

/* rotula cada ingrediente */
unsigned int rotulaIngredientes (unsigned int **copia, Imagem1C* img, Dim fig)
{
    unsigned int i, j, k, rotulo=ROTULO_INICIAL, tem_branco, contador, cont=0, a=0;
    GuardaPos pos[FIGURAS];

    for(i=0; i < img->altura; i++)// percorro a matriz ate achar um branco
    {
        for(j=0; j < img->largura; j++)
        {
            if(copia[i][j] == BRANCO)
            {
                copia[i][j] = rotulo;
                pos[0].pos_i = fig.y_0[rotulo-ROTULO_INICIAL] = i;
                pos[0].pos_j = j;
                do{
                    tem_branco = 0;
                    contador = cont;
                    for(k=a; k <= contador; k++)// pra cada posicao no vetor de struct, acessa e rotula
                    {
                        if((pos[k].pos_i-1 >= 0) && copia[pos[k].pos_i-1][pos[k].pos_j] == BRANCO)
                        {// pixel superior
                            copia[pos[k].pos_i-1][pos[k].pos_j]=rotulo;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i-1;
                            pos[cont].pos_j=pos[k].pos_j;
                            tem_branco=1;
                        }
                        if((pos[k].pos_i+1 < img->altura && pos[k].pos_j+1 < img->largura) && copia[pos[k].pos_i+1][pos[k].pos_j+1] == BRANCO)
                        {// pixel diagonal direito inferior
                            copia[pos[k].pos_i+1][pos[k].pos_j+1]=rotulo;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i+1;
                            pos[cont].pos_j=pos[k].pos_j+1;
                            tem_branco=1;
                        }
                        if((pos[k].pos_j-1 >= 0) && copia[pos[k].pos_i][pos[k].pos_j-1] == BRANCO)
                        {// pixel esquerdo
                            copia[pos[k].pos_i][pos[k].pos_j-1]=rotulo;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i;
                            pos[cont].pos_j=pos[k].pos_j-1;
                            tem_branco=1;
                        }
                        if((pos[k].pos_i-1 >= 0 && pos[k].pos_j+1 < img->largura) && (copia[pos[k].pos_i-1][pos[k].pos_j+1] == BRANCO))
                        {// pixel diagonal direito superior
                            copia[pos[k].pos_i-1][pos[k].pos_j+1]=rotulo;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i-1;
                            pos[cont].pos_j=pos[k].pos_j+1;
                            tem_branco=1;
                        }
                        if(pos[k].pos_i+1 < img->altura && copia[pos[k].pos_i+1][pos[k].pos_j] == BRANCO)
                        {// pixel inferior
                            copia[pos[k].pos_i+1][pos[k].pos_j]=rotulo;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i+1;
                            pos[cont].pos_j=pos[k].pos_j;
                            tem_branco=1;
                        }
                        if((pos[k].pos_i-1 >= 0 && pos[k].pos_j >= 0) && copia[pos[k].pos_i-1][pos[k].pos_j-1] == BRANCO)
                        {// pixel diagonal esquerdo superior
                            copia[pos[k].pos_i-1][pos[k].pos_j-1]=rotulo;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i-1;
                            pos[cont].pos_j=pos[k].pos_j-1;
                            tem_branco=1;
                        }
                        if(j+1 < img->largura && copia[pos[k].pos_i][pos[k].pos_j+1] == BRANCO)
                        {// pixel direito
                            copia[pos[k].pos_i][pos[k].pos_j+1]=rotulo;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i;
                            pos[cont].pos_j=pos[k].pos_j+1;
                            tem_branco=1;
                        }
                        if((pos[k].pos_i+1 < img->altura && pos[k].pos_j>=0) && copia[pos[k].pos_i+1][pos[k].pos_j-1] == BRANCO)
                        {// pixel diagonal esquerdo inferior
                            copia[pos[k].pos_i-1][pos[k].pos_j-1]=rotulo;
                            cont++;
                            pos[cont].pos_i=pos[k].pos_i-1;
                            pos[cont].pos_j=pos[k].pos_j-1;
                            tem_branco=1;
                        }
                    }
                    a = k+1;
                }while(tem_branco);
                rotulo++;
                a = 0;
                cont = 0;
            }
        }
    }
    return (rotulo - 1);
}

/* classifica o ingrediente com base em suas dimensoes */
int classificaForma(Dim fig)
{
    double perimetro_circ, area_circ, perimetro_ret, area_ret, media_diagonal, media_hor_vert, media_todos, diagonal_ret;
    if(fig.horizontal < RETANG_LARG_MIN || fig.vertical < RETANG_ALT_MIN)
        return 0;
    media_diagonal = (fig.diagonal_bar + fig.diagonal_inv)/2.0;
    media_hor_vert = (fig.horizontal + fig.vertical)/2.0;
    media_todos = (fig.horizontal + fig.vertical + fig.diagonal_inv + fig.diagonal_bar)/4.0;
    perimetro_circ = M_PI*max(fig.horizontal, fig.vertical);
    area_circ = M_PI*pow((max(fig.horizontal, fig.vertical)/2), 2);
    perimetro_ret = 2*(fig.horizontal + fig.vertical);
    area_ret = fig.horizontal*fig.vertical;
    diagonal_ret = sqrt(pow(fig.horizontal, 2) + pow(fig.vertical, 2));
    printf("\nf%u h%u v%u d%u di%u pp%u ap%u media%lf aret%.3lf pret%.3lf acir%.3lf pcir%.3lf", fig.rotulo, fig.horizontal, fig.vertical, fig.diagonal_bar, fig.diagonal_inv, fig.perimetro_pix, fig.area_pix, media_todos, area_ret, perimetro_ret, area_circ, perimetro_circ);
//TESTE
    if((area_ret - fig.area_pix >= -AREA_LIM && area_ret - fig.area_pix <= AREA_LIM) ||
       (perimetro_ret - fig.perimetro_pix >= -PERIMETRO_LIM && perimetro_ret - fig.perimetro_pix <= PERIMETRO_LIM) ||
       (diagonal_ret - media_diagonal >= -DIAGONAL_LIM && diagonal_ret - media_diagonal <= DIAGONAL_LIM) ||
       (fig.horizontal >= fig.vertical + GRANDE_HORI))
        return RETANGULO;
    else if(((area_circ - fig.area_pix >= -AREA_LIM && area_circ - fig.area_pix <= AREA_LIM) ||
            (perimetro_circ - fig.perimetro_pix >= -PERIMETRO_LIM && perimetro_circ - fig.perimetro_pix <= PERIMETRO_LIM) ||
            (media_todos - fig.horizontal >= -FORMA_LIM && media_todos - fig.horizontal <= FORMA_LIM &&
            media_todos - fig.vertical >= -FORMA_LIM && media_todos - fig.vertical <= FORMA_LIM &&
            media_todos - fig.diagonal_inv >= -FORMA_LIM && media_todos - fig.diagonal_inv <= FORMA_LIM &&
            media_todos - fig.diagonal_bar >= -FORMA_LIM && media_todos - fig.diagonal_bar <= FORMA_LIM)) &&
            (fig.horizontal >= DIAMETRO_MIN || fig.vertical >= DIAMETRO_MIN ||
            fig.diagonal_inv >= DIAMETRO_MIN || fig.diagonal_bar >= DIAMETRO_MIN))
        return CIRCULO;
    else
        return RETANGULO;
}

/* elimina a matriz copia */
void destroiCopia (unsigned int **copia, Imagem1C* img)
{
    unsigned int i;
    for(i=0; i < img->altura; i++)
        free(copia[i]);
    free(copia);
}
