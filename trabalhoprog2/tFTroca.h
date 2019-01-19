#ifndef TFTROCA_H
#define TFTROCA_H

typedef struct{
    //Vetor que armazena as futuras trocas lidas no arquivo config.txt
    int tam[1001][200];
    /*
     Modo de armazemagem:
  
     tam[geracao][(0,1),(2,3)...(x,y)
         
     */
} tFTroca;

#endif /* TFTROCA_H */
