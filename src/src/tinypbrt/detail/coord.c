#include <tinypbrt/pch.h>

#include <tinypbrt/detail/coord_internal.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_coordinate_system_from_string(const tpbrt_string_t* const coord_str,
	  tpbrt_coordinate_system_t* const coord) {
		static const tpbrt_string_t COORDS_STRS[TPBRT_COORDINATE_SYSTEM_MAX_NUM] = {
			TPBRT_STRING("cameraworld"),
			TPBRT_STRING("camera"),
			TPBRT_STRING("world"),
		};

			if (coord_str == TPBRT_NULL || coord == TPBRT_NULL || coord_str->data == TPBRT_NULL || coord_str->size == 0) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_coordinate_system_t c = 0; c < TPBRT_COORDINATE_SYSTEM_MAX_NUM; ++c) {
					if (tpbrt_string_equals(coord_str, COORDS_STRS + c)) {
						*coord = c;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_COORDINATE_SYSTEM;
	}

#ifdef __cplusplus
}
#endif