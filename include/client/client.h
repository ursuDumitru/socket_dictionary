#ifndef CLIENT_H
#define CLIENT_H

#include "../../include/common/common.h"

extern char acceptedInputCommands[];

// void sendRequest(int socket, const commandHandler *request);
void sendRequest(int socket, const void *request);
commandHandler manageInput();
char handleServerResponse(const char *response);
bool checkDefinition(const char *input);
void addDefinition(int socket);

#endif /* CLIENT_H */
