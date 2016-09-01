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
#include <IpComm/OpResult.h>
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

		IpAddress RemoteIP;
		Port      RemotePort;

		IpAddress LocalIP;
		Port      LocalPort;

		OpResult  LastError;

		TcpConnOpaque();
		virtual ~TcpConnOpaque();

		static void setLastError(TcpConnection* connection, const OpResult error);
	};
}

#endif// _IP_COMM_TCP_CONN_OPAQUE_H_