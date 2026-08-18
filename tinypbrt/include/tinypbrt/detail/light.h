#pragma once
#ifndef _TINYPBRT_LIGHT_H_
#define _TINYPBRT_LIGHT_H_

#include <tinypbrt/detail/color.h>
#include <tinypbrt/detail/math.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_LIGHT_SOURCE_DISTANT	   = 0,
		TPBRT_LIGHT_SOURCE_GONIOMETRIC = 1,
		TPBRT_LIGHT_SOURCE_INFINITE	   = 2,
		TPBRT_LIGHT_SOURCE_POINT	   = 3,
		TPBRT_LIGHT_SOURCE_PROJECTION  = 4,
		TPBRT_LIGHT_SOURCE_SPOT		   = 5,
	} tpbrt_light_source_type_t;

	typedef struct {
		tpbrt_spectrum_t L;
		tpbrt_point_t from;
		tpbrt_point_t to;
	} tpbrt_light_source_distant_params_t;

	typedef struct {
		tpbrt_string_t file_name;
		tpbrt_spectrum_t I;
	} tpbrt_light_source_goniometric_params_t;

	typedef struct {
		tpbrt_string_t file_name;
		tpbrt_point3_t portal[4];
		tpbrt_spectrum_t L;
	} tpbrt_light_source_infinite_params_t;

	typedef struct {
		tpbrt_spectrum_t I;
		tpbrt_point_t from;
	} tpbrt_light_source_point_params_t;

	typedef struct {
		tpbrt_spectrum_t I;
		tpbrt_float_t fov;
		tpbrt_string_t file_name;
	} tpbrt_light_source_projection_params_t;

	typedef struct {
		tpbrt_spectrum_t I;
		tpbrt_point_t from, to;
		tpbrt_float_t cone_angle;
		tpbrt_float_t cone_delta_angle;
	} tpbrt_light_source_spot_params_t;

	typedef struct {
		tpbrt_mat4_t transform;
		tpbrt_light_source_type_t type;
		tpbrt_opt_float_t power_illuminance;
		tpbrt_float_t scale;

		union {
			tpbrt_light_source_distant_params_t distant_params;
			tpbrt_light_source_goniometric_params_t goniometric_params;
			tpbrt_light_source_infinite_params_t infinite_params;
			tpbrt_light_source_point_params_t point_params;
			tpbrt_light_source_projection_params_t projection_params;
			tpbrt_light_source_spot_params_t spot_params;
		};
	} tpbrt_light_source_t;

	typedef enum : uint8_t {
		TPBRT_AREA_LIGHT_DIFFUSE = 0,
	} tpbrt_area_light_type_t;

	typedef struct {
		tpbrt_string_t file_name;
		tpbrt_spectrum_t L;
		tpbrt_bool_t two_sided;
	} tpbrt_area_light_diffuse_params_t;

	typedef struct {
		tpbrt_area_light_type_t type;

		union {
			tpbrt_area_light_diffuse_params_t diffuse_params;
		};
	} tpbrt_area_light_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_LIGHT_H_
