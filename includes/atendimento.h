#ifndef ATENDIMENTO_H
#define ATENDIMENTO_H

// Inicializa a thread atendente, mantém ela alerta para clientes na fila.
void *inicializarAtendente(void *args);

// Interações com a fila de clientes e assentos do Cinema.
void atenderCliente(Cliente *cliente, int atendente_id);

#endif
