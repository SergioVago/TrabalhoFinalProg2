#ifndef TARMAZENA_H
#define TARMAZENA_H

#include <stdio.h>

typedef struct {
    int coisa[3]; //Armazena o maximo de geracoes, celualas vivas e celulas
        //mortas em forma de vetor
    /*
        0 -> Maximo de Geracoes
        1 -> Numero celulas vivas
        2 -> Numero celulas Mortas
     */
    
    float media; //Media de Celulas Vivas
    
    int soma; //Soma das celulas vivas para o calculo da media
    
    int maiorVivas; //Armazena a maior quantidade de celulas vivas
    
    int gerVivas; //Armazena a geracao da maior quantidade de celulas vivas
    
    int lista[1001][1]; //Lista das geracoes e suas respectivas quantidades de
        //celulas vivas
    
    int maiorMortas; //Armazena a maior quantidade de celulas mortas
    
    int gerMortas; //Armazena a geracao da maior quantidade de celulas mortas
    
    int contaStill[5][1]; //Armazena a quantidade de aparicoes de cada 
        //Strill Lifes 
    /*
        0 -> Beehive 
        1 -> Blocks  
        2 -> Boat   
        3 -> Loaf   
        4 -> Tub    
     */
    
    char ghost[2500][2500]; //Matriz para armazenar o Ghost
} tArmazena;


typedef struct{
    int ger[1001]; //Armazena a geracao
    int *celulas[1000]; //Com a respectiva celula viva
} tPLista;

//Inicia e Preenche o vetor da struct com 0
int IniciaArmazem(tArmazena *armazem);

//Passa os paramentros para dentro do vetor da struct
int ArmazenaCoisas(tArmazena *armazem,int max,int viva,int morta);

//Atribui o valor a alguma variavel do tipo inteiro
int AtribuiMaxGen(tArmazena *armazem);
int AtribuiVivas(tArmazena *armazem);
int AtribuiMortas(tArmazena *armazem);

//Compara pra saber a maior quantidade de celulas vivas na simulacao
void ComparaVivas(tArmazena *armazem, int i);

//Compara pra saber a maior quantidade de celulas mortas na simulacao
void ComparaMortas(tArmazena *armazem,int i);

//Armazena a quantidade de celulas vivas
void SomaCelulasVivas(tArmazena *armazem);

//Calcula a media total de celuas vivas
void CalculaMedia(tArmazena *armazem);

//Atribue a qnt de celulcas vivas de alguma geracao na lista
void GuardaLista(tArmazena *armazem,int ger);

//Gera os arquivos de estatisticacs
int GerarArquivoDeEstatisticas(tArmazena *armazem,FILE *pSaida,char** argv,tPLista *pLista, int *w,int *h);

//Inicia o still lifes
int IniciaStills(tArmazena *armazem);

//Imprime o still lifes
void ImprimeStill(FILE *pSaida,int vetorPosicoes,tArmazena *armazem);

#endif /* TARMAZENA_H */
