#include <IpComm/TcpStream.h>

#include <StdExt/Number.h>

using namespace Serialize::Binary;
using namespace StdExt;

IpComm::TcpStream::TcpStream()
{
}

IpComm::TcpStream::~TcpStream()
{
}

void IpComm::TcpStream::readRaw(void* destination, bytesize_t byteLength)
{
	bytesize_t available_bytes = mBufferStream.bytesAvailable();

	if (available_bytes < byteLength)
	{
		bytesize_t bytes_to_read = std::max<bytesize_t>(
			byteLength - available_bytes,
			Number::convert<bytesize_t>(mTcpConnection.bytesAvailable())
		);
		mTcpConnection.read(mBufferStream.expandForWrite(bytes_to_read), bytes_to_read, true);
	}

	mBufferStream.readRaw(destination, byteLength);
}

void IpComm::TcpStream::writeRaw(const void* data, bytesize_t byteLength)
{
	mTcpConnection.write(data, byteLength);
}

bytesize_t IpComm::TcpStream::bytesAvailable() const
{
	return mBufferStream.bytesAvailable() + Number::convert<bytesize_t>(mTcpConnection.bytesAvailable());
}

bool IpComm::TcpStream::canRead(bytesize_t numBytes)
{
	return bytesAvailable() >= numBytes;
}

bool IpComm::TcpStream::canWrite(bytesize_t numBytes, bool autoExpand)
{
	return mTcpConnection.isConnected();
}
