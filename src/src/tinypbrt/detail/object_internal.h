#pragma once
#ifndef _TINYPBRT_OBJECT_INTERNAL_H_
#define _TINYPBRT_OBJECT_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/object.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region OBJECT
	tpbrt_error_t tpbrt_object_init(tpbrt_string_t name, tpbrt_object_t* object);
	tpbrt_error_t tpbrt_object_add_shape(tpbrt_object_t* object, const tpbrt_shape_t* shape);
	void tpbrt_object_free(tpbrt_object_t* object);
#pragma endregion

#pragma region OBJECTS_LIST
	tpbrt_error_t tpbrt_objects_list_init(tpbrt_objects_list_t* objects_list);
	tpbrt_error_t tpbrt_objects_list_append(tpbrt_objects_list_t* objects_list, const tpbrt_object_t* object);
	tpbrt_error_t tpbrt_objects_list_get(tpbrt_objects_list_t objects_list, tpbrt_string_t object_name,
	  const tpbrt_object_t** object);
	tpbrt_error_t tpbrt_objects_list_get_handle(tpbrt_objects_list_t objects_list, tpbrt_string_t object_name,
	  tpbrt_object_handle_t* object_handle);
	tpbrt_bool_t tpbrt_objects_list_empty(tpbrt_objects_list_t objects_list);
	void tpbrt_objects_list_free(tpbrt_objects_list_t* objects_list);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_OBJECT_INTERNAL_H_
