#include "NetworkingC.h"

const int PORT = 1111;
const char IP_ADDRESS[] = ("127.0.0.1");

#ifdef _WIN32 // Для Windows
void InitializeNetwork() {    //Ініціалізуємо бібліотеку WSA для роботи з мережею
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        exit(1);
    }
}

SOCKET ConnectToServer() {   //Створюємо сокет та з'язок з сервером
    SOCKET Connection = socket(AF_INET, SOCK_STREAM, 0);
    if (Connection == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        exit(1);
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    if (connect(Connection, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != 0) {
        std::cerr << "Failed to connect to server" << std::endl;
        closesocket(Connection);
        WSACleanup();
        exit(1);
    }

    std::cout << "Connected to server" << std::endl;
    return Connection;
}

void CleanupNetwork(SOCKET conn) {    //Чистимо сокет та WSA
    closesocket(conn);
    WSACleanup();
}
#else // Для Linux
void InitializeNetwork() {
    // Ініціалізація WSA не потрібна на Linux
}

SOCKET ConnectToServer() {    //Створюємо сокет та з'язок з сервером
    SOCKET Connection = socket(AF_INET, SOCK_STREAM, 0);
    if (Connection == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(1111);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(Connection, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != 0) {
        perror("Failed to connect to server");
        close(Connection);
        exit(1);
    }

    std::cout << "Connected to server" << std::endl;
    return Connection;
}

void CleanupNetwork(SOCKET conn) {
    close(conn);
}
#endif
