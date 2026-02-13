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

// ====== Variaveis globais e flags
const int linhas = 6;
const int colunas = 7;
int jogoContinua = 1;
int vitoriaJog1 = 0;
int vitoriaJog2 = 0;

// ====== Utilidades diversas

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

// ======= Exibição do jogo

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
                    else if(strcmp(casa.ficha.tipo, "explosiva") == 0)
                        printf("\033[34m @ \033[0m");
                    else if(strcmp(casa.ficha.tipo, "portal") == 0)
                        printf("\033[34m + \033[0m");
                } 

                else if(casa.ficha.dono.precedencia == 2){
                    if(strcmp(casa.ficha.tipo,"normal") == 0)
                        printf("\033[31m O \033[0m");
                    else if(strcmp(casa.ficha.tipo, "explosiva") == 0)
                        printf("\033[31m @ \033[0m");
                    else if(strcmp(casa.ficha.tipo, "portal") == 0)
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

// ======= Verificação de vitória

void vitoria(Tabuleiro6x7 *jogo, jogador vencedor){
    jogoContinua = 0;

    jogadorHall vencedorHall = {"", vencedor.turno};
    strcpy(vencedorHall.nome, vencedor.nome);

    LimparTerminal();
    ExibirJogo(jogo);
    printf("\n==== %s Ganhou!! ====\n", vencedor.nome);
    adicionarAoHall(vencedorHall);
}

int verificaVitoria(Tabuleiro6x7 *jogo, Ficha ficha, int row, int col){
    /* Impede chamar vitoria a partir de um ponto vazio */
    if(jogo->tabuleiro[row][col].vazio ){
        return 0;
    }

    /* Verificação pra baixo */
    int countSequence = 1;
    for(int i = row + 1; i < linhas ; i++){
        Casa casa = jogo->tabuleiro[i][col];
        if(casa.ficha.dono.precedencia == ficha.dono.precedencia){
            countSequence++;
            if(countSequence == 4){
                if(ficha.dono.precedencia == 1){
                    vitoriaJog1 = 1;
                }
                else{
                    vitoriaJog2 = 1;
                }
                return 1;
                break;
            }
        }else{
            break;
        }
    }

    /* Lateral */
    countSequence = 1;
    /* Verificação a direita */
    for(int j = col + 1; j < colunas; j++){
        Casa casa = jogo->tabuleiro[row][j];
        if(casa.ficha.dono.precedencia == ficha.dono.precedencia){
            countSequence++;
            if(countSequence == 4){
                if(ficha.dono.precedencia == 1){
                    vitoriaJog1 = 1;
                }
                else{
                    vitoriaJog2 = 1;
                }
                return 1;
                break;
            }
        }else{
            break;
        }
    }
    /* Verificação a esquerda */
    for(int j = col -1; j >= 0; j--){
        Casa casa = jogo->tabuleiro[row][j];
        if(casa.ficha.dono.precedencia == ficha.dono.precedencia){
            countSequence++;
            if(countSequence == 4){
                if(ficha.dono.precedencia == 1){
                    vitoriaJog1 = 1;
                }
                else{
                    vitoriaJog2 = 1;
                }
                return 1;
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
        if(casa.ficha.dono.precedencia == ficha.dono.precedencia){
            countSequence++;
            if(countSequence == 4){
                if(ficha.dono.precedencia == 1){
                    vitoriaJog1 = 1;
                }
                else{
                    vitoriaJog2 = 1;
                }
                return 1;
                break;
            }
        }else{
            break;
        }
    }

    /* Verificação (-,-) */
    for(int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--){
        Casa casa = jogo->tabuleiro[i][j];
        if(casa.ficha.dono.precedencia == ficha.dono.precedencia){
            countSequence++;
            if(countSequence == 4){
                if(ficha.dono.precedencia == 1){
                    vitoriaJog1 = 1;
                }
                else{
                    vitoriaJog2 = 1;
                }
                return 1;
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
        if(casa.ficha.dono.precedencia == ficha.dono.precedencia){
            countSequence++;
            if(countSequence == 4){
                if(ficha.dono.precedencia == 1){
                    vitoriaJog1 = 1;
                }
                else{
                    vitoriaJog2 = 1;
                }
                return 1;
                break;
            }
        }else{
            break;
        }
    }

    /* Verificação (-,+) */
    for(int i = row - 1, j = col + 1; i >= 0 && j < colunas; i--, j++){
        Casa casa = jogo->tabuleiro[i][j];
        if(casa.ficha.dono.precedencia == ficha.dono.precedencia){
            countSequence++;
            if(countSequence == 4){
                if(ficha.dono.precedencia == 1){
                    vitoriaJog1 = 1;
                }
                else{
                    vitoriaJog2 = 1;
                }
                return 1;
                break;
            }
        }else{
            break;
        }
    }

    return 0;
}

// ======= Gravidade

void AplicarGravidadeFicha(Tabuleiro6x7 *jogo, int col){
    Ficha ficha = jogo->tabuleiro[0][col].ficha; /* Pega a ficha que esta na primeira linha da coluna */
    int finalRow = 0;

    for(int row = 0; row < (linhas-1); row++){      /* Percorre a coluna do topo para baixo */
        if(jogo->tabuleiro[row+1][col].vazio){      /* Testa se a linha de baixo está vazia */
            limparCasa(jogo,row,col);               /* Deleta a ficha da casa atual */
            preencherCasa(jogo,(row+1),col,ficha);  /* Posiciona ela na linha de baixo */
            finalRow = row+1;

            /* Logica de animação */
            LimparTerminal();
            ExibirJogo(jogo);
            esperar(200);
        }  
        else{
            /* Se a casa abaixo está preenchida, a ficha já esta no ponto mais baixo possivel */
            break;
        }
    }

    if(strcmp(ficha.tipo,"portal") == 0){
        Teleportar(jogo,finalRow,col);
    }

    /* Verifica a vitoria apos mover a ficha */
    if(verificaVitoria(jogo, ficha, finalRow, col)){
        /* Chama a vitoria antes para impedir um possivel efeito de explosão e otimizar o codigo */
        vitoria(jogo, ficha.dono);        
    }

    if((finalRow + 1) < 6){
        Ficha fichaAbaixo = jogo->tabuleiro[finalRow+1][col].ficha;
        if(strcmp(fichaAbaixo.tipo,"explosiva") == 0 && fichaAbaixo.dono.precedencia != ficha.dono.precedencia){
            GerarExplosao(jogo,finalRow+1,col,ficha);
        }

        /* Se algum jogador venceu apos ativar uma ficha explosiva roda os seguintes testes */
        if(vitoriaJog1 || vitoriaJog2){
            jogador jog1;
            jogador jog2;

            /* Define quem é o jogador 1 e quem é o jogador 2 */
            if(ficha.dono.precedencia == 1){
                jog1 = ficha.dono;
                jog2 = fichaAbaixo.dono;
            }
            else{
                jog1 = fichaAbaixo.dono;
                jog2 = ficha.dono;
            }

            /* Se ao menos um deles venceu mas a flag do jogador 2 continua zerada. então o jogador 1 venceu */
            if(vitoriaJog2 == 0){
                vitoria(jogo, jog1);
            }

            /* Se ao menos um deles venceu mas a flag do jogador 1 continua zerada. então o jogador 2 venceu */
            else if(vitoriaJog1 == 0){
                vitoria(jogo, jog2);
            }

            /* Se ambos venceram, então da a vitória para o dono da ultima ficha jogada (ultimo jogador à jogar, dono do turno e quem ativou a ficha explosiva) */
            else if(vitoriaJog1 && vitoriaJog2){
                vitoria(jogo, ficha.dono);
            }
        }
    }
}

int TemBuracoNaColuna(Tabuleiro6x7 *jogo, int col){
    for(int row = 0; row < linhas; row++){           // Roda o looping até achar a primeira casa preenchida
        if(!jogo->tabuleiro[row][col].vazio){
            for(; row < linhas; row++){              /* Se achar uma casa preenchida, procura por uma casa vazia abaixo dela */
                if(jogo->tabuleiro[row][col].vazio){ /* Se existe uma casa vazia abaixo de uma preenchida, existe um buraco */
                    return 1;
                }
            }
        }
    }
    return 0;
}

void aplicarGravidadeColuna(Tabuleiro6x7 *jogo, int col){
    int linhaDeEscrita;
    /* O processo está propositalmente ineficiente, essa logica menos otimizada permite a ilusão de uma animação de queda */
    /* Se o proposito for otimização maxima, retire o teste da linha de escrita no primeiro if e o laço while */
    while(TemBuracoNaColuna(jogo,col)){
        linhaDeEscrita = linhas - 1;
        for(int row = (linhas - 1); row >= 0; row--){ /* Pecorre a coluna de baixo para cima */

            /* No primeiro buraco que encontrar, avança o looping para tornar a linha de escrita abaixo da linha de leitura */
            if(jogo->tabuleiro[row][col].vazio && linhaDeEscrita == row){ 
            continue;
            }

            /* Se a casa está preenchida e já foi detectado um buraco (linha de estrica diferente da linha de leitura) */
            /* Ele transfere o conteudo lido para a linha de escrita (vai ser a linha abaixo) */
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
        if(!jogo->tabuleiro[row][col].vazio){
            verificaVitoria(jogo, jogo->tabuleiro[row][col].ficha, row, col);
        }
    }
}
// ======= Logica de Jogadas

void limparCasa(Tabuleiro6x7 *jogo, int row, int col){
    Ficha nula = { { {0} }, "nulo" };
    jogo->tabuleiro[row][col].ficha = nula;
    jogo->tabuleiro[row][col].vazio = 1;
}

void preencherCasa(Tabuleiro6x7 *jogo, int row, int col, Ficha ficha){
    jogo->tabuleiro[row][col].ficha = ficha;
    jogo->tabuleiro[row][col].vazio = 0;
}

void posicionarFicha(Tabuleiro6x7 *jogo, int col, Ficha ficha){
    preencherCasa(jogo,0,col,ficha); /* Posiciona a ficha na prieira casa da coluna */
    AplicarGravidadeFicha(jogo,col); /* Aplica a gravidade para fazer ela descer até o ponto correto, também aplica o teste de vitoria */
}

void verificarJogada(Tabuleiro6x7 *jogo, Ficha ficha){
    int col;
    while(1){
        printf("Escolha uma coluna (1-7): ");

        /* Entrada invalida */
        if(scanf("%d", &col) == 0){
            printf("\nDigite uma das alternativas!\n");
            while (getchar() != '\n');
            continue;
        }
        col--; /* Redução para se adequar ao index do array (0-6) */

        /* Out of bounds */
        if(col < 0 || col >= colunas){
            printf("Coluna invalida. Tente novamente.\n");
            continue;
        }
        /* Coluna lotada */
        if(jogo->tabuleiro[0][col].vazio == 0){
            printf("Coluna cheia. Escolha outra coluna.\n");
            continue;
        }
        /* Quando a jogada é valida, chama a função que realiza o posicionamento da ficha */
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
    /* Se o tabuleiro estiver cheio, decreta empate e finaliza o jogo */
    printf("Tabuleiro cheio! Jogo empatado.\n");
    jogoContinua = 0;
    return 0;
}

// ======= Iniciar jogo

Tabuleiro6x7 MontarTabuleiro(int linhas, int colunas){
    Tabuleiro6x7 tab;
    Ficha nula = { { {0} }, "nulo" };

    /* Gera um tabuleiro de fichas nulas com todas as casas definidas como vazias */
    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            tab.tabuleiro[i][j].ficha = nula;
            tab.tabuleiro[i][j].vazio = 1;
        }
    }
    return tab;
}

Tabuleiro6x7 MontarJogo(){
    /* Cria um novo tabuleiro, e o exibe no terminal */
    Tabuleiro6x7 Tabuleiro = MontarTabuleiro(linhas,colunas);
    ExibirJogo(&Tabuleiro);
    return Tabuleiro;
}

// ======= Jogadores

void CriarJogador(jogador *player, int precedencia){
    /* Solicita o nome ao usuario */
    scanf("%s", player->nome);

    /* Quantidade padrão de fichas iniciais */
    player->QtdFichaNormal = 21;
    player->QtdFichaExplosiva = 0;
    player->QtdFichaPortal = 0;

    /* Variaveis de turno e ordem de jogada */
    player->turno = 0;
    player->precedencia = precedencia;
}

void ExibirDadosDeJogador(jogador player){
    printf("-------------------\n");
    printf("Jogador %d: %s\n",player.precedencia, player.nome);
    printf("turno: %d\n", player.turno);
    printf("\n");
    printf("QTD fichas normais(1): %d\n", player.QtdFichaNormal);
    printf("QTD fichas explosivasivas(2): %d\n", player.QtdFichaExplosiva);
    printf("QTD fichas Portal(3): %d\n", player.QtdFichaPortal);
    printf("\n");
}

void IniciarTurnoDoJogador(jogador *player){
    player->turno++; /* Aumenta o turno do jogador */

    /* Se o turno for um multiplo de 5, gera as fichas especiais */
    if(player->turno%5 == 0 && player->turno != 0){ 
        player->QtdFichaExplosiva += 2;
        player->QtdFichaPortal += 2;
    }
}

Ficha SelecionarFicha(jogador *player){
    /* Gera uma ficha do tipo selecionada pelo jogador */
    int fichaEscolhida = 0;
    Ficha ficha = {*player, "nulo"};
    
    printf("escolha um tipo de ficha: ");

    /* Tratamento de exceção */
    while(1){
        if(scanf("%d", &fichaEscolhida) == 0){
            printf("\nDigite uma das alternativas!\n");
            while (getchar() != '\n');
            continue;
        }
        /* Ficha normal */
        if(fichaEscolhida == 1){
            if(player->QtdFichaNormal == 0){
                printf("Ficha esgotada, escolha outra ficha: ");
                continue;
            }
            player->QtdFichaNormal--;
            strcpy(ficha.tipo, "normal");
            return ficha;
        }
        /* Ficha explosiva */
        if(fichaEscolhida == 2){
            if(player->QtdFichaExplosiva == 0){
                printf("Ficha esgotada, escolha outra ficha: ");
                continue;
            }
            player->QtdFichaExplosiva--;
            strcpy(ficha.tipo, "explosiva");
            return ficha;
        }
        /* Ficha portal */
        if(fichaEscolhida == 3){
            if(player->QtdFichaPortal == 0){
                printf("Ficha esgotada, escolha outra ficha: ");
                continue;
            }
            player->QtdFichaPortal--;
            strcpy(ficha.tipo, "portal");
            return ficha;
        }
        else{
            printf("Valor invalido, digite um novo valor: ");
        }
    }
}

// ======= Bots

void BotJogada(Tabuleiro6x7 *jogo, jogador *bot){
    Ficha ficha = BotSelecionarFicha(bot);
    
    int col;
    int tentativas = 0;
    
    // Tenta achar uma coluna aleatória válida
    while(1){
        col = rand() % colunas; // Gera 0 a 6
        
        // Verifica se a coluna escolhida não está cheia (linha 0 vazia)
        if(jogo->tabuleiro[0][col].vazio){
            printf("Bot jogou uma ficha %s na coluna %d", ficha.tipo, (col+1));
            esperar(800);
            posicionarFicha(jogo, col, ficha);
            break;
        }
        
        // Safety break: se o tabuleiro estiver quase cheio, evita loop infinito
        tentativas++;
        if(tentativas > 20) {
            // Se falhar muito no aleatório, procura a primeira livre (fallback)
            for(int i = 0; i < colunas; i++){
                 if(jogo->tabuleiro[0][i].vazio){
                    printf("Bot jogou uma ficha %s na coluna %d", ficha.tipo, (i+1));
                    esperar(800);
                    posicionarFicha(jogo, i, ficha);
                    return;
                 }
            }
        }
    }
}

void CriarBot(jogador *bot, int precedencia){
    /* Nome definido pela ordem de jogada */
    if(precedencia == 1)
        strcpy(bot->nome, "bot1");
    else
        strcpy(bot->nome, "bot2");
    /* Quantidade padrão de fichas iniciais */
    bot->QtdFichaNormal = 21;
    bot->QtdFichaExplosiva = 0;
    bot->QtdFichaPortal = 0;
    /* Controle de turno e ordem de jogada */
    bot->turno = 0;
    bot->precedencia = precedencia;
}

Ficha BotSelecionarFicha(jogador *bot){
    /* Gera uma ficha do bot com o tipo nulo (o tipo vai ser definido depois) */
    Ficha ficha = {*bot,"nulo"};

    /* Escolhe a ficha normal como padrão */
    int fichaEscolhida = 1; 

    /* Se ele possuir pelo menos uma ficha especial */
    if(bot->QtdFichaExplosiva != 0 || bot->QtdFichaPortal != 0){ 
        /* Gera um numero aleatório entre 1 e 3 (definição do tipo) */
        fichaEscolhida = rand()%3 + 1;

        /* Se ele escolheu a ficha explosivasiva mas só tinha ficha Portal, muda o tipo */
        if(fichaEscolhida == 2 && bot->QtdFichaExplosiva == 0){ 
            if(bot->QtdFichaNormal > 0) fichaEscolhida = 1;
            else fichaEscolhida = 3;
        }
        /* Se ele escolheu a ficha Portal mas so tinha explosivasiva, muda o tipo */
        else if(fichaEscolhida == 3 && bot->QtdFichaPortal == 0){
            if(bot->QtdFichaNormal > 0) fichaEscolhida = 1;
            else fichaEscolhida = 2;
        }
    }

    switch(fichaEscolhida){
        case 1: // Ficha normal
            bot->QtdFichaNormal--; 
            strcpy(ficha.tipo, "normal");
            break;
        case 2: // Ficha explosiva
            bot->QtdFichaExplosiva--;
            strcpy(ficha.tipo, "explosiva");
            break;
        case 3: // Ficha Portal
            bot->QtdFichaPortal--;
            strcpy(ficha.tipo, "portal");
            break;
    }
    return ficha;
}

// ======= Fichas especiais

void GerarExplosao(Tabuleiro6x7 *jogo, int row, int col, Ficha gatilho){
    explodirBomba(jogo,row,col); /* Gera a explosão inicial que pode resultar em recursividade */

    for(int col =  0; col < (colunas -1); col++){
        /* Aplica a gravidade em todo o tabuleiro após a explosão recursiva */
        /* Junto com a gravidade, aplica também a verificação de vitoria */
        aplicarGravidadeColuna(jogo,col);
    }

    explosaoTardia(jogo);
}

void explodirBomba(Tabuleiro6x7 *jogo, int row, int col){
    /* Apaga a bomba central primeiro para evitar loop infinito na recursão */
    limparCasa(jogo, row, col);

    /* Começa na coluna anterior e vai até a coluna posterior */
    for(int coluna = (col - 1); coluna <= (col + 1); coluna++){
        /* Bounds check usando as variáveis globais 'colunas' */
        if(coluna < 0 || coluna >= colunas){ 
            continue;
        }

        /* Começa na linha acima e vai até a linha abaixo */
        for(int linha = (row - 1); linha <= (row + 1); linha++){
            /* Bounds check usando as variáveis globais 'linhas' */
            if(linha < 0 || linha >= linhas){ 
                continue;
            }

            /* Ignora a própria casa que já foi apagada (otimização) */
            if (linha == row && coluna == col) continue;

            /* Verifica se a casa não está vazia */
            if(jogo->tabuleiro[linha][coluna].vazio == 0){
                
                /* Verifica se é explosiva antes de apagar */
                int ehExplosiva = (strcmp(jogo->tabuleiro[linha][coluna].ficha.tipo, "explosiva") == 0);

                /* Esvazia a casa (explode a ficha vizinha) */
                limparCasa(jogo, linha, coluna);

                /* Recursividade só acontece se esta ficha específica era explosiva */
                if(ehExplosiva){
                    explodirBomba(jogo, linha, coluna);
                }
            }
        }
    }
}

void explosaoTardia(Tabuleiro6x7 *jogo){
    for(int col = 0; col < colunas; col++){
        for(int linha = 0; linha < (linhas-1); linha++){
            if(!jogo->tabuleiro[linha+1][col].vazio && !jogo->tabuleiro[linha][col].vazio){
                Ficha fichaAbaixo = jogo->tabuleiro[linha+1][col].ficha;
                if(strcmp(fichaAbaixo.tipo,"explosiva") == 0 && fichaAbaixo.dono.precedencia != jogo->tabuleiro[linha][col].ficha.dono.precedencia){
                    GerarExplosao(jogo,linha+1,col,jogo->tabuleiro[linha][col].ficha);
                }
            }

        }
    }
    
}

void Teleportar(Tabuleiro6x7 *jogo, int row, int col){
    /* Bounds */
    if((row+1) < 6){
        /* Teste de mesmo dono usando a precedencia do jogador */
        if(jogo->tabuleiro[row][col].ficha.dono.precedencia != jogo->tabuleiro[row + 1][col].ficha.dono.precedencia){
            /* Apaga a ficha adversaria abaixo*/
            limparCasa(jogo,(row+1),col);
        }
    }
    /* Sempre se apaga no final */
    limparCasa(jogo,row,col);
}

// ======= Modos de jogo

void PrincipalJxJ(){
    int TurnoGlobal = 0;
    jogador jogador1, jogador2;

    /* Cria os jogadores, solicita os nomes ao usuario */
    printf("nome do jogador 1: ");
    CriarJogador(&jogador1,1);

    printf("nome do jogador 2: ");
    CriarJogador(&jogador2,2);

    /* Inicia o tabuleiro do jogo */
    Tabuleiro6x7 jogo = MontarJogo();

    vitoriaJog1 = 0;
    vitoriaJog2 = 0;
    jogoContinua = 1;
    while(jogoContinua){
        TurnoGlobal++;
        LimparTerminal();
        ExibirJogo(&jogo);

        /* O jogador 2 joga em turnos pares */
        if(TurnoGlobal%2 == 0){
            IniciarTurnoDoJogador(&jogador2);
            ExibirDadosDeJogador(jogador2);
            /*verifica se o tabuleiro não está lotado*/
            if(existeJogadaValida(&jogo)){
                Ficha Ficha = SelecionarFicha(&jogador2);
                verificarJogada(&jogo, Ficha);
            }
        }
        /* O jogador 1 joga em turnos impares */
        else{
            IniciarTurnoDoJogador(&jogador1);
            ExibirDadosDeJogador(jogador1);
            /* Verifica se o tabuleiro não está lotado */
            if(existeJogadaValida(&jogo)){
                Ficha Ficha = SelecionarFicha(&jogador1);
                verificarJogada(&jogo, Ficha);
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

    vitoriaJog1 = 0;
    vitoriaJog2 = 0;
    jogoContinua = 1;
    while(jogoContinua){
        TurnoGlobal++;
        LimparTerminal();
        ExibirJogo(&jogo);

        /* Bot joga em turnos pares */
        if(TurnoGlobal%2 == 0){
            IniciarTurnoDoJogador(&Jogador2Bot);
            ExibirDadosDeJogador(Jogador2Bot);
            esperar(1500);
            /* Verifica se o tabuleiro não está lotado */
            if(existeJogadaValida(&jogo)){
                BotJogada(&jogo, &Jogador2Bot);
            }
        }
        /* Jogador joga em turnos impares */
        else{
            IniciarTurnoDoJogador(&jogador1);
            ExibirDadosDeJogador(jogador1);
            /* Verifica se o tabuleiro não está lotado */
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

    vitoriaJog1 = 0;
    vitoriaJog2 = 0;
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
            /* Verifica se o tabuleiro não está lotado */
            if(existeJogadaValida(&jogo)){
                BotJogada(&jogo, &Jogador2Bot);
            }
        }
        else{
            IniciarTurnoDoJogador(&jogador1Bot);
            ExibirDadosDeJogador(jogador1Bot);
            esperar(2000);
            /* Verifica se o tabuleiro não está lotado */
            if(existeJogadaValida(&jogo)){
                BotJogada(&jogo, &jogador1Bot);
            }
        }
    }
}

// ====== Hall da fama

void inicializarHallDaFama(){
    /* Abre em leitura */
    FILE *fp = fopen("hall_da_fama", "r");
    /* Se não existir, cria-se o arquivo */
    if(fp == NULL){
        FILE * createfp = fopen("hall_da_fama", "w");
        if(createfp == NULL){
            printf("Erro: Falha ao criar arquivo Hall da Fama!\n");
            return;
        }
        fclose(createfp);
        return;
    }
    fclose(fp);
}

void limparHallDaFama(){
    /* Basta abrir um arquivo em modo de escrita para sobrescrever ele */
    FILE *fp = fopen("hall_da_fama", "w");
    if(fp == NULL){
        printf("Erro: Falha ao limpar arquivo!\n");
        return;
    }
    fclose(fp);
}

int lerHallDaFama(jogadorHall *jogadores, int max){
    /* Abre arquivo em leitura */
    FILE *fp = fopen("hall_da_fama", "rb");
    /* Verifica se encontrou */
    if(fp == NULL){
        return 0;
    }
    /* Pega o total de elementos dentro do arquivo, enquanto os lê*/
    int total = fread(jogadores, sizeof(jogadorHall), max, fp);

    /* Fecha arquivo */
    fclose(fp);
    return total;
}

void adicionarAoHall(jogadorHall jogador){
    if(jogador.pontuacao < 4){
        printf("Erro: Jogador nao pode entrar no hall com pontuacao menor que 4!\n");
        return;   
    }
    /* Impede o bot de entrar no ranking */
    if(strcmp(jogador.nome, "bot1") == 0 || strcmp(jogador.nome, "bot2") == 0){
        return;
    }

    /* Inicializa os elementos do vetor */
    jogadorHall jogadores[4];
    memset(jogadores, 0, sizeof(jogadores));

    int total = lerHallDaFama(jogadores, 3);

    /* Abre arquivo em escrita binária */
    FILE *fp = fopen("hall_da_fama", "wb");
    if(fp == NULL){
        printf("Erro: Falha ao escrever arquivo!\n");
        return;
    }

    /* Substitui jogador de mesmo nome se pontuação atual for superior */
    int substituiu = 0;
    for(int i = 0; i < total; i++){
        if(strcmp(jogador.nome, jogadores[i].nome) == 0){
            if(jogador.pontuacao < jogadores[i].pontuacao){
                jogadores[i] = jogador;
            }
            substituiu = 1;
            break;
        }
    }

    /* Adiciona jogador a lista se não houver substituição */
    if(!substituiu){
        /* Evita buffer overflow */
        if(total < 4){
            jogadores[total++] = jogador;
        }
    }

    /* Faz a ordenação */
    for(int i = 0; i < total; i++){
        for(int j = 0; j < total; j++){
            if(jogadores[j].pontuacao > jogadores[i].pontuacao){
                jogadorHall temp = jogadores[i];
                jogadores[i] = jogadores[j];
                jogadores[j] = temp;
            }
        }
    }

    /* Sobrescreve com os novos dados */
    fwrite(jogadores, sizeof(jogadorHall), total > 3 ? 3: total , fp);

    fclose(fp);
}

void exibirHallDaFama(){
    jogadorHall jogadores[3];

    int total = lerHallDaFama(jogadores, 3);

    /* Se nenhum jogador encontrado */
    if(total == 0){
        printf("=================\n");
        printf("Nao ha vitoriosos\n");
        printf("=================\n");
        return;
    }

    /* Para cada jogador exibe seu nome e pontuação */
    for(int i = 0; i < total; i++){
        printf("-> %i lugar\n", i + 1);
        printf("Nome do jogador: %s\n", jogadores[i].nome);
        printf("Pontuacao: %i\n", jogadores[i].pontuacao);
        printf("------------\n");
    }

    /* Saída */
    char c;
    printf("Digite algo para sair:");
    getchar();
    scanf("%c", &c);

    LimparTerminal();
}

void HallDaFama(){
    exibirHallDaFama();
}

// ====== Função principal

void Lig4(){
    int escolha = 0;
    LimparTerminal();

    while(1){
        /* Menu principal */
        printf("modos de jogo:\n\n");
        printf("1 - jogador x jogador\n");
        printf("2 - jogador x maquina\n");
        printf("3 - maquina x maquina\n\n");

        printf("digite 0 para volta.\n");
        
        /* Mantem o looping até uma entrada valida */
        printf("esolha: ");
        if(scanf("%d", &escolha) == 0){
            LimparTerminal();
            printf("\nDigite uma das alternativas!\n");
            while (getchar() != '\n');
            continue;
        }
        
        /* Opções de jogo */
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