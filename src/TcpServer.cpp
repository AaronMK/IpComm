#include <IpComm/TcpServer.h>

#include "private_include/TcpConnOpaque.h"

namespace IpComm
{
	struct TcpServerOpaque
	{
		OpResult LastError;

		SOCKET Socket;
		IpAddress ListenIP;
		Port      ListenPort;

		TcpServerOpaque()
			: LastError(OpResult::NONE)
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
		WSADATA wsaData;

		mInternal.reset(new TcpServerOpaque());
	}
	
	TcpServer::~TcpServer()
	{

	}

	std::unique_ptr<TcpConnection> TcpServer::getClient()
	{
		if (false == isListening())
		{
			mInternal->LastError = OpResult::NOT_CONNECTED;
			return nullptr;
		}

		sockaddr_in inAddr;
		int addrLength = sizeof(sockaddr_in);

		SOCKET acceptSocket = accept(mInternal->Socket, (sockaddr*)&inAddr, &addrLength);
		
		if (INVALID_SOCKET == acceptSocket)
		{
			switch (WSAGetLastError())
			{
			case WSAEINVAL:
				mInternal->LastError = OpResult::NOT_LISTENING;
				break;
			case WSAECONNRESET:
				mInternal->LastError = OpResult::CONNCETION_RESET;
				break;
			default:
				mInternal->LastError = OpResult::INTERNAL_SUBSYSTEM_FAILURE;
				break;
			}
			
			return nullptr;
		}

		std::unique_ptr<TcpConnOpaque> ptrRet(new TcpConnOpaque());
		ptrRet->Socket = acceptSocket;
		ptrRet->RemoteIP = IpAddress(&inAddr.sin_addr);
		ptrRet->RemotePort = inAddr.sin_port;

		struct sockaddr_in addrLocal;
		getsockname(ptrRet->Socket, (sockaddr*)&addrLocal, &addrLength);
		ptrRet->LocalIP = IpAddress(&addrLocal.sin_addr);
		ptrRet->LocalPort = mInternal->ListenPort;

		mInternal->LastError = OpResult::SUCCESS;
		return std::unique_ptr<TcpConnection>(new TcpConnection(std::move(ptrRet)));
	}

	OpResult TcpServer::bind(Port port, IpVersion version)
	{
		return bind(IpAddress::any(version), port);
	}

	OpResult TcpServer::bind(IpAddress addr, Port port)
	{
		if ( isListening() )
		{
			mInternal->LastError = OpResult::ALREADY_CONNECTED;
			return mInternal->LastError;
		} 

		else if (false == addr.isValid())
		{
			mInternal->LastError = OpResult::INVALID_IP_ADDRESS;
			return mInternal->LastError;
		}


		if (addr.version() == IpVersion::V4)
		{
			mInternal->Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			if (INVALID_SOCKET == mInternal->Socket)
			{
				mInternal->LastError = OpResult::INTERNAL_SUBSYSTEM_FAILURE;
				return mInternal->LastError;
			}

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

				mInternal->LastError = OpResult::SUCCESS;
				return mInternal->LastError;
			}
			else
			{
				closesocket(mInternal->Socket);
				mInternal->Socket = INVALID_SOCKET;

				mInternal->LastError = OpResult::INTERNAL_SUBSYSTEM_FAILURE;
				return mInternal->LastError;
			}
		}
		else if (addr.version() == IpVersion::V6)
		{
			mInternal->LastError = OpResult::NOT_IMPLEMENTED;
			return mInternal->LastError;
		}

		mInternal->LastError = OpResult::UNKNOWN_ERROR;
		return mInternal->LastError;
	}

	bool TcpServer::isListening() const
	{
		return (INVALID_SOCKET != mInternal->Socket);
	}

	void TcpServer::disconnect()
	{
		mInternal->LastError = OpResult::SUCCESS;
	}

	OpResult TcpServer::getLastError() const
	{
		return mInternal->LastError;
	}
}