#include <tinypbrt/pch.h>

#include <tinypbrt/detail/name_index_internal.h>
#include <tinypbrt/detail/object_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/shape_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region OBJECT

	tpbrt_error_t tpbrt_object_init(const tpbrt_string_t name, tpbrt_object_t* const object) {
			if (object == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(object, 0, sizeof(tpbrt_object_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		object->idx		  = TPBRT_OBJECT_IDX_INVALID;

		tpbrt_error_t err = tpbrt_shapes_list_init(&object->children);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_object_free(object);
				return err;
			}

		err = tpbrt_string_copy(&object->name, name);
			if (err != TPBRT_ERROR_NONE) { tpbrt_object_free(object); }

		return err;
	}

	tpbrt_error_t tpbrt_object_add_shape(tpbrt_object_t* const object, const tpbrt_shape_t* const shape) {
			if (object == TPBRT_NULL || shape == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		return tpbrt_shapes_list_append(&object->children, shape);
	}

	void tpbrt_object_free(tpbrt_object_t* const object) {
			if (object == TPBRT_NULL) { return; }

		tpbrt_string_free(&object->name);
		tpbrt_shapes_list_free(&object->children);
	}

#pragma endregion

#pragma region OBJECTS_LIST
#define TPBRT_EMPTY_OBJECTS_LIST (tpbrt_objects_list_t) { .objects = TPBRT_NULL, .count = 0 }

	tpbrt_error_t tpbrt_objects_list_init(tpbrt_objects_list_t* const objects_list) {
			if (objects_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*objects_list = TPBRT_EMPTY_OBJECTS_LIST;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_objects_list_append(tpbrt_objects_list_t* const objects_list, const tpbrt_object_t* const object) {
			if (object == TPBRT_NULL || objects_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_object_t* const new_list =
		  tpbrt_array_reserve_append(objects_list->objects, objects_list->count, sizeof(tpbrt_object_t));
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		const tpbrt_size_t idx = objects_list->count;
		new_list[idx]		   = *object;
		new_list[idx].idx	   = (tpbrt_idx_t)idx;
		objects_list->objects  = new_list;
		objects_list->count	   = idx + 1u;

		return tpbrt_name_index_insert(&objects_list->names, &(TPBRT_NAME_INDEX_VIEW(new_list, idx + 1u, tpbrt_object_t)),
		  (tpbrt_idx_t)idx);
	}

	tpbrt_error_t tpbrt_objects_list_get(const tpbrt_objects_list_t* const objects_list, const tpbrt_string_t object_name,
	  const tpbrt_object_t** const object) {
			if (object == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_idx_t idx = tpbrt_name_index_find(&objects_list->names,
		  &(TPBRT_NAME_INDEX_VIEW(objects_list->objects, objects_list->count, tpbrt_object_t)), object_name, TPBRT_NULL,
		  TPBRT_NULL);
			if (idx == TPBRT_IDX_INVALID) {
				*object = TPBRT_NULL;
				return TPBRT_ERROR_NOT_FOUND;
			}

		*object = &objects_list->objects[idx];
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_objects_list_shrink_to_fit(tpbrt_objects_list_t* const objects_list) {
			if (objects_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		objects_list->objects = tpbrt_array_shrink_to_fit(objects_list->objects, objects_list->count, sizeof(tpbrt_object_t));
		return tpbrt_name_index_shrink(&objects_list->names,
		  &(TPBRT_NAME_INDEX_VIEW(objects_list->objects, objects_list->count, tpbrt_object_t)));
	}

	tpbrt_error_t tpbrt_objects_list_get_handle(const tpbrt_objects_list_t* const objects_list, const tpbrt_string_t object_name,
	  tpbrt_object_handle_t* const object_handle) {
			if (object_handle == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_object_t* object;
		const tpbrt_error_t err = tpbrt_objects_list_get(objects_list, object_name, &object);

			if (err != TPBRT_ERROR_NONE) {
				*object_handle = TPBRT_OBJECT_HANDLE_INVALID;
				return err;
			}

		object_handle->object_idx = object->idx;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_bool_t tpbrt_objects_list_empty(const tpbrt_objects_list_t* const objects_list) {
		return objects_list->objects == TPBRT_NULL || objects_list->count == 0 ? TPBRT_TRUE : TPBRT_FALSE;
	}

	void tpbrt_objects_list_free(tpbrt_objects_list_t* const objects_list) {
			if (objects_list == TPBRT_NULL) { return; }

		tpbrt_name_index_free(&objects_list->names);

			if (objects_list->objects == TPBRT_NULL) { return; }

			for (tpbrt_size_t i = 0; i < objects_list->count; ++i) { tpbrt_object_free(objects_list->objects + i); }

		free(objects_list->objects);
		*objects_list = TPBRT_EMPTY_OBJECTS_LIST;
	}

#pragma endregion

#pragma region API

	tpbrt_error_t tpbrt_get_object_by_name(const tpbrt_objects_list_t* const objects, const tpbrt_string_t name,
	  const tpbrt_object_t** const object) {
		return tpbrt_objects_list_get(objects, name, object);
	}

	tpbrt_error_t tpbrt_get_object_by_handle(const tpbrt_objects_list_t* const objects, const tpbrt_object_handle_t handle,
	  const tpbrt_object_t** const object) {
			if (object == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (handle.object_idx == TPBRT_OBJECT_IDX_INVALID || handle.object_idx >= objects->count) {
				*object = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_HANDLE;
			}

		*object = objects->objects + handle.object_idx;
		return TPBRT_ERROR_NONE;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif
