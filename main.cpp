#include <stdio.h>
#include <conio2.h>
#include <stdlib.h>
#include <ctype.h>
#include <String.h>

#define cor_fundo BLACK
#define cor_texto WHITE

// Structs
struct TpPalavra
{
	char Port[30], Ing[30], Sign[100];
	int Pontuacao, Status;
};

struct TpData
{
	int Dia, Mes, Ano;
};

struct TpPessoa
{
	char Login[10], Nome[40];
	int TotPontos, Senha, Status;
	TpData Data;
};

/* Protótipos */
// Tela e Menu
void DesenharTela(int CInicial, int LInicial, int CFinal, int LFinal, const char *Titulo, int TJanela);
void putsxy(char *Texto, int Coluna, int Linha);
void ExibeMenu(void);


int main(void)
{
	textcolor(cor_texto);
	textbackground(cor_fundo);
	clrscr();
	DesenharTela(1,1,80,25,"MENU",1);
	ExibeMenu();
	getch();
	
	return 0;
}

/* Exibe uma string no local especificado */
void putsxy(const char *Texto, int Coluna, int Linha)
{
	gotoxy(Coluna, Linha);
	puts(Texto);
}

/* Desenha as bordas da janela, e suas divisões caso seja
	especificado um valor especifico no campo TJanela */
void DesenharTela(int CInicial, int LInicial, int CFinal, int LFinal, const char *Titulo, int TJanela)
{
	int i, j;
		
	// Desenha as bordas
	for(i=LInicial+1; i < LFinal; i++)
	{
		gotoxy(CInicial, i);
		printf("%c", 186);
		gotoxy(CFinal, i);
		printf("%c", 186);
	}
	
	for(i=CInicial+1; i < CFinal; i++)
	{
		gotoxy(i, LInicial);
		printf("%c", 205);
		gotoxy(i, LFinal);
		printf("%c", 205);
		
		// Espaço para Titulo da Janela
		gotoxy(i, LInicial+2);
		printf("%c", 205);
	}
	
	// Desenha os cantos da tela
	gotoxy(CInicial, LInicial);
	printf("%c", 201);
	gotoxy(CFinal, LInicial);
	printf("%c", 187);
	gotoxy(CInicial, LFinal);
	printf("%c", 200);
	gotoxy(CFinal, LFinal);
	printf("%c", 188);
	
	// Canto do titulo da janela
	gotoxy(CInicial, LInicial+2);
	printf("%c", 204);
	gotoxy(CFinal, LInicial+2);
	printf("%c", 185);
	
	// Se a janela for do tipo Menu, desenha suas divisoes
	if(TJanela == 1)
	{
		
		for(i=CInicial+1; i < CFinal; i++)
		{
			gotoxy(i,LInicial+4);
			printf("%c", 205);
			gotoxy(i,LInicial+6);
			printf("%c", 205);
		}
		
		for(i=(CFinal/3); i<(CFinal/3)*3; i+=i)
		{
			for(j=LInicial+5; j < LFinal; j++)
			{
 				gotoxy(i,j);
				printf("%c",186);
			}
		}
		
		// Canto das divisões da janela
		gotoxy(CInicial, LInicial+4);
		printf("%c", 204);
		gotoxy(CFinal, LInicial+4);
		printf("%c", 185);
		
		gotoxy((CFinal/3),LInicial+4);
		printf("%c", 203);
		gotoxy((CFinal/3),LFinal);
		printf("%c", 202);
		
		gotoxy((CFinal/3)*2,LInicial+4);
		printf("%c", 203);
		gotoxy((CFinal/3)*2,LFinal);
		printf("%c", 202);
	}
	putsxy(Titulo,37,2);
}

void ExibeMenu(void)
{	
	// Coluna com as opções relacionadas a usuarios
	putsxy("Usuarios",9,6);
	putsxy("Cadastrar",9,8);
	putsxy("Alterar",9,9);
	putsxy("Consultar",9,10);
	putsxy("Ordenar(?)",9,11);
	putsxy("Relatório",9,12);
	
	// Coluna com as opções relacionadas ao dicionário de palavras
	putsxy("Dicionario",34,6);
	putsxy("Cadastrar",34,8);
	putsxy("Alterar",34,9);
	putsxy("Consultar",34,10);
	putsxy("Ordenar(?)",34,11);
	putsxy("Relatório",34,12);
	putsxy("Tradutor",34,13);
	
	// Coluna relacionada ao jogo
	putsxy("Jogar",64,6);
	putsxy("Jogo da Forca",60,8);
	putsxy("Ranking",60,9);
	
	// Botão de sair
	putsxy("Sair",70,24);
}

/*void CadastraPessoa(FILE *B_Arquivo)
{
	TpPessoa RegPessoa;
	B_Arquivo = fopen("Pessoa.dat","ab+");
	clrscr();
	/* Trocar essa area por uma tela utilizando conio2 /
	printf("\n** Cadastro de Pessoas **\n");
	printf("\nDigite o login: ");
	gets(RegPessoa.Login);
	
}

int VerificaCadastroPessoa(FILE *B_Arquivo)
{
	
}*/
