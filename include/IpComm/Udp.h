#ifndef _IP_COMM_UDP_H_
#define _IP_COMM_UDP_H_

#include "IpEndPoint.h"

#include <StdExt/Buffer.h>

namespace IpComm::UDP
{
	struct Packet
	{
		IpEndPoint Source;
		IpEndPoint Destination;

		StdExt::Buffer Payload;
	};


}

#endif // !_IP_COMM_UDP_H_