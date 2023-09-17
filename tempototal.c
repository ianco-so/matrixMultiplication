#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s P\n", argv[0]);
        return 1;
    }

    int P = atoi(argv[1]);
    double tempo_total = 0.0;

    for (int i = 0; i < P; i++) {
        char filename[50];
        snprintf(filename, sizeof(filename), "part%d_result.txt", i);
        FILE *file = fopen(filename, "r");

        if (file == NULL) {
            perror("Erro ao abrir arquivo");
            return 1;
        }

        // Pula para a última linha do arquivo
        fseek(file, -1, SEEK_END);
        while (fgetc(file) != '\n') {
            fseek(file, -2, SEEK_CUR);
        }

        // Lê o tempo da última linha
        double tempo_thread;
        fscanf(file, "%lf", &tempo_thread);

        tempo_total += tempo_thread;

        fclose(file);
    }

    // Salva o tempo total no arquivo "tempo_total.txt"
    FILE *file_tempo_total = fopen("tempo_total.txt", "w");
    if (file_tempo_total == NULL) {
        perror("Erro ao criar arquivo 'tempo_total.txt'");
        return 1;
    }

    fprintf(file_tempo_total, "Tempo Total: %lf\n", tempo_total);
    fclose(file_tempo_total);

    printf("Tempo Total: %lf\n", tempo_total);

    return 0;
}
