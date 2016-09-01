#ifndef _IP_COMM_OP_RESULT_H_
#define _IP_COMM_OP_RESULT_H_

#include <IpComm/Config.h>

#include <cstdint>

namespace IpComm
{
	/**
	 * @brief
	 *  Object returned to report success/failure of an operation.
	 *
	 *  The %OpResult is implemented as a glorified enumeration.  In the interest of
	 *  binary compatibility when compiled as a shared library, actual values
	 *  corresponding to results are buried inside the implementation.  Even if a client
	 *  of the library was not compiled against the same version of the library, toString()
	 *  will still return relevent information.
	 */
	struct IP_COMM_EXPORT OpResult
	{
		friend struct OpResultInternal;

		OpResult(const OpResult &other);

		/**
		 * @brief
		 *  Return true for any value that denotes a successful operation, or a lack
		 *  of an error condition.
		 */
		operator bool() const;

		bool operator==(const OpResult &other) const;
		OpResult& operator=(const OpResult &other);

		const char* toString() const;
		
		/**
		 * @brief
		 *  No operation has occured that would generate a success or fail result.
		 */
		static const OpResult NONE;

		/**
		 * @brief
		 *  Operation was a success.
		 */
		static const OpResult SUCCESS;

		/**
		 * @brief
		 *  The operation did not complete within a prescribed amount of time.
		 */
		static const OpResult TIME_OUT;

		/**
		 * @brief
		 *  The operation failed because a connection has not been established.
		 */
		static const OpResult NOT_CONNECTED;

		/**
		 * @brief
		 *  The operation failed because the server is not listening.
		 */
		static const OpResult NOT_LISTENING;

		/**
		 * @brief
		 *  The operation was failed because it was invalid for
		 *  an object already in the connected state.
		 */
		static const OpResult ALREADY_CONNECTED;

		/**
		 * @brief
		 *  The IP Address passed was not valid.  For servers this usually means a
		 *  passed address does not have an associated IP Interface.
		 */
		static const OpResult INVALID_IP_ADDRESS;

		/**
		 * @brief
		 *  There was a failure of the internal socketing subsystem.
		 */
		static const OpResult INTERNAL_SUBSYSTEM_FAILURE;

		/**
		 * @brief
		 *  The buffer passed to receive data was not valid.  Usually this
		 *  means the pointer was null.
		 */
		static const OpResult INVALID_BUFFER;

		/**
		 * @brief
		 *  The buffer passed to receive data was not valid.  Usually this
		 *  means the pointer was null.
		 */
		static const OpResult CONNCETION_REJECTED;

		/**
		 * @brief
		 *  The connection was reset.
		 */
		static const OpResult CONNCETION_RESET;

		/**
		 * @brief
		 *  The IP/Port combination passed to create a server is already
		 *  in use.
		 */
		static const OpResult IP_PORT_IN_USE;

		/**
		 * @brief
		 *  The requested functionality has not been implemented.
		 */
		static const OpResult NOT_IMPLEMENTED;

		/**
		 * @brief
		 *  An unknown error has occured.
		 */
		static const OpResult UNKNOWN_ERROR;

	private:
		OpResult(uint32_t intVal);

		const char* ErrorText;
		uint32_t mValue;
	};
}

#endif // _IP_COMM_OP_RESULT_H_