#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double dist(double X, double Y){
    double valor = sqrt( pow(X,2) ) + sqrt( pow(Y,2) );
    return valor;
}

int main(int argc, char *argv[]) {

    char arqCresc[100];
    char arqDesc[100];

    //recebe os argumentos de entrada
    if (argc < 2) {
        printf("Digite: %s <Tamanho>\n",argv[0]);
        return 1;
    }

    sprintf(arqCresc, "cresc_%s.txt", argv[1]);
    sprintf(arqDesc, "desc_%s.txt", argv[1]);

    FILE *file = fopen(arqCresc, "w"); // Abre arquivo em modo de escrita 
    FILE *fileD = fopen(arqDesc, "w"); // Abre arquivo em modo de escrita 

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int size = atoi(argv[1]);

    // Preenche o arquivo com as posições
    for(int i=0; i < size; i++ ){
        fprintf(fileD, "%d;%d;%.0f\n", (size-1)-i, (size-1)-i, dist((size-1)-i,(size-1)-i));
        fprintf(file, "%d;%d;%.0f\n", i, i, dist(i,i));
    } 

    fclose(file); // Fecha o arquivo
    fclose(fileD); // Fecha o arquivo

    printf("OS dados foram gravados no arquivo.\n");

    return 0;
}
