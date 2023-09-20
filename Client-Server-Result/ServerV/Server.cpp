#include "NetworkingS.h"
#include "ApiS.h"
#include <thread>

int main(int argc, char* argv[]) {
    if (!InitializeNetworkLibrary()) {
        printf("Error initializing network library\n");
        return 1;
    }

    SOCKET serverSocket = CreateSocket();
    if (serverSocket == INVALID_SOCKET) {
        printf("Error creating socket\n");
        CleanupNetworkLibrary();
        return 1;
    }

    if (!BindSocket(serverSocket, "127.0.0.1", 1111)) {
        printf("Error binding socket\n");
        CloseSocket(serverSocket);
        CleanupNetworkLibrary();
        return 1;
    }

    if (!ListenSocket(serverSocket)) {
        printf("Error listening on socket\n");
        CloseSocket(serverSocket);
        CleanupNetworkLibrary();
        return 1;
    }

    while (true) {
        printf("Waiting for connection...\n");

        sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);
        SOCKET clientSocket = AcceptSocket(serverSocket, clientAddress, clientAddressSize);
        if (clientSocket == INVALID_SOCKET) {
            printf("Error accepting client connection\n");
        }
        else {
            printf("Client Connected!\n");

            std::thread clientThread(HandleClient, clientSocket);
            clientThread.detach();
        }
    }

    CloseSocket(serverSocket);
    CleanupNetworkLibrary();

    return 0;
}
