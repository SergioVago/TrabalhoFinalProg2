#include "ParametrosInicializacao.h"
#include "tMensagem.h"
#include <stdio.h>
#include <string.h>

//Testa os parametros na inicializacao
int TestaArq(int argc,char** argv){
    
    //Imprime o erro caso nao seja passado o diretorio na linha de comando
    if(argc < 2){
        MsgErro("O diretorio de arquivos "
                "de configuracao nao foi informado"
                ". O formato correto e: "
                "./nomedoprogoma /caminhodoarquivo/teste/");
        return 1;
    }
    
    //Imprime o erro caso o caminho seja maior do que 1000 caracteres
    if(strlen(argv[1]) > 1000) {
        MsgErro("Caminho muito extenso! O tamanho"
                " maximo e de 1000 caracteres.");
        return 2;
    }
    
    return 0;
}