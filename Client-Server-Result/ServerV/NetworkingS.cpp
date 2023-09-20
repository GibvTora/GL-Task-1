#include "NetworkingS.h"

bool InitializeNetworkLibrary() {
#ifdef _WIN32
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 2);
    return WSAStartup(DLLVersion, &wsaData) == 0;
#else
    // Ініціалізація для Linux, якщо потрібно
    return true;
#endif
}

void CleanupNetworkLibrary() {
#ifdef _WIN32
    WSACleanup();
#else
    // Завершення для Linux, якщо потрібно
#endif
}

SOCKET CreateSocket() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

void CloseSocket(SOCKET socket) {
    closesocket(socket);
}

bool BindSocket(SOCKET socket, const char* ipAddress, int port) {
    sockaddr_in address;
    address.sin_addr.s_addr = inet_addr(ipAddress);
    address.sin_port = htons(port);
    address.sin_family = AF_INET;
    return bind(socket, (sockaddr*)&address, sizeof(address)) == 0;
}

bool ListenSocket(SOCKET socket) {
    return listen(socket, SOMAXCONN) == 0;
}

SOCKET AcceptSocket(SOCKET serverSocket, sockaddr_in& clientAddress, int& clientAddressSize) {
    sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    SOCKET clientSocket;

#ifdef _WIN32
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddressSize);
#else
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, (socklen_t*)&clientAddressSize);
#endif

    if (clientSocket != INVALID_SOCKET) {
        clientAddress = clientAddr;
    }

    return clientSocket;
}