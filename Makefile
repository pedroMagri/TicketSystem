CC = gcc
CFLAGS = -Wall -pthread
TARGET = TicketSystem
SRC = ./src/ticketSystem.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
