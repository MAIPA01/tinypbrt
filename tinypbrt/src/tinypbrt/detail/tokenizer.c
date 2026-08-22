#include <tinypbrt/pch.h>

#include <tinypbrt/detail/tokenizer_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	void tpbrt_tokenizer_init(tpbrt_tokenizer_t* const tokenizer, const tpbrt_char_t* const str, const tpbrt_size_t length) {
			if (tokenizer == TPBRT_NULL) { return; }

		tokenizer->str.data = (tpbrt_char_t*)str;
		tokenizer->str.size = length;
		tokenizer->offset	= 0u;
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

	tpbrt_bool_t tpbrt_tokenizer_next(tpbrt_tokenizer_t* tokenizer, tpbrt_token_t* const out_token) {
			if (tokenizer == TPBRT_NULL || out_token == TPBRT_NULL) { return TPBRT_FALSE; }

			while (tokenizer->offset < tokenizer->str.size) {
				const tpbrt_size_t start = tokenizer->offset;
				const tpbrt_char_t ch	 = tokenizer->str.data[tokenizer->offset];

				tokenizer->offset++;

					switch (ch) {
					case '[':
						case ']': {
							const tpbrt_string_t view = { .data = tokenizer->str.data + start, .size = 1u };
							tpbrt_create_token(&view, out_token);
							return TPBRT_TRUE;
						}
					case ' ':
					case '\n':
					case '\t':
					case '\r':	  continue;
						case '"': {
								while (tokenizer->offset < tokenizer->str.size) {
									const tpbrt_char_t c = tokenizer->str.data[tokenizer->offset++];
										if (c == '"') { break; }
								}
							const tpbrt_string_t view = { .data = tokenizer->str.data + start,
								.size							= tokenizer->offset - start };
							tpbrt_create_token(&view, out_token);
							return TPBRT_TRUE;
						}
						case '#': {
								while (tokenizer->offset < tokenizer->str.size) {
									const char c = tokenizer->str.data[tokenizer->offset];
										if (c == '\r' || c == '\n') { break; }
									tokenizer->offset++;
								}
							continue;
						}
						default: {
								while (tokenizer->offset < tokenizer->str.size) {
									const char c = tokenizer->str.data[tokenizer->offset];
										if (c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == '"' || c == '[' || c == ']') {
											break;
										}
									tokenizer->offset++;
								}
							const tpbrt_string_t view = { .data = tokenizer->str.data + start,
								.size							= tokenizer->offset - start };
							tpbrt_create_token(&view, out_token);
							return TPBRT_TRUE;
						}
					}
			}

		return TPBRT_FALSE;
	}

#ifdef __cplusplus
}
#endif