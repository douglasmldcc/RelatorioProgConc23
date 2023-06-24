#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Digite: %s <Nome do Arquivo>\n",argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r"); //Abre arquivo em modo de leitura 
    int x, y, z;
    while (fscanf(file, "%d;%d;%d", &x, &y, &z) == 3) {
        printf("X: %d, Y: %d, Z: %d\n", x, y, z);
    }

    if (!feof(file)) {
        printf("Formato Invalido.\n");
    }

    fclose(file); // Fecha o arquivo

    return 0;
}