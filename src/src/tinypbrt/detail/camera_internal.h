#pragma once
#ifndef _TINYPBRT_CAMERA_INTERNAL_H_
#define _TINYPBRT_CAMERA_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/camera.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_camera_type_from_string(tpbrt_string_t type_str, tpbrt_camera_type_t* type);

	tpbrt_error_t tpbrt_camera_spherical_mapping_from_string(tpbrt_string_t mapping_str,
	  tpbrt_camera_spherical_mapping_t* mapping);

	tpbrt_error_t tpbrt_camera_realistic_aperture_builtin_from_string(tpbrt_string_t builtin_str,
	  tpbrt_camera_realistic_aperture_builtin_t* builtin);

	tpbrt_error_t tpbrt_camera_init(tpbrt_string_t type_str, tpbrt_params_list_t params, const tpbrt_film_t* film,
	  const tpbrt_mat4_animated_t* ctm, tpbrt_media_handle_t interior_media_handle, tpbrt_media_handle_t exterior_media_handle,
	  tpbrt_camera_t* camera);

	void tpbrt_camera_free(tpbrt_camera_t* camera);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_CAMERA_INTERNAL_H_