#pragma once
#ifndef _TINYPBRT_TRANSFORM_INTERNAL_H_
#define _TINYPBRT_TRANSFORM_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/transform.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region TRANSFORMS_LIST
	tpbrt_error_t tpbrt_transforms_list_init(tpbrt_transforms_list_t* transforms_list);
	tpbrt_error_t tpbrt_transforms_list_append(tpbrt_transforms_list_t* transforms_list, const tpbrt_mat4_t* transform,
	  tpbrt_transform_handle_t* out_handle);
	tpbrt_error_t tpbrt_transforms_list_get(tpbrt_transforms_list_t transforms_list, tpbrt_transform_handle_t transform_handle,
	  const tpbrt_mat4_t** transform);
	void tpbrt_transforms_list_free(tpbrt_transforms_list_t* transforms_list);

	tpbrt_bool_t tpbrt_transforms_list_empty(tpbrt_transforms_list_t transforms_list);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_TRANSFORM_INTERNAL_H_
