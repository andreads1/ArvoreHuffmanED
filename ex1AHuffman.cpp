#include<stdio.h>
#include<stdlib.h>
#include<conio2.h>
#include<string.h>

struct TpTree{
    int freq, simbolo;
    struct TpTree *esq, *dir;
};
typedef struct TpTree Tree;

struct TpLista{
    Tree *raiz;
    struct TpLista *prox;
};
typedef struct TpLista Lista;

struct TpReg{
    char palavra[30], cod[15];//
    int simbolo, frequencia;
};
typedef struct TpReg Reg;

//funções de pilha para fazer percurso pela arvore - pré ordem
struct TpPilha{
    Tree *raiz;
    struct TpPilha *prox;
};
typedef struct TpPilha Pilha;


void initP(Pilha **P){
    *P = NULL;
}

char isEmpty(Pilha *P){
    return P == NULL;
}

void push(Pilha **P, Tree *raiz){
    Pilha *nova = (Pilha*) malloc(sizeof(Pilha));

    nova->raiz = raiz;
    nova->prox = *P;
    *P = nova;
}

void pop(Pilha **P, Tree **raiz){
    if(!isEmpty(*P)){
        Pilha *aux = *P;
        *raiz = (*P)->raiz;
        *P = (*P)->prox;
        free(aux);
    }
    else
        *raiz = NULL;
}

//construção da arvore de huffman usando lista encadeada ordenada //calculo da frequencia
void init(Tree **raiz){
    *raiz = NULL;
}

void initLista(Lista **L){
    *L = NULL;
}
Lista *criaCaixa(Tree *no){ 
    Lista *nc = (Lista*) malloc(sizeof(Lista));
    nc->raiz = no;
    nc->prox = NULL;
    return nc;
}

Tree *criaNo(int simb, int freq){
    Tree *no = (Tree*) malloc(sizeof(Tree));
    no->simbolo = simb;
    no->freq = freq;
    no->esq = no->dir = NULL;
    return no;
}

int buscaPalavraVetor(char palavra[30], Reg vet[], int TL) // verifica se uma palavra já existe no vetor de registros
{
	int i;
	if(TL>0){
		
		for(i=0; i<TL;i++)
		{
			if (strcmp(vet[i].palavra, palavra)==0)
			{
				return i;//posição em que a palavra está
			}
		}		
	}
	return -1;

}

void exibeVetorRegistro(Reg vet[], int TL)
{
	int i;
	for(i=0; i<TL;i++)
	{
		printf("Simbolo: %d, Palavra: %s, Frequencia: %d\n", vet[i].simbolo, vet[i].palavra, vet[i].frequencia);
	}
}

void ordenaVetor(Reg vetor[], int TL) //ordena o vetor de registros em ordem crescente
{
	Reg aux;
	
    for (int i = 0; i < TL; i++)
    {
        for (int j = i; j < TL; j++)
        {
            if (vetor[i].frequencia > vetor[j].frequencia)
            {
                aux = vetor[i];
                vetor[i] = vetor[j];
                vetor[j] = aux;
            }
        }
    }
}

Lista *insereFinal(Lista *L, Tree *no)
{
	Lista *novo;
	novo = criaCaixa(no);
	
	if(L==NULL)
	{
		//printf("vazia!!\n");
		L = novo;
	}
	else
	{
		Lista *aux = L;
		while(aux->prox!= NULL)
			aux = aux->prox;
			
		aux->prox = novo;
	}
	return L;
}

void exibeLista(Lista *L)
{
	Lista *aux;
	aux=L;
	while(aux!= NULL)
	{
		printf("Simbolo: %d, Frequencia: %d\n", aux->raiz->simbolo, aux->raiz->freq);
		aux= aux->prox;
	}
}


Lista *criaLista(Reg vet[], int TL)
{
	FILE *arq = fopen("arquivo.dat", "wb");
	Tree *arv;
	Lista *L= NULL;
	Reg reg;
	
	int i;
	for(i=0; i<TL;i++)
	{
		//já insere no arquivo as plavras existentes
    	reg.simbolo = vet[i].simbolo;
    	strcpy(reg.palavra, vet[i].palavra);
    	reg.frequencia = vet[i].frequencia;
		strcpy(reg.cod, "0");
		
	    fwrite(&reg, sizeof(Reg), 1, arq);
		
		arv = criaNo(vet[i].simbolo, vet[i].frequencia);
		L = insereFinal(L, arv);
	}	
	//exibeLista(L);
	
	fclose(arq);
	
	return L;
}

Lista *geraVetPalavras(Lista **L, char frase[200]) 
{
	int TL=0, i, j, qtd_palavras=0, tam;
	int simbolo=0, frequencia=0; 
	int buscador;
	tam = strlen(frase);
	char palavra[20];
	
	Lista *ListaHuff;

	for(i=0; i<tam; i++)
	{
		if(frase[i] == ' ')
			qtd_palavras++;
	}
	qtd_palavras++;
	//printf("Letras: %d; Quant. palavras: %d\n\n", tam, qtd_palavras);
	
	Reg vetRegistro[qtd_palavras];
	palavra[0] = '\0';
	j=0;
	
	for(i=0; i< tam+1; i++)
	{
		if(frase[i] == ' ' || frase[i]== '\0')
		{
			palavra[j] = '\0';
			j=0;
			buscador = buscaPalavraVetor(palavra, vetRegistro, TL);
			if(buscador> -1)
			{
				vetRegistro[buscador].frequencia++;
			}
			else{
				strcpy(vetRegistro[TL].palavra, palavra);
				vetRegistro[TL].simbolo = simbolo++;
				vetRegistro[TL].frequencia = ++frequencia;
				TL++;
				//printf("Inseriu nova palavra\n");
			}
			frequencia =0;
		}
		else
		{
			palavra[j++] = frase[i];
		}
	}
	ordenaVetor(vetRegistro, TL);
	//exibeVetorRegistro(vetRegistro, TL);
	ListaHuff = criaLista(vetRegistro, TL);	
	return ListaHuff;
}

Lista *insereHuffman(Lista *L, Tree *no){
	
    Lista *atual, *nc;
    Tree *guarda;

    nc = criaCaixa(no);
    nc-> prox = L;
    L = nc;

    atual = L;    
    while(atual->prox != NULL && atual->raiz->freq >= atual->prox->raiz->freq )
	{ 
		guarda = atual->raiz;
		//printf("1%d", guarda->simbolo);
		atual->raiz = atual->prox->raiz;
		atual->prox->raiz = guarda;
		
        atual = atual->prox;
    }
    return L;
}

Tree *retiraElemLista(Lista **L)// vai retirando os primeiros elementos da lista conforme for chamada
{
	Lista *retirar;
	retirar = *L;
	
    Tree *aux;
	aux = (*L)->raiz;
    *L = (*L)->prox;
    free(retirar);
    
    return aux;
}

void geraArvore(Tree **raiz, Lista *L){
    Tree *no1, *no2, *nc;
    int freq;

    while(L->prox != NULL){ //enquanto não acaba, vai retirando os primeiros 2 elementos e criando a estrutura de huffman   

        no1 = retiraElemLista(&L);
        no2 = retiraElemLista(&L);

        freq = no1->freq + no2->freq;
        nc = criaNo(-1, freq);  

        nc->esq = no2;
        nc->dir = no1;

        L = insereHuffman(L, nc);

    }
    *raiz = L->raiz;
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

void geraCodigoHuffman(Tree *raiz, Tree *no, int i, char lado, char auxcod[15], char cod[15]){
    if(raiz != NULL){ //não é null então decide se insere 0 ou 1 no código até chegar em NULL
        auxcod[i] = lado;
        if(raiz == no)
		{
            auxcod[++i] = '\0';
            for(int i = 0, j=1; j<15; j++, i++)
                cod[i] = auxcod[j];
        }
        geraCodigoHuffman(raiz->esq, no, ++i, '0', auxcod, cod);
        i--;
        geraCodigoHuffman(raiz->dir, no, ++i, '1', auxcod, cod);
        i--;
    }
}

void procuraPalavra(int simbolo, char palavra[30], char cod[15]){
    FILE *arq = fopen("arquivo.dat", "rb");
    Reg reg;
	Reg auxReg;
	
    rewind(arq);
    fread(&reg, sizeof(Reg), 1, arq);
    while(!feof(arq) && reg.simbolo != simbolo)
        fread(&reg, sizeof(Reg), 1, arq);

    if(reg.simbolo == simbolo)   
        strcpy(palavra, reg.palavra);
       
    FILE *arq2 = fopen("tabela.dat", "ab+");
    
	//insere no arquivo da tabela agora com o código   
    auxReg.simbolo = simbolo; 
    strcpy(auxReg.palavra, palavra);
    auxReg.frequencia = reg.frequencia;
    strcpy(auxReg.cod, cod);
 
 	fwrite(&auxReg, sizeof(Reg), 1, arq2);
        
    fclose(arq);
    fclose(arq2);
}

void geraArquivoTabela(Tree *raiz){
    Tree *aux;
    Reg reg;
    Pilha *P;
    char codigo[15], palavra[30], auxCod[15];
    int i;

	aux = raiz;
    initP(&P);
	
    push(&P, raiz);
    while(!isEmpty(P))
	{
        if(raiz != NULL){
            pop(&P, &raiz);
            while(raiz != NULL)
			{
                if(raiz->simbolo != -1)
				{  
                	i=0;
                    geraCodigoHuffman(aux, raiz, i, '0', auxCod, codigo);
                    strcpy(reg.cod, codigo);

                    reg.simbolo = raiz->simbolo;
			
                    procuraPalavra(reg.simbolo, palavra, codigo);
                }

                push(&P, raiz);
                raiz = raiz->esq;
            }
        }
        pop(&P, &raiz);
        raiz = raiz->dir;
        if(raiz != NULL)
            push(&P, raiz);
    }
}

void pegaCodigo(char palavra[30], char cod[15])
{
	FILE *arq = fopen("tabela.dat", "rb");
    Reg reg;
    
    fread(&reg, sizeof(Reg), 1, arq);
    while(!feof(arq) && strcmp(palavra, reg.palavra)!= 0)
        fread(&reg, sizeof(Reg), 1, arq);

    strcpy(cod, reg.cod);
        
    fclose(arq);
}


void codificaFrase(char frase[200]){
    FILE *arq = fopen("codigofrase.txt", "w+");

    int tam = strlen(frase);
	int i, j;
    char palavra[30], codigo[15];

    for(i=0; i< tam+1; i++)
	{
		if(frase[i] == ' ' || frase[i]== '\0')
		{
			palavra[j] = '\0';
			j=0;
			pegaCodigo(palavra, codigo);
			fputs(codigo, arq);
		}
		else
		{
			palavra[j++] = frase[i];
		}
	}

    fclose(arq);
}

///////////////
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


int main(){
    Tree *raiz;
    Lista *L = NULL;
	

    FILE *arq = fopen("tabela.dat", "wb");
    fclose(arq);

    init(&raiz);

    char frase[200] = "amo como ama o amor nao conheco nenhuma outra razao para amar senao amar que queres que te diga alem de que te amo se o que quero dizer te e que te amo";
	//char frase[200] = "amor amor amado amar";
	L = geraVetPalavras(&L, frase);

	//exibeLista(L);
	geraArvore(&raiz, L);

	geraArquivoTabela(raiz);
	//mostraArquivo();
	codificaFrase(frase);
	//in_ordem(raiz);
}
