#include <tinypbrt/pch.h>

#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region PARSING

	static const tpbrt_char_t* tpbrt_next_token(const tpbrt_char_t* current, const tpbrt_char_t* const end,
	  tpbrt_size_t* const out_len) {
			if (current == TPBRT_NULL || end == TPBRT_NULL || out_len == TPBRT_NULL) { return TPBRT_NULL; }

			while (current < end && isspace((tpbrt_uint_t)*current)) { ++current; }
			if (current >= end) { return TPBRT_NULL; }

		const tpbrt_char_t* start = current;
			while (current < end && !isspace((tpbrt_uint_t)*current)) { ++current; }

		*out_len = (tpbrt_size_t)(current - start);
		return start;
	}

	static tpbrt_bool_t tpbrt_parse_float_token(const tpbrt_char_t* const token, const tpbrt_size_t len,
	  tpbrt_float_t* const out_val) {
		static const tpbrt_size_t MAX_BUFFER_SIZE = 64;

			if (token == TPBRT_NULL || out_val == TPBRT_NULL) { return TPBRT_FALSE; }

		tpbrt_char_t buf[MAX_BUFFER_SIZE];
		const tpbrt_size_t n = len < MAX_BUFFER_SIZE - 1 ? len : MAX_BUFFER_SIZE - 1;
		strncpy_s(buf, sizeof(tpbrt_size_t) * MAX_BUFFER_SIZE, token, n);
		buf[n] = '\0';
		tpbrt_char_t* end;
		*out_val = strtof(buf, &end);
		return end != buf;
	}

	static tpbrt_bool_t tpbrt_parse_int_token(const tpbrt_char_t* token, const tpbrt_size_t len, tpbrt_int_t* const out_val) {
		static const tpbrt_size_t MAX_BUFFER_SIZE = 64;
		static const tpbrt_int_t INT_RANIX		  = 10;

			if (token == TPBRT_NULL || out_val == TPBRT_NULL) { return TPBRT_FALSE; }

		tpbrt_char_t buf[MAX_BUFFER_SIZE];
		const tpbrt_size_t n = len < MAX_BUFFER_SIZE - 1 ? len : MAX_BUFFER_SIZE - 1;
		strncpy_s(buf, sizeof(tpbrt_char_t) * MAX_BUFFER_SIZE, token, n);
		buf[n] = '\0';
		tpbrt_char_t* end;
		*out_val = (tpbrt_int_t)strtol(buf, &end, INT_RANIX);
		return end != buf;
	}

	static tpbrt_bool_t tpbrt_parse_uint_token(const tpbrt_char_t* token, const tpbrt_size_t len, tpbrt_uint_t* const out_val) {
		static const tpbrt_size_t MAX_BUFFER_SIZE = 64;
		static const tpbrt_int_t UINT_RANIX		  = 10;

			if (token == TPBRT_NULL || out_val == TPBRT_NULL) { return TPBRT_FALSE; }

		tpbrt_char_t buf[MAX_BUFFER_SIZE];
		const tpbrt_size_t n = len < MAX_BUFFER_SIZE - 1 ? len : MAX_BUFFER_SIZE - 1;
		strncpy_s(buf, sizeof(tpbrt_char_t) * MAX_BUFFER_SIZE, token, n);
		buf[n] = '\0';
		tpbrt_char_t* end;
		*out_val = (tpbrt_uint_t)strtol(buf, &end, UINT_RANIX);
		return end != buf;
	}

#pragma endregion

#pragma region PARAM

	static tpbrt_error_t tpbrt_param_type_from_string(const tpbrt_string_t* const type_str, tpbrt_param_type_t* const type_out) {
		static const tpbrt_string_t INTEGER_STR	  = { .chars = "integer", .size = 7 };
		static const tpbrt_string_t FLOAT_STR	  = { .chars = "float", .size = 5 };
		static const tpbrt_string_t POINT2_STR	  = { .chars = "point2", .size = 6 };
		static const tpbrt_string_t VECTOR2_STR	  = { .chars = "vector2", .size = 7 };
		static const tpbrt_string_t POINT3_STR	  = { .chars = "point3", .size = 6 };
		static const tpbrt_string_t POINT_STR	  = { .chars = "point", .size = 5 };
		static const tpbrt_string_t VECTOR3_STR	  = { .chars = "vector3", .size = 7 };
		static const tpbrt_string_t VECTOR_STR	  = { .chars = "vector", .size = 6 };
		static const tpbrt_string_t NORMAL3_STR	  = { .chars = "normal3", .size = 7 };
		static const tpbrt_string_t NORMAL_STR	  = { .chars = "normal", .size = 6 };
		static const tpbrt_string_t SPECTRUM_STR  = { .chars = "spectrum", .size = 8 };
		static const tpbrt_string_t RGB_STR		  = { .chars = "rgb", .size = 3 };
		static const tpbrt_string_t BLACKBODY_STR = { .chars = "blackbody", .size = 9 };
		static const tpbrt_string_t BOOL_STR	  = { .chars = "bool", .size = 4 };
		static const tpbrt_string_t STRING_STR	  = { .chars = "string", .size = 6 };
		static const tpbrt_string_t TEXTURE_STR	  = { .chars = "texture", .size = 7 };

			if (type_str == TPBRT_NULL || type_out == TPBRT_NULL || type_str->chars == TPBRT_NULL || type_str->size == 0) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (type_str->size == INTEGER_STR.size && strncmp(type_str->chars, INTEGER_STR.chars, INTEGER_STR.size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_INTEGER;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == FLOAT_STR.size && strncmp(type_str->chars, FLOAT_STR.chars, FLOAT_STR.size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_FLOAT;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == POINT2_STR.size && strncmp(type_str->chars, POINT2_STR.chars, POINT2_STR.size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_POINT2;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == VECTOR2_STR.size && strncmp(type_str->chars, VECTOR2_STR.chars, VECTOR2_STR.size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_VECTOR2;
				return TPBRT_ERROR_NONE;
			}

			if ((type_str->size == POINT3_STR.size && strncmp(type_str->chars, POINT3_STR.chars, POINT3_STR.size) == 0) ||
				(type_str->size == POINT_STR.size && strncmp(type_str->chars, POINT_STR.chars, POINT_STR.size) == 0)) {
				*type_out = TPBRT_PARAM_TYPE_POINT3;
				return TPBRT_ERROR_NONE;
			}

			if ((type_str->size == VECTOR3_STR.size && strncmp(type_str->chars, VECTOR3_STR.chars, VECTOR3_STR.size) == 0) ||
				(type_str->size == VECTOR_STR.size && strncmp(type_str->chars, VECTOR_STR.chars, VECTOR_STR.size) == 0)) {
				*type_out = TPBRT_PARAM_TYPE_VECTOR3;
				return TPBRT_ERROR_NONE;
			}

			if ((type_str->size == NORMAL3_STR.size && strncmp(type_str->chars, NORMAL3_STR.chars, NORMAL3_STR.size) == 0) ||
				(type_str->size == NORMAL_STR.size && strncmp(type_str->chars, NORMAL_STR.chars, NORMAL_STR.size) == 0)) {
				*type_out = TPBRT_PARAM_TYPE_NORMAL3;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == SPECTRUM_STR.size && strncmp(type_str->chars, SPECTRUM_STR.chars, SPECTRUM_STR.size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_SPECTRUM;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == RGB_STR.size && strncmp(type_str->chars, RGB_STR.chars, RGB_STR.size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_RGB;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == BLACKBODY_STR.size && strncmp(type_str->chars, BLACKBODY_STR.chars, BLACKBODY_STR.size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_BLACKBODY;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == BOOL_STR.size && strncmp(type_str->chars, BOOL_STR.chars, BOOL_STR.size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_BOOL;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == STRING_STR.size && strncmp(type_str->chars, STRING_STR.chars, STRING_STR.size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_STRING;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == TEXTURE_STR.size && strncmp(type_str->chars, TEXTURE_STR.chars, TEXTURE_STR.size) == 0) {
				*type_out = TPBRT_PARAM_TYPE_TEXTURE;
				return TPBRT_ERROR_NONE;
			}

		return TPBRT_ERROR_INVALID_PARAM_TYPE;
	}

	tpbrt_error_t tpbrt_create_param(const tpbrt_string_t* const declaration, const tpbrt_param_value_type_t value_type,
	  const tpbrt_string_t* const value_str, tpbrt_param_t** const param) {
			if (param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			// declaration -> "type name"
			if (declaration == TPBRT_NULL || declaration->chars == TPBRT_NULL || declaration->size == 0) {
				return TPBRT_ERROR_INVALID_PARAM_TYPE;
			}

		const tpbrt_char_t* curr = declaration->chars;
		const tpbrt_char_t* end	 = declaration->chars + declaration->size;
		tpbrt_size_t t_len;

		const tpbrt_string_t type_str = { .chars = (tpbrt_char_t*)tpbrt_next_token(curr, end, &t_len), .size = t_len };
			if (type_str.chars == TPBRT_NULL || type_str.size == 0) { return TPBRT_ERROR_INVALID_PARAM_TYPE; }
		curr += t_len;

		tpbrt_param_type_t param_type;
		const tpbrt_error_t error = tpbrt_param_type_from_string(&type_str, &param_type);
			if (error != TPBRT_ERROR_NONE) { return error; }

		const tpbrt_string_t name_str = { .chars = (tpbrt_char_t*)tpbrt_next_token(curr, end, &t_len), .size = t_len };
			if (name_str.chars == TPBRT_NULL || name_str.size == 0) { return TPBRT_ERROR_INVALID_PARAM_NAME; }

		*param = malloc(sizeof(tpbrt_param_t));
			if (param == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		(*param)->name		 = name_str;
		(*param)->type		 = param_type;
		(*param)->value_type = value_type;
		(*param)->value		 = *value_str;
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_param(tpbrt_param_t** const param) {
			if (param == TPBRT_NULL || *param == TPBRT_NULL) { return; }

		free(*param);
		*param = TPBRT_NULL;
	}

	tpbrt_error_t tpbrt_param_as_floats(const tpbrt_param_t* const param, tpbrt_float_array_t* const out_floats) {
			if (param == TPBRT_NULL || out_floats == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		 = 0;
		const tpbrt_char_t* curr = param->value.chars;
		const tpbrt_char_t* end	 = curr + param->value.size;
		tpbrt_size_t t_len;

			while ((curr = tpbrt_next_token(curr, end, &t_len)) != TPBRT_NULL) {
				++count;
				curr += t_len;
			}

			if (count == 0) {
				out_floats->values = TPBRT_NULL;
				out_floats->count  = 0;
				return TPBRT_ERROR_NONE;
			}

		out_floats->values = malloc(sizeof(tpbrt_float_t) * count);
			if (out_floats->values == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		curr			 = param->value.chars;
		tpbrt_size_t idx = 0;
			while ((curr = tpbrt_next_token(curr, end, &t_len)) != TPBRT_NULL) {
					if (!tpbrt_parse_float_token(curr, t_len, &out_floats->values[idx])) {
						free(out_floats->values);
						out_floats->values = TPBRT_NULL;
						out_floats->count  = 0;
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				++idx;
				curr += t_len;
			}

		out_floats->count = count;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_ints(const tpbrt_param_t* const param, tpbrt_int_array_t* const out_ints) {
			if (param == TPBRT_NULL || out_ints == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		 = 0;
		const tpbrt_char_t* curr = param->value.chars;
		const tpbrt_char_t* end	 = curr + param->value.size;
		tpbrt_size_t t_len;

			while ((curr = tpbrt_next_token(curr, end, &t_len)) != TPBRT_NULL) {
				++count;
				curr += t_len;
			}

			if (count == 0) {
				out_ints->values = TPBRT_NULL;
				out_ints->count	 = 0;
				return TPBRT_ERROR_NONE;
			}

		out_ints->values = malloc(sizeof(tpbrt_int_t) * count);
			if (out_ints->values == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		curr			 = param->value.chars;
		tpbrt_size_t idx = 0;
			while ((curr = tpbrt_next_token(curr, end, &t_len)) != TPBRT_NULL) {
					if (!tpbrt_parse_int_token(curr, t_len, &out_ints->values[idx])) {
						free(out_ints->values);
						out_ints->values = TPBRT_NULL;
						out_ints->count	 = 0;
						return TPBRT_ERROR_PARSE_INT;
					}
				++idx;
				curr += t_len;
			}

		out_ints->count = count;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_uints(const tpbrt_param_t* const param, tpbrt_uint_array_t* const out_uints) {
			if (param == TPBRT_NULL || out_uints == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		 = 0;
		const tpbrt_char_t* curr = param->value.chars;
		const tpbrt_char_t* end	 = curr + param->value.size;
		tpbrt_size_t t_len;

			while ((curr = tpbrt_next_token(curr, end, &t_len)) != TPBRT_NULL) {
				++count;
				curr += t_len;
			}

			if (count == 0) {
				out_uints->values = TPBRT_NULL;
				out_uints->count  = 0;
				return TPBRT_ERROR_NONE;
			}

		out_uints->values = malloc(sizeof(tpbrt_int_t) * count);
			if (out_uints->values == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		curr			 = param->value.chars;
		tpbrt_size_t idx = 0;
			while ((curr = tpbrt_next_token(curr, end, &t_len)) != TPBRT_NULL) {
					if (!tpbrt_parse_uint_token(curr, t_len, &out_uints->values[idx])) {
						free(out_uints->values);
						out_uints->values = TPBRT_NULL;
						out_uints->count  = 0;
						return TPBRT_ERROR_PARSE_UINT;
					}
				++idx;
				curr += t_len;
			}

		out_uints->count = count;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_float(const tpbrt_param_t* const param, tpbrt_float_t* const out_float) {
			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY && param->value_type != TPBRT_PARAM_VALUE_TYPE_SINGLE) {
				return TPBRT_ERROR_INVALID_OBJECT_TYPE;
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				tpbrt_float_array_t floats;
				const tpbrt_error_t err = tpbrt_param_as_floats(param, &floats);
					if (err != TPBRT_ERROR_NONE) { return err; }

					if (floats.count < 1) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }

					if (floats.count > 1) {
						free(floats.values);
						return TPBRT_ERROR_TOO_MANY_VALUES;
					}

				*out_float = floats.values[0];
				free(floats.values);
				return TPBRT_ERROR_NONE;
			}

		tpbrt_size_t t_len;
		const tpbrt_char_t* token = tpbrt_next_token(param->value.chars, param->value.chars + param->value.size, &t_len);
			if (token == TPBRT_NULL) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }
			if (!tpbrt_parse_float_token(token, t_len, out_float)) { return TPBRT_ERROR_PARSE_FLOAT; }
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_int(const tpbrt_param_t* const param, tpbrt_int_t* const out_int) {
			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY && param->value_type != TPBRT_PARAM_VALUE_TYPE_SINGLE) {
				return TPBRT_ERROR_INVALID_OBJECT_TYPE;
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				tpbrt_int_array_t ints;
				const tpbrt_error_t err = tpbrt_param_as_ints(param, &ints);
					if (err != TPBRT_ERROR_NONE) { return err; }

					if (ints.count < 1) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }

					if (ints.count > 1) {
						free(ints.values);
						return TPBRT_ERROR_TOO_MANY_VALUES;
					}

				*out_int = ints.values[0];
				free(ints.values);
				return TPBRT_ERROR_NONE;
			}

		tpbrt_size_t t_len;
		const tpbrt_char_t* token = tpbrt_next_token(param->value.chars, param->value.chars + param->value.size, &t_len);
			if (token == TPBRT_NULL) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }
			if (!tpbrt_parse_int_token(token, t_len, out_int)) { return TPBRT_ERROR_PARSE_INT; }
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_uint(const tpbrt_param_t* const param, tpbrt_uint_t* const out_uint) {
			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY && param->value_type != TPBRT_PARAM_VALUE_TYPE_SINGLE) {
				return TPBRT_ERROR_INVALID_OBJECT_TYPE;
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				tpbrt_uint_array_t uints;
				const tpbrt_error_t err = tpbrt_param_as_uints(param, &uints);
					if (err != TPBRT_ERROR_NONE) { return err; }

					if (uints.count < 1) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }

					if (uints.count > 1) {
						free(uints.values);
						return TPBRT_ERROR_TOO_MANY_VALUES;
					}

				*out_uint = uints.values[0];
				free(uints.values);
				return TPBRT_ERROR_NONE;
			}

		tpbrt_size_t t_len;
		const tpbrt_char_t* token = tpbrt_next_token(param->value.chars, param->value.chars + param->value.size, &t_len);
			if (token == TPBRT_NULL) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }
			if (!tpbrt_parse_uint_token(token, t_len, out_uint)) { return TPBRT_ERROR_PARSE_UINT; }
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_bool(const tpbrt_param_t* const param, tpbrt_bool_t* const out_bool) {
		static const tpbrt_string_t TRUE_STR  = { .chars = "true", .size = 4 };
		static const tpbrt_string_t FALSE_STR = { .chars = "false", .size = 5 };

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_SINGLE) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t t_len;
		const tpbrt_char_t* token = tpbrt_next_token(param->value.chars, param->value.chars + param->value.size, &t_len);
			if (token == TPBRT_NULL) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }

			if (t_len == TRUE_STR.size && strncmp(token, TRUE_STR.chars, TRUE_STR.size) == 0) {
				*out_bool = TPBRT_TRUE;
				return TPBRT_ERROR_NONE;
			}

			if (t_len == FALSE_STR.size && strncmp(token, FALSE_STR.chars, FALSE_STR.size) == 0) {
				*out_bool = TPBRT_FALSE;
				return TPBRT_ERROR_NONE;
			}

		return TPBRT_ERROR_PARSE_BOOL;
	}

	tpbrt_error_t tpbrt_param_as_rgb(const tpbrt_param_t* const param, tpbrt_rgb_t* const out_rgb) {
			if (param == TPBRT_NULL || out_rgb == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_float_array_t floats;
		const tpbrt_error_t err = tpbrt_param_as_floats(param, &floats);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (floats.count < 3) {
					if (floats.values != TPBRT_NULL) { free(floats.values); }
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

			if (floats.count > 3) {
				free(floats.values);
				return TPBRT_ERROR_TOO_MANY_VALUES;
			}

		out_rgb->r = floats.values[0];
		out_rgb->g = floats.values[1];
		out_rgb->b = floats.values[2];
		free(floats.values);
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_wavelengths(const tpbrt_param_t* const param, tpbrt_wavelength_array_t* const out_wavelengths) {
			if (param == TPBRT_NULL || out_wavelengths == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		 = 0;
		const tpbrt_char_t* curr = param->value.chars;
		const tpbrt_char_t* end	 = curr + param->value.size;
		tpbrt_size_t t_len;

			while ((curr = tpbrt_next_token(curr, end, &t_len)) != TPBRT_NULL) {
				++count;
				curr += t_len;
			}

			if (count == 0) {
				out_wavelengths->values = TPBRT_NULL;
				out_wavelengths->count	= 0;
				return TPBRT_ERROR_NONE;
			}

			if (count % 2 != 0) {
				out_wavelengths->values = TPBRT_NULL;
				out_wavelengths->count	= 0;
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

		count /= 2;

		out_wavelengths->values = malloc(sizeof(tpbrt_wavelength_t) * count);
			if (out_wavelengths->values == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		curr			 = param->value.chars;
		tpbrt_size_t idx = 0;
			while ((curr = tpbrt_next_token(curr, end, &t_len)) != TPBRT_NULL) {
					if (!tpbrt_parse_uint_token(curr, t_len, &out_wavelengths->values[idx].wavelength)) {
						free(out_wavelengths->values);
						out_wavelengths->values = TPBRT_NULL;
						out_wavelengths->count	= 0;
						return TPBRT_ERROR_PARSE_UINT;
					}
				curr += t_len;

				curr = tpbrt_next_token(curr, end, &t_len);
					if (!tpbrt_parse_float_token(curr, t_len, &out_wavelengths->values[idx].value)) {
						free(out_wavelengths->values);
						out_wavelengths->values = TPBRT_NULL;
						out_wavelengths->count	= 0;
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				curr += t_len;

				++idx;
			}

		out_wavelengths->count = count;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_spectrum(const tpbrt_param_t* const param, tpbrt_spectrum_t* const out_spectrum) {
			if (param == TPBRT_NULL || out_spectrum == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->type == TPBRT_PARAM_TYPE_RGB) {
				out_spectrum->type = TPBRT_SPECTRUM_TYPE_RGB;
				return tpbrt_param_as_rgb(param, &out_spectrum->rgb);
			}

			if (param->type == TPBRT_PARAM_TYPE_BLACKBODY) {
				out_spectrum->type = TPBRT_SPECTRUM_TYPE_BLACKBODY;
				return tpbrt_param_as_uint(param, &out_spectrum->blackbody);
			}

			if (param->type == TPBRT_PARAM_TYPE_SPECTRUM) {
				// wavelengths list
				tpbrt_error_t err = tpbrt_param_as_wavelengths(param, &out_spectrum->wavelengths);
				// possible error: PARSE_FLOAT, PARSE_UINT, MISSING_REQUIRED_PARAMETER, ERROR_INVALID_POINTER, INVALID_OBJECT_TYPE
					if (err == TPBRT_ERROR_NONE) {
						out_spectrum->type = TPBRT_SPECTRUM_TYPE_WAVELENGTH;
						return TPBRT_ERROR_NONE;
					}

					if (err != TPBRT_ERROR_INVALID_OBJECT_TYPE) { return err; }

					if (param->value_type != TPBRT_PARAM_VALUE_TYPE_STRING) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

				// builtin
				err = tpbrt_spectrum_builtin_from_string(&param->value, &out_spectrum->builtin);
					// possible error: UNKNOWN_SPECTRUM_BUILTIN
					if (err == TPBRT_ERROR_NONE) {
						out_spectrum->type = TPBRT_SPECTRUM_TYPE_BUILTIN;
						return TPBRT_ERROR_NONE;
					}

				// filename
				out_spectrum->type			  = TPBRT_SPECTRUM_TYPE_FILE;
				out_spectrum->file_name.chars = malloc(sizeof(tpbrt_char_t) * param->value.size);
					if (out_spectrum->file_name.chars == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
				memcpy(out_spectrum->file_name.chars, param->value.chars, param->value.size * sizeof(tpbrt_char_t));
				out_spectrum->file_name.size = param->value.size;
				return TPBRT_ERROR_NONE;
			}

		return TPBRT_ERROR_INVALID_OBJECT_TYPE;
	}

#pragma endregion

#pragma region PARAMS_LIST

	tpbrt_error_t tpbrt_create_empty_params_list(tpbrt_params_list_t** const params_list) {
			if (params_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*params_list = malloc(sizeof(tpbrt_params_list_t));
			if (*params_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		(*params_list)->params = TPBRT_NULL;
		(*params_list)->count  = 0;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_params_list_add_param(tpbrt_params_list_t* const params_list, const tpbrt_param_t* const param) {
			if (param == TPBRT_NULL || params_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (params_list->params == TPBRT_NULL) {
				params_list->params = malloc(sizeof(tpbrt_param_t));
					if (params_list->params == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

				params_list->params[0] = *param;
				params_list->count	   = 1;
				return TPBRT_ERROR_NONE;
			}

			for (tpbrt_size_t i = 0; i < params_list->count; i++) {
					if (params_list->params[i].name.size == param->name.size &&
						strncmp(params_list->params[i].name.chars, param->name.chars, param->name.size) == 0) {
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

	tpbrt_error_t tpbrt_params_list_extend(tpbrt_params_list_t* const dest_params_list,
	  const tpbrt_params_list_t* const src_params_list) {
			if (dest_params_list == TPBRT_NULL || src_params_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			for (tpbrt_size_t i = 0; i < src_params_list->count; i++) {
				tpbrt_param_t* existing;
					if (tpbrt_params_list_get_param(dest_params_list, &src_params_list->params[i].name, &existing) ==
						TPBRT_ERROR_NONE) {
						*existing = src_params_list->params[i];
					}
					else {
						const tpbrt_error_t err = tpbrt_params_list_add_param(dest_params_list, &src_params_list->params[i]);
							if (err != TPBRT_ERROR_NONE) { return err; }
					}
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_params_list_get_param(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_param_t** const param) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || param_name->chars == TPBRT_NULL ||
				param_name->size == 0 || param == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_size_t i = 0; i < params_list->count; i++) {
					if (params_list->params[i].name.size == param_name->size &&
						strncmp(params_list->params[i].name.chars, param_name->chars, param_name->size) == 0) {
						*param = &params_list->params[i];
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_NOT_FOUND;
	}

	tpbrt_error_t tpbrt_params_list_get_param_const(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  const tpbrt_param_t** param) {
		tpbrt_param_t* non_const_param = TPBRT_NULL;
		const tpbrt_error_t err		   = tpbrt_params_list_get_param(params_list, param_name, &non_const_param);
		*param						   = (const tpbrt_param_t*)non_const_param;
		return err;
	}

	void tpbrt_free_params_list(tpbrt_params_list_t** params_list) {
			if (params_list == TPBRT_NULL || *params_list == TPBRT_NULL) { return; }

			for (tpbrt_size_t i = 0; i < (*params_list)->count; i++) {
				tpbrt_param_t* param = (*params_list)->params + i;
				tpbrt_free_param(&param);
			}

		free((*params_list)->params);
		free(*params_list);
		*params_list = TPBRT_NULL;
	}

	tpbrt_size_t tpbrt_params_list_size(const tpbrt_params_list_t* const params_list) {
		return params_list != TPBRT_NULL ? params_list->count : 0;
	}

	tpbrt_bool_t tpbrt_params_list_is_empty(const tpbrt_params_list_t* const params_list) {
		return tpbrt_params_list_size(params_list) == 0 ? TPBRT_TRUE : TPBRT_FALSE;
	}

	tpbrt_error_t tpbrt_params_list_get_floats(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_float_array_t* const out_floats) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_floats == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				out_floats->values = TPBRT_NULL;
				out_floats->count  = 0;
				return err;
			}

		return tpbrt_param_as_floats(param, out_floats);
	}

	tpbrt_error_t tpbrt_params_list_get_ints(const tpbrt_params_list_t* const params_list, const tpbrt_string_t* const param_name,
	  tpbrt_int_array_t* const out_ints) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_ints == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				out_ints->values = TPBRT_NULL;
				out_ints->count	 = 0;
				return err;
			}

		return tpbrt_param_as_ints(param, out_ints);
	}

	tpbrt_error_t tpbrt_params_list_get_uints(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_uint_array_t* const out_uints) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_uints == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				out_uints->values = TPBRT_NULL;
				out_uints->count  = 0;
				return err;
			}

		return tpbrt_param_as_uints(param, out_uints);
	}

	tpbrt_error_t tpbrt_params_list_get_float(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, const tpbrt_float_t default_val, tpbrt_float_t* const out_val) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_val == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err == TPBRT_ERROR_NOT_FOUND) {
				*out_val = default_val;
				return TPBRT_ERROR_NONE;
			}

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_float(param, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_int(const tpbrt_params_list_t* const params_list, const tpbrt_string_t* const param_name,
	  const tpbrt_int_t default_val, tpbrt_int_t* const out_val) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_val == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err == TPBRT_ERROR_NOT_FOUND) {
				*out_val = default_val;
				return TPBRT_ERROR_NONE;
			}

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_int(param, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_uint(const tpbrt_params_list_t* const params_list, const tpbrt_string_t* const param_name,
	  const tpbrt_uint_t default_val, tpbrt_uint_t* const out_val) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_val == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err == TPBRT_ERROR_NOT_FOUND) {
				*out_val = default_val;
				return TPBRT_ERROR_NONE;
			}

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_uint(param, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_bool(const tpbrt_params_list_t* const params_list, const tpbrt_string_t* const param_name,
	  const tpbrt_bool_t default_val, tpbrt_bool_t* const out_val) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_val == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err == TPBRT_ERROR_NOT_FOUND) {
				*out_val = default_val;
				return TPBRT_ERROR_NONE;
			}

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_bool(param, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_string(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_string_t* const out_val) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_val == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err == TPBRT_ERROR_NONE) { *out_val = param->value; }

		return err;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif