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
			if (transforms_list == TPBRT_NULL || transform == TPBRT_NULL || out_handle == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
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

			if (transform_handle.transform_idx == TPBRT_TRANSFORM_IDX_INVALID ||
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

#pragma region TRANSFORMS_LIST_BUILDER

	tpbrt_error_t tpbrt_transforms_list_builder_init(tpbrt_transforms_list_builder_t* const builder,
	  tpbrt_transforms_list_t* const list) {
			if (builder == TPBRT_NULL || list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*builder =
		  (tpbrt_transforms_list_builder_t) { .list = list, .capacity = list->count, .buckets = TPBRT_NULL, .buckets_count = 0u };
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_transforms_list_builder_rehash(tpbrt_transforms_list_builder_t* const builder,
	  const tpbrt_size_t new_buckets_count) {
			if (builder == TPBRT_NULL || builder->list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_transform_idx_t* const new_buckets = realloc(builder->buckets, new_buckets_count * sizeof(tpbrt_transform_idx_t));
			if (new_buckets == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		memset(new_buckets, TPBRT_TRANSFORM_IDX_INVALID, new_buckets_count * sizeof(tpbrt_transform_idx_t));

		const tpbrt_size_t mask = new_buckets_count - 1u;
			for (tpbrt_size_t i = 0u; i < builder->list->count; ++i) {
				tpbrt_size_t slot = tpbrt_mat4_hash(builder->list->transforms + i) & mask;
					while (new_buckets[slot] != TPBRT_TRANSFORM_IDX_INVALID) { slot = (slot + 1u) & mask; }
				new_buckets[slot] = (tpbrt_transform_idx_t)i;
			}

		free(builder->buckets);
		builder->buckets	   = new_buckets;
		builder->buckets_count = new_buckets_count;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_transforms_list_builder_append(tpbrt_transforms_list_builder_t* const builder,
	  const tpbrt_mat4_t* const transform, tpbrt_transform_handle_t* const out_handle) {
			if (builder == TPBRT_NULL || builder->list == TPBRT_NULL || transform == TPBRT_NULL || out_handle == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}
		*out_handle							= TPBRT_TRANSFORM_HANDLE_INVALID;
		tpbrt_transforms_list_t* const list = builder->list;

			if ((list->count + 1u) * 2u > builder->buckets_count) {
				const tpbrt_size_t n	= builder->buckets_count == 0u ? 64u : builder->buckets_count * 2u;
				const tpbrt_error_t err = tpbrt_transforms_list_builder_rehash(builder, n);
					if (err != TPBRT_ERROR_NONE) { return err; }
			}

		const tpbrt_size_t mask = builder->buckets_count - 1u;
		tpbrt_size_t slot		= tpbrt_mat4_hash(transform) & mask;
			while (builder->buckets[slot] != TPBRT_TRANSFORM_IDX_INVALID) {
				const tpbrt_transform_idx_t idx = builder->buckets[slot];
					if (tpbrt_mat4_equals(list->transforms + idx, transform) == TPBRT_TRUE) {
						out_handle->transform_idx = idx;
						return TPBRT_ERROR_NONE;
					}
				slot = (slot + 1u) & mask;
			}

			if (list->count >= (tpbrt_size_t)TPBRT_TRANSFORM_IDX_INVALID) { return TPBRT_ERROR_OUT_OF_MEMORY; }
			if (list->count == builder->capacity) {
				const tpbrt_size_t cap	= builder->capacity == 0u ? 16u : builder->capacity * 2u;
				tpbrt_mat4_t* const tmp = realloc(list->transforms, cap * sizeof(tpbrt_mat4_t));
					if (tmp == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
				list->transforms  = tmp;
				builder->capacity = cap;
			}

		const tpbrt_transform_idx_t idx = (tpbrt_transform_idx_t)list->count++;
		list->transforms[idx]			= *transform;
		builder->buckets[slot]			= idx;
		out_handle->transform_idx		= idx;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_transforms_list_builder_append_animated(tpbrt_transforms_list_builder_t* const builder,
	  const tpbrt_mat4_animated_t* const transform, tpbrt_transform_animated_t* const out_transform) {
			if (builder == TPBRT_NULL || builder->list == TPBRT_NULL || transform == TPBRT_NULL || out_transform == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_error_t err =
		  tpbrt_transforms_list_builder_append(builder, &transform->start, &out_transform->start_transform);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_transforms_list_builder_append(builder, &transform->end, &out_transform->end_transform);
	}

	void tpbrt_transforms_list_builder_finish(tpbrt_transforms_list_builder_t* const builder) {
			if (builder == TPBRT_NULL) { return; }

		free(builder->buckets);

		tpbrt_transforms_list_t* const list = builder->list;
			if (list != TPBRT_NULL && list->count > 0u && builder->capacity > list->count) {
				tpbrt_mat4_t* const tmp = realloc(list->transforms, list->count * sizeof(tpbrt_mat4_t));
					if (tmp != TPBRT_NULL) { list->transforms = tmp; }
			}

		*builder =
		  (tpbrt_transforms_list_builder_t) { .list = TPBRT_NULL, .capacity = 0u, .buckets = TPBRT_NULL, .buckets_count = 0u };
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