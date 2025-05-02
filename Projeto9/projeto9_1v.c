#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <time.h>

#define N 50

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

// Versão 1: Duas listas com regiões críticas nomeadas (OpenMP)
void versao1() {
    Node* lista1 = NULL;
    Node* lista2 = NULL;
    struct timeval inicio, fim;
    gettimeofday(&inicio, NULL);

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        unsigned int seed = (unsigned int)(time(NULL) ^ omp_get_thread_num() ^ i);
        int num     = rand_r(&seed) % 100;
        int escolha = rand_r(&seed) % 2;

        if (escolha == 0) {
            #pragma omp critical(lista1)
            {
                inserir(&lista1, num);
                //printf("Task %2d | Thread %2d | Valor: %2d -> Lista 1\n", i, omp_get_thread_num(), num);
            }
        } else {
            #pragma omp critical(lista2)
            {
                inserir(&lista2, num);
                //printf("Task %2d | Thread %2d | Valor: %2d -> Lista 2\n", i, omp_get_thread_num(), num);
            }
        }
    }

    gettimeofday(&fim, NULL);
    double tempo = (fim.tv_sec - inicio.tv_sec) * 1000.0 + (fim.tv_usec - inicio.tv_usec) / 1000.0;
    printf("Tempo de execucao (Versao 1): %.2f ms\n\n", tempo);

    //print_list("Lista 1", lista1);
    //print_list("Lista 2", lista2);
}

int main() {
    printf("--- Versao 1 ---\n");
    versao1();
    return 0;
}
