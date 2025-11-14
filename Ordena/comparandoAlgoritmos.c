#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ===================================================================
// FUNÇÕES AUXILIARES
// ===================================================================

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

// ===================================================================
// GERADORES DE VETORES
// ===================================================================

int* generateRandomArray(int n) {
    int* A = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        A[i] = rand() % (n * 10);
    }
    return A;
}

int* generateSortedArray(int n) {
    int* A = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        A[i] = i;
    }
    return A;
}

int* generateReverseSortedArray(int n) {
    int* A = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        A[i] = n - 1 - i;
    }
    return A;
}

// ===================================================================
// INSERTION SORT
// ===================================================================

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

// ===================================================================
// MERGE SORT
// ===================================================================

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

// ===================================================================
// QUICK SORT COM PIVÔ ALEATÓRIO
// ===================================================================

int partitionRandomized(int* A, int low, int high) {
    int randomIndex = low + rand() % (high - low + 1);
    swap(&A[randomIndex], &A[high]);
    
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

void quickSort(int *A, int n) {
    quickSortRecursive(A, 0, n - 1);
}

// ===================================================================
// HEAP SORT
// ===================================================================

void maxheapfy(int *A, int m, int i) {
    int e, d, aux, maior;
    e = 2 * i + 1;  // Filho esquerdo
    d = 2 * i + 2;  // Filho direito
    
    // Encontra o maior entre pai, filho esquerdo e direito
    if ((e <= m) && (A[e] > A[i]))
        maior = e;
    else
        maior = i;
    
    if ((d <= m) && (A[d] > A[maior]))  // CORRIGIDO: era d<=e
        maior = d;
    
    // Se o maior não é o pai, troca e continua heapificando
    if (maior != i) {
        aux = A[i];
        A[i] = A[maior];
        A[maior] = aux;
        maxheapfy(A, m, maior);
    }
}

void build_maxheap(int *A, int n) {
    for (int i = n / 2; i >= 0; i--)
        maxheapfy(A, n - 1, i);  // CORRIGIDO: passa n-1 como limite
}

void heapSort(int *A, int n) {
    int m, aux;
    build_maxheap(A, n);
    m = n - 1;
    
    for (int i = n - 1; i >= 1; i--) {
        // Troca o maior (raiz) com o último elemento
        aux = A[0];
        A[0] = A[i];
        A[i] = aux;
        m--;
        maxheapfy(A, m, 0);
    }
}

// ===================================================================
// MAIN - COM PARÂMETROS CONFIGURÁVEIS
// ===================================================================

int main() {
    srand(time(NULL));

    // ===== CONFIGURAÇÕES =====
    int repet = 10;           // Número de repetições por tamanho
    int inicio = 10000;      // Tamanho inicial do vetor
    int fim = 250000;        // Tamanho final do vetor
    int step = 10000;        // Incremento
    // =========================

    FILE* csvFile = fopen("resultados_ordenacao.csv", "w");
    if (csvFile == NULL) {
        printf("Erro ao criar arquivo CSV.\n");
        return 1;
    }

    // Cabeçalho do CSV
    fprintf(csvFile, "Repeticao,Tamanho,Tipo_Vetor,Algoritmo,Tempo\n");

    printf("============================================\n");
    printf("INICIANDO TESTES DE ORDENACAO\n");
    printf("============================================\n");
    printf("Repeticoes: %d\n", repet);
    printf("Tamanho: %d ate %d (incremento: %d)\n", inicio, fim, step);
    printf("Algoritmos: Insertion, Merge, Quick, Heap\n");
    printf("============================================\n\n");

    // Loop pelos tamanhos
    for (int tamanho = inicio; tamanho <= fim; tamanho += step) {
        printf("\n>>> Testando tamanho: %d\n", tamanho);

        // ===== VETOR ALEATÓRIO =====
        printf("\n--- Vetor Aleatorio ---\n");
        for (int rep = 1; rep <= repet; rep++) {
            int* vetor_original = generateRandomArray(tamanho);
            int* vetor_trabalho = (int*)malloc(tamanho * sizeof(int));

            /*
            copyArray(vetor_original, vetor_trabalho, tamanho);
            clock_t start = clock();
            insertionSort(vetor_trabalho, tamanho);
            clock_t end = clock();
            double tempo = (double)(end - start) / CLOCKS_PER_SEC;
            fprintf(csvFile, "%d,%d,Aleatorio,InsertionSort,%.6f\n", rep, tamanho, tempo);
            printf("  Rep %d - Insertion Sort: %.6f s\n", rep, tempo);
            */
            // Merge Sort
            copyArray(vetor_original, vetor_trabalho, tamanho);
            clock_t start = clock();
            mergeSort(vetor_trabalho, tamanho);
            clock_t end = clock();
            double tempo = (double)(end - start) / CLOCKS_PER_SEC;
            fprintf(csvFile, "%d,%d,Aleatorio,MergeSort,%.6f\n", rep, tamanho, tempo);
            printf("  Rep %d - Merge Sort: %.6f s\n", rep, tempo);

            // Quick Sort
            copyArray(vetor_original, vetor_trabalho, tamanho);
            start = clock();
            quickSort(vetor_trabalho, tamanho);
            end = clock();
            tempo = (double)(end - start) / CLOCKS_PER_SEC;
            fprintf(csvFile, "%d,%d,Aleatorio,QuickSort,%.6f\n", rep, tamanho, tempo);
            printf("  Rep %d - Quick Sort: %.6f s\n", rep, tempo);

            // Heap Sort
            copyArray(vetor_original, vetor_trabalho, tamanho);
            start = clock();
            heapSort(vetor_trabalho, tamanho);
            end = clock();
            tempo = (double)(end - start) / CLOCKS_PER_SEC;
            fprintf(csvFile, "%d,%d,Aleatorio,HeapSort,%.6f\n", rep, tamanho, tempo);
            printf("  Rep %d - Heap Sort: %.6f s\n", rep, tempo);

            free(vetor_original);
            free(vetor_trabalho);
        }

        // ===== VETOR CRESCENTE =====
        printf("\n--- Vetor Crescente ---\n");
        for (int rep = 1; rep <= repet; rep++) {
            int* vetor_original = generateSortedArray(tamanho);
            int* vetor_trabalho = (int*)malloc(tamanho * sizeof(int));

            /*/
            copyArray(vetor_original, vetor_trabalho, tamanho);
            clock_t start = clock();
            insertionSort(vetor_trabalho, tamanho);
            clock_t end = clock();
            double tempo = (double)(end - start) / CLOCKS_PER_SEC;
            fprintf(csvFile, "%d,%d,Crescente,InsertionSort,%.6f\n", rep, tamanho, tempo);
            printf("  Rep %d - Insertion Sort: %.6f s\n", rep, tempo);
            */
            // Merge Sort
            copyArray(vetor_original, vetor_trabalho, tamanho);
            clock_t start = clock();
            mergeSort(vetor_trabalho, tamanho);
            clock_t end = clock();
            double tempo = (double)(end - start) / CLOCKS_PER_SEC;
            fprintf(csvFile, "%d,%d,Crescente,MergeSort,%.6f\n", rep, tamanho, tempo);
            printf("  Rep %d - Merge Sort: %.6f s\n", rep, tempo);

            // Quick Sort
            copyArray(vetor_original, vetor_trabalho, tamanho);
            start = clock();
            quickSort(vetor_trabalho, tamanho);
            end = clock();
            tempo = (double)(end - start) / CLOCKS_PER_SEC;
            fprintf(csvFile, "%d,%d,Crescente,QuickSort,%.6f\n", rep, tamanho, tempo);
            printf("  Rep %d - Quick Sort: %.6f s\n", rep, tempo);

            // Heap Sort
            copyArray(vetor_original, vetor_trabalho, tamanho);
            start = clock();
            heapSort(vetor_trabalho, tamanho);
            end = clock();
            tempo = (double)(end - start) / CLOCKS_PER_SEC;
            fprintf(csvFile, "%d,%d,Crescente,HeapSort,%.6f\n", rep, tamanho, tempo);
            printf("  Rep %d - Heap Sort: %.6f s\n", rep, tempo);

            free(vetor_original);
            free(vetor_trabalho);
        }

        // ===== VETOR DECRESCENTE =====
        printf("\n--- Vetor Decrescente ---\n");
        for (int rep = 1; rep <= repet; rep++) {
            int* vetor_original = generateReverseSortedArray(tamanho);
            int* vetor_trabalho = (int*)malloc(tamanho * sizeof(int));

            // // Insertion Sort
            // copyArray(vetor_original, vetor_trabalho, tamanho);
            // clock_t start = clock();
            // insertionSort(vetor_trabalho, tamanho);
            // clock_t end = clock();
            // double tempo = (double)(end - start) / CLOCKS_PER_SEC;
            // fprintf(csvFile, "%d,%d,Decrescente,InsertionSort,%.6f\n", rep, tamanho, tempo);
            // printf("  Rep %d - Insertion Sort: %.6f s\n", rep, tempo);

            // Merge Sort
            copyArray(vetor_original, vetor_trabalho, tamanho);
            clock_t start = clock();
            mergeSort(vetor_trabalho, tamanho);
            clock_t end = clock();
            double tempo = (double)(end - start) / CLOCKS_PER_SEC;
            fprintf(csvFile, "%d,%d,Decrescente,MergeSort,%.6f\n", rep, tamanho, tempo);
            printf("  Rep %d - Merge Sort: %.6f s\n", rep, tempo);

            // Quick Sort
            copyArray(vetor_original, vetor_trabalho, tamanho);
            start = clock();
            quickSort(vetor_trabalho, tamanho);
            end = clock();
            tempo = (double)(end - start) / CLOCKS_PER_SEC;
            fprintf(csvFile, "%d,%d,Decrescente,QuickSort,%.6f\n", rep, tamanho, tempo);
            printf("  Rep %d - Quick Sort: %.6f s\n", rep, tempo);

            // Heap Sort
            copyArray(vetor_original, vetor_trabalho, tamanho);
            start = clock();
            heapSort(vetor_trabalho, tamanho);
            end = clock();
            tempo = (double)(end - start) / CLOCKS_PER_SEC;
            fprintf(csvFile, "%d,%d,Decrescente,HeapSort,%.6f\n", rep, tamanho, tempo);
            printf("  Rep %d - Heap Sort: %.6f s\n", rep, tempo);

            free(vetor_original);
            free(vetor_trabalho);
        }
    }

    fclose(csvFile);
    printf("\n============================================\n");
    printf("TESTES CONCLUIDOS!\n");
    printf("Resultados salvos em: resultados_ordenacao.csv\n");
    printf("============================================\n");

    return 0;
}