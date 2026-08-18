#include <tinypbrt/pch.h>

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/coord_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_coordinate_system_from_string(const tpbrt_string_t* const coord_str,
	  tpbrt_coordinate_system_t* const coord) {
		static const tpbrt_string_t CAMERA_WORLD_STR = { .chars = "cameraworld", .size = 11 };
		static const tpbrt_string_t CAMERA_STR		 = { .chars = "camera", .size = 6 };
		static const tpbrt_string_t WORLD_STR		 = { .chars = "world", .size = 5 };

			if (coord_str == TPBRT_NULL || coord == TPBRT_NULL || coord_str->chars == TPBRT_NULL || coord_str->size == 0) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (coord_str->size == CAMERA_WORLD_STR.size &&
				strncmp(coord_str->chars, CAMERA_WORLD_STR.chars, CAMERA_WORLD_STR.size) == 0) {
				*coord = TPBRT_COORDINATE_SYSTEM_CAMERA_WORLD;
				return TPBRT_ERROR_NONE;
			}

			if (coord_str->size == CAMERA_STR.size && strncmp(coord_str->chars, CAMERA_STR.chars, CAMERA_STR.size) == 0) {
				*coord = TPBRT_COORDINATE_SYSTEM_CAMERA;
				return TPBRT_ERROR_NONE;
			}

			if (coord_str->size == WORLD_STR.size && strncmp(coord_str->chars, WORLD_STR.chars, WORLD_STR.size) == 0) {
				*coord = TPBRT_COORDINATE_SYSTEM_WORLD;
				return TPBRT_ERROR_NONE;
			}

		return TPBRT_ERROR_UNKNOWN_COORDINATE_SYSTEM;
	}

#ifdef __cplusplus
}
#endif