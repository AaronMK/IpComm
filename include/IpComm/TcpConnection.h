#ifndef _IP_COMM_TCP_CONNECTION_H_
#define _IP_COMM_TCP_CONNECTION_H_

#ifdef _WIN32
#	pragma warning(push)
#	pragma warning(disable: 4251)
#endif // _WIN32

#include <IpComm/IpComm.h>
#include <IpComm/IpAddress.h>

#include <cstdint>
#include <memory>

namespace IpComm
{
	/**
	 * @brief
	 *  A TCP/IP socket connection.
	 *
	 * @todo
	 *  Implment IPv6 functionality.
	 */
	class IP_COMM_EXPORT TcpConnection
	{
	public:
		friend class TcpServer;
		friend struct TcpConnOpaque;

		TcpConnection(const TcpConnection&) = delete;
		TcpConnection& operator=(const TcpConnection&) const = delete;
		TcpConnection& operator=(TcpConnection&& RHS) = delete;

		/**
		 * @brief
		 *  Creates a connection object.
		 */
		TcpConnection();

		/**
		 * @brief
		 *  Transfers a connection and all its current state.
		 */
		TcpConnection(TcpConnection&& other);

		/**
		 * @brief
		 *  Destructor atuomatically closes the connection if it is open.
		 */
		virtual ~TcpConnection();

		/**
		 * @brief
		 *  Acceots a connection.  This is only applicable for connections
		 *  recieved by a TcpServer.
		 */
		bool accept();

		/**
		 * @brief
		 *  Attempts to connect to the passed IP and port, blocking
		 *  until success or failure is determined.
		 */
		void connect(IpAddress ip, Port port);
		
		/**
		 * @brief
		 *  If connected, closes the connection.
		 */
		void disconnect();

		/**
		 * @brief
		 *  Returns true if connected.
		 */
		bool isConnected() const;

		/**
		 * @brief
		 *  Reads data from the socket.
		 *
		 * @param buffer
		 *  The buffer in which to store the read data.
		 *
		 * @param size
		 *  The amount of data to be read in bytes.  This should be less than or equal to the size
		 *  of the buffer.
		 *
		 * @param fill
		 *  If true, this function will block until either an error or disconnect on the connection
		 *  occurs, or size bytes are available and read.  If false, the function will return
		 *  immediately, reading what is ready up to the size limit.
		 *
		 * @return
		 *  The number of bytes read, or a negative number if an error or disconnect occured. getLastError()
		 *  can be used to get additional details.
		 */
		int read(void* buffer, size_t size, bool fill);

		/**
		 * @brief
		 *  Writes data onto the socket.
		 */
		void write(const void* data, size_t byteLength);

		/**
		 * @brief
		 *  Gets the number of bytes available for reading.
		 */
		size_t bytesAvailable() const;

		/**
		 * @brief
		 *  Gets the remote IP Address of the connection.
		 */
		IpAddress remoteIp() const;
		
		/**
		 * @brief
		 *  Gets the remote port of the connection.
		 */
		Port remotePort() const;

		/**
		 * @brief
		 *  Gets the local IP Address.  When accepted by a TcpServer
		 *  this will be the listening IP of the server that accepted
		 *  the connection.
		 */
		IpAddress localIp() const;

		/**
		 * @brief
		 *  Gets the local port.  When accepted by a TcpServer
		 *  this will be the listening port of the server that accepted
		 *  the connection.
		 */
		Port localPort() const;

	private:
		TcpConnection(std::unique_ptr<TcpConnOpaque>&& opaque);

		std::unique_ptr<TcpConnOpaque> mInternal;
	};
}

#ifdef _WIN32
#	pragma warning(pop)
#endif // _WIN32

#endif // _IP_COMM_TCP_CONNECTION_H_