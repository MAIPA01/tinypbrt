#pragma once
#ifndef _TINYPBRT_TOKEN_INTERNAL_H_
#define _TINYPBRT_TOKEN_INTERNAL_H_

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum tpbrt_directive_t : uint8_t {
		TPBRT_DIRECTIVE_IDENTITY			= 0,
		TPBRT_DIRECTIVE_TRANSLATE			= 1,
		TPBRT_DIRECTIVE_SCALE				= 2,
		TPBRT_DIRECTIVE_ROTATE				= 3,
		TPBRT_DIRECTIVE_LOOK_AT				= 4,
		TPBRT_DIRECTIVE_COORDINATE_SYSTEM	= 5,
		TPBRT_DIRECTIVE_COORD_SYS_TRANSFORM = 6,
		TPBRT_DIRECTIVE_TRANSFORM			= 7,
		TPBRT_DIRECTIVE_CONCAT_TRANSFORM	= 8,
		TPBRT_DIRECTIVE_TRANSFORM_TIMES		= 9,
		TPBRT_DIRECTIVE_ACTIVE_TRANSFORM	= 10,

		TPBRT_DIRECTIVE_INCLUDE				= 11,
		TPBRT_DIRECTIVE_IMPORT				= 12,

		TPBRT_DIRECTIVE_OPTION				= 13,

		TPBRT_DIRECTIVE_CAMERA				= 14,
		TPBRT_DIRECTIVE_SAMPLER				= 15,
		TPBRT_DIRECTIVE_COLOR_SPACE			= 16,
		TPBRT_DIRECTIVE_FILM				= 17,
		TPBRT_DIRECTIVE_PIXEL_FILTER		= 18,
		TPBRT_DIRECTIVE_INTEGRATOR			= 19,
		TPBRT_DIRECTIVE_ACCELERATOR			= 20,

		TPBRT_DIRECTIVE_WORLD_BEGIN			= 21,

		TPBRT_DIRECTIVE_ATTRIBUTE_BEGIN		= 22,
		TPBRT_DIRECTIVE_ATTRIBUTE_END		= 23,
		TPBRT_DIRECTIVE_ATTRIBUTE			= 24,

		TPBRT_DIRECTIVE_REVERSE_ORIENTATION = 25,
		TPBRT_DIRECTIVE_SHAPE				= 26,
		TPBRT_DIRECTIVE_OBJECT_BEGIN		= 27,
		TPBRT_DIRECTIVE_OBJECT_END			= 28,
		TPBRT_DIRECTIVE_OBJECT_INSTANCE		= 29,

		TPBRT_DIRECTIVE_LIGHT_SOURCE		= 30,
		TPBRT_DIRECTIVE_AREA_LIGHT_SOURCE	= 31,

		TPBRT_DIRECTIVE_MATERIAL			= 32,
		TPBRT_DIRECTIVE_MAKE_NAMED_MATERIAL = 33,
		TPBRT_DIRECTIVE_NAMED_MATERIAL		= 34,

		TPBRT_DIRECTIVE_TEXTURE				= 35,

		TPBRT_DIRECTIVE_MAKE_NAMED_MEDIUM	= 36,
		TPBRT_DIRECTIVE_MEDIUM_INTERFACE	= 37,

		TPBRT_DIRECTIVE_MAX_NUM				= 38,
	} tpbrt_directive_t;

	typedef enum tpbrt_token_type_t : uint8_t {
		TPBRT_TOKEN_TYPE_SINGLE		   = 0,
		TPBRT_TOKEN_TYPE_QUOTED_STRING = 1,
		TPBRT_TOKEN_TYPE_DIRECTIVE	   = 2,
	} tpbrt_token_type_t;

	typedef struct tpbrt_token_t {
		tpbrt_token_type_t type;
		tpbrt_string_t value;
		tpbrt_directive_t directive;
	} tpbrt_token_t;

	tpbrt_error_t tpbrt_create_token(const tpbrt_string_t* value, tpbrt_token_t* token);

	tpbrt_size_t tpbrt_token_size(const tpbrt_token_t* token);
	tpbrt_bool_t tpbrt_token_is_quoted_string(const tpbrt_token_t* token);
	tpbrt_bool_t tpbrt_token_is_directive(const tpbrt_token_t* token);
	tpbrt_bool_t tpbrt_token_is_open_brace(const tpbrt_token_t* token);
	tpbrt_bool_t tpbrt_token_is_close_brace(const tpbrt_token_t* token);
	tpbrt_bool_t tpbrt_token_is_valid(const tpbrt_token_t* token);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_TOKEN_INTERNAL_H_
