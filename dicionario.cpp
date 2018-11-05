#include<stdio.h>
#include<conio2.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

struct TpDicionario{
	char port[20], ing[20], sig[400];
};

void menu(){
	clrscr();
	gotoxy(3,3);printf("A - Cadastro");
	gotoxy(3,4);printf("B - Remover");
	gotoxy(3,5);printf("C - Listar");
	gotoxy(3,6);printf("D - Tradutor");
}

void inclui(FILE *PtrArq){
	TpDicionario dicio;
	clrscr();
	PtrArq = fopen("dicionario.dat", "ab+");
	if(PtrArq == NULL){
		gotoxy(3,3);printf("Arquivo nao encontrado, deseja abrir? (S/N)");
		if(toupper(getche()) == 'S'){
			PtrArq = fopen("dicionario.dat", "ab+");
		}
	}
	clrscr();
	gotoxy(3,3);printf("Palavra em Portugues: ");
	gets(dicio.port);
	while(strcmp(dicio.port, "\0") != 0){
		
		gotoxy(3,4);printf("Palavra em Ingles: ");
		gets(dicio.ing);
		gotoxy(3,5);printf("Significado: ");
		gets(dicio.sig);
		fwrite(&dicio, sizeof(TpDicionario), 1, PtrArq);
		gotoxy(3,3);printf("Palavra em Portugues: ");
		gets(dicio.port);
	}
	fclose(PtrArq);
}
void remover(FILE *PtrArq){
	remove("dicionario.dat");
}

void lista(FILE *PtrArq){
	TpDicionario dicio;
	clrscr();
	PtrArq = fopen("dicionario.dat", "rb");
	fread(&dicio, sizeof(TpDicionario), 1, PtrArq);
	while(!feof(PtrArq)){
		printf("%s\n", dicio.port);
		printf("%s \n", dicio.ing);
		printf("%s \n", dicio.sig);
		fread(&dicio, sizeof(TpDicionario), 1, PtrArq);
	}
	fclose(PtrArq);
	getch();
}

void traduz(FILE *PtrArq){
	TpDicionario r;
	char p[20];
	clrscr();
	PtrArq = fopen("dicionario.dat", "rb");
	printf("Palavra: ");
	gets(p);
	
	fread(&r, sizeof(TpDicionario), 1, PtrArq);
	while(!feof(PtrArq)){
		if(strcmp(r.port, p) == 0){
			printf("\n%s", r.ing);
		}
		fread(&r, sizeof(TpDicionario), 1, PtrArq);
	}
	getch();
}

void executa(){
	menu();
	char op;
	FILE *PtrArq;
	op = toupper(getche());
	do{
		
		switch(op){
			case 'A':
				inclui(PtrArq);break;
			case 'B':
				remover(PtrArq);break;
			case 'C':
				lista(PtrArq);break;
			case 'D':
				traduz(PtrArq);break;
			default: 
				gotoxy(3,5);printf("Nao encontrado");
		}
		menu();
		op = toupper(getche());
	}while(op != 27);
}

int main(){
	executa();
	getch();
}
