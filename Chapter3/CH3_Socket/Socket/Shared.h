#include <WinSock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <memory>
#include <string>
#include <vector>

using std::shared_ptr;
using std::make_shared;
using std::string;
using std::vector;

#include "SocketAddress.h"
#include "SocketAdressFactory.h"
#include "UDPSocket.h"
#include "TCPSocket.h"
#include "SocketUtil.h"