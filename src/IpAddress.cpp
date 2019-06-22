#include <IpComm/IpAddress.h>

#include <Serialize/Exceptions.h>
#include <Serialize/Binary/ByteStream.h>


#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <ws2tcpip.h>
#include <In6addr.h>
#include "..\include\IpComm\IpAddress.h"

#pragma comment(lib, "ws2_32.lib")

using namespace StdExt;

namespace IpComm
{
	IpAddress IpAddress::any(IpVersion version)
	{
		if (IpVersion::V4 == version)
			return IpAddress(&in4addr_any);
		else if (IpVersion::V6 == version)
			return IpAddress(&in6addr_any);

		return IpAddress();
	}

	IpAddress::IpAddress()
	{
		static_assert( sizeof(IpAddress::mData) >= sizeof(in_addr) &&
		               sizeof(IpAddress::mData) >= sizeof(in6_addr),
		               "Not enough local data to store system IpAddress data.");

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

	IpAddress::IpAddress(const std::string& addr)
		: IpAddress(addr.c_str())
	{
	}

	IpAddress::IpAddress(const StdExt::String& addr)
		: IpAddress(addr.getNullTerminated().data())
	{
	}

	IpAddress::IpAddress(const char* addr, IpVersion version)
		: IpAddress()
	{
		memset(mData, 0, sizeof(mData));

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
	
	IpAddress::IpAddress(const std::string& addr, IpVersion version)
		: IpAddress(addr.c_str(), version)
	{
	}

	IpAddress::IpAddress(const StdExt::String& addr, IpVersion version)
		: IpAddress(addr.getNullTerminated().data(), version)
	{
	}

	IpAddress::IpAddress(const in_addr* addr)
	{
		memset(mData, 0, sizeof(mData));

		mVersion = IpVersion::V4;
		memcpy(&mData[0], addr, sizeof(in_addr));
	}

	IpAddress::IpAddress(const in6_addr* addr)
	{
		memset(mData, 0, sizeof(mData));

		mVersion = IpVersion::V6;
		memcpy(&mData[0], addr, sizeof(in6_addr));
	}

	IpAddress& IpAddress::operator=(const IpAddress &other)
	{
		mVersion = other.mVersion;

		if (IpVersion::NONE != mVersion)
			memcpy(mData, other.mData, sizeof(mData));

		return *this;
	}

	bool IpAddress::operator==(const IpAddress &other) const
	{
		if (mVersion != other.mVersion)
			return false;
		else if (IpVersion::NONE == mVersion)
			return true;
		else
			return (0 == memcmp(mData, &other.mData, sizeof(mData)));
	}

	bool IpAddress::operator!=(const IpAddress& other) const
	{
		if (mVersion != other.mVersion)
			return true;
		else if (IpVersion::NONE == mVersion)
			return false;
		else
			return (0 != memcmp(mData, &other.mData, sizeof(mData)));
	}

	bool IpAddress::operator<(const IpAddress &other) const
	{
		return
			mVersion < other.mVersion ||
			(IpVersion::NONE != mVersion && memcmp(this, &other, sizeof(IpAddress)) < 0);
	}

	StdExt::String IpAddress::toString() const
	{
		if (isValid())
		{
			char strBuffer[64];
			memset(strBuffer, 0, sizeof(strBuffer));

			if (IpVersion::V4 == mVersion)
			{
				in_addr* addr = (in_addr*)&mData[0];
				inet_ntop(AF_INET, addr, strBuffer, sizeof(strBuffer));
			}
			else if (IpVersion::V6 == mVersion)
			{
				in_addr6* addr = (in_addr6*)&mData[0];
				inet_ntop(AF_INET6, addr, strBuffer, sizeof(strBuffer));
			}

			return StdExt::String(strBuffer);
		}

		return StringLiteral("");
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

namespace Serialize::Binary
{
	using namespace IpComm;

	template<>
	void read<IpAddress>(ByteStream* stream, IpAddress *out)
	{
		out->mVersion = stream->read<IpVersion>();

		if (IpVersion::NONE != out->mVersion)
			stream->readRaw(out->mData, sizeof(out->mData));
	}

	template<>
	void write<IpAddress>(ByteStream* stream, const IpAddress &val)
	{
		stream->write(val.mVersion);

		if (IpVersion::NONE != val.mVersion)
			stream->writeRaw(val.mData, sizeof(val.mData));
	}
}
