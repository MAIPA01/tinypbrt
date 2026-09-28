#pragma once
#ifndef _TINYPBRT_COORD_INTERNAL_H_
#define _TINYPBRT_COORD_INTERNAL_H_

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/coord.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/math.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_coord_system_type_from_string(tpbrt_string_t coord_str, tpbrt_coord_system_type_t* coord);

	typedef struct tpbrt_coord_system_map_entry_t {
		tpbrt_string_t name;
		tpbrt_mat4_animated_t transform;
	} tpbrt_coord_system_map_entry_t;

	typedef struct tpbrt_coord_system_map_t {
		tpbrt_coord_system_map_entry_t* entries;
		tpbrt_size_t count;
		tpbrt_size_t capacity;
	} tpbrt_coord_system_map_t;

	tpbrt_error_t tpbrt_coord_system_map_init(tpbrt_coord_system_map_t* map);
	tpbrt_error_t tpbrt_coord_system_map_insert(tpbrt_coord_system_map_t* map, tpbrt_string_t name,
	  const tpbrt_mat4_animated_t* transform);
	tpbrt_bool_t tpbrt_coord_system_map_get(const tpbrt_coord_system_map_t* map, tpbrt_string_t name,
	  tpbrt_mat4_animated_t* out_transform);
	void tpbrt_coord_system_map_free(tpbrt_coord_system_map_t* map);

#ifdef __cplusplus
}
#endif
#endif // TINYPBRT_COORD_INTERNAL_H
