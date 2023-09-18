#include "NetworkingS.h"
#include "ApiS.h"
#include <thread>

int main(int argc, char* argv[]) {
    // Ініціалізація бібліотеки WSA
    if (!InitializeWinsock()) {
        std::cout << "Error initializing Winsock" << std::endl;
        return 1;
    }

    // Створення сокету
    SOCKET serverSocket = CreateSocket();
    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Error creating socket" << std::endl;
        CleanupWinsock();
        return 1;
    }

    // Прив'язка сокету до порту
    if (!BindSocket(serverSocket, "127.0.0.1", 1111)) {
        std::cout << "Error binding socket" << std::endl;
        CloseSocket(serverSocket);
        CleanupWinsock();
        return 1;
    }

    // Прослуховування з'єднань
    if (!ListenSocket(serverSocket)) {
        std::cout << "Error listening on socket" << std::endl;
        CloseSocket(serverSocket);
        CleanupWinsock();
        return 1;
    }

    while (true) {
        std::cout << "Waiting for connection..." << std::endl;

        SOCKADDR_IN clientAddress;
        int clientAddressSize = sizeof(clientAddress);
        SOCKET clientSocket = AcceptSocket(serverSocket, clientAddress, clientAddressSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "Error accepting client connection" << std::endl;
        }
        else {
            std::cout << "Client Connected!\n";

            // Створення окремого потоку для обробки клієнтського запиту
            std::thread clientThread(HandleClient, clientSocket);
            clientThread.detach(); // Відокремлення потоку, щоб він працював автономно
            // Без з'єднання цикл продовжить слухати нові підключення
        }
    }

    CloseSocket(serverSocket);
    CleanupWinsock();

    return 0;
}