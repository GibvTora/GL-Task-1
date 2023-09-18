#include "NetworkingC.h"

#pragma warning(disable: 4996)

void InitializeNetwork() {
    // Ініціалізація бібліотеки WSA
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error" << std::endl;
        exit(1);
    }
}

SOCKET ConnectToServer() {
    // Створення сокету та підключення до сервера
    SOCKADDR_IN address;
    int sizeofaddr = sizeof(address);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(1111);
    address.sin_family = AF_INET;

    SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(Connection, (SOCKADDR*)&address, sizeof(address)) != 0) {
        std::cout << "Error: failed connect to server.\n";
        exit(1);
    }
    std::cout << "Connected!\n";

    return Connection;
}

void CleanupNetwork(SOCKET conn) {
    // Закриття сокету та очищення бібліотеки WSA
    closesocket(conn);
    WSACleanup();
}
