CC = gcc
CFLAGS = -Wall -pthread

OBJ = main.o atendimento.o entrada.o gerador.o sistema.o

EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

main.o: src/main.c includes/sistema.h includes/atendimento.h includes/entrada.h includes/gerador.h
	$(CC) $(CFLAGS) -c src/main.c

atendimento.o: src/atendimento.c includes/sistema.h includes/atendimento.h
	$(CC) $(CFLAGS) -c src/atendimento.c

entrada.o: src/entrada.c includes/sistema.h includes/entrada.h
	$(CC) $(CFLAGS) -c src/entrada.c

gerador.o: src/gerador.c includes/sistema.h includes/gerador.h
	$(CC) $(CFLAGS) -c src/gerador.c

sistema.o: src/sistema.c includes/sistema.h
	$(CC) $(CFLAGS) -c src/sistema.c

clean:
	rm -f $(EXEC) $(OBJ)