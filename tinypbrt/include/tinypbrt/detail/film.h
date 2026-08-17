#pragma once
#ifndef _TINYPBRT_FILM_H_
#define _TINYPBRT_FILM_H_

#include "../param.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_FILM_RGB		= 0,
		TPBRT_FILM_GBUFFER	= 1,
		TPBRT_FILM_SPECTRAL = 2,
	} tpbrt_film_type_t;

	typedef struct {
		tpbrt_string_t coordinate_system;
	} tpbrt_film_gbuffer_params_t;

	typedef struct {
		uint32_t nbuckets;
		float lambda_min;
		float lambda_max;
	} tpbrt_film_spectral_params_t;

	typedef struct {
		float crop_window[4];
		uint32_t pixel_bounds[4];
		tpbrt_string_t filename;
		tpbrt_string_t sensor;
		uint32_t x_resolution;
		uint32_t y_resolution;
		float diagonal;
		tpbrt_bool_t save_fp16;
		float ios;
		float white_balance;
		float max_component_value;
		tpbrt_film_type_t type;
		union {
			tpbrt_film_gbuffer_params_t gbuffer_params;
			tpbrt_film_spectral_params_t spectral_params;
		};
	} tpbrt_film_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_FILM_H_
