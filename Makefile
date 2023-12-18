# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRCDIR = source
OBJDIR = obj
BINDIR = bin

CLIENT_SRCDIR = $(SRCDIR)/client
SERVER_SRCDIR = $(SRCDIR)/server

CLIENT_OBJDIR = $(OBJDIR)/client
SERVER_OBJDIR = $(OBJDIR)/server

CLIENT_SOURCES = $(wildcard $(CLIENT_SRCDIR)/*.c)
SERVER_SOURCES = $(wildcard $(SERVER_SRCDIR)/*.c)

CLIENT_OBJECTS = $(patsubst $(CLIENT_SRCDIR)/%.c, $(CLIENT_OBJDIR)/%.o, $(CLIENT_SOURCES))
SERVER_OBJECTS = $(patsubst $(SERVER_SRCDIR)/%.c, $(SERVER_OBJDIR)/%.o, $(SERVER_SOURCES))

CLIENT_EXECUTABLE = $(BINDIR)/client
SERVER_EXECUTABLE = $(BINDIR)/server

.PHONY: all clean

all: $(CLIENT_EXECUTABLE) $(SERVER_EXECUTABLE)

$(CLIENT_EXECUTABLE): $(CLIENT_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

$(SERVER_EXECUTABLE): $(SERVER_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

$(CLIENT_OBJDIR)/%.o: $(CLIENT_SRCDIR)/%.c
	@mkdir -p $(CLIENT_OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER_OBJDIR)/%.o: $(SERVER_SRCDIR)/%.c
	@mkdir -p $(SERVER_OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

run_client: $(CLIENT_EXECUTABLE)
	./$(CLIENT_EXECUTABLE)

run_server: $(SERVER_EXECUTABLE)
	./$(SERVER_EXECUTABLE)

clean:
	rm -rf $(OBJDIR) $(BINDIR)