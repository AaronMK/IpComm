#include <IpComm/TcpConnection.h>

#include "private_include/TcpConnOpaque.h"

#include <cassert>

namespace IpComm
{
	TcpConnOpaque::TcpConnOpaque()
		: LastError(OpResult::NONE)
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2,2), &wsaData);

		Socket = INVALID_SOCKET;
		RemotePort = 0;
	}

	TcpConnOpaque::~TcpConnOpaque()
	{
		WSACleanup();
	}

	void TcpConnOpaque::setLastError(TcpConnection* connection, const OpResult error)
	{
		if (!connection->mInternal)
			connection->mInternal.reset(new TcpConnOpaque());

		connection->mInternal->LastError = error;
	}

	TcpConnection::TcpConnection()
	{
	}

	TcpConnection::TcpConnection(TcpConnection&& other)
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

	bool TcpConnection::connect(IpAddress IP, Port port)
	{
		if ( isConnected() )
		{
			mInternal->LastError = OpResult::ALREADY_CONNECTED;
			return false;
		} 
		else if (false == IP.isValid())
		{
			TcpConnOpaque::setLastError(this, OpResult::INVALID_IP_ADDRESS);
			return false;
		}

		if (!mInternal)
			mInternal.reset(new TcpConnOpaque());

		if (IP.version() == IpVersion::V4)
		{
			mInternal->Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			if (INVALID_SOCKET == mInternal->Socket)
			{
				mInternal->LastError = OpResult::INTERNAL_SUBSYSTEM_FAILURE;
				return false;
			}

			sockaddr_in sockAddr;
			memset(&sockAddr, 0, sizeof(sockaddr_in) );

			sockAddr.sin_family = AF_INET;
			sockAddr.sin_port = htons(port);
			IP.getSysAddress(&sockAddr.sin_addr);

			if (0 == ::connect(mInternal->Socket, (sockaddr*)&sockAddr, sizeof(sockaddr_in)))
			{
				mInternal->RemoteIP = IP;
				mInternal->RemotePort = port;

				struct sockaddr_in addrLocal;
				int addrLength = sizeof(sockaddr_in);
				getsockname(mInternal->Socket, (sockaddr*)&addrLocal, &addrLength);
				mInternal->LocalIP = IpAddress(&addrLocal.sin_addr);
				mInternal->LocalPort = addrLocal.sin_port;

				mInternal->LastError = OpResult::SUCCESS;
				return true;
			}
			else
			{
				closesocket(mInternal->Socket);
				mInternal->Socket = INVALID_SOCKET;

				mInternal->LastError = OpResult::INTERNAL_SUBSYSTEM_FAILURE;
				return false;
			}
		}
		else if (IP.version() == IpVersion::V6)
		{
			// Need to implement
			assert(false);
		}

		mInternal->LastError = OpResult::UNKNOWN_ERROR;
		return false;
	}

	void TcpConnection::disconnect()
	{
		if ( isConnected() )
		{
			closesocket(mInternal->Socket);
			
			mInternal->Socket = INVALID_SOCKET;
			mInternal->RemoteIP = IpAddress();
			mInternal->RemotePort = 0;
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
			TcpConnOpaque::setLastError(this, OpResult::NOT_CONNECTED);
			return -1;
		}
		else if (0 == size)
		{
			mInternal->LastError = OpResult::SUCCESS;
			return 0;
		}
		else if (nullptr == buffer)
		{
			mInternal->LastError = OpResult::INVALID_BUFFER;
			return -1;
		}
		else
		{
			int flags = (fill) ? MSG_WAITALL : 0;
			int readResult = recv(mInternal->Socket, (char*)buffer, size, flags);

			if (readResult < 0)
			{
				switch (readResult)
				{
				case WSAENOTCONN:
					mInternal->LastError = OpResult::NOT_CONNECTED;
					disconnect();
					return -1;
				case WSAECONNABORTED:
				case WSAETIMEDOUT:
					mInternal->LastError = OpResult::TIME_OUT;
					disconnect();
					return -1;
				default:
					mInternal->LastError = OpResult::UNKNOWN_ERROR;
					return -1;
				}
			}

			mInternal->LastError = OpResult::SUCCESS;
			return readResult;
		}

		mInternal->LastError = OpResult::UNKNOWN_ERROR;
		return 0;
	}

	bool TcpConnection::write(const void* data, size_t byteLength)
	{
		if (!isConnected())
		{
			mInternal->LastError = OpResult::NOT_CONNECTED;
			return false;
		}
		else if ( 0 == byteLength )
		{
			mInternal->LastError = OpResult::SUCCESS;
			return true;
		}
		else
		{
			int sendResult = send(mInternal->Socket, (char*)data, byteLength, 0);

			if (sendResult > 0)
			{
				mInternal->LastError = OpResult::SUCCESS;
				return true;
			}
		}

		mInternal->LastError = OpResult::UNKNOWN_ERROR;
		return false;
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
		return ( isConnected() ) ? mInternal->RemoteIP : IpAddress();
	}
		
	Port TcpConnection::remotePort() const
	{
		return ( isConnected() ) ? mInternal->RemotePort : 0;
	}

	IpAddress TcpConnection::localIp() const
	{
		return ( isConnected() ) ? mInternal->LocalIP : IpAddress();
	}
		
	Port TcpConnection::localPort() const
	{
		return ( isConnected() ) ? mInternal->LocalPort : 0;
	}

	OpResult TcpConnection::getLastError() const
	{
		if (mInternal)
			return mInternal->LastError;

		return OpResult::NONE;
	}
}