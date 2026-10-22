#pragma once
#ifndef _TINYPBRT_OBJECT_H_
#define _TINYPBRT_OBJECT_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/shape.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct tpbrt_object_t {
		tpbrt_object_idx_t idx;
		tpbrt_string_t name;
		tpbrt_shapes_list_t children;
	} tpbrt_object_t;

	typedef struct tpbrt_objects_list_t {
		tpbrt_object_t* objects;
		tpbrt_size_t count;
	} tpbrt_objects_list_t;

	typedef struct tpbrt_object_handle_t {
		tpbrt_object_idx_t object_idx;
	} tpbrt_object_handle_t;

#define TPBRT_OBJECT_HANDLE_INVALID                                    \
	(tpbrt_object_handle_t) { .object_idx = TPBRT_OBJECT_IDX_INVALID }

	TPBRT_API tpbrt_error_t tpbrt_get_object_by_name(tpbrt_objects_list_t objects, tpbrt_string_t name, const tpbrt_object_t** object);
	TPBRT_API tpbrt_error_t tpbrt_get_object_by_handle(tpbrt_objects_list_t objects, tpbrt_object_handle_t handle,
	  const tpbrt_object_t** object);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_OBJECT_H_
