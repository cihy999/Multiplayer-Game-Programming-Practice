#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
    WSADATA wsaData;
    int nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address;
    memset(address.sin_zero, 0, sizeof(address.sin_zero));
    address.sin_family = AF_INET;
    address.sin_port = htons(80);
    address.sin_addr.S_un.S_un_b.s_b1 = 65;
    address.sin_addr.S_un.S_un_b.s_b2 = 254;
    address.sin_addr.S_un.S_un_b.s_b3 = 248;
    address.sin_addr.S_un.S_un_b.s_b4 = 180;

    shutdown(tcpSocket, SD_BOTH);
    closesocket(tcpSocket);

    WSACleanup();
}
