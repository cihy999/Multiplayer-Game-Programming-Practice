class TCPSocket
{
public:
	~TCPSocket();

	int Connect(const SocketAddress& inAddress);
	int Bind(const SocketAddress& inBindAddress);
	int Listen(int inBackLog = 32);
	shared_ptr<TCPSocket> Accept(SocketAddress& inFromAddress);
	int Send(const void* inData, int inLen);
	int Receive(void* inBuffer, int inLen);

private:
	friend class SocketUtil;
	TCPSocket(SOCKET inSocket) : mSocket(inSocket) {}
	SOCKET mSocket;
};

typedef shared_ptr<TCPSocket> TCPSocketPtr;