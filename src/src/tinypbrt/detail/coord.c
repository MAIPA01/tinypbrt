#include <tinypbrt/pch.h>

#include <tinypbrt/detail/coord_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TPBRT_EMPTY_COORD_SYSTEM_MAP (tpbrt_coord_system_map_t) { .entries = TPBRT_NULL, .count = 0, .capacity = 0 }

	tpbrt_error_t tpbrt_coord_system_type_from_string(const tpbrt_string_t coord_str, tpbrt_coord_system_type_t* const coord) {
		static const tpbrt_string_t COORDS_STRS[TPBRT_COORD_SYSTEM_TYPE_MAX_NUM] = {
			TPBRT_STRING("cameraworld"),
			TPBRT_STRING("camera"),
			TPBRT_STRING("world"),
		};

			if (coord == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(coord_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_COORD_SYSTEM_TYPE; }

			for (tpbrt_coord_system_type_t c = 0; c < TPBRT_COORD_SYSTEM_TYPE_MAX_NUM; ++c) {
					if (tpbrt_string_equals(coord_str, COORDS_STRS[c]) == TPBRT_TRUE) {
						*coord = c;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_COORD_SYSTEM_TYPE;
	}

	tpbrt_error_t tpbrt_coord_system_map_init(tpbrt_coord_system_map_t* const map) {
			if (map == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*map = TPBRT_EMPTY_COORD_SYSTEM_MAP;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_coord_system_map_insert(tpbrt_coord_system_map_t* const map, const tpbrt_string_t name,
	  const tpbrt_mat4_animated_t* const transform) {
		const tpbrt_coord_system_map_entry_t entry = (tpbrt_coord_system_map_entry_t) { .name = name, .transform = *transform };

			for (tpbrt_size_t i = 0; i < map->count; ++i) {
					if (tpbrt_string_equals(map->entries[i].name, name) == TPBRT_TRUE) {
						map->entries[i].transform = *transform;
						return TPBRT_ERROR_NONE;
					}
			}

			if (map->count >= map->capacity) {
				static const tpbrt_uint_t DEFAULT_MIN_CAPACITY = 8u;
				map->capacity								   = map->capacity == 0 ? DEFAULT_MIN_CAPACITY : map->capacity * 2u;
				map->entries = realloc(map->entries, map->capacity * sizeof(tpbrt_coord_system_map_entry_t));
			}
		map->entries[map->count++] = entry;

		return TPBRT_ERROR_NONE;
	}

	tpbrt_bool_t tpbrt_coord_system_map_get(const tpbrt_coord_system_map_t* const map, const tpbrt_string_t name,
	  tpbrt_mat4_animated_t* const out_transform) {
			for (tpbrt_size_t i = 0; i < map->count; i++) {
					if (tpbrt_string_equals(map->entries[i].name, name)) {
						*out_transform = map->entries[i].transform;
						return TPBRT_TRUE;
					}
			}
		return TPBRT_FALSE;
	}

	void tpbrt_coord_system_map_free(tpbrt_coord_system_map_t* const map) {
			if (map == TPBRT_NULL) { return; }

			if (map->entries != TPBRT_NULL) { free(map->entries); }
		*map = TPBRT_EMPTY_COORD_SYSTEM_MAP;
	}

#ifdef __cplusplus
}
#endif