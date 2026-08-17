#include <tinypbrt/pch.h>

#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_param_type_t_from_string(const tpbrt_string_t* type_str, tpbrt_param_type_t* type_out) {
			if (strstr(type_str->chars, "integer") != TPBRT_NULL) {
				*type_out = TPBRT_PARAM_TYPE_INTEGER;
				return TPBRT_ERROR_NONE;
			}

			if (strstr(type_str->chars, "float") != TPBRT_NULL) {
				*type_out = TPBRT_PARAM_TYPE_FLOAT;
				return TPBRT_ERROR_NONE;
			}

			if (strstr(type_str->chars, "point2") != TPBRT_NULL) {
				*type_out = TPBRT_PARAM_TYPE_POINT2;
				return TPBRT_ERROR_NONE;
			}

			if (strstr(type_str->chars, "vector2") != TPBRT_NULL) {
				*type_out = TPBRT_PARAM_TYPE_VECTOR2;
				return TPBRT_ERROR_NONE;
			}

			if (strstr(type_str->chars, "point3") != TPBRT_NULL || strstr(type_str->chars, "point") != TPBRT_NULL) {
				*type_out = TPBRT_PARAM_TYPE_POINT3;
				return TPBRT_ERROR_NONE;
			}

			if (strstr(type_str->chars, "vector3") != TPBRT_NULL || strstr(type_str->chars, "vector") != TPBRT_NULL) {
				*type_out = TPBRT_PARAM_TYPE_VECTOR3;
				return TPBRT_ERROR_NONE;
			}

			if (strstr(type_str->chars, "normal3") != TPBRT_NULL || strstr(type_str->chars, "normal") != TPBRT_NULL) {
				*type_out = TPBRT_PARAM_TYPE_NORMAL3;
				return TPBRT_ERROR_NONE;
			}

			if (strstr(type_str->chars, "spectrum") != TPBRT_NULL) {
				*type_out = TPBRT_PARAM_TYPE_SPECTRUM;
				return TPBRT_ERROR_NONE;
			}

			if (strstr(type_str->chars, "rgb") != TPBRT_NULL) {
				*type_out = TPBRT_PARAM_TYPE_RGB;
				return TPBRT_ERROR_NONE;
			}

			if (strstr(type_str->chars, "blackbody") != TPBRT_NULL) {
				*type_out = TPBRT_PARAM_TYPE_BLACKBODY;
				return TPBRT_ERROR_NONE;
			}

			if (strstr(type_str->chars, "bool") != TPBRT_NULL) {
				*type_out = TPBRT_PARAM_TYPE_BOOL;
				return TPBRT_ERROR_NONE;
			}

			if (strstr(type_str->chars, "string") != TPBRT_NULL) {
				*type_out = TPBRT_PARAM_TYPE_STRING;
				return TPBRT_ERROR_NONE;
			}

			if (strstr(type_str->chars, "texture") != TPBRT_NULL) {
				*type_out = TPBRT_PARAM_TYPE_TEXTURE;
				return TPBRT_ERROR_NONE;
			}

		return TPBRT_ERROR_INVALID_PARAM_TYPE;
	}

	tpbrt_error_t tpbrt_fill_param(const tpbrt_string_t* type_and_name, const tpbrt_string_t* value) {
		// "type name"
		if (type_and_name == NULL) {
			return TPBRT_ERROR_INVALID_PARAM_NAME;
		}
	}

#ifdef __cplusplus
}
#endif