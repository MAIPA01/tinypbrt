#pragma once
#ifndef _TINYPBRT_PARSER_INTERNAL_H_
#define _TINYPBRT_PARSER_INTERNAL_H_

#include <tinypbrt/detail/tokenizer_internal.h>

#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		tpbrt_directive_t type;

		union {
			// Include, Import
			struct {
				tpbrt_string_t path;
			} include_import;

			// Option
			struct {
				tpbrt_param_t param;
			} option;

			// Film, Camera, Sampler, Integrator, Accelerator, LightSource, AreaLightSource,
			// Material, Shape
			struct {
				tpbrt_string_t type_name;
				tpbrt_params_list_t params;
			} generic_with_params;

			// ColorSpace, ActiveTransform
			struct {
				tpbrt_string_t type_name;
			} single_string;

			// CoordinateSystem, CoordSysTransform, PixelFilter, NamedMaterial,
			// ObjectBegin, ObjectInstance
			struct {
				tpbrt_string_t name;
			} named_entity;

			// Translate, Scale
			struct {
				tpbrt_float_t v[3];
			} vector3;

			// Rotate
			struct {
				tpbrt_float_t angle;
				tpbrt_float_t v[3];
			} rotate;

			// LookAt
			struct {
				tpbrt_float_t eye[3];
				tpbrt_float_t look_at[3];
				tpbrt_float_t up[3];
			} look_at;

			// Transform, ConcatTransform
			struct {
				tpbrt_float_t m[16];
			} transform;

			// TransformTimes
			struct {
				tpbrt_float_t start;
				tpbrt_float_t end;
			} transform_times;

			// Attribute
			struct {
				tpbrt_string_t target;
				tpbrt_params_list_t params;
			} attribute;

			// MakeNamedMaterial
			struct {
				tpbrt_string_t name;
				tpbrt_params_list_t params;
			} named_with_params;

			// Texture
			struct {
				tpbrt_string_t name;
				tpbrt_string_t type_name;
				tpbrt_string_t class_name;
				tpbrt_params_list_t params;
			} texture;

			// MakeNamedMedium
			struct {
				tpbrt_string_t name;
				tpbrt_string_t type;
				tpbrt_params_list_t params;
			} named_medium;

			// MediumInterface
			struct {
				tpbrt_string_t interior;
				tpbrt_string_t exterior;
			} medium_interface;
		} as;
	} tpbrt_element_t;

	typedef struct {
		tpbrt_tokenizer_t tokenizer;
	} tpbrt_parser_t;

	tpbrt_error_t tpbrt_parser_init(tpbrt_parser_t* parser, const tpbrt_string_t* str);

	tpbrt_error_t tpbrt_parser_parse_next(tpbrt_parser_t* parser, tpbrt_element_t* out_element);

	void tpbrt_free_element(tpbrt_element_t* element);

#ifdef __cplusplus
}
#endif

#endif // _TINYPBRT_PARSER_INTERNAL_H_
