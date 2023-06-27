#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "timer.h"

#define TAMANHO 400000

typedef struct {
    int* array;
    int inicio;
    int fim;
} tArgs;

void merge(int arr[], int esq, int meio, int dirt) {
    int i, j, k;
    int n1 = meio - esq + 1;
    int n2 = dirt - meio;

    int* esqArr = (int*)malloc(n1 * sizeof(int));
    int* dirtArr = (int*)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        esqArr[i] = arr[esq + i];
    for (j = 0; j < n2; j++)
        dirtArr[j] = arr[meio + 1 + j];

    i = 0;
    j = 0;
    k = esq;

    while (i < n1 && j < n2) {
        if (esqArr[i] <= dirtArr[j]) {
            arr[k] = esqArr[i];
            i++;
        } else {
            arr[k] = dirtArr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = esqArr[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = dirtArr[j];
        j++;
        k++;
    }

    free(esqArr);
    free(dirtArr);
}

void mergeSort(int arr[], int esq, int dirt) {
    if (esq < dirt) {
        int meio = esq + (dirt - esq) / 2;

        mergeSort(arr, esq, meio);
        mergeSort(arr, meio + 1, dirt);

        merge(arr, esq, meio, dirt);
    }
}

void* mergeSortThread(void* arguments) {
    tArgs* args = (tArgs*)arguments;

    int* arr = args->array;
    int esq = args->inicio;
    int dirt = args->fim;

    mergeSort(arr, esq, dirt);

    pthread_exit(NULL);
}

void mergeArrays(int arr[], int inicio, int meio, int fim) {
    int i, j, k;
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;

    int* temp1 = (int*)malloc(n1 * sizeof(int));
    int* temp2 = (int*)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        temp1[i] = arr[inicio + i];
    for (j = 0; j < n2; j++)
        temp2[j] = arr[meio + 1 + j];

    i = 0;
    j = 0;
    k = inicio;

    while (i < n1 && j < n2) {
        if (temp1[i] <= temp2[j]) {
            arr[k] = temp1[i];
            i++;
        } else {
            arr[k] = temp2[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = temp1[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = temp2[j];
        j++;
        k++;
    }

    free(temp1);
    free(temp2);
}

void mergeSortConcorrente(int arr[], int n, int numThreads) {

    pthread_t threads[numThreads];
    tArgs args[numThreads];

    int partTam = n / numThreads;
    int extraTam = n % numThreads;

    int i;
    for (i = 0; i < numThreads; i++) {
        int inicio = i * partTam;
        int fim = inicio + partTam - 1;

        if (i == numThreads - 1)
            fim += extraTam;

        args[i].array = arr;
        args[i].inicio = inicio;
        args[i].fim = fim;

        pthread_create(&threads[i], NULL, mergeSortThread, (void*)&args[i]);
    }

    for (i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    int passo = partTam;
    while (passo < n) {
        for (i = 0; i < n; i += 2 * passo) {
            int inicio = i;
            int meio = i + passo - 1;
            int fim = (i + 2 * passo - 1 < n) ? i + 2 * passo - 1 : n - 1;

            mergeArrays(arr, inicio, meio, fim);
        }
        passo *= 2;
    }
}

void printArrayExtr(int arr[], int size) {
    printf("Menor valor: %d ; Maior valor: %d\n", arr[0], arr[size - 1]);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Digite: %s <Nome do Arquivo>\n", argv[0]);
        return 1;
    }
    if (argc < 3) {
        printf("Digite: %s <Nome do Arquivo> <Numero de Threads>\n", argv[0]);
        return 1;
    }

    // inicializamos as variaveis do sistema 
    int n, numThreads;
    double tiinicio, tifim, tidelta;
    double tinicio, tfim, tdelta;
    double tcinicio, tcfim, tcdelta;

    GET_TIME(tiinicio); //Tomada de tempo da inicializacao

    numThreads = atoi(argv[2]); // Numeros threads 

    char nomArq[100];
    strncpy(nomArq, argv[1], sizeof(nomArq) - 1);
    nomArq[sizeof(nomArq) - 1] = '\0';
    size_t strTam = strlen(nomArq);
    memmove(nomArq, nomArq + 5, strTam - 4);
    nomArq[strTam - 4] = '\0';
    n = atoi(nomArq); // Tamanho do array extraido 

    //Inicializamos os arrays 
    int* arrx = (int*)malloc(TAMANHO * sizeof(int));
    int* arry = (int*)malloc(TAMANHO * sizeof(int));
    int* arrz = (int*)malloc(TAMANHO * sizeof(int));
    
    int* arrxConc = (int*)malloc(TAMANHO * sizeof(int));
    int* arryConc = (int*)malloc(TAMANHO * sizeof(int));
    int* arrzConc = (int*)malloc(TAMANHO * sizeof(int));
    
    // Verifica a alocação de memória dos arrays
    if (arrx == NULL || arry == NULL || arrz == NULL || arrxConc == NULL || arryConc == NULL || arrzConc == NULL) {
        printf("Falha na alocação de memória.\n");
        return 1;
    }
    // Verifica o tamanho do array, medida de protecao 
    if (n > TAMANHO || n <= 0) {
        printf("Tamanho inválido. O tamanho deve estar entre 1 e %d.\n", TAMANHO);
        return 1;
    }
    printf("Tamanho do array: %d\n", n);

    // Copiamos as posicoes para os arrays
    int ind=0;

    FILE *file = fopen(argv[1], "r"); //Abre arquivo em modo de leitura 
    int x, y, z;
    while (fscanf(file, "%d;%d;%d", &x, &y, &z) == 3) {
        arrx[ind]= x;
        arry[ind] = y;
        arrz[ind] = z;
        arrxConc[ind]= x;
        arryConc[ind] = y;
        arrzConc[ind] = z;
        ind++;
    }
    
    GET_TIME(tifim); //Fim da tomada de tempo da inicializacao
    tidelta = tifim - tiinicio; //Tomada de tempo total  da inicializacao
    printf("Tempo de inicializacao : %lf segundos\n", tidelta);

    //Iniciamos os algoritmos
    printf("Executando merge sort...\n");
    // A solucao concorrente so e acionada quando ha mais de 1 thread 
    if(numThreads > 1){
        printf("\n Merge sort concorrente \n");
        GET_TIME(tcinicio);
        mergeSortConcorrente(arrxConc, n, numThreads);
        mergeSortConcorrente(arryConc, n, numThreads);
        mergeSortConcorrente(arrzConc, n, numThreads);
        GET_TIME(tcfim);
        tcdelta = tcfim - tcinicio;

        printf("Arrays ordenados:\n");
        printArrayExtr(arrxConc, n);
        printArrayExtr(arryConc, n);
        printArrayExtr(arrzConc, n);

        printf("Tempo de execução Concorrente: %lf segundos\n", tcdelta);
        
    }
    // Execucao sequencial padrao do merge sort
    printf("\n Merge sort sequencial\n");

    GET_TIME(tinicio);
    mergeSort(arrx, 0, n - 1);
    mergeSort(arry, 0, n - 1);
    mergeSort(arrz, 0, n - 1);
    GET_TIME(tfim);
    tdelta = tfim - tinicio;

    printf("Array ordenado:\n");
    printArrayExtr(arrx, n);
    printArrayExtr(arry, n);
    printArrayExtr(arrz, n);
    
    printf("Tempo de execução: %lf segundos\n", tdelta);

    //Liberação da memória
    free(arrx);
    free(arry);
    free(arrz);
    free(arrxConc);
    free(arryConc);
    free(arrzConc);

    return 0;
}   
