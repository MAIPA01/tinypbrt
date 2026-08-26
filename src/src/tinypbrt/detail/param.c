#include <tinypbrt/pch.h>

#include <tinypbrt/detail/param_internal.h>

#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/texture_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region PARAM

	static tpbrt_error_t tpbrt_param_type_from_string(const tpbrt_string_t* const type_str, tpbrt_param_type_t* const type_out) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_PARAM_TYPE_MAX_NUM] = {
			TPBRT_STRING("integer"),
			TPBRT_STRING("float"),
			TPBRT_STRING("point2"),
			TPBRT_STRING("vector2"),
			TPBRT_STRING("point3"),
			TPBRT_STRING("point"),
			TPBRT_STRING("vector3"),
			TPBRT_STRING("vector"),
			TPBRT_STRING("normal3"),
			TPBRT_STRING("normal"),
			TPBRT_STRING("spectrum"),
			TPBRT_STRING("rgb"),
			TPBRT_STRING("blackbody"),
			TPBRT_STRING("bool"),
			TPBRT_STRING("string"),
			TPBRT_STRING("texture"),
		};

			if (type_str == TPBRT_NULL || type_out == TPBRT_NULL || type_str->data == TPBRT_NULL || type_str->size == 0) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_param_type_t t = 0; t < TPBRT_PARAM_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS + t)) {
						*type_out = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_PARAM_TYPE;
	}

	tpbrt_error_t tpbrt_create_param(const tpbrt_string_t* const declaration, const tpbrt_param_value_type_t value_type,
	  const tpbrt_string_t* const value_str, tpbrt_param_t* const param) {
			if (param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(param, 0, sizeof(tpbrt_param_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			// declaration -> "type name"
			if (declaration == TPBRT_NULL || declaration->data == TPBRT_NULL || declaration->size == 0) {
				return TPBRT_ERROR_INVALID_PARAM_TYPE;
			}

		tpbrt_string_t token		  = { .data = declaration->data, .size = declaration->size };
		const tpbrt_char_t* end		  = declaration->data + declaration->size;

		const tpbrt_string_t type_str = tpbrt_next_token(token.data, end);
			if (type_str.data == TPBRT_NULL || type_str.size == 0) { return TPBRT_ERROR_INVALID_PARAM_TYPE; }
		token.data += token.size;

		tpbrt_param_type_t param_type;
		const tpbrt_error_t error = tpbrt_param_type_from_string(&type_str, &param_type);
			if (error != TPBRT_ERROR_NONE) { return error; }

		const tpbrt_string_t name_str = tpbrt_next_token(token.data, end);
			if (name_str.data == TPBRT_NULL || name_str.size == 0) { return TPBRT_ERROR_INVALID_PARAM_NAME; }

		param->name		  = name_str;
		param->type		  = param_type;
		param->value_type = value_type;
		param->value	  = *value_str;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_strings(const tpbrt_param_t* const param, tpbrt_string_array_t* const out_strings) {
			if (param == TPBRT_NULL || out_strings == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				out_strings->data  = TPBRT_NULL;
				out_strings->count = 0;
				return TPBRT_ERROR_INVALID_PARAM_TYPE;
			}

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_strings->data  = TPBRT_NULL;
				out_strings->count = 0;
				return TPBRT_ERROR_NONE;
			}

		out_strings->data = calloc(count, sizeof(tpbrt_string_t));
			if (out_strings->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_strings->count = count;

		token.data		   = param->value.data;
		tpbrt_size_t idx   = 0;
			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
				tpbrt_string_t temp;
					if (!tpbrt_parse_string_token(&token, &temp)) {
						tpbrt_free_string_array(out_strings);
						return TPBRT_ERROR_PARSE_STRING;
					}
				tpbrt_copy_string(&out_strings->data[idx], &temp);

				++idx;
				token.data += token.size;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_string(const tpbrt_param_t* const param, tpbrt_string_t* const out_string) {
			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY && param->value_type != TPBRT_PARAM_VALUE_TYPE_STRING) {
				return TPBRT_ERROR_INVALID_OBJECT_TYPE;
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				tpbrt_string_array_t strings;
				tpbrt_error_t err = tpbrt_param_as_strings(param, &strings);
					if (err != TPBRT_ERROR_NONE) { return err; }

					if (strings.count < 1) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }

					if (strings.count > 1) {
						tpbrt_free_string_array(&strings);
						return TPBRT_ERROR_TOO_MANY_VALUES;
					}

				err = tpbrt_copy_string(out_string, strings.data);
				tpbrt_free_string_array(&strings);
				return err;
			}

		const tpbrt_string_t token = tpbrt_next_token(param->value.data, param->value.data + param->value.size);
			if (token.data == TPBRT_NULL) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }
		tpbrt_string_t temp;
			if (!tpbrt_parse_string_token(&token, &temp)) { return TPBRT_ERROR_PARSE_STRING; }
		return tpbrt_copy_string(out_string, &temp);
	}

	tpbrt_error_t tpbrt_param_as_bools(const tpbrt_param_t* const param, tpbrt_bool_array_t* const out_bools) {
			if (param == TPBRT_NULL || out_bools == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_bools->data	 = TPBRT_NULL;
				out_bools->count = 0;
				return TPBRT_ERROR_NONE;
			}

		out_bools->data = malloc(sizeof(tpbrt_bool_t) * count);
			if (out_bools->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_bools->count = count;


		token.data		 = param->value.data;
		tpbrt_size_t idx = 0;
			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
					if (!tpbrt_parse_bool_token(&token, &out_bools->data[idx])) {
						tpbrt_free_bool_array(out_bools);
						return TPBRT_ERROR_PARSE_BOOL;
					}
				++idx;
				token.data += token.size;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_floats(const tpbrt_param_t* const param, tpbrt_float_array_t* const out_floats) {
			if (param == TPBRT_NULL || out_floats == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_floats->data  = TPBRT_NULL;
				out_floats->count = 0;
				return TPBRT_ERROR_NONE;
			}

		out_floats->data = malloc(sizeof(tpbrt_float_t) * count);
			if (out_floats->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_floats->count = count;

		token.data		  = param->value.data;
		tpbrt_size_t idx  = 0;
			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
					if (!tpbrt_parse_float_token(&token, &out_floats->data[idx])) {
						tpbrt_free_float_array(out_floats);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				++idx;
				token.data += token.size;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_ints(const tpbrt_param_t* const param, tpbrt_int_array_t* const out_ints) {
			if (param == TPBRT_NULL || out_ints == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_ints->data	= TPBRT_NULL;
				out_ints->count = 0;
				return TPBRT_ERROR_NONE;
			}

		out_ints->data = malloc(sizeof(tpbrt_int_t) * count);
			if (out_ints->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_ints->count	 = count;

		token.data		 = param->value.data;
		tpbrt_size_t idx = 0;
			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
					if (!tpbrt_parse_int_token(&token, &out_ints->data[idx])) {
						tpbrt_free_int_array(out_ints);
						return TPBRT_ERROR_PARSE_INT;
					}
				++idx;
				token.data += token.size;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_uints(const tpbrt_param_t* const param, tpbrt_uint_array_t* const out_uints) {
			if (param == TPBRT_NULL || out_uints == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_uints->data	 = TPBRT_NULL;
				out_uints->count = 0;
				return TPBRT_ERROR_NONE;
			}

		out_uints->data = malloc(sizeof(tpbrt_int_t) * count);
			if (out_uints->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_uints->count = count;

		token.data		 = param->value.data;
		tpbrt_size_t idx = 0;
			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
					if (!tpbrt_parse_uint_token(&token, &out_uints->data[idx])) {
						tpbrt_free_uint_array(out_uints);
						return TPBRT_ERROR_PARSE_UINT;
					}
				++idx;
				token.data += token.size;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_bool(const tpbrt_param_t* const param, tpbrt_bool_t* const out_bool) {
			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY && param->value_type != TPBRT_PARAM_VALUE_TYPE_SINGLE) {
				return TPBRT_ERROR_INVALID_OBJECT_TYPE;
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				tpbrt_bool_array_t bools;
				const tpbrt_error_t err = tpbrt_param_as_bools(param, &bools);
					if (err != TPBRT_ERROR_NONE) { return err; }

					if (bools.count < 1) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }

					if (bools.count > 1) {
						tpbrt_free_bool_array(&bools);
						return TPBRT_ERROR_TOO_MANY_VALUES;
					}

				*out_bool = bools.data[0];
				tpbrt_free_bool_array(&bools);
				return TPBRT_ERROR_NONE;
			}

		const tpbrt_string_t token = tpbrt_next_token(param->value.data, param->value.data + param->value.size);
			if (token.data == TPBRT_NULL) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }
			if (!tpbrt_parse_bool_token(&token, out_bool)) { return TPBRT_ERROR_PARSE_BOOL; }
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
						tpbrt_free_float_array(&floats);
						return TPBRT_ERROR_TOO_MANY_VALUES;
					}

				*out_float = floats.data[0];
				tpbrt_free_float_array(&floats);
				return TPBRT_ERROR_NONE;
			}

		const tpbrt_string_t token = tpbrt_next_token(param->value.data, param->value.data + param->value.size);
			if (token.data == TPBRT_NULL) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }
			if (!tpbrt_parse_float_token(&token, out_float)) { return TPBRT_ERROR_PARSE_FLOAT; }
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
						tpbrt_free_int_array(&ints);
						return TPBRT_ERROR_TOO_MANY_VALUES;
					}

				*out_int = ints.data[0];
				tpbrt_free_int_array(&ints);
				return TPBRT_ERROR_NONE;
			}

		const tpbrt_string_t token = tpbrt_next_token(param->value.data, param->value.data + param->value.size);
			if (token.data == TPBRT_NULL) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }
			if (!tpbrt_parse_int_token(&token, out_int)) { return TPBRT_ERROR_PARSE_INT; }
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
						tpbrt_free_uint_array(&uints);
						return TPBRT_ERROR_TOO_MANY_VALUES;
					}

				*out_uint = uints.data[0];
				tpbrt_free_uint_array(&uints);
				return TPBRT_ERROR_NONE;
			}

		const tpbrt_string_t token = tpbrt_next_token(param->value.data, param->value.data + param->value.size);
			if (token.data == TPBRT_NULL) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }
			if (!tpbrt_parse_uint_token(&token, out_uint)) { return TPBRT_ERROR_PARSE_UINT; }
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_rgbs(const tpbrt_param_t* const param, tpbrt_rgb_array_t* const out_rgbs) {
			if (param == TPBRT_NULL || out_rgbs == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_rgbs->data	= TPBRT_NULL;
				out_rgbs->count = 0;
				return TPBRT_ERROR_NONE;
			}

			if (count % 3 != 0) {
				out_rgbs->data	= TPBRT_NULL;
				out_rgbs->count = 0;
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

		count /= 3;

		out_rgbs->data = malloc(sizeof(tpbrt_rgb_t) * count);
			if (out_rgbs->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_rgbs->count	 = count;

		token.data		 = param->value.data;
		tpbrt_size_t idx = 0;
			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
					if (!tpbrt_parse_float_token(&token, &out_rgbs->data[idx].r)) {
						tpbrt_free_rgb_array(out_rgbs);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				token.data += token.size;

				token = tpbrt_next_token(token.data, end);
					if (!tpbrt_parse_float_token(&token, &out_rgbs->data[idx].g)) {
						tpbrt_free_rgb_array(out_rgbs);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				token.data += token.size;

				token = tpbrt_next_token(token.data, end);
					if (!tpbrt_parse_float_token(&token, &out_rgbs->data[idx].b)) {
						tpbrt_free_rgb_array(out_rgbs);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				token.data += token.size;

				++idx;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_rgb(const tpbrt_param_t* const param, tpbrt_rgb_t* const out_rgb) {
			if (param == TPBRT_NULL || out_rgb == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_float_array_t floats;
		const tpbrt_error_t err = tpbrt_param_as_floats(param, &floats);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (floats.count < 3) {
				tpbrt_free_float_array(&floats);
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

			if (floats.count > 3) {
				tpbrt_free_float_array(&floats);
				return TPBRT_ERROR_TOO_MANY_VALUES;
			}

		out_rgb->r = floats.data[0];
		out_rgb->g = floats.data[1];
		out_rgb->b = floats.data[2];
		tpbrt_free_float_array(&floats);
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_wavelengths(const tpbrt_param_t* const param, tpbrt_wavelength_array_t* const out_wavelengths) {
			if (param == TPBRT_NULL || out_wavelengths == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_wavelengths->data  = TPBRT_NULL;
				out_wavelengths->count = 0;
				return TPBRT_ERROR_NONE;
			}

			if (count % 2 != 0) {
				out_wavelengths->data  = TPBRT_NULL;
				out_wavelengths->count = 0;
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

		count /= 2;

		out_wavelengths->data = malloc(sizeof(tpbrt_wavelength_t) * count);
			if (out_wavelengths->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_wavelengths->count = count;

		token.data			   = param->value.data;
		tpbrt_size_t idx	   = 0;
			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
					if (!tpbrt_parse_uint_token(&token, &out_wavelengths->data[idx].wavelength)) {
						tpbrt_free_wavelength_array(out_wavelengths);
						return TPBRT_ERROR_PARSE_UINT;
					}
				token.data += token.size;

				token = tpbrt_next_token(token.data, end);
					if (!tpbrt_parse_float_token(&token, &out_wavelengths->data[idx].value)) {
						tpbrt_free_wavelength_array(out_wavelengths);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				token.data += token.size;

				++idx;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_spectrum(const tpbrt_param_t* const param, tpbrt_spectrum_t* const out_spectrum) {
			if (param == TPBRT_NULL || out_spectrum == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->type == TPBRT_PARAM_TYPE_RGB) {
				out_spectrum->type = TPBRT_SPECTRUM_TYPE_RGB;
				return tpbrt_param_as_rgb(param, &out_spectrum->as.rgb);
			}

			if (param->type == TPBRT_PARAM_TYPE_BLACKBODY) {
				out_spectrum->type = TPBRT_SPECTRUM_TYPE_BLACKBODY;
				return tpbrt_param_as_uint(param, &out_spectrum->as.blackbody);
			}

			if (param->type == TPBRT_PARAM_TYPE_SPECTRUM) {
				// wavelengths list
				tpbrt_error_t err = tpbrt_param_as_wavelengths(param, &out_spectrum->as.wavelengths);
				// possible error: PARSE_FLOAT, PARSE_UINT, MISSING_REQUIRED_PARAMETER, ERROR_INVALID_POINTER, INVALID_OBJECT_TYPE
					if (err == TPBRT_ERROR_NONE) {
						out_spectrum->type = TPBRT_SPECTRUM_TYPE_WAVELENGTH;
						return TPBRT_ERROR_NONE;
					}

					if (err != TPBRT_ERROR_INVALID_OBJECT_TYPE) { return err; }

					if (param->value_type != TPBRT_PARAM_VALUE_TYPE_STRING) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

				// builtin
				err = tpbrt_spectrum_builtin_from_string(&param->value, &out_spectrum->as.builtin);
					// possible error: UNKNOWN_SPECTRUM_BUILTIN
					if (err == TPBRT_ERROR_NONE) {
						out_spectrum->type = TPBRT_SPECTRUM_TYPE_BUILTIN;
						return TPBRT_ERROR_NONE;
					}

				// filename
				out_spectrum->type = TPBRT_SPECTRUM_TYPE_FILE;
				return tpbrt_copy_string(&out_spectrum->as.file_name, &param->value);
			}

		return TPBRT_ERROR_INVALID_OBJECT_TYPE;
	}

	tpbrt_error_t tpbrt_param_as_vec2(const tpbrt_param_t* const param, tpbrt_vec2_t* const out_vec2) {
			if (param == TPBRT_NULL || out_vec2 == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_float_array_t floats;
		const tpbrt_error_t err = tpbrt_param_as_floats(param, &floats);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (floats.count < 2) {
				tpbrt_free_float_array(&floats);
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

			if (floats.count > 2) {
				tpbrt_free_float_array(&floats);
				return TPBRT_ERROR_TOO_MANY_VALUES;
			}

		out_vec2->x = floats.data[0];
		out_vec2->y = floats.data[1];
		tpbrt_free_float_array(&floats);
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_point2(const tpbrt_param_t* const param, tpbrt_point2_t* const out_point2) {
		return tpbrt_param_as_vec2(param, out_point2);
	}

	tpbrt_error_t tpbrt_param_as_vec2s(const tpbrt_param_t* const param, tpbrt_vec2_array_t* const out_vec2s) {
			if (param == TPBRT_NULL || out_vec2s == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_vec2s->data	 = TPBRT_NULL;
				out_vec2s->count = 0;
				return TPBRT_ERROR_NONE;
			}

			if (count % 2 != 0) {
				out_vec2s->data	 = TPBRT_NULL;
				out_vec2s->count = 0;
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

		count /= 2;

		out_vec2s->data = malloc(sizeof(tpbrt_vec2_t) * count);
			if (out_vec2s->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_vec2s->count = count;

		token.data		 = param->value.data;
		tpbrt_size_t idx = 0;
			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
					if (!tpbrt_parse_float_token(&token, &out_vec2s->data[idx].x)) {
						tpbrt_free_vec2_array(out_vec2s);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				token.data += token.size;

				token = tpbrt_next_token(token.data, end);
					if (!tpbrt_parse_float_token(&token, &out_vec2s->data[idx].y)) {
						tpbrt_free_vec2_array(out_vec2s);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				token.data += token.size;

				++idx;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_point2s(const tpbrt_param_t* const param, tpbrt_point2_array_t* const out_point2s) {
		return tpbrt_param_as_vec2s(param, out_point2s);
	}

	tpbrt_error_t tpbrt_param_as_vec3(const tpbrt_param_t* const param, tpbrt_vec3_t* const out_vec3) {
			if (param == TPBRT_NULL || out_vec3 == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_float_array_t floats;
		const tpbrt_error_t err = tpbrt_param_as_floats(param, &floats);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (floats.count < 3) {
				tpbrt_free_float_array(&floats);
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

			if (floats.count > 3) {
				tpbrt_free_float_array(&floats);
				return TPBRT_ERROR_TOO_MANY_VALUES;
			}

		out_vec3->x = floats.data[0];
		out_vec3->y = floats.data[1];
		out_vec3->z = floats.data[2];
		tpbrt_free_float_array(&floats);
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_vec(const tpbrt_param_t* const param, tpbrt_vec_t* const out_vec) {
		return tpbrt_param_as_vec3(param, out_vec);
	}

	tpbrt_error_t tpbrt_param_as_point3(const tpbrt_param_t* const param, tpbrt_point3_t* const out_point3) {
		return tpbrt_param_as_vec3(param, out_point3);
	}

	tpbrt_error_t tpbrt_param_as_point(const tpbrt_param_t* const param, tpbrt_point_t* const out_point) {
		return tpbrt_param_as_point3(param, out_point);
	}

	tpbrt_error_t tpbrt_param_as_normal3(const tpbrt_param_t* const param, tpbrt_normal3_t* const out_normal3) {
		return tpbrt_param_as_vec3(param, out_normal3);
	}

	tpbrt_error_t tpbrt_param_as_normal(const tpbrt_param_t* const param, tpbrt_normal_t* const out_normal) {
		return tpbrt_param_as_normal3(param, out_normal);
	}

	tpbrt_error_t tpbrt_param_as_vec3s(const tpbrt_param_t* const param, tpbrt_vec3_array_t* const out_vec3s) {
			if (param == TPBRT_NULL || out_vec3s == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_vec3s->data	 = TPBRT_NULL;
				out_vec3s->count = 0;
				return TPBRT_ERROR_NONE;
			}

			if (count % 3 != 0) {
				out_vec3s->data	 = TPBRT_NULL;
				out_vec3s->count = 0;
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

		count /= 3;

		out_vec3s->data = malloc(sizeof(tpbrt_vec3_t) * count);
			if (out_vec3s->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_vec3s->count = count;

		token.data		 = param->value.data;
		tpbrt_size_t idx = 0;
			while ((token = tpbrt_next_token(token.data, end)).data != TPBRT_NULL) {
					if (!tpbrt_parse_float_token(&token, &out_vec3s->data[idx].x)) {
						tpbrt_free_vec3_array(out_vec3s);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				token.data += token.size;

				token = tpbrt_next_token(token.data, end);
					if (!tpbrt_parse_float_token(&token, &out_vec3s->data[idx].y)) {
						tpbrt_free_vec3_array(out_vec3s);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				token.data += token.size;

				token = tpbrt_next_token(token.data, end);
					if (!tpbrt_parse_float_token(&token, &out_vec3s->data[idx].z)) {
						tpbrt_free_vec3_array(out_vec3s);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				token.data += token.size;

				++idx;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_vecs(const tpbrt_param_t* const param, tpbrt_vec_array_t* const out_vecs) {
		return tpbrt_param_as_vec3s(param, out_vecs);
	}

	tpbrt_error_t tpbrt_param_as_point3s(const tpbrt_param_t* const param, tpbrt_point3_array_t* const out_point3s) {
		return tpbrt_param_as_vec3s(param, out_point3s);
	}

	tpbrt_error_t tpbrt_param_as_points(const tpbrt_param_t* const param, tpbrt_point_array_t* const out_points) {
		return tpbrt_param_as_point3s(param, out_points);
	}

	tpbrt_error_t tpbrt_param_as_normal3s(const tpbrt_param_t* const param, tpbrt_normal3_array_t* const out_normal3s) {
		return tpbrt_param_as_vec3s(param, out_normal3s);
	}

	tpbrt_error_t tpbrt_param_as_normals(const tpbrt_param_t* const param, tpbrt_normal_array_t* const out_normals) {
		return tpbrt_param_as_normal3s(param, out_normals);
	}

	tpbrt_error_t tpbrt_param_as_texture(const tpbrt_param_t* const param, const tpbrt_textures_list_t* const textures_list,
	  tpbrt_texture_handle_t* const out_texture) {
			if (param == TPBRT_NULL || textures_list == TPBRT_NULL || out_texture == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		tpbrt_error_t err = TPBRT_ERROR_NONE;
			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_SINGLE || param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				// float
				err = tpbrt_param_as_float(param, &out_texture->as.float_value);
					if (err == TPBRT_ERROR_NONE) {
						out_texture->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
						return TPBRT_ERROR_NONE;
					}
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_STRING || param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				// texture | spectrum builtin
				tpbrt_string_t temp_string;
				err = tpbrt_param_as_string(param, &temp_string);
					if (err == TPBRT_ERROR_NONE) {
						// spectrum builtin
						err = tpbrt_spectrum_builtin_from_string(&temp_string, &out_texture->as.spectrum_builtin);
							if (err == TPBRT_ERROR_NONE) {
								tpbrt_free_string(&temp_string);
								out_texture->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM_BUILTIN;
								return TPBRT_ERROR_NONE;
							}

						// texture
						err = tpbrt_textures_list_get_texture_handle(textures_list, &temp_string, out_texture);
						tpbrt_free_string(&temp_string);
						return err;
					}
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				// rgb
				err = tpbrt_param_as_rgb(param, &out_texture->as.rgb);
					if (err == TPBRT_ERROR_NONE) {
						out_texture->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_RGB;
						return TPBRT_ERROR_NONE;
					}
			}

		out_texture->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE;
		return err;
	}

	tpbrt_error_t tpbrt_param_as_texture_of_type(const tpbrt_param_t* const param,
	  const tpbrt_textures_list_t* const textures_list, const tpbrt_texture_type_t texture_type,
	  tpbrt_texture_handle_t* out_texture) {
			if (param == TPBRT_NULL || textures_list == TPBRT_NULL || out_texture == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		tpbrt_error_t err = TPBRT_ERROR_NONE;
			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_SINGLE || param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				// float
				err = tpbrt_param_as_float(param, &out_texture->as.float_value);
					if (err == TPBRT_ERROR_NONE) {
						out_texture->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
						return TPBRT_ERROR_NONE;
					}
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_STRING || param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				// texture | spectrum builtin
				tpbrt_string_t temp_string;
				err = tpbrt_param_as_string(param, &temp_string);
					if (err == TPBRT_ERROR_NONE) {
						// spectrum builtin
						err = tpbrt_spectrum_builtin_from_string(&temp_string, &out_texture->as.spectrum_builtin);
							if (err == TPBRT_ERROR_NONE) {
								tpbrt_free_string(&temp_string);
								out_texture->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM_BUILTIN;
								return TPBRT_ERROR_NONE;
							}

						// texture
						err =
						  tpbrt_textures_list_get_texture_of_type_handle(textures_list, &temp_string, texture_type, out_texture);
						tpbrt_free_string(&temp_string);
						return err;
					}
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				// rgb
				err = tpbrt_param_as_rgb(param, &out_texture->as.rgb);
					if (err == TPBRT_ERROR_NONE) {
						out_texture->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_RGB;
						return TPBRT_ERROR_NONE;
					}
			}

		out_texture->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE;
		return err;
	}

#pragma endregion

#pragma region PARAMS_LIST

	tpbrt_error_t tpbrt_init_params_list(tpbrt_params_list_t* const params_list) {
			if (params_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		params_list->params = TPBRT_NULL;
		params_list->count	= 0;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_params_list_add_param(tpbrt_params_list_t* const params_list, const tpbrt_param_t* const param) {
			if (param == TPBRT_NULL || params_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_size_t new_count = params_list->count + 1;

		tpbrt_param_t* new_list		 = realloc(params_list->params, sizeof(tpbrt_param_t) * new_count);
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		params_list->params						= new_list;
		params_list->params[params_list->count] = *param;
		params_list->count						= new_count;
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
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || param_name->data == TPBRT_NULL || param == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_size_t i = 0; i < params_list->count; i++) {
					if (tpbrt_string_equals(&params_list->params[i].name, param_name)) {
						*param = &params_list->params[i];
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_NOT_FOUND;
	}

	tpbrt_error_t tpbrt_params_list_get_param_const(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, const tpbrt_param_t** const param) {
		tpbrt_param_t* non_const_param = TPBRT_NULL;
		const tpbrt_error_t err		   = tpbrt_params_list_get_param(params_list, param_name, &non_const_param);
		*param						   = (const tpbrt_param_t*)non_const_param;
		return err;
	}

	void tpbrt_free_params_list(tpbrt_params_list_t* const params_list) {
			if (params_list == TPBRT_NULL || params_list->params == TPBRT_NULL) { return; }

		free(params_list->params);
		params_list->params = TPBRT_NULL;
		params_list->count	= 0;
	}

	tpbrt_size_t tpbrt_params_list_size(const tpbrt_params_list_t* const params_list) {
		return params_list != TPBRT_NULL ? params_list->count : 0;
	}

	tpbrt_bool_t tpbrt_params_list_is_empty(const tpbrt_params_list_t* const params_list) {
		return tpbrt_params_list_size(params_list) == 0 ? TPBRT_TRUE : TPBRT_FALSE;
	}

	tpbrt_error_t tpbrt_params_list_get_strings(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_string_array_t* const out_vals) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_vals == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return err;
			}

		return tpbrt_param_as_strings(param, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_string(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_string_t* const out_val) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_val == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_string(param, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_bools(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_bool_array_t* const out_vals) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_vals == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return err;
			}

		return tpbrt_param_as_bools(param, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_floats(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_float_array_t* const out_vals) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_vals == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return err;
			}

		return tpbrt_param_as_floats(param, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_ints(const tpbrt_params_list_t* const params_list, const tpbrt_string_t* const param_name,
	  tpbrt_int_array_t* const out_vals) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_vals == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return err;
			}

		return tpbrt_param_as_ints(param, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_uints(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_uint_array_t* const out_vals) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_vals == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return err;
			}

		return tpbrt_param_as_uints(param, out_vals);
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

	tpbrt_error_t tpbrt_params_list_get_opt_float(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_opt_float_t* const out_val) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_val == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err == TPBRT_ERROR_NOT_FOUND) {
				out_val->has_value = TPBRT_FALSE;
				return TPBRT_ERROR_NONE;
			}

			if (err != TPBRT_ERROR_NONE) {
				out_val->has_value = TPBRT_FALSE;
				return err;
			}

		err = tpbrt_param_as_float(param, &out_val->value);
			if (err != TPBRT_ERROR_NONE) {
				out_val->has_value = TPBRT_FALSE;
				return err;
			}
		out_val->has_value = TPBRT_TRUE;
		return TPBRT_ERROR_NONE;
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

	tpbrt_error_t tpbrt_params_list_get_rgbs(const tpbrt_params_list_t* const params_list, const tpbrt_string_t* const param_name,
	  tpbrt_rgb_array_t* const out_vals) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_vals == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return err;
			}

		return tpbrt_param_as_rgbs(param, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_rgb(const tpbrt_params_list_t* const params_list, const tpbrt_string_t* const param_name,
	  tpbrt_rgb_t* const out_val) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_val == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_rgb(param, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_spectrum(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_spectrum_t* const out_val) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_val == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_spectrum(param, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_vec2(const tpbrt_params_list_t* const params_list, const tpbrt_string_t* const param_name,
	  tpbrt_vec2_t* const out_val) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_val == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_vec2(param, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_point2(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_point2_t* const out_val) {
		return tpbrt_params_list_get_vec2(params_list, param_name, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_vec2s(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_vec2_array_t* const out_vals) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_vals == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return err;
			}

		return tpbrt_param_as_vec2s(param, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_point2s(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_point2_array_t* const out_vals) {
		return tpbrt_params_list_get_vec2s(params_list, param_name, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_vec3(const tpbrt_params_list_t* const params_list, const tpbrt_string_t* const param_name,
	  tpbrt_vec3_t* const out_val) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_val == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_vec3(param, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_vec(const tpbrt_params_list_t* const params_list, const tpbrt_string_t* const param_name,
	  tpbrt_vec_t* const out_val) {
		return tpbrt_params_list_get_vec3(params_list, param_name, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_point3(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_point3_t* const out_val) {
		return tpbrt_params_list_get_vec3(params_list, param_name, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_point(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_point_t* const out_val) {
		return tpbrt_params_list_get_point3(params_list, param_name, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_normal3(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_normal3_t* const out_val) {
		return tpbrt_params_list_get_vec3(params_list, param_name, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_normal(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_normal_t* const out_val) {
		return tpbrt_params_list_get_normal3(params_list, param_name, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_vec3s(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_vec3_array_t* const out_vals) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_vals == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return err;
			}

		return tpbrt_param_as_vec3s(param, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_vecs(const tpbrt_params_list_t* const params_list, const tpbrt_string_t* const param_name,
	  tpbrt_vec_array_t* const out_vals) {
		return tpbrt_params_list_get_vec3s(params_list, param_name, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_point3s(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_point3_array_t* const out_vals) {
		return tpbrt_params_list_get_vec3s(params_list, param_name, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_points(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_point_array_t* const out_vals) {
		return tpbrt_params_list_get_point3s(params_list, param_name, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_normal3s(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_normal3_array_t* const out_vals) {
		return tpbrt_params_list_get_vec3s(params_list, param_name, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_normals(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, tpbrt_normal_array_t* const out_vals) {
		return tpbrt_params_list_get_normal3s(params_list, param_name, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_texture(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, const tpbrt_textures_list_t* const textures_list,
	  tpbrt_texture_handle_t* const out_val) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_val == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				out_val->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE;
				return err;
			}

		return tpbrt_param_as_texture(param, textures_list, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_texture_of_type(const tpbrt_params_list_t* const params_list,
	  const tpbrt_string_t* const param_name, const tpbrt_textures_list_t* const textures_list,
	  const tpbrt_texture_type_t texture_type, tpbrt_texture_handle_t* const out_val) {
			if (params_list == TPBRT_NULL || param_name == TPBRT_NULL || out_val == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_param_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				out_val->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE;
				return err;
			}

		return tpbrt_param_as_texture_of_type(param, textures_list, texture_type, out_val);
	}

#pragma endregion

#ifdef __cplusplus
}
#endif