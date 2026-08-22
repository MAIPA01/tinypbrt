#pragma once
#ifndef _TINYPBRT_SHAPE_INTERNAL_H_
#define _TINYPBRT_SHAPE_INTERNAL_H_

#include <tinypbrt/detail/shape.h>

#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region SHAPE
	tpbrt_error_t tpbrt_create_shape(const tpbrt_string_t* type_str, const tpbrt_params_list_t* params,
	  const tpbrt_textures_list_t* textures, const tpbrt_mat4_t* ctm, const tpbrt_material_handle_t* material,
	  tpbrt_shape_t* shape);
	void tpbrt_free_shape(tpbrt_shape_t* shape);
#pragma endregion

#pragma region SHAPE_ARRAY
	tpbrt_error_t tpbrt_init_shape_array(tpbrt_shape_array_t* shape_array);
	tpbrt_error_t tpbrt_shape_array_add_shape(tpbrt_shape_array_t* shape_array, const tpbrt_shape_t* shape);
	void tpbrt_free_shape_array(tpbrt_shape_array_t* shape_array);
#pragma endregion

#pragma region OBJECT
	tpbrt_error_t tpbrt_create_object(const tpbrt_string_t* name, tpbrt_object_t* object);
	tpbrt_error_t tpbrt_object_add_shape(tpbrt_object_t* object, const tpbrt_shape_t* shape);
	void tpbrt_free_object(tpbrt_object_t* object);
#pragma endregion

#pragma region OBJECTS_LIST
	tpbrt_error_t tpbrt_init_objects_list(tpbrt_objects_list_t* objects_list);
	tpbrt_error_t tpbrt_objects_list_add_object(tpbrt_objects_list_t* objects_list, const tpbrt_object_t* object);
	tpbrt_error_t tpbrt_objects_list_get_object(const tpbrt_objects_list_t* objects_list, const tpbrt_string_t* object_name,
	  const tpbrt_object_t** object);
	tpbrt_error_t tpbrt_objects_list_get_object_handle(const tpbrt_objects_list_t* objects_list,
	  const tpbrt_string_t* object_name, tpbrt_object_handle_t* object_handle);
	void tpbrt_free_objects_list(tpbrt_objects_list_t* objects_list);

	tpbrt_size_t tpbrt_objects_list_size(const tpbrt_objects_list_t* objects_list);
	tpbrt_bool_t tpbrt_objects_list_is_empty(const tpbrt_objects_list_t* objects_list);
#pragma endregion

#pragma region OBJECT_INSTANCE
	tpbrt_error_t tpbrt_create_instance(const tpbrt_string_t* object_name, const tpbrt_objects_list_t* objects,
	  const tpbrt_mat4_t* ctm, tpbrt_instance_t* instance);
	void tpbrt_free_instance(tpbrt_instance_t* instance);
#pragma endregion

#pragma region OBJECT_INSTANCES_LIST
	tpbrt_error_t tpbrt_init_instances_list(tpbrt_instances_list_t* instances_list);
	tpbrt_error_t tpbrt_instances_list_add_instance(tpbrt_instances_list_t* instances_list, const tpbrt_instance_t* instance);
	void tpbrt_free_instances_list(tpbrt_instances_list_t* instances_list);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SHAPE_INTERNAL_H_
