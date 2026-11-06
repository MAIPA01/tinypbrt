#pragma once
#ifndef _TINYPBRT_TOKENIZER_INTERNAL_H_
#define _TINYPBRT_TOKENIZER_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/error.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup tokenizer-private Tokenizer
	 * @ingroup tinypbrt-private
	 */

	/**
	 * @brief Tokenizer data.
	 * @ingroup tokenizer-private
	 */
	typedef struct tpbrt_tokenizer_t {
		/// @brief String to tokenize.
		tpbrt_string_t str;
		/// @brief Current offset in string.
		tpbrt_size_t offset;
	} tpbrt_tokenizer_t;

	/**
	 * @brief Initializes tokenizer.
	 * @param tokenizer Tokenizer data.
	 * @param str String to tokenize.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup tokenizer-private
	 */
	tpbrt_error_t tpbrt_tokenizer_init(tpbrt_tokenizer_t* tokenizer, tpbrt_string_t str);

	/**
	 * @brief Gets next token. Doesn't change current tokenizer offset.
	 * @param tokenizer Tokenizer data.
	 * @param out_token Output token value.
	 * @return TPBRT_TRUE if could get next token else TPBRT_FALSE.
	 * @ingroup tokenizer-private
	 */
	tpbrt_bool_t tpbrt_tokenizer_peek(tpbrt_tokenizer_t* tokenizer, tpbrt_token_t* out_token);

	/**
	 * @brief Gets next token. Changes current tokenizer offset.
	 * @param tokenizer Tokenizer data.
	 * @param out_token Output token value.
	 * @return TPBRT_TRUE if could get next token else TPBRT_FALSE.
	 * @ingroup tokenizer-private
	 */
	tpbrt_bool_t tpbrt_tokenizer_next(tpbrt_tokenizer_t* tokenizer, tpbrt_token_t* out_token);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_TOKENIZER_INTERNAL_H_
