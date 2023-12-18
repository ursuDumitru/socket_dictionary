#include "../../include/server/server.h"
#include "../../include/common/common.h"

void initializeDictionary(Dictionary *dict)
{
    dict->initialized = true;
    dict->count = 0;
    // clear all words and definitions
    for (int i = 0; i < MAX_DICT_SIZE; ++i)
    {
        strcpy(dict->entries[i].word, "");
        strcpy(dict->entries[i].definition, "");
    }
}

int findWordIndex(Dictionary *dict, const char *word)
{
    for (int i = 0; i < dict->count; ++i)
        if (strcmp(dict->entries[i].word, word) == 0)
            return i;

    return -1; // Word not found
}

void addWord(Dictionary *dict, const char *word)
{
    if (dict->count < MAX_DICT_SIZE)
    {
        strcpy(dict->entries[dict->count].word, word);
        strcpy(dict->entries[dict->count].definition, "");
        dict->count++;
    }
}

void addDefinition(Dictionary *dict, const char *word, const char *definition)
{
    int index = findWordIndex(dict, word);
    if (index != -1)
        strcpy(dict->entries[index].definition, definition);
}

void deleteWord(Dictionary *dict, const int index)
{
    // Shift elements to fill the gap
    for (int i = index; i < dict->count - 1; ++i)
    {
        strcpy(dict->entries[i].word, dict->entries[i + 1].word);
        strcpy(dict->entries[i].definition, dict->entries[i + 1].definition);
    }
    dict->count--;
}

void handleClient(int clientSocket, Dictionary *dict)
{
    char buffer[MAX_BUFFER_SIZE] = {0};
    int bytesReceived;
    commandHandler inCommand;

    while (1)
    {
        // TODO - make client send a message when it disconnects
        //      - implement a way for the client to get a word with its definition
        //      - implement a way for the client to get all words with their definitions
        //      - add the word in the server response to the client

        // Receive client request
        bytesReceived = recv(clientSocket, &inCommand, MAX_BUFFER_SIZE, 0);

        if(bytesReceived == 0)
        {
            printf("Client disconnected\n");
            break;
        }

        // Process client request
        if (inCommand.command == 'i')                                   // initialize dictionary
        {
            // Initialize dictionary
            initializeDictionary(dict);

            // Send response to client
            send(clientSocket, "Dictionary initialized succesfully", MAX_BUFFER_SIZE, 0);
        }
        else if (inCommand.command == 'a' || inCommand.command == 'A')  // add word
        {
            if (dict->initialized == false)
            {
                // Send error response to client
                send(clientSocket, "Dictionary is not initialized", MAX_BUFFER_SIZE, 0);
                continue;
            }

            if (dict->count == MAX_DICT_SIZE)
            {
                // Send error response to client
                send(clientSocket, "Dictionary is full", MAX_BUFFER_SIZE, 0);
                continue;
            }

            if (findWordIndex(dict, inCommand.argument) == -1)
            {
                // Add word
                addWord(dict, inCommand.argument);

                // Send response to client
                sprintf(buffer, "Word <%s> added succesfully", inCommand.argument);
                send(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
            }
            else
            {
                // Send error response to client
                sprintf(buffer, "Word <%s> already exists", inCommand.argument);
                send(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
                continue;
            }
        }
        else if (inCommand.command == 'd')                              // add definition
        {
            if (dict->initialized == false)
            {
                // Send error response to client
                send(clientSocket, "Dictionary is not initialized", MAX_BUFFER_SIZE, 0);
                continue;
            }

            if (findWordIndex(dict, inCommand.argument) == -1)
            {
                // Send error response to client
                sprintf(buffer, "Word <%s> does not exist", inCommand.argument);
                send(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
                continue;
            }
            else
            {
                // make a request back at the client to write a word definition
                sprintf(buffer, "REQUEST:DEFINITION, Please enter a definition for the word <%s>:", inCommand.argument);
                send(clientSocket, buffer, MAX_BUFFER_SIZE, 0);

                // recieve definition
                recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
                addDefinition(dict, inCommand.argument, buffer);

                // Send response to client
                sprintf(buffer, "Definition for word <%s> added succesfully", inCommand.argument);
                send(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
            }
        }
        else if (inCommand.command == 'g')                                  // get definition
        {
            if (dict->initialized == false)
            {
                // Send error response to client
                send(clientSocket, "Dictionary is not initialized", MAX_BUFFER_SIZE, 0);
                continue;
            }

            if (dict->count == 0)
            {
                // Send error response to client
                send(clientSocket, "Dictionary does not contain any words", MAX_BUFFER_SIZE, 0);
                continue;
            }

            int index = findWordIndex(dict, inCommand.argument);
            if (index == -1)
            {
                // Send error response to client
                sprintf(buffer, "Word <%s> does not exist", inCommand.argument);
                send(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
                continue;
            }
            else
            {
                // Send response to client
                sprintf(buffer, "Definition for word <%s>: %s", inCommand.argument, dict->entries[index].definition);
                send(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
            }
        }
        else if (inCommand.command == 'w')                                  // list all words
        {
            if (dict->initialized == false)
            {
                // Send error response to client
                send(clientSocket, "Dictionary is not initialized", MAX_BUFFER_SIZE, 0);
                continue;
            }

            if (dict->count == 0)
            {
                // Send error response to client
                send(clientSocket, "Dictionary does not contain any words", MAX_BUFFER_SIZE, 0);
                continue;
            }

            // Send response to client
            sprintf(buffer, "Words in dictionary: ");
            for (int i = 0; i < dict->count; ++i)
            {
                strcat(buffer, dict->entries[i].word);
                strcat(buffer, ", ");
            }
            send(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
        }
        else if (inCommand.command == 's')                                  // delete word
        {
            if (dict->initialized == false)
            {
                // Send error response to client
                send(clientSocket, "Dictionary is not initialized", MAX_BUFFER_SIZE, 0);
                continue;
            }

            int index = findWordIndex(dict, inCommand.argument);
            if (index == -1)
            {
                // Send error response to client
                sprintf(buffer, "Word <%s> does not exist", inCommand.argument);
                send(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
                continue;
            }
            else
            {
                // Delete word
                deleteWord(dict, index);

                // Send response to client
                sprintf(buffer, "Word <%s> deleted succesfully", inCommand.argument);
                send(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
            }
        }
        else if (inCommand.command == 'e')                                  // exit
        {
            // Send response to client
            send(clientSocket, "REQUEST:EXIT, goodbye!", MAX_BUFFER_SIZE, 0);

            // Close connection
            // break;
        }
        else
        {
            // Send error response to client
            sprintf(buffer, "Invalid command <%c>", inCommand.command);
            send(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
            continue;
        }

        // Clear input Command
        memset(&inCommand, 0, sizeof(commandHandler));
        memset(buffer, 0, sizeof(buffer));
    }
}