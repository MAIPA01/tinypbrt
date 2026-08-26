#pragma once
#ifndef _TINYPBRT_COMMON_INTERNAL_H_
#define _TINYPBRT_COMMON_INTERNAL_H_

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TPBRT_STRING(value) (tpbrt_string_t) { .data = value, .size = sizeof(value) - 1 }

	tpbrt_string_t tpbrt_next_token(const tpbrt_char_t* current, const tpbrt_char_t* end);
	tpbrt_bool_t tpbrt_parse_string_token(const tpbrt_string_t* token, tpbrt_string_t* out_val);
	tpbrt_bool_t tpbrt_parse_bool_token(const tpbrt_string_t* token, tpbrt_bool_t* out_val);
	tpbrt_bool_t tpbrt_parse_float_token(const tpbrt_string_t* token, tpbrt_float_t* out_val);
	tpbrt_bool_t tpbrt_parse_int_token(const tpbrt_string_t* token, tpbrt_int_t* out_val);
	tpbrt_bool_t tpbrt_parse_uint_token(const tpbrt_string_t* token, tpbrt_uint_t* out_val);

	tpbrt_error_t tpbrt_copy_string(tpbrt_string_t* dst, const tpbrt_string_t* src);
	tpbrt_bool_t tpbrt_string_equals_cstr(const tpbrt_string_t* a, const tpbrt_char_t* cstr);
	tpbrt_bool_t tpbrt_string_equals_literal(const tpbrt_string_t* a, tpbrt_string_t literal);
	tpbrt_bool_t tpbrt_string_equals(const tpbrt_string_t* a, const tpbrt_string_t* b);
	void tpbrt_free_string(tpbrt_string_t* str);

	void tpbrt_free_string_array(tpbrt_string_array_t* array);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_COMMON_INTERNAL_H_