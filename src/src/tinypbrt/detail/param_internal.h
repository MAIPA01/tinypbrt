#pragma once
#ifndef _TINYPBRT_PARAM_INTERNAL_H_
#define _TINYPBRT_PARAM_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/error.h>

#include <tinypbrt/detail/texture_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region PARAM

	typedef enum tpbrt_param_type_t : uint8_t {
		TPBRT_PARAM_TYPE_INTEGER   = 0,
		TPBRT_PARAM_TYPE_FLOAT	   = 1,
		TPBRT_PARAM_TYPE_POINT2	   = 2,
		TPBRT_PARAM_TYPE_VECTOR2   = 3,
		TPBRT_PARAM_TYPE_POINT3	   = 4,
		TPBRT_PARAM_TYPE_POINT	   = 5,
		TPBRT_PARAM_TYPE_VECTOR3   = 6,
		TPBRT_PARAM_TYPE_VECTOR	   = 7,
		TPBRT_PARAM_TYPE_NORMAL3   = 8,
		TPBRT_PARAM_TYPE_NORMAL	   = 9,
		TPBRT_PARAM_TYPE_SPECTRUM  = 10,
		TPBRT_PARAM_TYPE_RGB	   = 11,
		TPBRT_PARAM_TYPE_BLACKBODY = 12,
		TPBRT_PARAM_TYPE_BOOL	   = 13,
		TPBRT_PARAM_TYPE_STRING	   = 14,
		TPBRT_PARAM_TYPE_TEXTURE   = 15,
		TPBRT_PARAM_TYPE_MAX_NUM   = 16,
	} tpbrt_param_type_t;

	typedef enum tpbrt_param_value_type_t : uint8_t {
		TPBRT_PARAM_VALUE_TYPE_SINGLE = 0,
		TPBRT_PARAM_VALUE_TYPE_ARRAY  = 1,
		TPBRT_PARAM_VALUE_TYPE_STRING = 2,
	} tpbrt_param_value_type_t;

	typedef struct tpbrt_param_t {
		tpbrt_string_t name;
		tpbrt_string_t value;
		tpbrt_param_type_t type				: 4;
		tpbrt_param_value_type_t value_type : 2;
	} tpbrt_param_t;

	tpbrt_error_t tpbrt_param_type_from_string(tpbrt_string_t type_str, tpbrt_param_type_t* type_out);

	tpbrt_error_t tpbrt_param_init(tpbrt_string_t declaration, tpbrt_param_value_type_t value_type, tpbrt_string_t value_str,
	  tpbrt_param_t* param);

#pragma region STRING
	tpbrt_error_t tpbrt_param_as_strings(const tpbrt_param_t* param, tpbrt_string_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_strings_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_string_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_string(const tpbrt_param_t* param, tpbrt_string_t* out_val);
#pragma endregion

#pragma region BOOL
	tpbrt_error_t tpbrt_param_as_bools(const tpbrt_param_t* param, tpbrt_bool_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_bools_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_bool_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_bool(const tpbrt_param_t* param, tpbrt_bool_t* out_val);
#pragma endregion

#pragma region FLOAT
	tpbrt_error_t tpbrt_param_as_floats(const tpbrt_param_t* param, tpbrt_float_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_floats_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_float_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_float(const tpbrt_param_t* param, tpbrt_float_t* out_val);
#pragma endregion

#pragma region INT
	tpbrt_error_t tpbrt_param_as_ints(const tpbrt_param_t* param, tpbrt_int_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_ints_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size, tpbrt_int_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_int(const tpbrt_param_t* param, tpbrt_int_t* out_val);
#pragma endregion

#pragma region UINT
	tpbrt_error_t tpbrt_param_as_uints(const tpbrt_param_t* param, tpbrt_uint_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_uints_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_uint_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_uint(const tpbrt_param_t* param, tpbrt_uint_t* out_val);
#pragma endregion

#pragma region RGB
	tpbrt_error_t tpbrt_param_as_rgbs(const tpbrt_param_t* param, tpbrt_rgb_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_rgbs_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size, tpbrt_rgb_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_rgb(const tpbrt_param_t* param, tpbrt_rgb_t* out_val);
#pragma endregion

#pragma region WAVELENGTH
	tpbrt_error_t tpbrt_param_as_wavelengths(const tpbrt_param_t* param, tpbrt_wavelength_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_wavelengths_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_wavelength_array_t* out_vals);
#pragma endregion

#pragma region SPECTRUM
	tpbrt_error_t tpbrt_param_as_spectrum(const tpbrt_param_t* param, tpbrt_spectrum_t* out_val);
#pragma endregion

#pragma region VEC2
	tpbrt_error_t tpbrt_param_as_vec2s(const tpbrt_param_t* param, tpbrt_vec2_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_vec2s_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_vec2_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_vec2(const tpbrt_param_t* param, tpbrt_vec2_t* out_val);

	tpbrt_error_t tpbrt_param_as_point2s(const tpbrt_param_t* param, tpbrt_point2_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_point2s_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_point2_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_point2(const tpbrt_param_t* param, tpbrt_point2_t* out_val);
#pragma endregion

#pragma region VEC3
	tpbrt_error_t tpbrt_param_as_vec3s(const tpbrt_param_t* param, tpbrt_vec3_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_vec3s_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_vec3_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_vec3(const tpbrt_param_t* param, tpbrt_vec3_t* out_val);

	tpbrt_error_t tpbrt_param_as_vecs(const tpbrt_param_t* param, tpbrt_vec_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_vecs_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size, tpbrt_vec_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_vec(const tpbrt_param_t* param, tpbrt_vec_t* out_val);

	tpbrt_error_t tpbrt_param_as_point3s(const tpbrt_param_t* param, tpbrt_point3_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_point3s_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_point3_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_point3(const tpbrt_param_t* param, tpbrt_point3_t* out_val);

	tpbrt_error_t tpbrt_param_as_points(const tpbrt_param_t* param, tpbrt_point_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_points_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_point_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_point(const tpbrt_param_t* param, tpbrt_point_t* out_val);

	tpbrt_error_t tpbrt_param_as_normal3s(const tpbrt_param_t* param, tpbrt_normal3_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_normal3s_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_normal3_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_normal3(const tpbrt_param_t* param, tpbrt_normal3_t* out_val);

	tpbrt_error_t tpbrt_param_as_normals(const tpbrt_param_t* param, tpbrt_normal_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_normals_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_normal_array_t* out_vals);
	tpbrt_error_t tpbrt_param_as_normal(const tpbrt_param_t* param, tpbrt_normal_t* out_val);
#pragma endregion

#pragma region TEXTURE
	tpbrt_error_t tpbrt_param_as_texture(const tpbrt_param_t* param, tpbrt_textures_list_t textures_list,
	  tpbrt_texture_handle_t* out_val);
	tpbrt_error_t tpbrt_param_as_texture_of_type(const tpbrt_param_t* param, tpbrt_textures_list_t textures_list,
	  tpbrt_texture_type_t texture_type, tpbrt_texture_handle_t* out_val);
#pragma endregion
#pragma endregion

#pragma region PARAMS_LIST

	typedef struct tpbrt_params_list_t {
		tpbrt_param_t* params;
		tpbrt_size_t count;
		tpbrt_size_t capacity;
	} tpbrt_params_list_t;

	tpbrt_error_t tpbrt_params_list_init(tpbrt_params_list_t* params_list);
	tpbrt_error_t tpbrt_params_list_reserve(tpbrt_params_list_t* params_list, tpbrt_size_t new_capacity);
	tpbrt_error_t tpbrt_params_list_resize(tpbrt_params_list_t* params_list, tpbrt_size_t new_size);
	tpbrt_error_t tpbrt_params_list_append(tpbrt_params_list_t* params_list, const tpbrt_param_t* param);
	tpbrt_error_t tpbrt_params_list_extend(tpbrt_params_list_t* dst_params_list, tpbrt_params_list_t src_params_list);
	tpbrt_error_t tpbrt_params_list_get(tpbrt_params_list_t params_list, tpbrt_string_t param_name, tpbrt_param_t** param);
	tpbrt_error_t tpbrt_params_list_get_const(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  const tpbrt_param_t** param);
	void tpbrt_params_list_free(tpbrt_params_list_t* params_list);

	tpbrt_bool_t tpbrt_params_list_empty(tpbrt_params_list_t params_list);

#pragma region STRING
	tpbrt_error_t tpbrt_params_list_get_strings(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_string_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_strings_of_size(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_string_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_string(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_string_t* out_val);
#pragma endregion

#pragma region BOOL
	tpbrt_error_t tpbrt_params_list_get_bools(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_bool_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_bools_of_size(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_bool_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_bool(tpbrt_params_list_t params_list, tpbrt_string_t param_name, tpbrt_bool_t default_val,
	  tpbrt_bool_t* out_val);
#pragma endregion

#pragma region FLOAT
	tpbrt_error_t tpbrt_params_list_get_floats(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_float_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_floats_of_size(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_float_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_float(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_float_t default_val, tpbrt_float_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_opt_float(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_opt_float_t* out_val);
#pragma endregion

#pragma region INT
	tpbrt_error_t tpbrt_params_list_get_ints(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_int_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_ints_of_size(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_int_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_int(tpbrt_params_list_t params_list, tpbrt_string_t param_name, tpbrt_int_t default_val,
	  tpbrt_int_t* out_val);
#pragma endregion

#pragma region UINT
	tpbrt_error_t tpbrt_params_list_get_uints(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_uint_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_uints_of_size(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_uint_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_uint(tpbrt_params_list_t params_list, tpbrt_string_t param_name, tpbrt_uint_t default_val,
	  tpbrt_uint_t* out_val);
#pragma endregion

#pragma region RGB
	tpbrt_error_t tpbrt_params_list_get_rgbs(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_rgb_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_rgbs_of_size(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_rgb_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_rgb(tpbrt_params_list_t params_list, tpbrt_string_t param_name, tpbrt_rgb_t* out_val);
#pragma endregion

#pragma region WAVELENGTH
	tpbrt_error_t tpbrt_params_list_get_wavelengths(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_wavelength_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_wavelengths_of_size(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_wavelength_array_t* out_vals);
#pragma endregion

#pragma region SPECTRUM
	tpbrt_error_t tpbrt_params_list_get_spectrum(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_spectrum_t* out_val);
#pragma endregion

#pragma region VEC2
	tpbrt_error_t tpbrt_params_list_get_vec2s(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_vec2_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_vec2s_of_size(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_vec2_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_vec2(tpbrt_params_list_t params_list, tpbrt_string_t param_name, tpbrt_vec2_t* out_val);

	tpbrt_error_t tpbrt_params_list_get_point2s(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_point2_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_point2s_of_size(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_point2_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_point2(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_point2_t* out_val);
#pragma endregion

#pragma region VEC3
	tpbrt_error_t tpbrt_params_list_get_vec3s(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_vec3_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_vec3s_of_size(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_vec3_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_vec3(tpbrt_params_list_t params_list, tpbrt_string_t param_name, tpbrt_vec3_t* out_val);

	tpbrt_error_t tpbrt_params_list_get_vecs(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_vec_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_vecs_of_size(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_vec_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_vec(tpbrt_params_list_t params_list, tpbrt_string_t param_name, tpbrt_vec_t* out_val);

	tpbrt_error_t tpbrt_params_list_get_point3s(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_point3_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_point3s_of_size(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_point3_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_point3(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_point3_t* out_val);

	tpbrt_error_t tpbrt_params_list_get_points(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_point_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_points_of_size(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_point_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_point(tpbrt_params_list_t params_list, tpbrt_string_t param_name, tpbrt_point_t* out_val);

	tpbrt_error_t tpbrt_params_list_get_normal3s(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_normal3_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_normal3s_of_size(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_normal3_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_normal3(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_normal3_t* out_val);

	tpbrt_error_t tpbrt_params_list_get_normals(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_normal_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_normals_of_size(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_normal_array_t* out_vals);
	tpbrt_error_t tpbrt_params_list_get_normal(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_normal_t* out_val);
#pragma endregion

#pragma region TEXTURE
	tpbrt_error_t tpbrt_params_list_get_texture(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_textures_list_t textures_list, tpbrt_texture_handle_t* out_val);
	tpbrt_error_t tpbrt_params_list_get_texture_of_type(tpbrt_params_list_t params_list, tpbrt_string_t param_name,
	  tpbrt_textures_list_t textures_list, tpbrt_texture_type_t texture_type, tpbrt_texture_handle_t* out_val);
#pragma endregion
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_PARAM_INTERNAL_H_
