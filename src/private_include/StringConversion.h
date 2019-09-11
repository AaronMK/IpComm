#ifndef _IP_COMM_STRING_CONVERSION_H_
#define _IP_COMM_STRING_CONVERSION_H_

#ifdef _WIN32

#include <StdExt/String.h>
#include <StdExt/Memory.h>

#include <wchar.h>
#include <errno.h>

#include <limits>

namespace StdExt
{

	static String fromWChar(const wchar_t* str)
	{
		size_t requiredSize = 0;
		mbstate_t mbState;

		errno_t result = wcsrtombs_s(
			&requiredSize, nullptr, 0, 
			&str, std::numeric_limits<size_t>::max(),
			&mbState
		);

		StackBuffer<128> buffer(requiredSize);

		result = wcsrtombs_s(
			&requiredSize, (char*)buffer.data(), buffer.size(), 
			&str, std::numeric_limits<size_t>::max(),
			&mbState
		);

		return String((char*)buffer.data(), buffer.size());
	}
}

#endif // _WIN32

#endif // _IP_COMM_STRING_CONVERSION_H_