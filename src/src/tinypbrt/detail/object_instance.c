#include <tinypbrt/pch.h>

#include <tinypbrt/detail/object_instance_internal.h>

#include <tinypbrt/detail/object_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region OBJECT_INSTANCE

	tpbrt_error_t tpbrt_object_instance_init(const tpbrt_string_t object_name, const tpbrt_objects_list_t objects,
	  const tpbrt_mat4_animated_t* const ctm, tpbrt_object_instance_t* const instance) {
			if (ctm == TPBRT_NULL || instance == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(instance, 0, sizeof(tpbrt_object_instance_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		instance->transform		= *ctm;

		const tpbrt_error_t err = tpbrt_objects_list_get_handle(objects, object_name, &instance->object);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_object_instance_free(instance);
				return err;
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_object_instance_free(tpbrt_object_instance_t* const instance) {
			if (instance == TPBRT_NULL) { return; }
		instance->object = TPBRT_OBJECT_HANDLE_INVALID;
	}

#pragma endregion

#pragma region OBJECT_INSTANCES_LIST
#define TPBRT_EMPTY_OBJECTS_INSTANCES_LIST (tpbrt_objects_instances_list_t) { .instances = TPBRT_NULL, .count = 0 }

	tpbrt_error_t tpbrt_objects_instances_list_init(tpbrt_objects_instances_list_t* const instances_list) {
			if (instances_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*instances_list = TPBRT_EMPTY_OBJECTS_INSTANCES_LIST;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_objects_instances_list_append(tpbrt_objects_instances_list_t* const instances_list,
	  const tpbrt_object_instance_t* const instance) {
			if (instances_list == TPBRT_NULL || instance == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_size_t new_count	  = instances_list->count + 1;

		tpbrt_object_instance_t* new_list = realloc(instances_list->instances, sizeof(tpbrt_object_instance_t) * new_count);
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		new_list[instances_list->count] = *instance;
		*instances_list					= (tpbrt_objects_instances_list_t) { .instances = new_list, .count = new_count };
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_objects_instances_list_free(tpbrt_objects_instances_list_t* const instances_list) {
			if (instances_list == TPBRT_NULL || instances_list->instances == TPBRT_NULL) { return; }

			for (tpbrt_size_t i = 0; i < instances_list->count; ++i) {
				tpbrt_object_instance_free(instances_list->instances + i);
			}

		free(instances_list->instances);
		*instances_list = TPBRT_EMPTY_OBJECTS_INSTANCES_LIST;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif