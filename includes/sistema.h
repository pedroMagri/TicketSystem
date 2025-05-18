#ifndef SISTEMA_H
#define SISTEMA_H

#include <pthread.h>

// Número de atendentes e tempo que a bilheteria ficará aberta
#define NUM_ATENDENTES 4
#define MAX_ASSENTOS 128
#define MAX_CLIENTES 64

// Estrutura de dados
typedef struct Assento {
    int id;
    pthread_mutex_t mutexAssento;
} Assento;

typedef struct Cliente {
    int id;
    int assento_desejado;
} Cliente;

// =-=-=-=-=-=-=-=-=-= Variáveis globais =-=-=-=-=-=-=-=-=-=
// Cria um cinema com 128 assentos.
extern Assento cinema[MAX_ASSENTOS];
// Cria uma fila vazia com capacidade para 64 pessoas.
extern Cliente fila[MAX_CLIENTES];

extern int num_assentos;
extern int qntd_clientes_na_fila;
extern int sistema_ativo;
extern pthread_mutex_t mutexFila;
extern pthread_cond_t condFila;

// Funções do sistema
void inicializarAssentos();
void encerrarSistema();

#endif
