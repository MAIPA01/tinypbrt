#pragma once
#ifndef _TINYPBRT_CAMERA_H_
#define _TINYPBRT_CAMERA_H_

#include "../param.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum : uint8_t {
	TPBRT_CAMERA_ORTHOGRAPHIC = 0,
	TPBRT_CAMERA_PERSPECTIVE  = 1,
	TPBRT_CAMERA_REALISTIC	  = 2,
	TPBRT_CAMERA_SPHERICAL	  = 3,
} tpbrt_camera_type_t;

typedef struct {
	float frame_aspect_ratio;
	float screen_window;
	float lens_radius;
	float focal_distance;
} tpbrt_camera_orthographic_params_t;

typedef struct {
	float frame_aspect_ratio;
	float screen_window;
	float lens_radius;
	float focal_distance;
	float fov;
} tpbrt_camera_perspective_params_t;

typedef struct {
	tpbrt_string_t lens_file;
	float aperture_diameter;
	float focus_distance;
	tpbrt_string_t aperture;
} tpbrt_camera_realistic_params_t;

typedef struct {
	tpbrt_string_t mapping;
} tpbrt_camera_spherical_params_t;

typedef struct {
	tpbrt_camera_type_t type;
	float shutter_open;
	float shutter_close;

	union {
		tpbrt_camera_orthographic_params_t orthographic_params;
		tpbrt_camera_perspective_params_t perspective_params;
		tpbrt_camera_realistic_params_t realistic_params;
		tpbrt_camera_spherical_params_t spherical_params;
	};
} tpbrt_camera_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_CAMERA_H_
