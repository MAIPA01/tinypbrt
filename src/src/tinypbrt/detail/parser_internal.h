#pragma once
#ifndef _TINYPBRT_PARSER_INTERNAL_H_
#define _TINYPBRT_PARSER_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/error.h>

#include <tinypbrt/detail/param_internal.h>
#include <tinypbrt/detail/token_internal.h>
#include <tinypbrt/detail/tokenizer_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup parser-private Parser
	 * @ingroup tinypbrt-private
	 */

	/**
	 * @brief Parsed element data.
	 * @ingroup parser-private
	 */
	typedef struct tpbrt_element_t {
		/// @brief Value dependent on directive.
		union {
			/// @brief Used for Include and Import directive.
			struct {
				/// @brief Path to file.
				tpbrt_string_t path;
			} include_import;

			/// @brief Used for Option directive.
			struct {
				/// @brief Parameters.
				tpbrt_param_t param;
			} option;

			/// @brief Used for Film, Camera, Sampler, Integrator, Accelerator, LightSource, AreaLightSource, Material and Shape directives.
			struct {
				/// @brief Type name of current object.
				tpbrt_string_t type_name;
				/// @brief Parameters.
				tpbrt_params_list_t params;
			} generic_with_params;

			/// @brief Used for ColorSpace and ActiveTransform directives.
			struct {
				/// @brief Type name of current object.
				tpbrt_string_t type_name;
			} single_string;

			/// @brief Used for CoordinateSystem, CoordSysTransform, PixelFilter, NamedMaterial, ObjectBegin and ObjectInstance directives.
			struct {
				/// @brief Name.
				tpbrt_string_t name;
			} named_entity;

			/// @brief Used for Translate and Scale directives.
			struct {
				/// @brief 3 component vector.
				tpbrt_vec3_t v;
			} vector3;

			/// @brief Used for Rotate directive.
			struct {
				/// @brief Rotation angle.
				tpbrt_float_t angle;
				/// @brief Rotation axis.
				tpbrt_vec3_t v;
			} rotate;

			/// @brief Used for LookAt directive.
			struct {
				/// @brief Eye position.
				tpbrt_vec3_t eye;
				/// @brief Look at object position.
				tpbrt_vec3_t look_at;
				/// @brief Up vector.
				tpbrt_vec3_t up;
			} look_at;

			/// @brief Used for Transform and ConcatTransform directives.
			struct {
				/// @brief 4 by 4 matrix.
				tpbrt_mat4_t m;
			} transform;

			/// @brief Used for TransformTimes directive.
			struct {
				/// @brief Start time.
				tpbrt_float_t start;
				/// @brief End time.
				tpbrt_float_t end;
			} transform_times;

			/// @brief Used for Attribute directive.
			struct {
				/// @brief Target object type.
				tpbrt_string_t target;
				/// @brief Parameters.
				tpbrt_params_list_t params;
			} attribute;

			/// @brief Used for MakeNamedMaterial directive.
			struct {
				/// @brief Material name.
				tpbrt_string_t name;
				/// @brief Parameters.
				tpbrt_params_list_t params;
			} named_with_params;

			/// @brief Used for Texture directive.
			struct {
				/// @brief Texture name.
				tpbrt_string_t name;
				/// @brief Texture type name.
				tpbrt_string_t type_name;
				/// @brief Texture class name.
				tpbrt_string_t class_name;
				/// @brief Parameters.
				tpbrt_params_list_t params;
			} texture;

			/// @brief Used for MakeNamedMedium directive.
			struct {
				/// @brief Medium name.
				tpbrt_string_t name;
				/// @brief Medium type.
				tpbrt_string_t type;
				/// @brief Parameters.
				tpbrt_params_list_t params;
			} named_medium;

			/// @brief Used for MediumInterface directive.
			struct {
				/// @brief Interior medium name.
				tpbrt_string_t interior;
				/// @brief Exterior medium name.
				tpbrt_string_t exterior;
			} medium_interface;
		} as;

		/// @brief Directive value.
		tpbrt_directive_t type : 7;
	} tpbrt_element_t;

	/**
	 * @brief Parser data.
	 * @ingroup parser-private
	 */
	typedef struct tpbrt_parser_t {
		/// @brief Tokenizer data.
		tpbrt_tokenizer_t tokenizer;
	} tpbrt_parser_t;

	/**
	 * @brief Frees element data.
	 * @param element Element data to free.
	 * @ingroup parser-private
	 */
	void tpbrt_element_free(tpbrt_element_t* element);

	/**
	 * @brief Initializes parser.
	 * @param parser Parser data.
	 * @param str String to parse.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE)
	 * @ingroup parser-private
	 */
	tpbrt_error_t tpbrt_parser_init(tpbrt_parser_t* parser, tpbrt_string_t str);

	/**
	 * @brief Returns next parsed element.
	 * @param parser Parser data.
	 * @param out_element Output element data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_DIRECTIVE, TPBRT_ERROR_NONE).
	 * @ingroup parser-private
	 */
	tpbrt_error_t tpbrt_parser_next(tpbrt_parser_t* parser, tpbrt_element_t* out_element);

#ifdef __cplusplus
}
#endif

#endif // _TINYPBRT_PARSER_INTERNAL_H_
