#include <IpComm/TcpConnection.h>
#include <IpComm/Exceptions.h>

#include <StdExt/Exceptions.h>

#include "private_include/TcpConnOpaque.h"

#include <cassert>
#include "..\include\IpComm\TcpConnection.h"

namespace IpComm
{
	TcpConnOpaque::TcpConnOpaque()
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2,2), &wsaData);
		Socket = INVALID_SOCKET;
	}

	TcpConnOpaque::~TcpConnOpaque()
	{
		WSACleanup();
	}

	TcpConnection::TcpConnection()
	{
	}

	TcpConnection::TcpConnection(TcpConnection&& other) noexcept
	{
		mInternal = std::move(other.mInternal);
	}

	TcpConnection::TcpConnection(std::unique_ptr<TcpConnOpaque>&& opaque)
		: mInternal(std::move(opaque))
	{
	}

	TcpConnection::~TcpConnection()
	{
		disconnect();
	}

	TcpConnection& TcpConnection::operator=(TcpConnection&& RHS) noexcept
	{
		mInternal = std::move(RHS.mInternal);
		return *this;
	}

	bool TcpConnection::accept()
	{
		throw StdExt::not_implemented();

		return false;
	}

	void TcpConnection::connect(IpAddress IP, Port port)
	{
		if ( isConnected() )
		{
			throw AlreadyConnected();
		} 
		else if (false == IP.isValid())
		{
			throw InvalidIpAddress();
		}

		if (!mInternal)
			mInternal.reset(new TcpConnOpaque());

		if (IP.version() == IpVersion::V4)
		{
			mInternal->Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			if (INVALID_SOCKET == mInternal->Socket)
				throw InternalSubsystemFailure();

			sockaddr_in sockAddr;
			memset(&sockAddr, 0, sizeof(sockaddr_in) );

			sockAddr.sin_family = AF_INET;
			sockAddr.sin_port = htons(port);
			IP.getSysAddress(&sockAddr.sin_addr);

			if (0 == ::connect(mInternal->Socket, (sockaddr*)&sockAddr, sizeof(sockaddr_in)))
			{
				mInternal->RemoteEndPoint = IpEndPoint(IP, port);

				struct sockaddr_in addrLocal;
				int addrLength = sizeof(sockaddr_in);
				getsockname(mInternal->Socket, (sockaddr*)&addrLocal, &addrLength);

				mInternal->LocalEndPoint = IpEndPoint(IpAddress(&addrLocal.sin_addr), addrLocal.sin_port);

				return;
			}
			else
			{
				closesocket(mInternal->Socket);
				mInternal->Socket = INVALID_SOCKET;

				throw InternalSubsystemFailure();
			}
		}
		else if (IP.version() == IpVersion::V6)
		{
			throw StdExt::not_implemented("IPv6 connection support not yet implemented.");
		}
	}

	void TcpConnection::disconnect()
	{
		if ( isConnected() )
		{
			closesocket(mInternal->Socket);
			
			mInternal->Socket = INVALID_SOCKET;
			mInternal->RemoteEndPoint = IpEndPoint();
		}
	}

	bool TcpConnection::isConnected() const
	{
		return (mInternal && INVALID_SOCKET != mInternal->Socket);
	}

	int TcpConnection::read(void* buffer, size_t size, bool fill)
	{
		if (!isConnected())
		{
			throw NotConnected();
		}
		else if (0 == size)
		{
			return 0;
		}
		else if (nullptr == buffer)
		{
			throw std::invalid_argument("read buffer cannot be null.");
		}
		else
		{
			int flags = (fill) ? MSG_WAITALL : 0;
			int readResult = recv(mInternal->Socket, (char*)buffer, (int)size, flags);

			if (readResult < 0)
			{
				switch (readResult)
				{
				case WSAENOTCONN:
					disconnect();
					throw NotConnected();
				case WSAECONNABORTED:
				case WSAETIMEDOUT:
					disconnect();
					throw TimeOut();
					return -1;
				default:
					throw UnknownError();
				}
			}

			return readResult;
		}
	}

	void TcpConnection::write(const void* data, size_t byteLength)
	{
		if (!isConnected())
		{
			throw NotConnected();
		}
		else if ( 0 == byteLength )
		{
			return;
		}
		else
		{
			if (send(mInternal->Socket, (char*)data, (int)byteLength, 0) > 0)
				return;
		}

		throw UnknownError();
	}

	size_t TcpConnection::bytesAvailable() const
	{
		if ( isConnected() )
		{
			u_long out;
			ioctlsocket(mInternal->Socket, FIONREAD, &out);

			return out;
		}

		return 0;
	}

	IpAddress TcpConnection::remoteIp() const
	{
		return isConnected() ? mInternal->RemoteEndPoint.ip : IpAddress();
	}
		
	Port TcpConnection::remotePort() const
	{
		return isConnected() ? mInternal->RemoteEndPoint.port : 0;
	}

	IpEndPoint TcpConnection::remoteEndpoint() const
	{
		return isConnected() ? mInternal->RemoteEndPoint : IpEndPoint();
	}

	IpAddress TcpConnection::localIp() const
	{
		return isConnected() ? mInternal->LocalEndPoint.ip : IpAddress();
	}
		
	Port TcpConnection::localPort() const
	{
		return isConnected() ? mInternal->LocalEndPoint.port : 0;
	}

	IpEndPoint TcpConnection::localEndpoint() const
	{
		return isConnected() ? mInternal->LocalEndPoint : IpEndPoint();
	}
}