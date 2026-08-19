#pragma once
#ifndef _TINYPBRT_PARAM_INTERNAL_H_
#define _TINYPBRT_PARAM_INTERNAL_H_

#include <tinypbrt/detail/param.h>

#include <tinypbrt/detail/color.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/math.h>

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

	tpbrt_error_t tpbrt_param_as_vec2(const tpbrt_param_t* param, tpbrt_vec2_t* out_vec2);
	tpbrt_error_t tpbrt_param_as_point2(const tpbrt_param_t* param, tpbrt_point2_t* out_point2);

	tpbrt_error_t tpbrt_param_as_vec2s(const tpbrt_param_t* param, tpbrt_vec2_array_t* out_vec2s);
	tpbrt_error_t tpbrt_param_as_point2s(const tpbrt_param_t* param, tpbrt_point2_array_t* out_point2s);

	tpbrt_error_t tpbrt_param_as_vec3(const tpbrt_param_t* param, tpbrt_vec3_t* out_vec3);
	tpbrt_error_t tpbrt_param_as_vec(const tpbrt_param_t* param, tpbrt_vec_t* out_vec);
	tpbrt_error_t tpbrt_param_as_point3(const tpbrt_param_t* param, tpbrt_point3_t* out_point3);
	tpbrt_error_t tpbrt_param_as_point(const tpbrt_param_t* param, tpbrt_point_t* out_point);
	tpbrt_error_t tpbrt_param_as_normal3(const tpbrt_param_t* param, tpbrt_normal3_t* out_normal3);
	tpbrt_error_t tpbrt_param_as_normal(const tpbrt_param_t* param, tpbrt_normal_t* out_normal);

	tpbrt_error_t tpbrt_param_as_vec3s(const tpbrt_param_t* param, tpbrt_vec3_array_t* out_vec3s);
	tpbrt_error_t tpbrt_param_as_vecs(const tpbrt_param_t* param, tpbrt_vec_array_t* out_vecs);
	tpbrt_error_t tpbrt_param_as_point3s(const tpbrt_param_t* param, tpbrt_point3_array_t* out_point3s);
	tpbrt_error_t tpbrt_param_as_points(const tpbrt_param_t* param, tpbrt_point_array_t* out_points);
	tpbrt_error_t tpbrt_param_as_normal3s(const tpbrt_param_t* param, tpbrt_normal3_array_t* out_normal3s);
	tpbrt_error_t tpbrt_param_as_normals(const tpbrt_param_t* param, tpbrt_normal_array_t* out_normals);
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
	  tpbrt_float_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_ints(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_int_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_uints(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_uint_array_t* out_vals);

	tpbrt_error_t tpbrt_params_list_get_float(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_float_t default_val, tpbrt_float_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_opt_float(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_opt_float_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_int(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_int_t default_val, tpbrt_int_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_uint(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_uint_t default_val, tpbrt_uint_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_bool(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_bool_t default_val, tpbrt_bool_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_string(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_string_t* out_val);

	tpbrt_error_t tpbrt_params_list_get_rgb(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_rgb_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_spectrum(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_spectrum_t* out_val);

	tpbrt_error_t tpbrt_params_list_get_vec2(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_vec2_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_point2(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_point2_t* out_val);

	tpbrt_error_t tpbrt_params_list_get_vec2s(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_vec2_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_point2s(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_point2_array_t* out_vals);

	tpbrt_error_t tpbrt_params_list_get_vec3(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_vec3_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_vec(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_vec_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_point3(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_point3_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_point(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_point_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_normal3(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_normal3_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_normal(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_normal_t* out_val);

	tpbrt_error_t tpbrt_params_list_get_vec3s(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_vec3_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_vecs(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_vec_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_point3s(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_point3_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_points(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_point_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_normal3s(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_normal3_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_normals(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name,
	  tpbrt_normal_array_t* out_vals);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_PARAM_INTERNAL_H_
