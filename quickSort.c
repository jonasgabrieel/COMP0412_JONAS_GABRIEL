#include <stdio.h>
#include <time.h>

// Protótipos das funções
void swap(int* a, int* b);
int partition(int arr[], int low, int high);
void quickSort(int arr[], int low, int high);
void printArray(int arr[], int size);

/**
 * @brief Função utilitária para trocar dois elementos de lugar.
 * Recebe ponteiros para os elementos.
 */
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

/**
 * @brief Função de Partição (Esquema de Lomuto).
 * Esta função pega o último elemento como pivô, coloca
 * o pivô em sua posição correta no array ordenado,
 * e coloca todos os menores (menores que o pivô)
 * à sua esquerda e todos os maiores à sua direita.
 * @param arr O array a ser particionado.
 * @param low Índice inicial (baixo).
 * @param high Índice final (alto).
 * @return O índice da posição final do pivô.
 */
int partition(int arr[], int low, int high) {
    // 1. Escolhe o pivô (último elemento)
    int pivo = arr[high];
   
    // i é o índice do último elemento menor que o pivô
    int i = (low - 1);

    // 2. Particionar
    // Percorre o array de 'low' até 'high-1'
    for (int j = low; j <= high - 1; j++) {
        // Se o elemento atual é menor ou igual ao pivô
        if (arr[j] <= pivo) {
            i++; // Incrementa o índice do "menor elemento"
            swap(&arr[i], &arr[j]);
        }
    }
   
    // 3. Colocar o pivô no lugar
    // Troca o pivô (arr[high]) com o elemento em (i + 1)
    swap(&arr[i + 1], &arr[high]);
   
    // Retorna o índice onde o pivô agora está
    return (i + 1);
}

// Função principal do QuickSort.
// arr O array a ser ordenado.
// low O índice inicial.
// high O índice final.
 

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        // pi é o índice de particionamento, arr[pi] já está no lugar certo.
        int pi = partition(arr, low, high);

        // 2. Conquistar (Ordena recursivamente)
        // Ordena elementos antes da partição
        quickSort(arr, low, pi - 1);
        // Ordena elementos depois da partição
        quickSort(arr, pi + 1, high);
    }
}

// Função utilitária para imprimir um array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// Função main para testar o algoritmo
int main() {
    int arr[] = {13,12,10,15,20,2,5,8,9,4,7};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array original: \n");
    printArray(arr, n);

    // Chama o quickSort
    quickSort(arr, 0, n - 1);

    printf("\nArray ordenado: \n");
    printArray(arr, n);
   
    return 0;
}