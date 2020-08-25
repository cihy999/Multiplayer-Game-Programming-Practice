class SocketAddress
{
public:
	SocketAddress(uint32_t inAddress, uint16_t inPort);
	SocketAddress(const sockaddr& inSockAddr);
	SocketAddress();

	size_t GetSize() const { return sizeof(sockaddr); }

private:
	friend class UDPSocket;
	friend class TCPSocket;

	sockaddr mSockAddr;
	sockaddr_in* GetAsSockAddrIn() { return reinterpret_cast<sockaddr_in*>(&mSockAddr); }
};

typedef shared_ptr<SocketAddress> SocketAddressPtr;