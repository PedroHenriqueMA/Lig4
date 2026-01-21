#ifndef UTILS_H
#define UTILS_H

// Declaração da função para limpar o terminal
void LimparTerminal();

typedef struct{
    char tipo[20];
    char dono[20];
} Ficha;

typedef struct{
    int vazio;
    Ficha ficha;
}Casa;

typedef struct{
    Casa tabuleiro[7][7];
} Tabuleiro7x7;

// Função para gerar uma matriz de fichas nulas com um tamanho indefinido
Tabuleiro7x7 MontarTabuleiro(int coluna, int linhas);

// Função que imprime o tabuleiro na tela
void CarregarJogo(Tabuleiro7x7 jogo);

// Gera uma nova matriz 7x7 nula com "MontarTabuleiro" e depois imprime na tela com "CarregarJogo"
Tabuleiro7x7 MontarJogo();

#endif