#ifndef _IP_COMM_CONFIG_H_
#define _IP_COMM_CONFIG_H_

////////////////////////////////////////////////////////

#if !defined(IP_COMM_DYNAMIC) || !defined(_WIN32)
#	define IP_COMM_EXPORT
#else
#	ifdef IP_COMM_BUILD
#		define IP_COMM_EXPORT __declspec(dllexport)
#	else
#		define IP_COMM_EXPORT __declspec(dllimport)
#	endif
#endif

#endif // _IP_COMM_CONFIG_H_