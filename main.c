#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <windows.h>

void menu();


// estrutura do tabuleiro
typedef struct {
    char tabuleiro[3][3];
} Tabuleiro;


// estrutura das vitorias
typedef struct {
    char nomeJogadorX[50];
    char nomeJogadorO[50];
    int vitoriasX;
    int vitoriasO;
} Pontuacao;


// inicializar o tabuleiro

void inicializarTabuleiro(Tabuleiro *tab) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            tab->tabuleiro[i][j] = '-';
        }
    }
}


// printar o tabuleiro
void exibirtabuleiro(Tabuleiro *tab){

    system ("cls");
    for(int coluna =0; coluna <3; coluna++){
        for(int linha=0; linha <3; linha++){
           printf("%c ", tab->tabuleiro[coluna][linha]);
    }

    printf("\n");
 }

}


// função para fazer a jogada

void fazerJogada(Tabuleiro *tab, char jogador) {
    int linha, coluna;
    printf("Jogador %c, escolha uma linha (1-3) e uma coluna (1-3): ", jogador);
    scanf("%d %d", &linha, &coluna);
    linha--;
    coluna--;
    if(linha >= 0 && linha < 3 && coluna >= 0 && coluna < 3 && tab->tabuleiro[linha][coluna] == '-') {
        tab->tabuleiro[linha][coluna] = jogador;
    } else {
        printf("Jogada inválida. Tente novamente.\n");
        fazerJogada(tab, jogador);
    }
}


//funcão para encontrar o vencedor

int verificarVencedor(Tabuleiro *tab) {
    // Verificar linhas
    for(int i = 0; i < 3; i++) {
        if(tab->tabuleiro[i][0] == tab->tabuleiro[i][1] && tab->tabuleiro[i][1] == tab->tabuleiro[i][2] && tab->tabuleiro[i][0] != '-') {
            return 1; // Vencedor encontrado
        }
    }

    // Verificar colunas
    for(int j = 0; j < 3; j++) {
        if(tab->tabuleiro[0][j] == tab->tabuleiro[1][j] && tab->tabuleiro[1][j] == tab->tabuleiro[2][j] && tab->tabuleiro[0][j] != '-') {
            return 1; // Vencedor encontrado
        }
    }

    // Verificar diagonais
    if((tab->tabuleiro[0][0] == tab->tabuleiro[1][1] && tab->tabuleiro[1][1] == tab->tabuleiro[2][2] && tab->tabuleiro[0][0] != '-') ||
       (tab->tabuleiro[0][2] == tab->tabuleiro[1][1] && tab->tabuleiro[1][1] == tab->tabuleiro[2][0] && tab->tabuleiro[0][2] != '-')) {
        return 1; // Vencedor encontrado
    }

    // Se não houver vencedor
    return 0;
}


// funcão para exibir as pontuações(utlizando arquivos)

void exibirPontuacoes() {

    system("cls");
    FILE *arquivo;
    Pontuacao pontuacao;
    arquivo = fopen("pontuacoes.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de pontuações.\n");
        return;
    }
    fread(&pontuacao, sizeof(Pontuacao), 1, arquivo);
    fclose(arquivo);
    printf("Pontuações:\n");
    printf("Jogador X: %d\n", pontuacao.vitoriasX);
    printf("Jogador O: %d\n", pontuacao.vitoriasO);

}


// funcão para realizar as atualiza��es dos pontos(utlilizando arquivos)

void atualizarPontuacoes(char *nomeJogadorX, char *nomeJogadorO, int vitoriasX, int vitoriasO) {
    FILE *arquivo;
    Pontuacao pontuacao = {vitoriasX, vitoriasO};
    arquivo = fopen("pontuacoes.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de pontuações.\n");
        return;
    }
    fwrite(&pontuacao, sizeof(Pontuacao), 1, arquivo);
    fclose(arquivo);
}


// função para abrir o tutorial

void comojogar(){


    char voltar;

    system ("cls");

    while (voltar != 'f' || voltar != 'F'){

    printf ("\n** COMO JOGAR **\n");

    printf ("\nEscolha a linha e a coluna que deseja colocar sua letra\n");
    printf ("\nAVISO: Após escolher a linha, aperte Enter ANTES de escolher a coluna\n");
    printf ("\nExemplo: linha 1 e coluna 3 --> 1 Enter 3 Enter Enter\n\n");

    printf("\n\n*Aperte a tecla 'f' para retornar ao menu*\n\n");
    fflush(stdin);
    scanf("%c", &voltar);

    switch(voltar){
        case 'F':
        case 'f' :
        return;
            break;

    }
    system ("cls");

    fflush(stdin);
    getchar();
    }
}


// função para começar o jogo

void jogar(Pontuacao *pontuacao) {
    Tabuleiro tab;
    inicializarTabuleiro(&tab);
    char jogadorAtual = 'X';
    int jogadas = 0;

    while(1) {
        exibirtabuleiro(&tab);
        char opcao;

        fazerJogada(&tab, jogadorAtual);
        jogadas++;

        if(verificarVencedor(&tab) || jogadas == 9) {
            exibirtabuleiro(&tab);


            if(verificarVencedor(&tab)) {
                printf("\nO jogador %s venceu!\n", (jogadorAtual == 'X') ? pontuacao->nomeJogadorX : pontuacao->nomeJogadorO);
                if(jogadorAtual == 'X') {
                    pontuacao->vitoriasX++;
                } else {
                    pontuacao->vitoriasO++;
                }
            } else {
                printf("\nO jogo terminou em empate!\n");
            }
             atualizarPontuacoes(pontuacao->nomeJogadorX, pontuacao->nomeJogadorO, pontuacao->vitoriasX, pontuacao->vitoriasO);

            printf("Pontuações:\n");
            printf("Jogador %s: %d\n", pontuacao->nomeJogadorX, pontuacao->vitoriasX);
            printf("Jogador %s: %d\n", pontuacao->nomeJogadorO, pontuacao->vitoriasO);

            printf("Selecione 'j' para jogar novamente ou 'f' para voltar ao menu\n");
            getchar();
            scanf("%c", &opcao);


            if(opcao == 'j' || opcao =='J'){
                jogar(pontuacao);

            }
             else if(opcao == 'f' || opcao =='F'){
                menu(pontuacao);
                }
            else{
                printf("Operação invalida!");
            }
                break;
        }

        jogadorAtual = (jogadorAtual == 'X') ? 'O' : 'X';

        fflush(stdin);
        getchar();
    }
}


// função para abrir o menu

void menu(Pontuacao *pontuacao)
{
    int opcao=0;

    while(opcao!=3)
    {
        system("cls");


printf ("\n\n** JOGO DA VELHA **\n\n");

printf ("1) Jogar\n");
printf ("2) Como jogar\n");
printf ("3) Sair\n");

printf ("\n\nEscolha uma das opções acima: ");
scanf("%d", &opcao);

    switch (opcao)
        {
        case 1:
            system ("cls");
             printf("Digite o nome do Jogador X: ");
                scanf("%s", pontuacao->nomeJogadorX);
                printf("Digite o nome do Jogador O: ");
                scanf("%s", pontuacao->nomeJogadorO);
            jogar(pontuacao);
            return ;
        case 2:
            comojogar();
            break;

        case 3:
            system ("cls");
            printf ("Obrigado por jogar!\n");
            break;

        default:
            printf ("\n\nOpção Inválida\n");
            usleep(600000);
        }

    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    Pontuacao pontuacao = {0, 0};

    menu(&pontuacao);

    return 0;
}

