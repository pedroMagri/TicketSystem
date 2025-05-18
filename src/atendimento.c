#include "../includes/sistema.h"
#include "../includes/atendimento.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *inicializarAtendente(void *args)
{
  int atendente_id = *(int *)args;

  free(args);

  printf("[ATENDENTE %d] Bilheteria aberta!\n", atendente_id);

  while (1)
  {

    Cliente cliente;

    pthread_mutex_lock(&mutexFila);
    while (qntd_clientes_na_fila == 0 && sistema_ativo)
    {
      printf("[ATENDENTE %d] Aguardando clientes.\n", atendente_id);
      pthread_cond_wait(&condFila, &mutexFila);
    }

    if (!sistema_ativo)
    {
      pthread_mutex_unlock(&mutexFila);
      printf("[ATENDENTE %d] Encerrando atividades.\n", atendente_id);
      return NULL;
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

void atenderCliente(Cliente *cliente, int atendente_id)
{
  printf("[ATENDENTE %d] Atendendo cliente %d. \n", atendente_id, cliente->id);
  sleep(5);

  // Tenta adquirir o mutex do assento desejado
  if (pthread_mutex_trylock(&cinema[cliente->assento_desejado].mutexAssento) == 0)
  {
    printf("[ATENDENTE %d] Assento %d vendido para o cliente %d.\n", atendente_id, cliente->assento_desejado, cliente->id);
  }
  else
  {
    printf("[ATENDENTE %d] O assento %d já está ocupado.\n [CLIENTE %d] O cliente não conseguiu reservar e meteu o pé.\n", atendente_id, cliente->assento_desejado, cliente->id);
  }
}