#pragma once
#ifndef _TINYPBRT_COMMON_INTERNAL_H_
#define _TINYPBRT_COMMON_INTERNAL_H_

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TPBRT_STRING(value) (tpbrt_string_t) { .data = value, .size = sizeof(value) - 1 }

	const tpbrt_char_t* tpbrt_next_token(const tpbrt_char_t* current, const tpbrt_char_t* end, tpbrt_size_t* out_len);
	tpbrt_bool_t tpbrt_parse_string_token(const tpbrt_char_t* token, tpbrt_size_t len, tpbrt_string_t* out_val);
	tpbrt_bool_t tpbrt_parse_float_token(const tpbrt_char_t* token, tpbrt_size_t len, tpbrt_float_t* out_val);
	tpbrt_bool_t tpbrt_parse_int_token(const tpbrt_char_t* token, tpbrt_size_t len, tpbrt_int_t* out_val);
	tpbrt_bool_t tpbrt_parse_uint_token(const tpbrt_char_t* token, tpbrt_size_t len, tpbrt_uint_t* out_val);
	tpbrt_error_t tpbrt_copy_string(tpbrt_string_t* dst, const tpbrt_string_t* src);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_COMMON_INTERNAL_H_