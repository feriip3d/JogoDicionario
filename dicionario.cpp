#include <stdio.h>
#include <conio2.h>
#include <stdlib.h>
#include <ctype.h>
#include <String.h>
#include <Windows.h>
#include <time.h>

#define cor_fundo BLUE
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

struct PalavraSort
{
	char Palavra[30];
	char Login[10];
};

struct TpDicionario
{
	char port[30], ing[30], sig[100];
	int Pontos, Status;
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
void getsxy(char *PtrStr, int Coluna, int Linha);
void ExibeMenu(void);
int Menu(void);
int Nav(int &pos);
void BotaoSelecionado(const char *Botao, int Coluna, int Linha, int pos);
void Executa(FILE *Arquivo);
void DeletaLinha(int x, int y);
int TamanhoArquivo(FILE *PtrArq);
void strcat_char(char *Destino, char letra);
int TelaFim(int tipo);

// Funções do menu Usuarios
void ReescreverArquivoPessoa(FILE *B_Arquivo);
void CadastrarPessoa(FILE *B_Arquivo);
int BuscaExaustivaPessoa(FILE *B_Arquivo, TpPessoa PBuscar);
void AlterarCadPessoa(FILE *B_Arquivo);
void ConsultarCadPessoa(FILE *B_Arquivo);
void SelecionaAlteraPessoa(int opcao, TpPessoa &Reg);
void RelatorioPessoa(FILE *B_Arquivo);
void ExcluirPessoa(FILE *B_Arquivo);

// Funções do menu Dicionario
void IncluirPalavra(FILE *PtrArq);
int BuscaPalavra(FILE *PtrArq, TpDicionario PalavraB);
void AlteraPalavra(FILE *PtrArq);
void SelecionaAlteraPalavra(int opcao, TpDicionario &Reg);
void ConsultarPalavra(FILE *PtrArq);
void RelatorioPalavra(FILE *PtrArq);
void ExcluirPalavra(FILE *PtrArq);

// Jogo da Forca
void DesenhaForca(TpPessoa Usuario);
void Enforcar(int tentativa);

// Jogos
void JogoForca(void);


int main(void)
{
	srand(time(NULL));
	FILE *Arquivo;
	textcolor(cor_texto);
	textbackground(cor_fundo);
	ReescreverArquivoPessoa(Arquivo);
	clrscr();
	
	Executa(Arquivo);
	getch();
	
	return 0;
}

void ordenaPalavra(FILE *PtrArq){
	TpDicionario r, b, aux;
	int i=1, posA, posB;
	FILE *pos;
	PtrArq = fopen("dicionario.dat", "rb");
	pos = fopen("dicionario.dat", "rb");
	fread(&r, sizeof(TpDicionario),1,PtrArq);
	fread(&b, sizeof(TpDicionario),1,pos);
	while(!feof(PtrArq)-(sizeof(TpDicionario)*i)){
		rewind(pos);
		fread(&b, sizeof(TpDicionario),1,pos);
		while(!feof(pos)-(sizeof(TpDicionario)*i)){
			if(strcmp(r.port, b.port) < 0){
				posA = int(PtrArq);
				posB = int(pos);
				fseek(PtrArq, posA, 0);
				fwrite(&r, sizeof(TpDicionario), 1, PtrArq);
				fseek(pos, posB, 0);
				fwrite(&b, sizeof(TpDicionario), 1, pos);
			}
			fread(&r, sizeof(TpDicionario),1,pos);
		}
		i++;
	}
	fclose(PtrArq);
	fclose(pos);
}

// Dicionario
void IncluirPalavra(FILE *PtrArq){
	TpDicionario dicio;
	int pos;
	clrscr();
	PtrArq = fopen("dicionario.dat", "rb+");
	
	if(PtrArq == NULL){
		DesenharTela(1,1,80,25,"DICIONARIO",0);
		gotoxy(3,5);printf("Arquivo nao encontrado, deseja abrir? (S/N)");
		if(toupper(getche()) == 'S'){
			PtrArq = fopen("dicionario.dat", "ab+");
		}
	}
	else{
		fclose(PtrArq);
		fopen("dicionario.dat", "ab+");
	}
	clrscr();
	DesenharTela(1,1,80,25,"CADASTRAR PALAVRA",0);
	gotoxy(3,5);printf("Palavra em Portugues: ");
	gets(dicio.port);
	pos = BuscaPalavra(PtrArq, dicio);
	if(pos < 0)
	{
		while(strcmp(dicio.port, "\0") != 0){
			do{
				gotoxy(3,7);printf("Palavra em Ingles:                          ");
				gotoxy(22,7);gets(dicio.ing);
			}while(strcmp(dicio.ing, "\0") ==0);
			do{
				gotoxy(3,9);printf("Significado:                             ");
				gotoxy(16,9);gets(dicio.sig);
			}while(strcmp(dicio.sig, "\0") ==0);
			do{
				gotoxy(3,11);printf("Pontuacao:                             ");
				gotoxy(14,11);scanf("%d",&dicio.Pontos);
			}while(dicio.Pontos<= 0);
			dicio.Status = 0;
			fwrite(&dicio, sizeof(TpDicionario), 1, PtrArq);
			clrscr();
			DesenharTela(1,1,80,25,"CADASTRAR PALAVRA",0);
			gotoxy(3,5);printf("Palavra em Portugues: ");
			fflush(stdin);
			gets(dicio.port);
		}
	}
	else
	{
		putsxy("A palavra ja existe!",3,24);
		getch();
	}
	fclose(PtrArq);
}

// ???

int BuscaPalavra(FILE *PtrArq, TpDicionario PalavraB){
	TpDicionario Reg;
	rewind(PtrArq);
	
	Reg.Status = 0;
	fread(&Reg, sizeof(TpDicionario), 1, PtrArq);
	while((!feof(PtrArq) && stricmp(Reg.port, PalavraB.port) != 0) || Reg.Status == 1)
		fread(&Reg, sizeof(TpDicionario), 1, PtrArq);
			
	if(!feof(PtrArq))
		return ftell(PtrArq)-sizeof(TpDicionario);
	else
		return -1;
}


void AlteraPalavra(FILE *PtrArq)
{
	int pos, i;
	TpDicionario Reg;
	char op;
	PtrArq = fopen("dicionario.dat","rb+");
	
	clrscr();
	DesenharTela(1,1,80,25,"ALTERAR PALAVRA",0);
	
	putsxy("Digite a palavra a ser alterada (port.): ",3,5);
	fflush(stdin);
	getsxy(Reg.port,44,5);
	
	pos = BuscaPalavra(PtrArq, Reg);
	if(pos >= 0)
	{
		putsxy("Pressione ENTER para prosseguir ou BACKSPACE para alterar.",3,24);
		fseek(PtrArq, pos, SEEK_SET);
		fread(&Reg, sizeof(TpDicionario), 1, PtrArq);
		putsxy("Portugues: ",5,7);
		putsxy(Reg.port,16,7);
		putsxy("Ingles: ",5,9);
		putsxy(Reg.ing,13,9);
		putsxy("Significado: ",5,11);
		putsxy(Reg.sig,18,11);
		putsxy("Pontuacao: ",5,13);
		gotoxy(16,13);
		printf("%d",Reg.Pontos);
				
		for(i=7; i<=13; i+=2)
		{
			gotoxy(3,i);
			printf("%c", 16);
			fflush(stdin);
			op = getch();
			if (op == 8)
				SelecionaAlteraPalavra(i, Reg);
			putsxy(" ",3,i);
		}
		
		fseek(PtrArq, pos, SEEK_SET);
		fwrite(&Reg, sizeof(TpDicionario), 1, PtrArq);
	}
	else
	{
		putsxy("Palavra nao cadastrada!",5,7);
		getch();
	}
	fclose(PtrArq);
}


void JogoForca(void)
{
	TpPessoa User;
	TpDicionario Palavra;
	char letras_utilizadas[30]="", letra;
	FILE *PtrArq;
	int login_status=1, qt_palavras, i, j, sorteada=0, encontradas, tent_rest, l_valida,ok;
	
	//login_status=Login(User);
	
	while(login_status == 1)
	{
		PtrArq = fopen("dicionario.dat","rb");
		qt_palavras = TamanhoArquivo(PtrArq)/sizeof(TpDicionario);
		//SorteiaPalavra(sorteada, qt_palavras);
		
		
		clrscr();
		DesenhaForca(User);
		fseek(PtrArq, 0, SEEK_SET);
		fread(&Palavra, sizeof(TpDicionario), 1, PtrArq);
		for(i=0, j=5; i<strlen(Palavra.port); i++, j+=2)
			putsxy("_",j,18);
			
		putsxy("Significado: ",5,21);
		putsxy(Palavra.sig,5,22);
		
		encontradas=0; tent_rest=6;
		while(encontradas<strlen(Palavra.port) && tent_rest > 0)
		{
			putsxy(letras_utilizadas, 6,7);
			gotoxy(74,17);
			printf("%d",tent_rest);
			fflush(stdin);
			letra = toupper(getch());
			
			for(i=0, l_valida=0; i<strlen(letras_utilizadas); i++)
				if(letra == letras_utilizadas[i])
					l_valida=1;
					
			if(l_valida == 0)
			{
				for(i=0, j=5, ok=0; i<strlen(Palavra.port); i++, j+=2)
				{
					if(letra == toupper(Palavra.port[i]))
					{
						gotoxy(j,18);
						printf("%c", letra);
						encontradas++;
						ok=1;
					}
				}
				
				if(ok == 0)
				{
					Enforcar(tent_rest);
					tent_rest--;
				}
				strcat_char(letras_utilizadas,letra);
			}
		}
		
		if(tent_rest == 0)
			login_status = TelaFim(1);
		else
		{
			login_status = TelaFim(2);
			//registraPontos(Palavra, User, lang;
		}
		
		fclose(PtrArq);
	}
	getch();
}

/*void SorteiaPalavra(FILE *PtrArq, int QtdePalavras, TpDicionario Palavra)
{
	pos = (rand() % QtdePalavras);
	lang = (rand() % 2)+1;
	fread(&Palavra, sizeof(TpPessoa), 1, PtrArq);
	VerificaSorteio(Usuario,Palavra,lang);
	while(Palavra.Status == 1 || )
}

int VerificaSorteio(TpPessoa Usuario, TpDicionario Palavra, int lang)
{
	FILE *B_Arquivo;
	PalavraSort Sort_Anterior;
	B_Arquivo = fopen("Sorteados.dat","rb");
	
	fread(&Sort_Anterior,sizeof(PalavraSort),1,B_Arquivo);
	if(lang == 1)
	{
		while(!feof(B_Arquivo) && Palavra.Port != Sort_Anterior.Palavra && Usuario.Login != Sort_Anterior.Login)
		{
				
		}
	}
}

/*
void RegistrarPontos(TpDicionario Palavra, TpPessoa Usuario, int lang)
{
	FILE *PtrArq;
	PalavraSort Sorteado;
	
	TpPessoa.TotPontos += Palavra.Pontuacao;
	strcpy(Sorteado.Login,Usuario.Login);
	if(lang == 1)
		strcpy(Sorteado.Palavra,Palavra.Port);
	else
		strcpy(Sorteado.Palavra,Palavra.Ing);
		
	PtrArq = fopen("Pessoa.dat","rb+");
	fseek(PtrArq,BuscaExaustivaPessoa(PtrArq, Usuario),SEEK_SET);
	fwrite(&Usuario, sizeof(TpPessoa), 1, PtrArq);
	fclose(PtrArq);
	
	PtrArq = fopen("Sorteados","ab+");
	fwrite(PtrArq,sizeof(PalavraSort),1,PtrArq);
	fclose(PtrArq);
}
*/
int TelaFim(int tipo)
{
	int continuar;
	
	switch(tipo)
	{
		case 1:
			putsxy("\\",68,11);
			textbackground(RED);
			putsxy("              ",18,8);
			putsxy(" VOCE PERDEU! ",18,9);
			putsxy("              ",18,10);
			textbackground(cor_fundo);
			putsxy("Deseja jogar novamente?(s/n)",11,12);
			switch(toupper(getch()))
			{
				case 'S':
					continuar = 1;
				case 'N':
					continuar = 0;
			}
			
			break;
			
		case 2:
			textbackground(GREEN);
			putsxy("              ",18,8);
			putsxy(" VOCE GANHOU! ",18,9);
			putsxy("              ",18,10);
			textbackground(cor_fundo);
			putsxy("Deseja continuar?(s/n)",14,12);
			switch(toupper(getch()))
			{
				case 'S':
					continuar = 1;
				case 'N':
					continuar = 0;
			}
			break;
	}
	
	return continuar;
}

void Enforcar(int tentativa)
{
	switch(tentativa)
	{
		case 6:
			putsxy("O",67,8);
			break;
		case 5:
			gotoxy(67,9);
			printf("%c",179);
			break;
		case 4:
			putsxy("/",68,9);
			break;
		case 3:
			putsxy("\\",66,9);
			break;
		case 2:
			gotoxy(67,10);
			printf("%c", 179);
		case 1:
			putsxy("/",66,11);
			break;
	}
}

void strcat_char(char *Destino, char letra)
{
	Destino[strlen(Destino)] = letra;
	Destino[strlen(Destino)+1] = '\0';	
}

int BuscaPraFrase(FILE *PtrArq, char frase[400]){
	TpDicionario Reg;
	rewind(PtrArq);
	
	Reg.Status = 0;
	fread(&Reg, sizeof(TpDicionario), 1, PtrArq);
	while((!feof(PtrArq) && stricmp(Reg.port, frase) != 0))
		fread(&Reg, sizeof(TpDicionario), 1, PtrArq);
			
	if(!feof(PtrArq))
		return ftell(PtrArq)-sizeof(TpDicionario);
	else
		return -1;
}

void traduzFrase(FILE *PtrArq){
	TpDicionario dicio;
	char frase[400], palavra[400], concat[400];
	int tam = 0, pos, i, local;
	PtrArq =fopen("dicionario.dat", "rb");
	clrscr();
	DesenharTela(1,1,80,25, "Tradutor Palavra", 0);
	gotoxy(3,5);printf("Frase: ");
	fflush(stdin);gets(frase);
	local = strlen(frase);
	for(i =0; i<local; i++){
		if(frase[i] != ' '){
			palavra[tam] = frase[i];
			palavra[tam+1] = '\0';
			tam++;
			gotoxy(3,7);puts(palavra);
		}
		if(frase[i] == ' '){
			if(tam >0){
				pos =BuscaPraFrase(PtrArq, palavra);
				if(pos == -1){
					strcat(concat, palavra);
				}else{
					fseek(PtrArq, pos, 0);
					fread(&dicio, sizeof(TpDicionario), 1, PtrArq);
					strcat(concat, dicio.ing);
				}
				strcat(concat, " ");
				tam=0;
				palavra[0] = '\0';
			}
		}
	}
	if(tam > 0){
		pos =BuscaPraFrase(PtrArq, palavra);
		if(pos == -1){
			strcat(concat, palavra);
		}else{
			fseek(PtrArq, pos, 0);
			fread(&dicio, sizeof(TpDicionario), 1, PtrArq);
			strcat(concat, dicio.ing);
		}
		strcat(concat, " ");
		tam=0;
		palavra[0] = '\0';
	}
	putsxy(concat,3,7);gotoxy(3,7);printf("%c", 16);gotoxy(3,8);
	getch();
	fclose(PtrArq);
}

void traduzPalavra(FILE *PtrArq){
	TpDicionario r;
	char p[20];
	clrscr();
	DesenharTela(1,1,80,25, "Tradutor Palavra", 0);
	gotoxy(3,5);printf("Palavra: ");
	gets(p);
	
	fread(&r, sizeof(TpDicionario), 1, PtrArq);
	while(!feof(PtrArq)){
		if(stricmp(r.port, p) == 0){
			gotoxy(3,6);printf("Traducao: %s", r.ing);
			gotoxy(3,7);printf("Significado: %s", r.sig);
		}
		fread(&r, sizeof(TpDicionario), 1, PtrArq);
	}
	getch();
}

int escolheTradutor(int op, int escolha){

	putsxy("Palavra",4,5);
	putsxy("Frase", 4,6);
	
	if(op == 80){
		escolha = escolha + 10;
	}
	if(op ==  72){
		escolha = escolha -10;
	}
	if(escolha == 0){
		escolha = 20;
	}
	if(escolha == 30){
		escolha =10;
	}
	if(escolha == 10){
		gotoxy(3,5);printf("%c", 16);
	}
	if(escolha == 20){
		gotoxy(3,6);printf("%c", 16);
	}
return escolha;
}

void traduz(FILE *PtrArq){
	int op, escolha = 10;
	clrscr();
	DesenharTela(1,1,80,25, "Tradutor", 0);
	PtrArq = fopen("dicionario.dat", "rb");
	do{
		escolha = escolheTradutor(op, escolha);
		op = getch();
		putsxy(" ", 3,5);
		putsxy(" ", 3,6);
	}while(op != 13);
	if(escolha == 10){
		traduzPalavra(PtrArq);
	}
	if(escolha == 20){
		traduzFrase(PtrArq);
	}
	fclose(PtrArq);
}

void DesenhaForca(TpPessoa Usuario)
{
	int i;
	DesenharTela(1,1,80,25,"FORCA",2);
	gotoxy(75,5);
	printf("%c",187);
	for(i=6; i<15; i++)
	{
		gotoxy(75,i);
		printf("%c",186);
	}
	for(i=78; i>64; i--)
	{
		gotoxy(i, 15);
		printf("%c",205);
	}
	for(i=74; i>67; i--)
	{
		gotoxy(i,5);
		printf("%c", 205);
	}
	gotoxy(75,15);
	printf("%c", 202);
	gotoxy(67,5);
	printf("%c", 201);
	for(i=6; i<8; i++)
	{
		gotoxy(67,i);
		printf("|");
	}
	
	putsxy("Tentativas Rest.: ",57,17);
	putsxy("Pont. da Palavra: ",57,19);
	putsxy("Sua Pont. Total: ",57,21);
	putsxy("Usuario: ",5,5);
	putsxy("Usuario_PH",14,5);
}

int TamanhoArquivo(FILE *PtrArq)
{
	fseek(PtrArq,0,SEEK_END);
	return ftell(PtrArq);
}

int Login(TpPessoa &User)
{
	int pos, senha;
	FILE *PtrArq;
	
	PtrArq = fopen("Pessoa.dat","rb");
	
	clrscr();
	DesenharTela(1,1,80,25,"LOGIN",0);
	putsxy("Login: ",3,5);
	putsxy("Senha: ",3,7);
	
	getsxy(User.Login,10,5);
	pos = BuscaExaustivaPessoa(PtrArq, User);
	if(pos >= 0)
	{
		gotoxy(11,7);
		scanf("%d", &senha);
		
		if(senha == User.Senha)
		{
			fclose(PtrArq);
			return 1;
		}
		else
			putsxy("Senha incorreta!",3,9);
	}
	else
		putsxy("Usuario não encontrado!",3,9);
	
	fclose(PtrArq);
	return -1;
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
					break;
				case 15:
					RelatorioPessoa(Arquivo);
					break;
				case 16:
					ExcluirPessoa(Arquivo);
					break;
					
				case 21:
					IncluirPalavra(Arquivo);
					break;
				case 22:
					AlteraPalavra(Arquivo);
					break;
				case 23:
					ConsultarPalavra(Arquivo);
					break;
				case 24:
					ordenaPalavra(Arquivo);break;
				case 25:
					RelatorioPalavra(Arquivo);
					break;
				case 27:
					ExcluirPalavra(Arquivo);
					break;
				case 26:
					traduz(Arquivo);break;	
					
				case 31:
					JogoForca();
					break;
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

/* Lê uma string no local especificado */
void getsxy(char *PtrStr, int Coluna, int Linha)
{
	gotoxy(Coluna, Linha);
	gets(PtrStr);
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
			case 16:
				BotaoSelecionado("EXCLUIR",9,13,pos);
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
			case 27:
				BotaoSelecionado("EXCLUIR",34,14,pos);
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
		local+=6;
	if(local == 17)
		local-=6;
	if(local < 10)
		local = 33;
	if(local == 34)
		local = 31;
	if(local > 34)
		local = 11;
	if(local == 20)
		local = 27;
	if(local == 28)
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
	else if(TJanela == 2)
	{
		gotoxy(55,25);
		printf("%c",202);
		gotoxy(55,3);
		printf("%c",203);
		
		for(i=4; i<25; i++)
		{
			gotoxy(55,i);
			printf("%c",186);
		}
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
	putsxy("Relatorio",9,12);
	putsxy("Excluir",9,13);
	
	// Coluna com as opções relacionadas ao dicionário de palavras
	putsxy("Dicionario",34,6);
	putsxy("Cadastrar",34,8);
	putsxy("Alterar",34,9);
	putsxy("Consultar",34,10);
	putsxy("Ordenar(?)",34,11);
	putsxy("Relatorio",34,12);
	putsxy("Tradutor",34,13);
	putsxy("Excluir",34,14);
	
	// Coluna relacionada ao jogo
	putsxy("Jogar",64,6);
	putsxy("Jogo da Forca",60,8);
	putsxy("Ranking",60,9);
	
	// Botão de sair
	putsxy("Sair",70,24);
}

void ReescreverArquivoPessoa(FILE *B_Arquivo)
{
	FILE *Arq_Antigo;
	TpPessoa Reg;
	if(rename("Pessoa.dat","Pessoa.old") == 0)
	{
		clrscr();
		DesenharTela(1,1,80,20,"ATUALIZANDO",0);
		putsxy("Atualizando arquivos...",5,5);
		B_Arquivo = fopen("Pessoa.dat","wb");
		Arq_Antigo = fopen("Pessoa.old","rb");
		
		fread(&Reg, sizeof(TpPessoa), 1, Arq_Antigo);
		while(!feof(Arq_Antigo))
		{
			if(Reg.Status == 0)
				fwrite(&Reg, sizeof(TpPessoa), 1, B_Arquivo);
			fread(&Reg, sizeof(TpPessoa), 1, Arq_Antigo);
		}
		
		fclose(Arq_Antigo);
		fclose(B_Arquivo);
		
		remove("Pessoa.old");
	}
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
		DeletaLinha(10,5);
		fflush(stdin);
		getsxy(RegPessoa.Login,10,5);
		CadExiste = BuscaExaustivaPessoa(B_Arquivo, RegPessoa);
	}
		
	fflush(stdin);
	getsxy(RegPessoa.Nome,9,7);
	while(strcmp(RegPessoa.Nome,"\0") == 0)
	{
		DeletaLinha(9,7);
		fflush(stdin);
		getsxy(RegPessoa.Nome,9,7);
	}
	
	fflush(stdin);
	getsxy(cbuffer,10,9);
	RegPessoa.Senha = atoi(cbuffer);
	while(RegPessoa.Senha <= 0)
	{
		DeletaLinha(10,9);
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

void ExcluirPessoa(FILE *B_Arquivo)
{
	int pos, i;
	TpPessoa Reg;
	B_Arquivo = fopen("Pessoa.dat","rb+");
	
	clrscr();
	DesenharTela(1,1,80,25,"EXCLUIR CADASTRO",0);
	
	putsxy("Digite o login do usuario a ser excluido: ",3,5);
	fflush(stdin);
	getsxy(Reg.Login,45,5);
	
	pos = BuscaExaustivaPessoa(B_Arquivo, Reg);
	if(pos >= 0)
	{
		fseek(B_Arquivo, pos, SEEK_SET);
		fread(&Reg, sizeof(TpPessoa),1, B_Arquivo);
		putsxy("Nome: ",5,7);
		putsxy(Reg.Nome,11,7);
		putsxy("Total de Pontos: ",5,9);
		gotoxy(27,9);
		printf("%d", Reg.TotPontos);
		putsxy("Data de Cadastro: ",5,11);
		gotoxy(27,11);
		printf("%02d/%d/%d",Reg.Data.Dia, Reg.Data.Mes, Reg.Data.Ano);
		
		putsxy("Deseja remover esse cadastro? (s/n).",3,23);
		gotoxy(3,24);
		printf("%c: ", 16);
		if(tolower(getche()) == 's')
		{
			Reg.Status = 1;
			putsxy("Deletado!",5,13);
		}
		else
			putsxy("Cancelado!",5,13);
		
		fseek(B_Arquivo,pos,SEEK_SET);
		fwrite(&Reg, sizeof(TpPessoa), 1, B_Arquivo);
		getch();
	}
	else
		putsxy("Usuario nao cadastrado!",5,7);
	
	fclose(B_Arquivo);
}

void ExcluirPalavra(FILE *PtrArq)
{
	int pos, i;
	TpDicionario Reg;
	PtrArq = fopen("dicionario.dat","rb+");
	
	clrscr();
	DesenharTela(1,1,80,25,"EXCLUIR PALAVRA",0);
	
	putsxy("Digite a palavra a ser excluida (port): ",3,5);
	fflush(stdin);
	getsxy(Reg.port,43,5);
	
	pos = BuscaPalavra(PtrArq, Reg);
	if(pos >= 0)
	{
		fseek(PtrArq,pos,SEEK_SET);
		fread(&Reg, sizeof(TpDicionario), 1, PtrArq);
		putsxy("Portugues: ",5,7);
		putsxy(Reg.port,16,7);
		putsxy("Ingles: ",5,9);
		putsxy(Reg.ing,13,9);
		putsxy("Significado: ",5,11);
		putsxy(Reg.sig,18,11);
		putsxy("Pontuacao: ",5,13);
		gotoxy(16,13);
		printf("%d",Reg.Pontos);
		
		putsxy("Deseja remover essa palavra? (s/n).",3,23);
		gotoxy(3,24);
		printf("%c: ", 16);
		if(tolower(getche()) == 's')
		{
			Reg.Status = 1;
			putsxy("Deletada!",5,13);
		}
		else
			putsxy("Cancelado!",5,13);
		
		fseek(PtrArq,pos,SEEK_SET);
		fwrite(&Reg, sizeof(TpDicionario), 1, PtrArq);
		getch();
	}
	else
		putsxy("Palavra nao cadastrada!",5,7);
	
	fclose(PtrArq);
}

void RelatorioPalavra(FILE *PtrArq)
{
	TpDicionario Reg;
	int linha=5, i;
	
	clrscr();
	PtrArq = fopen("dicionario.dat","rb");
	
	DesenharTela(1,1,80,25,"RELATORIO", 0);
	putsxy("Portugues",3,4);
	putsxy("Ingles",23,4);
	putsxy("Pontos",43,4);
	putsxy("Significado",57,4);
	
	fread(&Reg, sizeof(TpDicionario), 1, PtrArq);
	while(!feof(PtrArq))
	{
		putsxy("Pressione ENTER para continuar.",3,24);
		if (linha == 23)
		{
			getch();
			for(i=5; i<linha; i++)
				DeletaLinha(2,i);
			linha = 5;
		}
		
		if(Reg.Status == 0)
		{
			putsxy(Reg.port,3,linha);
			putsxy(Reg.ing,23,linha);
			gotoxy(43,linha);
			printf("%d", Reg.Pontos);
			putsxy(Reg.sig,57,linha);
			linha++;
		}
		fread(&Reg, sizeof(TpDicionario), 1, PtrArq);
	}
	fclose(PtrArq);
	getch();
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
		
		if(Reg.Status == 0)
		{
			putsxy(Reg.Login,3,linha);
			putsxy(Reg.Nome,13,linha);
			gotoxy(53,linha);
			printf("%d", Reg.TotPontos);
			gotoxy(64,linha);
			printf("%02d/%d/%d", Reg.Data.Dia, Reg.Data.Mes, Reg.Data.Ano);
			linha++;
		}
		fread(&Reg, sizeof(TpPessoa), 1, B_Arquivo);
	}
	
	fclose(B_Arquivo);
	getch();
}

void AlterarCadPessoa(FILE *B_Arquivo)
{
	int pos, i, confirmar, bksenha;
	TpPessoa Reg;
	char senha_bf[50], op;
	B_Arquivo = fopen("Pessoa.dat","rb+");
	
	clrscr();
	DesenharTela(1,1,80,25,"ALTERAR CADASTRO",0);
	
	putsxy("Digite o login do usuario a ser alterado: ",3,5);
	fflush(stdin);
	getsxy(Reg.Login,45,5);
	
	pos = BuscaExaustivaPessoa(B_Arquivo, Reg);
	if(pos >= 0)
	{
		putsxy("Pressione ENTER para prosseguir ou BACKSPACE para alterar.",3,24);
		fseek(B_Arquivo, pos, SEEK_SET);
		fread(&Reg, sizeof(TpPessoa),1, B_Arquivo);
		putsxy("Nome: ",5,7);
		putsxy(Reg.Nome,11,7);
		putsxy("Senha: ",5,9);
		bksenha = Reg.Senha;
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
		putsxy("Digite a senha atual para confirmar: ",5,11);
		gotoxy(42,11);
		scanf("%d",&confirmar);
		
		DeletaLinha(5,11);
		if(bksenha == confirmar)
		{
			fseek(B_Arquivo,pos,SEEK_SET);
			fwrite(&Reg, sizeof(TpPessoa), 1, B_Arquivo);
			putsxy("Alterações gravadas!",5,11);
			getch();
		}
		else
		{
			putsxy("Senha Incorreta!",5,11);
			getch();
		}
	}
	else
	{
		putsxy("Usuario nao cadastrado!",5,7);
		getch();
	}
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
	
	else if(opcao == 9)
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

void ConsultarPalavra(FILE *PtrArq)
{
	int pos;
	TpDicionario Reg;
	PtrArq = fopen("dicionario.dat","rb");
	
	clrscr();
	DesenharTela(1,1,80,25,"CONSULTAR PALAVRA",0);
	
	putsxy("Digite a palavra a ser consultada (port.): ",3,5);
	fflush(stdin);
	getsxy(Reg.port,46,5);
	pos = BuscaPalavra(PtrArq, Reg);
	if(pos >= 0)
	{
		fseek(PtrArq,pos,SEEK_SET);
		fread(&Reg, sizeof(TpDicionario), 1, PtrArq);
		putsxy("Portugues: ",5,7);
		putsxy(Reg.port,16,7);
		putsxy("Ingles: ",5,9);
		putsxy(Reg.ing,13,9);
		putsxy("Significado: ",5,11);
		putsxy(Reg.sig,18,11);
		putsxy("Pontuacao: ",5,13);
		gotoxy(16,13);
		printf("%d",Reg.Pontos);
	}
	else
		putsxy("Palavra nao cadastrada!",5,7);
		
	getch();
	fclose(PtrArq);
}

void ConsultarCadPessoa(FILE *B_Arquivo)
{
	int pos;
	TpPessoa Reg;
	B_Arquivo = fopen("Pessoa.dat","rb");
	
	clrscr();
	DesenharTela(1,1,80,25,"CONSULTAR CADASTRO",0);
	
	putsxy("Digite o login do usuario a ser consultado: ",3,5);
	fflush(stdin);
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
		printf("%02d/%d/%d", Reg.Data.Dia, Reg.Data.Mes, Reg.Data.Ano);
	}
	else
		putsxy("Usuario nao cadastrado!",5,7);
		
	getch();
	fclose(B_Arquivo);
}

void SelecionaAlteraPalavra(int opcao, TpDicionario &Reg)
{
	if(opcao == 7)
	{
		fflush(stdin);
		DeletaLinha(16,7);
		getsxy(Reg.port,16,7);
		while(strcmp(Reg.port,"\0") == 0)
		{
			DeletaLinha(16,7);
			fflush(stdin);
			getsxy(Reg.port,16,7);
		}
	}
	else if(opcao == 9)
	{
		fflush(stdin);
		DeletaLinha(13,9);
		getsxy(Reg.ing,13,9);
		while(strcmp(Reg.ing,"\0") == 0)
		{
			DeletaLinha(13,9);
			fflush(stdin);
			getsxy(Reg.ing,13,9);
		}
	}
	else if(opcao == 11)
	{
		fflush(stdin);
		DeletaLinha(18,11);
		getsxy(Reg.sig,18,11);
		while(strcmp(Reg.sig,"\0") == 0)
		{
			DeletaLinha(18,11);
			fflush(stdin);
			getsxy(Reg.sig,18,11);
		}
	}
	
	else if(opcao == 13)
	{
		DeletaLinha(16,13);
		scanf("%d",&Reg.Pontos);
		while(Reg.Pontos <= 0)
		{
			DeletaLinha(16,13);
			scanf("%d",&Reg.Pontos);
		}
	}
	

}

int BuscaExaustivaPessoa(FILE *B_Arquivo, TpPessoa PBuscar)
{
	TpPessoa Reg;
	rewind(B_Arquivo);
	
	Reg.Status=0;

	fread(&Reg, sizeof(TpPessoa), 1, B_Arquivo);
	while((!feof(B_Arquivo) && stricmp(Reg.Login, PBuscar.Login) != 0) || Reg.Status == 1)
		fread(&Reg,sizeof(TpPessoa),1,B_Arquivo);

	
	if(!feof(B_Arquivo))
		return ftell(B_Arquivo)-sizeof(TpPessoa);
	else
		return -1;
}
