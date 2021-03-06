#ifndef _IP_COMM_OP_RESULT_H_
#define _IP_COMM_OP_RESULT_H_

#include <IpComm/Config.h>

#include <cstdint>
#include <stdexcept>

namespace IpComm
{

	/**
	 * @brief
	 *  The operation did not complete within a prescribed amount of time.
	 */
	class IP_COMM_EXPORT TimeOut : public std::runtime_error
	{
	public:
		TimeOut();
		TimeOut(const char* message);
		TimeOut(const std::string& message);
	};

	/**
	 * @brief
	 *  The operation failed because a connection has not been established.
	 */
	class IP_COMM_EXPORT NotConnected : public std::runtime_error
	{
	public:
		NotConnected();
		NotConnected(const char* message);
		NotConnected(const std::string& message);
	};

	/**
	 * @brief
	 *  The operation failed because the server is not listening.
	 */
	class IP_COMM_EXPORT NotListening : public std::runtime_error
	{
	public:
		NotListening();
		NotListening(const char* message);
		NotListening(const std::string& message);
	};

	/**
	 * @brief
	 *  The operation was failed because it was invalid for
	 *  an object already in the connected state.
	 */
	class IP_COMM_EXPORT AlreadyConnected : public std::runtime_error
	{
	public:
		AlreadyConnected();
		AlreadyConnected(const char* message);
		AlreadyConnected(const std::string& message);
	};

	/**
	 * @brief
	 *  The IP Address passed was not valid.  For servers this usually means a
	 *  passed address does not have an associated IP Interface.
	 */
	class IP_COMM_EXPORT InvalidIpAddress : public std::runtime_error
	{
	public:
		InvalidIpAddress();
		InvalidIpAddress(const char* message);
		InvalidIpAddress(const std::string& message);
	};

	/**
	 * @brief
	 *  There was a failure of the internal socketing subsystem.
	 */
	class IP_COMM_EXPORT InternalSubsystemFailure : public std::runtime_error
	{
	public:
		InternalSubsystemFailure();
		InternalSubsystemFailure(const char* message);
		InternalSubsystemFailure(const std::string& message);
	};

	/**
	 * @brief
	 *  The buffer passed to receive data was not valid.  Usually this
	 *  means the pointer was null.
	 */
	class IP_COMM_EXPORT InvalidBuffer : public std::runtime_error
	{
	public:
		InvalidBuffer();
		InvalidBuffer(const char* message);
		InvalidBuffer(const std::string& message);
	};

	/**
	 * @brief
	 *  The connection was rejected by the remote server.
	 */
	class IP_COMM_EXPORT ConnectionRejected : public std::runtime_error
	{
	public:
		ConnectionRejected();
		ConnectionRejected(const char* message);
		ConnectionRejected(const std::string& message);
	};

	/**
	 * @brief
	 *  The connection was reset by the remote server.
	 */
	class IP_COMM_EXPORT ConnectionReset : public std::runtime_error
	{
	public:
		ConnectionReset();
		ConnectionReset(const char* message);
		ConnectionReset(const std::string& message);
	};

	/**
	 * @brief
	 *  The IP/Port combination passed to create a server is already
	 *  in use.
	 */
	class IP_COMM_EXPORT PortInUse : public std::runtime_error
	{
	public:
		PortInUse();
		PortInUse(const char* message);
		PortInUse(const std::string& message);
	};

	/**
	 * @brief
	 *  Unknown Error
	 */
	class IP_COMM_EXPORT UnknownError : public std::runtime_error
	{
	public:
		UnknownError();
		UnknownError(const char* message);
		UnknownError(const std::string& message);
	};
}

#endif // _IP_COMM_OP_RESULT_H_