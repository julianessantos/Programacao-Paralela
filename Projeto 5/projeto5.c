#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

int eh_primo(int num) {
    if (num < 2) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

int contar_primos_sequencial(int max) {
    int count = 0;
    for (int i = 2; i <= max; i++) {
        if (eh_primo(i)) count++;
    }
    return count;
}

int contar_primos_paralelo(int max) {
    int count = 0;

    #pragma omp parallel for
    for (int i = 2; i <= max; i++) {
        if (eh_primo(i)) count++;
    }
    
    return count;
}

double tempo_execucao(struct timeval inicio, struct timeval fim) {
    return (fim.tv_sec - inicio.tv_sec) + (fim.tv_usec - inicio.tv_usec) / 1e6;
}

int main() {
    struct timeval t1, t2;
    int total_seq, total_par;

    FILE *fp = fopen("resultados.csv", "w");
    if (!fp) {
        printf("Erro ao criar arquivo!\n");
        return 1;
    }

    fprintf(fp, "MAX,Tempo_Sequencial,Primos_Sequencial,Tempo_Paralelo,Primos_Paralelo\n");

    for (int max = 1000000; max <= 10000000; max += 1000000) {
        // Sequencial
        gettimeofday(&t1, NULL);
        total_seq = contar_primos_sequencial(max);
        gettimeofday(&t2, NULL);
        double tempo_seq = tempo_execucao(t1, t2);

        // Paralelo
        gettimeofday(&t1, NULL);
        total_par = contar_primos_paralelo(max);
        gettimeofday(&t2, NULL);
        double tempo_par = tempo_execucao(t1, t2);

        printf("MAX: %d\n", max);
        printf("Sequencial: %d primos em %.6f s\n", total_seq, tempo_seq);
        printf("Paralelo: %d primos em %.6f s\n\n", total_par, tempo_par);

        fprintf(fp, "%d,%.6f,%d,%.6f,%d\n", max, tempo_seq, total_seq, tempo_par, total_par);
    }

    fclose(fp);
    return 0;
}