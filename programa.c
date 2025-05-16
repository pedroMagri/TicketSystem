#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

// Número de atendentes e tempo que a bilheteria ficará aberta
#define NUM_ATENDENTES 4

// Cria tipos Assento e Cliente
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
int num_assentos = sizeof(cinema) / sizeof(cinema[0]);

// Cria uma fila vazia com capacidade para 64 pessoas.
Cliente fila[64];
int qntd_clientes_na_fila = 0;

// Mutex para garantir que só UMA thread atendente chame o primeiro cliente da fila.
pthread_mutex_t mutexFila;

// Variável de condição para que os atendentes esperem se a fila estiver vazia.
pthread_cond_t condFila;

// Inicializa a thread atendente, mantém ela alerta para clientes na fila.
void *inicializarAtendente(void *args);

void *gerarClientes(void *args);

void atenderCliente(Cliente *cliente, int atendente_id);

int main(int argc, char *argv[])
{
  srand(time(NULL));
  pthread_t atendentes[NUM_ATENDENTES];
  pthread_t geradorClientes;
  int tempo_bilheteria = 20;

  pthread_mutex_init(&mutexFila, NULL);
  pthread_cond_init(&condFila, NULL);

  for (int i = 0; i < num_assentos; i++)
  {
    cinema[i].id = i;
    pthread_mutex_init(&cinema[i].mutexAssento, NULL);
  }

  for (int i = 0; i < NUM_ATENDENTES; i++)
  {
    usleep(10000);
    int *id = malloc(sizeof(int)); // aloca memória separada
    if (id == NULL)
    {
      perror("malloc");
      exit(1);
    }
    *id = i + 1;

    pthread_create(&atendentes[i], NULL, inicializarAtendente, id);
  }

  pthread_create(&geradorClientes, NULL, gerarClientes, &tempo_bilheteria);

  pthread_join(geradorClientes, NULL);

  for (int i = 0; i < NUM_ATENDENTES; i++)
  {
    pthread_cancel(atendentes[i]);
    pthread_join(atendentes[i], NULL);
  }

  pthread_mutex_destroy(&mutexFila);
  pthread_cond_destroy(&condFila);

  return 0;
}

void *inicializarAtendente(void *args)
{
  int atendente_id = *(int *)args;

  free(args);

  printf("=-=-=-=-=-=-= Atendente número %d abriu a bilheteria! =-=-=-=-=-=-=\n", atendente_id);

  while (1)
  {

    Cliente cliente;

    pthread_mutex_lock(&mutexFila);
    while (qntd_clientes_na_fila == 0)
    {
      printf("\n=-=-=-=-=-=-=-=-= [AGUARDANDO CLIENTES] =-=-=-=-=-=-=-=-=\n");
      pthread_cond_wait(&condFila, &mutexFila);
    }

    // Chama o primeiro cliente da fila
    cliente = fila[0];

    // Faz a fila "andar"
    for (int i = 0; i < qntd_clientes_na_fila - 1; i++)
    {
      fila[i] = fila[i + 1];
    }
    qntd_clientes_na_fila--;
    pthread_mutex_unlock(&mutexFila);

    // Atende o cliente
    atenderCliente(&cliente, atendente_id);
  }

  return NULL;
}

void *gerarClientes(void *args)
{
  int tempo_bilheteria = *(int *)args;
  int id_cliente = 1;

  while (tempo_bilheteria--)
  {
    Cliente novo_cliente;
    novo_cliente.id = id_cliente++;
    novo_cliente.assento_desejado = rand() % num_assentos;

    pthread_mutex_lock(&mutexFila);
    if (qntd_clientes_na_fila < 64)
    {
      fila[qntd_clientes_na_fila++] = novo_cliente;
      printf("Cliente %d entrou na fila e deseja o assento %d\n", novo_cliente.id, novo_cliente.assento_desejado);
      pthread_cond_signal(&condFila);
    }
    else
    {
      printf("Fila cheia! O cliente %d não conseguiu entrar. \n", novo_cliente.id);
    }
    pthread_mutex_unlock(&mutexFila);

    sleep(5);
  }

  return NULL;
}

void atenderCliente(Cliente *cliente, int atendente_id)
{
  printf("Atendente número %d atendendo cliente %d...\n", atendente_id, cliente->id);
  sleep(5);

  // Tenta adquirir o mutex do assento desejado
  if (pthread_mutex_trylock(&cinema[cliente->assento_desejado].mutexAssento) == 0)
  {
    printf("[ASSENTO %d VENDIDO PARA O CLIENTE %d]\n", cliente->assento_desejado, cliente->id);
  }
  else
  {
    printf("[O ASSENTO %d JÁ ESTÁ OCUPADO]\nO Cliente %d não conseguiu reservar e meteu o pé.\n", cliente->assento_desejado, cliente->id);
  }
}