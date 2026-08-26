#include <inttypes.h>
#include <tinypbrt/pch.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region PARSING

	tpbrt_string_t tpbrt_next_token(const tpbrt_char_t* current, const tpbrt_char_t* const end) {
		tpbrt_string_t token = { .data = TPBRT_NULL, .size = 0 };

			if (current == TPBRT_NULL || end == TPBRT_NULL) { return token; }

			while (current < end && isspace((tpbrt_uint_t)*current)) { ++current; }
			if (current >= end) { return token; }

		const tpbrt_char_t* start	= current;
		const tpbrt_bool_t isString = *start == '\"' ? TPBRT_TRUE : TPBRT_FALSE;
			if (isString) { ++current; }
			while (current < end && ((!isString && !isspace((tpbrt_uint_t)*current)) || (isString && *current != '\"'))) {
				++current;
			}

		token.data = (tpbrt_char_t*)start;
		token.size = (tpbrt_size_t)(current - start);
		return token;
	}

	tpbrt_bool_t tpbrt_parse_string_token(const tpbrt_string_t* const token, tpbrt_string_t* const out_val) {
			if (token == TPBRT_NULL || token->data == TPBRT_NULL || token->size < 2 || out_val == TPBRT_NULL) {
				return TPBRT_FALSE;
			}

			if (token->data[0] != '\"' || token->data[token->size - 1] != '\"') { return TPBRT_FALSE; }

		out_val->data = token->data + 1;
		out_val->size = token->size - 2;
		return TPBRT_TRUE;
	}

	tpbrt_bool_t tpbrt_parse_bool_token(const tpbrt_string_t* const token, tpbrt_bool_t* const out_val) {
		static const tpbrt_string_t TRUE_STR  = TPBRT_STRING("true");
		static const tpbrt_string_t FALSE_STR = TPBRT_STRING("false");

			if (token == TPBRT_NULL || token->data == TPBRT_NULL || token->size < 4 || out_val == TPBRT_NULL) {
				return TPBRT_FALSE;
			}

			if (tpbrt_string_equals(token, &TRUE_STR)) {
				*out_val = TPBRT_TRUE;
				return TPBRT_TRUE;
			}

			if (tpbrt_string_equals(token, &FALSE_STR)) {
				*out_val = TPBRT_FALSE;
				return TPBRT_TRUE;
			}

		return TPBRT_FALSE;
	}

	tpbrt_bool_t tpbrt_parse_float_token(const tpbrt_string_t* const token, tpbrt_float_t* const out_val) {
		static const tpbrt_size_t MAX_BUFFER_SIZE = 64;

			if (token == TPBRT_NULL || token->data == TPBRT_NULL || token->size < 1 || out_val == TPBRT_NULL) {
				return TPBRT_FALSE;
			}

		tpbrt_char_t buf[MAX_BUFFER_SIZE];
		const tpbrt_size_t n = min(token->size, MAX_BUFFER_SIZE - 1);
		strncpy_s(buf, sizeof(tpbrt_char_t) * MAX_BUFFER_SIZE, token->data, n);
		buf[n] = '\0';
		tpbrt_char_t* end;
		*out_val = strtof(buf, &end);
		return end != buf;
	}

	tpbrt_bool_t tpbrt_parse_int_token(const tpbrt_string_t* const token, tpbrt_int_t* const out_val) {
		static const tpbrt_size_t MAX_BUFFER_SIZE = 64;
		static const tpbrt_int_t INT_RANIX		  = 10;

			if (token == TPBRT_NULL || token->data == TPBRT_NULL || token->size < 1 || out_val == TPBRT_NULL) {
				return TPBRT_FALSE;
			}

		tpbrt_char_t buf[MAX_BUFFER_SIZE];
		const tpbrt_size_t n = min(token->size, MAX_BUFFER_SIZE - 1);
		strncpy_s(buf, sizeof(tpbrt_char_t) * MAX_BUFFER_SIZE, token->data, n);
		buf[n] = '\0';
		tpbrt_char_t* end;
		*out_val = (tpbrt_int_t)strtol(buf, &end, INT_RANIX);
		return end != buf;
	}

	tpbrt_bool_t tpbrt_parse_uint_token(const tpbrt_string_t* token, tpbrt_uint_t* const out_val) {
		static const tpbrt_size_t MAX_BUFFER_SIZE = 64;
		static const tpbrt_int_t UINT_RANIX		  = 10;

			if (token == TPBRT_NULL || token->data == TPBRT_NULL || token->size < 1 || out_val == TPBRT_NULL) {
				return TPBRT_FALSE;
			}

		tpbrt_char_t buf[MAX_BUFFER_SIZE];
		const tpbrt_size_t n = min(token->size, MAX_BUFFER_SIZE - 1);
		strncpy_s(buf, sizeof(tpbrt_char_t) * MAX_BUFFER_SIZE, token->data, n);
		buf[n] = '\0';
		tpbrt_char_t* end;
		*out_val = (tpbrt_uint_t)strtoul(buf, &end, UINT_RANIX);
		return end != buf;
	}

#pragma endregion

	tpbrt_error_t tpbrt_copy_string(tpbrt_string_t* const dst, const tpbrt_string_t* const src) {
			if (dst == TPBRT_NULL || src == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (src->size == 0 || src->data == TPBRT_NULL) {
				dst->data = TPBRT_NULL;
				dst->size = 0;
				return TPBRT_ERROR_NONE;
			}

		dst->data = malloc(sizeof(tpbrt_char_t) * (src->size + 1));
			if (dst->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		memcpy(dst->data, src->data, sizeof(tpbrt_char_t) * src->size);
		dst->data[src->size] = '\0';
		dst->size			 = src->size;

		return TPBRT_ERROR_NONE;
	}

	tpbrt_bool_t tpbrt_string_equals_cstr(const tpbrt_string_t* const a, const tpbrt_char_t* const cstr) {
		const tpbrt_string_t str = { .data = (tpbrt_char_t*)cstr, .size = strlen(cstr) };
		return tpbrt_string_equals(a, &str);
	}

	tpbrt_bool_t tpbrt_string_equals_literal(const tpbrt_string_t* a, const tpbrt_string_t literal) {
		return tpbrt_string_equals(a, &literal);
	}

	tpbrt_bool_t tpbrt_string_equals(const tpbrt_string_t* a, const tpbrt_string_t* b) {
			if (a == TPBRT_NULL || b == TPBRT_NULL) { return TPBRT_FALSE; }
			if (a->size != b->size) { return TPBRT_FALSE; }
		return strncmp(a->data, b->data, a->size) == 0;
	}

	void tpbrt_free_string(tpbrt_string_t* str) {
			if (str == TPBRT_NULL || str->data == TPBRT_NULL) { return; }
		free(str->data);
		str->data = TPBRT_NULL;
		str->size = 0;
	}

	void tpbrt_free_string_array(tpbrt_string_array_t* const array) {
			if (array == TPBRT_NULL || array->data == TPBRT_NULL) { return; }

			for (tpbrt_size_t i = 0; i < array->count; ++i) { tpbrt_free_string(&array->data[i]); }

		free(array->data);
		array->data	 = TPBRT_NULL;
		array->count = 0;
	}

#ifdef __cplusplus
}
#endif