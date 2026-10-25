#include <tinypbrt/pch.h>

#include <tinypbrt/detail/common_internal.h>

#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region PARSING

	static tpbrt_bool_t tpbrt_is_space(const tpbrt_char_t c) {
		return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f' ? TPBRT_TRUE : TPBRT_FALSE;
	}

	tpbrt_size_t tpbrt_count_values(const tpbrt_string_t values) {
		tpbrt_size_t count			= 0u;
		const tpbrt_char_t* p		= values.data;
		const tpbrt_char_t* const e = values.data + values.size;
			if (p == TPBRT_NULL) { return 0u; }

			while (p < e) {
					while (p < e && tpbrt_is_space(*p) == TPBRT_TRUE) { ++p; }
					if (p >= e) { break; }
				++count;
					if (*p == '"') {
						++p;
							while (p < e && *p != '"') { ++p; }
							if (p < e) { ++p; }
					}
					else {
							while (p < e && tpbrt_is_space(*p) == TPBRT_FALSE) { ++p; }
					}
			}
		return count;
	}

	tpbrt_string_t tpbrt_next_value(const tpbrt_char_t* current, const tpbrt_char_t* const end) {
		tpbrt_string_t value = TPBRT_EMPTY_STRING;

			if (current == TPBRT_NULL || end == TPBRT_NULL) { return value; }

			while (current < end && tpbrt_is_space(*current) == TPBRT_TRUE) { ++current; }
			if (current >= end) { return value; }

		const tpbrt_char_t* start	 = current;
		const tpbrt_bool_t is_string = *start == '\"' ? TPBRT_TRUE : TPBRT_FALSE;
			if (is_string && current < end) { ++current; }
			while (current < end &&
				   ((!is_string && tpbrt_is_space(*current) == TPBRT_FALSE) || (is_string && *current != '\"'))) {
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
		static const tpbrt_string_t TRUE_STR  = TPBRT_STRING_INIT("true");
		static const tpbrt_string_t FALSE_STR = TPBRT_STRING_INIT("false");

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

	static tpbrt_bool_t tpbrt_is_digit(const tpbrt_char_t c) { return c >= '0' && c <= '9' ? TPBRT_TRUE : TPBRT_FALSE; }

	enum { TPBRT_DECIMAL_BASE = 10 };

	typedef struct tpbrt_decimal_t {
		uint64_t mantissa;
		int digits;
		int exponent;
		tpbrt_bool_t any_digit;
	} tpbrt_decimal_t;

	static void tpbrt_decimal_push_digit(tpbrt_decimal_t* const decimal, const tpbrt_char_t c, const tpbrt_bool_t fraction) {
		enum { MAX_DIGITS = 19 }; // 10^19 < 2^64

		const unsigned d   = (unsigned)(c - '0');
		decimal->any_digit = TPBRT_TRUE;
			if (decimal->mantissa == 0u && d == 0u) {
					if (fraction == TPBRT_TRUE) { --decimal->exponent; }
				return;
			}
			if (decimal->digits < MAX_DIGITS) {
				decimal->mantissa = (decimal->mantissa * TPBRT_DECIMAL_BASE) + d;
				++decimal->digits;
					if (fraction == TPBRT_TRUE) { --decimal->exponent; }
			}
			else if (fraction == TPBRT_FALSE) { ++decimal->exponent; }
	}

	static tpbrt_bool_t tpbrt_decimal_read_exponent(const tpbrt_char_t** const cursor, const tpbrt_char_t* const end,
	  int* const exponent) {
		// clang-format off
		enum { MAX_EXPONENT = 100000 };

		// clang-format on

		const tpbrt_char_t* p = *cursor;
			if (p >= end || (*p != 'e' && *p != 'E')) { return TPBRT_TRUE; }
		++p;

		const tpbrt_bool_t negative = p < end && *p == '-' ? TPBRT_TRUE : TPBRT_FALSE;
			if (p < end && (*p == '-' || *p == '+')) { ++p; }
			if (p >= end || tpbrt_is_digit(*p) == TPBRT_FALSE) { return TPBRT_FALSE; }

		int value = 0;
			for (; p < end && tpbrt_is_digit(*p) == TPBRT_TRUE; ++p) {
					if (value < MAX_EXPONENT) { value = (value * TPBRT_DECIMAL_BASE) + (*p - '0'); }
			}
		*exponent += negative == TPBRT_TRUE ? -value : value;
		*cursor = p;
		return TPBRT_TRUE;
	}

	static tpbrt_double_t tpbrt_decimal_to_double(const tpbrt_decimal_t* const decimal) {
		// 10^0 .. 10^22 są dokładnie reprezentowalne w double (szybka ścieżka Clingera)
		static const tpbrt_double_t POW10[] = { 1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10, 1e11, 1e12, 1e13, 1e14,
			1e15, 1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22 };

		enum { MAX_FAST_EXPONENT = 22, MAX_EXACT_DIGITS = 15 };

			if (decimal->mantissa == 0u) { return 0.0; }
			if (decimal->digits <= MAX_EXACT_DIGITS && decimal->exponent >= -MAX_FAST_EXPONENT &&
				decimal->exponent <= MAX_FAST_EXPONENT) {
				return decimal->exponent < 0 ? (tpbrt_double_t)decimal->mantissa / POW10[-decimal->exponent] :
											   (tpbrt_double_t)decimal->mantissa * POW10[decimal->exponent];
			}
		return (tpbrt_double_t)decimal->mantissa * pow((tpbrt_double_t)TPBRT_DECIMAL_BASE, (tpbrt_double_t)decimal->exponent);
	}

	tpbrt_bool_t tpbrt_read_float_value(const tpbrt_string_t value_str, tpbrt_float_t* const out_val) {
			if (tpbrt_string_empty(value_str) == TPBRT_TRUE || out_val == TPBRT_NULL) { return TPBRT_FALSE; }

		const tpbrt_char_t* p		= value_str.data;
		const tpbrt_char_t* const e = value_str.data + value_str.size;

		const tpbrt_bool_t negative = *p == '-' ? TPBRT_TRUE : TPBRT_FALSE;
			if (*p == '-' || *p == '+') { ++p; }

		tpbrt_decimal_t decimal = { .mantissa = 0u, .digits = 0, .exponent = 0, .any_digit = TPBRT_FALSE };
			for (; p < e && tpbrt_is_digit(*p) == TPBRT_TRUE; ++p) { tpbrt_decimal_push_digit(&decimal, *p, TPBRT_FALSE); }
			if (p < e && *p == '.') {
					for (++p; p < e && tpbrt_is_digit(*p) == TPBRT_TRUE; ++p) {
						tpbrt_decimal_push_digit(&decimal, *p, TPBRT_TRUE);
					}
			}

			if (decimal.any_digit == TPBRT_FALSE || tpbrt_decimal_read_exponent(&p, e, &decimal.exponent) == TPBRT_FALSE ||
				p != e) {
				return TPBRT_FALSE;
			}

		const tpbrt_double_t value = tpbrt_decimal_to_double(&decimal);
			if (value > (tpbrt_double_t)TPBRT_FLT_MAX) { return TPBRT_FALSE; }

		*out_val = (tpbrt_float_t)(negative == TPBRT_TRUE ? -value : value);
		return TPBRT_TRUE;
	}

	static tpbrt_bool_t tpbrt_read_digits(const tpbrt_char_t* p, const tpbrt_char_t* const e, const uint64_t max_value,
	  uint64_t* const out_val) {
			if (p >= e) { return TPBRT_FALSE; }

		uint64_t value = 0u;
			for (; p < e; ++p) {
					if (tpbrt_is_digit(*p) == TPBRT_FALSE) { return TPBRT_FALSE; }
				value = (value * TPBRT_DECIMAL_BASE) + (uint64_t)(*p - '0');
					if (value > max_value) { return TPBRT_FALSE; }
			}
		*out_val = value;
		return TPBRT_TRUE;
	}

	tpbrt_bool_t tpbrt_read_int_value(const tpbrt_string_t value_str, tpbrt_int_t* const out_val) {
			if (tpbrt_string_empty(value_str) == TPBRT_TRUE || out_val == TPBRT_NULL) { return TPBRT_FALSE; }

		const tpbrt_char_t* p		= value_str.data;
		const tpbrt_char_t* const e = value_str.data + value_str.size;
		const tpbrt_bool_t negative = *p == '-' ? TPBRT_TRUE : TPBRT_FALSE;
			if (*p == '-' || *p == '+') { ++p; }

		uint64_t magnitude = 0u;
		const uint64_t max = negative == TPBRT_TRUE ? (uint64_t)TPBRT_INT_MAX + 1u : (uint64_t)TPBRT_INT_MAX;
			if (tpbrt_read_digits(p, e, max, &magnitude) == TPBRT_FALSE) { return TPBRT_FALSE; }

		*out_val = negative == TPBRT_TRUE ? (tpbrt_int_t)(-(int64_t)magnitude) : (tpbrt_int_t)magnitude;
		return TPBRT_TRUE;
	}

	tpbrt_bool_t tpbrt_read_uint_value(const tpbrt_string_t value_str, tpbrt_uint_t* const out_val) {
			if (tpbrt_string_empty(value_str) == TPBRT_TRUE || out_val == TPBRT_NULL) { return TPBRT_FALSE; }

		const tpbrt_char_t* p = value_str.data;
			if (*p == '+') { ++p; }

		uint64_t value = 0u;
			if (tpbrt_read_digits(p, value_str.data + value_str.size, (uint64_t)TPBRT_UINT_MAX, &value) == TPBRT_FALSE) {
				return TPBRT_FALSE;
			}
		*out_val = (tpbrt_uint_t)value;
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
