#include "../../include/common/common.h"
#include "../../include/server/server.h"

int main()
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(clientAddr);

    Dictionary dictionary;
    dictionary.initialized = false;

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1)
    {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    system("clear");
    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        // Accept incoming connection
        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize)) == -1)
        {
            perror("Error accepting connection");
            exit(EXIT_FAILURE);
        }

        printf("Client connected: %s\n", inet_ntoa(clientAddr.sin_addr));

        // Create child process to handle client
        if (fork() == 0)
        {
            close(serverSocket); // Child process doesn't need the listener socket
            handleClient(clientSocket, &dictionary);
            close(clientSocket);
            exit(EXIT_SUCCESS);
        }

        close(clientSocket); // Parent process doesn't need the client socket
    }

    return 0;
}
