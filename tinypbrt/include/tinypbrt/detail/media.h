#pragma once
#ifndef _TINYPBRT_MEDIA_H_
#define _TINYPBRT_MEDIA_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/color.h>
#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/math.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum tpbrt_media_type_t : uint8_t {
		TPBRT_MEDIA_TYPE_CLOUD		  = 0,
		TPBRT_MEDIA_TYPE_HOMOGENEOUS  = 1,
		TPBRT_MEDIA_TYPE_NANO_VDB	  = 2,
		TPBRT_MEDIA_TYPE_RGB_GRID	  = 3,
		TPBRT_MEDIA_TYPE_UNIFORM_GRID = 4,
		TPBRT_MEDIA_TYPE_MAX_NUM	  = 5,
	} tpbrt_media_type_t;

	typedef struct tpbrt_media_cloud_params_t {
		tpbrt_float_t density;
		tpbrt_float_t frequency;
		tpbrt_float_t g;
		tpbrt_point3_t p0, p1;
		tpbrt_spectrum_t sigma_a;
		tpbrt_spectrum_t sigma_s;
		tpbrt_float_t wispiness;
	} tpbrt_media_cloud_params_t;

	typedef struct tpbrt_media_homogeneous_params_t {
		tpbrt_float_t g;
		tpbrt_spectrum_t Le;
		tpbrt_float_t Le_scale;
		tpbrt_string_t preset;
		tpbrt_spectrum_t sigma_a;
		tpbrt_spectrum_t sigma_s;
		tpbrt_float_t scale;
	} tpbrt_media_homogeneous_params_t;

	typedef struct tpbrt_media_nano_vdb_params_t {
		tpbrt_float_t g;
		tpbrt_float_t Le_scale;
		tpbrt_spectrum_t sigma_a;
		tpbrt_spectrum_t sigma_s;
		tpbrt_float_t scale;
		tpbrt_string_t file_name;
		tpbrt_float_t temperature_offset;
		tpbrt_float_t temperature_scale;
	} tpbrt_media_nano_vdb_params_t;

	typedef struct tpbrt_media_rgb_grid_params_t {
		tpbrt_float_t g;
		tpbrt_rgb_array_t Le;
		tpbrt_float_t Le_scale;
		tpbrt_point3_t p0, p1;
		tpbrt_rgb_array_t sigma_a;
		tpbrt_rgb_array_t sigma_s;
		tpbrt_float_t scale;
	} tpbrt_media_rgb_grid_params_t;

	typedef struct tpbrt_media_uniform_grid_params_t {
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

	typedef struct tpbrt_media_t {
		tpbrt_size_t idx;
		tpbrt_media_type_t type;
		tpbrt_string_t name;

		union {
			tpbrt_media_cloud_params_t cloud;
			tpbrt_media_homogeneous_params_t homogeneous;
			tpbrt_media_uniform_grid_params_t uniform_grid;
			tpbrt_media_rgb_grid_params_t rgb_grid;
			tpbrt_media_nano_vdb_params_t nano_vdb;
		} as;
	} tpbrt_media_t;

	typedef tpbrt_size_t tpbrt_media_handle_t;

	typedef struct tpbrt_medias_list_t {
		tpbrt_media_t* medias;
		tpbrt_size_t count;
	} tpbrt_medias_list_t;

	tpbrt_error_t tpbrt_get_media_by_name(const tpbrt_medias_list_t* medias, const tpbrt_string_t* name,
	  const tpbrt_media_t** media);
	tpbrt_error_t tpbrt_get_media_by_handle(const tpbrt_medias_list_t* medias, const tpbrt_media_handle_t* handle,
	  const tpbrt_media_t** media);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MEDIA_H_
