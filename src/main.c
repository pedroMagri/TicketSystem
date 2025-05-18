#include "../includes/sistema.h"
#include "../includes/atendimento.h"
#include "../includes/entrada.h"
#include "../includes/gerador.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  srand(time(NULL));
  pthread_t atendentes[NUM_ATENDENTES];
  pthread_t geradorClientes;
  pthread_t threadEntrada;
  int tempo_bilheteria = 60;

  inicializarAssentos();

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

  pthread_create(&threadEntrada, NULL, entradaUsuario, &tempo_bilheteria);
  pthread_create(&geradorClientes, NULL, gerarClientes, &tempo_bilheteria);

  pthread_join(geradorClientes, NULL);
  pthread_join(threadEntrada, NULL);
  printf("[SISTEMA] Horário da bilheteria encerrado.\n");
  encerrarSistema();

  for (int i = 0; i < NUM_ATENDENTES; i++)
  {
    pthread_join(atendentes[i], NULL);
  }

  pthread_mutex_destroy(&mutexFila);
  pthread_cond_destroy(&condFila);

  return 0;
}