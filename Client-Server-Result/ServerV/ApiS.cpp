#include "ApiS.h"
#include "NetworkingS.h"
#ifdef linux
#include "signal.h"
#include "cwchar"
#include <limits.h>
#endif

const int PROCESS_LIST_BUFFER_SIZE = 4000;

void ConvertWCHARtoChar(const wchar_t* wideStr, char* charStr, int charStrSize) {
    wcstombs(charStr, wideStr, charStrSize);
}

void GetProcessList(wchar_t* msg, int bufferSize) {
    // Function retrieves a list of running processes on the system and stores it in 'msg'
    
#ifdef _WIN32
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        // Unable to create a process snapshot
        return;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    msg[0] = L'\0';

    if (Process32First(hProcessSnap, &pe32)) {
        // Add the first process to the 'msg'
        swprintf(msg, bufferSize, L"%d: %s\n", pe32.th32ProcessID, pe32.szExeFile);

        while (Process32Next(hProcessSnap, &pe32)) {
            // Add the next process to the 'msg'
            wcscat_s(msg, bufferSize, L"\n");
            swprintf(msg + wcslen(msg), bufferSize - wcslen(msg), L"%d: %s", pe32.th32ProcessID, pe32.szExeFile);
        }
    }

    CloseHandle(hProcessSnap);
#else
    // Iterate through the /proc directory to gather process information
    DIR* dp = opendir("/proc");
    if (!dp) {
        // Unable to open the /proc directory
        return;
    }

    struct dirent* dirp;
    while ((dirp = readdir(dp)) != NULL) {
        if (dirp->d_type == DT_DIR) {
            // Build the path to the cmdline file
            char path[PATH_MAX];
            snprintf(path, sizeof(path), "/proc/%s/cmdline", dirp->d_name);

            FILE* fp = fopen(path, "r");
            if (fp) {
                char cmd[255];
                fgets(cmd, sizeof(cmd), fp);
                fclose(fp);
                if (strlen(cmd) > 0) {
                    // Add the process information to 'msg'.
                    swprintf(msg + wcslen(msg), bufferSize - wcslen(msg), L"%d: %s%s", atoi(dirp->d_name), cmd, wcslen(msg) > 0 ? L"\n" : L"");
                }
            }
        }
    }

    closedir(dp);
#endif
}

void HandleClient(SOCKET clientSocket) {
    while (true) {
        wchar_t processListData[PROCESS_LIST_BUFFER_SIZE] = { 0 };
        GetProcessList(processListData, sizeof(processListData) / sizeof(processListData[0]));

        char processList[PROCESS_LIST_BUFFER_SIZE];
        ConvertWCHARtoChar(processListData, processList, sizeof(processList));

        send(clientSocket, processList, sizeof(processList), 0);

        char buffer[255];
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';

            printf("Client sent ID: %s\n", buffer);

            int pid = atoi(buffer);

#ifdef _WIN32
            bool isProcessTerminated = TerminateProcess(OpenProcess(PROCESS_TERMINATE, FALSE, pid), 0) != 0;
#else
            bool isProcessTerminated = kill(pid, SIGTERM) == 0;
#endif

            send(clientSocket, reinterpret_cast<char*>(&isProcessTerminated), sizeof(isProcessTerminated), 0);

            if (isProcessTerminated) {
                printf("Process %d closed successfully!\n", pid);
            }
            else {
                printf("Error: failed to terminate process %d\n", pid);
            }
        }
    }

    CloseSocket(clientSocket);
}
