class UDPSocket
{
public:
	~UDPSocket();

	int Bind(const SocketAddress& inBindAddress);
	int SendTo(const void* inData, int inLen, const SocketAddress& inTo);
	int ReceiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom);

	int SetNoBlockingMode(bool inShouldBeNonBlocking);

private:
	friend class SocketUtil;
	UDPSocket(SOCKET inSocket) : mSocket(inSocket) {}
	SOCKET mSocket;
};

typedef std::shared_ptr<UDPSocket> UDPSocketPtr;