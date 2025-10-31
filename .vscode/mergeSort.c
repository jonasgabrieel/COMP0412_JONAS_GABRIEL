#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

// Protótipos das funções
void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);
void printArray(int A[], int size);

/**
 * @brief Função principal do MergeSort.
 * @param arr O array a ser ordenado.
 * @param l O índice da esquerda (início).
 * @param r O índice da direita (fim).
 */
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        // 1. Dividir
        // Encontra o ponto médio de forma segura (evita overflow para l+r grande)
        int m = l + (r - l) / 2;

        // 2. Conquistar (Ordena as duas metades)
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        // 3. Combinar (Mescla as metades ordenadas)
        merge(arr, l, m, r);
    }
}

/**
 * @brief Mescla dois subarrays de arr[].
 * O primeiro subarray é arr[l..m]
 * O segundo subarray é arr[m+1..r]
 */
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1; // Tamanho do subarray da esquerda
    int n2 = r - m;     // Tamanho do subarray da direita

    // Aloca memória para os arrays temporários
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    // Verificação de falha na alocação de memória
    if (L == NULL || R == NULL) {
        printf("Erro: Falha ao alocar memória.\n");
        exit(1); // Encerra o programa se a memória não puder ser alocada
    }

    // Copia os dados para os arrays temporários L[] e R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Índices iniciais dos subarrays
    i = 0; // Índice inicial do subarray L
    j = 0; // Índice inicial do subarray R
    k = l; // Índice inicial do subarray mesclado (no array original 'arr')

    // Mescla os arrays temporários de volta em arr[l..r]
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copia os elementos restantes de L[], se houver algum
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copia os elementos restantes de R[], se houver algum
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    // Libera a memória alocada para os arrays temporários
    free(L);
    free(R);
}

// Função utilitária para imprimir um array
void printArray(int A[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}

// Função main para testar o algoritmo
int main() {
    int arr[] = {12, 11, 13, 5, 6, 7, 2, 9};
    int arr_size = sizeof(arr) / sizeof(arr[0]);

    printf("Array original: \n");
    printArray(arr, arr_size);

    // Chama o mergeSort
    mergeSort(arr, 0, arr_size - 1);

    printf("\nArray ordenado: \n");
    printArray(arr, arr_size);
   
    return 0;
}
