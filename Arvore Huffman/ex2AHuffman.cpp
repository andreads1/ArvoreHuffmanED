#include<stdio.h>
#include<stdlib.h>
#include<conio2.h>
#include<string.h>

struct TpTree{
    int freq, simbolo;
    struct TpTree *esq, *dir;
};
typedef struct TpTree Tree;

struct TpReg{
    char palavra[30], cod[15];
    int simbolo, frequencia;
};
typedef struct TpReg Reg;

void init(Tree **raiz)
{
    *raiz = NULL;
}

Tree *criaNo(int simb, int freq)
{
    Tree *no = (Tree*) malloc(sizeof(Tree));
    no->simbolo = simb;
    no->freq = freq;
    no->esq = no->dir = NULL;
    return no;
}

int buscaFrequenciaSimb(int simb)
{
	FILE *arq = fopen("tabela.dat", "rb");
	int freq;
	Reg reg;
	
	fread(&reg, sizeof(Reg), 1, arq);
    while(!feof(arq) && reg.simbolo != simb)
        fread(&reg, sizeof(Reg), 1, arq);
	
	freq = reg.frequencia;
	fclose(arq);
	return freq;
}

//
void criaArvHuff(Tree **raiz)
{	
	char cod[15];
	int i, frequencia, tamCod = 0;
	Tree *nc;
	FILE *arq = fopen("tabela.dat","rb");
	Reg registro;
	Tree *no;
	
	fread(&registro, sizeof(Reg), 1, arq);
	while(!feof(arq)){
		strcpy(cod, registro.cod);
		tamCod = strlen(cod); //tamanho do codigo 
		no = *raiz;
		if(*raiz == NULL)
		{
            nc = criaNo(-1, 0);
            *raiz = nc;
            no = *raiz;
        }
        for(i=0; i<tamCod; i++)
        {
        	if(i == (tamCod-1))//quando chega ao final do código do simbolo grava ele e sua frequencia
			{ 
                frequencia = buscaFrequenciaSimb(registro.simbolo);
                nc = criaNo(registro.simbolo, frequencia);
            }
            else
                nc = criaNo(-1, 0); //nó que será utilizado apenas para o percurso

            if(cod[i] == '0') //é 0 então vai para a esquerda
			{  
                if(no->esq == NULL)
				{
                    no->esq = nc;
                    no = no->esq;
                }
                else
                    no = no->esq;
            }
            else
			if(cod[i]=='1') //é 1 então vai para a direita
			{   
                if(no->dir == NULL)
				{
                    no->dir = nc;
                    no = no->dir;
                }
                else
                    no = no->dir;
            }
		}
		fread(&registro, sizeof(Reg), 1, arq);
	}
	fclose(arq);
}

void mostraArquivo(){
    FILE *arq = fopen("tabela.dat", "rb");
    Reg reg;

    fread(&reg, sizeof(Reg), 1, arq);
    while(!feof(arq)){
        printf("%d  \t  %s \t %d \t %s\n", reg.simbolo, reg.palavra, reg.frequencia, reg.cod);
        fread(&reg, sizeof(Reg), 1, arq);
    }
    fclose(arq);
}

////para ver a estrutura da arvore
void exibeArvore(Tree *raiz, int col, int lin, int dist){
    if(raiz != NULL){
        gotoxy(col, lin);
        printf("%d", raiz->simbolo);

        if(raiz->esq != NULL){
            gotoxy(col-dist/2, lin+1);
            printf("/");
        }
        if(raiz->dir != NULL){
            gotoxy(col+dist/2, lin+1);
            printf("\\");
        }

        exibeArvore(raiz->esq, col-dist, lin+2, dist/2);
        exibeArvore(raiz->dir, col+dist, lin+2, dist/2);
    }
}

void buscaPalavra(char cod[15])
{
	FILE *arq = fopen("tabela.dat", "rb");
	Reg reg;
	
	fread(&reg, sizeof(Reg), 1, arq);
    while(!feof(arq) && strcmp(reg.cod, cod)!=0)
        fread(&reg, sizeof(Reg), 1, arq);
	
	printf("%s ", reg.palavra);
}

void decodificarCodigo(Tree *raiz)
{
	FILE *arq = fopen("codigofrase.txt", "r");
	Tree *arv;
	
	char codParc;
	int i=0;
	char cod[15];
	
	arv = raiz;
	
	do{
		codParc = fgetc(arq);
		//printf("%c ", codParc);
		if(arv->simbolo>-1)//simbolo nao é -1 então há uma palavra para ser mostrada
		{
			arv = raiz;
			cod[i++] = '\0';
			buscaPalavra(cod);
			i=0;
		}		
		
		//percorre a arvore para esquerda ou direita (0/1)
		if(codParc =='0')
		{
			cod[i++] = '0';
			arv = arv->esq;
		}
		else
		{
			cod[i++] = '1';
			arv = arv->dir;
		}
	}while(!feof(arq));
	fclose(arq);
}

int main(){
	Tree *raiz;
	init(&raiz);
	criaArvHuff(&raiz);
	
	decodificarCodigo(raiz);

}
