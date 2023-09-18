#include "NetworkingS.h"

bool InitializeWinsock() {
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    return WSAStartup(DLLVersion, &wsaData) == 0;
}

void CleanupWinsock() {
    WSACleanup();
}

SOCKET CreateSocket() {
    return socket(AF_INET, SOCK_STREAM, NULL);
}

void CloseSocket(SOCKET socket) {
    closesocket(socket);
}

bool BindSocket(SOCKET socket, const char* ipAddress, int port) {
    SOCKADDR_IN address;
    int sizeofaddress = sizeof(address);
    address.sin_addr.s_addr = inet_addr(ipAddress);
    address.sin_port = htons(port);
    address.sin_family = AF_INET;
    return bind(socket, (SOCKADDR*)&address, sizeof(address)) == 0;
}

bool ListenSocket(SOCKET socket) {
    return listen(socket, SOMAXCONN) == 0;
}

SOCKET AcceptSocket(SOCKET serverSocket, SOCKADDR_IN& clientAddress, int& clientAddressSize) {
    return accept(serverSocket, (SOCKADDR*)&clientAddress, &clientAddressSize);
}