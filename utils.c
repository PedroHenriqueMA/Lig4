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
int jogoContinua = 1; /* Controle do while */

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

void verificaVitoria(Tabuleiro6x7 *jogo, Ficha ficha, int row, int col){
    /* verificação pra baixo */
    int countSequence = 1;
    for(int i = row + 1; i < linhas ; i++){
        Casa casa = jogo->tabuleiro[i][col];
        if(strcmp(casa.ficha.dono, ficha.dono) == 0){ 
            countSequence++;
                /* Vitória */
                if(countSequence == 4){
                    LimparTerminal();
                    printf("\n======%s Ganhou!!======\n", ficha.dono);
                    CarregarJogo(*jogo);
                    jogoContinua = 0;
                    /* Verificar se é candidato a entrar no Hall da fama */
                    break;
                }
        }else{
            countSequence = 1;
            break;
        }
    }

    /* Verificar nos outros sentidos */
}

void aplicarGravidadeColuna(Tabuleiro6x7 *jogo, int col){
    Ficha fichas[6];
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

    int rowUltimaFicha = linhas - count;
    Ficha UltimaFichaDaColuna = jogo->tabuleiro[rowUltimaFicha][col].ficha;
    verificaVitoria(jogo, UltimaFichaDaColuna, rowUltimaFicha, col);

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

int existeJogadaValida(Tabuleiro6x7 *jogo){
    for(int col = 0; col < colunas; col++){
        if(jogo->tabuleiro[0][col].vazio){
            return 1;
        }
    }
    printf("Tabuleiro cheio! Jogo empatado.\n");
    jogoContinua = 0;
    return 0;
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
    printf("-------------------\n");
    printf("Jogador 1: %s\n", player.nome);
    printf("turno: %d\n", player.turno);
    printf("\n");
    printf("QTD fichas normais(1): %d\n", player.QtdFichaNormal);
    printf("QTD fichas explosivas(2): %d\n", player.QtdFichaExplosiva);
    printf("QTD fichas portal(3): %d\n", player.QtdFichaPortal);
    printf("\n");
}

void IniciarTurnoDoJogador(jogador *player){
    player->turno++;
    if(player->turno%5 == 0 && player->turno != 0){
        player->QtdFichaExplosiva += 2;
        player->QtdFichaPortal += 2;
    }
}

Ficha SelecionarFicha(jogador *player){
    int fichaEscolhida = 0;
    char nome[20];
    if(player->precedencia == 1)
        strcpy(nome, "jog1");
    else
        strcpy(nome, "jog2");

    printf("escolha um tipo de ficha: ");

    while(1){
        scanf("%d",&fichaEscolhida);

        if(fichaEscolhida == 1){
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

        if(fichaEscolhida == 2){
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

        if(fichaEscolhida == 3){
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

    jogoContinua = 1;
    while(jogoContinua){
        TurnoGlobal++;
        LimparTerminal();
        CarregarJogo(jogo);

        if(TurnoGlobal%2 == 0){
            IniciarTurnoDoJogador(&jogador2);
            ExibirDadosDeJogador(jogador2);
            if(existeJogadaValida(&jogo)){
                Ficha Ficha = SelecionarFicha(&jogador2);
                verificarJogada(&jogo, Ficha);
            }
        }
        else{
            IniciarTurnoDoJogador(&jogador1);
            ExibirDadosDeJogador(jogador1);
            if(existeJogadaValida(&jogo)){
                Ficha Ficha = SelecionarFicha(&jogador1);
                verificarJogada(&jogo, Ficha);
            }
        }
    }
}