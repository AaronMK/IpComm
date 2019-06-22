#ifndef _IP_COMM_TCP_CONN_OPAQUE_H_
#define _IP_COMM_TCP_CONN_OPAQUE_H_

#ifdef _WIN32
#	define WIN32_LEAN_AND_MEAN

#	include <windows.h>
#	include <winsock2.h>
#	include <ws2tcpip.h>

#	pragma comment (lib, "Ws2_32.lib")
#endif // _WIN32

#include <IpComm/IpComm.h>
#include <IpComm/Exceptions.h>
#include <IpComm/IpAddress.h>

namespace IpComm
{
	class TcpConnection;
	
	/**
	 * @internal
	 *
	 * @brief
	 *  Opaque data and functionality for TcpConnection.
	 */
	struct TcpConnOpaque
	{
		SOCKET Socket;

		IpEndpoint RemoteEndPoint;
		IpEndpoint LocalEndPoint;

		TcpConnOpaque();
		virtual ~TcpConnOpaque();
	};
}

#endif// _IP_COMM_TCP_CONN_OPAQUE_H_