#include <stdio.h>
#include <conio2.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>

struct TpData
{
	int dia, mes, ano;	
};

struct TpDicionario{
	char Port[30], Ing[30], Significado[100];
};

struct TpPessoa
{
	char login[10], nome[40];
	int senha, status, totPontos;
	TpData Data;	
};


char Menu(void)
{
	int local;
	clrscr();
	
	
	gotoxy(2,2);printf("Cadastrar pessoa");
	gotoxy(2,3);printf("Gerar relatorio");
	gotoxy(2,4);printf("Procura");
	gotoxy(2,5);printf("Sair");
	
	
	
	return toupper(getche());
}

float local(){
	float local = 1.1;
	int tecla;
	tecla = getch();
	
	if(tecla == -32){
		tecla = getch();
		switch(tecla){
			case 80: //baixo
				local = local+0.1;break;
			case 72: //cima
				local = local -0.1;break;
			case 77: //direita
				local++;break;
			case 75: // esquerda
				local--;break;
					
		}	
	}
	
	if(local == 1)
		local = 1.5;
	if(local < 1)
		local = 3.3;
		
	if(local == 3.4)
		local = 3.1;
	
	if(local > 3.4)
		local = 1.1;
	
	if(local == 2)
		local = 2.6;
	if(local == 2.7)
		local = 2.1;
	if(local == 3)
		local = 3.3;
}

void dicionario(FILE *ponteiro){
	TpDicionario dicio;
	ponteiro = fopen("dicionario.sql", "rb");
	if(ponteiro == NULL){
		ponteiro = fopen("dicionario.dat","rb");
		
	}
	fclose(ponteiro);
}

void CadPessoa(FILE *PtrArq)
{
	TpPessoa Reg;
	
	clrscr();
	
	PtrArq = fopen("Pessoa.dat", "ab+");
	
	if(PtrArq == NULL)
		printf("Impossivel abrir arquivo\n");
	else
	{
		printf("Login:\n");
		fflush(stdin);
		gets(Reg.login);
		
		while(strcmp(Reg.login, "\0") != 0)
		{
			printf("Nome:\n");
			fflush(stdin);
			gets(Reg.nome);
			
			printf("Senha:\n");
			textcolor(0);
			scanf("%d", &Reg.senha);
			
			textcolor(7);
			
			fflush(stdin);
			
			printf("Data (dd mm aaaa):\n");
			scanf("%d %d %d", &Reg.Data.dia, &Reg.Data.mes, &Reg.Data.ano);
			
			Reg.totPontos = 0;
			Reg.status = 1;
			
			fwrite(&Reg, sizeof(TpPessoa), 1, PtrArq);
			
			printf("\n\nPessoa cadastrada!\n\n");
			printf("Login:\n");
			fflush(stdin);
			gets(Reg.login);
		}
		
	}
	
	fclose(PtrArq);
	
	getch();
}

void Ordena(FILE *PtrArq){
	int a, b, QtdeR;
	TpPessoa regA, regB;
	PtrArq = fopen("Pessoa.dat", "rb+");fseek(PtrArq, 0, 2);
	QtdeR = ftell(PtrArq)/sizeof(TpPessoa);
	
	for(a = 0; a< QtdeR -1 ;a++){
		for(b = a+1; b <QtdeR; b++){
			fseek(PtrArq, a*sizeof(TpPessoa), 0);
			fread(&regA, sizeof(TpPessoa), 1, PtrArq);
			fseek(PtrArq, b*sizeof(TpPessoa), 0);
			fread(&regB, sizeof(TpPessoa), 1, PtrArq);
			if(strcmp(regA.nome, regB.nome)>0){
				fseek(PtrArq, a*sizeof(TpPessoa), 0);
				fwrite(&regB, sizeof(TpPessoa), 1, PtrArq);
				fseek(PtrArq, b*sizeof(TpPessoa), 0);
				fwrite(&regA, sizeof(TpPessoa), 1, PtrArq);
			}
		}
	}
	getch();
	fclose(PtrArq);
}

void Relatorio(FILE *PtrArq)
{
	TpPessoa Reg;
	
	clrscr();
	
	PtrArq = fopen("Pessoa.dat", "rb");
	
	if(PtrArq == NULL)
		printf("Impossivel abrir arquivo\n");
	else
	{
		fread(&Reg, sizeof(TpPessoa), 1, PtrArq);
		while(!feof(PtrArq))
		{
			
			
			printf("Nome: %s\n", Reg.nome);
			printf("Login: %s\n", Reg.login);
			printf("Senha: %d\n", Reg.senha);
			printf("Status: %d\n", Reg.status);
			printf("%d pontos\n", Reg.totPontos);
			printf("Data: %d/%d/%d", Reg.Data.dia, Reg.Data.mes, Reg.Data.ano);
			printf("\n**********\n");
			fread(&Reg, sizeof(TpPessoa), 1, PtrArq);
		}
		
		printf("\n\nListagem concluida\n\n");
	}
	
	fclose(PtrArq);
	
	getch();
}

void RelatorioNome(FILE *PtrArq)
{
	TpPessoa R;
	char aux;
	clrscr();
	PtrArq=fopen("Pessoa.dat","rb");
	if(PtrArq == NULL)
		printf("Arquivo não existe!");
	else
	{
		printf("\n Informe a Letra que deseja buscas: ");
		fflush(stdin);
		scanf("%c",&aux);
		fread(&R,sizeof(TpPessoa),1,PtrArq);
		while(!feof(PtrArq))
		{
			if(R.nome[0]==aux)
			{
				printf("Nome: %s\n", R.nome);
				printf("Login: %s\n", R.login);
				printf("Senha: %d\n", R.senha);
				printf("Status: %d\n", R.status);
				printf("%d pontos\n", R.totPontos);
				printf("Data: %d/%d/%d", R.Data.dia, R.Data.mes, R.Data.ano);
				printf("\n**********\n");
			}
			fread(&R, sizeof(TpPessoa), 1, PtrArq);
		}
	}
	getch();
	fclose(PtrArq);
}

int BuscaExaustiva(FILE *fp, char LOGIN[10]){
	TpPessoa R;
	char nome[40];
	
	rewind(fp);
	
	fread(&R, sizeof(TpPessoa), 1, fp);
	while(!feof(fp) && strcmp(R.login, LOGIN) != 0)
		fread(&R, sizeof(TpPessoa), 1, fp);
	
	
	if(strcmp(strupr(R.login), strupr(LOGIN)) == 0)
		return (ftell(fp) - sizeof(TpPessoa));
	else
		return -1;
}

void Altera(FILE *PtrArq){
	int pos; TpPessoa R, novo;
	PtrArq = fopen("Pessoa.dat", "rb+");
	clrscr();printf("**Alteracao**\n");
	printf("\nDigite o Login\n");fflush(stdin);
	gets(R.login);
	pos = BuscaExaustiva(PtrArq, R.login);
	if(pos == -1)
		printf("\nLogin nao encontrado!\n");
	else{
		
		fseek(PtrArq, pos, 0);
		fread(&R, sizeof(TpPessoa), 1, PtrArq);
		printf("\nDados de contrato\n");
		printf("login: %s \n Nome: %s \n Pontuacao %d \n Status %d \n Data %d/%d/%d\n", R.login, R.nome, R.totPontos, R.status, R.Data.dia,R.Data.mes,R.Data.ano);
		printf("\n\n Deseja Alterar? ");
		if(toupper(getche()) == 'S'){
		
			printf("\nLogin:");fflush(stdin);
			gets(R.login);
			printf("Nome:\n");
			fflush(stdin);
			gets(R.nome);
				
			printf("Senha:\n");
			textcolor(0);
			scanf("%d", &R.senha);
			
			textcolor(7);
			
			fflush(stdin);
			
			printf("Data (dd mm aaaa):\n");
			scanf("%d %d %d", &R.Data.dia, &R.Data.mes, &R.Data.ano);
			R.totPontos = 0;
			R.status = 1;
			fseek(PtrArq,pos,0);
			fwrite(&R, sizeof(TpPessoa),1,PtrArq);
		}
		else{
			printf("Cancelado");
		}
	}
	fclose(PtrArq);
	
}

void Consulta(FILE *PtrArq){
	int pos; TpPessoa R;
	PtrArq = fopen("Pessoa.dat","rb");
	clrscr();printf("\n**Consulta**\n");
	printf("\nDigite o Login: ");fflush(stdin);
	gets(R.login);
	while(strcmp(R.login, "\0")!=0){
		pos = BuscaExaustiva(PtrArq, R.login);
		if(pos== -1)
			printf("\nLogin Nao Cadastrado!\n");
		else{
			/*fseek -> Filtragem do arquivo, parametros:
				0 - do inicio, 0 --SEEK_SET
				1 - corrente, a partir do atual -- SEEK_CUR
				2 - a partir do final -- SEEK_END
			*/
			fseek(PtrArq, pos, 0);
			fread(&R, sizeof(TpPessoa), 1, PtrArq);
			printf("\nDados de contrato\n");
			printf("login: %s \n Nome: %s \n Pontuacao %d \n Status %d \n Data %d/%d/%d\n", R.login, R.nome, R.totPontos, R.status, R.Data.dia,R.Data.mes,R.Data.ano);
		}
		printf("\n\nDigite o Login: ");fflush(stdin);
		gets(R.login);
	}
	fclose(PtrArq);
}

void Executa(void)
{
	char Op;
	FILE *ArqP;
	
	Op = Menu();
	
	while(Op != 27)
	{
		switch(Op)
		{
			case 'A':
				CadPessoa(ArqP);
			break;
			case 'B':
				Relatorio(ArqP);
			break;
			case 'C':
				RelatorioNome(ArqP);
				break;
			case 'D':
				Altera(ArqP);
				break;
			case 'E':
				Ordena(ArqP);
				break;
			default:
				printf("Opcao invalida\n");
				getch();
		}
		
		Op = Menu();
	}
	
	getch();
}



int main(void)
{
	Executa();
	return 1;
}
