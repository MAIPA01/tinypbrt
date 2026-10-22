#pragma once
#ifndef _TINYPBRT_CAMERA_INTERNAL_H_
#define _TINYPBRT_CAMERA_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/camera.h>

#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct tpbrt_camera_create_info_t {
		tpbrt_params_list_t params;
		tpbrt_transform_animated_t camera_transform;
		tpbrt_media_handle_t interior_media_handle;
		tpbrt_media_handle_t exterior_media_handle;
		tpbrt_camera_type_t type : 4;
	} tpbrt_camera_create_info_t;

	tpbrt_error_t tpbrt_camera_type_from_string(tpbrt_string_t type_str, tpbrt_camera_type_t* type);

	tpbrt_error_t tpbrt_camera_spherical_mapping_from_string(tpbrt_string_t mapping_str,
	  tpbrt_camera_spherical_mapping_t* mapping);

	tpbrt_error_t tpbrt_camera_realistic_aperture_builtin_from_string(tpbrt_string_t builtin_str,
	  tpbrt_camera_realistic_aperture_builtin_t* builtin);

	tpbrt_error_t tpbrt_camera_create_info_init_default(tpbrt_camera_create_info_t* info);

	tpbrt_error_t tpbrt_camera_create_info_init(tpbrt_string_t type_str, tpbrt_params_list_t params,
	  tpbrt_transform_animated_t transform, tpbrt_media_handle_t interior_media_handle,
	  tpbrt_media_handle_t exterior_media_handle, tpbrt_camera_create_info_t* info);

	void tpbrt_camera_create_info_free(tpbrt_camera_create_info_t* info);

	tpbrt_error_t tpbrt_camera_init(const tpbrt_camera_create_info_t* info, const tpbrt_film_t* film, tpbrt_camera_t* camera);

	void tpbrt_camera_free(tpbrt_camera_t* camera);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_CAMERA_INTERNAL_H_
