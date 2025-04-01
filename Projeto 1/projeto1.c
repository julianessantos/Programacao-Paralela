#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define N 2000  // Tamanho desejado

int matrix[N][N];
int vetor[N];
int resultado_linhas[N];
int resultado_colunas[N];

// Função para medir o tempo
double get_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) * 1000.0 + (tv.tv_usec) / 1000.0;
}

// Função para inicializar a matriz e o vetor de tamanho N
void inicializar() {
    srand(time(NULL));
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            matrix[i][j] = rand() % 100;
        }
        vetor[j] = rand() % 100;
        resultado_linhas[j] = 0;
        resultado_colunas[j] = 0;
    }
}

int main() {
    inicializar();

    // Versão por linhas
    double start_linhas = get_time_ms();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            resultado_linhas[i] += matrix[i][j] * vetor[j];
        }
    }
    double end_linhas = get_time_ms();

    // Versão por colunas
    double start_colunas = get_time_ms();
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            resultado_colunas[i] += matrix[i][j] * vetor[j];
        }
    }
    double end_colunas = get_time_ms();

    printf("Tempo por LINHAS: %.15f ms\n", end_linhas - start_linhas);
    printf("Tempo por COLUNAS: %.15f ms\n", end_colunas - start_colunas);

    return 0;
}