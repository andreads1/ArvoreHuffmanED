#include<stdio.h>
#include<stdlib.h>
#include<conio2.h>
#include<math.h>
#include<time.h>
#define K 2
#define TF 6

struct kdtree
{
	int ponto[K];
	struct kdtree *esq, *dir;	
};
typedef struct kdtree kdTree;

kdTree *Cria_no(int ponto[K])
{
	int i;
	kdTree *no=(kdTree*)malloc(sizeof(kdTree));
	for(i=0; i<K; i++)
	{
		no->ponto[i]=ponto[i];
	}
	no->esq=no->dir=NULL;
	return no;
}

void ordena_pelo_X(int pontos[TF][K], int ini, int fim)
{
	int i, j;
	int aux[K];
	for (i = ini; i <= fim; i++)
    {
        for (j = i+1; j <= fim; j++)
        {
            if (pontos[i][0] > pontos[j][0])
            {
                aux[0] = pontos[i][0];
                aux[1] = pontos[i][1];
                pontos[i][0] = pontos[j][0];
                pontos[i][1] = pontos[j][1];
                pontos[j][0] = aux[0];
                pontos[j][1] = aux[1];
            }
        }
    }
}

void ordena_pelo_Y(int pontos[TF][K], int ini, int fim)
{
	int i, j;
	int aux[K];
	for (i = ini; i <= fim; i++)
    {
        for (j = i+1; j <= fim; j++)
        {
            if (pontos[i][1] > pontos[j][1])
            {
                aux[0] = pontos[i][0];
                aux[1] = pontos[i][1];
                pontos[i][0] = pontos[j][0];
                pontos[i][1] = pontos[j][1];
                pontos[j][0] = aux[0];
                pontos[j][1] = aux[1];
            }
        }
    }
}

void exibePontos(int pontos[TF][K], int TL)
{
	int i;
	for(i=0; i<TL; i++)
	{
		printf("(%d, %d)\n", pontos[i][0], pontos[i][1]);
	}
	//printf("\n");
}

void insereBalanceado(kdTree **raiz, int pontos[TF][K], int ini, int fim, int n)
{
	int d, meio;
	int ponto[K];
	
	if(ini<= fim)
	{	
		d = n%K;
		if(d==0)
			ordena_pelo_X(pontos, ini, fim);//poderia também ser uma unica função passando o discriminador (d) como parametro e entao dentro dela verificando se deve ordenar por x ou y
		else
			ordena_pelo_Y(pontos, ini, fim);
		meio = (ini+fim)/2;
		ponto[0] = pontos[meio][0];
		ponto[1] = pontos[meio][1];
		
		*raiz = Cria_no(ponto);
		
		insereBalanceado(&(*raiz)->esq, pontos, ini, meio-1, n+1);
		insereBalanceado(&(*raiz)->dir, pontos, meio+1, fim, n+1);
	}
}

void retornaPontosProximos(kdTree *raiz, int perto[TF][K], int ponto[K], double raio, int *TL)
{
	double distEuclid;
	if(raiz!= NULL)
	{
		distEuclid = sqrt(pow((ponto[0]- raiz->ponto[0]), 2) + pow((ponto[1] - raiz->ponto[1]), 2));
		if(distEuclid <= raio)
		{
			//printf("Ponto (%d,%d) esta proximo\n", raiz->ponto[0], raiz->ponto[1]);
			perto[*TL][0] = raiz->ponto[0];
			perto[*TL][1] = raiz->ponto[1];
			*TL = *TL +1;
		}
		retornaPontosProximos(raiz->esq, perto, ponto, raio, &*TL);
		retornaPontosProximos(raiz->dir, perto, ponto, raio, &*TL);
	}

}

void exibekdTree(kdTree* raiz,int x,int y,int dist)
{
	int i;
	if(raiz!=NULL)
	{
		gotoxy(x,y);
		for(i=0; i<K; i++)
			printf("%d,",raiz->ponto[i]);
		if(raiz->esq!=NULL)
		{
			gotoxy(x-dist/2,y+1);
			printf("/");
		}
	    if(raiz->dir!=NULL)
		{
			gotoxy(x+dist/2,y+1);
			printf("\\");
		}		
		exibekdTree(raiz->esq,x-dist,y+2,dist/2);
		exibekdTree(raiz->dir,x+dist,y+2,dist/2);
	}
}

void geraVetor(int vet[TF][K], int TL)
{
	int tam = TL;
	int ponto[K];
	srand(time(NULL));
	for(int i= 0; i< TF; i++)
	{
		ponto[0]= rand() % 30; 
		ponto[1]= rand() % 30;
		vet[tam][0] = ponto[0];
		vet[tam][1] = ponto[1];
		tam++;
	}
}

int main()
{
	kdTree*raiz=NULL;
	
	//int pontos[TF][K] = {{30,40},{5,25},{10,12},{70,70},{50,30},{35,45}};
	
	int vet[TF][K];
	geraVetor(vet, 0); //0 = TL do vetor de pontos a ser gerado
	//exibePontos(vet, TF); printf("\n");
	
	//questao A
	insereBalanceado(&raiz,vet,0, TF-1, 0);
	
	//questao B
	int TL=0;
	int ponto[K] = {8,18};
	int proximos[TF][K]; //para guardar pontos que estiverem proximos de 8,18
	retornaPontosProximos(raiz, proximos, ponto, 10, &TL);
	if(TL ==0)
		printf("**Nao ha pontos proximos do ponto dado**\n");
	else
	{
		printf("***Pontos proximos de (%d,%d)***\n", ponto[0], ponto[1]);
		exibePontos(proximos, TL);
	}
	getch();
	
	//questao C
	system("cls");
	exibekdTree(raiz,40,1,17);
	getch();
}
