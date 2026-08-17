#include <tinypbrt/pch.h>

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	static const tpbrt_string_t TPBRT_ERROR_MSGS[] = {
		{ .chars = "No error",													   .size = 8	 }, // TPBRT_ERROR_NONE
		{ .chars = "No tokens",													  .size = 9  }, // TPBRT_ERROR_END_OF_FILE
		{ .chars = "Token expected, got end of stream",								.size = 33 }, // TPBRT_ERROR_NO_TOKEN
		{ .chars = "Failed to read file",											.size = 19 }, // TPBRT_ERROR_IO
		{ .chars = "Invalid token",												  .size = 13 }, // TPBRT_ERROR_INVALID_TOKEN
		{ .chars = "Unable to parse float",											.size = 21 }, // TPBRT_ERROR_PARSE_FLOAT
		{ .chars = "Unable to parse integer",										  .size = 23 }, // TPBRT_ERROR_PARSE_INT
		{ .chars = "Unable to parse bool",										   .size = 20 }, // TPBRT_ERROR_PARSE_BOOL
		{ .chars = "Unexpected number of arguments in array",						  .size = 39 }, // TPBRT_ERROR_SLICE
		{ .chars = "Unsupported directive",											.size = 21 }, // TPBRT_ERROR_UNKNOWN_DIRECTIVE
		{ .chars = "Expected string token",											.size = 21 }, // TPBRT_ERROR_INVALID_STRING
		{ .chars = "Unable to parse option value",								   .size = 28 }, // TPBRT_ERROR_INVALID_OPTION_VALUE
		{ .chars = "Unsupported coordinate system",									.size = 29 }, // TPBRT_ERROR_UNKNOWN_COORDINATE_SYSTEM
		{ .chars = "Invalid parameter name",										 .size = 22 }, // TPBRT_ERROR_INVALID_PARAM_NAME
		{ .chars = "Parameter type is invalid",										.size = 25 }, // TPBRT_ERROR_INVALID_PARAM_TYPE
		{ .chars = "Found duplicated parameter",									 .size = 26 }, // TPBRT_ERROR_DUPLICATE_PARAM_NAME
		{ .chars = "Duplicated WorldBegin statement",								  .size = 31 }, // TPBRT_ERROR_WORLD_ALREADY_STARTED
		{ .chars = "Element is not allowed",										 .size = 22 }, // TPBRT_ERROR_ELEMENT_NOT_ALLOWED
		{ .chars = "Too many AttributeEnd",											.size = 21 }, // TPBRT_ERROR_TOO_MANY_END_ATTRIBUTES
		{ .chars  = "Attempt to restore CoordSysTransform matrix with invalid name",
			.size = 61																			 }, // TPBRT_ERROR_INVALID_MATRIX_NAME
		{ .chars = "Invalid camera type",											.size = 19 }, // TPBRT_ERROR_INVALID_CAMERA_TYPE
		{ .chars = "Unknown object type",											.size = 19 }, // TPBRT_ERROR_INVALID_OBJECT_TYPE
		{ .chars = "Unexpected token received",										.size = 25 }, // TPBRT_ERROR_UNEXPECTED_TOKEN
		{ .chars = "Required param is missing",										.size = 25 }, // TPBRT_ERROR_MISSING_REQUIRED_PARAMETER
		{ .chars = "Nested object attributes are not allowed",					   .size = 40 }, // TPBRT_ERROR_NESTED_OBJECTS
		{ .chars = "Not found",													  .size = 9  }, // TPBRT_ERROR_NOT_FOUND
		{ .chars = "Undefined error",												.size = 15 }, // DEFAULT
	};

	tpbrt_size_t tpbrt_get_error_message(tpbrt_error_t error, tpbrt_char_t* buffer, const tpbrt_size_t buffer_size) {
		error					= min(error, TPBRT_ERROR_MAX_NUM);
		const tpbrt_size_t size = min(TPBRT_ERROR_MSGS[error].size, buffer_size - 1u);
		memcpy(buffer, TPBRT_ERROR_MSGS[error].chars, size);
		buffer[size] = '\0';
		return size;
	}

#ifdef __cplusplus
}
#endif