#pragma once
#ifndef _TINYPBRT_FWD_INTERNAL_H_
#define _TINYPBRT_FWD_INTERNAL_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region TOKENIZER
	typedef struct tpbrt_tokenizer_t tpbrt_tokenizer_t;
#pragma endregion

#pragma region TOKEN
	typedef struct tpbrt_token_t tpbrt_token_t;
#pragma endregion

#pragma region STATE
	typedef struct tpbrt_state_t tpbrt_state_t;
	typedef struct tpbrt_states_stack_t tpbrt_states_stack_t;
#pragma endregion

#pragma region PARSER
	typedef struct tpbrt_element_t tpbrt_element_t;

	typedef struct tpbrt_parser_t tpbrt_parser_t;
#pragma endregion

#pragma region PARAM
	typedef struct tpbrt_param_t tpbrt_param_t;
	typedef struct tpbrt_params_list_t tpbrt_params_list_t;
#pragma endregion

#pragma region COORD
	typedef struct tpbrt_coord_system_map_t tpbrt_coord_system_map_t;
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_FWD_INTERNAL_H_
