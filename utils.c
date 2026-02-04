#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h> 
#endif

#include "utils.h"

void esperar(int milisegundos){
    #ifdef _WIN32
        Sleep(milisegundos);
    #else
        usleep(milisegundos * 1000);
    #endif
}

void LimparTerminal() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

const int linhas = 6;
const int colunas = 7;
int jogoContinua = 1;

Tabuleiro6x7 MontarTabuleiro(int linhas, int colunas){
    Tabuleiro6x7 tab;
    Ficha nula = {{0},"nulo"};

    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            tab.tabuleiro[i][j].ficha = nula;
            tab.tabuleiro[i][j].vazio = 1;
        }
    }

    return tab;
}

void CarregarJogo(Tabuleiro6x7 jogo){
    printf("\n\n\n");
    for(int li = 0; li < (linhas+1); li++){
        for(int col = 0; col < colunas; col++){
            if(li<linhas){
                Casa casa = jogo.tabuleiro[li][col];
                if(casa.vazio)
                    printf(" . ");

                else if(casa.ficha.dono.precedencia == 1){
                    printf("\033[34m X \033[0m");
                } 

                else if(casa.ficha.dono.precedencia == 2){
                    printf("\033[31m O \033[0m");
                }

                else{
                    printf(" ? ");
                }
            }
            else{
                if(jogo.tabuleiro[0][col].vazio == 0){
                    printf("\033[31m %d \033[0m", col + 1);
                }

                else{
                    printf(" %d ", col + 1);
                }
            }
        }
        printf("\n");
    }
}

void vitoria(Tabuleiro6x7 *jogo, jogador vencedor){
    jogoContinua = 0;

    jogadorHall vencedorHall = {"", vencedor.turno};
    strcpy(vencedorHall.nome, vencedor.nome);

    LimparTerminal();
    CarregarJogo(*jogo);
    printf("\n====== %s Ganhou!! ======\n", vencedor.nome);
    adicionarAoHall(vencedorHall);
}

void verificaVitoria(Tabuleiro6x7 *jogo, Ficha ficha, int row, int col){
    /* verificação pra baixo */
    int countSequence = 1;
    for(int i = row + 1; i < linhas ; i++){
        Casa casa = jogo->tabuleiro[i][col];
        if(strcmp(casa.ficha.dono.nome, ficha.dono.nome) == 0){ 
            countSequence++;
                if(countSequence == 4){
                    vitoria(jogo, ficha.dono);
                    break;
                }
        }else{
            break;
        }
    }

    /* Lateral */
    countSequence = 1;
    /* Verificação a direita */
    for(int j = col + 1; j <= colunas; j++){
        Casa casa = jogo->tabuleiro[row][j];
        if(strcmp(casa.ficha.dono.nome, ficha.dono.nome) == 0){ 
            countSequence++;
            if(countSequence == 4){
                vitoria(jogo, ficha.dono);
                break;
            }
        }else{
            break;
        }
    }
    /* Verificação a esquerda */
    for(int j = col -1; j >= 0; j--){
        Casa casa = jogo->tabuleiro[row][j];
        if(strcmp(casa.ficha.dono.nome, ficha.dono.nome) == 0){ 
            countSequence++;
            if(countSequence == 4){
                vitoria(jogo, ficha.dono);
                break;
            }
        }else{
            break;
        }
    }


    /* Diagonal coordenadas iguais */
    countSequence = 1; 
    /* Verificação (+,+)*/
    for(int i = row + 1, j = col + 1; i < linhas && j < colunas; i++, j++){
        Casa casa = jogo->tabuleiro[i][j];
        if(strcmp(casa.ficha.dono.nome, ficha.dono.nome) == 0){
            countSequence++;
            if(countSequence == 4){
                vitoria(jogo, ficha.dono);
                break;
            }
        }else{
            break;
        }
    }

    /* Verificação (-,-) */
    for(int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--){
        Casa casa = jogo->tabuleiro[i][j];
        if(strcmp(casa.ficha.dono.nome, ficha.dono.nome) == 0){
            countSequence++;
            if(countSequence == 4){
                vitoria(jogo, ficha.dono);
                break;
            }
        }else{
            break;
        }
    }
    
    /* Diagonal coordenadas opostas */
    countSequence = 1;
    /* Verificação (+,-) */
    for(int i = row + 1, j = col - 1; i < linhas && j >= 0; i++, j--){
        Casa casa = jogo->tabuleiro[i][j];
        if(strcmp(casa.ficha.dono.nome, ficha.dono.nome) == 0){
            countSequence++;
            if(countSequence == 4){
                vitoria(jogo, ficha.dono);
                break;
            }
        }else{
            break;
        }
    }

    /* Verificação (-,+) */
    for(int i = row - 1, j = col + 1; i >= 0 && j < colunas; i--, j++){
        Casa casa = jogo->tabuleiro[i][j];
        if(strcmp(casa.ficha.dono.nome, ficha.dono.nome) == 0){
            countSequence++;
            if(countSequence == 4){
                vitoria(jogo, ficha.dono);
                break;
            }
        }else{
            break;
        }
    }
}

void AplicarGravidadeFicha(Tabuleiro6x7 *jogo, int col){
    Ficha ficha = jogo->tabuleiro[0][col].ficha;

    for(int row = 0; row < (linhas-1); row++){
        if(jogo->tabuleiro[row+1][col].vazio){
            limparCasa(jogo,row,col);
            preencherCasa(jogo,(row+1),col,ficha);
            CarregarJogo(*jogo);
            esperar(500);
        }  
        else{
            verificaVitoria(jogo,ficha,row,col);
            break;
        }
    }
}

void limparCasa(Tabuleiro6x7 *jogo, int row, int col){
    Ficha nula = {{0} ,"nulo"};
    jogo->tabuleiro[row][col].ficha = nula;
    jogo->tabuleiro[row][col].vazio = 1;
}

void preencherCasa(Tabuleiro6x7 *jogo, int row, int col, Ficha ficha){
    jogo->tabuleiro[row][col].ficha = ficha;
    jogo->tabuleiro[row][col].vazio = 0;
}


void posicionarFicha(Tabuleiro6x7 *jogo, int col, Ficha ficha){
    preencherCasa(jogo,0,col,ficha);
    AplicarGravidadeFicha(jogo,col);
}

void verificarJogada(Tabuleiro6x7 *jogo, Ficha ficha){
    int col;
    while(1){
        printf("Escolha uma coluna (1-7): ");
        if(scanf("%d", &col) == 0){
            printf("\nDigite uma das alternativas!\n");
            while (getchar() != '\n');
            continue;
        }
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

void CriarBot(jogador *bot, int precedencia){
    if(precedencia == 1)
        strcpy(bot->nome, "bot1");
    else
        strcpy(bot->nome, "bot2");

    bot->QtdFichaNormal = 21;
    bot->QtdFichaExplosiva = 0;
    bot->QtdFichaPortal = 0;
    bot->turno = 0;
    bot->precedencia = precedencia;
}

void ExibirDadosDeJogador(jogador player){
    printf("-------------------\n");
    printf("Jogador %d: %s\n",player.precedencia, player.nome);
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
    Ficha ficha = {*player, "nulo"};
    
    printf("escolha um tipo de ficha: ");

    while(1){
        if(scanf("%d", &fichaEscolhida) == 0){
            printf("\nDigite uma das alternativas!\n");
            while (getchar() != '\n');
            continue;
        }

        if(fichaEscolhida == 1){
            if(player->QtdFichaNormal == 0){
                printf("Ficha esgotada, escolha outra ficha: ");
                continue;
            }
            player->QtdFichaNormal--;
            strcpy(ficha.tipo, "normal");
            return ficha;
        }

        if(fichaEscolhida == 2){
            if(player->QtdFichaExplosiva == 0){
                printf("Ficha esgotada, escolha outra ficha: ");
                continue;
            }
            player->QtdFichaExplosiva--;
            strcpy(ficha.tipo, "explo");
            return ficha;
        }

        if(fichaEscolhida == 3){
            if(player->QtdFichaPortal == 0){
                printf("Ficha esgotada, escolha outra ficha: ");
                continue;
            }
            player->QtdFichaPortal--;
            strcpy(ficha.tipo, "port");
            return ficha;
        }

        else{
            printf("Valor invalido, digite um novo valor: ");
        }
    }
}

Ficha BotSelecionarFicha(jogador *bot){
    Ficha ficha = {*bot,"nulo"};

    int fichaEscolhida = 1; 

    if(bot->QtdFichaExplosiva != 0 || bot->QtdFichaPortal != 0){ 
        fichaEscolhida = rand()%2 + 1;

        if(fichaEscolhida == 2 && bot->QtdFichaExplosiva == 0){ 
            if(bot->QtdFichaNormal < 0) fichaEscolhida = 1;
            else fichaEscolhida = 3;
        }
        else if(fichaEscolhida == 3 && bot->QtdFichaPortal == 0){
            if(bot->QtdFichaNormal < 0) fichaEscolhida = 1;
            else fichaEscolhida = 2;
        }
    }

    switch(fichaEscolhida){
        case 1:
            bot->QtdFichaNormal--; 
            strcpy(ficha.tipo, "normal");
            break;
        case 2:
            bot->QtdFichaExplosiva--;
            strcpy(ficha.tipo, "explo");
            break;
        case 3:
            bot->QtdFichaPortal--;
            strcpy(ficha.tipo, "port");
            break;
    }
    return ficha;
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

void BotJogada(Tabuleiro6x7 *jogo, jogador *bot){
    Ficha ficha = BotSelecionarFicha(bot);
    
    int col;
    int tentativas = 0;
    
    // Tenta achar uma coluna aleatória válida
    while(1){
        col = rand() % colunas; // Gera 0 a 6
        
        // Verifica se a coluna não está cheia (linha 0 vazia)
        if(jogo->tabuleiro[0][col].vazio){
            posicionarFicha(jogo, col, ficha); 
            break;
        }
        
        // Safety break: se o tabuleiro estiver quase cheio, evita loop infinito
        tentativas++;
        if(tentativas > 20) {
            // Se falhar muito no aleatório, procura a primeira livre (fallback)
            for(int i = 0; i < colunas; i++){
                 if(jogo->tabuleiro[0][i].vazio){
                    posicionarFicha(jogo, i, ficha);
                    return;
                 }
            }
        }
    }
}

void principalJxBot(){
    int TurnoGlobal = 0;
    jogador jogador1;

    printf("nome do jogador 1: ");
    CriarJogador(&jogador1,1);

    jogador Jogador2Bot;
    CriarBot(&Jogador2Bot, 2);

    Tabuleiro6x7 jogo = MontarJogo();

    jogoContinua = 1;
    while(jogoContinua){
        TurnoGlobal++;
        LimparTerminal();
        CarregarJogo(jogo);

        if(TurnoGlobal%2 == 0){
            IniciarTurnoDoJogador(&Jogador2Bot);
            ExibirDadosDeJogador(Jogador2Bot);
            if(existeJogadaValida(&jogo)){
                BotJogada(&jogo, &Jogador2Bot);
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

void principalBotxBot(){
    int TurnoGlobal = 0;
    jogador jogador1Bot;
    CriarBot(&jogador1Bot, 1);

    jogador Jogador2Bot;
    CriarBot(&Jogador2Bot, 2);

    Tabuleiro6x7 jogo = MontarJogo();

    jogoContinua = 1;
    while(jogoContinua){
        TurnoGlobal++;
        LimparTerminal();
        CarregarJogo(jogo);
        esperar(2000);


        if(TurnoGlobal%2 == 0){
            IniciarTurnoDoJogador(&Jogador2Bot);
            ExibirDadosDeJogador(Jogador2Bot);
            esperar(2000);

            if(existeJogadaValida(&jogo)){
                BotJogada(&jogo, &Jogador2Bot);
            }
        }
        else{
            IniciarTurnoDoJogador(&jogador1Bot);
            ExibirDadosDeJogador(jogador1Bot);
            esperar(2000);

            if(existeJogadaValida(&jogo)){
                BotJogada(&jogo, &jogador1Bot);
            }
        }
    }
}

void inicializarHallDaFama(){
    FILE *fp = fopen("hall_da_fama", "r");
    if(fp == NULL){
        FILE * createfp = fopen("hall_da_fama", "w");
        if(createfp == NULL){
            printf("Erro: Falha ao criar arquivo Hall da Fama!");
            return;
        }
        fclose(createfp);
        return;
    }
    fclose(fp);
}

void limparHallDaFama(){
    FILE *fp = fopen("hall_da_fama", "w");
    if(fp == NULL){
        printf("Erro: Falha ao limpar arquivo!");
        return;
    }
    fclose(fp);
}

void adicionarAoHall(jogadorHall jogador){
    if(jogador.pontuacao < 4){
        printf("Erro: Jogador nao pode entrar no hall com pontuacao < 4!\n");
        return;   
    }

    FILE *fp = fopen("hall_da_fama", "rb+");
    if(fp == NULL){
        printf("Erro: Falha ao procurar arquivo!");
        return;
    }
    
    jogadorHall jogadores[4];
    memset(jogadores, 0, sizeof(jogadores));

    int total = 0;
    while(fread(&jogadores[total], sizeof(jogadorHall), 1, fp) == 1){
        total++;
    }
    
    jogadores[total] = jogador;
    total++;

    /* Bubble sort, dá pra mudar depois, mas nesse caso como n=4 O(n^2) n chega a ser problema */
    for(int i = 0; i < total; i++){
        for(int j = 0; j < total; j++){
            if(jogadores[j].pontuacao > jogadores[i].pontuacao){
                jogadorHall temp = jogadores[i];
                jogadores[i] = jogadores[j];
                jogadores[j] = temp;
            }
        }
    }

    /* volta ao início do arquivo */
    rewind(fp);

    /* sobrescreve com os novos dados */
    fwrite(jogadores, sizeof(jogadorHall), total > 3 ? 3: total , fp);

    fclose(fp);
}

void exibirHallDaFama(){
    FILE *fp = fopen("hall_da_fama", "rb");
    if(fp == NULL){
        printf("Erro: Falha ao procurar arquivo!");
        return;
    }
    
    jogadorHall jogador;
    int count = 0;
    while(fread(&jogador, sizeof(jogadorHall), 1, fp) == 1) {
        count++;
        printf("-> %i lugar \n", count);
        printf("Nome do jogador: %s\n", jogador.nome);
        printf("Pontuacao: %i\n", jogador.pontuacao);
        printf("------------\n");
    }
    if(count == 0){
        printf("=================\n");
        printf("Nao ha vitoriosos\n");
        printf("=================\n");
    }
    char c;
    printf("Digite algo para sair:");
    getchar();
    scanf("%c", &c);

    LimparTerminal();

    fclose(fp);
}

void HallDaFama(){
    inicializarHallDaFama();
    exibirHallDaFama();
}

void Lig4(){
    int escolha = 0;
    LimparTerminal();

    while(1){
        printf("modos de jogo:\n\n");
        printf("1 - jogador x jogador\n");
        printf("2 - jogador x maquina\n");
        printf("3 - maquina x maquina\n\n");

        printf("digite 0 para volta.\n");
        
        printf("esolha: ");
        if(scanf("%d", &escolha) == 0){
            LimparTerminal();
            printf("\nDigite uma das alternativas!\n");
            while (getchar() != '\n');
            continue;
        }
        

        switch(escolha){
            case 0:
                LimparTerminal();
                return;
            case 1:
                LimparTerminal();
                PrincipalJxJ();
                continue;
                break;
            case 2:
                srand(time(0));
                LimparTerminal();
                principalJxBot();
                continue;
                break;
            case 3:
                srand(time(0));
                LimparTerminal();
                principalBotxBot();
                continue;
                break;
            default:
                continue;
        }
    }
}
