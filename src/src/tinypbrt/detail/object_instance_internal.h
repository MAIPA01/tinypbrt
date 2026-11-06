#pragma once
#ifndef _TINYPBRT_OBJECT_INSTANCE_INTERNAL_H_
#define _TINYPBRT_OBJECT_INSTANCE_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/object_instance.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup object-instance-private Object Instance
	 * @ingroup tinypbrt-private
	 */

#pragma region OBJECT_INSTANCE

	/**
	 * @brief Initializes object instance data.
	 * @param object_name Object name.
	 * @param objects Objects list.
	 * @param transform Animated transform of object instance.
	 * @param instance Output object instance data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup object-instance-private
	 */
	tpbrt_error_t tpbrt_object_instance_init(tpbrt_string_t object_name, const tpbrt_objects_list_t* objects,
	  tpbrt_transform_animated_t transform, tpbrt_object_instance_t* instance);
	/**
	 * @brief Frees object instance data.
	 * @param instance Object instance data to free.
	 * @ingroup object-instance-private
	 */
	void tpbrt_object_instance_free(tpbrt_object_instance_t* instance);

#pragma endregion

#pragma region OBJECT_INSTANCES_LIST
	/**
	 * @brief Initializes objects instances list.
	 * @param instances_list Objects instance list.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup object-instance-private
	 */
	tpbrt_error_t tpbrt_objects_instances_list_init(tpbrt_objects_instances_list_t* instances_list);
	/**
	 * @brief Adds object instance to list.
	 * @param instances_list Objects instances list.
	 * @param instance Instance to add to list.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup object-instance-private
	 */
	tpbrt_error_t tpbrt_objects_instances_list_append(tpbrt_objects_instances_list_t* instances_list,
	  const tpbrt_object_instance_t* instance);
	/**
	 * @brief Shrinks objects instances list to minimum size.
	 * @param instances_list Objects instances list to shrink.
	 * @ingroup object-instance-private
	 */
	void tpbrt_objects_instances_list_shrink_to_fit(tpbrt_objects_instances_list_t* instances_list);
	/**
	 * @brief Frees objects instances list.
	 * @param instances_list Objects instances list to free.
	 * @ingroup object-instance-private
	 */
	void tpbrt_objects_instances_list_free(tpbrt_objects_instances_list_t* instances_list);

#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_OBJECT_INSTANCE_INTERNAL_H_
