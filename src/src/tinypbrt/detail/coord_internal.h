#pragma once
#ifndef _TINYPBRT_COORD_INTERNAL_H_
#define _TINYPBRT_COORD_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/coord.h>

#include <tinypbrt/detail/error.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup coord-private Coordinates
	 * @ingroup tinypbrt-private
	 */

	/**
	 * @brief Returns coordinate system type based on string value.
	 * @param coord_str String which is convertible to coordinate system type.
	 * @param coord Output coordinate system type value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_COORD_SYSTEM_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup coord-private
	 */
	tpbrt_error_t tpbrt_coord_system_type_from_string(tpbrt_string_t coord_str, tpbrt_coord_system_type_t* coord);

	/**
	 * @brief Coordinate system map entry data.
	 * @ingroup coord-private
	 */
	typedef struct tpbrt_coord_system_map_entry_t {
		/// @brief Entry name.
		tpbrt_string_t name;
		/// @brief Animated transform.
		tpbrt_mat4_animated_t transform;
	} tpbrt_coord_system_map_entry_t;

	/**
	 * @brief Coordinate system map.
	 * @ingroup coord-private
	 */
	typedef struct tpbrt_coord_system_map_t {
		/// @brief Entries.
		tpbrt_coord_system_map_entry_t* entries;
		/// @brief Number of entries.
		tpbrt_size_t count;
		/// @brief Number of possible entries in current allocated memory.
		tpbrt_size_t capacity;
	} tpbrt_coord_system_map_t;

	/**
	 * @brief Initializes map structure.
	 * @param map Map data structure.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup coord-private
	 */
	tpbrt_error_t tpbrt_coord_system_map_init(tpbrt_coord_system_map_t* map);
	/**
	 * @brief Inserts new entry to map.
	 * @param map Map data structure.
	 * @param name Coordinate system name.
	 * @param transform Coordinate system animated transform.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup coord-private
	 */
	tpbrt_error_t tpbrt_coord_system_map_insert(tpbrt_coord_system_map_t* map, tpbrt_string_t name,
	  const tpbrt_mat4_animated_t* transform);
	/**
	 * @brief Returns entry from map with given name if exists.
	 * @param map Map data structure.
	 * @param name Name of the entry to return.
	 * @param out_transform Animated transform where transform from entry will be written.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup coord-private
	 */
	tpbrt_error_t tpbrt_coord_system_map_get(const tpbrt_coord_system_map_t* map, tpbrt_string_t name,
	  tpbrt_mat4_animated_t* out_transform);
	/**
	 * @brief Frees all map resources.
	 * @param map Map data structure.
	 * @ingroup coord-private
	 */
	void tpbrt_coord_system_map_free(tpbrt_coord_system_map_t* map);

#ifdef __cplusplus
}
#endif
#endif // TINYPBRT_COORD_INTERNAL_H
