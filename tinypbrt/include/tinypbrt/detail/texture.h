#pragma once
#ifndef _TINYPBRT_TEXTURE_H_
#define _TINYPBRT_TEXTURE_H_

#include "param.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_TEXTURE_TYPE_BILERP		 = 0,
		TPBRT_TEXTURE_TYPE_CHECKER_BOARD = 1,
		TPBRT_TEXTURE_TYPE_CONSTANT		 = 2,
		TPBRT_TEXTURE_TYPE_DIRECTION_MIX = 3,
		TPBRT_TEXTURE_TYPE_DOTS			 = 4,
		TPBRT_TEXTURE_TYPE_FBM			 = 5,
		TPBRT_TEXTURE_TYPE_IMAGE_MAP	 = 6,
		TPBRT_TEXTURE_TYPE_MARBLE		 = 7,
		TPBRT_TEXTURE_TYPE_MIX			 = 8,
		TPBRT_TEXTURE_TYPE_PTEX			 = 9,
		TPBRT_TEXTURE_TYPE_SCALE		 = 10,
		TPBRT_TEXTURE_TYPE_WINDY		 = 11,
		TPBRT_TEXTURE_TYPE_WRINKLED		 = 12,
	} tpbrt_texture_type_t;

	struct tpbrt_texture_t;

	typedef struct {
		float value;
		struct tpbrt_texture_t* texture;
	} tpbrt_float_valued_texture_t;

	typedef struct {
		tpbrt_spectrum_t value;
		struct tpbrt_texture_t* texture;
	} tpbrt_spectrum_valued_texture_t;

	typedef struct {
		union {
			tpbrt_float_valued_texture_t f32_texture;
			tpbrt_spectrum_valued_texture_t spectrum_texture;
		} v[4];
	} tpbrt_texture_bilinear_params_t;

	typedef struct {
		uint32_t dimension;
		union {
			tpbrt_float_valued_texture_t f32_texture;
			tpbrt_spectrum_valued_texture_t spectrum_texture;
		} tex1;
		union {
			tpbrt_float_valued_texture_t f32_texture;
			tpbrt_spectrum_valued_texture_t spectrum_texture;
		} tex2;
	} tpbrt_texture_checkboard_params_t;

	typedef struct {
		union {
			tpbrt_float_valued_texture_t f32_texture;
			tpbrt_spectrum_valued_texture_t spectrum_texture;
		} value;
	} tpbrt_texture_constant_params_t;

	typedef struct {
		union {
			tpbrt_float_valued_texture_t f32_texture;
			tpbrt_spectrum_valued_texture_t spectrum_texture;
		} tex1;
		union {
			tpbrt_float_valued_texture_t f32_texture;
			tpbrt_spectrum_valued_texture_t spectrum_texture;
		} tex2;
		tpbrt_vec3_t dir;
	} tpbrt_texture_direction_mix_params_t;

	typedef struct {
		union {
			tpbrt_float_valued_texture_t f32_texture;
			tpbrt_spectrum_valued_texture_t spectrum_texture;
		} inside, outside;
	} tpbrt_texture_dots_params_t;

	typedef struct {
		uint32_t octaves;
		float roughness;
	} tpbrt_texture_fbm_wrinkled_windy_params_t;

	typedef enum : uint8_t {
		TPBRT_TEXTURE_WRAP_REPEAT = 0,
		TPBRT_TEXTURE_WRAP_BLACK = 1,
		TPBRT_TEXTURE_WRAP_CLAMP = 2,
	} tpbrt_texture_wrap_type_t;

	typedef enum : uint8_t {
		TPBRT_TEXTURE_FILTER_BILINEAR = 0,
		TPBRT_TEXTURE_FILTER_EWA = 1,
		TPBRT_TEXTURE_FILTER_TRILINEAR = 2,
		TPBRT_TEXTURE_FILTER_POINT = 3,
	} tpbrt_texture_filter_type_t;

	typedef enum : uint8_t {
		TPBRT_TEXTURE_ENCODING_SRGB = 0,
		TPBRT_TEXTURE_ENCODING_LINEAR = 1,
		TPBRT_TEXTURE_ENCODING_GAMMA_VAL = 2,
	} tpbrt_texture_encoding_type_t;

	typedef struct {
		tpbrt_string_t file_name;
		tpbrt_texture_wrap_type_t wrap;
		float max_anisotropy;
		tpbrt_texture_filter_type_t filter;
		tpbrt_texture_encoding_type_t encoding;
		float gamma;
		float scale;
		tpbrt_bool_t invert;
	} tpbrt_texture_image_map_params_t;

	typedef struct {
		uint32_t octaves;
		float roughness;
		float scale;
		float variation;
	} tpbrt_texture_marble_params_t;

	typedef struct {
		union {
			tpbrt_float_valued_texture_t f32_texture;
			tpbrt_spectrum_valued_texture_t spectrum_texture;
		} tex1;
		union {
			tpbrt_float_valued_texture_t f32_texture;
			tpbrt_spectrum_valued_texture_t spectrum_texture;
		} tex2;
		tpbrt_float_valued_texture_t amount;
	} tpbrt_texture_mix_params_t;

	typedef struct {
		tpbrt_texture_encoding_type_t encoding;
		float gamma;
		tpbrt_string_t file_name;
		float scale;
	} tpbrt_texture_ptex_params_t;

	typedef struct {
		union {
			tpbrt_float_valued_texture_t f32_texture;
			tpbrt_spectrum_valued_texture_t spectrum_texture;
		} tex;
		tpbrt_float_valued_texture_t scale;
	} tpbrt_texture_scale_params_t;

	typedef enum : uint8_t {
		TPBRT_TEXTURE_MAPPING_UV = 0,
		TPBRT_TEXTURE_MAPPING_SPHERICAL = 1,
		TPBRT_TEXTURE_MAPPING_CYLINDRICAL = 2,
		TPBRT_TEXTURE_MAPPING_PLANAR = 3,
	} tpbrt_texture_mapping_type_t;

	typedef struct {
		float u_scale;
		float v_scale;
		float u_delta;
		float v_delta;
	} tpbrt_texture_mapping_uv_params_t;

	typedef struct {
		float u_delta;
		float v_delta;
		tpbrt_vec3_t v1;
		tpbrt_vec3_t v2;
	} tpbrt_texture_mapping_planar_params_t;

	typedef struct {
		tpbrt_string_t name;
		tpbrt_texture_type_t type;
		tpbrt_texture_mapping_type_t mapping;
		union {
			tpbrt_texture_mapping_uv_params_t uv_params;
			tpbrt_texture_mapping_planar_params_t planar_params;
		};
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
		};
	} tpbrt_texture_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_TEXTURE_H_
