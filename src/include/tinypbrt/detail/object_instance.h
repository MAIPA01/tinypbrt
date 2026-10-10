#pragma once
#ifndef _TINYPBRT_OBJECT_INSTANCE_H_
#define _TINYPBRT_OBJECT_INSTANCE_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/math.h>
#include <tinypbrt/detail/object.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct tpbrt_object_instance_t {
		tpbrt_object_handle_t object;
		tpbrt_mat4_animated_t transform;
	} tpbrt_object_instance_t;

	typedef struct tpbrt_objects_instances_list_t {
		tpbrt_object_instance_t* instances;
		tpbrt_size_t count;
	} tpbrt_objects_instances_list_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_OBJECT_INSTANCE_H_
