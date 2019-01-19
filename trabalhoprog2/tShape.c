#include <stdio.h>
#include "tShape.h"
#include "tMensagem.h"
#include "tFTroca.h"
#include "tArmazena.h"
#include <string.h>

int LeArq(tShape *shape,FILE *pArquivo, char *caminho, tFTroca *troca,tArmazena *armazem,int *w,int *h){   
    int i=0,x,y;
    char caminhoConf[1012];
    strcpy(caminhoConf,caminho);
    strcat(caminhoConf,"/config.txt");
    
    //Abre o diretorio
    pArquivo = fopen(caminhoConf,"r");
    
    //Testa a leitura do caminho
    if(!pArquivo){
        printf("A leitura do caminho %s falhou!\n",caminhoConf);
        return 1;
    } else {
        LeConf(pArquivo,shape,troca,armazem,w,h);
    }
    
    //Fecha o arquivo
    fclose(pArquivo);
    
    return 0;
}

//Le as configuracoes
void LeConf(FILE *pArquivo,tShape *shape,tFTroca *troca,tArmazena *armazem, int *w, int *h){
    int i,j,n,x,y,g;
 
    //Le os parametros iniciais
    fscanf(pArquivo,"shape=%d,%d\nmax_gen=%d\nstart: %d",w,h,&g,&n);
    
    
    //Armazena de acordo com o descrito em tArmazem.h
    ArmazenaCoisas(armazem,g,n,((*w)*(*h) - n));
    
    //Preenche todo o shape com '+'
    for(i=0;i<2500;i++){
        for(j=0;j<2500;j++){
            shape->corpo[i][j] = '+';
            armazem->ghost[i][j] = '+';
        }
    }
    
    //Preenche todo o shape que sera utilizado com '-'
    for(i=0;i<*h;i++){
        for(j=0;j<*w;j++){
            shape->corpo[i][j] = '-';
            armazem->ghost[i][j] = '-';
        }
    }
    
    //Preenche todo o vizinho com '+'
    for(i=0;i<*h;i++){
        for(j=0;j<*w;j++){
            shape->vizinho[i][j] = '+';
        }
    }
    
    //Preenche todo o troca com -1
    for(i=0;i<1001;i++){
        for(j=0;j<200;j++){
            troca->tam[i][j] = -1;
        }
    }
    
    
    //Le e preenche com as primeiras celulas vivas
    for(i=0;i < n; i++){
        fscanf(pArquivo," %d,%d",&x,&y);
        shape->corpo[y][x] = 'O';
    }
    
    fscanf(pArquivo,"\n");
    //Le ate o final do arquivo as futuras geracoes a serem modificadas
    while(!feof(pArquivo)){
        fscanf(pArquivo,"gen %d: %d",&g,&n);
        
        for(i=0;i < (n*2);i+=2){
            fscanf(pArquivo," %d,%d",&x,&y);
            //Armazena as futuras geracoes a serem trocadas
            troca->tam[g][i] = x;
            troca->tam[g][i+1] = y;
        }
        fscanf(pArquivo,"\n");
    }   
}

//Le o aquivo e Cria o jogo, caso haja erro, imprime a msg de erro e retorna 0
int CriaJogo(tShape *shape,FILE *pArquivo, char *caminho,tFTroca *troca,tArmazena *armazem,int *w, int *h){
    //Retorna verdadeiro caso de certo
    if(!LeArq(shape,pArquivo,caminho,troca,armazem,w,h)){
        return 1;
    } 
    //Retorna falso caso de errado
    else {
        MsgErro("Falha na criacao!");
        return 0;
    }
}

void RealizaSimulacao(tShape *shape,tArmazena *armazem, tFTroca *troca,int *w,int *h){
    int maxGen,vivas,mortas,i,x,y,posi1,posi2,gerac,j,k;
        
    //Atribui valores por funcao
    maxGen = AtribuiMaxGen(armazem);
    vivas = AtribuiVivas(armazem);
    mortas = AtribuiMortas(armazem);
    
    for(i=0;i<maxGen;i++){
        if(i!=0){
            posi1=0;
            posi2=1;
           
            //Realiza as trocas descritas pelo config.txt antes de realizar a
                //simulacao
            while(troca->tam[i][posi1] != -1 && (posi2 < 2500 && posi1 < 25000)){
                x = troca->tam[i][posi1];
                y = troca->tam[i][posi2];
                if(shape->corpo[y][x] == '-')
                    shape->corpo[y][x] = 'O';
                else shape->corpo[y][x] = '-';
                posi1+=2;
                posi2=posi1+1;
            }
            MudaCelula(shape,w,h);
        
            //Contas as celulas vivas e salva no armazem
            armazem->coisa[1] = 0;
            for(k=0;k<*h;k++){
                for(j=0;j<*w;j++){
                    armazem->coisa[1] += (VerificaVivo(shape,k,j));
                }
            }
 
            //Salva a quantidade de vivas em uma variavel
            vivas = AtribuiVivas(armazem);
            
            //Contas as celulas mortas e salva no armazem
            armazem->coisa[2] = ((*h)*(*w)) - armazem->coisa[1];
            
            //Salva a quantidade de vivas em uma variavel
            mortas = AtribuiMortas(armazem);
        } 
        
        //Chamada das funcoes necessarias 
        TestaStills(shape,armazem,h,w);
        ComparaVivas(armazem,i);
        ComparaMortas(armazem,i);
        SomaCelulasVivas(armazem);
        GuardaLista(armazem, i);
        SalvaGhost(shape,armazem,w,h);
        ImprimeSimulacao(i,vivas,mortas,shape,w,h);
    }
    
    CalculaMedia(armazem);
}

void ImprimeSimulacao(int ger,int vivas,int mortas,tShape *shape, int *w, int *h){
    int i,j;
    printf("Geracao: %d\n"
            "N. celulas vivas: %d\n"
            "N. celulas mortas: %d\n"
            ,ger,vivas,mortas);
    for(i=0;i<*h;i++){
        for(j=0;j<*w;j++){
            printf("%c",shape->corpo[i][j]);
        }
        printf("\n");
    }
}

void MudaCelula(tShape *shape,int *w,int *h){
    int i,j;
    int contador[2];
    //Posicao 0 -> Celulas Mortas
    //Posicao 1 -> Celulas Vivas
    
    //Aplica as regras de viver ou matar alguma celula
    for(i=0;i<*h;i++){
        for(j=0;j<*w;j++){
            VerificaVizinho(shape,i,j,contador);
            if(VerificaPosicaoEspecifica(shape,i,j)){
                if(contador[1] < 2 || contador[1] > 3){
                    shape->vizinho[i][j] = '-';
                }
            } else {
                if(contador[1] == 3){
                    shape->vizinho[i][j] = 'O';
                }
            }           
        }
    }
    
    //Copia mudadas pro shape
    for(i=0;i<*h;i++){
        for(j=0;j<*w;j++){
            if(shape->vizinho[i][j] == '-'){
                shape->corpo[i][j] = '-';
            } else if(shape->vizinho[i][j] == 'O'){
                shape->corpo[i][j] = 'O';
            }
        }
    }
}

void VerificaVizinho(tShape *shape,int i,int j,int *contador){
    int k,d,contadora=0; 
    int contadorViva = 0;
    int contadorMorta = 0;
    int VetorPosicoes[8];
    
    for(k=-1;k<2;k++){
        for(d=-1;d<2;d++){
            if(d!=0 || k!=0){
                VetorPosicoes[contadora] = VerificaPosicaoEspecifica(shape,i+k,j+d);
                contadora++;
            } 
        }
    }
    /* 
      Vizinhos - VetorPosicoes[<valor>]:
        0 -> Diagonal Esquerdo Superior
        1 -> Superior
        2 -> Diagonal Direitro Superior
        3 -> Esquerdo
        4 -> Direito
        5 -> Diagonal Esquerdo Inferior 
        6 -> Esquerdo
        7 -> Diagonal Direita Inferior
     */
    
    IniciaContador(contador);
    SomaContador(VetorPosicoes, contador);
}

void SomaContador(int* direcao,int *contador){
    int i;

    for(i=0;i<8;i++){
        //Se for dentro da matriz
        if(direcao[i] != 2){
            //Se for viva, soma no vivos, senao nos mortos
            if(direcao[i]) contador[1]+=1;
            else contador[0]+=1;
        }
    }
}

int VerificaPosicaoEspecifica(tShape *shape,int i,int j){
    //Testa se ta dentro da matriz
    if((i < 0 || j < 0) || (shape->corpo[i][j] == '+') || (i > 2500 || j > 2500)) return 2;
    else if(VerificaVivo(shape,i,j)) return 1;
    return 0;
}

int VerificaVivo(tShape *shape,int i,int j){
    if(shape->corpo[i][j] == '-') return 0;
    return 1;
}

void IniciaContador(int *contador){
    contador[0] = 0;
    contador[1] = 0;
}

void TestaStills(tShape *shape,tArmazena *armazem,int *h,int *w){
    int i,j,contagem,y,z,sumY,sumZ;
 
    //Faz o teste de acordo com cada tipo
    for(i=0;i<*h;i++){
        for(j=0;j<*w;j++){
            if(shape->corpo[i][j] == 'O'){
       
                /*
                 Esses teste seguem o seguinte padrao:
                 *  1 - Testa se celulas necessarias para definiar a forma
                 * estao dentro do shape
                 *  2 - Testa se celulas necessarias para definiar a forma
                 * estao vivas
                 *  3 - Conta a quantidade de celulas vivas no retangulo da forma
                 * Ex:
                 * Um block:
                 * 
                 * Roda a matriz daqui -> ----
                 *                        -OO-
                 *                        -OO-
                 *                        ---- <- ate aqui
                 * Se a contagem de vivas for 4, temos um block 
                 */
                
                //Block
                if((j+1 < *w) && 
                        (i+1 < *h) &&
                        (i+1 < *h && j+1 < *w)){    
                    if((shape->corpo[i][j+1] == 'O') &&
                            (shape->corpo[i+1][j] == 'O') &&
                            (shape->corpo[i+1][j+1] == 'O')){
                        contagem = VerificaVivosDentroDoTipo(shape,-1,-1,3,3,i,j,h,w);
                        if(contagem==4){
                            armazem->contaStill[1][0] += 1;
                        }       
                    }       
                }  
                
                
                //Beehive e Loaf
                if((j+1 < *w) &&
                        (j-1 > -1 && i+1 < *h) &&
                        (i+2 < *h) &&
                        (i+1 < *h && j+2 < *w)){
                    if(i+2 < *h && j+1 < *w){
                    //Beehive
                        if((shape->corpo[i][j+1] == 'O') &&
                                (shape->corpo[i+2][j] == 'O') &&
                                (shape->corpo[i+2][j+1] == 'O') &&
                                (shape->corpo[i+1][j+2] == 'O') &&
                                (shape->corpo[i+1][j-1] == 'O')){
                            contagem = VerificaVivosDentroDoTipo(shape,-1,-2,4,4,i,j,h,w);
                            if(contagem==6){
                                armazem->contaStill[0][0] += 1;
                            }
                        }
                    } 
                    //Loaf
                    if((i+2 < *h && j+2 < *w) &&
                            (i+3 < *h && j+1 < *w)) {
                        if((shape->corpo[i][j+1] == 'O') &&
                                (shape->corpo[i+2][j] == 'O') &&
                                (shape->corpo[i+2][j+2] == 'O') &&
                                (shape->corpo[i+1][j+2] == 'O') &&
                                (shape->corpo[i+1][j-1] == 'O') &&
                                (shape->corpo[i+3][j+1] == 'O')){                          
                            contagem = VerificaVivosDentroDoTipo(shape,-1,-2,5,4,i,j,h,w);
                            if(contagem==7){
                                armazem->contaStill[3][0] += 1;
                            }
                        }
                    }
                }  
                
                //Boat
                if((j+1 < *w) &&
                        (j+2 < *w && i+1 < *h) &&
                        (i+1 < *h) &&
                        (i+2 < *h && j+1)){
                    if((shape->corpo[i][j+1] == 'O') &&
                            (shape->corpo[i+1][j] == 'O') &&    
                            (shape->corpo[i+2][j+1] == 'O') &&
                            (shape->corpo[i+1][j+2] == 'O')){
                        contagem = VerificaVivosDentroDoTipo(shape,-1,-1,4,4,i,j,h,w);
                        if(contagem==5){
                            armazem->contaStill[2][0] += 1;
                        }
                    }
                }

                //Tub
                if((j+1 < *w && i+1 < *h) &&
                        (j-1 > -1 && i+1 < *h) &&
                        (i+2 < *h)){
                    if((shape->corpo[i+1][j+1] == 'O') &&
                            (shape->corpo[i+2][j] == 'O') &&
                            (shape->corpo[i+1][j-1] == 'O')){
                        contagem = VerificaVivosDentroDoTipo(shape,-1,-2,4,3,i,j,h,w);
                        if(contagem==4){
                            armazem->contaStill[4][0] += 1;                            
                        }
                    }
                }
            }
        }
    }
}

void SalvaGhost(tShape *shape,tArmazena *armazem,int *w,int *h){
    int i,j;
    
    //Roda o shape verificando as celulas vivas e armazena elas no ghost
    for(i=0;i<*h;i++){
        for(j=0;j<*w;j++){
            if(shape->corpo[i][j] == 'O'){
                armazem->ghost[i][j] = 'O';
            }
        }
    }
}

int VerificaVivosDentroDoTipo(tShape *shape,int iniI,int iniJ,int fimI,int fimJ,int posiI, int posiJ,int *h,int *w){
    int contador,i,j,sumI,sumJ;
    contador = 0;
    for(i=iniI;i<fimI;i++){
        for(j=iniJ;j<fimJ;j++){
            sumI = posiI + i;
            sumJ = posiJ + j;
            //Verifica os vivos dentro dentro do retangulo pre-definido pelos
                //parametros passados
            //Caso tenha alguma celulas fora, ja descartamos a possiblidade
                //de termos uma forma
            if((sumI < 0 || sumI >= *h) || (sumJ < 0 || sumJ >= *w)){
                return 0;
            } else if(VerificaVivo(shape,sumI,sumJ)){
                contador += 1;
            }
        }
    }
    return contador;
}