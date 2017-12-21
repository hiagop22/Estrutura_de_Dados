/*	@file: patinhos.c
* 	@author: Hiago dos Santos (hiagop22@gmail.com)
*
* 	Subtração de valores de 20 algarismos, tomados como strings
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int condicao_de_parada(char* num_patos){

    return num_patos[0] == '-' ? 1 : 0 ;
}

void apaga_zeros_desnecessarios(char* num_patos){
	int i, j;

	while(1){
		if(strlen(num_patos) == 1 || num_patos[0] != '0')
			break;

		for(i=0, j = 1; num_patos[i] != '\0'; ++j, ++i)
			num_patos[i] = num_patos[j];
	}
}

void subtrai_um(char* num_patos){
	int pos = strlen(num_patos) - 1;

	if(num_patos[0] == '0' && num_patos[1] == '\0')
		return;

	while(pos>=0){
    	if(num_patos[pos] == '0'){
            num_patos[pos] = '9';
            --pos;
        }
        else{
            num_patos[pos] = num_patos[pos] - 1 ;
            break;
        }
    }

    apaga_zeros_desnecessarios(num_patos);
}

int main(int argc, char** argv) {
	char num_patos[21];

    do{
      scanf(" %20[^\n]", num_patos);
      if(condicao_de_parada(num_patos))
        break;

      subtrai_um(num_patos);
      printf("%s\n", num_patos);
    }
    while(1);

	return EXIT_SUCCESS;
}
