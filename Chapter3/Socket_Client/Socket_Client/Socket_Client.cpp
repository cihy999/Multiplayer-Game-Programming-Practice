#include <WinSock2.h>   // Linker：專案 屬性/連結器/輸入/其他相依性，加入Ws2_32.lib才能使用socket
#include <iostream>
#include <cassert>

using namespace std;

const char IP_NUM[] = "127.0.0.1";
const WORD PORT_NUM = 9527;

int main()
{
    int result;

    cout << "1. 初始化 Windows Sockets DLL" << endl;
    WSAData wsaData;
    WORD version = MAKEWORD(2, 2);
    result = WSAStartup(version, &wsaData);
    if (result == 0) 
    {
        cout << "result = " << result << "; Windows Sockets DLL 初始化成功..." << endl;
    }
    else 
    {
        cout << "result = " << result << "; Windows Sockets DLL 初始化失敗..." << endl;
    }

    cout << "\n2.建立 連線Socket" << endl;
    SOCKET sConnect = socket(AF_INET, SOCK_STREAM, NULL);

    cout << "設定 Server IP:Port...";
    SOCKADDR_IN addr;
    memset(&addr, 0, sizeof(addr));                 // 清空,將資料設為 0    
    addr.sin_addr.s_addr = inet_addr(IP_NUM);       // 設定 IP
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_NUM);                // 設定 port

    cout << "開始連線...";
    result = connect(sConnect, (SOCKADDR*)&addr, sizeof(addr));
    if (result != INVALID_SOCKET)
    {
        cout << "連線成功！" << endl;
    }
    else
    {
        cout << "連線失敗！error: " << WSAGetLastError() << endl;
        closesocket(sConnect);
        WSACleanup();
        return 1;
    }

    cout << "\n3.傳送一個訊息..." << endl;
    char sendbuf[] = "sending data test";
    result = send(sConnect, sendbuf, sizeof(sendbuf), 0);
    if (result != SOCKET_ERROR)
    {
        cout << "傳送出去了." << endl;
    }
    else
    {
        cout << "傳送失敗！error: " << WSAGetLastError() << endl;
        closesocket(sConnect);
        WSACleanup();
        return 1;
    }

    cout << "\n4.接收訊息..." << endl;
    char message[200];
    ZeroMemory(message, 200);
    result = recv(sConnect, message, sizeof(message), 0);
    if (result > 0)
    {
        cout << "來自： " << message << endl;
    }
    else if (result == 0)
    {
        cout << "連線已關閉！" << endl;
        closesocket(sConnect);
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "連線失敗！error: " << WSAGetLastError() << endl;
        closesocket(sConnect);
        WSACleanup();
        return 1;
    }

    cout << "關閉連線." << endl;
    closesocket(sConnect);
    WSACleanup();
}