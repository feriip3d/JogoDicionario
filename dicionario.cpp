#include<stdio.h>
#include<conio2.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

struct TpDicionario{
	char port[20], ing[20], sig[400];
};

int menu(){
	clrscr();
	int pos = 10, tecla;
	do{
		if(tecla == 80){
			pos = pos+10;
		}
		if(tecla == 72){
			pos = pos-10;
		}
		if(pos == 0){
			pos = 60;
		}
		if(pos == 70){
			pos =10;
		}
		gotoxy(3,3);printf("Cadastro");
		gotoxy(3,4);printf("Remover");
		gotoxy(3,5);printf("Listar");
		gotoxy(3,6);printf("Tradutor");
		gotoxy(3,7);printf("Alterar");
		gotoxy(3,8);printf("Sair");
	
		if(pos == 10){
			gotoxy(3,3);textbackground(WHITE);textcolor(BLACK);printf("CADASTRO");textbackground(BLACK);textcolor(WHITE);
		}
		if(pos == 20){
			gotoxy(3,4);textbackground(WHITE);textcolor(BLACK);printf("REMOVER");textbackground(BLACK);textcolor(WHITE);
		}
		if(pos == 30){
			gotoxy(3,5);textbackground(WHITE);textcolor(BLACK);printf("LISTAR");textbackground(BLACK);textcolor(WHITE);
		}
		if(pos == 40){
			gotoxy(3,6);textbackground(WHITE);textcolor(BLACK);printf("TRADUTOR");textbackground(BLACK);textcolor(WHITE);
		}
		if(pos == 50){
			gotoxy(3,7);textbackground(WHITE);textcolor(BLACK);printf("ALTERAR");textbackground(BLACK);textcolor(WHITE);
		}
		if(pos == 60){
			gotoxy(3,8);textbackground(WHITE);textcolor(BLACK);printf("SAIR");textbackground(BLACK);textcolor(WHITE);
		}
		tecla = getch();
	}while(tecla != 13);
return pos;		
}

void inclui(FILE *PtrArq){
	TpDicionario dicio;
	clrscr();
	PtrArq = fopen("dicionario.dat", "rb+");
	
	if(PtrArq == NULL){
		gotoxy(3,3);printf("Arquivo nao encontrado, deseja abrir? (S/N)");
		if(toupper(getche()) == 'S'){
			PtrArq = fopen("dicionario.dat", "ab+");
		}
	}
	else{
		fclose(PtrArq);
		fopen("dicionario.dat", "ab+");
	}
	clrscr();
	gotoxy(3,3);printf("Palavra em Portugues: ");
	gets(dicio.port);
	while(strcmp(dicio.port, "\0") != 0){
		do{
			gotoxy(3,4);printf("Palavra em Ingles:                          ");
			gotoxy(21,4);gets(dicio.ing);
		}while(strcmp(dicio.ing, "\0") ==0);
		do{
			gotoxy(3,5);printf("Significado:                             ");
			gotoxy(15,5);gets(dicio.sig);
		}while(strcmp(dicio.sig, "\0") ==0);
		fwrite(&dicio, sizeof(TpDicionario), 1, PtrArq);
		clrscr();
		gotoxy(3,3);printf("Palavra em Portugues: ");
		gets(dicio.port);
	}
	fclose(PtrArq);
}
void remover(FILE *PtrArq){
	remove("dicionario.dat");
}

int busca(FILE *PtrArq, char palavra[50], int tipo){
	TpDicionario r;
	fread(&r, sizeof(TpDicionario),1, PtrArq);
	if(tipo == 1){
	
		while(!feof(PtrArq)){
			if(stricmp(r.port, palavra) == 0){
				return ftell(PtrArq);
			}
			fread(&r, sizeof(TpDicionario),1, PtrArq);
		}
	}else
	if(tipo == 2){
		while(!feof(PtrArq)){
			if(stricmp(r.ing, palavra)==0){
				return ftell(PtrArq)-sizeof(TpDicionario);
			}
			fread(&r, sizeof(TpDicionario),1, PtrArq);
		}
	}else{
			gotoxy(3,5);printf("Nao encontrado");
	}
return -1;
}

void altera(FILE *PtrArq){
	TpDicionario dicio;
	char palavra[20];
	int tipo = 0, pos = -1;
	PtrArq = fopen("TpDicionario", "rb+");
	clrscr();
	gotoxy(3,3);printf("1 - Portugues");
	gotoxy(3,4);printf("2 - Ingles");
	tipo = getch();
	if(tipo == 1||tipo == 2){
		do{
			gotoxy(3,6);printf("Digite a palavra:                                     ");
			gotoxy(21,6);gets(palavra);
		}while(strcmp(palavra, "\0") == 0);
		pos=busca(PtrArq,palavra, tipo);
	}else{
		gotoxy(3, 6);printf("Invalido");
	}
	
	if(pos != -1){
		if(tipo == 1){
			fseek(PtrArq, pos, 0);
			fread(&dicio, sizeof(TpDicionario),1,PtrArq);
			fflush(stdin);scanf("%s", &dicio.ing);
			fseek(PtrArq,pos,0);
			fwrite(&dicio, sizeof(TpDicionario), 1, PtrArq);
		}
	}
	fclose(PtrArq);
}

void lista(FILE *PtrArq){
	TpDicionario dicio;int i = 4;
	clrscr();
	PtrArq = fopen("dicionario.dat", "rb");
	fread(&dicio, sizeof(TpDicionario), 1, PtrArq);
	gotoxy(3,3);printf("Port");gotoxy(15,3);printf("Ingles");gotoxy(30,3);printf("Significado");
	while(!feof(PtrArq)){
		gotoxy(3,i);printf("%s",dicio.port);gotoxy(15,i);printf("%s",dicio.ing);gotoxy(30,i);printf("%s", dicio.sig);
		fread(&dicio, sizeof(TpDicionario), 1, PtrArq);
		i++;
	}
	fclose(PtrArq);
	getch();
}

void traduz(FILE *PtrArq){
	TpDicionario r;
	char p[20];
	clrscr();
	PtrArq = fopen("dicionario.dat", "rb");
	gotoxy(3,3);printf("Palavra: ");
	gets(p);
	
	fread(&r, sizeof(TpDicionario), 1, PtrArq);
	while(!feof(PtrArq)){
		if(strcmp(r.port, p) == 0){
			gotoxy(3,4);printf("Traducao: %s", r.ing);
			gotoxy(3,5);printf("Significado: %s", r.sig);
		}
		fread(&r, sizeof(TpDicionario), 1, PtrArq);
	}
	getch();
	fclose(PtrArq);
}

void executa(){
	FILE *PtrArq;
	int pos =0;
	do{
		pos = menu();
		switch(pos){
			case 10:
				inclui(PtrArq);break;
			case 20:
				remover(PtrArq);break;
			case 30:
				lista(PtrArq);break;
			case 40:
				traduz(PtrArq);break;
			case 50:
				altera(PtrArq);break;
			case 60:
				pos = 27;break;
		}
		
	}while(pos != 27);
}

int main(){
	executa();
	getch();
}
