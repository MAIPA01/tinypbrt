#pragma once
#ifndef _TINYPBRT_PARAM_INTERNAL_H_
#define _TINYPBRT_PARAM_INTERNAL_H_

#include <tinypbrt/detail/color.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/math.h>
#include <tinypbrt/detail/param.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region PARAM
	tpbrt_error_t tpbrt_create_param(const tpbrt_string_t* declaration, tpbrt_param_value_type_t value_type,
	  const tpbrt_string_t* value_str, tpbrt_param_t** param);
	void tpbrt_free_param(tpbrt_param_t** param);

	tpbrt_error_t tpbrt_param_as_floats(const tpbrt_param_t* param, tpbrt_float_array_t* out_floats);
	tpbrt_error_t tpbrt_param_as_ints(const tpbrt_param_t* param, tpbrt_int_array_t* out_ints);
	tpbrt_error_t tpbrt_param_as_uints(const tpbrt_param_t* param, tpbrt_uint_array_t* out_uints);

	tpbrt_error_t tpbrt_param_as_float(const tpbrt_param_t* param, tpbrt_float_t* out_float);
	tpbrt_error_t tpbrt_param_as_int(const tpbrt_param_t* param, tpbrt_int_t* out_int);
	tpbrt_error_t tpbrt_param_as_uint(const tpbrt_param_t* param, tpbrt_uint_t* out_uint);
	tpbrt_error_t tpbrt_param_as_bool(const tpbrt_param_t* param, tpbrt_bool_t* out_bool);

	tpbrt_error_t tpbrt_param_as_rgb(const tpbrt_param_t* param, tpbrt_rgb_t* out_rgb);
	tpbrt_error_t tpbrt_param_as_wavelengths(const tpbrt_param_t* param, tpbrt_wavelength_array_t* out_wavelengths);
	tpbrt_error_t tpbrt_param_as_spectrum(const tpbrt_param_t* param, tpbrt_spectrum_t* out_spectrum);
#pragma endregion

#pragma region PARAMS_LIST
	tpbrt_error_t tpbrt_create_empty_params_list(tpbrt_params_list_t** params_list);
	tpbrt_error_t tpbrt_params_list_add_param(tpbrt_params_list_t* params_list, const tpbrt_param_t* param);
	tpbrt_error_t tpbrt_params_list_extend(tpbrt_params_list_t* dest_params_list, const tpbrt_params_list_t* src_params_list);
	tpbrt_error_t tpbrt_params_list_get_param(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_param_t** param);
	tpbrt_error_t tpbrt_params_list_get_param_const(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  const tpbrt_param_t** param);
	void tpbrt_free_params_list(tpbrt_params_list_t** params_list);

	tpbrt_size_t tpbrt_params_list_size(const tpbrt_params_list_t* params_list);
	tpbrt_bool_t tpbrt_params_list_is_empty(const tpbrt_params_list_t* params_list);

	tpbrt_error_t tpbrt_params_list_get_floats(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_float_array_t* out_floats);
	tpbrt_error_t tpbrt_params_list_get_ints(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_int_array_t* out_ints);
	tpbrt_error_t tpbrt_params_list_get_uints(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_uint_array_t* out_uints);

	tpbrt_error_t tpbrt_params_list_get_float(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_float_t default_val, tpbrt_float_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_int(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_int_t default_val, tpbrt_int_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_uint(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_uint_t default_val, tpbrt_uint_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_bool(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_bool_t default_val, tpbrt_bool_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_string(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_string_t* out_val);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_PARAM_INTERNAL_H_
