#include "Shared.h"

#pragma region Declaration

bool gIsGameRunning;
const int GOOD_SEGMENT_SIZE = 1300;

void DoBasicSocket();

void DoGameLoop();
void DoGameFrame();

void ProcessReceiveData(char* data, int bytesReceived, SocketAddress socketAddress);

void DoTCPLoop();
void ProcessNewClient(TCPSocketPtr socket, SocketAddress clientAddress);
void ProcessDataFromClient(const TCPSocketPtr& socket, char* segment, int dataReceived);

#pragma endregion

int main()
{

}

void DoBasicSocket() 
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

void DoGameLoop() 
{
    UDPSocketPtr mySock = SocketUtil::CreateUDPSocket(INET);
    mySock->SetNoBlockingMode(true);

    while(gIsGameRunning)
    {
        char data[1500];
        SocketAddress socketAddress;

        int bytesReceived = mySock->ReceiveFrom(data, sizeof(data), socketAddress);
        if (bytesReceived > 0) 
        {
            ProcessReceiveData(data, bytesReceived, socketAddress);
        }
        DoGameFrame();
    }
}

void DoGameFrame() 
{}

void ProcessReceiveData(char* data, int bytesReceived, SocketAddress socketAddress) 
{}

void DoTCPLoop() 
{
    TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(INET);
    SocketAddress receivingAddress(INADDR_ANY, 48000);
    if (listenSocket->Bind(receivingAddress) != NO_ERROR)
    {
        return;
    }
    vector<TCPSocketPtr> readBlockSockets;
    readBlockSockets.push_back(listenSocket);

    vector<TCPSocketPtr> readableSockets;

    while (gIsGameRunning) 
    {
        if (SocketUtil::Select( &readBlockSockets, &readableSockets,
                                nullptr, nullptr,
                                nullptr, nullptr))
        {
            //we got a packet-loop through the set ones...
            for (const TCPSocketPtr& socket : readableSockets) 
            {
                if (socket == listenSocket)
                {
                    //it's the listen socket, accept a new connection
                    SocketAddress newClientAddress;
                    auto newSocket = listenSocket->Accept(newClientAddress);
                    readBlockSockets.push_back(newSocket);
                    ProcessNewClient(newSocket, newClientAddress);
                }
                else 
                {
                    //it's a regular socket-process the data...
                    char segment[GOOD_SEGMENT_SIZE];
                    int dataReceived = socket->Receive(segment, GOOD_SEGMENT_SIZE);
                    if (dataReceived > 0) 
                    {
                        ProcessDataFromClient(socket, segment, dataReceived);
                    }
                }
            }
        }
    }
}

void ProcessNewClient(TCPSocketPtr socket, SocketAddress clientAddress)
{}

void ProcessDataFromClient(const TCPSocketPtr& socket, char* segment, int dataReceived)
{}