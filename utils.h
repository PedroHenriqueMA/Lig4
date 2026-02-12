#ifndef UTILS_H
#define UTILS_H

/* ====== UTILIDADES DIVERSAS ====== */
// Função para limpar o terminal
void LimparTerminal();

// Faz o codigo aguardar um tempo antes de continuar a execução, recebe o tempo em milisegundos
void esperar(int milisegundos);


/* ====== ESTRUTURAS DE DADOS ====== */
typedef struct{
    char nome[20];
    int turno;
    int QtdFichaNormal;
    int QtdFichaExplosiva;
    int QtdFichaPortal;
    int precedencia;
} jogador;

typedef struct {
    char nome[30];
    int pontuacao;
} jogadorHall;

typedef struct{
    jogador dono;
    char tipo[20];
} Ficha;

typedef struct{
    int vazio;
    Ficha ficha;
}Casa;

typedef struct{
    Casa tabuleiro[6][7];
} Tabuleiro6x7;


/* ====== FICHAS ESPECIAIS ========*/
// Apaga uma ficha e a ficha abaixo dela caso tenham donos distintos
void Teleportar(Tabuleiro6x7 *jogo, int row, int col);

// Elimina a vizinhança 8 de uma ficha e aplica recursividade
void explodirBomba(Tabuleiro6x7 *jogo, int row, int col);

// Aplica a explodir bomba em uma ficha especifica e depois aplica gravidade em todo o tabuleiro
void GerarExplosao(Tabuleiro6x7 *jogo, int row, int col, Ficha gatilho);

/* ====== INICIALIZAÇÃO DO JOGO ====== */
// Função para gerar uma matriz de fichas nulas
Tabuleiro6x7 MontarTabuleiro(int linhas, int colunas);

// Gera uma nova matriz 6x7 nula com "MontarTabuleiro" e depois a exibe na tela
Tabuleiro6x7 MontarJogo();


/* ====== EXIBIÇÃO DO JOGO ====== */
// Função que imprime o tabuleiro na tela
void ExibirJogo(Tabuleiro6x7 *jogo);


/* ====== LÓGICA DE JOGADAS ====== */
// Limpa uma casa, definindo a ficha como nula e vazio como 1
void limparCasa(Tabuleiro6x7 *jogo, int row, int col);

// Preenche uma casa com uma ficha e define vazio como 0
void preencherCasa(Tabuleiro6x7 *jogo, int row, int col, Ficha ficha);

// Posiciona uma ficha na posição livre mais alta de uma coluna
void posicionarFicha(Tabuleiro6x7 *jogo, int col, Ficha ficha);

// Verifica se a coluna selecionada é valida antes de posicionar a ficha
void verificarJogada(Tabuleiro6x7 *jogo, Ficha ficha);

// Verifica se ainda existe ao menos uma jogada válida
int existeJogadaValida(Tabuleiro6x7 *jogo);


/* ====== GRAVIDADE E QUEDA DAS FICHAS ====== */
// Aplica gravidade em uma ficha individual, movendo ela para baixo
void AplicarGravidadeFicha(Tabuleiro6x7 *jogo, int col);

// Verifica se há um "buraco" (casa vazia com casa preenchida abaixo) em uma coluna
int TemBuracoNaColuna(Tabuleiro6x7 *jogo, int col);

// Aplica gravidade em todas as fichas de uma coluna, corrigindo buracos 
void aplicarGravidadeColuna(Tabuleiro6x7 *jogo, int col);


/* ====== VERIFICAÇÃO DE VITÓRIA ====== */
// Verifica se a peça gera condição de vitória (4 em linha)
int verificaVitoria(Tabuleiro6x7 *jogo, Ficha ficha, int row, int col);

// Finaliza o jogo, exibe vencedor e adiciona ao hall da fama
void vitoria(Tabuleiro6x7 *jogo, jogador vencedor);


/* ====== GERENCIAMENTO DE JOGADORES ====== */
// Atualiza os dados de um jogador para o padrão inicial, solicita nome ao usuário
void CriarJogador(jogador *player, int precedencia);

// Imprime na tela os dados do jogador (turno, quantidade de fichas, nome...)
void ExibirDadosDeJogador(jogador player);

// Incrementa o turno e gera fichas especiais quando necessário
void IniciarTurnoDoJogador(jogador *player);

// Gera uma ficha do tipo selecionado pelo jogador
Ficha SelecionarFicha(jogador *player);


/* ====== GERENCIAMENTO DE BOTS ====== */
// Cria um bot com configurações iniciais
void CriarBot(jogador *bot, int precedencia);

// Seleciona aleatoriamente uma ficha para o bot (respeitando as regras)
Ficha BotSelecionarFicha(jogador *bot);

// Gera uma jogada válida para um bot
void BotJogada(Tabuleiro6x7 *jogo, jogador *bot);


/* ====== MODOS DE JOGO ====== */
// Modo de jogo: jogador vs jogador
void PrincipalJxJ();

// Modo de jogo: jogador vs máquina
void principalJxBot();

// Modo de jogo: máquina vs máquina
void principalBotxBot();

// Função principal que exibe menu e gera o jogo
void Lig4();


/* ====== HALL DA FAMA ====== */
// Garante a existência do arquivo hall_da_fama
void inicializarHallDaFama();

// Limpa tudo que estiver escrito no arquivo hall_da_fama
void limparHallDaFama();

// Lê o Hall da fama e retorna o total de jogadores lidos
int lerHallDaFama(jogadorHall *jogadores, int max);

// Busca os jogadores no arquivo e realiza comparação com o jogador passado
void adicionarAoHall(jogadorHall jogador);

// Mostra no terminal os jogadores dentro do arquivo
void exibirHallDaFama();

// Inicializa e exibe o hall da fama
void HallDaFama();

#endif
