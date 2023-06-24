#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Estrutura para representar os argumentos passados para cada thread
typedef struct {
    int* array;
    int start;
    int end;
} ThreadArgs;

// Função para mesclar dois subarrays ordenados
void merge(int arr[], int start, int mid, int end) {
    int leftLength = mid - start + 1;
    int rightLength = end - mid;

    int* leftArray = (int*) malloc(leftLength * sizeof(int));
    int* rightArray = (int*) malloc(rightLength * sizeof(int));

    int i, j, k;

    for (i = 0; i < leftLength; i++)
        leftArray[i] = arr[start + i];
    for (j = 0; j < rightLength; j++)
        rightArray[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = start;

    while (i < leftLength && j < rightLength) {
        if (leftArray[i] <= rightArray[j]) {
            arr[k] = leftArray[i];
            i++;
        } else {
            arr[k] = rightArray[j];
            j++;
        }
        k++;
    }

    while (i < leftLength) {
        arr[k] = leftArray[i];
        i++;
        k++;
    }

    while (j < rightLength) {
        arr[k] = rightArray[j];
        j++;
        k++;
    }

    free(leftArray);
    free(rightArray);
}

// Função recursiva para realizar o Merge Sort
void mergeSort(int arr[], int start, int end) {
    if (start < end) {
        int mid = start + (end - start) / 2;

        // Divisão do array em dois subarrays
        mergeSort(arr, start, mid);
        mergeSort(arr, mid + 1, end);

        // Mescla os subarrays ordenados
        merge(arr, start, mid, end);
    }
}

// Função para criar uma thread para o Merge Sort
void* mergeSortThread(void* args) {
    ThreadArgs* threadArgs = (ThreadArgs*) args;
    int start = threadArgs->start;
    int end = threadArgs->end;
    int* arr = threadArgs->array;

    mergeSort(arr, start, end);

    pthread_exit(NULL);
}

// Função principal
int main() {
    int n;

    printf("Digite o tamanho do vetor: ");
    scanf("%d", &n);

    int* arr = (int*) malloc(n * sizeof(int));

    printf("Digite os elementos do vetor:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    ThreadArgs args = {arr, 0, n - 1};

    // Criação da thread principal para o Merge Sort
    pthread_t tid;
    pthread_create(&tid, NULL, mergeSortThread, &args);

    // Aguarda o término da thread
    pthread_join(tid, NULL);

    printf("\nVetor ordenado:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);

    printf("\n");

    free(arr);

    return 0;
}