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
    int num_threads;
    int num_execucoes = 10;
    double tempo_total = 0.0;

    for (int j = 0; j < num_execucoes; j++) {
        double inicio = tempo_em_segundos();

        #pragma omp parallel
        {
            int id = omp_get_thread_num();
            int local_acertos = 0;

            #pragma omp single
            num_threads = omp_get_num_threads();

            static int* vetor_acertos;
            #pragma omp single
            vetor_acertos = (int*)calloc(num_threads, sizeof(int));

            #pragma omp for
            for (int i = 0; i < total_pontos; i++) {
                double x = (double)rand() / RAND_MAX;
                double y = (double)rand() / RAND_MAX;
                if (x * x + y * y <= 1.0)
                    local_acertos++;
            }

            vetor_acertos[id] = local_acertos;

            #pragma omp barrier
            #pragma omp single
            {
                int total = 0;
                for (int i = 0; i < num_threads; i++)
                    total += vetor_acertos[i];

                double fim = tempo_em_segundos();
                double tempo_execucao = fim - inicio;
                tempo_total += tempo_execucao;

                printf("Execução %d - Pi estimado: %f\n", j + 1, 4.0 * total / total_pontos);
                printf("Tempo execução %d: %f segundos\n", j + 1, tempo_execucao);
                free(vetor_acertos);
            }
        }
    }

    double tempo_medio = tempo_total / num_execucoes;
    printf("\nMédia dos tempos de execução: %f segundos\n", tempo_medio);

    return 0;
}
