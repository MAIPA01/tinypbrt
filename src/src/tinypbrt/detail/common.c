#include <tinypbrt/pch.h>

#include <tinypbrt/detail/common_internal.h>

#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region PARSING

	tpbrt_string_t tpbrt_next_value(const tpbrt_char_t* current, const tpbrt_char_t* const end) {
		tpbrt_string_t value = TPBRT_EMPTY_STRING;

			if (current == TPBRT_NULL || end == TPBRT_NULL) { return value; }

			while (current < end && isspace((tpbrt_uint_t)*current)) { ++current; }
			if (current >= end) { return value; }

		const tpbrt_char_t* start	 = current;
		const tpbrt_bool_t is_string = *start == '\"' ? TPBRT_TRUE : TPBRT_FALSE;
			if (is_string && current < end) { ++current; }
			while (current < end && ((!is_string && !isspace((tpbrt_uint_t)*current)) || (is_string && *current != '\"'))) {
				++current;
			}
			if (is_string && current < end) { ++current; }

		value = (tpbrt_string_t) { .data = (tpbrt_char_t*)start, .size = current - start };
		return value;
	}

	tpbrt_bool_t tpbrt_read_string_value(const tpbrt_string_t value_str, tpbrt_string_t* const out_val) {
			if (tpbrt_string_empty(value_str) == TPBRT_TRUE || value_str.size < 2u || out_val == TPBRT_NULL) {
				return TPBRT_FALSE;
			}

			if (value_str.data[0u] != '\"' || value_str.data[value_str.size - 1u] != '\"') { return TPBRT_FALSE; }

		*out_val = (tpbrt_string_t) { .data = value_str.data + 1u, .size = value_str.size - 2u };
		return TPBRT_TRUE;
	}

	tpbrt_bool_t tpbrt_read_bool_value(const tpbrt_string_t value_str, tpbrt_bool_t* const out_val) {
		static const tpbrt_string_t TRUE_STR  = TPBRT_STRING("true");
		static const tpbrt_string_t FALSE_STR = TPBRT_STRING("false");

			if (tpbrt_string_empty(value_str) == TPBRT_TRUE || value_str.size < 4u || out_val == TPBRT_NULL) {
				return TPBRT_FALSE;
			}

			if (tpbrt_string_equals(value_str, TRUE_STR) == TPBRT_TRUE) {
				*out_val = TPBRT_TRUE;
				return TPBRT_TRUE;
			}

			if (tpbrt_string_equals(value_str, FALSE_STR) == TPBRT_TRUE) {
				*out_val = TPBRT_FALSE;
				return TPBRT_TRUE;
			}

		return TPBRT_FALSE;
	}

	tpbrt_bool_t tpbrt_read_float_value(const tpbrt_string_t value_str, tpbrt_float_t* const out_val) {
		static const tpbrt_size_t MAX_BUFFER_SIZE = 64u;

			if (tpbrt_string_empty(value_str) == TPBRT_TRUE || value_str.size < 1u || out_val == TPBRT_NULL) {
				return TPBRT_FALSE;
			}

		tpbrt_char_t buf[MAX_BUFFER_SIZE];
			if (tpbrt_string_copy_to(value_str, buf, MAX_BUFFER_SIZE, TPBRT_NULL) != TPBRT_ERROR_NONE) { return TPBRT_FALSE; }

		tpbrt_char_t* end;
		*out_val = strtof(buf, &end);
		return end != buf;
	}

	tpbrt_bool_t tpbrt_read_int_value(const tpbrt_string_t value_str, tpbrt_int_t* const out_val) {
		static const tpbrt_size_t MAX_BUFFER_SIZE = 64u;
		static const tpbrt_int_t INT_RANIX		  = 10;

			if (tpbrt_string_empty(value_str) == TPBRT_TRUE || value_str.size < 1u || out_val == TPBRT_NULL) {
				return TPBRT_FALSE;
			}

		tpbrt_char_t buf[MAX_BUFFER_SIZE];
			if (tpbrt_string_copy_to(value_str, buf, MAX_BUFFER_SIZE, TPBRT_NULL) != TPBRT_ERROR_NONE) { return TPBRT_FALSE; }

		tpbrt_char_t* end;
		const tpbrt_longlong_t parsed = strtoll(buf, &end, INT_RANIX);
			if (end == buf || parsed > (tpbrt_longlong_t)TPBRT_INT_MAX || parsed < (tpbrt_longlong_t)TPBRT_INT_MIN) {
				return TPBRT_FALSE;
			}
		*out_val = (tpbrt_int_t)parsed;
		return end != buf;
	}

	tpbrt_bool_t tpbrt_read_uint_value(const tpbrt_string_t value_str, tpbrt_uint_t* const out_val) {
		static const tpbrt_size_t MAX_BUFFER_SIZE = 64u;
		static const tpbrt_int_t UINT_RANIX		  = 10;

			if (tpbrt_string_empty(value_str) == TPBRT_TRUE || value_str.size < 1u || out_val == TPBRT_NULL) {
				return TPBRT_FALSE;
			}

		tpbrt_char_t buf[MAX_BUFFER_SIZE];
			if (tpbrt_string_copy_to(value_str, buf, MAX_BUFFER_SIZE, TPBRT_NULL) != TPBRT_ERROR_NONE) { return TPBRT_FALSE; }

		const tpbrt_char_t* first = buf;
			while ((*first == ' ' || *first == '\t') && first != buf + MAX_BUFFER_SIZE) { ++first; }
			if (*first == '-' || first == buf + MAX_BUFFER_SIZE) { return TPBRT_FALSE; }

		tpbrt_char_t* end;
		const tpbrt_ulonglong_t parsed = strtoull(buf, &end, UINT_RANIX);
			if (end == buf || parsed > (tpbrt_ulonglong_t)TPBRT_UINT_MAX) { return TPBRT_FALSE; }
		*out_val = (tpbrt_uint_t)parsed;
		return TPBRT_TRUE;
	}

#pragma endregion

#pragma region STRING

	tpbrt_error_t tpbrt_copy_chars(tpbrt_char_t* const dst, const tpbrt_size_t dst_size, const tpbrt_char_t* const src,
	  const tpbrt_size_t n) {
			if (dst == TPBRT_NULL || dst_size == 0u || n >= dst_size || (src == TPBRT_NULL && n > 0u)) {
				return TPBRT_ERROR_INVALID_STRING;
			}
			if (n > 0u) { memcpy(dst, src, n); }
		dst[n] = '\0';
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_string_from_cstr(const tpbrt_char_t* cstr, tpbrt_string_t* const out_val) {
			if (cstr == TPBRT_NULL || out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*out_val = (tpbrt_string_t) { .data = cstr, .size = strlen(cstr) };
		return TPBRT_ERROR_NONE;
	}

	tpbrt_bool_t tpbrt_string_empty(const tpbrt_string_t str) {
		return str.data == NULL || str.size == 0 ? TPBRT_TRUE : TPBRT_FALSE;
	}

	tpbrt_error_t tpbrt_string_copy_unescaped(tpbrt_string_t* const dst, const tpbrt_string_t src) {
			if (dst == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(src)) {
				*dst = TPBRT_EMPTY_STRING;
				return TPBRT_ERROR_NONE;
			}

		tpbrt_char_t* const out = malloc(sizeof(tpbrt_char_t) * (src.size + 1u));
			if (out == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		tpbrt_size_t n = 0;
			for (tpbrt_size_t i = 0u; i < src.size; ++i) {
				tpbrt_char_t c = src.data[i];
					if (c == '\\' && i + 1u < src.size) {
						c = src.data[++i];
							switch (c) {
								case 'b': {
									c = '\b';
									break;
								}
								case 'f': {
									c = '\f';
									break;
								}
								case 'n': {
									c = '\n';
									break;
								}
								case 'r': {
									c = '\r';
									break;
								}
								case 't': {
									c = '\t';
									break;
								}
							case '\\':
							case '\'':
								case '"': {
									break;
								}
								default: {
									free(out);
									return TPBRT_ERROR_INVALID_STRING;
								}
							}
					}
				out[n++] = c;
			}
		out[n] = '\0';
		*dst   = (tpbrt_string_t) { .data = out, .size = n };
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_string_copy(tpbrt_string_t* const dst, const tpbrt_string_t src) {
			if (dst == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(src)) {
				*dst = TPBRT_EMPTY_STRING;
				return TPBRT_ERROR_NONE;
			}

		dst->data = malloc(sizeof(tpbrt_char_t) * (src.size + 1));
			if (dst->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		static const tpbrt_char_t NULL_TERMINATOR = '\0';
		memcpy((tpbrt_char_t*)dst->data, src.data, sizeof(tpbrt_char_t) * src.size);
		memcpy((tpbrt_char_t*)(dst->data + src.size), &NULL_TERMINATOR, sizeof(tpbrt_char_t));
		dst->size = src.size;

		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_string_copy_to(const tpbrt_string_t src, tpbrt_char_t* const dst, const tpbrt_size_t dst_size,
	  tpbrt_size_t* const out_size) {
			if (dst == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_size_t n	= TPBRT_MIN(src.size, dst_size - 1);
		const tpbrt_error_t err = tpbrt_copy_chars(dst, dst_size, src.data, n);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (out_size != TPBRT_NULL) { *out_size = n; }
		return TPBRT_ERROR_NONE;
	}

	tpbrt_bool_t tpbrt_string_equals_cstr(const tpbrt_string_t a, const tpbrt_char_t* const cstr) {
		tpbrt_string_t str;
			if (tpbrt_string_from_cstr(cstr, &str) != TPBRT_ERROR_NONE) { return TPBRT_FALSE; }
		return tpbrt_string_equals(a, str);
	}

	tpbrt_bool_t tpbrt_string_equals(const tpbrt_string_t a, const tpbrt_string_t b) {
			if (a.size != b.size) { return TPBRT_FALSE; }
		return strncmp(a.data, b.data, a.size) == 0 ? TPBRT_TRUE : TPBRT_FALSE;
	}

	void tpbrt_string_free(tpbrt_string_t* str) {
			if (str == TPBRT_NULL || str->data == TPBRT_NULL) { return; }

		free((tpbrt_char_t*)str->data);
		*str = TPBRT_EMPTY_STRING;
	}

#pragma endregion

#pragma region STRING_ARRAY

	void tpbrt_string_array_free(tpbrt_string_array_t* const array) {
			if (array == TPBRT_NULL || array->data == TPBRT_NULL) { return; }

			for (tpbrt_size_t i = 0; i < array->count; ++i) { tpbrt_string_free(&array->data[i]); }

		free(array->data);
		*array = TPBRT_EMPTY_STRING_ARRAY;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif