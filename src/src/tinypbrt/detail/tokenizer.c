#include <tinypbrt/pch.h>

#include <tinypbrt/detail/tokenizer_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/token_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_tokenizer_init(tpbrt_tokenizer_t* const tokenizer, const tpbrt_string_t str) {
			if (tokenizer == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*tokenizer = (tpbrt_tokenizer_t) { .str = str, .offset = 0u };
		return TPBRT_ERROR_NONE;
	}

	tpbrt_size_t tpbrt_tokenizer_offset(const tpbrt_tokenizer_t* const tokenizer) {
		return tokenizer != TPBRT_NULL ? tokenizer->offset : 0u;
	}

	tpbrt_bool_t tpbrt_tokenizer_peek(tpbrt_tokenizer_t* const tokenizer, tpbrt_token_t* const out_token) {
			if (tokenizer == TPBRT_NULL || out_token == TPBRT_NULL) { return TPBRT_FALSE; }

		const tpbrt_size_t saved_offset = tokenizer->offset;
		const tpbrt_bool_t has_token	= tpbrt_tokenizer_next(tokenizer, out_token);
		tokenizer->offset				= saved_offset;

		return has_token;
	}

	static tpbrt_string_t tpbrt_tokenizer_process_string(tpbrt_tokenizer_t* const tokenizer, const tpbrt_size_t start) {
		const tpbrt_char_t* const data = tokenizer->str.data;
		const tpbrt_char_t* const end  = data + tokenizer->str.size;
		const tpbrt_char_t* p		   = data + tokenizer->offset;

			while (p < end) {
				const tpbrt_char_t c = *p++;
					if (c == '\\') {
							if (p < end) { ++p; }
						continue;
					}
					if (c == '"') { break; }
			}

		tokenizer->offset = (tpbrt_size_t)(p - data);
		return (tpbrt_string_t) { .data = data + start, .size = tokenizer->offset - start };
	}

	static void tpbrt_tokenizer_process_comment(tpbrt_tokenizer_t* const tokenizer) {
		const tpbrt_char_t* const data = tokenizer->str.data;
		const tpbrt_char_t* const end  = data + tokenizer->str.size;
		const tpbrt_char_t* p		   = data + tokenizer->offset;
			while (p < end && *p != '\r' && *p != '\n') { ++p; }
		tokenizer->offset = (tpbrt_size_t)(p - data);
	}

	static tpbrt_string_t tpbrt_tokenizer_process_text(tpbrt_tokenizer_t* const tokenizer, const tpbrt_size_t start) {
		const tpbrt_char_t* const data = tokenizer->str.data;
		const tpbrt_char_t* const end  = data + tokenizer->str.size;
		const tpbrt_char_t* p		   = data + tokenizer->offset;

			while (p < end) {
				const tpbrt_char_t c = *p;
					if (c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == '"' || c == '[' || c == ']') { break; }
				++p;
			}

		tokenizer->offset = (tpbrt_size_t)(p - data);
		return (tpbrt_string_t) { .data = data + start, .size = tokenizer->offset - start };
	}

	tpbrt_bool_t tpbrt_tokenizer_next(tpbrt_tokenizer_t* const tokenizer, tpbrt_token_t* const out_token) {
			if (tokenizer == TPBRT_NULL || out_token == TPBRT_NULL || tokenizer->str.data == TPBRT_NULL) { return TPBRT_FALSE; }

			while (tokenizer->offset < tokenizer->str.size) {
				const tpbrt_size_t start = tokenizer->offset;
				const tpbrt_char_t ch	 = tokenizer->str.data[tokenizer->offset];

				tokenizer->offset++;

					switch (ch) {
					case '[':
						case ']': {
							const tpbrt_string_t view = { .data = tokenizer->str.data + start, .size = 1u };
							tpbrt_token_init(view, out_token);
							return TPBRT_TRUE;
						}
					case ' ':
					case '\n':
					case '\t':
					case '\r':	  continue;
						case '"': {
							const tpbrt_string_t view = tpbrt_tokenizer_process_string(tokenizer, start);
							tpbrt_token_init(view, out_token);
							return TPBRT_TRUE;
						}
						case '#': {
							tpbrt_tokenizer_process_comment(tokenizer);
							continue;
						}
						default: {
							const tpbrt_string_t view = tpbrt_tokenizer_process_text(tokenizer, start);
							tpbrt_token_init(view, out_token);
							return TPBRT_TRUE;
						}
					}
			}

		return TPBRT_FALSE;
	}

#ifdef __cplusplus
}
#endif
