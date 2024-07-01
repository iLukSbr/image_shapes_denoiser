#include "imagem.h"
#include<stdlib.h>
#include "trabalho3.h"
#define PRETO 0
#define BRANCO_LIM 200
#define BRANCO 255

typedef struct//struct pra guardar as posições de cada ponto a ser colorido
{
    int pos_i;
    int pos_j;

}GuardaPos;

void contaIngredientes (Imagem1C* img, int* n_circulos, int* n_retangulos)
{
    int i, j,k, cor=100,tem_branco=1,cont=0,a=0,contador;
    GuardaPos pos[5000];
    for(i = 0; i<img->altura; i++)
    {
        for(j = 0; j<img->largura; j++)
        {
            //printf("%d ", img->dados[i][j]);
            if(img->dados[i][j] > BRANCO_LIM)
            {
              //  conta++;
                img->dados[i][j] = BRANCO;
            }
            else
                img->dados[i][j] = PRETO;
        }
    }
    salvaImagem1C(img, "att.bmp");

    for(i=0;i<img->altura;i++)//percorro a matriz até achar um branco
    {
        for(j=0;j<img->largura;j++)
        {
                    if(img->dados[i][j]==BRANCO)
                    {
                        img->dados[i][j]=cor;
                        pos[0].pos_i=i;
                        pos[0].pos_j=j;
                        tem_branco=1;

                        while(tem_branco)
                        {
                            tem_branco=0;
                            contador=cont;
                            for(k=a;k<=contador;k++)//pra cada posição no vetor de struct ele acessa e colore
                            {

                                 if( (pos[k].pos_i-1>=0) && img->dados[pos[k].pos_i-1][pos[k].pos_j]==BRANCO)
                                {
                                    img->dados[pos[k].pos_i-1][pos[k].pos_j]=cor;
                                    cont++;
                                    pos[cont].pos_i=pos[k].pos_i-1;
                                    pos[cont].pos_j=pos[k].pos_j;
                                    tem_branco=1;
                                }

                                if((pos[k].pos_i+1<img->altura&&pos[k].pos_j+1<img->largura)&&img->dados[pos[k].pos_i+1][pos[k].pos_j+1]==BRANCO)//diagonal direita superior
                                {
                                    img->dados[pos[k].pos_i+1][pos[k].pos_j+1]=cor;
                                    cont++;
                                    pos[cont].pos_i=pos[k].pos_i+1;
                                    pos[cont].pos_j=pos[k].pos_j+1;
                                    tem_branco=1;
                                }
                                if((pos[k].pos_j-1>=0) && img->dados[pos[k].pos_i][pos[k].pos_j-1]==BRANCO)
                                {
                                    img->dados[pos[k].pos_i][pos[k].pos_j-1]=cor;
                                    cont++;
                                    pos[cont].pos_i=pos[k].pos_i;
                                    pos[cont].pos_j=pos[k].pos_j-1;
                                    tem_branco=1;
                                }
                                if((pos[k].pos_i-1>=0&&pos[k].pos_j+1<img->largura)&&(img->dados[pos[k].pos_i-1][pos[k].pos_j+1]==BRANCO))//diagonal direita inferior
                                {
                                    img->dados[pos[k].pos_i-1][pos[k].pos_j+1]=cor;
                                    cont++;
                                    pos[cont].pos_i=pos[k].pos_i-1;
                                    pos[cont].pos_j=pos[k].pos_j+1;
                                    tem_branco=1;
                                }
                               if(pos[k].pos_i+1<img->largura && img->dados[pos[k].pos_i+1][pos[k].pos_j]==BRANCO)
                                {
                                    img->dados[pos[k].pos_i+1][pos[k].pos_j]=cor;
                                    cont++;
                                    pos[cont].pos_i=pos[k].pos_i+1;
                                    pos[cont].pos_j=pos[k].pos_j;
                                    tem_branco=1;
                                }
                                if((pos[k].pos_i-1>=0&&pos[k].pos_j>=0)&&img->dados[pos[k].pos_i-1][pos[k].pos_j-1]==BRANCO)
                                {
                                    img->dados[pos[k].pos_i-1][pos[k].pos_j-1]=cor;
                                    cont++;
                                    pos[cont].pos_i=pos[k].pos_i-1;
                                    pos[cont].pos_j=pos[k].pos_j-1;
                                    tem_branco=1;
                                }

                                if(j+1<img->altura && img->dados[pos[k].pos_i][pos[k].pos_j+1]==BRANCO)
                                {
                                    img->dados[pos[k].pos_i][pos[k].pos_j+1]=cor;
                                    cont++;
                                    pos[cont].pos_i=pos[k].pos_i;
                                    pos[cont].pos_j=pos[k].pos_j+1;
                                    tem_branco=1;
                                }
                                if((pos[k].pos_i+1<=img->altura&&pos[k].pos_j>=0)&&img->dados[pos[k].pos_i+1][pos[k].pos_j-1]==BRANCO)
                                {
                                    img->dados[pos[k].pos_i-1][pos[k].pos_j-1]=cor;
                                    cont++;
                                    pos[cont].pos_i=pos[k].pos_i-1;
                                    pos[cont].pos_j=pos[k].pos_j-1;
                                    tem_branco=1;
                                }
                            }

                           a=k+1;

                        }
                        cor++;
                        a=0;
                        cont=0;
                    }
            }
        }


    salvaImagem1C(img, "colorindo.bmp");

}


