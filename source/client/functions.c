#include <ctype.h>

#include "../../include/client/client.h"
#include "../../include/common/common.h"

char acceptedInputCommands[] = {
    'i',      // Initialize dictionary
    'a', 'A', // Add word
    'd',      // Add definition
    'g',      // Get definition
    'w',      // Get whole dictionary
    's',      // Delete word
    'e'       // Exit
};

// void sendRequest(int socket, const commandHandler *request)
void sendRequest(int socket, const void *request)
{
    send(socket, request, MAX_BUFFER_SIZE, 0);
}

bool isValidWord(const char *word)
{
    for (int i = 0; word[i] != '\0'; i++)
        if (!isalpha(word[i]))
            return false;

    return true;
}

bool isValidInputCommand(char *input)
{
    // check if input command is valid
    if (strchr(acceptedInputCommands, input[0]) == NULL)
    {
        printf("\nInvalid input, nu such know command: %c.\n\n", input[0]);
        return false;
    }

    if (input[0] != 'i' && input[0] != 'e' && input[0] != 'w')
    {
        // check if input command has a word after it
        if (input[1] != ' ')
        {
            printf("\nInvalid input, no word provided after the command.\n\n");
            return false;
        }

        // check if the word is valid
        if (!isValidWord(input + 2))
        {
            printf("\nInvalid input, word is not valid: %s.\n\n", input + 2);
            return false;
        }
    }

    return true;
}

void printMenu()
{
    printf("\nEnter command:\n");
    printf("i\t\t->\tInitialize dictionary\n");
    printf("a|A\t<word>\t->\tAdd word\n");
    printf("d\t<word>\t->\tAdd definition\n");
    printf("g\t<word>\t->\tGet definition\n");
    printf("w\t\t->\tGet whole dictionary\n");
    printf("s\t<word>\t->\tDelete word\n");
    printf("e\t\t->\tExit\n");
    printf("\n>> ");
}

commandHandler manageInput()
{
    static char buffer[1024];
    commandHandler inCommand;

    printMenu();
    fgets(buffer, sizeof(buffer), stdin);

    // Remove newline character at the end of the input
    buffer[strcspn(buffer, "\n")] = 0;

    if (!isValidInputCommand(buffer))
        return manageInput();

    // fill the commandHandler struct var
    if (buffer[0] == 'i' || buffer[0] == 'e' || buffer[0] == 'w')
    {
        inCommand.command = buffer[0];
        strcpy(inCommand.argument, "");
    }
    else
    {
        inCommand.command = buffer[0];
        strcpy(inCommand.argument, buffer + 2);
    }

    return inCommand;
}

char handleServerResponse(const char *response)
{
    printf("\nServer response: %s\n", response);

    if (strncmp(response, "REQUEST:", 8) == 0)
    {
        if (strncmp(response + 8, "EXIT", 4) == 0)
            return 'e';

        if (strncmp(response + 8, "DEFINITION", 10) == 0)
            return 'd';
    }

    return 'n'; // nothing - default
}

bool checkDefinition(const char *input)
{
    if (!isalnum(input[0]))
    {
        printf("\nInvalid word definition, try again...\n\n");
        return false;
    }

    return true;
}

void addDefinition(int socket)
{
    char buffer[MAX_BUFFER_SIZE] = {0};
    while (1)
    {
        printf(">> ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);
        if (checkDefinition(buffer))
            break;
    }
    sendRequest(socket, buffer);
    recv(socket, buffer, MAX_BUFFER_SIZE, 0); // Wait for server response
    printf("Server response: %s\n", buffer);
}
