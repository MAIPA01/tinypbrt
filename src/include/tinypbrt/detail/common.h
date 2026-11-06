#pragma once
#ifndef _TINYPBRT_COMMON_H_
#define _TINYPBRT_COMMON_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup common-public Common
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Simple string representation (not always ends with '\0', size determines final length of string)
	 * @ingroup common-public
	 */
	typedef struct tpbrt_string_t {
		/// @brief String characters.
		const tpbrt_char_t* data;
		/// @brief String size.
		tpbrt_size_t size;
	} tpbrt_string_t;

	/**
	 * @brief Array of strings
	 * @ingroup common-public
	 */
	typedef struct tpbrt_string_array_t {
		/// @brief Array data.
		tpbrt_string_t* data;
		/// @brief Array items count.
		tpbrt_size_t count;
	} tpbrt_string_array_t;

	/**
	 * @brief Keeps name indexes for faster lookups.
	 * @ingroup common-public
	 */
	typedef struct tpbrt_name_index_t {
		/// @brief Indexes.
		tpbrt_idx_t* slots;
		/// @brief Slots capacity.
		tpbrt_uint_t capacity;
		/// @brief Slots count.
		tpbrt_uint_t count;
	} tpbrt_name_index_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_TYPES_H_
