#pragma once
#ifndef _TINYPBRT_OBJECT_INSTANCE_INTERNAL_H_
#define _TINYPBRT_OBJECT_INSTANCE_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/object_instance.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region OBJECT_INSTANCE

	tpbrt_error_t tpbrt_object_instance_init(tpbrt_string_t object_name, tpbrt_objects_list_t objects,
	  const tpbrt_mat4_animated_t* ctm, tpbrt_object_instance_t* instance);
	void tpbrt_object_instance_free(tpbrt_object_instance_t* instance);

#pragma endregion

#pragma region OBJECT_INSTANCES_LIST
	tpbrt_error_t tpbrt_objects_instances_list_init(tpbrt_objects_instances_list_t* instances_list);
	tpbrt_error_t tpbrt_objects_instances_list_append(tpbrt_objects_instances_list_t* instances_list,
	  const tpbrt_object_instance_t* instance);
	void tpbrt_objects_instances_list_free(tpbrt_objects_instances_list_t* instances_list);

#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_OBJECT_INSTANCE_INTERNAL_H_
