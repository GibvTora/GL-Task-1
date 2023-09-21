#include "ApiC.h"
#include "NetworkingC.h"

int main(int argc, char* argv[]) {
    InitializeNetwork();

    SOCKET Connection = ConnectToServer();

    while (true) {
        std::thread clientThread(GetUserInput, Connection);
        clientThread.join();
    }

    CleanupNetwork(Connection);

    return 0;
}