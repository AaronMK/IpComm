#ifndef _IP_COMM_IP_END_POINT_H_
#define _IP_COMM_IP_END_POINT_H_

#include "IpAddress.h"

namespace IpComm
{
	struct IpEndpoint
	{
		IpAddress ip;
		Port port;

		IpEndpoint()
		{
			port = 0;
		}

		IpEndpoint(const IpAddress& _ip, Port _port)
			: ip(_ip), port(_port)
		{
			port = ip.isValid() ? _port : 0;
		}

		bool operator==(const IpEndpoint& other) const
		{
			return (port == other.port && ip == other.ip);
		}

		bool operator!=(const IpEndpoint& other) const
		{
			return (port != other.port || ip != other.ip);
		}

		bool operator<(const IpEndpoint& other) const
		{
			if (ip != other.ip)
				return (ip < other.ip);
			else
				return (port < other.port);
		}
	};
}

#endif // !_IP_COMM_IP_END_POINT_H_