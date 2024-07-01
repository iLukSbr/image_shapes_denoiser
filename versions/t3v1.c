#include "imagem.h"
#include "trabalho3.h"
#define PRETO 0
#define BRANCO_LIM 200
#define BRANCO 255

void contaIngredientes (Imagem1C* img, int* n_circulos, int* n_retangulos)
{
    int i, j, achou = 1, cor;
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



    for(cor = 180; achou; cor+=10)
    {
        achou = 0;
        for(i = 0; i<img->altura; i++)
        {
            for(j = 0; j<img->largura; j++)
            {
                if(img->dados[i][j] == BRANCO)
                {
                    if(!achou)
                        img->dados[i][j] = cor;
                    if(i > 0)
                        if(img->dados[i - 1][j] == cor)
                            img->dados[i][j] = cor;
                    if(j < img->largura - 1)
                        if(img->dados[i][j + 1] == cor)
                            img->dados[i][j] = cor;
                    if(j > 0)
                        if(img->dados[i][j - 1] == cor)
                            img->dados[i][j] = cor;
                    if(i < img->altura - 1)
                        if(img->dados[i + 1][j] == cor)
                            img->dados[i][j] = cor;
                    achou = 1;
                }
            }
        }
    }

    salvaImagem1C(img, "cor.bmp");
//            else
//                if(conta >= 5)
//                {
//                    conta /= 2;
//                    for(desce = i+1; ; desce++)
//                    {
//                        if(img->dados[desce][conta])
//                        {
//
//                        }
//                    }
//                }
//                conta = 0;
//        }
//        printf("\n=======================================================\n");
//    }
//    printf("\n%d %d\n", img->largura, img->altura);
}
