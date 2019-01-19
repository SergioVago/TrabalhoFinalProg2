#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tShape.h"
#include "ParametrosInicializacao.h"
#include "tMensagem.h"
#include "tFTroca.h"
#include "tArmazena.h" 

tShape shape;
tFTroca troca;
tArmazena armazem;
tPLista pLista;

int main(int argc, char** argv){
    char caminho[1001];
    int h,w;
    
    //Testa o diretorio passado, caso haja erro, finaliza o programa
    if(TestaArq(argc,argv)){  
        return 10;
    } 
    
    FILE *pArquivo;
    FILE *pSaida;
    
    //Le o aquivo e Cria o jogo, caso haja erro, finaliza o progroma
    if(CriaJogo(&shape,pArquivo,argv[1],&troca,&armazem,&w,&h)){
        RealizaSimulacao(&shape,&armazem,&troca,&w,&h);
    } else return 20;
    
    if(GerarArquivoDeEstatisticas(&armazem,pSaida,argv,&pLista,&w,&h))
        return 30;
    
    return (EXIT_SUCCESS);
}
    