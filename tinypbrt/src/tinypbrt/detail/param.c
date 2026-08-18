#include <tinypbrt/pch.h>

#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	static tpbrt_error_t tpbrt_param_type_from_string(const tpbrt_string_t* const type_str, tpbrt_param_type_t* const type_out) {
			if (type_str == TPBRT_NULL || type_out == TPBRT_NULL || type_str->chars == TPBRT_NULL || type_str->size == 0) {
				return TPBRT_ERROR_INVALID_PARAM_TYPE;
			}

			if (strncmp(type_str->chars, "integer", type_str->size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_INTEGER;
				return TPBRT_ERROR_NONE;
			}

			if (strncmp(type_str->chars, "float", type_str->size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_FLOAT;
				return TPBRT_ERROR_NONE;
			}

			if (strncmp(type_str->chars, "point2", type_str->size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_POINT2;
				return TPBRT_ERROR_NONE;
			}

			if (strncmp(type_str->chars, "vector2", type_str->size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_VECTOR2;
				return TPBRT_ERROR_NONE;
			}

			if (strncmp(type_str->chars, "point3", type_str->size) == 0 ||
				strncmp(type_str->chars, "point", type_str->size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_POINT3;
				return TPBRT_ERROR_NONE;
			}

			if (strncmp(type_str->chars, "vector3", type_str->size) == 0 ||
				strncmp(type_str->chars, "vector", type_str->size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_VECTOR3;
				return TPBRT_ERROR_NONE;
			}

			if (strncmp(type_str->chars, "normal3", type_str->size) == 0 ||
				strncmp(type_str->chars, "normal", type_str->size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_NORMAL3;
				return TPBRT_ERROR_NONE;
			}

			if (strncmp(type_str->chars, "spectrum", type_str->size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_SPECTRUM;
				return TPBRT_ERROR_NONE;
			}

			if (strncmp(type_str->chars, "rgb", type_str->size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_RGB;
				return TPBRT_ERROR_NONE;
			}

			if (strncmp(type_str->chars, "blackbody", type_str->size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_BLACKBODY;
				return TPBRT_ERROR_NONE;
			}

			if (strncmp(type_str->chars, "bool", type_str->size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_BOOL;
				return TPBRT_ERROR_NONE;
			}

			if (strncmp(type_str->chars, "string", type_str->size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_STRING;
				return TPBRT_ERROR_NONE;
			}

			if (strncmp(type_str->chars, "texture", type_str->size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_TEXTURE;
				return TPBRT_ERROR_NONE;
			}

		return TPBRT_ERROR_INVALID_PARAM_TYPE;
	}

	tpbrt_error_t tpbrt_create_param(const tpbrt_string_t* const declaration, const tpbrt_string_t* const value_str,
	  tpbrt_param_t** const param) {
			if (param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			// declaration -> "type name"
			if (declaration == TPBRT_NULL || declaration->chars == TPBRT_NULL || declaration->size == 0) {
				return TPBRT_ERROR_INVALID_PARAM_TYPE;
			}

		tpbrt_string_t type_str = { .chars = declaration->chars, .size = 0 };
			for (tpbrt_size_t i = 0; i < declaration->size; i++) {
					if (isspace(declaration->chars[i])) {
						type_str.size = i;
						break;
					}
			}
			if (type_str.size == 0) { return TPBRT_ERROR_INVALID_PARAM_TYPE; }

		tpbrt_param_type_t param_type;
		const tpbrt_error_t error = tpbrt_param_type_from_string(&type_str, &param_type);
			if (error != TPBRT_ERROR_NONE) { return error; }

		tpbrt_string_t name_str = { .chars = TPBRT_NULL, .size = 0 };
			for (tpbrt_size_t i = type_str.size; i < declaration->size; i++) {
					if (!isspace(declaration->chars[i])) {
						name_str.chars = declaration->chars + i;
						name_str.size  = declaration->size - i;
						break;
					}
			}
			if (name_str.chars == TPBRT_NULL || name_str.size == 0) { return TPBRT_ERROR_INVALID_PARAM_NAME; }

		*param = malloc(sizeof(tpbrt_param_t));
			if (param == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		(*param)->name	= name_str;
		(*param)->type	= param_type;
		(*param)->value = *value_str;
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_param(tpbrt_param_t** param) {
			if (param == TPBRT_NULL || *param == TPBRT_NULL) { return; }

		free(*param);
		param = TPBRT_NULL;
	}

	tpbrt_error_t tpbrt_create_empty_params_list(tpbrt_params_list_t** const params_list) {
			if (params_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*params_list = malloc(sizeof(tpbrt_params_list_t));
			if (*params_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		(*params_list)->params = TPBRT_NULL;
		(*params_list)->count  = 0;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_add_param(tpbrt_params_list_t* const params_list, const tpbrt_param_t* const param) {
			if (param == TPBRT_NULL || params_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (params_list->params == TPBRT_NULL) {
				params_list->params = malloc(sizeof(tpbrt_param_t));
					if (params_list->params == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

				params_list->params[0] = *param;
				params_list->count	   = 1;
				return TPBRT_ERROR_NONE;
			}

			for (tpbrt_size_t i = 0; i < params_list->count; i++) {
					if (strncmp(params_list->params[i].name.chars, param->name.chars,
						  min(params_list->params[i].name.size, param->name.size)) == 0) {
						return TPBRT_ERROR_DUPLICATE_PARAM_NAME;
					}
			}

		tpbrt_param_t* new_list = malloc(sizeof(tpbrt_param_t) * (params_list->count + 1));
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

			for (tpbrt_size_t i = 0; i < params_list->count; i++) { new_list[i] = params_list->params[i]; }
		new_list[params_list->count++] = *param;
		free(params_list->params);
		params_list->params = new_list;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_get_param(const tpbrt_params_list_t* const params_list, const tpbrt_string_t* const param_name,
	  const tpbrt_param_t** const param) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || param_name->chars == TPBRT_NULL ||
				param_name->size == 0 || param == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_size_t i = 0; i < params_list->count; i++) {
					if (strncmp(params_list->params[i].name.chars, param_name->chars,
						  min(params_list->params[i].name.size, param_name->size)) == 0) {
						*param = &params_list->params[i];
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_NOT_FOUND;
	}

	void tpbrt_free_params_list(tpbrt_params_list_t** params_list) {
			if (params_list == TPBRT_NULL || *params_list == TPBRT_NULL) { return; }

		free((*params_list)->params);
		free(*params_list);
		params_list = TPBRT_NULL;
	}

#ifdef __cplusplus
}
#endif