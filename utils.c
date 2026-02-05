#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "utils.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h> 
#endif


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

void ExibirJogo(Tabuleiro6x7 *jogo){
    printf("\n\n\n");
    for(int li = 0; li < (linhas+1); li++){
        for(int col = 0; col < colunas; col++){
            if(li<linhas){
                Casa casa = jogo->tabuleiro[li][col];
                if(casa.vazio)
                    printf(" . ");

                else if(casa.ficha.dono.precedencia == 1){
                    if(strcmp(casa.ficha.tipo,"normal") == 0)
                        printf("\033[34m X \033[0m");
                    else if(strcmp(casa.ficha.tipo, "explo") == 0)
                        printf("\033[34m @ \033[0m");
                    else if(strcmp(casa.ficha.tipo, "port") == 0)
                        printf("\033[34m + \033[0m");
                } 

                else if(casa.ficha.dono.precedencia == 2){
                    if(strcmp(casa.ficha.tipo,"normal") == 0)
                        printf("\033[31m O \033[0m");
                    else if(strcmp(casa.ficha.tipo, "explo") == 0)
                        printf("\033[31m @ \033[0m");
                    else if(strcmp(casa.ficha.tipo, "port") == 0)
                        printf("\033[31m + \033[0m");
                }

                else{
                    printf(" ? ");
                }
            }
            else{
                if(jogo->tabuleiro[0][col].vazio == 0){
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
    ExibirJogo(jogo);
    printf("\n==== %s Ganhou!! ====\n", vencedor.nome);
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
    Ficha ficha = jogo->tabuleiro[0][col].ficha; /*Pega a ficha que esta na primeira linha da coluna*/
    int finalRow = 0;

    for(int row = 0; row < (linhas-1); row++){      /*Percorre a coluna do topo para baixo*/
        if(jogo->tabuleiro[row+1][col].vazio){      /*testa se a linha de baixo está vazia*/
            limparCasa(jogo,row,col);               /*deleta a ficha da casa atual*/
            preencherCasa(jogo,(row+1),col,ficha);  /*posiciona ela na linha de baixo*/
            finalRow = row+1;

            /*Logica de animação*/
            LimparTerminal();
            ExibirJogo(jogo);
            esperar(200);
        }  
        else{
            /*Se a casa abaixo está preenchida, a ficha já esta no ponto mais baixo possivel*/
            break;
        }
    }
    /*Verifica a vitoria apos mover a ficha*/
    verificaVitoria(jogo,ficha, finalRow, col);
}

int TemBuracoNaColuna(Tabuleiro6x7 *jogo, int col){
    /* Retorna 1 se houver um "buraco", 0 caso contrario */

    for(int row = 0; row < linhas; row++){           /*Roda o looping até achar a primeira casa preenchida*/
        if(!jogo->tabuleiro[row][col].vazio){
            for(; row < linhas; row++){              /*se achar uma casa preenchida, procura por uma casa vazia abaixo dela*/
                if(jogo->tabuleiro[row][col].vazio){ /*Se existe uma casa vazia abaixo de uma preenchida, existe um buraco*/
                    return 1;
                }
            }
        }
    }

    /*Se o codigo rodou até aqui, ou não exisitia nenhuma casa preenchida, ou não haviam casas vazias entre elas*/
    /*sem buracos então*/
    return 0;
}

void aplicarGravidadeColuna(Tabuleiro6x7 *jogo, int col){
    int linhaDeEscrita;
    /*o processo está propositalmente ineficiente, essa logica menos otimizada permite a ilusão de uma animação de queda*/
    /*se o proposito for otimização maxima, retire o teste da linha de escrita no primeiro if e o laço while*/
    while(TemBuracoNaColuna(jogo,col)){
        linhaDeEscrita = linhas - 1;
        for(int row = (linhas - 1); row >= 0; row--){ /*pecorre a coluna de baixo para cima*/

            /*no primeiro buraco que encontrar, avança o looping para tornar a linha de escrita abaixo da linha de leitura*/
            if(jogo->tabuleiro[row][col].vazio && linhaDeEscrita == row){ 
            continue;
            }

            /*se a casa está preenchida e já foi detectado um buraco (linha de estrica diferente da linha de leitura)*/
            /*ele transfere o conteudo lido para a linha de escrita (vai ser a linha abaixo)*/
            if(!jogo->tabuleiro[row][col].vazio && linhaDeEscrita != row){
                Ficha ficha = jogo->tabuleiro[row][col].ficha;
                limparCasa(jogo, row, col);
                preencherCasa(jogo,linhaDeEscrita,col,ficha);
                LimparTerminal();
                ExibirJogo(jogo);
                esperar(200);
            }

            linhaDeEscrita--;
        }
    }
    for(int row = (linhas-1); row >= 0; row--){
        verificaVitoria(jogo,jogo->tabuleiro[row][col].ficha,row,col);
    }
}

void limparCasa(Tabuleiro6x7 *jogo, int row, int col){
    Ficha nula = {{0},"nulo"};
    jogo->tabuleiro[row][col].ficha = nula;
    jogo->tabuleiro[row][col].vazio = 1;
}

void preencherCasa(Tabuleiro6x7 *jogo, int row, int col, Ficha ficha){
    jogo->tabuleiro[row][col].ficha = ficha;
    jogo->tabuleiro[row][col].vazio = 0;
}

void posicionarFicha(Tabuleiro6x7 *jogo, int col, Ficha ficha){
    preencherCasa(jogo,0,col,ficha); /*posiciona a ficha na prieira casa da coluna*/
    AplicarGravidadeFicha(jogo,col); /*aplica a gravidade para fazer ela descer até o ponto correto*/
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
    /*Verifica se existe ao menos uma coluna sem ficha no topo*/
    for(int col = 0; col < colunas; col++){
        if(jogo->tabuleiro[0][col].vazio){
            return 1;
        }
    }
    /*se o tabuleiro estiver cheio, decreta empate e finaliza o jogo*/
    printf("Tabuleiro cheio! Jogo empatado.\n");
    jogoContinua = 0;
    return 0;
}

Tabuleiro6x7 MontarJogo(){
    /*cria um novo tabuleiro, e o exibe no terminal*/
    Tabuleiro6x7 Tabuleiro = MontarTabuleiro(linhas,colunas);
    ExibirJogo(&Tabuleiro);

    return Tabuleiro;
}

void CriarJogador(jogador *player, int precedencia){
    /*carrega os dados do jogador, solicita o nome ao usuario*/
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
    player->turno++; /*aumenta o turno do jogador*/

    /*se o turno for um multiplo de 5, gera as fichas especiais*/
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
    /*gera uma ficha do bot com o tipo nulo (o tipo vai ser definido depois)*/
    Ficha ficha = {*bot,"nulo"};

    /*escolhe a ficha normal como padrão*/
    int fichaEscolhida = 1; 

    /*se ele possuir pelo menos uma ficha especial*/
    if(bot->QtdFichaExplosiva != 0 || bot->QtdFichaPortal != 0){ 
        /*gera um numero aleatório entre 1 e 3 (definição do tipo)*/
        fichaEscolhida = rand()%3 + 1;

        /*se ele escolheu a ficha explosiva mas só tinha ficha portal, muda o tipo*/
        if(fichaEscolhida == 2 && bot->QtdFichaExplosiva == 0){ 
            if(bot->QtdFichaNormal > 0) fichaEscolhida = 1;
            else fichaEscolhida = 3;
        }
        /*se ele escolheu a ficha portal mas so tinha explosiva, muda o tipo*/
        else if(fichaEscolhida == 3 && bot->QtdFichaPortal == 0){
            if(bot->QtdFichaNormal > 0) fichaEscolhida = 1;
            else fichaEscolhida = 2;
        }
    }

    switch(fichaEscolhida){
        case 1: //ficha normal
            bot->QtdFichaNormal--; 
            strcpy(ficha.tipo, "normal");
            break;
        case 2: //ficha explosiva
            bot->QtdFichaExplosiva--;
            strcpy(ficha.tipo, "explo");
            break;
        case 3: //ficha portal
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
        ExibirJogo(&jogo);

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
        ExibirJogo(&jogo);

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
        ExibirJogo(&jogo);
        esperar(1500);


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

/*Ambiente de testes*/
/*
int main(){
    
    jogador jog1;
    CriarBot(&jog1,2);
    Tabuleiro6x7 jogo = MontarJogo();
    Ficha ficha = {jog1,"normal"};
    //preencherCasa(&jogo,5,1,ficha);
    preencherCasa(&jogo,4,1,ficha);
    //preencherCasa(&jogo,3,1,ficha);
    preencherCasa(&jogo,2,1,ficha);
    preencherCasa(&jogo,1,1,ficha);
    preencherCasa(&jogo,0,1,ficha);
    ExibirJogo(&jogo);
    aplicarGravidadeColuna(&jogo,1);
}
*/
