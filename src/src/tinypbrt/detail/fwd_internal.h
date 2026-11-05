#pragma once
#ifndef _TINYPBRT_FWD_INTERNAL_H_
#define _TINYPBRT_FWD_INTERNAL_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup tinypbrt-private tinypbrt-internal
	 * @brief Library for parsing pbrt-v4 files. Internal structures.
	 * More info about pbrt-v4 file format you can find here: https://pbrt.org/fileformat-v4
	 */

#pragma region TOKENIZER
	/**
	 * @brief Tokenizer type.
	 * @ingroup tokenizer-private
	 */
	typedef struct tpbrt_tokenizer_t tpbrt_tokenizer_t;
#pragma endregion

#pragma region TOKEN
	/**
	 * @brief Token type.
	 * @ingroup token-private
	 */
	typedef struct tpbrt_token_t tpbrt_token_t;
#pragma endregion

#pragma region STATE
	/**
	 * @brief State type.
	 * @ingroup state-private
	 */
	typedef struct tpbrt_state_t tpbrt_state_t;
	/**
	 * @brief States stack type.
	 * @ingroup state-private
	 */
	typedef struct tpbrt_states_stack_t tpbrt_states_stack_t;
#pragma endregion

#pragma region PARSER
	/**
	 * @brief Parsed element type.
	 * @ingroup parser-private
	 */
	typedef struct tpbrt_element_t tpbrt_element_t;

	/**
	 * @brief Parser type.
	 * @ingroup parser-private
	 */
	typedef struct tpbrt_parser_t tpbrt_parser_t;
#pragma endregion

#pragma region PARAM
	/**
	 * @brief Parameter type.
	 * @ingroup param-private
	 */
	typedef struct tpbrt_param_t tpbrt_param_t;
	/**
	 * @brief List of parameters type.
	 * @ingroup param-private
	 */
	typedef struct tpbrt_params_list_t tpbrt_params_list_t;
#pragma endregion

#pragma region COORD
	/**
	 * @brief Coordinate systems map type.
	 * @ingroup coord-private
	 */
	typedef struct tpbrt_coord_system_map_t tpbrt_coord_system_map_t;
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_FWD_INTERNAL_H_
