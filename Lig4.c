#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

//gcc Lig4.c utils.c -o Lig4.exe
//./Lig4.exe

int main(){

    int escolha = 0;
    Tabuleiro6x7 jogo;

    do{
        
        printf("=======================================\n");
        printf("Lig4\n");
        printf("Selecione:\n");
        printf("1 - jogar\n");
        printf("2 - hall da fama\n");
        printf("3 - sair\n\n");


        printf("Digite sua escolha: ");

        scanf("%d", &escolha);
        LimparTerminal();

        switch(escolha){
        case 1:
            PrincipalJxJ();
            break;
        case 2:
            HallDaFama();
            break;
        default:
            break;
        }
    }while(escolha != 3);
    

    return 0;
}
