#include "../includes/sistema.h"
#include "../includes/gerador.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

void *gerarClientes(void *args)
{
  int tempo_bilheteria = *(int *)args;
  int id_cliente = 1;
  unsigned int seed = time(NULL) ^ pthread_self();
  time_t inicio = time(NULL);

  while (time(NULL) - inicio < tempo_bilheteria)
  {
    Cliente novo_cliente;
    novo_cliente.id = id_cliente++;
    novo_cliente.assento_desejado = rand_r(&seed) % num_assentos;

    pthread_mutex_lock(&mutexFila);
    if (qntd_clientes_na_fila < 64)
    {
      fila[qntd_clientes_na_fila++] = novo_cliente;
      printf("[CLIENTE %d] Entrou na fila e deseja o assento %d. \n", novo_cliente.id, novo_cliente.assento_desejado);
      pthread_cond_signal(&condFila);
    }
    else
    {
      printf("[CLIENTE %d] Fila cheia! O cliente não conseguiu entrar. \n", novo_cliente.id);
    }
    pthread_mutex_unlock(&mutexFila);

    int tempo_aleatorio = (rand_r(&seed) % 5) + 1;
    sleep(tempo_aleatorio);
  }

  return NULL;
}
