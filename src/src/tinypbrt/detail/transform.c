#include <tinypbrt/pch.h>

#include <tinypbrt/detail/transform_internal.h>

#include <tinypbrt/detail/math_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region TRANSFORMS_LIST
#define TPBRT_EMPTY_TRANSFORMS_LIST (tpbrt_transforms_list_t) { .transforms = TPBRT_NULL, .count = 0u }

	tpbrt_error_t tpbrt_transforms_list_init(tpbrt_transforms_list_t* const transforms_list) {
			if (transforms_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*transforms_list = TPBRT_EMPTY_TRANSFORMS_LIST;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_transforms_list_append(tpbrt_transforms_list_t* const transforms_list,
	  const tpbrt_mat4_t* const transform, tpbrt_transform_handle_t* const out_handle) {
			if (transforms_list == TPBRT_NULL || transform == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (out_handle != TPBRT_NULL) { *out_handle = TPBRT_TRANSFORM_HANDLE_INVALID; }

			if (transforms_list->count > 0u) {
					for (tpbrt_size_t i = 0u; i < transforms_list->count; ++i) {
							if (tpbrt_mat4_equals(transforms_list->transforms + i, transform) == TPBRT_TRUE) {
								out_handle->transform_idx = i;
								return TPBRT_ERROR_NONE;
							}
					}
			}

		const tpbrt_size_t new_count = transforms_list->count + 1;

		tpbrt_mat4_t* new_list		 = realloc(transforms_list->transforms, sizeof(tpbrt_mat4_t) * new_count);
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		transforms_list->transforms							= new_list;
		transforms_list->transforms[transforms_list->count] = *transform;
			if (out_handle != TPBRT_NULL) { out_handle->transform_idx = transforms_list->count; }
		transforms_list->count = new_count;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_transforms_list_get(const tpbrt_transforms_list_t transforms_list,
	  const tpbrt_transform_handle_t transform_handle, const tpbrt_mat4_t** const transform) {
			if (transform == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (transform_handle.transform_idx == TPBRT_MEDIA_IDX_INVALID ||
				transform_handle.transform_idx >= transforms_list.count) {
				*transform = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_HANDLE;
			}

		*transform = transforms_list.transforms + transform_handle.transform_idx;
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_transforms_list_free(tpbrt_transforms_list_t* const transforms_list) {
			if (transforms_list == TPBRT_NULL || transforms_list->transforms == TPBRT_NULL) { return; }

		free(transforms_list->transforms);
		*transforms_list = TPBRT_EMPTY_TRANSFORMS_LIST;
	}

	tpbrt_bool_t tpbrt_transforms_list_empty(const tpbrt_transforms_list_t transforms_list) {
		return transforms_list.transforms == TPBRT_NULL || transforms_list.count == 0u;
	}

#pragma endregion

#pragma region API

	tpbrt_error_t tpbrt_get_transform(const tpbrt_transforms_list_t transforms, const tpbrt_transform_handle_t handle,
	  const tpbrt_mat4_t** transform) {
		return tpbrt_transforms_list_get(transforms, handle, transform);
	}

#pragma endregion

#ifdef __cplusplus
}
#endif