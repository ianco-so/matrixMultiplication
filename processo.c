#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

// Função para multiplicar uma parte da matriz resultado em um processo
void multiplyPart(int process_id, int n1, int m1, int n2, int m2, int **matrix_N, int **matrix_M) {
    int P = n1 * m2;
    int elements_per_process = P / P;
    int start = process_id * elements_per_process;
    int end = (process_id == P - 1) ? P : start + elements_per_process;

    int **result = (int **)malloc(n1 * sizeof(int *));
    for (int i = 0; i < n1; i++) {
        result[i] = (int *)malloc(m2 * sizeof(int));
    }

    clock_t start_time = clock();

    for (int k = 0; k < n2; k++) {
        for (int i = 0; i < n1; i++) {
            for (int j = start; j < end; j++) {
                result[i][j] += matrix_N[i][k] * matrix_M[k][j];
            }
        }
    }

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // Salva o resultado em um arquivo
    char filename[50];
    snprintf(filename, sizeof(filename), "part%d_processo.txt", process_id);
    FILE *file_result = fopen(filename, "w");
    if (file_result == NULL) {
        perror("Erro ao abrir o arquivo de resultado");
        exit(EXIT_FAILURE);
    }

    // Escreve as dimensões do resultado
    fprintf(file_result, "%d %d\n", n1, m2);

    // Escreve os elementos do resultado
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            fprintf(file_result, "c%d%d %d\n", i + 1, j + 1, result[i][j]);
        }
    }

    // Escreve o tempo de cálculo
    fprintf(file_result, "%f", elapsed_time);

    fclose(file_result);

    // Libera a memória alocada para a parte do resultado
    for (int i = 0; i < n1; i++) {
        free(result[i]);
    }
    free(result);
}

int ** readMatrixFromFile(const char *filename, int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    // Lê as dimensões da matriz
    fscanf(file, "%d %d", rows, cols);

    // Aloca memória para a matriz
    int **matrix = (int **)malloc((*rows) * sizeof(int *));
    for (int i = 0; i < (*rows); i++) {
        matrix[i] = (int *)malloc((*cols) * sizeof(int));
    }

    // Lê os elementos da matriz
    for (int i = 0; i < (*rows); i++) {
        for (int j = 0; j < (*cols); j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }

    fclose(file);
    return matrix;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s arquivo_N arquivo_M P\n", argv[0]);
        return 1;
    }

    const char *file_N = argv[1];
    const char *file_M = argv[2];
    int P = atoi(argv[3]);

    int n1, m1, n2, m2;
    int **matrix_N = readMatrixFromFile(file_N, &n1, &m1);
    int **matrix_M = readMatrixFromFile(file_M, &n2, &m2);

    if (m1 != n2) {
        fprintf(stderr, "Erro: As dimensões das matrizes não são compatíveis para multiplicação.\n");
        exit(EXIT_FAILURE);
    }

    clock_t start_time = clock();

    for (int i = 0; i < P; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // Código do processo filho
            multiplyPart(i, n1, m1, n2, m2, matrix_N, matrix_M);
            exit(0);
        } else if (pid < 0) {
            perror("Erro ao criar processo");
            exit(EXIT_FAILURE);
        }
    }

    // Aguarda todos os processos filhos terminarem
    for (int i = 0; i < P; i++) {
        wait(NULL);
    }

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Tempo total de cálculo: %f segundos\n", elapsed_time);

    // Libera a memória alocada para as matrizes
    for (int i = 0; i < n1; i++) {
        free(matrix_N[i]);
    }
    for (int i = 0; i < n2; i++) {
        free(matrix_M[i]);
    }
    free(matrix_N);
    free(matrix_M);

    return 0;
}
