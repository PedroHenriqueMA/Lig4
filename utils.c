#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include "utils.h"

void LimparTerminal() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void esperar(int segundos){
    #ifdef _WIN32
        Sleep(segundos * 1000);
    
    #else
        usleep(segundos);
    
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
    printf("\n\n");
    for(int li = 0; li < (linhas+1); li++){
        for(int col = 0; col < colunas; col++){

            if(li<linhas){
                Casa casa = jogo.tabuleiro[li][col];

                if(casa.vazio)
                    printf(" . ");

                if(strcmp(casa.ficha.dono, "jog1") == 0){
                    if(casa.ficha.tipo == "explo")
                        printf("\033[34m Q \033[0m");

                    else
                        printf("\033[34m X \033[0m");
                }
                    
                if(strcmp(casa.ficha.dono, "jog2") == 0){
                    if(casa.ficha.tipo == "explo")
                        printf("\033[31m Q \033[0m");

                    else
                        printf("\033[31m O \033[0m");
                } 

            }

            else{
                if(jogo.tabuleiro[0][col].vazio == 0)
                    printf("\033[31m %d \033[0m", col + 1);
                else
                    printf(" %d ", col + 1);
                
            }
        }
        printf("\n");
    }
}

void aplicarGravidadeColuna(Tabuleiro6x7 *jogo, int col){
    Ficha fichas[linhas];
    int count = 0;
    for(int row = linhas - 1; row >= 0; row--){
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

    for(int j = 0; j < count; j++){
        /*aplicar teste de condição de vitoria a cada ficha - futuramente*/
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

int VerificarLotacao(Tabuleiro6x7 *jogo){
    int lotado = 1;
    for(int col = 0; col < colunas; col++){
        if(jogo->tabuleiro[0][col].vazio){
            lotado = 0;
            break;
        }
    }
    return lotado;
}

void jogada(Tabuleiro6x7 *jogo, Ficha ficha){
    /*existia uma verificação aqui, mas essa verificação funcionava melhor como uma função a parte.*/
    /*da forma como o codigo está organizado, adicionar uma verficação aqui seria redundante*/
    verificarJogada(jogo, ficha);
}

Tabuleiro6x7 MontarJogo(){
    Tabuleiro6x7 Tabuleiro = MontarTabuleiro(linhas,colunas);
    CarregarJogo(Tabuleiro);

    return Tabuleiro;
}

void CriarJogador(jogador *player, int precedencia){
    scanf("%s", player->nome);
    player->QtdFichaNormal = 21;
    player->QtdFichaExplosiva = 0;
    player->QtdFichaPortal = 0;
    player->turno = 0;
    player->precedencia = precedencia;

}

void ExibirDadosDeJogador(jogador player){
    printf("Jogador %d: %s\n", player.precedencia, player.nome);
    printf("turno: %d\n", player.turno);
    printf("QTD fichas normais(1): %d\n", player.QtdFichaNormal);
    printf("QTD fichas explosivas(2): %d\n", player.QtdFichaExplosiva);
    printf("QTD fichas portal(3): %d\n", player.QtdFichaPortal);

}

void IniciarTurnoDoJogador(jogador *player){
    player->turno++;
    if(player->turno%5 == 0 && player->turno != 0){
        player->QtdFichaExplosiva += 2;
        player->QtdFichaPortal += 2;
    }
}

Ficha SelecionarFicha(jogador *player){
    int f=0;
    char nome[20];
    if(player->precedencia == 1)
        strcpy(nome, "jog1");
    else
        strcpy(nome, "jog2");

    printf("escolha um tipo de ficha: ");

    while(1){
        scanf("%d",&f);

        if(f == 1){
            if(player->QtdFichaNormal == 0){
                printf("Ficha esgotada, escolha outra ficha: ");
                continue;
            }
            player->QtdFichaNormal--;
            Ficha ficha;
            strcpy(ficha.dono, nome);
            strcpy(ficha.tipo, "normal");
            return ficha;
        }

        if(f == 2){
            if(player->QtdFichaExplosiva == 0){
                printf("Ficha esgotada, escolha outra ficha: ");
                continue;
            }
            player->QtdFichaExplosiva--;
            Ficha ficha;
            strcpy(ficha.dono, nome);
            strcpy(ficha.tipo, "explo");
            return ficha;
        }

        if(f == 3){
            if(player->QtdFichaPortal == 0){
                printf("Ficha esgotada, escolha outra ficha: ");
                continue;
            }
            player->QtdFichaPortal--;
            Ficha ficha;
            strcpy(ficha.dono, nome);
            strcpy(ficha.tipo, "port");
            return ficha;
        }

        else{
            printf("Valor invalido, digite um novo valor: ");
        }
    }
}

void PrincipalJxJ(){
    int TurnoGlobal = 0;
    jogador jogador1, jogador2;

    printf("nome do jogador 1: ");
    CriarJogador(&jogador1,1);

    printf("nome do jogador 2: ");
    CriarJogador(&jogador2,2);

    Tabuleiro6x7 jogo = MontarJogo();

    while(1){
        TurnoGlobal++;
        CarregarJogo(jogo);
        if(VerificarLotacao( &jogo)){
            printf("jogo encerrado, tabaluleiro lotado\n");
            printf("Partida empatada\n");
            break;
        }

        int jogador1TotalFicha = jogador1.QtdFichaNormal + jogador1.QtdFichaExplosiva + jogador1.QtdFichaPortal;
        int jogador2TotalFicha = jogador2.QtdFichaNormal + jogador2.QtdFichaExplosiva + jogador2.QtdFichaPortal;

        if(jogador1TotalFicha == 0 && jogador2TotalFicha == 0){
            printf("jogo encerrado, sem fichas restantes\n");
            printf("Partida empatada\n");
            break;
        }

        if(TurnoGlobal%2 == 0){
            IniciarTurnoDoJogador(&jogador2);
            ExibirDadosDeJogador(jogador2);

            if(jogador2TotalFicha == 0){
                printf("\nSem fichas disponiveis\n");
                printf("Iniciando turno do proximo jogador\n");
                esperar(2);
                continue;
            }

            Ficha Ficha = SelecionarFicha(&jogador2);
            jogada(&jogo, Ficha);
        }
        else{
            IniciarTurnoDoJogador(&jogador1);
            ExibirDadosDeJogador(jogador1);

            if(jogador1TotalFicha == 0){
                printf("\nSem fichas disponiveis\n");
                printf("Iniciando turno do proximo jogador\n");
                sleep(2);
                continue;
            }

            Ficha Ficha = SelecionarFicha(&jogador1);
            jogada(&jogo, Ficha);
        }
    }
}