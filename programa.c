#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_ATENDENTES 1

typedef struct Assento
{
  int id;
  pthread_mutex_t mutexAssento;
} Assento;

typedef struct Cliente
{
  int id;
  int assento_desejado;
} Cliente;

// Cria um cinema com 128 assentos.
Assento cinema[128];

// Cria uma fila vazia com capacidade para 64 pessoas.
Cliente fila[64];
int qntd_clientes_na_fila = 0;

// Mutex para garantir que só UMA thread atendente chame o primeiro cliente da fila.
pthread_mutex_t mutexFila;

// Variável de condição para que os atendentes esperem se a fila estiver vazia.
pthread_cond_t condFila;

// Função usada para iniciar as threads atendentes da pool.
void *inicializarAtendente(void *args) {}

// Função que a thread executa para tentar colocar o cliente no assento.
void atenderCliente(Cliente *cliente) {}

void *cumprimentar(void *args) {}

void addClienteNaFila(Cliente *cliente) {}

int main(int argc, char *argv[])
{
  pthread_t atendentes[NUM_ATENDENTES];
  pthread_mutex_init(&mutexFila, NULL);
  pthread_cond_init(&condFila, NULL);

  for (int i = 0; i < cinema; i++)
  {
    cinema[i].id = i;
    pthread_mutex_init(&cinema[i].mutexAssento, NULL);
  }

  pthread_mutex_destroy(&mutexFila);
  pthread_cond_destroy(&condFila);
}

// Inicializa a thread atendente, mantém ela alerta para clientes na fila.
void *inicializarAtendente(void *args)
{
  while (1)
  {
    Cliente cliente;
    pthread_mutex_lock(&mutexFila);

    while (qntd_clientes_na_fila == 0)
    {
      pthread_cond_wait(&condFila, &mutexFila);
    }
    cliente = fila[0];
    for (int i = 0; i < qntd_clientes_na_fila - 1; i++)
    {
      fila[i] = fila[i + 1];
    }
    qntd_clientes_na_fila--;

    pthread_mutex_unlock(&mutexFila);

    atenderCliente(&cliente);
  }
}

void *atenderCliente(Cliente *cliente)
{
  printf("Atendendo cliente...");
  sleep(1);
  if (pthread_mutex_trylock(&cinema[cliente->assento_desejado]) == 0)
  {
    printf("ASSENTO LIBERADO!");
  }
}

void *cumprimentar()
{
  printf("Olá cliente!\n");
}