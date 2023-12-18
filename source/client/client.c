#include "../../include/client/client.h"

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    serverAddr.sin_port = htons(PORT);

    // Connect to server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    system("clear");
    printf("Connected to server on: %s:%d\n\n", SERV_HOST_ADDR, PORT);

    char buffer[MAX_BUFFER_SIZE] = {0};

    while (1)
    {
        // Get user input
        commandHandler inCommand = manageInput();

        // Send request to server
        sendRequest(clientSocket, &inCommand);

        // Receive and print server response
        recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);

        switch (handleServerResponse(buffer))
        {
        case 'e':
            close(clientSocket);
            exit(EXIT_SUCCESS);

        case 'd':
            addDefinition(clientSocket);
            break;

        default:
            // do nothing
            break;
        }

        // Clear buffer
        memset(buffer, 0, sizeof(buffer));
    }

    // Close socket
    close(clientSocket);

    return 0;
}
