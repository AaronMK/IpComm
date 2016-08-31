#include <IpComm/IpAddress.h>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <ws2tcpip.h>
#include <In6addr.h>

#pragma comment(lib, "ws2_32.lib")

namespace IpComm
{
	IpAddress IpAddress::any(IpVersion version)
	{
		if (IpVersion::V4 == version)
		{
			in_addr winAddr;
			memset(&winAddr, 0, sizeof(in_addr));

			winAddr.S_un.S_addr = INADDR_ANY;

			return IpAddress(&winAddr);
		}
		else if (IpVersion::V6 == version)
		{
			return IpAddress(&in6addr_any);
		}

		return IpAddress();
	}

	IpAddress::IpAddress()
	{
		static_assert( sizeof(IpAddress::mData) >= sizeof(in_addr) &&
		               sizeof(IpAddress::mData) >= sizeof(in6_addr),
		               "Not enough local data to store system IpAddress data.");

		memset(mData, 0, sizeof(mData));
		mVersion = IpVersion::NONE;
	}
	
	IpAddress::IpAddress(const char* addr)
		: IpAddress()
	{
		IpAddress addrTest(addr, IpVersion::V4);

		if (addrTest.isValid())
		{
			*this = addrTest;
			return;
		}

		addrTest = IpAddress(addr, IpVersion::V6);

		if (addrTest.isValid())
		{
			*this = addrTest;
			return;
		}
	}

	IpAddress::IpAddress(const char* addr, IpVersion version)
	{
		memset(mData, 0, sizeof(mData));
		mVersion = IpVersion::NONE;

		if (IpVersion::V4 == version)
		{
			in_addr* addrPtr = reinterpret_cast<in_addr*>(&mData[0]);
			if ( 1 == InetPton(AF_INET, addr, addrPtr) )
			{
				mVersion = IpVersion::V4;
			}
		}
		else if (IpVersion::V6 == version)
		{
			in6_addr* addrPtr = reinterpret_cast<in6_addr*>(&mData[0]);
			if ( 1 == InetPton(AF_INET6, addr, addrPtr) )
			{
				mVersion = IpVersion::V6;
			}
		}
	}

	
	IpAddress::IpAddress(const in_addr* addr)
	{
		mVersion = IpVersion::V4;
		memcpy(&mData[0], addr, sizeof(in_addr));
	}

	IpAddress::IpAddress(const in6_addr* addr)
	{
		mVersion = IpVersion::V6;
		memcpy(&mData[0], addr, sizeof(in6_addr));
	}

	IpAddress& IpAddress::operator=(const IpAddress &other)
	{
		memcpy(mData, other.mData, sizeof(mData));
		mVersion = other.mVersion;

		return *this;
	}

	bool IpAddress::operator==(const IpAddress &other) const
	{
		return ( 0 == memcmp(mData, &other.mData, sizeof(mData)) &&
		         mVersion == other.mVersion);
	}

	bool IpAddress::operator<(const IpAddress &other) const
	{
		return (memcmp(this, &other, sizeof(IpAddress)) < 0);
	}

	std::string IpAddress::toString() const
	{
		if (isValid())
		{
			char strBuffer[64];
			memset(strBuffer, 0, sizeof(strBuffer));

			if (IpVersion::V4 == mVersion)
				InetNtop(AF_INET, (void*)&mData[0], strBuffer, sizeof(strBuffer));
			else if (IpVersion::V6 == mVersion)
				InetNtop(AF_INET6, (void*)&mData[0], strBuffer, sizeof(strBuffer));

			return std::string(strBuffer);
		}

		return "";
	}

	IpVersion IpAddress::version() const
	{
		return mVersion;
	}

	bool IpAddress::isValid() const
	{
		return (IpVersion::NONE != mVersion);
	}

	bool IpAddress::getSysAddress(in_addr* out) const
	{
		if (nullptr != out && IpVersion::V4 == mVersion)
		{
			const in_addr* addrPtr = reinterpret_cast<const in_addr*>(&mData[0]);
			*out = *addrPtr;

			return true;
		}

		return false;
	}

	bool IpAddress::getSysAddress(in6_addr* out) const
	{
		if (nullptr != out && IpVersion::V6 == mVersion)
		{
			const in6_addr* addrPtr = reinterpret_cast<const in6_addr*>(&mData[0]);
			*out = *addrPtr;

			return true;
		}

		return false;
	}
}

#else
#	error "SysComm::IpAddress not supported."
#endif

namespace Serialize
{
	using namespace IpComm;

	/**
	 * @nodocument
	 */
	static const std::string IP_INVALID("IP_INVALID");

	template<>
	bool read<IpAddress>(ByteStream* stream, IpAddress *out)
	{
		seek_t seekBack = stream->getSeekPosition();

		std::string strVal;

		if (read(stream, &strVal))
		{
			IpAddress addr;

			if (addr.isValid() || IP_INVALID == strVal)
			{
				*out = IpAddress(strVal.c_str());
				return true;
			}
			else
			{
				stream->seek(seekBack);
				return false;
			}
		}

		return false;
	}

	template<>
	bool write<IpAddress>(ByteStream* stream, const IpAddress &val)
	{
		if (val.isValid())
			return write(stream, val.toString());

		return write(stream, IP_INVALID);
	}
}