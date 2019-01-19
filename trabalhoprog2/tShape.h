#ifndef TSHAPE_H
#define TSHAPE_H

#include "tFTroca.h"
#include "tArmazena.h" 

typedef struct{
    char corpo[2500][2500];
    char vizinho[2500][2500];
} tShape;

//Le e preenche o shape com 0
void LeEPreencheShape(tShape *shape);

//Funcao inicial
int CriaJogo(tShape *shape,FILE *pArquivo, char *caminho,tFTroca *troca,tArmazena *armazem,int *w,int *h);

//Abre e le o arquivo de configurcao
int LeArq(tShape *shape,FILE *pArquivo, char *caminho, tFTroca *troca,tArmazena *armazem,int *w,int *h);

//Le o aquivo de configuracao
void LeConf(FILE *pArquivo, tShape *shape, tFTroca *troca,tArmazena *armazem,int *w,int *h);

//Imprime a situacao atual da simulacao
void ImprimeSimulacao(int ger,int vivas,int mortas,tShape *shape, int *w, int *h);

//Realiza a simulacao
void RealizaSimulacao(tShape *shape,tArmazena *armazem, tFTroca *troca,int *w,int *h);

//Muda a cellula de acordo com as 4 regras
void MudaCelula(tShape *shape,int *w,int *h);

//Verifica quantos vizinho vivos e quantos mortos
void VerificaVizinho(tShape *shape,int i,int j,int *contador);

//Soma o contador de vizinho vivos ou mortos
void SomaContador(int* direcao,int *contador);

//Escolhe uma posicao especifica para verificar
int VerificaPosicaoEspecifica(tShape *shape,int i,int j);

//Verifica se uma celula estava viva ou morta em determinada posicao 
int VerificaVivo(tShape *shape,int i,int j);

//Inicia o contador com 0
void IniciaContador(int *contador);

void TestaStills(tShape *shape,tArmazena *armazem,int *h,int *w);

void SalvaGhost(tShape *shape,tArmazena *armazem,int *w,int *h);

int VerificaVivosDentroDoTipo(tShape *shape,int iniI,int iniJ,int fimI,int fimJ,int posiI, int posiJ,int *h,int *w);

#endif /* TSHAPE_H */

