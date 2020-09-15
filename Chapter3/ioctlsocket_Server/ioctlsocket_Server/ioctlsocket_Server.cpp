#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib") // 加入 lib 才能使用 WinSock2

using namespace std;

const char IP_NUM[] = "127.0.0.1";
const WORD PORT_NUM = 9527;

int main()
{
    int result;

    cout << "1. 初始化 Windows Sockets DLL." << endl;
    WSAData wsaData;
    WORD version = MAKEWORD(2, 2); //winsocket-dll version (最低版本, 最高版本)
    result = WSAStartup(version, &wsaData);
    if (result == 0)
    {
        cout << "Windows Sockets DLL 初始化成功." << endl;
    }
    else
    {
        cout << "result = " << result << "; Windows Sockets DLL 初始化失敗." << endl;
    }
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        cout << "找不到合適的 Winsock dll 版本." << endl;
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "Winsock 2.2 dll 版本已找到." << endl;
    }

    cout << "\n2. 建立 監聽Socket" << endl;
    SOCKET sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sListen == INVALID_SOCKET)
    {
        cout << "監聽Socket 建立失敗！" << endl;
        cout << "error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "建立成功！" << endl;
    }

    cout << "設置成非阻塞...";
    ULONG iMode = 1;
    result = ioctlsocket(sListen, FIONBIO, &iMode);
    if (result == SOCKET_ERROR) 
    {
        cout << "設置失敗！" << endl;
        cout << "error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "設置成功！" << endl;
    }

    cout << "設定 IP:Port...";
    SOCKADDR_IN addrListener;
    memset(&addrListener, 0, sizeof(addrListener));         // 清空,將資料設為 0    
    addrListener.sin_addr.s_addr = inet_addr(IP_NUM);       // 設定 IP
    addrListener.sin_family = AF_INET;
    addrListener.sin_port = htons(PORT_NUM);                // 設定 port

    cout << "綁定指定位址...";
    result = bind(sListen, (SOCKADDR*)&addrListener, sizeof(addrListener));
    if (result == SOCKET_ERROR)
    {
        cout << "綁定失敗！" << "error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "綁定成功！" << endl;
    }

    cout << "\n3. 開始監聽連線" << endl;
    result = listen(sListen, SOMAXCONN);
    if (result == SOCKET_ERROR)
    {
        cout << "監聽失敗！error: " << WSAGetLastError() << endl;
        closesocket(sListen);
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "監聽中..." << endl;
    }

    cout << "\n4. connect" << endl;
    SOCKET sConnect;
    struct sockaddr_in clientAddr;  // 儲存 client 端位址資訊
    int clientAddrLen = sizeof(clientAddr);
    while (true)
    {
        sConnect = accept(sListen, (SOCKADDR*)&clientAddr, &clientAddrLen);
        if (sConnect == INVALID_SOCKET)
        {
            result = WSAGetLastError();
            if (result == WSAEWOULDBLOCK)
            {
                Sleep(100);
                continue;
            }
            else
            {
                cout << "接收失敗！" << endl;
                closesocket(sListen);
                WSACleanup();
                return 1;
            }
        }

        cout << "連線出現！" << endl;
        cout << "Server : got a connection from : " << inet_ntoa(clientAddr.sin_addr) << endl;

        break;
    }

    while(true)
    {
        char message[200];
        ZeroMemory(message, 200);
        result = recv(sConnect, message, sizeof(message), 0);
        if (result > 0)
        {
            cout << "收到訊息：" << message << endl;

            const char* sendbuf = "Server: I receive your connection";
            while (true)
            {
                result = send(sConnect, sendbuf, strlen(sendbuf), 0);
                if (result == SOCKET_ERROR)
                {
                    int err = WSAGetLastError();
                    if (err == WSAEWOULDBLOCK)
                    {
                        Sleep(100);
                        continue;
                    }
                    else
                    {
                        cout << "send failed with error :" << WSAGetLastError() << endl;
                        closesocket(sConnect);
                        WSACleanup();
                        return 1;
                    }
                }
                break;
            }
        }
        else if (result == 0)
        {
            cout << "連線已關閉！error: " << WSAGetLastError() << endl;
            closesocket(sConnect);
            WSACleanup();
            return 1;
        }
        else
        {
            int err = WSAGetLastError();
            if (err == WSAEWOULDBLOCK)
            {
                Sleep(100);
                continue;
            }
            else
            {
                cout << "接收失敗！error: " << WSAGetLastError() << endl;
                closesocket(sConnect);
                WSACleanup();
                return 1;
            }
        }
    }

    cout << "關閉連線..." << endl;
    closesocket(sListen);
    WSACleanup();
}