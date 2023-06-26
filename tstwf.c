#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double dist(double X, double Y){
    double valor = sqrt( pow(X,2) ) + sqrt( pow(Y,2) );
    return valor;
}
int Passo(int valor){
    srand(time(NULL));
    int NumRand = rand();
    int resultado = (pow(-1,NumRand)) + valor;
    return valor;
}

int main(int argc, char *argv[]) {

    char arqCresc[100];
    char arqDesc[100];
    char arqPadr[100];

    //recebe os argumentos de entrada
    if (argc < 2) {
        printf("Digite: %s <Tamanho>\n",argv[0]);
        return 1;
    }

    sprintf(arqCresc, "cres_%s.txt", argv[1]);
    sprintf(arqDesc, "desc_%s.txt", argv[1]);
    sprintf(arqPadr, "padr_%s.txt", argv[1]);

    FILE *file = fopen(arqCresc, "w"); // Abre arquivo de calibracao crescente em modo de escrita 
    FILE *fileD = fopen(arqDesc, "w"); // Abre arquivo calibracao decrescente em modo de escrita 
    FILE *fileN = fopen(arqPadr, "w"); // Abre arquivo padrao em modo de escrita 

    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    int size = atoi(argv[1]);

    int pos1,pos2 = 0;

    // Preenche o arquivo com as posições
    for(int i=0; i < size; i++ ){
        pos1 = Passo(pos1);
        pos2 = Passo(pos2);
        fprintf(fileD, "%d;%d;%.0f\n", (size-1)-i, (size-1)-i, dist((size-1)-i,(size-1)-i)); //Descrescente
        fprintf(file, "%d;%d;%.0f\n", i, i, dist(i,i)); //Crescente
    } 

    fclose(file); // Fecha o arquivo
    fclose(fileD); // Fecha o arquivo
    fclose(fileN); // Fecha o arquivo

    printf("Data written to file successfully.\n");

    return 0;
}