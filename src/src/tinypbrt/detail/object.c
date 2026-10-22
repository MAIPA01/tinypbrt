#include <tinypbrt/pch.h>

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

		const tpbrt_size_t new_count = objects_list->count + 1;

		tpbrt_object_t* new_list	 = realloc(objects_list->objects, sizeof(tpbrt_object_t) * new_count);
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		new_list[objects_list->count]	  = *object;
		new_list[objects_list->count].idx = objects_list->count;
		*objects_list					  = (tpbrt_objects_list_t) { .objects = new_list, .count = new_count };
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_objects_list_get(const tpbrt_objects_list_t objects_list, const tpbrt_string_t object_name,
	  const tpbrt_object_t** const object) {
			if (object == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			for (tpbrt_size_t i = 0; i < objects_list.count; i++) {
					if (tpbrt_string_equals(objects_list.objects[i].name, object_name)) {
						*object = &objects_list.objects[i];
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_NOT_FOUND;
	}

	tpbrt_error_t tpbrt_objects_list_get_handle(const tpbrt_objects_list_t objects_list, const tpbrt_string_t object_name,
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

	tpbrt_bool_t tpbrt_objects_list_empty(const tpbrt_objects_list_t objects_list) {
		return objects_list.objects == TPBRT_NULL || objects_list.count == 0 ? TPBRT_TRUE : TPBRT_FALSE;
	}

	void tpbrt_objects_list_free(tpbrt_objects_list_t* const objects_list) {
			if (objects_list == TPBRT_NULL || objects_list->objects == TPBRT_NULL) { return; }

			for (tpbrt_size_t i = 0; i < objects_list->count; ++i) { tpbrt_object_free(objects_list->objects + i); }

		free(objects_list->objects);
		*objects_list = TPBRT_EMPTY_OBJECTS_LIST;
	}

#pragma endregion

#pragma region API

	tpbrt_error_t tpbrt_get_object_by_name(const tpbrt_objects_list_t objects, const tpbrt_string_t name,
	  const tpbrt_object_t** const object) {
		return tpbrt_objects_list_get(objects, name, object);
	}

	tpbrt_error_t tpbrt_get_object_by_handle(const tpbrt_objects_list_t objects, const tpbrt_object_handle_t handle,
	  const tpbrt_object_t** const object) {
			if (object == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (handle.object_idx == TPBRT_OBJECT_IDX_INVALID || handle.object_idx >= objects.count) {
				*object = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_HANDLE;
			}

		*object = objects.objects + handle.object_idx;
		return TPBRT_ERROR_NONE;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif
