#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>

#define N 10000000  // 10 milhões de elementos/operações TENTAR AUMENTAR O NUMERO DE THREASD
#define MAX_THREADS 16  // Número máximo de threads suportado
//HARDWARE MULTITHREADS

//PIPLINE: A CADA CICLO DE CLOCK UMA INSTRUÇÃO CONCLUIDA
//VETORIZAÇÃO:  
//TROCA DE CONTEXTO: TIRA UM PROGRAMA PARA EXECUTAR OUTRO
//MANTEVE AS ESTRUTURAS DE CONTROLE E MODIFICOU A ESTRUTURA DE BANCO DE DADOS FAZ A TROCA (INTERRUPTOR) DO BLOCO DE REGISTRADORES (CADA THREAD TEM UM BANCO DE REGISTRADORES DIFERENTES)
// 

// Função para medir o tempo com gettimeofday
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

void memory_bound_test(int num_threads, double *time_taken) {
    static double a[N], b[N], c[N];

    // Inicialização dos vetores
    for (int i = 0; i < N; i++) {
        a[i] = i * 1.0;
        b[i] = (N - i) * 1.0;
    }

    omp_set_num_threads(num_threads);
    double start_time = get_time();

    // Cálculo simples: soma e subtração
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        c[i] = a[i] + b[i] - i;
    }

    double end_time = get_time();
    *time_taken = end_time - start_time;
    
    printf("[Memory-Bound] Threads: %d | Tempo: %f s\n", num_threads, *time_taken);
}

void compute_bound_test(int num_threads, double *time_taken) {
    static double results[N];

    omp_set_num_threads(num_threads);
    double start_time = get_time();

    // Cálculo intensivo: seno, cosseno e exponencial
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        results[i] = sin(i) + cos(i) + exp(i * 0.000001);
    }

    double end_time = get_time();
    *time_taken = end_time - start_time;
    
    printf("[Compute-Bound] Threads: %d | Tempo: %f s\n", num_threads, *time_taken);
}

int main() {
    int threads[MAX_THREADS];
    int num_values;

    // Pergunta ao usuário quantos valores de threads deseja testar
    printf("Quantos valores de threads deseja testar? (Máx: %d): ", MAX_THREADS);
    scanf("%d", &num_values);

    if (num_values > MAX_THREADS || num_values <= 0) {
        printf("Número inválido! Deve ser entre 1 e %d.\n", MAX_THREADS);
        return 1;
    }

    // Entrada do usuário para definir os números de threads a testar
    printf("Digite os %d valores de threads (ex: 1 2 4 8 16):\n", num_values);
    for (int i = 0; i < num_values; i++) {
        scanf("%d", &threads[i]);
    }

    // Abre o arquivo CSV para salvar os resultados
    FILE *file = fopen("resultados.csv", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita!\n");
        return 1;
    }
    
    fprintf(file, "Threads,Memory-Bound,Compute-Bound\n"); // Cabeçalho do CSV

    printf("\nExecutando testes...\n\n");

    for (int i = 0; i < num_values; i++) {
        int num_threads = threads[i];
        double memory_time, compute_time;

        memory_bound_test(num_threads, &memory_time);
        compute_bound_test(num_threads, &compute_time);

        // Salva os resultados no CSV
        fprintf(file, "%d,%.6f,%.6f\n", num_threads, memory_time, compute_time);
        
        printf("\n");
    }

    fclose(file);
    printf("Resultados salvos em 'resultados.csv'.\n");

    return 0;
}