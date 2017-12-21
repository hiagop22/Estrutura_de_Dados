/*  @file: Clona_arquivo.c
*   @author: Hiago dos Santos (hiagop22@gmail.com)
*
*   Recebe o nome de um arquivo como parâmetro e faz uma cópia dele, sendo que
*   copia também as suas permissões. Logo em seguida, pergunta ao usuário se
*   deseja remover o arquivo original
*/


#include <stdio.h>
#include <stdlib.h>

typedef struct{
	unsigned char diretorio;
	char dono[3];
	char grupos[3];
	char outros[3];
}permissoes;

char* remove_char(char* s, char ch){
	int i, j;

	for(i=j=0; s[i]; ++i)
		if(s[i] != ch)
			s[j++] = s[i];

	s[j] = '\0';

	return s;
}

int verifica_se_arquivo_existe(char* arquivo){
	FILE* p_arq = fopen(arquivo, "r");

	if(!p_arq)
		return 0;

	fclose(p_arq);
	return 1;
}

int verifica_se_deseja_substituir(char* arquivo){
	printf("\"%s\" já existe, deseja substituí-lo? (s/n)\n", arquivo);

	switch(getchar()){
		case 's':
		case 'S': return 1;
	}

	return 0;
}

int clona_arquivo(char* original, char* copia){
	FILE* p_orig = fopen(original, "r");
	FILE* p_dest = fopen(copia, "w");
	char ch;

	if(!p_orig || !p_dest)
		return 0;

	while(fscanf(p_orig, "%c", &ch) != EOF)
		fprintf(p_dest, "%c", ch);

	fclose(p_orig);
	fclose(p_dest);

	return 1;
}

permissoes le_permissoes(char* arquivo){
	FILE* p_arq = fopen("saida.txt", "w+");
	char comando[200];
	permissoes s;

	sprintf(comando, "ls -lt '%s' > saida.txt", arquivo);
	system(comando);

	fread(&s, sizeof(s), 1, p_arq);
	system("rm -r saida.txt");

	fclose(p_arq);

	return s;
}

int compara_permissoes(permissoes s1, permissoes s2){

	return ((s1.dono != s2.dono)&&(s1.grupos != s2.grupos)&&(s1.outros != s2.outros)) ? 0 : 1;
}

int iguala_permissoes(permissoes s, char* arquivo){
	char dono[4], grupos[4], outros[4];
	char comando[400];

	sprintf(dono, "%.3s", s.dono);
	sprintf(grupos, "%.3s", s.grupos);
	sprintf(outros, "%.3s", s.outros);

	remove_char(dono, '-');
	remove_char(grupos, '-');
	remove_char(outros, '-');

	sprintf(comando, "chmod u+%s '%s'; chmod g+%s '%s'; chmod o+%s '%s'", dono, arquivo, grupos, arquivo, outros, arquivo);

	return system(comando);
}

int main(int argc, char* argv[]){
	char *original, *copia;
	permissoes s1, s2;

	if(argc < 3){
		printf("Informe o nome do arquivo original e o da copia.\n");
		return EXIT_FAILURE;
	}

	original = argv[1];
	copia = argv[2];
	if(!verifica_se_arquivo_existe(original)){
		printf("Não foi possível encontrar ou ler \"%s\".\n", original);
		return EXIT_FAILURE;
	}

	s1 = le_permissoes(original);
	if(s1.diretorio =='t'){
		printf("Não é possível fazer uma clonagem do diretorio \"%s\", apenas arquivos.\n", original);
		return EXIT_FAILURE;
	}

	if(verifica_se_arquivo_existe(copia))
		if(!verifica_se_deseja_substituir(copia))
			return EXIT_FAILURE;

	if(!clona_arquivo(original, copia)){
		printf("Não foi possível fazer uma cópia de %s com o nome %s.\n", original, copia);
			return EXIT_FAILURE;
		}


	s2 = le_permissoes(copia);

	if(!compara_permissoes(s1, s2))
		if(iguala_permissoes(s1, copia)){
			printf("Não foi possível igualar as permissões de \"%s\" com as do arquivo original, \"%s\".\n", original, copia);
			return EXIT_FAILURE;
		}

	printf("Arquivos clonados com sucesso!\n");

	return EXIT_SUCCESS;

	}