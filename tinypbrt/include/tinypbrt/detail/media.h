#pragma once
#ifndef _TINYPBRT_MEDIA_H_
#define _TINYPBRT_MEDIA_H_

#include <tinypbrt/detail/color.h>
#include <tinypbrt/detail/fwd.h>
#include <tinypbrt/detail/math.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_MEDIA_TYPE_CLOUD		  = 0,
		TPBRT_MEDIA_TYPE_HOMOGENEOUS  = 1,
		TPBRT_MEDIA_TYPE_NANO_VDB	  = 2,
		TPBRT_MEDIA_TYPE_RGB_GRID	  = 3,
		TPBRT_MEDIA_TYPE_UNIFORM_GRID = 4,
	} tpbrt_media_type_t;

	typedef struct {
		tpbrt_float_t g;
		tpbrt_spectrum_t Le;
		tpbrt_float_t Le_scale;
		tpbrt_string_t preset;
		tpbrt_spectrum_t sigma_a;
		tpbrt_spectrum_t sigma_s;
		tpbrt_float_t scale;
	} tpbrt_media_homogeneous_params_t;

	typedef struct {
		tpbrt_float_array_t density;
		tpbrt_float_t g;
		tpbrt_spectrum_t Le;
		tpbrt_float_t Le_scale;
		tpbrt_point3_t p0, p1;
		tpbrt_uint_t nx, ny, nz;
		tpbrt_string_t preset;
		tpbrt_spectrum_t sigma_a;
		tpbrt_spectrum_t sigma_s;
		tpbrt_float_t scale;
		tpbrt_float_array_t temperature;
		tpbrt_float_t temperature_offset;
		tpbrt_float_t temperature_scale;
	} tpbrt_media_uniform_grid_params_t;

	typedef struct {
		tpbrt_float_t g;
		tpbrt_rgb_array_t Le;
		tpbrt_float_t Le_scale;
		tpbrt_point3_t p0, p1;
		tpbrt_rgb_array_t sigma_a;
		tpbrt_rgb_array_t sigma_s;
		tpbrt_float_t scale;
	} tpbrt_media_rgb_grid_params_t;

	typedef struct {
		tpbrt_float_t density;
		tpbrt_float_t frequency;
		tpbrt_float_t g;
		tpbrt_point3_t p0, p1;
		tpbrt_spectrum_t sigma_a;
		tpbrt_spectrum_t sigma_s;
		tpbrt_float_t wispiness;
	} tpbrt_media_cloud_params_t;

	typedef struct {
		tpbrt_float_t g;
		tpbrt_float_t Le_scale;
		tpbrt_spectrum_t sigma_a;
		tpbrt_spectrum_t sigma_s;
		tpbrt_float_t scale;
		tpbrt_string_t file_name;
		tpbrt_float_t temperature_offset;
		tpbrt_float_t temperature_scale;
	} tpbrt_media_nano_vdb_params_t;

	typedef struct {
		tpbrt_media_type_t type;

		union {
			tpbrt_media_homogeneous_params_t homogeneous_params;
			tpbrt_media_uniform_grid_params_t uniform_grid_params;
			tpbrt_media_rgb_grid_params_t rgb_grid_params;
			tpbrt_media_cloud_params_t cloud_params;
			tpbrt_media_nano_vdb_params_t nano_vdb_params;
		};
	} tpbrt_media_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MEDIA_H_
