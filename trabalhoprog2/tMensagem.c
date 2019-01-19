#include "tMensagem.h"
#include <stdio.h>

//Imprime alguma mesagem com o \n
void Msg(char * msg){
    printf("%s\n", msg);
}

//Imprime alguma mesagem com o \n e ERRO: na ferente
void MsgErro(char * msg){
    printf("ERRO: %s\n", msg);
}
