#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Funções auxiliares

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void copyArray(int* origem, int* destino, int n) {
    for (int i = 0; i < n; i++) {
        destino[i] = origem[i];
    }
}

void printArray(int* A, int n) {
    int max_print = (n < 10) ? n : 10;
    for (int i = 0; i < max_print; i++) {
        printf("%d ", A[i]);
    }
    if (n > 10) {
        printf("... (mais %d elementos)", n - 10);
    }
    printf("\n");
}

// Geradores de vetores

void generateRandomArray(int* A, int n) {
    for (int i = 0; i < n; i++) {
        A[i] = rand() % (n * 10);
    }
}

void generateSortedArray(int* A, int n) {
    for (int i = 0; i < n; i++) {
        A[i] = i;
    }
}

void generateReverseSortedArray(int* A, int n) {
    for (int i = 0; i < n; i++) {
        A[i] = n - 1 - i;
    }
}

// Insertion Sort 

void insertionSort(int *A, int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = A[i];
        j = i - 1;
        while (j >= 0 && A[j] > key) {
            A[j + 1] = A[j];
            j = j - 1;
        }
        A[j + 1] = key;
    }
}

// Merge Sort

void merge(int* A, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = A[l + i];
    for (j = 0; j < n2; j++)
        R[j] = A[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            A[k] = L[i];
            i++;
        } else {
            A[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        A[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        A[k] = R[j];
        j++;
        k++;
    }
    
    free(L);
    free(R);
}

void mergeSortRecursive(int* A, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortRecursive(A, l, m);
        mergeSortRecursive(A, m + 1, r);
        merge(A, l, m, r);
    }
}

void mergeSort(int *A, int n) {
    mergeSortRecursive(A, 0, n - 1);
}

//Quick Sort com Pivô Aleatório

/**
 * Escolhe um pivô aleatório e coloca na última posição.
 * Isso evita o pior caso O(n²) em vetores já ordenados.
 */
int partitionRandomized(int* A, int low, int high) {
    // Escolhe um índice aleatório entre low e high
    int randomIndex = low + rand() % (high - low + 1);
    
    // Troca o elemento aleatório com o último
    swap(&A[randomIndex], &A[high]);
    
    // Agora faz a partição normal usando o último como pivô
    int pivot = A[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (A[j] <= pivot) {
            i++;
            swap(&A[i], &A[j]);
        }
    }
    swap(&A[i + 1], &A[high]);
    return (i + 1);
}

void quickSortRecursive(int* A, int low, int high) {
    if (low < high) {
        int pi = partitionRandomized(A, low, high);
        quickSortRecursive(A, low, pi - 1);
        quickSortRecursive(A, pi + 1, high);
    }
}

/**
 * Ordena o vetor A[0..n-1] usando Quick Sort com pivô aleatório.
 * Complexidade:
 * Melhor Caso: O(n log n)
 * Médio Caso: O(n log n)
 * Pior Caso: O(n²) - MAS muito improvável com pivô aleatório
 */
void quickSort(int *A, int n) {
    quickSortRecursive(A, 0, n - 1);
}

// FUNÇÃO DE TESTE

void runSortTest(void (*sortFunction)(int*, int), int* A, int* original, int n, const char* algorithmName) {
    copyArray(original, A, n);

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    sortFunction(A, n);
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("  %-15s: %f segundos\n", algorithmName, cpu_time_used);
}

int main() {
    srand(time(NULL));

    int sizes[] = {10000, 50000, 100000, 200000, 250000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        printf("\n============================================\n");
        printf("Iniciando testes para n = %d\n", n);
        printf("============================================\n");

        int* original = (int*)malloc(n * sizeof(int));
        int* A = (int*)malloc(n * sizeof(int));

        if (original == NULL || A == NULL) {
            printf("Erro ao alocar memória.\n");
            return 1;
        }

        // --- Teste 1: Vetor Aleatório ---
        printf("\n--- Teste 1: Vetor Aleatório ---\n");
        generateRandomArray(original, n);
        
        runSortTest(insertionSort, A, original, n, "Insertion Sort");
        runSortTest(mergeSort, A, original, n, "Merge Sort");
        runSortTest(quickSort, A, original, n, "Quick Sort"); 

        // --- Teste 2: Vetor Crescente ---
        printf("\n--- Teste 2: Vetor Crescente (Melhor Caso) ---\n");
        generateSortedArray(original, n);
        
        runSortTest(insertionSort, A, original, n, "Insertion Sort");
        runSortTest(mergeSort, A, original, n, "Merge Sort");
        runSortTest(quickSort, A, original, n, "Quick Sort");

        // --- Teste 3: Vetor Decrescente ---
        printf("\n--- Teste 3: Vetor Decrescente (Pior Caso) ---\n");
        generateReverseSortedArray(original, n);
        
        runSortTest(insertionSort, A, original, n, "Insertion Sort");
        runSortTest(mergeSort, A, original, n, "Merge Sort");
        runSortTest(quickSort, A, original, n, "Quick Sort");

        free(original);
        free(A);
    }

    return 0;
}