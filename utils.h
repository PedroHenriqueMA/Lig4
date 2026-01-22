#ifndef UTILS_H
#define UTILS_H

// Declaração da função para limpar o terminal
void LimparTerminal();

typedef struct{
    char nome[20];
    int turno;
    int QtdFichaNormal;
    int QtdFichaExplosiva;
    int QtdFichaPortal;
    int precedencia;
} jogador;

typedef struct{
    char dono[20];
    char tipo[20];
} Ficha;

typedef struct{
    int vazio;
    Ficha ficha;
}Casa;

typedef struct{
    Casa tabuleiro[6][7];
} Tabuleiro6x7;

// Função para gerar uma matriz de fichas nulas com um tamanho indefinido
Tabuleiro6x7 MontarTabuleiro(int linhas, int colunas);

// Função que imprime o tabuleiro na tela
void CarregarJogo(Tabuleiro6x7 jogo);

// Gera uma nova matriz 6x7 nula com "MontarTabuleiro" e depois imprime na tela com "CarregarJogo"
Tabuleiro6x7 MontarJogo();

// Verifica se a peça gera condição de vitória
void verificaVitoria(Tabuleiro6x7 *jogo, Ficha ficha, int row, int col);

// Aplica gravidade em uma coluna, movendo as fichas para baixo
void aplicarGravidadeColuna(Tabuleiro6x7 *jogo, int col);

// Limpa uma casa, definindo a ficha como nula e vazio como 1
void limparCasa(Tabuleiro6x7 *jogo, int row, int col);

// Preenche uma casa com uma ficha, setando vazio como 0
void preencherCasa(Tabuleiro6x7 *jogo, int row, int col, Ficha ficha);

// Posiciona uma ficha na possição livre mais baixa de uma coluna
void posicionarFicha(Tabuleiro6x7 *jogo, int col, Ficha ficha);

// Verifica se a coluna selecionada é valida antes de posicionar a ficha
void verificarJogada(Tabuleiro6x7 *jogo, Ficha ficha);

// Verifica se ainda existe ao menos uma jogada valida
int existeJogadaValida(Tabuleiro6x7 *jogo);

Ficha SelecionarFicha(jogador *player);

void CriarJogador(jogador *player, int precedencia);

void ExibirDadosDeJogador(jogador player);

void IniciarTurnoDoJogador(jogador *player);

void PrincipalJxJ();
#endif