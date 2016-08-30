#include <IpComm/TcpServer.h>

namespace IpComm
{
	struct TcpServerOpaque
	{
		TcpServerOpaque()
			: LastError(OpResult::NONE)
		{

		}

		OpResult LastError;
	};

	////////////////////////////////

	TcpServer::TcpServer()
	{
		mInternal.reset(new TcpServerOpaque());
	}
	
	TcpServer::~TcpServer()
	{

	}

	std::unique_ptr<TcpConnection> TcpServer::getClient()
	{
		mInternal->LastError = OpResult::NOT_IMPLEMENTED;
		return std::unique_ptr<TcpConnection>();
	}

	OpResult TcpServer::bind(Port port)
	{
		mInternal->LastError = OpResult::NOT_IMPLEMENTED;
		return mInternal->LastError;
	}

	OpResult TcpServer::bind(IpAddress addr, Port port)
	{
		mInternal->LastError = OpResult::NOT_IMPLEMENTED;
		return mInternal->LastError;
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