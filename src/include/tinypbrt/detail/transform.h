#pragma once
#ifndef _TINYPBRT_TRANSFORM_H_
#define _TINYPBRT_TRANSFORM_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/math.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct tpbrt_transform_handle_t {
		tpbrt_transform_idx_t transform_idx;
	} tpbrt_transform_handle_t;

#define TPBRT_TRANSFORM_HANDLE_INVALID_INIT { .transform_idx = TPBRT_TRANSFORM_IDX_INVALID }
#define TPBRT_TRANSFORM_HANDLE_INVALID		(tpbrt_transform_handle_t) TPBRT_TRANSFORM_HANDLE_INVALID_INIT

	typedef struct tpbrt_transform_animated_t {
		tpbrt_transform_handle_t start_transform;
		tpbrt_transform_handle_t end_transform;
	} tpbrt_transform_animated_t;

	typedef struct tpbrt_transforms_list_t {
		tpbrt_mat4_t* transforms;
		tpbrt_size_t count;
	} tpbrt_transforms_list_t;

	TPBRT_API tpbrt_error_t tpbrt_get_transform(const tpbrt_transforms_list_t* transforms, tpbrt_transform_handle_t handle,
	  const tpbrt_mat4_t** transform);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_TRANSFORM_H_
