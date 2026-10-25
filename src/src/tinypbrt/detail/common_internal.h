#pragma once
#ifndef _TINYPBRT_COMMON_INTERNAL_H_
#define _TINYPBRT_COMMON_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/common.h>

#include <tinypbrt/detail/error.h>

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TPBRT_MIN(a, b) ((a) < (b) ? (a) : (b))

#pragma region PARSING
	tpbrt_string_t tpbrt_next_value(const tpbrt_char_t* current, const tpbrt_char_t* end);
	tpbrt_size_t tpbrt_count_values(tpbrt_string_t values);
	tpbrt_bool_t tpbrt_read_string_value(tpbrt_string_t value_str, tpbrt_string_t* out_val);
	tpbrt_bool_t tpbrt_read_bool_value(tpbrt_string_t value_str, tpbrt_bool_t* out_val);
	tpbrt_bool_t tpbrt_read_float_value(tpbrt_string_t value_str, tpbrt_float_t* out_val);
	tpbrt_bool_t tpbrt_read_int_value(tpbrt_string_t value_str, tpbrt_int_t* out_val);
	tpbrt_bool_t tpbrt_read_uint_value(tpbrt_string_t value_str, tpbrt_uint_t* out_val);
#pragma endregion

#pragma region STRING
#define TPBRT_STRING_INIT(value) { .data = (value), .size = sizeof((value)) - 1u }
#define TPBRT_STRING(value)		 (tpbrt_string_t) TPBRT_STRING_INIT(value)
#define TPBRT_EMPTY_STRING_INIT	 { .data = TPBRT_NULL, .size = 0u }
#define TPBRT_EMPTY_STRING		 (tpbrt_string_t) TPBRT_EMPTY_STRING_INIT

	tpbrt_error_t tpbrt_copy_chars(tpbrt_char_t* dst, tpbrt_size_t dst_size, const tpbrt_char_t* src, tpbrt_size_t n);

	tpbrt_error_t tpbrt_string_from_cstr(const tpbrt_char_t* cstr, tpbrt_string_t* out_val);

	static inline tpbrt_bool_t tpbrt_string_empty(const tpbrt_string_t str) {
		return str.data == TPBRT_NULL || str.size == 0u ? TPBRT_TRUE : TPBRT_FALSE;
	}

	tpbrt_error_t tpbrt_string_copy_unescaped(tpbrt_string_t* dst, tpbrt_string_t src);
	tpbrt_error_t tpbrt_string_copy(tpbrt_string_t* dst, tpbrt_string_t src);
	tpbrt_error_t tpbrt_string_copy_to(tpbrt_string_t src, tpbrt_char_t* dst, tpbrt_size_t dst_size, tpbrt_size_t* out_size);

	tpbrt_bool_t tpbrt_string_equals_cstr(tpbrt_string_t a, const tpbrt_char_t* cstr);

	static inline tpbrt_bool_t tpbrt_string_equals(const tpbrt_string_t a, const tpbrt_string_t b) {
			if (a.size != b.size) { return TPBRT_FALSE; }
			if (a.size == 0u) { return TPBRT_TRUE; }
			if (a.data == TPBRT_NULL || b.data == TPBRT_NULL) { return a.data == b.data ? TPBRT_TRUE : TPBRT_FALSE; }
			if (a.data[0] != b.data[0]) { return TPBRT_FALSE; }
		return memcmp(a.data, b.data, a.size) == 0 ? TPBRT_TRUE : TPBRT_FALSE;
	}

	void tpbrt_string_free(tpbrt_string_t* str);
#pragma endregion

#pragma region STRING_ARRAY
#define TPBRT_EMPTY_STRING_ARRAY_INIT { .data = TPBRT_NULL, .count = 0 }
#define TPBRT_EMPTY_STRING_ARRAY	  (tpbrt_string_array_t) TPBRT_EMPTY_STRING_ARRAY_INIT

	void tpbrt_string_array_free(tpbrt_string_array_t* array);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_COMMON_INTERNAL_H_
