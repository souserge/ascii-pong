//Client
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>

using namespace std;

int handleClient()
{
    WSAData wsaData;
    WORD DllVersion = MAKEWORD(2, 1);
    if (WSAStartup(DllVersion, &wsaData) != 0) {
        MessageBoxA(NULL, "Winsock failed", "Error", MB_OK | MB_ICONERROR);
        exit(1);
    }

    SOCKADDR_IN addr;
    int addrLen = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    SOCKET connection =  socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(connection, (SOCKADDR*)&addr, addrLen) != 0) {
        MessageBoxA(NULL, "Failed to connect", "Error", MB_OK | MB_ICONERROR);
    }
    else {
        printf("Connected!\n");
    }
    return 0;
}
