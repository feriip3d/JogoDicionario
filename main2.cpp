#include <stdio.h>
#include <conio2.h>
#include <stdlib.h>
#include <ctype.h>
#include <String.h>
#include <Windows.h>

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
void putsxy(const char *Texto, int Coluna, int Linha);
void ExibeMenu(void);
int Menu(void);
int Nav(int &pos);
void BotaoSelecionado(const char *Botao, int Coluna, int Linha, int pos);
void Executa(FILE *Arquivo);
void DeletaLinha(int x, int y);

// Funções do menu Usuarios
void CadastrarPessoa(FILE *B_Arquivo);
int BuscaExaustivaPessoa(FILE *B_Arquivo, TpPessoa PBuscar);
void AlterarCadPessoa(FILE *B_Arquivo);
void ConsultarCadPessoa(FILE *B_Arquivo);
void SelecionaAlteraPessoa(int opcao, TpPessoa &Reg);
void RelatorioPessoa(FILE *B_Arquivo);


int main(void)
{
	FILE *Arquivo;
	textcolor(cor_texto);
	textbackground(cor_fundo);
	clrscr();
	
	Executa(Arquivo);
	getch();
	
	return 0;
}

// Executa uma ação de acordo com o valor retornado pelo Menu
void Executa(FILE *Arquivo)
{
	int op=0;
	do
	{
		{
			switch(op)
			{
				case 11:
					CadastrarPessoa(Arquivo);
					break;
				case 12:
					AlterarCadPessoa(Arquivo);
					break;
				case 13:
					ConsultarCadPessoa(Arquivo);
				case 15:
					RelatorioPessoa(Arquivo);
			}
		}
		op=Menu();
	}while(op != 33);
}

/* Exibe uma string no local especificado */
void putsxy(const char *Texto, int Coluna, int Linha)
{
	gotoxy(Coluna, Linha);
	puts(Texto);
}

void getsxy(char *PonteiroString, int Coluna, int Linha)
{
	gotoxy(Coluna, Linha);
	gets(PonteiroString);
}

int Menu(void)
{
	int pos = 11;
	clrscr();
	DesenharTela(1,1,80,25,"MENU",1);
	do
	{
		switch(pos)
		{
			// Usuarios
			case 11:
				BotaoSelecionado("CADASTRAR",9,8,pos);
				break;
			case 12:
				BotaoSelecionado("ALTERAR",9,9,pos);
				break;
			case 13:
				BotaoSelecionado("CONSULTAR",9,10,pos);
				break;
			case 14:
				BotaoSelecionado("ORDENAR(?)",9,11,pos);
				break;
			case 15:
				BotaoSelecionado("RELATORIO",9,12,pos);
				break;
			
			// Dicionario
			case 21:
				BotaoSelecionado("CADASTRAR",34,8,pos);
				break;
			case 22:
				BotaoSelecionado("ALTERAR",34,9,pos);
				break;
			case 23:
				BotaoSelecionado("CONSULTAR",34,10,pos);
				break;
			case 24:
				BotaoSelecionado("ORDENAR(?)",34,11,pos);
				break;
			case 25:
				BotaoSelecionado("RELATORIO",34,12,pos);
				break;
			case 26:
				BotaoSelecionado("TRADUTOR",34,13,pos);
				break;
				
			// Jogo
			case 31:
				BotaoSelecionado("JOGO DA FORCA",60,8,pos);
				break;
			case 32:
				BotaoSelecionado("RANKING",60,9,pos);
				break;
			case 33:
				BotaoSelecionado("SAIR",70,24,pos);
				
		}
	}while(Nav(pos) > 0);
	
	return pos;
}

int Nav(int &local)
{
	char tecla;
	tecla = getch();
	
	if(tecla == -32){
		tecla = getch();
		switch(tecla){
			case 80: //baixo
				local++;break;
			case 72: //cima
				local--;break;
			case 77: //direita
				local+=10;break;
			case 75: // esquerda
				local-=10;break;
					
		}	
	}
	
	if(tecla == 13)
		return -1;
	
	if(local == 10)
		local+=5;
	if(local == 16)
		local-=5;
	if(local < 10)
		local = 33;
	if(local == 34)
		local = 31;
	if(local > 34)
		local = 11;
	if(local == 20)
		local = 26;
	if(local == 27)
		local = 21;
	if(local == 30)
		local = 33;
		
	return 1;
}

void BotaoSelecionado(const char *Botao, int Coluna, int Linha, int pos)
{
	ExibeMenu();
	textcolor(cor_fundo);
	textbackground(cor_texto);
	putsxy(Botao,Coluna,Linha);
	if(pos > 20 && pos < 29)
		putsxy("DICIONARIO",34,6);
	else if (pos < 20)
		putsxy("USUARIOS",9,6);
	else
		putsxy("JOGAR",64,6);
	textcolor(cor_texto);
	textbackground(cor_fundo);
}

/* Desenha as bordas da janela, e suas divisões caso seja
	especificado um valor especifico no campo TJanela */
void DesenharTela(int CInicial, int LInicial, int CFinal, int LFinal, const char *Titulo, int TJanela)
{
	int i, j;
		
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

void CadastrarPessoa(FILE *B_Arquivo)
{
	TpPessoa RegPessoa;
	int CadExiste;
	char cbuffer[30];
	B_Arquivo = fopen("Pessoa.dat","ab+");
	clrscr();
	/* Trocar essa area por uma tela utilizando conio2 */
	DesenharTela(1,1,80,25,"CADASTRAR USUARIO",0);
	putsxy("Login: ",3,5);
	putsxy("Nome: ",3,7);
	putsxy("Senha: ",3,9);
	
	fflush(stdin);
	getsxy(RegPessoa.Login,10,5);
	CadExiste = BuscaExaustivaPessoa(B_Arquivo, RegPessoa);
	while(strcmp(RegPessoa.Login,"\0") == 0 || CadExiste >= 0)
	{
		fflush(stdin);
		getsxy(RegPessoa.Login,10,5);
		CadExiste = BuscaExaustivaPessoa(B_Arquivo, RegPessoa);
	}
		
	fflush(stdin);
	getsxy(RegPessoa.Nome,9,7);
	while(strcmp(RegPessoa.Nome,"\0") == 0)
	{
		fflush(stdin);
		getsxy(RegPessoa.Nome,9,7);
	}
	
	fflush(stdin);
	getsxy(cbuffer,10,9);
	RegPessoa.Senha = atoi(cbuffer);
	while(RegPessoa.Senha <= 0)
	{
		fflush(stdin);
		getsxy(cbuffer,10,9);
		RegPessoa.Senha = atoi(cbuffer);
	}
	
	RegPessoa.Status = 0;
	RegPessoa.TotPontos = 0;
	
	SYSTEMTIME tempo;
	GetSystemTime(&tempo);
	
	RegPessoa.Data.Dia = tempo.wDay;
	RegPessoa.Data.Mes = tempo.wMonth;
	RegPessoa.Data.Ano = tempo.wYear;

	fwrite(&RegPessoa, sizeof(TpPessoa), 1, B_Arquivo);
	fclose(B_Arquivo);
}

void RelatorioPessoa(FILE *B_Arquivo)
{
	TpPessoa Reg;
	int linha=5, i;
	
	clrscr();
	B_Arquivo = fopen("Pessoa.dat","rb");
	
	DesenharTela(1,1,80,25,"RELATORIO", 0);
	putsxy("Login",3,4);
	putsxy("Nome",13,4);
	putsxy("Pontos",53,4);
	putsxy("Data de Cadastro",64,4);
	
	fread(&Reg, sizeof(TpPessoa), 1, B_Arquivo);
	while(!feof(B_Arquivo))
	{
		putsxy("Pressione ENTER para continuar.",3,24);
		if (linha == 23)
		{
			getch();
			for(i=5; i<linha; i++)
				DeletaLinha(2,i);
			linha = 5;
		}
		
		putsxy(Reg.Login,3,linha);
		putsxy(Reg.Nome,13,linha);
		gotoxy(53,linha);
		printf("%d", Reg.TotPontos);
		gotoxy(64,linha);
		printf("%d/%d/%d", Reg.Data.Dia, Reg.Data.Mes, Reg.Data.Ano);
				
		linha++;
		fread(&Reg, sizeof(TpPessoa), 1, B_Arquivo);
	}
	
	getch();
}

void AlterarCadPessoa(FILE *B_Arquivo)
{
	int pos, i;
	TpPessoa Reg;
	char senha_bf[50], op;
	B_Arquivo = fopen("Pessoa.dat","rb+");
	
	clrscr();
	DesenharTela(1,1,80,25,"ALTERAR CADASTRO",0);
	
	putsxy("Digite o login do usuario a ser alterado: ",3,5);
	getsxy(Reg.Login,45,5);
	
	pos = BuscaExaustivaPessoa(B_Arquivo, Reg);
	if(BuscaExaustivaPessoa(B_Arquivo, Reg) >= 0)
	{
		putsxy("Pressione ENTER para prosseguir ou BACKSPACE para alterar.",3,24);
		rewind(B_Arquivo);
		fseek(B_Arquivo, pos, SEEK_SET);
		fread(&Reg, sizeof(TpPessoa),1, B_Arquivo);
		putsxy("Nome: ",5,7);
		putsxy(Reg.Nome,11,7);
		putsxy("Senha: ",5,9);
		sprintf(senha_bf,"%d",Reg.Senha);
		for(i=0; i<strlen(senha_bf); i++)
			putsxy("*",i+12,9);
				
		for(i=7; i<=9; i+=2)
		{
			gotoxy(3,i);
			printf("%c", 16);
			fflush(stdin);
			op = getch();
			if (op == 8)
				SelecionaAlteraPessoa(i, Reg);
			putsxy(" ",3,i);
		}
		
		fseek(B_Arquivo,pos,SEEK_SET);
		fwrite(&Reg, sizeof(TpPessoa), 1, B_Arquivo);
		putsxy("Alterações gravadas!",5,11);
		getch();
	}
	else
		putsxy("Usuario nao cadastrado!",5,7);
	fclose(B_Arquivo);
}

void DeletaLinha(int x, int y)
{
	for(; x<80; x++)
		putsxy(" ",x,y);
}

void SelecionaAlteraPessoa(int opcao, TpPessoa &Reg)
{
	if(opcao == 7)
	{
		fflush(stdin);
		DeletaLinha(11,7);
		getsxy(Reg.Nome,11,7);
		while(strcmp(Reg.Nome,"\0") == 0)
		{
			DeletaLinha(11,7);
			fflush(stdin);
			getsxy(Reg.Nome,11,7);
		}
	}
	
	if(opcao == 9)
	{
		char cbuffer[50];
		DeletaLinha(12,9);
		fflush(stdin);
		getsxy(cbuffer,12,9);
		Reg.Senha = atoi(cbuffer);
		while(Reg.Senha <= 0)
		{
			DeletaLinha(12,9);
			fflush(stdin);
			getsxy(cbuffer,12,9);
			Reg.Senha = atoi(cbuffer);
		}
	}
}

void ConsultarCadPessoa(FILE *B_Arquivo)
{
	int pos;
	TpPessoa Reg;
	B_Arquivo = fopen("Pessoa.dat","rb");
	
	clrscr();
	DesenharTela(1,1,80,25,"CONSULTAR CADASTRO",0);
	
	putsxy("Digite o login do usuario a ser consultado: ",3,5);
	getsxy(Reg.Login,47,5);
	pos = BuscaExaustivaPessoa(B_Arquivo, Reg);
	if(pos >= 0)
	{
		fseek(B_Arquivo,pos,SEEK_SET);
		fread(&Reg, sizeof(TpPessoa), 1, B_Arquivo);
		putsxy("Nome: ",5,7);
		putsxy(Reg.Nome,11,7);
		putsxy("Total de Pontos: ",5,9);
		gotoxy(22,9);
		printf("%d",Reg.TotPontos);
		putsxy("Data de Cadastro: ",5,11);
		gotoxy(23,11);
		printf("%d/%d/%d", Reg.Data.Dia, Reg.Data.Mes, Reg.Data.Ano);
	}
	else
		putsxy("Usuario nao cadastrado!",5,7);
		
	getch();
	fclose(B_Arquivo);
}

int BuscaExaustivaPessoa(FILE *B_Arquivo, TpPessoa PBuscar)
{
	TpPessoa Reg;
	rewind(B_Arquivo);
	
	fread(&Reg, sizeof(TpPessoa), 1, B_Arquivo);
	while(!feof(B_Arquivo) && stricmp(Reg.Login, PBuscar.Login) != 0)
		fread(&Reg, sizeof(TpPessoa), 1, B_Arquivo);
	
	if(!feof(B_Arquivo) && Reg.Status == 0)
		return ftell(B_Arquivo)-sizeof(TpPessoa);
	else
		return -1;
}
