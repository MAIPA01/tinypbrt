#pragma once
#ifndef _TINYPBRT_TOKEN_INTERNAL_H_
#define _TINYPBRT_TOKEN_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/error.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup token-private Token
	 * @ingroup tinypbrt-private
	 */

	/**
	 * @brief Directive values.
	 * @ingroup token-private
	 */
	typedef enum tpbrt_directive_t {
		/// @brief Identity directive.
		TPBRT_DIRECTIVE_IDENTITY			= 0,
		/// @brief Translate directive.
		TPBRT_DIRECTIVE_TRANSLATE			= 1,
		/// @brief Scale directive.
		TPBRT_DIRECTIVE_SCALE				= 2,
		/// @brief Rotate directive.
		TPBRT_DIRECTIVE_ROTATE				= 3,
		/// @brief LookAt directive.
		TPBRT_DIRECTIVE_LOOK_AT				= 4,
		/// @brief CoordinateSystem directive.
		TPBRT_DIRECTIVE_COORDINATE_SYSTEM	= 5,
		/// @brief CoordianteSystemTransform directive.
		TPBRT_DIRECTIVE_COORD_SYS_TRANSFORM = 6,
		/// @brief Transform directive.
		TPBRT_DIRECTIVE_TRANSFORM			= 7,
		/// @brief ConcatTransform directive.
		TPBRT_DIRECTIVE_CONCAT_TRANSFORM	= 8,
		/// @brief TransformTimes directive.
		TPBRT_DIRECTIVE_TRANSFORM_TIMES		= 9,
		/// @brief ActiveTransform directive.
		TPBRT_DIRECTIVE_ACTIVE_TRANSFORM	= 10,

		/// @brief Include directive.
		TPBRT_DIRECTIVE_INCLUDE				= 11,
		/// @brief Import directive.
		TPBRT_DIRECTIVE_IMPORT				= 12,

		/// @brief Option directive.
		TPBRT_DIRECTIVE_OPTION				= 13,

		/// @brief Camera directive.
		TPBRT_DIRECTIVE_CAMERA				= 14,
		/// @brief Sampler directive.
		TPBRT_DIRECTIVE_SAMPLER				= 15,
		/// @brief ColorSpace directive.
		TPBRT_DIRECTIVE_COLOR_SPACE			= 16,
		/// @brief Film directive.
		TPBRT_DIRECTIVE_FILM				= 17,
		/// @brief PixelFilter directive.
		TPBRT_DIRECTIVE_PIXEL_FILTER		= 18,
		/// @brief Integrator directive.
		TPBRT_DIRECTIVE_INTEGRATOR			= 19,
		/// @brief Accelerator directive.
		TPBRT_DIRECTIVE_ACCELERATOR			= 20,

		/// @brief WorldBegin directive.
		TPBRT_DIRECTIVE_WORLD_BEGIN			= 21,

		/// @brief AttributeBegin directive.
		TPBRT_DIRECTIVE_ATTRIBUTE_BEGIN		= 22,
		/// @brief AttributeEnd directive.
		TPBRT_DIRECTIVE_ATTRIBUTE_END		= 23,
		/// @brief Attribute directive.
		TPBRT_DIRECTIVE_ATTRIBUTE			= 24,

		/// @brief ReverseOrientation directive.
		TPBRT_DIRECTIVE_REVERSE_ORIENTATION = 25,
		/// @brief Shape directive.
		TPBRT_DIRECTIVE_SHAPE				= 26,
		/// @brief ObjectBegin directive.
		TPBRT_DIRECTIVE_OBJECT_BEGIN		= 27,
		/// @brief ObjectEnd directive.
		TPBRT_DIRECTIVE_OBJECT_END			= 28,
		/// @brief ObjectInstance directive.
		TPBRT_DIRECTIVE_OBJECT_INSTANCE		= 29,

		/// @brief LightSource directive.
		TPBRT_DIRECTIVE_LIGHT_SOURCE		= 30,
		/// @brief AreaLight directive.
		TPBRT_DIRECTIVE_AREA_LIGHT_SOURCE	= 31,

		/// @brief Material directive.
		TPBRT_DIRECTIVE_MATERIAL			= 32,
		/// @brief MakeNamedMaterial directive.
		TPBRT_DIRECTIVE_MAKE_NAMED_MATERIAL = 33,
		/// @brief NamedMaterial directive.
		TPBRT_DIRECTIVE_NAMED_MATERIAL		= 34,

		/// @brief Texture directive.
		TPBRT_DIRECTIVE_TEXTURE				= 35,

		/// @brief MakeNamedMedium directive.
		TPBRT_DIRECTIVE_MAKE_NAMED_MEDIUM	= 36,
		/// @brief MediumInterface directive.
		TPBRT_DIRECTIVE_MEDIUM_INTERFACE	= 37,

		/// @brief TransformBegin directive. Removed but used as AttributeBegin for back compatibility
		TPBRT_DIRECTIVE_TRANSFORM_BEGIN		= 38,
		/// @brief TransformEnd directive. Removed but used as AttributeEnd for back compatibility.
		TPBRT_DIRECTIVE_TRANSFORM_END		= 39,

		/// @brief Count of available directives.
		TPBRT_DIRECTIVE_MAX_NUM				= 40,
	} tpbrt_directive_t;

	/**
	 * @brief Token types.
	 * @ingroup token-private
	 */
	typedef enum tpbrt_token_type_t {
		/// @brief Single token (example "value")
		TPBRT_TOKEN_TYPE_SINGLE		   = 0,
		/// @brief Quoted string token (example "\"value\"")
		TPBRT_TOKEN_TYPE_QUOTED_STRING = 1,
		/// @brief Directive token (Single token but value is in directives list)
		TPBRT_TOKEN_TYPE_DIRECTIVE	   = 2,
		/// @brief Count of available token types.
		TPBRT_TOKEN_TYPE_MAX_NUM	   = 3,
	} tpbrt_token_type_t;

	/**
	 * @brief Token data.
	 * @ingroup token-private
	 */
	typedef struct tpbrt_token_t {
		/// @brief Value depends on token type.
		union {
			/// @brief Token value.
			tpbrt_string_t value;
			/// @brief Token directive type.
			tpbrt_directive_t directive : 7;
		};

		/// @brief Token type.
		tpbrt_token_type_t type : 3;
	} tpbrt_token_t;

	/**
	 * @brief Returns directive based on string value.
	 * @param directive_str String which is convertable to directive.
	 * @param directive_out Output directive value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_DIRECTIVE, TPBRT_ERROR_NONE).
	 * @ingroup token-private
	 */
	tpbrt_error_t tpbrt_directive_from_string(tpbrt_string_t directive_str, tpbrt_directive_t* directive_out);

	/**
	 * @brief Returns directive string size.
	 * @param directive Directive.
	 * @return Size of directive string.
	 * @ingroup token-private.
	 */
	tpbrt_size_t tpbrt_directive_size(tpbrt_directive_t directive);

	/**
	 * @brief Initializes token.
	 * @param value Token value.
	 * @param token Output token data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_DIRECTIVE, TPBRT_ERROR_NONE).
	 * @ingroup token-private
	 */
	tpbrt_error_t tpbrt_token_init(tpbrt_string_t value, tpbrt_token_t* token);

	/**
	 * @brief Returns token size.
	 * @param token Token data.
	 * @return Token value size.
	 * @ingroup token-private
	 */
	tpbrt_size_t tpbrt_token_size(const tpbrt_token_t* token);
	/**
	 * @brief Checks if token is a quoted string.
	 * @param token Token data.
	 * @return TPBRT_TRUE if token is quoted string else TPBRT_FALSE.
	 * @ingroup token-private
	 */
	tpbrt_bool_t tpbrt_token_is_quoted_string(const tpbrt_token_t* token);
	/**
	 * @brief Checks if token is directive.
	 * @param token Token data.
	 * @return TPBRT_TRUE if token is directive else TPBRT_FALSE.
	 * @ingroup token-private
	 */
	tpbrt_bool_t tpbrt_token_is_directive(const tpbrt_token_t* token);
	/**
	 * @brief Checks if token is single.
	 * @param token Token data.
	 * @return TPBRT_TRUE if token is single else TPBRT_FALSE.
	 * @ingroup token-private
	 */
	tpbrt_bool_t tpbrt_token_is_single(const tpbrt_token_t* token);
	/**
	 * @brief Checks if token is open brace.
	 * @param token Token data.
	 * @return TPBRT_TRUE if token is open brace else TPBRT_FALSE.
	 * @ingroup token-private
	 */
	tpbrt_bool_t tpbrt_token_is_open_brace(const tpbrt_token_t* token);
	/**
	 * @brief Checks if token is close brace.
	 * @param token Token data.
	 * @return TPBRT_TRUE if token is close brace else TPBRT_FALSE.
	 * @ingroup token-private
	 */
	tpbrt_bool_t tpbrt_token_is_close_brace(const tpbrt_token_t* token);
	/**
	 * @brief Checks if token is valid.
	 * @param token Token data.
	 * @return TPBRT_TRUE if token is valid else TPBRT_FALSE.
	 * @ingroup token-private
	 */
	tpbrt_bool_t tpbrt_token_is_valid(const tpbrt_token_t* token);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_TOKEN_INTERNAL_H_
