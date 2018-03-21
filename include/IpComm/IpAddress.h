#ifndef _IP_COMM_IP_ADDRESS_H_
#define _IP_COMM_IP_ADDRESS_H_

#include "Config.h"
#include "IpComm.h"

#include <Serialize/Serialize.h>

#include <cstdint>
#include <string>

#ifdef _WIN32
	struct in_addr;
	struct in6_addr;
#endif // _WIN32

namespace IpComm
{
	enum class IpVersion
	{
		NONE,
		V4,
		V6
	};

	/**
	 * @brief
	 *  Stores an IPv4 or IPv6 address.
	 */
	class IP_COMM_EXPORT IpAddress final
	{
	public:
		/**
		 * @brief
		 *  Returns an IP address that denotes "any" interface for
		 *  the passed IP version.
		 */
		static IpAddress any(IpVersion version);

		/**
		 * @brief
		 *  Constructs an invalid %IpAddress.
		 */
		IpAddress();

		/**
		 * @brief
		 *  Constructs an IP address from a string, automatically
		 *  determining the version.
		 */
		IpAddress(const char* addr);

		/**
		 * @brief
		 *  Constructs an IP address from a string, automatically
		 *  determining the version.
		 */
		IpAddress(const std::string& addr);

		/**
		 * @brief
		 *  Constructs an IP address of the passed version from a string.
		 *
		 *  IPv4 addresses must be in dotted notation. (ie. 123.45.6.7)
		 */
		IpAddress(const char* addr, IpVersion version);

		/**
		 * @brief
		 *  Constructs an IP address of the passed version from a string.
		 *
		 *  IPv4 addresses must be in dotted notation. (ie. 123.45.6.7)
		 */
		IpAddress(const std::string& addr, IpVersion version);

		#ifdef _WIN32
			IpAddress(const in_addr* addr);
			IpAddress(const in6_addr* addr);

			bool getSysAddress(in_addr* out) const;
			bool getSysAddress(in6_addr* out) const;
		#endif 

		IpAddress& operator=(const IpAddress &other);
		bool operator==(const IpAddress &other) const;
		bool operator<(const IpAddress &other) const;
		
		std::string toString() const;
		IpVersion version() const;

		bool isValid() const;

	private:
		uint8_t mData[16];
		IpVersion mVersion;
	};
}

namespace Serialize
{
	template<>
	IP_COMM_EXPORT void read<IpComm::IpAddress>(ByteStream* stream, IpComm::IpAddress *out);

	template<>
	IP_COMM_EXPORT void write<IpComm::IpAddress>(ByteStream* stream, const IpComm::IpAddress &val);
}

#endif // _IP_COMM_IP_ADDRESS_H_