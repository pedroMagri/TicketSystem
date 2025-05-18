#include "../includes/sistema.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// Variáveis globais
Assento cinema[MAX_ASSENTOS];
Cliente fila[MAX_CLIENTES];
int num_assentos = MAX_ASSENTOS;
int qntd_clientes_na_fila = 0;
int sistema_ativo = 1;
pthread_mutex_t mutexFila = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condFila = PTHREAD_COND_INITIALIZER;

// Função para inicializar os assentos
void inicializarAssentos() {
    for (int i = 0; i < num_assentos; i++) {
        cinema[i].id = i;
        pthread_mutex_init(&cinema[i].mutexAssento, NULL);
    }
}

// Função para encerrar o sistema
void encerrarSistema() {
    pthread_mutex_lock(&mutexFila);
    sistema_ativo = 0;
    pthread_cond_broadcast(&condFila);
    pthread_mutex_unlock(&mutexFila);
    printf("[SISTEMA] Sistema encerrado.\n");
}
