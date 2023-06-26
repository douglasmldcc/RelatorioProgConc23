Aqui está o código modificado, com a inclusão do mutex para evitar a seção crítica:

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define TAMANHO 400000

typedef struct {
    int* array;
    int inicio;
    int fim;
    pthread_mutex_t* mutex;
} ThreadArgs;

void merge(int arr[], int esq, int meio, int dirt) {
    int i, j, k;
    int n1 = meio - esq + 1;
    int n2 = dirt - meio;

    int esqArr[n1], dirtArr[n2];

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
    ThreadArgs* args = (ThreadArgs*)arguments;

    int* arr = args->array;
    int esq = args->inicio;
    int dirt = args->fim;
    pthread_mutex_t* mutex = args->mutex;

    if (esq < dirt) {
        int meio = esq + (dirt - esq) / 2;

        mergeSort(arr, esq, meio);
        mergeSort(arr, meio + 1, dirt);

        pthread_mutex_lock(mutex);
        merge(arr, esq, meio, dirt);
        pthread_mutex_unlock(mutex);
    }

    pthread_exit(NULL);
}

void mergeSortConcorrente(int arr[], int n, int numThreads) {
    if (numThreads <= 1) {
        mergeSort(arr, 0, n - 1);
        return;
    }

    pthread_t threads[numThreads];
    ThreadArgs args[numThreads];
    pthread_mutex_t mutex;

    int partTam = n / numThreads;
    int extraTam = n % numThreads;

    pthread_mutex_init(&mutex, NULL);

    int i;
    for (i = 0; i < numThreads; i++) {
        int inicio = i * partTam;
        int fim = inicio + partTam - 1;

        if (i == numThreads - 1)
            fim += extraTam;

        args[i].array = arr;
        args[i].inicio = inicio;
        args[i].fim = fim;
        args[i].mutex = &mutex;

        pthread_create(&threads[i], NULL, mergeSortThread, (void*)&args[i]);
    }

    for (i = 0;

 i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    int passo = partTam;
    while (passo < n) {
        for (i = 0; i < n; i += 2 * passo) {
            int esq = i;
            int meio = i + passo - 1;
            int dirt = (i + 2 * passo - 1 < n) ? i + 2 * passo - 1 : n - 1;

            merge(arr, esq, meio, dirt);
        }
        passo *= 2;
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void printArrayExtr(int arr[], int size) {
    printf("Menor valor: %d ; Maior valor : %d \n", arr[0], arr[size - 1]);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Digite: %s <Nome do Arquivo>\n", argv[0]);
        return 1;
    }

    int* arr = (int*)malloc(TAMANHO * sizeof(int));
    int* arry = (int*)malloc(TAMANHO * sizeof(int));
    int* arrz = (int*)malloc(TAMANHO * sizeof(int));

    if (arr == NULL || arry == NULL || arrz == NULL) {
        printf("Falha na alocação de memória.\n");
        return 1;
    }

    int n, numThreads;
    char nomArq[100];
    strncpy(nomArq, argv[1], sizeof(nomArq) - 1);
    nomArq[sizeof(nomArq) - 1] = '\0';
    size_t strTam = strlen(nomArq);
    memmove(nomArq, nomArq + 5, strTam - 4);
    nomArq[strTam - 4] = '\0';
    n = atoi(nomArq);

    printf("Tamanho do array: %d\n", n);

    if (n > TAMANHO || n <= 0) {
        printf("Tamanho inválido. O tamanho deve estar entre 1 e %d.\n", TAMANHO);
        return 1;
    }

    int ind = 0;
    FILE* file = fopen(argv[1], "r");
    int x, y, z;
    while (fscanf(file, "%d;%d;%d", &x, &y, &z) == 3) {
        arr[ind] = x;
        arry[ind] = y;
        arrz[ind] = z;
        ind++;
    }

    printf("Digite o número de threads: ");
    scanf("%d", &numThreads);

    if (numThreads <= 1) {
        printf("Merge Sort Sequencial\n");
    } else {
        printf("Merge Sort Concorrente\n");
    }

    mergeSortConcorrente(arr, n, numThreads);
    mergeSortConcorrente(arry, n, numThreads);
    mergeSortConcorrente(arrz, n, numThreads);

    printf("Array Ordenado X:\n");
    printArrayExtr(arr, n);
    printf("Array Ordenado Y:\n");
    printArrayExtr(arry, n);
    printf("Array Ordenado Z:\n");
    printArrayExtr(arrz

, n);

    free(arr);
    free(arry);
    free(arrz);

    return 0;
}
