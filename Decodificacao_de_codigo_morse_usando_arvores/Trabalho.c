/*	@file: trabalho2.c
* 	@authors: Hiago dos Santos - 160124492
*
*	Trabalho 2 de Estruturas de dados
*	Árvores Binarias (decodificação de código Morse)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#define TAM_NOME_ARQUIVO 50
#define TAM_PALAVRA_CODIFICADA 10
#define MAIOR(x,y) ((x>=y)?x:y)

typedef struct no{
	char ch;
	struct no* esq;
	struct no* dir;
}no;

typedef struct elemento{
	char codigo[TAM_PALAVRA_CODIFICADA];
	char ch;
	struct elemento* prox;
}elemento;

typedef struct lista{
	elemento* inicio;
	elemento* fim;
}lista;

void limpa_buffer(){
	char ch;

	while((ch = getchar())!= '\n');
}

no* cria_no(){
	no* raiz = (no*) malloc(sizeof(no));

	if(!raiz)
		return NULL;

	raiz->esq = NULL;
	raiz->dir = NULL;

	return raiz;
}

int folhas(no* raiz){
	if(!raiz)
		return 0;
	if(!raiz->esq && !raiz->dir)
		return 1;
	return folhas(raiz->esq) + folhas(raiz->dir);
}

int altura(no *raiz){
	if(!raiz)
		return 0;

	return 1 + MAIOR(altura(raiz->esq), altura(raiz->dir));
}

int passa_arquivo_chave_para_arvore(char* arquivo_chave, no* raiz){
	FILE* p_arq = fopen(arquivo_chave, "r");
	char ch1, ch2;
	no* raiz_atual = NULL;
	no* no_aux = NULL;

	if(!p_arq)
		return 0;
		
	while((ch1 = fgetc(p_arq)) != EOF){
		/*ler o espaço em branco*/
		ch2 = fgetc(p_arq);	
		raiz_atual = raiz;

		while((ch2 = fgetc(p_arq)) != '\n'){
			if(ch2 == '.'){
				if(!raiz_atual->esq){
					if(!(no_aux = cria_no())){
						fclose(p_arq);
						return 0;
					}
					raiz_atual->esq = no_aux;
				}	
				raiz_atual = raiz_atual->esq;
			}
			if(ch2 == '-'){
				if(!raiz_atual->dir){
					if(!(no_aux = cria_no())){
						fclose(p_arq);
						return 0;
					}
					raiz_atual->dir = no_aux;
				}
				raiz_atual = raiz_atual->dir;
			}
		}
		raiz_atual->ch = ch1;
	}
	
	fclose(p_arq);
	return 1;
}

char acha_na_lista(lista* l, char* str_aux){
	elemento* tmp = l->inicio;

	while(tmp && strcmp(tmp->codigo, str_aux))
		tmp = tmp->prox;
	
	return tmp->ch;

}

int decodifica_usando_lista(lista* l, char* arquivo_codificado){
	FILE* p_arq = fopen(arquivo_codificado, "r");
	char str_aux[TAM_PALAVRA_CODIFICADA];
	char ch_aux;
	int i = 0;
	
	if(!p_arq)
		return 0;
	
	while((ch_aux = fgetc(p_arq)) != EOF){
		switch(ch_aux){
			case '.':   
			case '-': 	str_aux[i++] = ch_aux;
					  	continue;
			case '/': 	fseek(p_arq, 1, SEEK_CUR);
						putchar(' ');
						continue;
			case ' ': 	str_aux[i] = '\0';
						/*printf("%s\n", str_aux);*/
						printf("%c", acha_na_lista(l, str_aux));
						str_aux[0] = '\0';
						i = 0;
			case '\n':  continue;
			default: 	printf("ERRO...Simbolo desconhecido!\n");
						fclose(p_arq);
						return 0;

		}	
	
	}
	puts("");
	fclose(p_arq);
	
	return 1;
}

int mostra_mensagem_codificada(char* arquivo_codificado){
	FILE* p_arq = fopen(arquivo_codificado, "r");
	char ch;

	if(!p_arq)
		return 0;
	
	puts("Mensagem criptografada: \n");
	
	while((ch = fgetc(p_arq)) != EOF){
		putchar(ch);
	}

	fclose(p_arq);
	
	return 1;
}

int decodifica_usando_arvore(no* raiz, char* arquivo_codificado){
	FILE* p_arq = fopen(arquivo_codificado, "r");
	char ch;
	no* raiz_atual = raiz;

	if(!p_arq)
		return 0;

	while((ch = fgetc(p_arq)) != EOF){
		switch(ch){
			case '.': raiz_atual = raiz_atual->esq; continue;
			case '-': raiz_atual = raiz_atual->dir; continue;
			case ' ': putchar(raiz_atual->ch);
					  raiz_atual = raiz; continue;
			case '/': putchar(' ');
						if((ch = fgetc(p_arq)) != ' ')
							fseek(p_arq, -1, SEEK_CUR);
					  raiz_atual = raiz;
			case '\n': continue;
			default:  printf("ERRO! Simbolo desconhecido....\n");
					  fclose(p_arq);
					  return 0;

		}
	}

	puts("");
	fclose(p_arq);
	return 1;
}


void zera_lista(lista* l){
	l->inicio = NULL;
	l->fim = NULL;
} 

/*Retorna o endereço do elemento alocado e NULL se não conseguir alocar espaço de memória*/
elemento* cria_elemento(){

	elemento* tmp = (elemento*) malloc(sizeof(elemento)); 
	
	if(!tmp)
		return NULL;

	tmp->prox = NULL; 

	return tmp;
}

/*Retorna 1 se conseguir adiconar o elemento na fim da lista e zero caso contrário*/
int adiciona_fim_lista(lista* l){
	elemento* tmp;
    
	tmp = cria_elemento();

	if(!tmp)
		return 0;

	if(!l->inicio){
		l->inicio = tmp;
		l->inicio->prox = tmp;
	}
	else
		l->fim->prox = tmp;
    
	l->fim = tmp;

	return 1;
}


int passa_arquivo_chave_para_lista(char* arquivo_chave, lista* l){
	FILE* p_arq = fopen(arquivo_chave, "r");
	char ch;
	int i;

	if(!p_arq)
		return 0;

	while((ch = fgetc(p_arq)) != EOF){
		
		if(!adiciona_fim_lista(l))
			return 0;
		
		l->fim->ch = ch;
		ch = fgetc(p_arq);

		for(i=0; (ch = fgetc(p_arq))!= '\n' && (ch != EOF); ++i){
			l->fim->codigo[i] = ch;

			if(i>=TAM_PALAVRA_CODIFICADA)
				return 0;
		}

		l->fim->codigo[i] = '\0';
	}

	fclose(p_arq);
	return 1;
}

void mostra_lista(lista* l){
	elemento* aux = l->inicio;

	while(aux){
		printf("%c %s\n", aux->ch,aux->codigo);
		
		aux = aux->prox;		
	}
}

int main(int argc, char* argv[]){
	no* raiz;
	lista l;
	char* arquivo_chave = "morse.txt";
	char* arquivo_codificado = "mensagem.txt";
	clock_t tempo_medido;

	raiz = cria_no();

	if(!raiz){	
		puts("Não foi possível alocar espaço de memória.");
		return EXIT_FAILURE;
	}

	if(!passa_arquivo_chave_para_arvore(arquivo_chave, raiz)){
		printf("Não foi possível passar o arquivo \'%s\' para a árvore.\n", arquivo_chave);
		return EXIT_FAILURE;
	}
	
	if(!mostra_mensagem_codificada(arquivo_codificado)){
		printf("Não foi possível mostrar o arquivo \"%s\"", arquivo_codificado);
		return EXIT_FAILURE;
	}
	
	zera_lista(&l);
	if(!passa_arquivo_chave_para_lista(arquivo_chave, &l)){
		printf("Não foi possível passar o arquivo \'%s\' para a lista.\n", arquivo_chave);
		return EXIT_FAILURE;
	}

	puts("\n# Decodificação pela árvore.\n");
	tempo_medido = clock();

	if(!decodifica_usando_arvore(raiz, arquivo_codificado)){
		printf("Erro ao tentar decodificar o arquivo \"%s\" usando árvore.", arquivo_codificado);
		return EXIT_FAILURE;
	}	

	tempo_medido = clock() - tempo_medido;
	printf("Tempo demorado usando árvore: %.06lf segundos.\n", ((float)tempo_medido)/CLOCKS_PER_SEC);

	
	puts("\n# Decodificação pela lista.\n");
	tempo_medido = clock();
	/*mostra_lista(&l);*/

	if(!decodifica_usando_lista(&l, arquivo_codificado)){
		printf("Erro ao tentar decodificar o arquivo \"%s\" usando a lista.", arquivo_codificado);
		return EXIT_FAILURE;
	}

	tempo_medido = clock() - tempo_medido;
	printf("Tempo demorado usando lista: %.06lf segundos.\n", ((float)tempo_medido)/CLOCKS_PER_SEC);

	puts("\n\n\t## Informações da árvore ##\n");
	printf(">>Nº de folhas: %i\n", folhas(raiz));
	printf(">>Altura: %i\n", altura(raiz));

	return EXIT_SUCCESS;
}