#include "tArmazena.h"
#include <string.h>
#include <stdio.h>
#include "tMensagem.h"

int ArmazenaCoisas(tArmazena *armazem,int max,int viva,int morta){
    
    //Testa se o armazem foi iniciado
    if(!IniciaArmazem(armazem)){
        return 0;
    }
    
    //Testa se o stills foi iniciado
    if(!IniciaStills(armazem)){
        return 0;
    }

    
    //Armazena de acordo com o descrito em tArmazem.h
    armazem->coisa[0] = max;
    armazem->coisa[1] = viva;
    armazem->coisa[2] = morta;
    
    return 1;
}

int IniciaArmazem(tArmazena *armazem){
    int i;
    
    //Preenche todos com o valor inicial de -1
    for(i=0;i<1001;i++)
        armazem->lista[i][0] = -1;
    armazem->media = -1;
    armazem->maiorVivas = -1;
    armazem->gerVivas = -1;
    armazem->maiorMortas = -1;
    armazem->gerMortas = -1;
 
    //Inicia o soma   
    armazem->soma = 0;
    
    //Incia o armazem, preenchendo com 0
    for(i=0;i < 3; i++){
        armazem->coisa[i] = 0;
        if(armazem->coisa[i] != 0) return 0;
    }
    
    return 1;
}

//Atribuem os respectivos valores no vetor coisa 
int AtribuiMaxGen(tArmazena *armazem){
    return armazem->coisa[0]; 
}

int AtribuiVivas(tArmazena *armazem){
    return armazem->coisa[1]; 
}

int AtribuiMortas(tArmazena *armazem){
    return armazem->coisa[2]; 
}

//Compara vivas para saber qual geracao possui o maior numero de cel. vivas
void ComparaVivas(tArmazena *armazem,int i){
    if(armazem->coisa[1] > armazem->maiorVivas){
        armazem->maiorVivas = armazem->coisa[1]; 
        armazem->gerVivas = i;
    }        
}

//Compara mortas para saber qual geracao possui o maior numero de cel. mortas
void ComparaMortas(tArmazena *armazem,int i){
    if(armazem->coisa[2] > armazem->maiorMortas){
        armazem->maiorMortas = armazem->coisa[2];
        armazem->gerMortas = i;
    }
}

void SomaCelulasVivas(tArmazena *armazem){
    armazem->soma+= armazem->coisa[1];
}

void CalculaMedia(tArmazena *armazem){
    armazem->media = (float)(armazem->soma) / (armazem->coisa[0]);
}

//Atribue a qnt de celulcas vivas de alguma geracao na lista
void GuardaLista(tArmazena *armazem,int ger){
    armazem->lista[ger][0] = armazem->coisa[1];
}

int GerarArquivoDeEstatisticas(tArmazena *armazem,FILE *pSaida,char** argv,tPLista *pLista, int *w,int *h){
    //-----------------
    //Escreve o arquivo Stats.txt
    char caminhoStats[1018];
    
    strcpy(caminhoStats,argv[1]);
    strcat(caminhoStats,"/saida/stats.txt");
    
    pSaida = fopen(caminhoStats,"w");
    
    if(!pSaida){
        printf("Falha ao gerar o arquivo %s",caminhoStats);
        return 1;
    }
    
    fprintf(pSaida,"A media de celulas vivas ao longo das geracoes foi %.2f.\n"
            "A maior quantidade de celulas vivas foi de %d na geracao %d.\n"
            "A maior quantidade de celulas mortas foi de %d na geracao %d.\n"
            "Lista de geracoes ordenada pela quantidade de celulas vivas:\n"
            ,armazem->media,armazem->maiorVivas,armazem->gerVivas
            ,armazem->maiorMortas,armazem->gerMortas);
    
    int i,j;
    
    //Faz o vetor de ponteiros celulas apontarem para a lista junto com a
        //respectiva geracao
    for(i=0;i<armazem->coisa[0];i++){
        pLista->celulas[i] = &(armazem->lista[i][0]);
        pLista->ger[i] = i;
    }
    
    //organiza a struct pLista em ordem decrescente
    int *p,m;
    for(i=0;i < armazem->coisa[0];i++){
        for(j=i;j < armazem->coisa[0];j++){
            if(*(pLista->celulas[i]) <= *(pLista->celulas[j])){
                if(*(pLista->celulas[i]) == *(pLista->celulas[j])){
                    if(pLista->ger[i] > pLista->ger[j]){
                        m = pLista->ger[i];
                        pLista->ger[i] = pLista->ger[j];
                        pLista->ger[j] = m;
                    }
                } else {
                    p = pLista->celulas[i];
                    pLista->celulas[i] = pLista->celulas[j];
                    pLista->celulas[j] = p;

                    m = pLista->ger[i];
                    pLista->ger[i] = pLista->ger[j];
                    pLista->ger[j] = m;
                }
            }
        }
    }
    
    //Imprime a struct pLista no arquivo
    for(i=0;i<armazem->coisa[0];i++){
        fprintf(pSaida," - Geracao %d: %d celulas\n",pLista->ger[i],*(pLista->celulas[i]));
    }
    
    fclose(pSaida);
    //-------------------
    
    //--------------------
    //Escreve o arquivo still_lifes.txt
    char caminhoStill[1023];
    
    strcpy(caminhoStill,argv[1]);
    strcat(caminhoStill,"/saida/still_lifes.txt");
    
    pSaida = fopen(caminhoStill,"w");
    
    if(!pSaida){
        printf("Falha ao gerar o arquivo %s",caminhoStill);
        return 1;
    }
    
  
    //Organiza em ordem decrescente  
    int vetorPosicoes[5] = {0,1,2,3,4};
    int vetorValores[5];
    for(i=0;i<5;i++){
        vetorValores[i] = armazem->contaStill[i][0];
    }
    
    int aux1,aux2;
    
    for(i=0;i<5;i++){
        for(j=i;j<5;j++){
            if(vetorValores[i] <  vetorValores[j]){
                aux1 = vetorValores[i];
                vetorValores[i] = vetorValores[j];
                vetorValores[j] = aux1;
                
                aux2 = vetorPosicoes[i];
                vetorPosicoes[i] = vetorPosicoes[j];
                vetorPosicoes[j] = aux2;
            }   
        }
    }
    
    //Organiza em ordem alfabetica a partir do indice de cada um
    for(i=0;i<5;i++){
        for(j=i;j<5;j++){
            if(vetorValores[i] ==  vetorValores[j]){
                if(vetorPosicoes[i]>vetorPosicoes[j]){
                    aux2 = vetorPosicoes[i];
                    vetorPosicoes[i] = vetorPosicoes[j];
                    vetorPosicoes[j] = aux2;    
                }
            }   
        }
    }    
    
    //Escreve os tipos e sua quantidade no arquivo
    for(i=0;i<5;i++){
        ImprimeStill(pSaida,vetorPosicoes[i],armazem);
    }
  
    fclose(pSaida);
    //------------------
    
    //------------------
    //Escreve o arquive ghost.txt
    char caminhoGhost[1017];
    
    strcpy(caminhoGhost,argv[1]);
    strcat(caminhoGhost,"/saida/ghost.txt");
    
    pSaida = fopen(caminhoGhost,"w");
    
    if(!pSaida){
        printf("Falha ao gerar o arquivo %s",caminhoGhost);
        return 1;
    }
    
    //Roda o vetor ghost para escrever no arquivo
    for(i=0;i<*h;i++){
        for(j=0;j<*w;j++){
            fprintf(pSaida,"%c",armazem->ghost[i][j]);
        }
        fprintf(pSaida,"\n");
    }
    //----------------------
    
    return 0;
}

int IniciaStills(tArmazena *armazem){
    int i;
    
    //Preenche o contaStill
    for(i=0;i<5;i++){
        armazem->contaStill[i][0] = 0;
    }
    
    for(i=0;i<5;i++){
        if(armazem->contaStill[i][0] != 0) return 0;
    }
    
    return 1;
}

void ImprimeStill(FILE *pSaida,int vetorPosicoes,tArmazena *armazem){
    if(vetorPosicoes == 1){
        fprintf(pSaida," - Block: %d\n",armazem->contaStill[1][0]);
    } else if(vetorPosicoes == 0){
        fprintf(pSaida," - Beehive: %d\n",armazem->contaStill[0][0]);
    } else if(vetorPosicoes == 3){
        fprintf(pSaida," - Loaf: %d\n",armazem->contaStill[3][0]);
    } else if(vetorPosicoes == 2){
        fprintf(pSaida," - Boat: %d\n",armazem->contaStill[2][0]);
    } else {
        fprintf(pSaida," - Tub: %d\n",armazem->contaStill[4][0]);
    }
}