#include <pch.hpp>

TEST(params, parse_param_type) {
	tpbrt_error_t err;
	tpbrt_param_type_t type;

	err = tpbrt_param_type_from_string(TPBRT_STRING("bool"), &type);
	ASSERT_EQ(err, TPBRT_ERROR_NONE);
	ASSERT_EQ(type, TPBRT_PARAM_TYPE_BOOL);
}