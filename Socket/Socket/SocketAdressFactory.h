#ifndef SOCKET_ADDRESS_FACTORY_H
#define SOCKET_ADDRESS_FACTORY_H

#include "SocketAddress.h"
#include <string>

class CSocketAdressFactory
{
public:
	static SocketAddressPtr CreateIPv4FormString(const std::string& inString);
};

#endif // !SOCKET_ADDRESS_FACTORY_H