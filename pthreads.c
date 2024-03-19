#define HAVE_STRUCT_TIMESPEC

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define ARRAY_SIZE 500000
#define N_BUCKETS 10
#define RANGE_START 100
#define RANGE_END 999
#define NUM_THREADS N_BUCKETS

int A[ARRAY_SIZE];
int buckets[N_BUCKETS][ARRAY_SIZE];
int bucketCounts[N_BUCKETS] = { 0 };

void insertionSort(int* arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void* sortBucket(void* arg) {
    int bucketIndex = *((int*)arg);
    insertionSort(buckets[bucketIndex], bucketCounts[bucketIndex]);
    pthread_exit(NULL);
    return NULL;
}

int main() {
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        A[i] = RANGE_START + rand() % (RANGE_END - RANGE_START + 1);
    }

    for (int i = 0; i < ARRAY_SIZE; i++) {
        int bucketIndex = (A[i] - RANGE_START) * N_BUCKETS / (RANGE_END - RANGE_START + 1);
        if (bucketIndex >= N_BUCKETS) bucketIndex = N_BUCKETS - 1;
        buckets[bucketIndex][bucketCounts[bucketIndex]++] = A[i];
    }

    clock_t start = clock();

    pthread_t threads[NUM_THREADS];
    int indices[NUM_THREADS]; 
    for (int i = 0; i < N_BUCKETS; i++) {
        indices[i] = i;
        pthread_create(&threads[i], NULL, sortBucket, (void*)&indices[i]);
    }

    for (int i = 0; i < N_BUCKETS; i++) {
        pthread_join(threads[i], NULL);
    }

    int k = 0;
    for (int i = 0; i < N_BUCKETS; i++) {
        for (int j = 0; j < bucketCounts[i]; j++) {
            A[k++] = buckets[i][j];
        }
    }
    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Algorithm execution took %f seconds.\n", cpu_time_used);

    return 0;
}
