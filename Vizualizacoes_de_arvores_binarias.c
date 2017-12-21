/*  @file: Arvore_binaria.c
*   @author: Hiago dos Santos (hiagop22@gmail.com)
*
*   Cria uma árvore com as entradas inteiras do usuário, logo em seguida
*	mostrando as diferentes formas de vizualização de uma árvore:
* 	Ordem, pós-ordem, pré-ordem e em nível.
*/


#include <stdio.h>
#include <stdlib.h>
#define MAIOR(x,y) ((x>=y)?x:y)

typedef struct no{
	int dado;
	struct no *esq;
	struct no *dir;
}no;

typedef struct elemento_no{
	no *dado;
	struct elemento_no *prox;
}elemento_no;

typedef struct lista{
	elemento_no *inicio;
	elemento_no *fim;
}lista;

void limpa_tela(){
	system("clear || cls");
}

elemento_no *cria_elemento_no(){
	elemento_no* tmp = (elemento_no*) malloc(sizeof(elemento_no));

	if(tmp){
	tmp->prox = NULL;
	tmp->prox = NULL;
	}
	return tmp;
}

no *busca(no *raiz, int dado){
	if(!raiz)
		return raiz;

	if(dado < raiz->dado)
		return busca(raiz->esq, dado);
	if(dado > raiz->dado)
		return busca(raiz->dir, dado);

	return raiz;
}

no *cria_no(int dado){
	no *tmp = (no*) malloc(sizeof(no));

	if(tmp){
		tmp->dado = dado;
		tmp->esq = NULL;
		tmp->dir = NULL;
	}

	return tmp;
}

no *adiciona_na_arvore(no *raiz, no *novo_no){
	if(!raiz)
		return novo_no;
	if(novo_no->dado > raiz->dado)
		raiz->dir = adiciona_na_arvore(raiz->dir, novo_no);
	if(novo_no->dado < raiz->dado)
		raiz->esq = adiciona_na_arvore(raiz->esq, novo_no);

	return raiz;
}

int altura_arvore(no *raiz){
	if(!raiz)
		return 0;

	return 1 + MAIOR(altura_arvore(raiz->esq), altura_arvore(raiz->dir));
}

/*Retorna 1 se a árvore estiver balanceada e 0 caso contrário*/
int balanceada(no *raiz){
	if(!raiz)
		return 1;

	if(!balanceada(raiz->esq))
		return 0;
	if(!balanceada(raiz->dir))
		return 0;

	if(abs(altura_arvore(raiz->esq) - altura_arvore(raiz->dir)) > 1)
		return 0;

	return 1;
}

int le_entrada(){
	int dado;

	do{
		limpa_tela();
		puts("\nPara deixar de adicionar elementos na arvore digite um número negativo.\n");
		printf("Valor que deseja adicionar na árvore: ");
		scanf("%i", &dado);
	}while(getchar() != '\n');

	return dado;
}

void mostra_arvore_pre_ordem(no *raiz){
	if(!raiz)
		return;

	printf("%i\n", raiz->dado);
	mostra_arvore_pre_ordem(raiz->esq);
	mostra_arvore_pre_ordem(raiz->dir);
}

void mostra_arvore_pos_ordem(no *raiz){
	if(!raiz)
		return;

	mostra_arvore_pos_ordem(raiz->esq);
	mostra_arvore_pos_ordem(raiz->dir);

	printf("%i\n", raiz->dado);
}

void mostra_arvore_em_ordem(no *raiz){
	if(!raiz)
			return;

	mostra_arvore_em_ordem(raiz->esq);
	printf("%i\n", raiz->dado);
	mostra_arvore_em_ordem(raiz->dir);
}

void zera_lista(lista *l){
	l->inicio = NULL;
	l->fim = NULL;
}

int adiciona_fim_lista(lista *l, no *raiz){
	elemento_no *tmp;

	tmp = cria_elemento_no();
	if(!tmp){
		puts("Não foi possível adicionar no fim da lista.");
		return 0;
	}

	if(l->inicio)
		l->fim->prox = tmp;
	else
		l->inicio = tmp;

	l->fim = tmp;
	l->fim->dado = raiz;

	return 1;
}

int remove_fila(lista *l){
	elemento_no *aux;
	int dado;

	if(!(l->inicio))
		return -1;

	if(l->inicio == l->fim){
		dado = l->inicio->dado->dado;

		free(l->inicio);
		zera_lista(l);

		return dado;
	}

	dado = l->inicio->dado->dado;
	aux = l->inicio;
	l->inicio = l->inicio->prox;

	free(aux);
	return dado;
}

void libera_memoria(lista *l){

	while(l->inicio)
		remove_fila(l);
}

int mostra_arvore_em_nivel(no *raiz){
	lista l;
	zera_lista(&l);

	if(!raiz)
		return 1;

	if(!adiciona_fim_lista(&l, raiz))
			return 0;
	do{
		if(l.inicio->dado->esq)
			adiciona_fim_lista(&l, l.inicio->dado->esq);
		if(l.inicio->dado->dir)
			adiciona_fim_lista(&l, l.inicio->dado->dir);
		printf("%i\n", remove_fila(&l));
	}while(l.inicio);

	libera_memoria(&l);
	return 1;
}

void destroi_arvore(no *raiz){
	if(!raiz)
		return;

	destroi_arvore(raiz->esq);
	destroi_arvore(raiz->dir);

	free(raiz);
}

no *rotaciona_esquerda(no *raiz){
	no *aux = NULL;

	if(!raiz->dir)
		return raiz;

	aux = raiz->dir;
	raiz->dir = raiz->dir->esq;
	aux->esq = raiz;

	return aux;
}

no *rotaciona_direita(no *raiz){
	no *aux = NULL;

	if(!raiz->esq)
		return raiz;

	aux = raiz->esq;
	raiz->esq = raiz->esq->dir;
	aux->dir = raiz;

	return aux;
}

no *balanceia_arvore(no *raiz){
	int altura = 0;
	if(!raiz)
		return NULL;

	raiz->esq = balanceia_arvore(raiz->esq);
	raiz->dir = balanceia_arvore(raiz->dir);

	altura = (altura_arvore(raiz->dir) - altura_arvore(raiz->esq));

	if(altura > 1){
		altura = (altura_arvore(raiz->dir->dir) - altura_arvore(raiz->dir->esq));
		if(altura>0){
			raiz->dir = rotaciona_direita(raiz->dir);
		}
		if(altura<0){
			raiz->dir = rotaciona_esquerda(raiz->dir);
		}
		raiz = rotaciona_esquerda(raiz);
	}

	if(altura < -1){
		altura = (altura_arvore(raiz->esq->dir) - altura_arvore(raiz->esq->esq));
		if(altura>0){
			raiz->esq = rotaciona_esquerda(raiz->esq);
		}
		if(altura<0){
			raiz->esq = rotaciona_direita(raiz->esq);
		}
		raiz = rotaciona_direita(raiz);
		raiz = rotaciona_direita(raiz);
	}

	return raiz;
}

int main(int argc, char *argv[]){
	no *raiz = NULL, *novo_no= NULL;
	int dado;

	/*Enquanto entrada >0 lê entrada, balanceando a árvore caso esteja desbalanceada (balanceamento AVL)*/
	while(1){
		dado = le_entrada();
		if(dado<0)
			break;
		novo_no = cria_no(dado);
			if(!novo_no){
				puts("Não foi possível criar um novo nó.");
				return EXIT_FAILURE;
			}
		raiz = adiciona_na_arvore(raiz, novo_no);

		while(!balanceada(raiz))
			raiz = balanceia_arvore(raiz);
	}

	puts("\nPós-ordem:\n");
	mostra_arvore_pos_ordem(raiz);

	puts("\nPré-ordem:\n");
	mostra_arvore_pre_ordem(raiz);

	puts("\nEm ordem:\n");
	mostra_arvore_em_ordem(raiz);

	puts("\nEm nivel:\n");
	if(!mostra_arvore_em_nivel(raiz))
		return EXIT_FAILURE;

	printf("Altura da árvore: %i\n", altura_arvore(raiz));

	destroi_arvore(raiz);

	return EXIT_SUCCESS;
}