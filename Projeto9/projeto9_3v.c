#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <time.h>

#define N 50
#define MAX_LISTAS 10

typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Insere um valor no início da lista encadeada
void inserir(Node** head, int valor) {
    Node* novo = (Node*)malloc(sizeof(Node));
    novo->data = valor;
    novo->next = *head;
    *head = novo;
}

// Imprime os elementos da lista
void print_list(const char* nome, Node* cabeca) {
    printf("%s:\n", nome);
    while (cabeca) {
        printf("%d -> ", cabeca->data);
        cabeca = cabeca->next;
    }
    printf("NULL\n");
}

// Versão 3: N listas com regiões críticas nomeadas (limitado a MAX_LISTAS)
void versao3(int qtd_listas) {
    if (qtd_listas > MAX_LISTAS) qtd_listas = MAX_LISTAS;
    Node* listas[MAX_LISTAS] = { NULL };

    struct timeval inicio, fim;
    gettimeofday(&inicio, NULL);

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        unsigned int seed = (unsigned int)(time(NULL) ^ omp_get_thread_num() ^ i);
        int num     = rand_r(&seed) % 100;
        int escolha = rand_r(&seed) % qtd_listas;

        // Todas as listas usam a mesma região crítica genérica,
        // pois não se pode nomear dinamicamente as critical sections.
        #pragma omp critical
        {
            inserir(&listas[escolha], num);
            //printf("Task %2d | Thread %2d | Valor: %2d -> Lista %d\n", i, omp_get_thread_num(), num, escolha);
        }
    }

    gettimeofday(&fim, NULL);
    double tempo = (fim.tv_sec - inicio.tv_sec) * 1000.0 + (fim.tv_usec - inicio.tv_usec) / 1000.0;
    printf("Tempo de execucao (Versao 3): %.2f ms\n\n", tempo);

    for (int j = 0; j < qtd_listas; j++) {
        char nome[16];
        //sprintf(nome, "Lista %d", j);
        //print_list(nome, listas[j]);
    }
}

int main() {
    printf("--- Versao 3 ---\n");
    versao3(10);  // Exemplo com 5 listas
    return 0;
}
