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

const int linhas = 6;
const int colunas = 7;

Tabuleiro6x7 MontarTabuleiro(int linhas, int colunas){
    Tabuleiro6x7 tab;
    Ficha nula = {"nula","nulo"};

    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            tab.tabuleiro[i][j].ficha = nula;
            tab.tabuleiro[i][j].vazio = 1;
        }
    }

    return tab;
}

void CarregarJogo(Tabuleiro6x7 jogo){
    LimparTerminal();
    for(int li = 0; li < (linhas+1); li++){
        for(int col = 0; col < colunas; col++){
            if(li<linhas){
                Casa casa = jogo.tabuleiro[li][col];
                if(casa.vazio)
                    printf(" . ");
                else if (strcmp(casa.ficha.dono, "jog1") == 0) {
                    printf("\033[34m X \033[0m");
                } else if (strcmp(casa.ficha.dono, "jog2") == 0) {
                    printf("\033[31m O \033[0m");
                } else if (strcmp(casa.ficha.dono, "nulo") == 0) {
                    printf(" . ");
                } else {
                    printf(" ? ");
                }
            }
            else{
                if(jogo.tabuleiro[0][col].vazio == 0){
                    printf("\033[31m %d \033[0m", col + 1);
                } else {
                    printf(" %d ", col + 1);
                }
            }
        }
        printf("\n");
    }
}

void aplicarGravidadeColuna(Tabuleiro6x7 *jogo, int col){
    Ficha fichas[linhas];
    int count = 0;
    for(int row = 0; row < linhas; row++){
        if(!jogo->tabuleiro[row][col].vazio){
            fichas[count++] = jogo->tabuleiro[row][col].ficha;
        }
    }

    for(int row = 0; row < linhas; row++){
        limparCasa(jogo, row, col);
    }

    for(int i = 0; i < count; i++){
        preencherCasa(jogo, linhas - 1 - i, col, fichas[i]);
    }
}

void limparCasa(Tabuleiro6x7 *jogo, int row, int col){
    Ficha nula = {"nula","nulo"};
    jogo->tabuleiro[row][col].ficha = nula;
    jogo->tabuleiro[row][col].vazio = 1;
}

void preencherCasa(Tabuleiro6x7 *jogo, int row, int col, Ficha ficha){
    jogo->tabuleiro[row][col].ficha = ficha;
    jogo->tabuleiro[row][col].vazio = 0;
}

void posicionarFicha(Tabuleiro6x7 *jogo, int col, Ficha ficha){
    preencherCasa(jogo, 0, col, ficha);
    aplicarGravidadeColuna(jogo, col);
    CarregarJogo(*jogo);
}

void verificarJogada(Tabuleiro6x7 *jogo, Ficha ficha){
    int col;
    while(1){
        printf("Escolha uma coluna (1-7): ");
        scanf("%d", &col);
        col--;
        if(col < 0 || col >= colunas){
            printf("Coluna invalida. Tente novamente.\n");
            continue;
        }
        if(jogo->tabuleiro[0][col].vazio == 0){
            printf("Coluna cheia. Escolha outra coluna.\n");
            continue;
        }
        
        posicionarFicha(jogo, col, ficha);
        break;
    }
}

void jogada(Tabuleiro6x7 *jogo, Ficha ficha){
    int canPlay = 0;
    for(int col = 0; col < colunas; col++){
        if(jogo->tabuleiro[0][col].vazio){
            canPlay = 1;
            break;
        }
    }
    if(!canPlay){
        printf("Tabuleiro cheio! Jogo empatado.\n");
        return;
    }

    verificarJogada(jogo, ficha);
}

Tabuleiro6x7 MontarJogo(){
    Tabuleiro6x7 Tabuleiro = MontarTabuleiro(linhas,colunas);
    CarregarJogo(Tabuleiro);

    return Tabuleiro;
}