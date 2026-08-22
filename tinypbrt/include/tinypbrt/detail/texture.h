#pragma once
#ifndef _TINYPBRT_TEXTURE_H_
#define _TINYPBRT_TEXTURE_H_

#include <tinypbrt/detail/color.h>
#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/fwd.h>
#include <tinypbrt/detail/math.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_TEXTURE_TYPE_FLOAT	= 0,
		TPBRT_TEXTURE_TYPE_SPECTRUM = 1,
		TPBRT_TEXTURE_TYPE_MAX_NUM	= 2,
	} tpbrt_texture_type_t;

	typedef enum : uint8_t {
		TPBRT_TEXTURE_CLASS_BILERP		  = 0,
		TPBRT_TEXTURE_CLASS_CHECKER_BOARD = 1,
		TPBRT_TEXTURE_CLASS_CONSTANT	  = 2,
		TPBRT_TEXTURE_CLASS_DIRECTION_MIX = 3,
		TPBRT_TEXTURE_CLASS_DOTS		  = 4,
		TPBRT_TEXTURE_CLASS_FBM			  = 5,
		TPBRT_TEXTURE_CLASS_IMAGE_MAP	  = 6,
		TPBRT_TEXTURE_CLASS_MARBLE		  = 7,
		TPBRT_TEXTURE_CLASS_MIX			  = 8,
		TPBRT_TEXTURE_CLASS_PTEX		  = 9,
		TPBRT_TEXTURE_CLASS_SCALE		  = 10,
		TPBRT_TEXTURE_CLASS_WINDY		  = 11,
		TPBRT_TEXTURE_CLASS_WRINKLED	  = 12,
		TPBRT_TEXTURE_CLASS_MAX_NUM		  = 13,
	} tpbrt_texture_class_t;

	typedef enum : uint8_t {
		TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE	 = 0,
		TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT	 = 1,
		TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM = 2,
		TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE	 = 3,
	} tpbrt_texture_handle_value_type_t;

	typedef struct {
		tpbrt_texture_handle_value_type_t value_type;

		union {
			tpbrt_size_t tex_idx;
			tpbrt_float_t f32;
			tpbrt_spectrum_t spectrum;
		} as;
	} tpbrt_texture_handle_t;

	typedef struct {
		tpbrt_texture_handle_t v00;
		tpbrt_texture_handle_t v01;
		tpbrt_texture_handle_t v10;
		tpbrt_texture_handle_t v11;
	} tpbrt_texture_bilinear_params_t;

	typedef struct {
		tpbrt_uint_t dimension;
		tpbrt_texture_handle_t tex1;
		tpbrt_texture_handle_t tex2;
	} tpbrt_texture_checkboard_params_t;

	typedef struct {
		tpbrt_texture_handle_t value;
	} tpbrt_texture_constant_params_t;

	typedef struct {
		tpbrt_texture_handle_t tex1;
		tpbrt_texture_handle_t tex2;
		tpbrt_vec_t dir;
	} tpbrt_texture_direction_mix_params_t;

	typedef struct {
		tpbrt_texture_handle_t inside;
		tpbrt_texture_handle_t outside;
	} tpbrt_texture_dots_params_t;

	typedef struct {
		tpbrt_uint_t octaves;
		tpbrt_float_t roughness;
	} tpbrt_texture_fbm_wrinkled_windy_params_t;

	typedef enum : uint8_t {
		TPBRT_TEXTURE_WRAP_REPEAT  = 0,
		TPBRT_TEXTURE_WRAP_BLACK   = 1,
		TPBRT_TEXTURE_WRAP_CLAMP   = 2,
		TPBRT_TEXTURE_WRAP_MAX_NUM = 3,
	} tpbrt_texture_wrap_t;

	typedef enum : uint8_t {
		TPBRT_TEXTURE_FILTER_BILINEAR  = 0,
		TPBRT_TEXTURE_FILTER_EWA	   = 1,
		TPBRT_TEXTURE_FILTER_TRILINEAR = 2,
		TPBRT_TEXTURE_FILTER_POINT	   = 3,
		TPBRT_TEXTURE_FILTER_MAX_NUM   = 4,
	} tpbrt_texture_filter_t;

	typedef enum : uint8_t {
		TPBRT_TEXTURE_ENCODING_SRGB		 = 0,
		TPBRT_TEXTURE_ENCODING_LINEAR	 = 1,
		TPBRT_TEXTURE_ENCODING_GAMMA_VAL = 2,
		TPBRT_TEXTURE_ENCODING_MAX_NUM	 = 3,
	} tpbrt_texture_encoding_t;

	typedef struct {
		tpbrt_string_t file_name;
		tpbrt_texture_wrap_t wrap;
		tpbrt_float_t max_anisotropy;
		tpbrt_texture_filter_t filter;
		tpbrt_texture_encoding_t encoding;
		tpbrt_opt_float_t gamma;
		tpbrt_float_t scale;
		tpbrt_bool_t invert;
	} tpbrt_texture_image_map_params_t;

	typedef struct {
		tpbrt_uint_t octaves;
		tpbrt_float_t roughness;
		tpbrt_float_t scale;
		tpbrt_float_t variation;
	} tpbrt_texture_marble_params_t;

	typedef struct {
		tpbrt_texture_handle_t tex1;
		tpbrt_texture_handle_t tex2;
		tpbrt_texture_handle_t amount;
	} tpbrt_texture_mix_params_t;

	typedef struct {
		tpbrt_texture_encoding_t encoding;
		tpbrt_opt_float_t gamma;
		tpbrt_string_t file_name;
		tpbrt_float_t scale;
	} tpbrt_texture_ptex_params_t;

	typedef struct {
		tpbrt_texture_handle_t tex;
		tpbrt_texture_handle_t scale;
	} tpbrt_texture_scale_params_t;

	typedef enum : uint8_t {
		TPBRT_TEXTURE_MAPPING_UV		  = 0,
		TPBRT_TEXTURE_MAPPING_SPHERICAL	  = 1,
		TPBRT_TEXTURE_MAPPING_CYLINDRICAL = 2,
		TPBRT_TEXTURE_MAPPING_PLANAR	  = 3,
		TPBRT_TEXTURE_MAPPING_MAX_NUM	  = 4,
	} tpbrt_texture_mapping_t;

	typedef struct {
		tpbrt_float_t u_scale, v_scale;
		tpbrt_float_t u_delta, v_delta;
	} tpbrt_texture_mapping_uv_params_t;

	typedef struct {
		tpbrt_mat4_t transform;
	} tpbrt_texture_mapping_spherical_params_t;

	typedef struct {
		tpbrt_mat4_t transform;
	} tpbrt_texture_mapping_cylindrical_params_t;

	typedef struct {
		tpbrt_float_t u_delta, v_delta;
		tpbrt_vec_t v1, v2;
	} tpbrt_texture_mapping_planar_params_t;

	typedef struct {
		tpbrt_size_t idx;
		tpbrt_string_t name;
		tpbrt_texture_type_t type;
		tpbrt_texture_class_t class;
		tpbrt_texture_mapping_t mapping;

		union {
			tpbrt_texture_mapping_uv_params_t uv;
			tpbrt_texture_mapping_spherical_params_t spherical;
			tpbrt_texture_mapping_cylindrical_params_t cylindrical;
			tpbrt_texture_mapping_planar_params_t planar;
		} mapping_as;

		union {
			tpbrt_texture_bilinear_params_t bilinear;
			tpbrt_texture_checkboard_params_t checkboard;
			tpbrt_texture_constant_params_t constant;
			tpbrt_texture_direction_mix_params_t direction_mix;
			tpbrt_texture_dots_params_t dot;
			tpbrt_texture_fbm_wrinkled_windy_params_t fbm_wrinkled_windy;
			tpbrt_texture_image_map_params_t image_map;
			tpbrt_texture_marble_params_t marble;
			tpbrt_texture_mix_params_t mix;
			tpbrt_texture_ptex_params_t ptex;
			tpbrt_texture_scale_params_t scale;
		} as;
	} tpbrt_texture_t;

	typedef struct {
		tpbrt_texture_t* textures;
		tpbrt_size_t count;
	} tpbrt_textures_list_t;

	tpbrt_error_t tpbrt_get_texture_by_name(const tpbrt_textures_list_t* textures, const tpbrt_string_t* name,
	  const tpbrt_texture_t** texture);
	tpbrt_error_t tpbrt_get_texture_by_handle(const tpbrt_textures_list_t* textures, const tpbrt_texture_handle_t* handle,
	  const tpbrt_texture_t** texture);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_TEXTURE_H_
