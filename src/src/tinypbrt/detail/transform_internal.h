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
	tpbrt_error_t tpbrt_transforms_list_get(const tpbrt_transforms_list_t* transforms_list,
	  tpbrt_transform_handle_t transform_handle, const tpbrt_mat4_t** transform);
	void tpbrt_transforms_list_free(tpbrt_transforms_list_t* transforms_list);

	tpbrt_bool_t tpbrt_transforms_list_empty(const tpbrt_transforms_list_t* transforms_list);
#pragma endregion

#pragma region TRANSFORMS_LIST_BUILDER

	typedef struct tpbrt_transforms_list_builder_t {
		tpbrt_transforms_list_t* list;
		tpbrt_size_t capacity;
		tpbrt_transform_idx_t* buckets;
		tpbrt_size_t buckets_count;
	} tpbrt_transforms_list_builder_t;

	tpbrt_error_t tpbrt_transforms_list_builder_init(tpbrt_transforms_list_builder_t* builder, tpbrt_transforms_list_t* list);
	tpbrt_error_t tpbrt_transforms_list_builder_append(tpbrt_transforms_list_builder_t* builder, const tpbrt_mat4_t* transform,
	  tpbrt_transform_handle_t* out_handle);
	tpbrt_error_t tpbrt_transforms_list_builder_append_animated(tpbrt_transforms_list_builder_t* builder,
	  const tpbrt_mat4_animated_t* transform, tpbrt_transform_animated_t* out_transform);
	void tpbrt_transforms_list_builder_finish(tpbrt_transforms_list_builder_t* builder);

#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_TRANSFORM_INTERNAL_H_
