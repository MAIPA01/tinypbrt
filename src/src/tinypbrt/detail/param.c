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

	tpbrt_error_t tpbrt_param_type_from_string(const tpbrt_string_t type_str, tpbrt_param_type_t* const type_out) {
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

			if (type_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(type_str)) { return TPBRT_ERROR_INVALID_PARAM_TYPE; }

			for (tpbrt_param_type_t t = 0; t < TPBRT_PARAM_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS[t])) {
						*type_out = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_PARAM_TYPE;
	}

	tpbrt_error_t tpbrt_param_init(const tpbrt_string_t declaration, const tpbrt_param_value_type_t value_type,
	  const tpbrt_string_t value_str, tpbrt_param_t* const param) {
			if (param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(param, 0, sizeof(tpbrt_param_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			// declaration -> "type name"
			if (tpbrt_string_empty(declaration)) { return TPBRT_ERROR_INVALID_PARAM_TYPE; }

		tpbrt_string_t token		  = declaration;
		const tpbrt_char_t* end		  = declaration.data + declaration.size;

		const tpbrt_string_t type_str = tpbrt_next_value(token.data, end);
			if (type_str.data == TPBRT_NULL || type_str.size == 0) { return TPBRT_ERROR_INVALID_PARAM_TYPE; }
		token.data += type_str.size;

		tpbrt_param_type_t param_type;
		const tpbrt_error_t error = tpbrt_param_type_from_string(type_str, &param_type);
			if (error != TPBRT_ERROR_NONE) { return error; }

		const tpbrt_string_t name_str = tpbrt_next_value(token.data, end);
			if (name_str.data == TPBRT_NULL || name_str.size == 0) { return TPBRT_ERROR_INVALID_PARAM_NAME; }

		*param = (tpbrt_param_t) {
			.name		= name_str,
			.type		= param_type,
			.value_type = value_type,
			.value		= value_str,
		};
		return TPBRT_ERROR_NONE;
	}

#pragma region STRING

	tpbrt_error_t tpbrt_param_as_strings(const tpbrt_param_t* const param, tpbrt_string_array_t* const out_vals) {
			if (param == TPBRT_NULL || out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return TPBRT_ERROR_INVALID_PARAM_TYPE;
			}

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_value(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return TPBRT_ERROR_NONE;
			}

		out_vals->data = calloc(count, sizeof(tpbrt_string_t));
			if (out_vals->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_vals->count	 = count;

		token.data		 = param->value.data;
		tpbrt_size_t idx = 0;
			while ((token = tpbrt_next_value(token.data, end)).data != TPBRT_NULL) {
				tpbrt_string_t temp;
					if (!tpbrt_read_string_value(token, &temp)) {
						tpbrt_string_array_free(out_vals);
						return TPBRT_ERROR_PARSE_STRING;
					}
				tpbrt_string_copy(&out_vals->data[idx], temp);

				++idx;
				token.data += token.size;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_strings_of_size(const tpbrt_param_t* const param, const tpbrt_size_t desired_size,
	  tpbrt_string_array_t* const out_vals) {
			if (param == TPBRT_NULL || out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_string_array_t values;
		const tpbrt_error_t err = tpbrt_param_as_strings(param, &values);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (values.count < desired_size) {
					if (values.data != TPBRT_NULL) { tpbrt_string_array_free(&values); }
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

			if (values.count > desired_size) {
				tpbrt_string_array_free(&values);
				return TPBRT_ERROR_TOO_MANY_VALUES;
			}

		*out_vals = values;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_string(const tpbrt_param_t* const param, tpbrt_string_t* const out_val) {
			if (param == TPBRT_NULL || out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY && param->value_type != TPBRT_PARAM_VALUE_TYPE_STRING) {
				return TPBRT_ERROR_INVALID_OBJECT_TYPE;
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				tpbrt_string_array_t strings;
				tpbrt_error_t err = tpbrt_param_as_strings_of_size(param, 1u, &strings);
					if (err != TPBRT_ERROR_NONE) { return err; }

				err = tpbrt_string_copy(out_val, strings.data[0]);
				tpbrt_string_array_free(&strings);
				return err;
			}

		return tpbrt_string_copy(out_val, param->value);
	}

#pragma endregion

#pragma region BOOL

	tpbrt_error_t tpbrt_param_as_bools(const tpbrt_param_t* const param, tpbrt_bool_array_t* const out_vals) {
			if (param == TPBRT_NULL || out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_value(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return TPBRT_ERROR_NONE;
			}

		out_vals->data = malloc(sizeof(tpbrt_bool_t) * count);
			if (out_vals->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_vals->count	 = count;


		token.data		 = param->value.data;
		tpbrt_size_t idx = 0;
			while ((token = tpbrt_next_value(token.data, end)).data != TPBRT_NULL) {
					if (!tpbrt_read_bool_value(token, &out_vals->data[idx])) {
						tpbrt_bool_array_free(out_vals);
						return TPBRT_ERROR_PARSE_BOOL;
					}
				++idx;
				token.data += token.size;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_bools_of_size(const tpbrt_param_t* const param, const tpbrt_size_t desired_size,
	  tpbrt_bool_array_t* const out_vals) {
			if (param == TPBRT_NULL || out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_bool_array_t values;
		const tpbrt_error_t err = tpbrt_param_as_bools(param, &values);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (values.count < desired_size) {
					if (values.data != TPBRT_NULL) { tpbrt_bool_array_free(&values); }
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

			if (values.count > desired_size) {
				tpbrt_bool_array_free(&values);
				return TPBRT_ERROR_TOO_MANY_VALUES;
			}

		*out_vals = values;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_bool(const tpbrt_param_t* const param, tpbrt_bool_t* const out_val) {
			if (param == TPBRT_NULL || out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY && param->value_type != TPBRT_PARAM_VALUE_TYPE_SINGLE) {
				return TPBRT_ERROR_INVALID_OBJECT_TYPE;
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				tpbrt_bool_array_t bools;
				const tpbrt_error_t err = tpbrt_param_as_bools_of_size(param, 1u, &bools);
					if (err != TPBRT_ERROR_NONE) { return err; }

				*out_val = bools.data[0];
				tpbrt_bool_array_free(&bools);
				return TPBRT_ERROR_NONE;
			}

		const tpbrt_string_t token = tpbrt_next_value(param->value.data, param->value.data + param->value.size);
			if (token.data == TPBRT_NULL) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }
			if (!tpbrt_read_bool_value(token, out_val)) { return TPBRT_ERROR_PARSE_BOOL; }
		return TPBRT_ERROR_NONE;
	}

#pragma endregion

#pragma region FLOAT

	tpbrt_error_t tpbrt_param_as_floats(const tpbrt_param_t* const param, tpbrt_float_array_t* const out_vals) {
			if (param == TPBRT_NULL || out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_value(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return TPBRT_ERROR_NONE;
			}

		out_vals->data = malloc(sizeof(tpbrt_float_t) * count);
			if (out_vals->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_vals->count	 = count;

		token.data		 = param->value.data;
		tpbrt_size_t idx = 0;
			while ((token = tpbrt_next_value(token.data, end)).data != TPBRT_NULL) {
					if (!tpbrt_read_float_value(token, &out_vals->data[idx])) {
						tpbrt_float_array_free(out_vals);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				++idx;
				token.data += token.size;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_floats_of_size(const tpbrt_param_t* const param, const tpbrt_size_t desired_size,
	  tpbrt_float_array_t* const out_vals) {
			if (param == TPBRT_NULL || out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_float_array_t values;
		const tpbrt_error_t err = tpbrt_param_as_floats(param, &values);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (values.count < desired_size) {
					if (values.data != TPBRT_NULL) { tpbrt_float_array_free(&values); }
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

			if (values.count > desired_size) {
				tpbrt_float_array_free(&values);
				return TPBRT_ERROR_TOO_MANY_VALUES;
			}

		*out_vals = values;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_float(const tpbrt_param_t* const param, tpbrt_float_t* const out_val) {
			if (param == TPBRT_NULL || out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY && param->value_type != TPBRT_PARAM_VALUE_TYPE_SINGLE) {
				return TPBRT_ERROR_INVALID_OBJECT_TYPE;
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				tpbrt_float_array_t floats;
				const tpbrt_error_t err = tpbrt_param_as_floats_of_size(param, 1u, &floats);
					if (err != TPBRT_ERROR_NONE) { return err; }

				*out_val = floats.data[0];
				tpbrt_float_array_free(&floats);
				return TPBRT_ERROR_NONE;
			}

		const tpbrt_string_t token = tpbrt_next_value(param->value.data, param->value.data + param->value.size);
			if (token.data == TPBRT_NULL) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }
			if (!tpbrt_read_float_value(token, out_val)) { return TPBRT_ERROR_PARSE_FLOAT; }
		return TPBRT_ERROR_NONE;
	}

#pragma endregion

#pragma region INT

	tpbrt_error_t tpbrt_param_as_ints(const tpbrt_param_t* const param, tpbrt_int_array_t* const out_vals) {
			if (param == TPBRT_NULL || out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_value(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return TPBRT_ERROR_NONE;
			}

		out_vals->data = malloc(sizeof(tpbrt_int_t) * count);
			if (out_vals->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_vals->count	 = count;

		token.data		 = param->value.data;
		tpbrt_size_t idx = 0;
			while ((token = tpbrt_next_value(token.data, end)).data != TPBRT_NULL) {
					if (!tpbrt_read_int_value(token, &out_vals->data[idx])) {
						tpbrt_int_array_free(out_vals);
						return TPBRT_ERROR_PARSE_INT;
					}
				++idx;
				token.data += token.size;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_ints_of_size(const tpbrt_param_t* const param, const tpbrt_size_t desired_size,
	  tpbrt_int_array_t* const out_vals) {
			if (param == TPBRT_NULL || out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_int_array_t values;
		const tpbrt_error_t err = tpbrt_param_as_ints(param, &values);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (values.count < desired_size) {
					if (values.data != TPBRT_NULL) { tpbrt_int_array_free(&values); }
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

			if (values.count > desired_size) {
				tpbrt_int_array_free(&values);
				return TPBRT_ERROR_TOO_MANY_VALUES;
			}

		*out_vals = values;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_int(const tpbrt_param_t* const param, tpbrt_int_t* const out_val) {
			if (param == TPBRT_NULL || out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY && param->value_type != TPBRT_PARAM_VALUE_TYPE_SINGLE) {
				return TPBRT_ERROR_INVALID_OBJECT_TYPE;
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				tpbrt_int_array_t ints;
				const tpbrt_error_t err = tpbrt_param_as_ints_of_size(param, 1u, &ints);
					if (err != TPBRT_ERROR_NONE) { return err; }

				*out_val = ints.data[0];
				tpbrt_int_array_free(&ints);
				return TPBRT_ERROR_NONE;
			}

		const tpbrt_string_t token = tpbrt_next_value(param->value.data, param->value.data + param->value.size);
			if (token.data == TPBRT_NULL) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }
			if (!tpbrt_read_int_value(token, out_val)) { return TPBRT_ERROR_PARSE_INT; }
		return TPBRT_ERROR_NONE;
	}

#pragma endregion

#pragma region UINT

	tpbrt_error_t tpbrt_param_as_uints(const tpbrt_param_t* const param, tpbrt_uint_array_t* const out_vals) {
			if (param == TPBRT_NULL || out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_value(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return TPBRT_ERROR_NONE;
			}

		out_vals->data = malloc(sizeof(tpbrt_int_t) * count);
			if (out_vals->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_vals->count	 = count;

		token.data		 = param->value.data;
		tpbrt_size_t idx = 0;
			while ((token = tpbrt_next_value(token.data, end)).data != TPBRT_NULL) {
					if (!tpbrt_read_uint_value(token, &out_vals->data[idx])) {
						tpbrt_uint_array_free(out_vals);
						return TPBRT_ERROR_PARSE_UINT;
					}
				++idx;
				token.data += token.size;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_uints_of_size(const tpbrt_param_t* const param, const tpbrt_size_t desired_size,
	  tpbrt_uint_array_t* const out_vals) {
			if (param == TPBRT_NULL || out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_uint_array_t values;
		const tpbrt_error_t err = tpbrt_param_as_uints(param, &values);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (values.count < desired_size) {
					if (values.data != TPBRT_NULL) { tpbrt_uint_array_free(&values); }
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

			if (values.count > desired_size) {
				tpbrt_uint_array_free(&values);
				return TPBRT_ERROR_TOO_MANY_VALUES;
			}

		*out_vals = values;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_uint(const tpbrt_param_t* const param, tpbrt_uint_t* const out_val) {
			if (param == TPBRT_NULL || out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY && param->value_type != TPBRT_PARAM_VALUE_TYPE_SINGLE) {
				return TPBRT_ERROR_INVALID_OBJECT_TYPE;
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				tpbrt_uint_array_t uints;
				const tpbrt_error_t err = tpbrt_param_as_uints_of_size(param, 1u, &uints);
					if (err != TPBRT_ERROR_NONE) { return err; }

				*out_val = uints.data[0];
				tpbrt_uint_array_free(&uints);
				return TPBRT_ERROR_NONE;
			}

		const tpbrt_string_t token = tpbrt_next_value(param->value.data, param->value.data + param->value.size);
			if (token.data == TPBRT_NULL) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }
			if (!tpbrt_read_uint_value(token, out_val)) { return TPBRT_ERROR_PARSE_UINT; }
		return TPBRT_ERROR_NONE;
	}

#pragma endregion

#pragma region RGB

	tpbrt_error_t tpbrt_param_as_rgbs(const tpbrt_param_t* const param, tpbrt_rgb_array_t* const out_vals) {
		return tpbrt_param_as_vec3s(param, out_vals);
	}

	tpbrt_error_t tpbrt_param_as_rgbs_of_size(const tpbrt_param_t* const param, const tpbrt_size_t desired_size,
	  tpbrt_rgb_array_t* const out_vals) {
		return tpbrt_param_as_vec3s_of_size(param, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_param_as_rgb(const tpbrt_param_t* const param, tpbrt_rgb_t* const out_val) {
		return tpbrt_param_as_vec3(param, out_val);
	}

#pragma endregion

#pragma region WAVELENGTH

	tpbrt_error_t tpbrt_param_as_wavelengths(const tpbrt_param_t* const param, tpbrt_wavelength_array_t* const out_vals) {
			if (param == TPBRT_NULL || out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_value(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return TPBRT_ERROR_NONE;
			}

			if (count % 2 != 0) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

		count /= 2;

		out_vals->data = malloc(sizeof(tpbrt_wavelength_t) * count);
			if (out_vals->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_vals->count	 = count;

		token.data		 = param->value.data;
		tpbrt_size_t idx = 0;
			while ((token = tpbrt_next_value(token.data, end)).data != TPBRT_NULL) {
					if (!tpbrt_read_float_value(token, &out_vals->data[idx].wavelength)) {
						tpbrt_wavelength_array_free(out_vals);
						return TPBRT_ERROR_PARSE_UINT;
					}
				token.data += token.size;

				token = tpbrt_next_value(token.data, end);
					if (!tpbrt_read_float_value(token, &out_vals->data[idx].value)) {
						tpbrt_wavelength_array_free(out_vals);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				token.data += token.size;

				++idx;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_wavelengths_of_size(const tpbrt_param_t* const param, const tpbrt_size_t desired_size,
	  tpbrt_wavelength_array_t* const out_vals) {
			if (param == TPBRT_NULL || out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_wavelength_array_t values;
		const tpbrt_error_t err = tpbrt_param_as_wavelengths(param, &values);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (values.count < desired_size) {
					if (values.data != TPBRT_NULL) { tpbrt_wavelength_array_free(&values); }
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

			if (values.count > desired_size) {
				tpbrt_wavelength_array_free(&values);
				return TPBRT_ERROR_TOO_MANY_VALUES;
			}

		*out_vals = values;
		return TPBRT_ERROR_NONE;
	}

#pragma endregion

#pragma region SPECTRUM

	tpbrt_error_t tpbrt_param_as_spectrum(const tpbrt_param_t* const param, tpbrt_spectrum_t* const out_val) {
			if (param == TPBRT_NULL || out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->type == TPBRT_PARAM_TYPE_RGB) {
				out_val->type = TPBRT_SPECTRUM_TYPE_RGB;
				return tpbrt_param_as_rgb(param, &out_val->as.rgb);
			}

			if (param->type == TPBRT_PARAM_TYPE_BLACKBODY) {
				out_val->type = TPBRT_SPECTRUM_TYPE_BLACKBODY;
				return tpbrt_param_as_float(param, &out_val->as.blackbody);
			}

			if (param->type == TPBRT_PARAM_TYPE_SPECTRUM) {
				// wavelengths list
				tpbrt_error_t err = tpbrt_param_as_wavelengths(param, &out_val->as.wavelengths);
				// possible error: PARSE_FLOAT, PARSE_UINT, MISSING_REQUIRED_PARAMETER, ERROR_INVALID_POINTER, INVALID_OBJECT_TYPE
					if (err == TPBRT_ERROR_NONE) {
						out_val->type = TPBRT_SPECTRUM_TYPE_WAVELENGTH;
						return TPBRT_ERROR_NONE;
					}

					if (err != TPBRT_ERROR_INVALID_OBJECT_TYPE) { return err; }

					if (param->value_type != TPBRT_PARAM_VALUE_TYPE_STRING) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

				// builtin
				tpbrt_spectrum_builtin_t builtin;
				err = tpbrt_spectrum_builtin_from_string(param->value, &builtin);
					// possible error: UNKNOWN_SPECTRUM_BUILTIN
					if (err == TPBRT_ERROR_NONE) {
						out_val->type		= TPBRT_SPECTRUM_TYPE_BUILTIN;
						out_val->as.builtin = builtin;
						return TPBRT_ERROR_NONE;
					}

				// filename
				out_val->type = TPBRT_SPECTRUM_TYPE_FILE;
				return tpbrt_string_copy(&out_val->as.file_name, param->value);
			}

		return TPBRT_ERROR_INVALID_OBJECT_TYPE;
	}

#pragma endregion

#pragma region VEC2

	tpbrt_error_t tpbrt_param_as_vec2s(const tpbrt_param_t* const param, tpbrt_vec2_array_t* const out_vals) {
			if (param == TPBRT_NULL || out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_value(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return TPBRT_ERROR_NONE;
			}

			if (count % 2 != 0) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

		count /= 2;

		out_vals->data = malloc(sizeof(tpbrt_vec2_t) * count);
			if (out_vals->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_vals->count	 = count;

		token.data		 = param->value.data;
		tpbrt_size_t idx = 0;
			while ((token = tpbrt_next_value(token.data, end)).data != TPBRT_NULL) {
					if (!tpbrt_read_float_value(token, &out_vals->data[idx].x)) {
						tpbrt_vec2_array_free(out_vals);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				token.data += token.size;

				token = tpbrt_next_value(token.data, end);
					if (!tpbrt_read_float_value(token, &out_vals->data[idx].y)) {
						tpbrt_vec2_array_free(out_vals);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				token.data += token.size;

				++idx;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_vec2s_of_size(const tpbrt_param_t* const param, const tpbrt_size_t desired_size,
	  tpbrt_vec2_array_t* const out_vals) {
			if (param == TPBRT_NULL || out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_vec2_array_t values;
		const tpbrt_error_t err = tpbrt_param_as_vec2s(param, &values);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (values.count < desired_size) {
					if (values.data != TPBRT_NULL) { tpbrt_vec2_array_free(&values); }
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

			if (values.count > desired_size) {
				tpbrt_vec2_array_free(&values);
				return TPBRT_ERROR_TOO_MANY_VALUES;
			}

		*out_vals = values;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_vec2(const tpbrt_param_t* const param, tpbrt_vec2_t* const out_val) {
			if (param == TPBRT_NULL || out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_float_array_t floats;
		const tpbrt_error_t err = tpbrt_param_as_floats_of_size(param, 2u, &floats);
			if (err != TPBRT_ERROR_NONE) { return err; }

		memcpy(out_val->v, floats.data, sizeof(tpbrt_float_t) * 2u);
		tpbrt_float_array_free(&floats);
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_point2s(const tpbrt_param_t* const param, tpbrt_point2_array_t* const out_vals) {
		return tpbrt_param_as_vec2s(param, out_vals);
	}

	tpbrt_error_t tpbrt_param_as_point2s_of_size(const tpbrt_param_t* const param, const tpbrt_size_t desired_size,
	  tpbrt_point2_array_t* const out_vals) {
		return tpbrt_param_as_vec2s_of_size(param, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_param_as_point2(const tpbrt_param_t* const param, tpbrt_point2_t* const out_val) {
		return tpbrt_param_as_vec2(param, out_val);
	}

#pragma endregion

#pragma region VEC3

	tpbrt_error_t tpbrt_param_as_vec3s(const tpbrt_param_t* const param, tpbrt_vec3_array_t* const out_vals) {
			if (param == TPBRT_NULL || out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_size_t count		= 0;
		tpbrt_string_t token	= param->value;
		const tpbrt_char_t* end = param->value.data + param->value.size;

			while ((token = tpbrt_next_value(token.data, end)).data != TPBRT_NULL) {
				++count;
				token.data += token.size;
			}

			if (count == 0) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return TPBRT_ERROR_NONE;
			}

			if (count % 3 != 0) {
				out_vals->data	= TPBRT_NULL;
				out_vals->count = 0;
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

		count /= 3;

		out_vals->data = malloc(sizeof(tpbrt_vec3_t) * count);
			if (out_vals->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }
		out_vals->count	 = count;

		token.data		 = param->value.data;
		tpbrt_size_t idx = 0;
			while ((token = tpbrt_next_value(token.data, end)).data != TPBRT_NULL) {
					if (!tpbrt_read_float_value(token, &out_vals->data[idx].x)) {
						tpbrt_vec3_array_free(out_vals);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				token.data += token.size;

				token = tpbrt_next_value(token.data, end);
					if (!tpbrt_read_float_value(token, &out_vals->data[idx].y)) {
						tpbrt_vec3_array_free(out_vals);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				token.data += token.size;

				token = tpbrt_next_value(token.data, end);
					if (!tpbrt_read_float_value(token, &out_vals->data[idx].z)) {
						tpbrt_vec3_array_free(out_vals);
						return TPBRT_ERROR_PARSE_FLOAT;
					}
				token.data += token.size;

				++idx;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_vec3s_of_size(const tpbrt_param_t* const param, const tpbrt_size_t desired_size,
	  tpbrt_vec3_array_t* const out_vals) {
			if (param == TPBRT_NULL || out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_vec3_array_t values;
		const tpbrt_error_t err = tpbrt_param_as_vec3s(param, &values);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (values.count < desired_size) {
					if (values.data != TPBRT_NULL) { tpbrt_vec3_array_free(&values); }
				return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
			}

			if (values.count > desired_size) {
				tpbrt_vec3_array_free(&values);
				return TPBRT_ERROR_TOO_MANY_VALUES;
			}

		*out_vals = values;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_vec3(const tpbrt_param_t* const param, tpbrt_vec3_t* const out_val) {
			if (param == TPBRT_NULL || out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (param->value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY) { return TPBRT_ERROR_INVALID_OBJECT_TYPE; }

		tpbrt_float_array_t floats;
		const tpbrt_error_t err = tpbrt_param_as_floats_of_size(param, 3u, &floats);
			if (err != TPBRT_ERROR_NONE) { return err; }

		memcpy(out_val->v, floats.data, sizeof(tpbrt_float_t) * 3u);
		tpbrt_float_array_free(&floats);
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_param_as_vecs(const tpbrt_param_t* const param, tpbrt_vec_array_t* const out_vals) {
		return tpbrt_param_as_vec3s(param, out_vals);
	}

	tpbrt_error_t tpbrt_param_as_vecs_of_size(const tpbrt_param_t* const param, const tpbrt_size_t desired_size,
	  tpbrt_vec_array_t* const out_vals) {
		return tpbrt_param_as_vec3s_of_size(param, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_param_as_vec(const tpbrt_param_t* const param, tpbrt_vec_t* const out_val) {
		return tpbrt_param_as_vec3(param, out_val);
	}

	tpbrt_error_t tpbrt_param_as_point3s(const tpbrt_param_t* const param, tpbrt_point3_array_t* const out_vals) {
		return tpbrt_param_as_vec3s(param, out_vals);
	}

	tpbrt_error_t tpbrt_param_as_point3s_of_size(const tpbrt_param_t* const param, const tpbrt_size_t desired_size,
	  tpbrt_point3_array_t* const out_vals) {
		return tpbrt_param_as_vec3s_of_size(param, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_param_as_point3(const tpbrt_param_t* const param, tpbrt_point3_t* const out_val) {
		return tpbrt_param_as_vec3(param, out_val);
	}

	tpbrt_error_t tpbrt_param_as_points(const tpbrt_param_t* const param, tpbrt_point_array_t* const out_vals) {
		return tpbrt_param_as_point3s(param, out_vals);
	}

	tpbrt_error_t tpbrt_param_as_points_of_size(const tpbrt_param_t* const param, const tpbrt_size_t desired_size,
	  tpbrt_point_array_t* const out_vals) {
		return tpbrt_param_as_vec3s_of_size(param, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_param_as_point(const tpbrt_param_t* const param, tpbrt_point_t* const out_val) {
		return tpbrt_param_as_point3(param, out_val);
	}

	tpbrt_error_t tpbrt_param_as_normal3s(const tpbrt_param_t* const param, tpbrt_normal3_array_t* const out_vals) {
		return tpbrt_param_as_vec3s(param, out_vals);
	}

	tpbrt_error_t tpbrt_param_as_normal3s_of_size(const tpbrt_param_t* const param, const tpbrt_size_t desired_size,
	  tpbrt_normal3_array_t* const out_vals) {
		return tpbrt_param_as_vec3s_of_size(param, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_param_as_normal3(const tpbrt_param_t* const param, tpbrt_normal3_t* const out_val) {
		return tpbrt_param_as_vec3(param, out_val);
	}

	tpbrt_error_t tpbrt_param_as_normals(const tpbrt_param_t* const param, tpbrt_normal_array_t* const out_vals) {
		return tpbrt_param_as_normal3s(param, out_vals);
	}

	tpbrt_error_t tpbrt_param_as_normals_of_size(const tpbrt_param_t* const param, const tpbrt_size_t desired_size,
	  tpbrt_normal_array_t* const out_vals) {
		return tpbrt_param_as_vec3s_of_size(param, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_param_as_normal(const tpbrt_param_t* const param, tpbrt_normal_t* const out_val) {
		return tpbrt_param_as_normal3(param, out_val);
	}

#pragma endregion

#pragma region TEXTURE

	tpbrt_error_t tpbrt_param_as_texture(const tpbrt_param_t* const param, const tpbrt_textures_list_t textures_list,
	  tpbrt_texture_handle_t* const out_val) {
			if (param == TPBRT_NULL || out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_error_t err = TPBRT_ERROR_NONE;
			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_SINGLE || param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				// float
				err = tpbrt_param_as_float(param, &out_val->as.float_value);
					if (err == TPBRT_ERROR_NONE) {
						out_val->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
						return TPBRT_ERROR_NONE;
					}
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_STRING || param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				// texture | spectrum builtin
				tpbrt_string_t temp_string;
				err = tpbrt_param_as_string(param, &temp_string);
					if (err == TPBRT_ERROR_NONE) {
						// spectrum builtin
						tpbrt_spectrum_builtin_t spectrum_builtin;
						err = tpbrt_spectrum_builtin_from_string(temp_string, &spectrum_builtin);
							if (err == TPBRT_ERROR_NONE) {
								tpbrt_string_free(&temp_string);
								*out_val = TPBRT_TEXTURE_HANDLE_SPECTRUM(spectrum_builtin);
								return TPBRT_ERROR_NONE;
							}

						// texture
						err = tpbrt_textures_list_get_handle(textures_list, temp_string, out_val);
						tpbrt_string_free(&temp_string);
						return err;
					}
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				// rgb
				err = tpbrt_param_as_rgb(param, &out_val->as.rgb);
					if (err == TPBRT_ERROR_NONE) {
						out_val->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_RGB;
						return TPBRT_ERROR_NONE;
					}
			}

		out_val->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE;
		return err;
	}

	tpbrt_error_t tpbrt_param_as_texture_of_type(const tpbrt_param_t* const param, const tpbrt_textures_list_t textures_list,
	  const tpbrt_texture_type_t texture_type, tpbrt_texture_handle_t* out_val) {
			if (param == TPBRT_NULL || out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_error_t err = TPBRT_ERROR_NONE;
			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_SINGLE || param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				// float
				err = tpbrt_param_as_float(param, &out_val->as.float_value);
					if (err == TPBRT_ERROR_NONE) {
						out_val->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
						return TPBRT_ERROR_NONE;
					}
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_STRING || param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				// texture | spectrum builtin
				tpbrt_string_t temp_string;
				err = tpbrt_param_as_string(param, &temp_string);
					if (err == TPBRT_ERROR_NONE) {
						// spectrum builtin
						tpbrt_spectrum_builtin_t spectrum_builtin;
						err = tpbrt_spectrum_builtin_from_string(temp_string, &spectrum_builtin);
							if (err == TPBRT_ERROR_NONE) {
								tpbrt_string_free(&temp_string);
								out_val->value_type			 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM_BUILTIN;
								out_val->as.spectrum_builtin = spectrum_builtin;
								return TPBRT_ERROR_NONE;
							}

						// texture
						err = tpbrt_textures_list_get_handle_with_type(textures_list, temp_string, texture_type, out_val);
						tpbrt_string_free(&temp_string);
						return err;
					}
			}

			if (param->value_type == TPBRT_PARAM_VALUE_TYPE_ARRAY) {
				// rgb
				err = tpbrt_param_as_rgb(param, &out_val->as.rgb);
					if (err == TPBRT_ERROR_NONE) {
						out_val->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_RGB;
						return TPBRT_ERROR_NONE;
					}
			}

		out_val->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE;
		return err;
	}

#pragma endregion

#pragma endregion

#pragma region PARAMS_LIST
#define TPBRT_EMPTY_PARAMS_LIST (tpbrt_params_list_t) { .params = TPBRT_NULL, .count = 0, .capacity = 0 }

	tpbrt_error_t tpbrt_params_list_init(tpbrt_params_list_t* const params_list) {
			if (params_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*params_list = TPBRT_EMPTY_PARAMS_LIST;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_params_list_reserve(tpbrt_params_list_t* const params_list, const tpbrt_size_t new_capacity) {
			if (params_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (params_list->capacity >= new_capacity) { return TPBRT_ERROR_NONE; }

		tpbrt_param_t* new_list = realloc(params_list->params, sizeof(tpbrt_param_t) * new_capacity);
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		*params_list = (tpbrt_params_list_t) { .params = new_list, .count = params_list->count, .capacity = new_capacity };
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_params_list_resize(tpbrt_params_list_t* const params_list, const tpbrt_size_t new_size) {
			if (params_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_param_t* new_list = realloc(params_list->params, sizeof(tpbrt_param_t) * new_size);
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		*params_list = (tpbrt_params_list_t) { .params = new_list, .count = new_size, .capacity = new_size };
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_params_list_append(tpbrt_params_list_t* const params_list, const tpbrt_param_t* const param) {
			if (param == TPBRT_NULL || params_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_error_t err = tpbrt_params_list_reserve(params_list, params_list->count + 1);
			if (err != TPBRT_ERROR_NONE) { return err; }

		params_list->params[params_list->count] = *param;
		++params_list->count;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_params_list_extend(tpbrt_params_list_t* const dst_params_list,
	  const tpbrt_params_list_t src_params_list) {
			if (dst_params_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_params_list_reserve(dst_params_list, dst_params_list->count + src_params_list.count);

			for (tpbrt_size_t i = 0; i < src_params_list.count; i++) {
				tpbrt_param_t* existing;
					if (tpbrt_params_list_get(*dst_params_list, src_params_list.params[i].name, &existing) == TPBRT_ERROR_NONE) {
						*existing = src_params_list.params[i];
						continue;
					}

				const tpbrt_error_t err = tpbrt_params_list_append(dst_params_list, &src_params_list.params[i]);
					if (err != TPBRT_ERROR_NONE) { return err; }
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_params_list_append_missing(tpbrt_params_list_t* const dst_params_list,
	  const tpbrt_params_list_t src_params_list) {
			if (dst_params_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_params_list_reserve(dst_params_list, dst_params_list->count + src_params_list.count);

			for (tpbrt_size_t i = 0; i < src_params_list.count; i++) {
				tpbrt_param_t* existing;
					if (tpbrt_params_list_get(*dst_params_list, src_params_list.params[i].name, &existing) == TPBRT_ERROR_NONE) {
						continue;
					}

				const tpbrt_error_t err = tpbrt_params_list_append(dst_params_list, &src_params_list.params[i]);
					if (err != TPBRT_ERROR_NONE) { return err; }
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_params_list_get(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_param_t** const param) {
			if (param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(param_name)) { return TPBRT_ERROR_INVALID_PARAM_NAME; }

			for (tpbrt_size_t i = 0; i < params_list.count; i++) {
					if (tpbrt_string_equals(params_list.params[i].name, param_name)) {
						*param = &params_list.params[i];
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_NOT_FOUND;
	}

	tpbrt_error_t tpbrt_params_list_get_const(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_param_t** const param) {
		tpbrt_param_t* non_const_param = TPBRT_NULL;
		const tpbrt_error_t err		   = tpbrt_params_list_get(params_list, param_name, &non_const_param);
		*param						   = (const tpbrt_param_t*)non_const_param;
		return err;
	}

	void tpbrt_params_list_free(tpbrt_params_list_t* const params_list) {
			if (params_list == TPBRT_NULL || params_list->params == TPBRT_NULL) { return; }

		free(params_list->params);
		*params_list = TPBRT_EMPTY_PARAMS_LIST;
	}

	tpbrt_bool_t tpbrt_params_list_empty(const tpbrt_params_list_t params_list) {
		return params_list.params == TPBRT_NULL || params_list.count == 0 ? TPBRT_TRUE : TPBRT_FALSE;
	}

#pragma region STRING

	tpbrt_error_t tpbrt_params_list_get_strings(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_string_array_t* const out_vals) {
			if (out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				*out_vals = TPBRT_EMPTY_STRING_ARRAY;
				return err;
			}

		return tpbrt_param_as_strings(param, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_strings_of_size(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_size_t desired_size, tpbrt_string_array_t* const out_vals) {
			if (out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				*out_vals = TPBRT_EMPTY_STRING_ARRAY;
				return err;
			}

		return tpbrt_param_as_strings_of_size(param, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_string(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_string_t* const out_val) {
			if (out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_string(param, out_val);
	}

#pragma endregion

#pragma region BOOL

	tpbrt_error_t tpbrt_params_list_get_bools(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_bool_array_t* const out_vals) {
			if (out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				*out_vals = TPBRT_EMPTY_BOOL_ARRAY;
				return err;
			}

		return tpbrt_param_as_bools(param, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_bools_of_size(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_size_t desired_size, tpbrt_bool_array_t* const out_vals) {
			if (out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				*out_vals = TPBRT_EMPTY_BOOL_ARRAY;
				return err;
			}

		return tpbrt_param_as_bools_of_size(param, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_bool(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_bool_t default_val, tpbrt_bool_t* const out_val) {
			if (out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err == TPBRT_ERROR_NOT_FOUND) {
				*out_val = default_val;
				return TPBRT_ERROR_NONE;
			}

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_bool(param, out_val);
	}

#pragma endregion

#pragma region FLOAT

	tpbrt_error_t tpbrt_params_list_get_floats(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_float_array_t* const out_vals) {
			if (out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				*out_vals = TPBRT_EMPTY_FLOAT_ARRAY;
				return err;
			}

		return tpbrt_param_as_floats(param, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_floats_of_size(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_size_t desired_size, tpbrt_float_array_t* const out_vals) {
			if (out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				*out_vals = TPBRT_EMPTY_FLOAT_ARRAY;
				return err;
			}

		return tpbrt_param_as_floats_of_size(param, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_float(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_float_t default_val, tpbrt_float_t* const out_val) {
			if (out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err == TPBRT_ERROR_NOT_FOUND) {
				*out_val = default_val;
				return TPBRT_ERROR_NONE;
			}

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_float(param, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_opt_float(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_opt_float_t* const out_val) {
			if (out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				out_val->has_value = TPBRT_FALSE;
				return err == TPBRT_ERROR_NOT_FOUND ? TPBRT_ERROR_NONE : err;
			}

		err = tpbrt_param_as_float(param, &out_val->value);
			if (err != TPBRT_ERROR_NONE) {
				out_val->has_value = TPBRT_FALSE;
				return err;
			}

		out_val->has_value = TPBRT_TRUE;
		return TPBRT_ERROR_NONE;
	}

#pragma endregion

#pragma region INT

	tpbrt_error_t tpbrt_params_list_get_ints(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_int_array_t* const out_vals) {
			if (out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				*out_vals = TPBRT_EMPTY_INT_ARRAY;
				return err;
			}

		return tpbrt_param_as_ints(param, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_ints_of_size(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_size_t desired_size, tpbrt_int_array_t* const out_vals) {
			if (out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				*out_vals = TPBRT_EMPTY_INT_ARRAY;
				return err;
			}

		return tpbrt_param_as_ints_of_size(param, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_int(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_int_t default_val, tpbrt_int_t* const out_val) {
			if (out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err == TPBRT_ERROR_NOT_FOUND) {
				*out_val = default_val;
				return TPBRT_ERROR_NONE;
			}

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_int(param, out_val);
	}

#pragma endregion

#pragma region UINT

	tpbrt_error_t tpbrt_params_list_get_uints(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_uint_array_t* const out_vals) {
			if (out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				*out_vals = TPBRT_EMPTY_UINT_ARRAY;
				return err;
			}

		return tpbrt_param_as_uints(param, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_uints_of_size(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_size_t desired_size, tpbrt_uint_array_t* const out_vals) {
			if (out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				*out_vals = TPBRT_EMPTY_UINT_ARRAY;
				return err;
			}

		return tpbrt_param_as_uints_of_size(param, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_uint(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_uint_t default_val, tpbrt_uint_t* const out_val) {
			if (out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err == TPBRT_ERROR_NOT_FOUND) {
				*out_val = default_val;
				return TPBRT_ERROR_NONE;
			}

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_uint(param, out_val);
	}

#pragma endregion

#pragma region RGB

	tpbrt_error_t tpbrt_params_list_get_rgbs(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_rgb_array_t* const out_vals) {
		return tpbrt_params_list_get_vec3s(params_list, param_name, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_rgbs_of_size(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_size_t desired_size, tpbrt_rgb_array_t* const out_vals) {
		return tpbrt_params_list_get_vec3s_of_size(params_list, param_name, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_rgb(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_rgb_t* const out_val) {
		return tpbrt_params_list_get_vec3(params_list, param_name, out_val);
	}

#pragma endregion

#pragma region WAVELENGTH

	tpbrt_error_t tpbrt_params_list_get_wavelengths(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_wavelength_array_t* const out_vals) {
			if (out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				*out_vals = TPBRT_EMPTY_WAVELENGTHS_ARRAY;
				return err;
			}

		return tpbrt_param_as_wavelengths(param, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_wavelengths_of_size(const tpbrt_params_list_t params_list,
	  const tpbrt_string_t param_name, const tpbrt_size_t desired_size, tpbrt_wavelength_array_t* const out_vals) {
			if (out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				*out_vals = TPBRT_EMPTY_WAVELENGTHS_ARRAY;
				return err;
			}

		return tpbrt_param_as_wavelengths_of_size(param, desired_size, out_vals);
	}

#pragma endregion

#pragma region SPECTRUM

	tpbrt_error_t tpbrt_params_list_get_spectrum(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_spectrum_t* const out_val) {
			if (out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_spectrum(param, out_val);
	}

#pragma endregion

#pragma region VEC2

	tpbrt_error_t tpbrt_params_list_get_vec2s(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_vec2_array_t* const out_vals) {
			if (out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				*out_vals = TPBRT_EMPTY_VEC2_ARRAY;
				return err;
			}

		return tpbrt_param_as_vec2s(param, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_vec2s_of_size(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_size_t desired_size, tpbrt_vec2_array_t* const out_vals) {
			if (out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				*out_vals = TPBRT_EMPTY_VEC2_ARRAY;
				return err;
			}

		return tpbrt_param_as_vec2s_of_size(param, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_vec2(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_vec2_t* const out_val) {
			if (out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_vec2(param, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_point2s(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_point2_array_t* const out_vals) {
		return tpbrt_params_list_get_vec2s(params_list, param_name, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_point2s_of_size(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_size_t desired_size, tpbrt_point2_array_t* const out_vals) {
		return tpbrt_params_list_get_vec2s_of_size(params_list, param_name, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_point2(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_point2_t* const out_val) {
		return tpbrt_params_list_get_vec2(params_list, param_name, out_val);
	}

#pragma endregion

#pragma region VEC3

	tpbrt_error_t tpbrt_params_list_get_vec3s(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_vec3_array_t* const out_vals) {
			if (out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				*out_vals = TPBRT_EMPTY_VEC3_ARRAY;
				return err;
			}

		return tpbrt_param_as_vec3s(param, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_vec3s_of_size(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_size_t desired_size, tpbrt_vec3_array_t* const out_vals) {
			if (out_vals == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				*out_vals = TPBRT_EMPTY_VEC3_ARRAY;
				return err;
			}

		return tpbrt_param_as_vec3s_of_size(param, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_vec3(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_vec3_t* const out_val) {
			if (out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_param_as_vec3(param, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_vecs(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_vec_array_t* const out_vals) {
		return tpbrt_params_list_get_vec3s(params_list, param_name, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_vecs_of_size(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_size_t desired_size, tpbrt_vec_array_t* const out_vals) {
		return tpbrt_params_list_get_vec3s_of_size(params_list, param_name, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_vec(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_vec_t* const out_val) {
		return tpbrt_params_list_get_vec3(params_list, param_name, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_point3s(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_point3_array_t* const out_vals) {
		return tpbrt_params_list_get_vec3s(params_list, param_name, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_point3s_of_size(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_size_t desired_size, tpbrt_point3_array_t* const out_vals) {
		return tpbrt_params_list_get_vec3s_of_size(params_list, param_name, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_point3(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_point3_t* const out_val) {
		return tpbrt_params_list_get_vec3(params_list, param_name, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_points(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_point_array_t* const out_vals) {
		return tpbrt_params_list_get_point3s(params_list, param_name, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_points_of_size(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_size_t desired_size, tpbrt_point_array_t* const out_vals) {
		return tpbrt_params_list_get_vec3s_of_size(params_list, param_name, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_point(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_point_t* const out_val) {
		return tpbrt_params_list_get_point3(params_list, param_name, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_normal3s(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_normal3_array_t* const out_vals) {
		return tpbrt_params_list_get_vec3s(params_list, param_name, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_normal3s_of_size(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_size_t desired_size, tpbrt_normal3_array_t* const out_vals) {
		return tpbrt_params_list_get_vec3s_of_size(params_list, param_name, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_normal3(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_normal3_t* const out_val) {
		return tpbrt_params_list_get_vec3(params_list, param_name, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_normals(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_normal_array_t* const out_vals) {
		return tpbrt_params_list_get_normal3s(params_list, param_name, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_normals_of_size(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_size_t desired_size, tpbrt_normal_array_t* const out_vals) {
		return tpbrt_params_list_get_vec3s_of_size(params_list, param_name, desired_size, out_vals);
	}

	tpbrt_error_t tpbrt_params_list_get_normal(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  tpbrt_normal_t* const out_val) {
		return tpbrt_params_list_get_normal3(params_list, param_name, out_val);
	}

#pragma endregion

#pragma region TEXTURE

	tpbrt_error_t tpbrt_params_list_get_texture(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_textures_list_t textures_list, tpbrt_texture_handle_t* const out_val) {
			if (out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				out_val->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE;
				return err;
			}

		return tpbrt_param_as_texture(param, textures_list, out_val);
	}

	tpbrt_error_t tpbrt_params_list_get_texture_of_type(const tpbrt_params_list_t params_list, const tpbrt_string_t param_name,
	  const tpbrt_textures_list_t textures_list, const tpbrt_texture_type_t texture_type, tpbrt_texture_handle_t* const out_val) {
			if (out_val == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_param_t* param;
		const tpbrt_error_t err = tpbrt_params_list_get_const(params_list, param_name, &param);

			if (err != TPBRT_ERROR_NONE) {
				out_val->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE;
				return err;
			}

		return tpbrt_param_as_texture_of_type(param, textures_list, texture_type, out_val);
	}

#pragma endregion

#pragma endregion

#ifdef __cplusplus
}
#endif