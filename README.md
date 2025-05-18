# TicketSystem
Um sistema de bilheteria multithread para simulação de venda de ingressos com filas, atendentes e interação em tempo real via terminal.

## Descrição
O sistema simula as interações de uma bilheteria de Cinema entre 4 atendentes e uma fila de clientes, os clientes são gerados automáticamente e aleatoriamente. O cliente chega entra na fila decidido do assento que irá escolher, se o assento não estiver disponível o cliente vai embora.

O usuário pode simular um número específico de clientes que entrarão na fila juntos a partir do comando "add <Número de Clientes>" com a finalidade de testar o funcionamento paralelo dos atendentes.

## Estrutura do Projeto
```
├── Makefile
├── README.md
├── includes
|   ├── atendimento.h   Declaração das funções usadas pelas threads Atendentes
|   ├── entrada.h       Definição da função de tratamento de entrada do Usuário
|   ├── gerador.h       Declaração da função utilizada pela thread geradora de Clientes
|   └── sistema.h       Declaração de estruturas de dados, variáveis globais e funções do sistema
└── src
    ├── main.c          Função principal de execução do programa
    ├── atendimento.c   Implantação das funções usadas pelas threads Atendentes
    ├── entrada.c       Implantação da função de tratamento de entrada do Usuário
    ├── gerador.c       Implatançaão da função utilizada pela thread geradora de Clientes
    └── sistema.c       Implantação das funções de controle de execução do sistema
```

## Compilação e Execução

No diretório raiz do programa compile o programa usando o Makefile
```bash
make
```
Execute o programa
```bash
./main
```

## Funcionalidades
O sistema conta com as seguintes funcionalidades

- `Sistema de assentos com mutex para garantir exclusão mútua.`

- `Fila de clientes com controle de concorrência usando mutex e variáveis de condição.`

- `Múltiplos atendentes para processamento simultâneo de clientes.`

- `Entrada manual de clientes via terminal. ("add <Número de Clientes>}")`

## Exemplo de Caso de Uso

Inicie o programa:

```bash
./main
```

Aguarde a inicialização dos atendentes e a geração automática de clientes.

Adicione manualmente 15 clientes à fila com o comando:

```bash
add 15
```

Observe os atendentes chamando os clientes da fila e tentando reservar os assentos desejados.

## Estruturas de sincronização

- Mutex (pthread_mutex_t)

    > Utilizado para garantir exclusão mútua ao acessar os assentos do cinema e a fila de clientes.

- Variáveis de Condição (pthread_cond_t)

    > Utilizadas para suspender as threads atendentes enquanto a fila está vazia.

## Padrões de projeto

- Thread Pool (Pool de Threads)

    > Conjunto de threads atendentes que processam clientes de forma paralela para melhorar o desempenho do sistema.

- Suspensão Controlada

    > Uso de variáveis de condição para colocar as threads atendentes em espera quando a fila está vazia, reduzindo o uso desnecessário de CPU.

- Produto/Consumidor

    > O sistema segue o padrão Produtor/Consumidor, onde as funções gerarClientes() e entradaUsuario() produzem clientes que são consumidos pelas threads atendentes em inicializarAtendente().