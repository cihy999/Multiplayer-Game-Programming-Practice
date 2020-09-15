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
    sockaddr_in clientAddr;
    int nClientAddrLen = sizeof(clientAddr);

    fd_set readSockets;                // 專門讀取
    FD_ZERO(&readSockets);
    FD_SET(sListen, &readSockets);     // 把 sListen 加進 fdSocket 的集合中

    fd_set writeSockets;
    FD_ZERO(&writeSockets);

    while (true)
    {
        fd_set fdRead = readSockets;
        fd_set fdWrite = writeSockets;
        result = select(NULL, &fdRead, &fdWrite, NULL, NULL);

        if (result <= 0) 
        {
            cout << "接收失敗！ error: " << WSAGetLastError() << endl;
            closesocket(sListen);
            WSACleanup();
            return 1;
        }
        else 
        {
            for (int i = 0; i < (int)readSockets.fd_count; i++) 
            {
                // Check for arriving connections on the listening socket.
                if (FD_ISSET(readSockets.fd_array[i], &fdRead))
                {
                    if (readSockets.fd_array[i] == sListen)
                    {
                        sConnect = accept(sListen, (sockaddr*)&clientAddr, &nClientAddrLen);
                        FD_SET(sConnect, &readSockets);

                        cout << "連線出現！" << endl;
                        cout << "Server : got a connection from : " << inet_ntoa(clientAddr.sin_addr) << endl;
                    }
                    else
                    {
                        char buffer[1024];
                        memset(buffer, 0, 1024);
                        result = recv(readSockets.fd_array[i], buffer, sizeof(buffer), 0);

                        if (result > 0)
                        {
                            cout << "收到訊息：" << buffer << endl;

                            FD_SET(sConnect, &writeSockets);
                        }
                        else
                        {
                            if (result == SOCKET_ERROR)
                                cout << "接收失敗！ error: " << WSAGetLastError() << endl;
                            closesocket(readSockets.fd_array[i]);
                            FD_CLR(readSockets.fd_array[i], &readSockets); // 刪除集合中的socket
                            break;
                        }
                    }
                }
            }

            for (int i = 0; i < (int)writeSockets.fd_count; i++) 
            {
                const char PRT[1024] = "I receive your message.";
                result = send(writeSockets.fd_array[i], PRT, sizeof(PRT), 0);
                if (result > 0)
                {
                    cout << "傳送成功！" << endl;
                    FD_CLR(writeSockets.fd_array[i], &writeSockets); // 刪除集合中的socket
                    break;
                }
                else
                {
                    cout << "傳送失敗！ error: " << WSAGetLastError() << endl;
                    closesocket(writeSockets.fd_array[i]);
                    FD_CLR(writeSockets.fd_array[i], &writeSockets); // 刪除集合中的socket
                    break;
                }
            }
        }
    }
}