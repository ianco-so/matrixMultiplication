#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função para gerar uma matriz aleatória de dimensões rows x cols
void generateRandomMatrix(int rows, int cols, int **matrix) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 100; // Gera números aleatórios de 0 a 99
        }
    }
}

// Função para salvar uma matriz em um arquivo
void saveMatrixToFile(int rows, int cols, int **matrix, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d %d\n", rows, cols); // Escreve as dimensões no arquivo

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d ", matrix[i][j]); // Escreve os elementos da matriz no arquivo
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Uso: %s n1 m1 n2 m2\n", argv[0]);
        return 1;
    }

    int n1 = atoi(argv[1]);
    int m1 = atoi(argv[2]);
    int n2 = atoi(argv[3]);
    int m2 = atoi(argv[4]);

    // Inicializa o gerador de números aleatórios com uma semente
    srand(time(NULL));

    // Aloca memória para as matrizes M1 e M2
    int **M1 = (int **)malloc(n1 * sizeof(int *));
    int **M2 = (int **)malloc(n2 * sizeof(int *));
    for (int i = 0; i < n1; i++) {
        M1[i] = (int *)malloc(m1 * sizeof(int));
    }
    for (int i = 0; i < n2; i++) {
        M2[i] = (int *)malloc(m2 * sizeof(int));
    }

    // Gera as matrizes aleatórias
    generateRandomMatrix(n1, m1, M1);
    generateRandomMatrix(n2, m2, M2);

    // Salva as matrizes em arquivos separados
    saveMatrixToFile(n1, m1, M1, "matrix1.txt");
    saveMatrixToFile(n2, m2, M2, "matrix2.txt");

    // Libera a memória alocada para as matrizes
    for (int i = 0; i < n1; i++) {
        free(M1[i]);
    }
    for (int i = 0; i < n2; i++) {
        free(M2[i]);
    }
    free(M1);
    free(M2);

    return 0;
}
