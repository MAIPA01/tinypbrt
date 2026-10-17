#pragma once
#ifndef _TINYPBRT_COMMON_INTERNAL_H_
#define _TINYPBRT_COMMON_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/common.h>

#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TPBRT_MIN(a, b) ((a) < (b) ? (a) : (b))

#pragma region PARSING
	tpbrt_string_t tpbrt_next_value(const tpbrt_char_t* current, const tpbrt_char_t* end);
	tpbrt_bool_t tpbrt_read_string_value(tpbrt_string_t value_str, tpbrt_string_t* out_val);
	tpbrt_bool_t tpbrt_read_bool_value(tpbrt_string_t value_str, tpbrt_bool_t* out_val);
	tpbrt_bool_t tpbrt_read_float_value(tpbrt_string_t value_str, tpbrt_float_t* out_val);
	tpbrt_bool_t tpbrt_read_int_value(tpbrt_string_t value_str, tpbrt_int_t* out_val);
	tpbrt_bool_t tpbrt_read_uint_value(tpbrt_string_t value_str, tpbrt_uint_t* out_val);
#pragma endregion

#pragma region STRING
#define TPBRT_STRING(value) (tpbrt_string_t) { .data = (value), .size = sizeof((value)) - 1u }
#define TPBRT_EMPTY_STRING	(tpbrt_string_t) { .data = TPBRT_NULL, .size = 0u }

	tpbrt_error_t tpbrt_string_from_cstr(const tpbrt_char_t* cstr, tpbrt_string_t* out_val);

	tpbrt_bool_t tpbrt_string_empty(tpbrt_string_t str);

	tpbrt_error_t tpbrt_string_copy_unescaped(tpbrt_string_t* dst, tpbrt_string_t src);
	tpbrt_error_t tpbrt_string_copy(tpbrt_string_t* dst, tpbrt_string_t src);
	tpbrt_error_t tpbrt_string_copy_to(tpbrt_string_t src, tpbrt_char_t* dst, tpbrt_size_t dst_size, tpbrt_size_t* out_size);

	tpbrt_bool_t tpbrt_string_equals_cstr(tpbrt_string_t a, const tpbrt_char_t* cstr);
	tpbrt_bool_t tpbrt_string_equals(tpbrt_string_t a, tpbrt_string_t b);

	void tpbrt_string_free(tpbrt_string_t* str);
#pragma endregion

#pragma region STRING_ARRAY
#define TPBRT_EMPTY_STRING_ARRAY (tpbrt_string_array_t) { .data = TPBRT_NULL, .count = 0 }

	void tpbrt_string_array_free(tpbrt_string_array_t* array);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_COMMON_INTERNAL_H_