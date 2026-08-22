#include <tinypbrt/pch.h>

#include <tinypbrt/detail/coord_internal.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_coordinate_system_from_string(const tpbrt_string_t* const coord_str,
	  tpbrt_coordinate_system_t* const coord) {
		static const tpbrt_string_t CAMERA_WORLD_STR = TPBRT_STRING("cameraworld");
		static const tpbrt_string_t CAMERA_STR		 = TPBRT_STRING("camera");
		static const tpbrt_string_t WORLD_STR		 = TPBRT_STRING("world");

			if (coord_str == TPBRT_NULL || coord == TPBRT_NULL || coord_str->data == TPBRT_NULL || coord_str->size == 0) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (coord_str->size == CAMERA_WORLD_STR.size &&
				strncmp(coord_str->data, CAMERA_WORLD_STR.data, CAMERA_WORLD_STR.size) == 0) {
				*coord = TPBRT_COORDINATE_SYSTEM_CAMERA_WORLD;
				return TPBRT_ERROR_NONE;
			}

			if (coord_str->size == CAMERA_STR.size && strncmp(coord_str->data, CAMERA_STR.data, CAMERA_STR.size) == 0) {
				*coord = TPBRT_COORDINATE_SYSTEM_CAMERA;
				return TPBRT_ERROR_NONE;
			}

			if (coord_str->size == WORLD_STR.size && strncmp(coord_str->data, WORLD_STR.data, WORLD_STR.size) == 0) {
				*coord = TPBRT_COORDINATE_SYSTEM_WORLD;
				return TPBRT_ERROR_NONE;
			}

		return TPBRT_ERROR_UNKNOWN_COORDINATE_SYSTEM;
	}

#ifdef __cplusplus
}
#endif