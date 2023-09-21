#include "ApiC.h"
#include "NetworkingC.h"

int main(int argc, char* argv[]) {
    InitializeNetwork();    //Ініціалізуємо WSA

    SOCKET Connection = ConnectToServer();    //Створюємо з'єднання з сервером

    while (true) {    
        std::thread clientThread(GetUserInput, Connection);    //Створюємо потік
        clientThread.join();                                   //Приєднуємося до потоку
    }

    CleanupNetwork(Connection);    //Чистимо залишки роботи з мережею(сокет, WSA)

    return 0;
}
