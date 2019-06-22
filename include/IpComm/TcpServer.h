#ifndef _IP_COMM_TCP_SERVER_H_
#define _IP_COMM_TCP_SERVER_H_

#include <IpComm/IpComm.h>
#include <IpComm/IpAddress.h>
#include <IpComm/TcpConnection.h>

#include <memory>
#include <cstdint>
#include <functional>

#ifdef _WIN32
#	pragma warning(push)
#	pragma warning(disable: 4251)
#endif // _WIN32

namespace IpComm
{
	struct TcpServerOpaque;

	class IP_COMM_EXPORT TcpServer
	{
	public:
		TcpServer(TcpServer&&) = delete;
		TcpServer(const TcpServer&) = delete;

		friend struct TcpServerOpaque;

		TcpServer();

		virtual ~TcpServer();

		/**
		 * @brief
		 *  Blocks until a connection request is made to the server.  If the server
		 *  is not listening, or is disconnected, the function will return a
		 *  nullptr. In that case, getLastError() can be used to get details. 
		 *
		 *  Connections that are received on the server must call TcpConnection::accept()
		 *  to activate the conneciton.  To reject the connection, just let the returned pointer
		 *  go out of scope.
		 */
		TcpConnection getClient();
		
		/**
		 * @brief
		 *  Binds the server to the passed port on all IP Interfaces with the 
		 *  passed version on the system.
		 */
		void bind(Port port, IpVersion version);
		
		/**
		 * @brief
		 *  Binds the server to the passed port on a specific Ip interface
		 *  of the system.
		 */
		void bind(IpAddress addr, Port port);

		bool isListening() const;

		/**
		 * @brief
		 *  Stops the server and frees the IP/Port combination.
		 */
		void disconnect();

	private:
		std::unique_ptr<TcpServerOpaque> mInternal;
	};
}

#ifdef _WIN32
#	pragma warning(pop)
#endif // _WIN32

#endif // _IP_COMM_TCP_SERVER_H_