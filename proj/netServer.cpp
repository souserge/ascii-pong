// Server
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>

using namespace std;

int handleServer()
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

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR*)&addr, addrLen);
    listen(sListen, SOMAXCONN);

    SOCKET newConnection;
    newConnection = accept(sListen, (SOCKADDR*)&addr, &addrLen);
    if (!newConnection) {
        printf("Failed to connect the client\n");
    }
    else {
        printf("Client Connected!\n");
    }

    getchar();
    return 0;
}
