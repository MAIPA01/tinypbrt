#pragma once
#ifndef _TINYPBRT_FILM_H_
#define _TINYPBRT_FILM_H_

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/coord.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_FILM_TYPE_RGB		 = 0,
		TPBRT_FILM_TYPE_GBUFFER	 = 1,
		TPBRT_FILM_TYPE_SPECTRAL = 2,
	} tpbrt_film_type_t;

	typedef struct {
		tpbrt_coordinate_system_t coordinate_system;
	} tpbrt_film_gbuffer_params_t;

	typedef struct {
		tpbrt_uint_t nbuckets;
		tpbrt_float_t lambda_min;
		tpbrt_float_t lambda_max;
	} tpbrt_film_spectral_params_t;

	typedef enum {
		TPBRT_FILM_SENSOR_CIE_1931			  = 0,
		TPBRT_FILM_SENSOR_CANON_EOS_100D	  = 1,
		TPBRT_FILM_SENSOR_CANON_EOS_1DX_MKII  = 2,
		TPBRT_FILM_SENSOR_CANON_EOS_200D	  = 3,
		TPBRT_FILM_SENSOR_CANON_EOS_200D_MKII = 4,
		TPBRT_FILM_SENSOR_CANON_EOS_5D		  = 5,
		TPBRT_FILM_SENSOR_CANON_EOS_5D_MKII	  = 6,
		TPBRT_FILM_SENSOR_CANON_EOS_5D_MKIII  = 7,
		TPBRT_FILM_SENSOR_CANON_EOS_5D_MKIV	  = 8,
		TPBRT_FILM_SENSOR_CANON_EOS_5DS		  = 9,
		TPBRT_FILM_SENSOR_CANON_EOS_M		  = 10,
		TPBRT_FILM_SENSOR_HASSELBLAD_L1D_20C  = 11,
		TPBRT_FILM_SENSOR_NIKON_D810		  = 12,
		TPBRT_FILM_SENSOR_NIKON_D850		  = 13,
		TPBRT_FILM_SENSOR_SONY_ILCE_6400	  = 14,
		TPBRT_FILM_SENSOR_SONY_ILCE_7M3		  = 15,
		TPBRT_FILM_SENSOR_SONY_ILCE_7RM3	  = 16,
		TPBRT_FILM_SENSOR_SONY_ILCE_9		  = 17,
	} tpbrt_film_sensor_t;

	typedef struct {
		tpbrt_float_t crop_window[4];
		tpbrt_uint_t pixel_bounds[4];
		tpbrt_string_t file_name;
		tpbrt_film_sensor_t sensor;
		tpbrt_uint_t x_resolution;
		tpbrt_uint_t y_resolution;
		tpbrt_float_t diagonal;
		tpbrt_bool_t save_fp16;
		tpbrt_float_t iso;
		tpbrt_float_t white_balance;
		tpbrt_float_t max_component_value;
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
