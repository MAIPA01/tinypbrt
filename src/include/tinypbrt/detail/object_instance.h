#pragma once
#ifndef _TINYPBRT_OBJECT_INSTANCE_H_
#define _TINYPBRT_OBJECT_INSTANCE_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/math.h>
#include <tinypbrt/detail/object.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup object-instance-public Object Instance
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief PBRT object instance representation.
	 * @ingroup object-instance-public
	 */
	typedef struct tpbrt_object_instance_t {
		/// @brief Handle to object.
		tpbrt_object_handle_t object;
		/// @brief Animated transform.
		tpbrt_transform_animated_t transform;
	} tpbrt_object_instance_t;

	/**
	 * @brief List of object instances.
	 * @ingroup object-instance-public
	 */
	typedef struct tpbrt_objects_instances_list_t {
		/// @brief Objects instances.
		tpbrt_object_instance_t* instances;
		/// @brief Number of objects instances in list.
		tpbrt_size_t count;
	} tpbrt_objects_instances_list_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_OBJECT_INSTANCE_H_
