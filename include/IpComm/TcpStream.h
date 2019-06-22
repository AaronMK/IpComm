#ifndef _IP_COMM_TCP_STREAM_H_
#define _IP_COMM_TCP_STREAM_H_

#include "IpComm.h"
#include "IpAddress.h"
#include "TcpConnection.h"

#include <Serialize/Binary/BufferedStream.h>

namespace IpComm
{
	class IP_COMM_EXPORT TcpStream : public Serialize::Binary::ByteStream
	{
	public:
		TcpStream();
		virtual ~TcpStream();

		virtual void readRaw(void* destination, Serialize::Binary::bytesize_t byteLength) override;
		virtual void writeRaw(const void* data, Serialize::Binary::bytesize_t byteLength) override;

		virtual Serialize::Binary::bytesize_t bytesAvailable() const override;
		virtual bool canRead(Serialize::Binary::bytesize_t numBytes) override;
		virtual bool canWrite(Serialize::Binary::bytesize_t numBytes, bool autoExpand = false) override;

	private:
		Serialize::Binary::BufferedStream mBufferStream;
		TcpConnection mTcpConnection;
	};
}

#endif _IP_COMM_TCP_STREAM_H_