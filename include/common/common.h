#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERV_HOST_ADDR "127.0.0.1"
#define MAX_BUFFER_SIZE 1024

// - 2 for space and the rest of word
typedef struct {
    char command; // acceptedInputCommands
    char argument[100]; // argument = word
} commandHandler;

#endif // COMMON_H