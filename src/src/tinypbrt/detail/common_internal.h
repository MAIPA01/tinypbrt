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

	/**
	 * @defgroup common-private Common
	 * @ingroup tinypbrt-private
	 */

/**
 * @brief Min macro.
 * @param a First value to check.
 * @param b Second value to check.
 * @ingroup common-private
 */
#define TPBRT_MIN(a, b) ((a) < (b) ? (a) : (b))

#pragma region PARSING
	/**
	 * @brief Go to the next char which is not any whitespace character.
	 * @param current Pointer to current character.
	 * @param end Pointer to end character (points outside of string).
	 * @return String structure from first not whitespace character to last after which is end or whitespace character.
	 * @ingroup common-private
	 */
	tpbrt_string_t tpbrt_next_value(const tpbrt_char_t* current, const tpbrt_char_t* end);
	/**
	 * @brief Counts words in given string. (word -> starts with not whitespace character and ends with last not whitespace character).
	 * @param values String with values separated by whitespace characters.
	 * @return Number of values.
	 * @ingroup common-private
	 */
	tpbrt_size_t tpbrt_count_values(tpbrt_string_t values);
	/**
	 * @brief Reads string value as a string convertible value. (example "\"str value\"").
	 * @param value_str String value.
	 * @param out_val Output value of string type.
	 * @return TPBRT_TRUE if string value was convertible to value of string type else TPBRT_FALSE.
	 * @ingroup common-private
	 */
	tpbrt_bool_t tpbrt_read_string_value(tpbrt_string_t value_str, tpbrt_string_t* out_val);
	/**
	 * @brief Reads string value as a bool convertible value. (example "true").
	 * @param value_str String value.
	 * @param out_val Output value of bool type.
	 * @return TPBRT_TRUE if string value was convertible to value of bool type else TPBRT_FALSE.
	 * @ingroup common-private
	 */
	tpbrt_bool_t tpbrt_read_bool_value(tpbrt_string_t value_str, tpbrt_bool_t* out_val);
	/**
	 * @brief Reads string value as a float convertible value. (example "0.0").
	 * @param value_str String value.
	 * @param out_val Output value of float type.
	 * @return TPBRT_TRUE if string value was convertible to value of float type else TPBRT_FALSE.
	 * @ingroup common-private
	 */
	tpbrt_bool_t tpbrt_read_float_value(tpbrt_string_t value_str, tpbrt_float_t* out_val);
	/**
	 * @brief Reads string value as a int convertible value. (example "-1").
	 * @param value_str String value.
	 * @param out_val Output value of int type.
	 * @return TPBRT_TRUE if string value was convertible to value of int type else TPBRT_FALSE.
	 * @ingroup common-private
	 */
	tpbrt_bool_t tpbrt_read_int_value(tpbrt_string_t value_str, tpbrt_int_t* out_val);
	/**
	 * @brief Reads string value as a uint convertible value. (example "1").
	 * @param value_str String value.
	 * @param out_val Output value of uint type.
	 * @return TPBRT_TRUE if string value was convertible to value of uint type else TPBRT_FALSE.
	 * @ingroup common-private
	 */
	tpbrt_bool_t tpbrt_read_uint_value(tpbrt_string_t value_str, tpbrt_uint_t* out_val);
#pragma endregion

#pragma region STRING
	/**
	 * @brief Initializes static string value.
	 * @param value Static string value.
	 * @ingroup common-private
	 */
#define TPBRT_STRING_INIT(value) { .data = (value), .size = sizeof((value)) - 1u }
	/**
	 * @brief Initializes static string value with type conversion.
	 * @param value Static string value.
	 * @ingroup common-private
	 */
#define TPBRT_STRING(value) (tpbrt_string_t) TPBRT_STRING_INIT(value)
	/**
	 * @brief Initializes empty string value.
	 * @ingroup common-private
	 */
#define TPBRT_EMPTY_STRING_INIT { .data = TPBRT_NULL, .size = 0u }
	/**
	 * @brief Initializes empty string value with type conversion.
	 * @ingroup common-private
	 */
#define TPBRT_EMPTY_STRING (tpbrt_string_t) TPBRT_EMPTY_STRING_INIT

	/**
	 * @brief Copies chars from one buffer to another.
	 * @param dst Destination buffer.
	 * @param dst_size Destination buffer size.
	 * @param src Source buffer.
	 * @param n Number of characters to copy.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_STRING, TPBRT_ERROR_NONE).
	 * @ingroup common-private
	 */
	tpbrt_error_t tpbrt_copy_chars(tpbrt_char_t* dst, tpbrt_size_t dst_size, const tpbrt_char_t* src, tpbrt_size_t n);

	/**
	 * @brief Initializes string from characters buffer.
	 * @param cstr Characters buffer.
	 * @param out_val Output string data structure.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup common-private
	 */
	tpbrt_error_t tpbrt_string_from_cstr(const tpbrt_char_t* cstr, tpbrt_string_t* out_val);

	/**
	 * @brief Checks if string is empty.
	 * @param str String to check.
	 * @return TPBRT_TRUE if string is empty else TPBRT_FALSE.
	 * @ingroup common-private
	 */
	static inline tpbrt_bool_t tpbrt_string_empty(const tpbrt_string_t str) {
		return str.data == TPBRT_NULL || str.size == 0u ? TPBRT_TRUE : TPBRT_FALSE;
	}

	/**
	 * @brief Copies string and converts "\\\\" to "\\" and "\\n" to "\n" etc.
	 * @param dst Destination string.
	 * @param src Source string.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_INVALID_STRING, TPBRT_ERROR_NONE).
	 * @ingroup common-private
	 */
	tpbrt_error_t tpbrt_string_copy_unescaped(tpbrt_string_t* dst, tpbrt_string_t src);
	/**
	 * @brief Copies string to another string.
	 * @param dst Destination string.
	 * @param src Source string.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup common-private
	 */
	tpbrt_error_t tpbrt_string_copy(tpbrt_string_t* dst, tpbrt_string_t src);
	/**
	 * @brief Copies string to char buffer.
	 * @param src Source string.
	 * @param dst Destination char buffer.
	 * @param dst_size Max size of destination buffer.
	 * @param out_size Output result size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_STRING, TPBRT_ERROR_NONE).
	 * @ingroup common-private
	 */
	tpbrt_error_t tpbrt_string_copy_to(tpbrt_string_t src, tpbrt_char_t* dst, tpbrt_size_t dst_size, tpbrt_size_t* out_size);

	/**
	 * @brief Checks if string is equal to char buffer.
	 * @param a String to check.
	 * @param cstr Char buffer.
	 * @return TPBRT_TRUE if is equal else TPBRT_FALSE.
	 * @ingroup common-private
	 */
	tpbrt_bool_t tpbrt_string_equals_cstr(tpbrt_string_t a, const tpbrt_char_t* cstr);

	/**
	 * @brief Checks if two strings are equal.
	 * @param a First string to check.
	 * @param b Second string to check.
	 * @return TPBRT_TRUE if is equal else TPBRT_FALSE.
	 * @ingroup common-private
	 */
	static inline tpbrt_bool_t tpbrt_string_equals(const tpbrt_string_t a, const tpbrt_string_t b) {
			if (a.size != b.size) { return TPBRT_FALSE; }
			if (a.size == 0u) { return TPBRT_TRUE; }
			if (a.data == TPBRT_NULL || b.data == TPBRT_NULL) { return a.data == b.data ? TPBRT_TRUE : TPBRT_FALSE; }
			if (a.data[0] != b.data[0]) { return TPBRT_FALSE; }
		return memcmp(a.data, b.data, a.size) == 0 ? TPBRT_TRUE : TPBRT_FALSE;
	}

	/**
	 * @brief Frees string data.
	 * @param str String to free.
	 * @ingroup common-private
	 */
	void tpbrt_string_free(tpbrt_string_t* str);
#pragma endregion

#pragma region STRING_ARRAY
	/**
	 * @brief Initializes empty strings array.
	 * @ingroup common-private
	 */
#define TPBRT_EMPTY_STRING_ARRAY_INIT { .data = TPBRT_NULL, .count = 0 }
	/**
	 * @brief Initializes empty strings array with type conversion.
	 * @ingroup common-private
	 */
#define TPBRT_EMPTY_STRING_ARRAY (tpbrt_string_array_t) TPBRT_EMPTY_STRING_ARRAY_INIT

	/**
	 * @brief Frees string array data.
	 * @param array String array to free.
	 * @ingroup common-private
	 */
	void tpbrt_string_array_free(tpbrt_string_array_t* array);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_COMMON_INTERNAL_H_
