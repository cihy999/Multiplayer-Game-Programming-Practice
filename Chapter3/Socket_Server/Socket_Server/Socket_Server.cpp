#include <WinSock2.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib") // 加入 lib 才能使用 WinSock2

using namespace std;

int main()
{
    cout << "初始化 Windows Sockets DLL" << endl;
    WSAData wsaData;
    WORD version = MAKEWORD(2, 2);
    int result = WSAStartup(version, &wsaData);
    if (result == 0)
    {
        cout << "result = " << result << "; Windows Sockets DLL 初始化成功" << endl;
    }
    else
    {
        cout << "result = " << result << "; Windows Sockets DLL 初始化失敗" << endl;
    }
}