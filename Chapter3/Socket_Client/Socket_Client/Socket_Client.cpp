#include <WinSock2.h>   // Linker：專案 屬性/連結器/輸入/其他相依性，加入Ws2_32.lib才能使用socket
#include <iostream>

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