#include "Shared.h"

UDPSocket::~UDPSocket()
{
	closesocket(mSocket);
}

int UDPSocket::Bind(const SocketAddress& inBindAddress)
{
	int err = bind(mSocket, &inBindAddress.mSockAddr, inBindAddress.GetSize());

	if (err != 0) 
	{
		SocketUtil::ReportError("UDPSocket::Bind");
		return SocketUtil::GetLastError();
	}
	return NO_ERROR;
}

int UDPSocket::SendTo(const void* inData, int inLen, const SocketAddress& inTo)
{
	int byteSentCount = sendto( mSocket, 
								static_cast<const char*>(inData), 
								inLen, 0, 
								&inTo.mSockAddr, inTo.GetSize());
	if (byteSentCount >= 0) 
	{
		return byteSentCount;
	}
	else
	{
		//return error as negative number
		SocketUtil::ReportError("UDPSocket::SendTo");
		return -SocketUtil::GetLastError();
	}
}

int UDPSocket::ReceiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom)
{
	int fromLength = outFrom.GetSize();
	int readByteCount = recvfrom(mSocket, static_cast<char*>(inBuffer), inLen, 0, &outFrom.mSockAddr, &fromLength);

	if (readByteCount) 
	{
		return readByteCount;
	}
	else
	{
		SocketUtil::ReportError("UDPSocket::ReceiveFrom");
		return SocketUtil::GetLastError();
	}
}

int UDPSocket::SetNoBlockingMode(bool inShouldBeNonBlocking) 
{
	u_long arg = inShouldBeNonBlocking ? 1 : 0;
	int result = ioctlsocket(mSocket, FIONBIO, &arg);

	if (result == SOCKET_ERROR) 
	{
		SocketUtil::ReportError("UDPSocket::SetNoBlockingMode");
		return -SocketUtil::GetLastError();
	}
	else
	{
		return NO_ERROR;
	}
}