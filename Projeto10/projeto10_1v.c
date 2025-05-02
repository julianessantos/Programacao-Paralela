#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <sys/time.h>

double tempo_em_segundos() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

int main() {
    int total_pontos = 100000000;
    int acertos_globais = 0;

    double inicio = tempo_em_segundos();

    #pragma omp parallel  
    {
        int acertos_privado = 0;
        unsigned int seed = omp_get_thread_num();

        #pragma omp for
        for (int i = 0; i < total_pontos; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
            if (x * x + y * y <= 1.0)
                acertos_privado++;
        }

        #pragma omp atomic
        acertos_globais += acertos_privado;
        
    }

    double fim = tempo_em_segundos();

    printf("Versão 1 - rand_r() + atomic\n");
    printf("Pi estimado: %f\n", 4.0 * acertos_globais / total_pontos);
    printf("Tempo: %f segundos\n", fim - inicio);
    return 0;
}