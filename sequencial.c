#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

// Função para multiplicar duas matrizes
void multiplyMatrices(int **A, int **B, int **C, int n, int m, int p) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            C[i][j] = 0;
            for (int k = 0; k < m; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Função ler dimensões de uma matriz de um arquivo
void readDimensionsFromFile(const char *filename, int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d %d", rows, cols); // Lê as dimensões da matriz do arquivo

    fclose(file);
}


// Função para ler uma matriz de um arquivo
void readMatrixFromFile(const char *filename, int **matrix, int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d %d", rows, cols); // Lê as dimensões da matriz do arquivo

    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            fscanf(file, "%d", &matrix[i][j]); // Lê os elementos da matriz do arquivo
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s arquivo_M1 arquivo_M2\n", argv[0]);
        return 1;
    }

    const char *filename_M1 = argv[1];
    const char *filename_M2 = argv[2];

    int rows_M1, cols_M1, rows_M2, cols_M2;

    // Lê as dimensões das matrizes M1 e M2 dos arquivos
    readDimensionsFromFile(filename_M1, &rows_M1, &cols_M1);
    readDimensionsFromFile(filename_M2, &rows_M2, &cols_M2);

    // Verifica se as matrizes podem ser multiplicadas
    if (cols_M1 != rows_M2) {
        printf("As matrizes não podem ser multiplicadas. O número de colunas de M1 deve ser igual ao número de linhas de M2.\n");
        return 1;
    }

    int **M1 = (int **)malloc(rows_M1 * sizeof(int *));
    int **M2 = (int **)malloc(rows_M2 * sizeof(int *));
    int **resultado = (int **)malloc(rows_M1 * sizeof(int *));
    for (int i = 0; i < rows_M1; i++) {
        M1[i] = (int *)malloc(cols_M1 * sizeof(int));
        resultado[i] = (int *)malloc(cols_M2 * sizeof(int));
    }
    for (int i = 0; i < rows_M2; i++) {
        M2[i] = (int *)malloc(cols_M2 * sizeof(int));
    }

    // Lê as matrizes M1 e M2 dos arquivos
    readMatrixFromFile(filename_M1, M1, &rows_M1, &cols_M1);
    readMatrixFromFile(filename_M2, M2, &rows_M2, &cols_M2);

    struct timeval start, end;
    // gettimeofday(&start, NULL); // Inicia a contagem do tempo
    clock_t start_time = clock();

    // Multiplica as matrizes M1 e M2
    multiplyMatrices(M1, M2, resultado, rows_M1, cols_M1, cols_M2);

    clock_t end_time = clock();
    double elapsed = (double)(end_time - start_time) / CLOCKS_PER_SEC;


    // gettimeofday(&end, NULL); // Finaliza a contagem do tempo
    // long seconds = end.tv_sec - start.tv_sec;
    // long microseconds = end.tv_usec - start.tv_usec;
    // double elapsed = seconds + (microseconds / 1e6);

    // Salva o resultado e o tempo de cálculo no arquivo "resultado.txt"
    FILE *file_resultado = fopen("resultado_sequencial.txt", "w");
    if (file_resultado == NULL) {
        perror("Erro ao abrir o arquivo de resultado");
        return 1;
    }

    fprintf(file_resultado, "%d %d\n", rows_M1, cols_M2); // Escreve as dimensões do resultado

    for (int i = 0; i < rows_M1; i++) {
        for (int j = 0; j < cols_M2; j++) {
            fprintf(file_resultado, "c%d%d %d\n", i + 1, j + 1, resultado[i][j]); // Escreve o resultado
        }
    }

    fprintf(file_resultado, "%lf", elapsed); // Escreve o tempo de cálculo
    fclose(file_resultado);

    // Libera a memória alocada para as matrizes
    for (int i = 0; i < rows_M1; i++) {
        free(M1[i]);
        free(resultado[i]);
    }
    for (int i = 0; i < rows_M2; i++) {
        free(M2[i]);
    }
    free(M1);
    free(M2);
    free(resultado);

    return 0;
}
