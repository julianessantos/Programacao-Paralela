#include <stdio.h>
#include <math.h>
#include <sys/time.h>  // Para gettimeofday()
#include <stdlib.h>  // Para fopen, fprintf, fclose

#define PI_REAL 3.14159265358979323846  // Valor real de π

// Função para calcular π usando a série de Leibniz
double calcular_pi(int iteracoes) {
    double pi = 0.0;
    for (int i = 0; i < iteracoes; i++) {
        double termo = (double) 4 / (2 * i + 1);
        if (i % 2 == 0) {
            pi += termo;
        } else {
            pi -= termo;
        }
    }
    return pi;
}

int main() {
    int iteracoes[] = {10000, 100000, 1000000, 10000000, 100000000};  // Diferentes quantidades de iterações
    int qtd_iteracoes = sizeof(iteracoes) / sizeof(iteracoes[0]);

    // Abrir arquivo CSV para escrita
    FILE *arquivo = fopen("resultados_pi.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo CSV.\n");
        return 1;
    }

    // Cabeçalho do CSV
    fprintf(arquivo, "Iteracoes,PiAproximado\n");

    printf("Iterações\tPi Aproximado\t\tErro Absoluto\t\tTempo (ms)\n");
    printf("---------------------------------------------------------------------\n");

    for (int i = 0; i < qtd_iteracoes; i++) {
        int n = iteracoes[i];

        // Medir tempo inicial
        struct timeval inicio, fim;
        gettimeofday(&inicio, NULL);

        // Calcular aproximação de π
        double pi_aprox = calcular_pi(n);

        // Medir tempo final
        gettimeofday(&fim, NULL);

        // Calcular tempo decorrido em milissegundos
        double tempo = (fim.tv_sec - inicio.tv_sec) * 1000.0 + (fim.tv_usec - inicio.tv_usec) / 1000.0;

        // Calcular erro absoluto
        double erro = fabs(PI_REAL - pi_aprox);

        // Imprimir no terminal
        printf("%d\t\t%.10f\t%.10f\t%.10f ms\n", n, pi_aprox, erro, tempo);

        // Escrever no CSV (com precisão alta)
        fprintf(arquivo, "%d,%.50f\n", n, pi_aprox);
    }

    fclose(arquivo);
    return 0;
}