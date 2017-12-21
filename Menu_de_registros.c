/*  @file: Menu_de_registros.c
*   @author: Hiago dos Santos (hiagop22@gmail.com)
*
*   Menu de registros que guarda em um arquivo binário as respectivas informações
*   do usuário:
*   nome, idade, salário, data de nascimento e se o usuário foi removido ou não
*
*   Sendo que, o menu oferece as opções de realizar uma busca por idade ou por nome,
*   e também de remover um usuário cadastrado
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 30
#define TAM_MES 4

typedef struct{
	int dia;
	char mes[TAM_MES]; /*JAN, FEV, MAR, ABR, MAI, JUN....*/
	int ano;
}DATA;

typedef struct s_pesssoa{
	char nome[TAM_NOME];
	int idade;
	float salario;
	DATA dt_nasc;
	int removido;/*caso tenha o número 1 indica que o registro foi apagado*/
}PESSOA;

void limpa_tela(){

	system("clear || cls");
}

int compara_strings(char* str1, char* str2, int n){
	int i;

	for(i =0; (str1[i] || str2[i]) && i<n; ++i)
		if(str1[i] != str2[i])
			return 0;

	return 1;
}

void limpa_buffer(){

	while(getchar() != '\n');
}

int verifica_arquivo(char* arquivo){
	FILE* fp = fopen(arquivo, "rb");

	if(!fp)
		return 0;

	fclose(fp);
	return 1;
}

int cria_arquivo(char* arquivo){
	FILE* fp = fopen(arquivo, "wb");

	if(!fp)
		return 0;

	fclose(fp);
	return 1;
}

void mostrar_pessoa(PESSOA x){

	printf("1. Nome: %s\n", x.nome);
	printf("2. Idade: %i anos\n", x.idade);
	printf("3. Salário: R$ %.2f\n", x.salario);
	printf("4. Data de nascimento: %02i/%.3s/%04i \n", x.dt_nasc.dia, x.dt_nasc.mes, x.dt_nasc.ano);
}

int inserir_registro(char* arquivo){
	PESSOA x;
	FILE* fp = fopen(arquivo, "ab");

	if(!fp)
		return 0;

	limpa_tela();

	puts(">> Novo registro: (\".\" para cancelar) \n");
	printf("Qual o nome: ");
	scanf(" %29[^\n]", x.nome);

	if(compara_strings(".", x.nome, strlen("."))){
		fclose(fp);
		return 1;
	}

	printf("Qual a idade: ");
	scanf("%i", &x.idade);

	printf("Qual o salário: ");
	scanf("%f", &x.salario);

	printf("Qual a data de nascimento: ");
	scanf("%i %3s %i", &x.dt_nasc.dia, x.dt_nasc.mes, &x.dt_nasc.ano);
	x.removido = 0 ;
	fwrite(&x, sizeof(x), 1, fp);

	fclose(fp);

	printf("Registro inserirido com sucesso, aperte <ENTER> para voltar ao menu...\n");
	getchar();

	return 1;
}

void layout_registro(){
	char linha_horizontal[] = "+------------------------------------------------------------+" ;

	puts(linha_horizontal);
	puts("|  N° do |                                                   |");
	puts("|registro|               PESSOA REGISTRADA                   |");
	puts(linha_horizontal);
}

int alterar_registro(char* arquivo){
	PESSOA x;
	char opcao;
	FILE* fp = fopen(arquivo, "rb+");
	int n_registro = 1, i = 1 , status;

	if(!fp){
		printf("Não foi possível abrir ou ler o arquivo \"%s\".\n", arquivo);
		return 0;
	}

	limpa_tela();
	layout_registro();

	while(fread(&x, sizeof(x), 1, fp)){
		if(!x.removido)/*indica que o registro não foi apagado*/
			printf("    %02i   :  %s\n", n_registro++, x.nome);
	}

	printf("\n>>N° do registro que deseja alterar (\"0\" para cancelar): ");
	scanf(" %i", &n_registro);

	if(!n_registro)
		return 1;

	rewind(fp);

	do{
		status = fread(&x, sizeof(x), 1, fp);
		if(x.removido)
			--i;
	}while((i++)<n_registro && (status  == 1));

	if(status == 1){
		fseek(fp, -sizeof(x), SEEK_CUR);
		do{
			limpa_tela();

			printf("Escolha o n° que está relacionado com a informação que deseja alterar.\n\n");
			mostrar_pessoa(x);
			printf("5. Nada\n\n");
			printf(">> N° escolhido: ");
			scanf(" %c", &opcao);
			} while( opcao<'1' || opcao>'5');

			switch(opcao){
				case '1': printf("Novo nome: ");
						  scanf(" %29[^\n]", x.nome);
						  break;
				case '2': printf("Nova idade: ");
						  scanf("%i", &x.idade);
						  break;
				case '3': printf("Novo salário: ");
						  scanf("%f", &x.salario);
						  break;
				case '4': printf("Nova data de nascimento: ");
						  scanf ("%2i %3s %4i", &x.dt_nasc.dia, x.dt_nasc.mes, &x.dt_nasc.ano);
						  break;
				case '5': break;
			}
			fwrite(&x, sizeof(x), 1, fp);

			limpa_buffer();
			printf("\nRegistro alterado com sucesso, aperte <ENTER> para voltar ao menu...\n");
	}

	else{
		limpa_buffer();
		printf("\nNão foi encontrado o registro de N° %i, aperte <ENTER> para voltar ao menu...\n", n_registro);
	}

	fclose(fp);
	getchar();
	return 1;
}

int apagar_registro(char* arquivo){
	PESSOA x;
	int n_registro = 1, status, i =1;
	FILE* fp = fopen(arquivo, "rb+");

	if(!fp)
		return 0;

	limpa_tela();
	layout_registro();

	while(fread(&x, sizeof(x), 1, fp)){
		if(!x.removido)/*indica que o registro não foi apagado*/
			printf("    %02i   :  %s\n", n_registro++, x.nome);
	}

	printf("\n>>N° do registro que você deseja apagar (\"0\" para cancelar): ");
	scanf(" %i", &n_registro);

	if(!n_registro)
		return 1;

	rewind(fp);

	do{
		status = fread(&x, sizeof(x), 1, fp);
		if(x.removido)
			--i;
	}while(i++<n_registro && (status  == 1));

	if(status == 1){

		fseek(fp, -sizeof(x), SEEK_CUR);
		x.removido = 1;
		fwrite(&x, sizeof(x), 1, fp);

		limpa_buffer();
		printf("\nRegistro apagado, aperte <ENTER> para voltar ao menu...\n");
		getchar();
	}

	else{
		limpa_buffer();
		printf("\nNão foi encontrado o registro de N° %i, aperte <ENTER> para voltar ao menu...\n", n_registro);
		getchar();
	}

	fclose(fp);
	return 1;
}

int listar_registros(char* arquivo){
	int i = 1;
	PESSOA x;
	FILE* fp = fopen(arquivo, "rb");

	if(!fp){
		printf("Não foi possível abrir ou ler o arquivo \"%s\".\n", arquivo);
		return 0;
	}

	limpa_tela();

	while(fread(&x, sizeof(x), 1, fp)){
		if(!x.removido){
			printf("#REGISTRO N° %02i\n", i++);
			mostrar_pessoa(x);
			putchar('\n');
		}
	}

	printf("Aperte <ENTER> para voltar ao menu...\n");
	getchar();

	return 1;
}

int pesquisar_por_idades(char* arquivo){
	int i= 0, idade1, idade2;
	PESSOA x;
	FILE* fp = fopen(arquivo, "rb");

	if(!fp)
		return 0;

	limpa_tela();
	printf("Qual o intervalo de idades que deseja pesquisar (\"0 0\" para cancelar): ");
	scanf("%i %i", &idade1, &idade2);

	if(!idade1 && !idade2)
		return 1;

	putchar('\n');
	while(fread(&x, sizeof(x), 1, fp)){
		if(!x.removido && x.idade>=idade1 && x.idade<=idade2){
			printf("#REGISTRO N° %02i\n", ++i);
			mostrar_pessoa(x);
			putchar('\n');
		}
	}
	limpa_buffer();

	if(!i)
		printf("Não foi encontrado algum resultado, aperte <ENTER> para voltar ao menu.\n");
	else
		printf("Aperte <ENTER> para voltar ao menu.\n");

	getchar();
	fclose(fp);
	return 1;
}

int pesquisar_por_nome(char* arquivo){
	char nome[TAM_NOME];
	PESSOA x;
	int i =0;
	FILE* fp = fopen(arquivo, "rb");

	if(!fp)
		return 0;

	limpa_tela();
	printf("Qual o nome da pessoa que deseja procurar (\".\" para cancelar): ");
	scanf(" %29[^\n]", nome);
	putchar('\n');

	if(compara_strings(".", nome, strlen(".")))
		return 1;

	while(fread(&x, sizeof(x), 1, fp)){
		if(!x.removido && compara_strings(nome, x.nome, strlen(nome))){
			printf("#REGISTRO N° %02i\n", ++i);
			mostrar_pessoa(x);
			putchar('\n');
		}
	}
	limpa_buffer();

	if(!i)
		printf("Não foi encontrado algum resultado para \"%s\", aperte <ENTER> para voltar ao menu\n", nome);
	else
		printf("Aperte <ENTER> para voltar ao menu.\n");

	getchar();
	fclose(fp);
	return 1;
}

int pesquisar(char* arquivo){
	int opcao;

	do{
		limpa_tela();

		puts("1. Pesquisas por Intervalo de Idades");
		puts("2. Pesquisar por Nome");
		puts("0. Voltar\n");
		printf(">>Escolha: ");
		scanf("%i", &opcao);
		while(getchar() != '\n');
	}while(opcao<0 || opcao>2);

	switch(opcao){
		case 1: if(!pesquisar_por_idades(arquivo))
					return 0;
				break;
		case 2: if(!pesquisar_por_nome(arquivo))
					return 0;
	}

	return 1;
}

int menu(){
	int opcao;

	do{
		limpa_tela();

		puts("# MENU DE OPÇÕES #\n");
		puts("1. Inserir Registro");
		puts("2. Alterar Registro");
		puts("3. Apagar Registro");
		puts("4. Listar Registros");
		puts("5. Pesquisar");
		puts("0. Sair\n");

		printf(">> Escolha: ");
		scanf("%i", &opcao);

		while(getchar() != '\n');
	}while(opcao<0 || opcao>5);

	return opcao;
}

int main(int argc, char* argv[]){
	char* arquivo = "DADOS.DAT";
	int erro = 0;

	if(!verifica_arquivo(arquivo))
		if(!cria_arquivo(arquivo)){
			printf("Não foi possível criar o arquivo \"%s\".\n", arquivo);
			return EXIT_FAILURE;
		}

do{
	switch(menu()){
		case 0: printf("\nObrigado por usar \"%s\".\n\n", argv[0]+2); return EXIT_SUCCESS;
		case 1: if(!inserir_registro(arquivo))
					++erro;
				break;
		case 2: if(!alterar_registro(arquivo))
					++erro;
				break;
		case 3: if(!apagar_registro(arquivo))
					++erro;
				break;
		case 4: if(!listar_registros(arquivo))
					++erro;
				break;
		case 5: if(!pesquisar(arquivo))
					++erro;
				break;
	}
}while(!erro);

	if(erro)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}