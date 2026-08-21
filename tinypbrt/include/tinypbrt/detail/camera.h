#pragma once
#ifndef _TINYPBRT_CAMERA_H_
#define _TINYPBRT_CAMERA_H_

#include <tinypbrt/detail/math.h>
#include "tinypbrt/detail/common.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_CAMERA_TYPE_ORTHOGRAPHIC = 0,
		TPBRT_CAMERA_TYPE_PERSPECTIVE  = 1,
		TPBRT_CAMERA_TYPE_REALISTIC	   = 2,
		TPBRT_CAMERA_TYPE_SPHERICAL	   = 3,
		TPBRT_CAMERA_TYPE_MAX_NUM	   = 4,
	} tpbrt_camera_type_t;

	typedef struct {
		tpbrt_float_t frame_aspect_ratio;
		tpbrt_float_t screen_window[4];
		tpbrt_float_t lens_radius;
		tpbrt_float_t focal_distance;
	} tpbrt_camera_orthographic_params_t;

	typedef struct {
		tpbrt_float_t frame_aspect_ratio;
		tpbrt_float_t screen_window[4];
		tpbrt_float_t lens_radius;
		tpbrt_float_t focal_distance;
		tpbrt_float_t fov;
	} tpbrt_camera_perspective_params_t;

	typedef enum : uint8_t {
		TPBRT_CAMERA_SPHERICAL_MAPPING_EQUAL_AREA			= 0,
		TPBRT_CAMERA_SPHERICAL_MAPPING_NOT_EQUI_RECTANGULAR = 1,
		TPBRT_CAMERA_SPHERICAL_MAPPING_MAX_NUM				= 2,
	} tpbrt_camera_spherical_mapping_t;

	typedef struct {
		tpbrt_camera_spherical_mapping_t mapping;
	} tpbrt_camera_spherical_params_t;

	typedef enum {
		TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_FILE_NAME = 0,
		TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_BUILTIN   = 1,
	} tpbrt_camera_realistic_aperture_type_t;

	typedef enum {
		TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_CIRCULAR = 0,
		TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_GAUSSIAN = 1,
		TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_SQUARE	 = 2,
		TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_PENTAGON = 3,
		TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_STAR	 = 4,
		TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_MAX_NUM	 = 5,
	} tpbrt_camera_realistic_aperture_builtin_t;

	typedef struct {
		tpbrt_camera_realistic_aperture_type_t type;

		union {
			tpbrt_camera_realistic_aperture_builtin_t builtin;
			tpbrt_string_t file_name;
		};
	} tpbrt_camera_realistic_aperture_t;

	typedef struct {
		tpbrt_string_t lens_file;
		tpbrt_float_t aperture_diameter;
		tpbrt_float_t focus_distance;
		tpbrt_camera_realistic_aperture_t aperture;
	} tpbrt_camera_realistic_params_t;

	typedef struct {
		tpbrt_camera_type_t type;
		tpbrt_float_t shutter_open;
		tpbrt_float_t shutter_close;
		tpbrt_mat4_t transform;

		union {
			tpbrt_camera_orthographic_params_t orthographic_params;
			tpbrt_camera_perspective_params_t perspective_params;
			tpbrt_camera_spherical_params_t spherical_params;
			tpbrt_camera_realistic_params_t realistic_params;
		};
	} tpbrt_camera_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_CAMERA_H_
