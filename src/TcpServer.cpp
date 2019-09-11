#include <IpComm/TcpServer.h>
#include <IpComm/Exceptions.h>

#include <StdExt/Exceptions.h>

#include "private_include/TcpConnOpaque.h"

namespace IpComm
{
	struct TcpServerOpaque
	{
		SOCKET Socket;
		IpAddress ListenIP;
		Port      ListenPort;

		TcpServerOpaque()
		{
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2,2), &wsaData);

			Socket = INVALID_SOCKET;
			ListenPort = 0;
		}

		virtual ~TcpServerOpaque()
		{
			closesocket(Socket);
			WSACleanup();
		}
	};

	////////////////////////////////

	TcpServer::TcpServer()
	{
		mInternal.reset(new TcpServerOpaque());
	}
	
	TcpServer::~TcpServer()
	{
		disconnect();
	}

	TcpConnection TcpServer::getClient()
	{
		if (false == isListening())
		{
			throw NotListening();
		}

		sockaddr_in inAddr;
		int addrLength = sizeof(sockaddr_in);

		SOCKET acceptSocket = accept(mInternal->Socket, (sockaddr*)&inAddr, &addrLength);
		
		if (INVALID_SOCKET == acceptSocket)
		{
			switch (WSAGetLastError())
			{
			case WSAEINVAL:
				throw NotListening();
				break;
			case WSAECONNRESET:
				throw ConnectionReset();
				break;
			default:
				throw InternalSubsystemFailure();
				break;
			}
			
			return nullptr;
		}

		std::unique_ptr<TcpConnOpaque> ptrRet(new TcpConnOpaque());
		ptrRet->Socket = acceptSocket;
		ptrRet->RemoteEndPoint = IpEndPoint(IpAddress(&inAddr.sin_addr), inAddr.sin_port);

		struct sockaddr_in addrLocal;
		getsockname(ptrRet->Socket, (sockaddr*)&addrLocal, &addrLength);
		ptrRet->LocalEndPoint = IpEndPoint(IpAddress(&addrLocal.sin_addr), mInternal->ListenPort);

		return TcpConnection(std::move(ptrRet));
	}

	void TcpServer::bind(Port port, IpVersion version)
	{
		return bind(IpAddress::any(version), port);
	}

	void TcpServer::bind(IpAddress addr, Port port)
	{
		if ( isListening() )
			throw AlreadyConnected();
		
		if (false == addr.isValid())
			throw InvalidIpAddress();


		if (addr.version() == IpVersion::V4)
		{
			mInternal->Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			if (INVALID_SOCKET == mInternal->Socket)
				throw InternalSubsystemFailure();

			sockaddr_in sockAddr;
			memset(&sockAddr, 0, sizeof(sockaddr_in) );

			sockAddr.sin_family = AF_INET;
			sockAddr.sin_port = htons(port);
			addr.getSysAddress(&sockAddr.sin_addr);

			if ( 0 == ::bind(mInternal->Socket, (sockaddr*)&sockAddr, sizeof(sockaddr_in)) &&
			     SOCKET_ERROR != listen(mInternal->Socket, SOMAXCONN))
			{
				mInternal->ListenIP = addr;
				mInternal->ListenPort = port;

				return;
			}
			else
			{
				closesocket(mInternal->Socket);
				mInternal->Socket = INVALID_SOCKET;

				throw InternalSubsystemFailure();
			}
		}
		else if (addr.version() == IpVersion::V6)
		{
			throw StdExt::not_implemented("IPv6 server support not yet implemeneted.");
		}
	}

	bool TcpServer::isListening() const
	{
		return ( mInternal && INVALID_SOCKET != mInternal->Socket);
	}

	void TcpServer::disconnect()
	{
		if (isListening())
		{
			closesocket(mInternal->Socket);
			mInternal->Socket = INVALID_SOCKET;

			mInternal->ListenIP = IpAddress();
			mInternal->ListenPort = 0;
		}
	}
}