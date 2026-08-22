#pragma once
#ifndef _TINYPBRT_CAMERA_INTERNAL_H_
#define _TINYPBRT_CAMERA_INTERNAL_H_

#include <tinypbrt/detail/camera.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/film_internal.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_create_camera(const tpbrt_string_t* type_str, const tpbrt_params_list_t* params, const tpbrt_film_t* film,
	  const tpbrt_mat4_t* ctm, tpbrt_camera_t** camera);

	void tpbrt_free_camera(tpbrt_camera_t** camera);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_CAMERA_INTERNAL_H_