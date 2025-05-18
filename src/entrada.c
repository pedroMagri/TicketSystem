#include "../includes/sistema.h"
#include "../includes/entrada.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include <time.h>

void *entradaUsuario(void *args)
{
  int tempo_bilheteria = *(int *)args;
  time_t inicio = time(NULL);
  int id_cliente_manual = 10000; // IDs manuais começam em 10000 para não conflitar
  fd_set readfds;
  struct timeval timeout;
  printf("[SISTEMA] Para adicionar clientes manualmente digite comando (ex: add 15):\n\n\n\n");
  
  while (time(NULL) - inicio < tempo_bilheteria)
  {

    // Configura o timeout para 1 segundo
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    int ready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

    if (ready == -1)
    {
      perror("select");
      break;
    }
    else if (ready == 0)
    {
      // Timeout, verifica se o tempo de bilheteria ainda está ativo
      continue;
    }

    char buffer[128];
    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
      // Remove o '\n' do final
      buffer[strcspn(buffer, "\n")] = 0;

      // Pega o comando (primeira palavra)
      char *comando = strtok(buffer, " ");
      if (comando != NULL && strcmp(comando, "add") == 0)
      {
        char *quant_str = strtok(NULL, " ");
        if (quant_str != NULL)
        {
          int quantidade = atoi(quant_str);
          if (quantidade > 0)
          {
            pthread_mutex_lock(&mutexFila);
            int adicionados = 0;
            for (int i = 0; i < quantidade; i++)
            {
              if (qntd_clientes_na_fila >= 64)
              {
                printf("\n[SISTEMA] Fila cheia! Foram adicionados %d clientes até agora.\n", adicionados);
                break;
              }
              Cliente novo_cliente;
              novo_cliente.id = id_cliente_manual++;
              novo_cliente.assento_desejado = rand() % num_assentos;

              fila[qntd_clientes_na_fila++] = novo_cliente;
              adicionados++;
            }
            if (adicionados > 0)
            {
              printf("\n[SISTEMA] Adicionados %d clientes na fila.\n", adicionados);
              pthread_cond_signal(&condFila); // Acorda atendente se estiver esperando
            }
            pthread_mutex_unlock(&mutexFila);
          }
          else
          {
            printf("\n[SISTEMA] Quantidade inválida.\n");
          }
        }
        else
        {
          printf("\n[SISTEMA] Use: add <quantidade>\n");
        }
      }
      else
      {
        printf("\n[SISTEMA] Comando inválido. Use: add <quantidade>\n");
      }
    }
  }
  return NULL;
}