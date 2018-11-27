#ifndef _IP_COMM_NETWORK_INTERFACE_H_
#define _IP_COMM_NETWORK_INTERFACE_H_

#include "Config.h"

#include "IpComm.h"
#include "IpAddress.h"

#include <StdExt/String.h>

#include <vector>

namespace IpComm
{
	class IP_COMM_EXPORT NetworkInterface
	{
	public:
		static NetworkInterface any(IpVersion version);
		static NetworkInterface localhost();

		static std::vector<NetworkInterface> allInterfaces();

		const IpAddress& address() const;
		const StdExt::String& name() const;

	private:
		NetworkInterface();

		StdExt::String mName;
		IpAddress mIpAddr;
	};
}

#endif // _IP_COMM_NETWORK_INTERFACE_H_