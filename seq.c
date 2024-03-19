#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 500000 
#define N_BUCKETS 10      
#define RANGE_START 100   
#define RANGE_END 999     

void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}


int A[ARRAY_SIZE]; 
int buckets[N_BUCKETS][ARRAY_SIZE]; 
int main() {
    int bucketCounts[N_BUCKETS] = { 0 }; 
    int range = RANGE_END - RANGE_START + 1;
    int bucketRange = range / N_BUCKETS;
    int i, j;
    clock_t start, end; 
    double cpu_time_used;
    srand(time(NULL));

    for (i = 0; i < ARRAY_SIZE; i++) {
        A[i] = RANGE_START + rand() % range;
    }
    start = clock(); 
    for (i = 0; i < ARRAY_SIZE; i++) {
        int bucketIndex = (A[i] - RANGE_START) / bucketRange;
        if (bucketIndex >= N_BUCKETS) {
            bucketIndex = N_BUCKETS - 1;
        }
        buckets[bucketIndex][bucketCounts[bucketIndex]++] = A[i];
    }

    for (i = 0; i < N_BUCKETS; i++) {
        insertionSort(buckets[i], bucketCounts[i]);
    }
    int k = 0;
    for (i = 0; i < N_BUCKETS; i++) {
        for (j = 0; j < bucketCounts[i]; j++) {
            A[k++] = buckets[i][j];
        }
    }

    end = clock(); 
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; 

    printf("Algorithm execution took %f seconds.\n", cpu_time_used);

    
    return 0;
}