#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s P thread/processo\n", argv[0]);
        return 1;
    }
    char *threadOrProcess = argv[2];
    if (strcmp(threadOrProcess, "thread") != 0 && strcmp(threadOrProcess, "processo") != 0) {
        printf("Uso: %s P thread/processo\n", argv[0]);
        return 1;
    }

    int P = atoi(argv[1]);
    double tempo_total = 0.0;

    char tOrp[25];
    if (strcmp(threadOrProcess, "thread") == 0){
        strcpy(tOrp, "thread");
    } else {
        strcpy(tOrp, "processo");
    }

    for (int i = 0; i < P; i++) {
        char filename[50];
        snprintf(filename, sizeof(filename), "part%d_%s.txt", i, tOrp);
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

    // Salva o tempo total no arquivo "tempo_total%s.txt"
    char filename_tempo_total[50];
    snprintf(filename_tempo_total, sizeof(filename_tempo_total), "tempo_total_%s.txt", tOrp);
    FILE *file_tempo_total = fopen(filename_tempo_total, "w");
    if (file_tempo_total == NULL) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    fprintf(file_tempo_total, "Tempo Total do %s:  %lf", tOrp, tempo_total);
    fclose(file_tempo_total);

    printf("Tempo Total do %s:  %lf\n", tOrp, tempo_total);

    return 0;
}
