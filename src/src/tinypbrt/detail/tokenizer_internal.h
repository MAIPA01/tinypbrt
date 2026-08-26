#pragma once
#ifndef _TINYPBRT_TOKENIZER_INTERNAL_H_
#define _TINYPBRT_TOKENIZER_INTERNAL_H_

#include <tinypbrt/detail/token_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct tpbrt_tokenizer_t {
		tpbrt_string_t str;
		tpbrt_size_t offset;
	} tpbrt_tokenizer_t;

	tpbrt_error_t tpbrt_tokenizer_init(tpbrt_tokenizer_t* tokenizer, const tpbrt_string_t* str);

	tpbrt_size_t tpbrt_tokenizer_offset(const tpbrt_tokenizer_t* tokenizer);

	tpbrt_bool_t tpbrt_tokenizer_peek(tpbrt_tokenizer_t* tokenizer, tpbrt_token_t* out_token);

	tpbrt_bool_t tpbrt_tokenizer_next(tpbrt_tokenizer_t* tokenizer, tpbrt_token_t* out_token);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_TOKENIZER_INTERNAL_H_
