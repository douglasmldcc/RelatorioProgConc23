#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100000

typedef struct {
    int* array;
    int start;
    int end;
} ThreadArgs;

void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int leftArray[n1], rightArray[n2];

    for (i = 0; i < n1; i++)
        leftArray[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        rightArray[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (leftArray[i] <= rightArray[j]) {
            arr[k] = leftArray[i];
            i++;
        } else {
            arr[k] = rightArray[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = leftArray[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = rightArray[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void* mergeSortThread(void* arguments) {
    ThreadArgs* args = (ThreadArgs*)arguments;

    int* arr = args->array;
    int left = args->start;
    int right = args->end;

    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }

    pthread_exit(NULL);
}

void mergeSortConcurrent(int arr[], int n, int numThreads) {
    if (numThreads <= 1) {
        mergeSort(arr, 0, n - 1);
        return;
    }

    pthread_t threads[numThreads];
    ThreadArgs args[numThreads];

    int segmentSize = n / numThreads;
    int extraSize = n % numThreads;

    int i;
    for (i = 0; i < numThreads; i++) {
        int start = i * segmentSize;
        int end = start + segmentSize - 1;

        if (i == numThreads - 1)
            end += extraSize;

        args[i].array = arr;
        args[i].start = start;
        args[i].end = end;

        pthread_create(&threads[i], NULL, mergeSortThread, (void*)&args[i]);
    }

    for (i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    int step = segmentSize;
    while (step < n) {
        for (i = 0; i < n; i += 2 * step) {
            int left = i;
            int mid = i + step - 1;
            int right = (i + 2 * step - 1 < n) ? i + 2 * step - 1 : n - 1;

            merge(arr, left, mid, right);
        }
        step *= 2;
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int arr[MAX_SIZE];
    int n, numThreads;

    printf("Digite o tamanho do array: ");
    scanf("%d", &n);

    if (n > MAX_SIZE || n <= 0) {
        printf("Tamanho inválido. O tamanho deve estar entre 1 e %d.\n", MAX_SIZE);
        return 1;
    }

    printf("Digite os elementos do array:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Digite o número de threads: ");
    scanf("%d", &numThreads);

    mergeSortConcurrent(arr, n, numThreads);

    printf("Array ordenado:\n");
    printArray(arr, n);

    return 0;
}