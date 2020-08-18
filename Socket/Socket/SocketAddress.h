#ifndef SOCKET_ADDRESS_H
#define SOCKET_ADDRESS_H

#include <cstdint>
#include <WinSock2.h>
#include <memory>

class CScoketAddress
{
public:
	CScoketAddress(uint32_t inAddress, uint16_t inPort);
	CScoketAddress(const sockaddr& inSockAddr);

	size_t GetSize() const { return sizeof(sockaddr); }

private:
	sockaddr mSockAddr;

	sockaddr_in* GetAsSockAddrIn() { return reinterpret_cast<sockaddr_in*>(&mSockAddr); }
};

typedef std::shared_ptr<CScoketAddress> SocketAddressPtr;

#endif // !SOCKET_ADDRESS_H