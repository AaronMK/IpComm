#include <IpComm/OpResult.h>

#include <array>
#include <mutex>

namespace IpComm
{
	enum : uint32_t
	{
		// Success Values
		OP_RESULT_NONE,
		OP_RESULT_SUCCESS,

		// Error Values
		OP_RESULT_TIME_OUT,
		OP_RESULT_NOT_CONNECTED,
		OP_RESULT_NOT_LISTENING,
		OP_RESULT_ALREADY_CONNECTED,
		OP_RESULT_INVALID_IP_ADDRESS,
		OP_RESULT_INTERNAL_SUBSYSTEM_FAILURE,
		OP_RESULT_INVALID_BUFFER,
		OP_RESULT_CONNCETION_REJECTED,
		OP_RESULT_CONNCETION_RESET,
		OP_RESULT_IP_PORT_IN_USE,
		OP_RESULT_NOT_IMPLEMENTED,
		OP_RESULT_UNKNOWN_ERROR,

		NUM_OP_RESULTS
	};

	std::once_flag initResultStrings;
	static std::array<char*, NUM_OP_RESULTS>ResultStrings;

	struct OpResultInternal
	{
		static OpResult create(int32_t value)
		{
			return OpResult(value);
		}
	};

// If doxygen sees this, it shows internal details in documentation not
// relevent to clients of the library.
#ifndef DOXYGEN 
	const OpResult OpResult::NONE = OpResultInternal::create(OP_RESULT_NONE);
	const OpResult OpResult::SUCCESS = OpResultInternal::create(OP_RESULT_SUCCESS);
	const OpResult OpResult::TIME_OUT = OpResultInternal::create(OP_RESULT_TIME_OUT);
	const OpResult OpResult::NOT_CONNECTED = OpResultInternal::create(OP_RESULT_NOT_CONNECTED);
	const OpResult OpResult::NOT_LISTENING = OpResultInternal::create(OP_RESULT_NOT_LISTENING);
	const OpResult OpResult::ALREADY_CONNECTED = OpResultInternal::create(OP_RESULT_ALREADY_CONNECTED);
	const OpResult OpResult::INVALID_IP_ADDRESS = OpResultInternal::create(OP_RESULT_INVALID_IP_ADDRESS);
	const OpResult OpResult::INTERNAL_SUBSYSTEM_FAILURE = OpResultInternal::create(OP_RESULT_INTERNAL_SUBSYSTEM_FAILURE);
	const OpResult OpResult::INVALID_BUFFER = OpResultInternal::create(OP_RESULT_INVALID_BUFFER);
	const OpResult OpResult::CONNCETION_REJECTED = OpResultInternal::create(OP_RESULT_CONNCETION_REJECTED);
	const OpResult OpResult::CONNCETION_RESET = OpResultInternal::create(OP_RESULT_CONNCETION_RESET);
	const OpResult OpResult::IP_PORT_IN_USE = OpResultInternal::create(OP_RESULT_IP_PORT_IN_USE);
	const OpResult OpResult::NOT_IMPLEMENTED = OpResultInternal::create(OP_RESULT_NOT_IMPLEMENTED);
	const OpResult OpResult::UNKNOWN_ERROR = OpResultInternal::create(OP_RESULT_UNKNOWN_ERROR);
#endif // DOXYGEN

	OpResult::OpResult(uint32_t intVal)
	{
		mValue = intVal;
		ErrorText = toString();
	}

	OpResult::OpResult(const OpResult &other)
	{
		mValue = other.mValue;
		ErrorText = toString();
	}

	OpResult::operator bool() const
	{
		return (mValue <= OP_RESULT_SUCCESS);
	}

	bool OpResult::operator==(const OpResult &other) const
	{
		return (other.mValue == mValue);
	}

	OpResult& OpResult::operator=(const OpResult &other)
	{
		mValue = other.mValue;
		return *this;
	}

	const char* OpResult::toString() const
	{
		std::call_once(initResultStrings, []()
		{
			ResultStrings.fill("");

			ResultStrings[OP_RESULT_NONE] = "None";
			ResultStrings[OP_RESULT_SUCCESS] = "Success";
			ResultStrings[OP_RESULT_TIME_OUT] = "Time Out";
			ResultStrings[OP_RESULT_NOT_CONNECTED] = "Not Connected";
			ResultStrings[OP_RESULT_NOT_LISTENING] = "Not Listening";
			ResultStrings[OP_RESULT_ALREADY_CONNECTED] = "Already Connected";
			ResultStrings[OP_RESULT_INVALID_IP_ADDRESS] = "Invalid IP Address";
			ResultStrings[OP_RESULT_INTERNAL_SUBSYSTEM_FAILURE] = "Internal Subsystem Failure";
			ResultStrings[OP_RESULT_INVALID_BUFFER] = "Invalid Buffer";
			ResultStrings[OP_RESULT_CONNCETION_REJECTED] = "Conneciton Rejected";
			ResultStrings[OP_RESULT_CONNCETION_RESET] = "Conneciton Reset";
			ResultStrings[OP_RESULT_IP_PORT_IN_USE] = "Requested IP/Port combination already in use.";
			ResultStrings[OP_RESULT_NOT_IMPLEMENTED] = "Functionality has not been implemented.";
			ResultStrings[OP_RESULT_UNKNOWN_ERROR] = "Unknown Error";
		});

		return (mValue < ResultStrings.size()) ? ResultStrings[mValue] : "<unknown value>";
	}
}