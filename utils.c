#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

void LimparTerminal() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

Tabuleiro7x7 MontarTabuleiro(int coluna, int linhas){
    Tabuleiro7x7 tab;
    Ficha nula = {"nula","nulo"};

    for(int i = 0; i < coluna; i++){
        for(int j = 0; j < linhas; j++){
            tab.tabuleiro[i][j] = nula;
        }
    }

    return tab;
}

void CarregarJogo(Tabuleiro7x7 tabuleiro){
    for(int linha = 0; linha < 8; linha++){
        for(int col = 0; col < 7; col++){
            if(linha<7){
                Ficha ficha = tabuleiro.tabuleiro[linha][col];
                if (strcmp(ficha.dono, "jog1") == 0) {
                    printf(" X ");
                } else if (strcmp(ficha.dono, "jog2") == 0) {
                    printf(" O ");
                } else if (strcmp(ficha.dono, "nulo") == 0) {
                    printf(" . ");
                } else {
                    printf(" ? ");
                }
            }
            else{
                printf(" %d ", col + 1);
            }
        }
        printf("\n");
    }
}

Tabuleiro7x7 MontarJogo(){
    Tabuleiro7x7 Tabuleiro = MontarTabuleiro(7,7);
    CarregarJogo(Tabuleiro);

    return Tabuleiro;
}