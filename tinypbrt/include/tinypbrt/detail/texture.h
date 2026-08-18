#pragma once
#ifndef _TINYPBRT_TEXTURE_H_
#define _TINYPBRT_TEXTURE_H_

#include <tinypbrt/detail/fwd.h>
#include <tinypbrt/detail/math.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_TEXTURE_TYPE_FLOAT	= 0,
		TPBRT_TEXTURE_TYPE_SPECTRUM = 1,
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
	} tpbrt_texture_class_t;

	typedef enum : uint8_t {
		TPBRT_TEXTURE_VALUE_TYPE_SINGLE	 = 0,
		TPBRT_TEXTURE_VALUE_TYPE_TEXTURE = 1,
	} tpbrt_texture_value_type_t;

	struct tpbrt_texture_t;

	typedef struct {
		struct tpbrt_texture_t* v[4];
	} tpbrt_texture_bilinear_params_t;

	typedef struct {
		tpbrt_uint_t dimension;
		struct tpbrt_texture_t* tex1;
		struct tpbrt_texture_t* tex2;
	} tpbrt_texture_checkboard_params_t;

	typedef struct {
		struct tpbrt_texture_t* value;
	} tpbrt_texture_constant_params_t;

	typedef struct {
		struct tpbrt_texture_t* tex1;
		struct tpbrt_texture_t* tex2;
		tpbrt_vec_t dir;
	} tpbrt_texture_direction_mix_params_t;

	typedef struct {
		struct tpbrt_texture_t* inside;
		struct tpbrt_texture_t* outside;
	} tpbrt_texture_dots_params_t;

	typedef struct {
		tpbrt_uint_t octaves;
		tpbrt_float_t roughness;
	} tpbrt_texture_fbm_wrinkled_windy_params_t;

	typedef enum : uint8_t {
		TPBRT_TEXTURE_WRAP_REPEAT = 0,
		TPBRT_TEXTURE_WRAP_BLACK  = 1,
		TPBRT_TEXTURE_WRAP_CLAMP  = 2,
	} tpbrt_texture_wrap_type_t;

	typedef enum : uint8_t {
		TPBRT_TEXTURE_FILTER_BILINEAR  = 0,
		TPBRT_TEXTURE_FILTER_EWA	   = 1,
		TPBRT_TEXTURE_FILTER_TRILINEAR = 2,
		TPBRT_TEXTURE_FILTER_POINT	   = 3,
	} tpbrt_texture_filter_type_t;

	typedef enum : uint8_t {
		TPBRT_TEXTURE_ENCODING_SRGB		 = 0,
		TPBRT_TEXTURE_ENCODING_LINEAR	 = 1,
		TPBRT_TEXTURE_ENCODING_GAMMA_VAL = 2,
	} tpbrt_texture_encoding_type_t;

	typedef struct {
		tpbrt_string_t file_name;
		tpbrt_texture_wrap_type_t wrap;
		tpbrt_float_t max_anisotropy;
		tpbrt_texture_filter_type_t filter;
		tpbrt_texture_encoding_type_t encoding;
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
		struct tpbrt_texture_t* tex1;
		struct tpbrt_texture_t* tex2;
		struct tpbrt_texture_t* amount;
	} tpbrt_texture_mix_params_t;

	typedef struct {
		tpbrt_texture_encoding_type_t encoding;
		tpbrt_opt_float_t gamma;
		tpbrt_string_t file_name;
		tpbrt_float_t scale;
	} tpbrt_texture_ptex_params_t;

	typedef struct {
		struct tpbrt_texture_t* tex;
		struct tpbrt_texture_t* scale;
	} tpbrt_texture_scale_params_t;

	typedef enum : uint8_t {
		TPBRT_TEXTURE_MAPPING_UV		  = 0,
		TPBRT_TEXTURE_MAPPING_SPHERICAL	  = 1,
		TPBRT_TEXTURE_MAPPING_CYLINDRICAL = 2,
		TPBRT_TEXTURE_MAPPING_PLANAR	  = 3,
	} tpbrt_texture_mapping_type_t;

	typedef struct {
		tpbrt_float_t u_scale, v_scale;
		tpbrt_float_t u_delta, v_delta;
	} tpbrt_texture_mapping_uv_params_t;

	typedef struct {
		tpbrt_float_t u_delta, v_delta;
		tpbrt_vec_t v1, v2;
	} tpbrt_texture_mapping_planar_params_t;

	typedef struct {
		tpbrt_string_t name;
		tpbrt_texture_type_t type;
		tpbrt_texture_class_t class;
		tpbrt_texture_mapping_type_t mapping;
		tpbrt_texture_value_type_t value_type;

		union {
			tpbrt_texture_mapping_uv_params_t uv_params;
			tpbrt_texture_mapping_planar_params_t planar_params;
		};

		union {
			tpbrt_float_t f32_value;
			tpbrt_spectrum_t spectrum_value;

			union {
				tpbrt_texture_bilinear_params_t bilinear_params;
				tpbrt_texture_checkboard_params_t checkboard_params;
				tpbrt_texture_constant_params_t constant_params;
				tpbrt_texture_direction_mix_params_t direction_mix_params;
				tpbrt_texture_dots_params_t dot_params;
				tpbrt_texture_fbm_wrinkled_windy_params_t fbm_wrinkled_windy_params;
				tpbrt_texture_image_map_params_t image_map_params;
				tpbrt_texture_marble_params_t marble_params;
				tpbrt_texture_mix_params_t mix_params;
				tpbrt_texture_ptex_params_t ptex_params;
				tpbrt_texture_scale_params_t scale_params;
			} texture_value;
		};
	} tpbrt_texture_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_TEXTURE_H_
