#pragma once
#ifndef _TINYPBRT_PARAM_INTERNAL_H_
#define _TINYPBRT_PARAM_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/error.h>

#include <tinypbrt/detail/texture_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup param-private Parameter
	 * @ingroup tinypbrt-private
	 */

#pragma region PARAM

	/**
	 * @brief Parameter type.
	 * @ingroup param-private
	 */
	typedef enum tpbrt_param_type_t {
		/// @brief Int type.
		TPBRT_PARAM_TYPE_INTEGER   = 0,
		/// @brief Float type.
		TPBRT_PARAM_TYPE_FLOAT	   = 1,
		/// @brief Point2 type.
		TPBRT_PARAM_TYPE_POINT2	   = 2,
		/// @brief Vector2 type.
		TPBRT_PARAM_TYPE_VECTOR2   = 3,
		/// @brief Point3 type.
		TPBRT_PARAM_TYPE_POINT3	   = 4,
		/// @brief Point type.
		TPBRT_PARAM_TYPE_POINT	   = 5,
		/// @brief Vector3 type.
		TPBRT_PARAM_TYPE_VECTOR3   = 6,
		/// @brief Vector type.
		TPBRT_PARAM_TYPE_VECTOR	   = 7,
		/// @brief Normal3 type.
		TPBRT_PARAM_TYPE_NORMAL3   = 8,
		/// @brief Normal type.
		TPBRT_PARAM_TYPE_NORMAL	   = 9,
		/// @brief Spectrum type.
		TPBRT_PARAM_TYPE_SPECTRUM  = 10,
		/// @brief RGB type.
		TPBRT_PARAM_TYPE_RGB	   = 11,
		/// @brief Blackbody type.
		TPBRT_PARAM_TYPE_BLACKBODY = 12,
		/// @brief Bool type.
		TPBRT_PARAM_TYPE_BOOL	   = 13,
		/// @brief String type.
		TPBRT_PARAM_TYPE_STRING	   = 14,
		/// @brief Texture type.
		TPBRT_PARAM_TYPE_TEXTURE   = 15,
		/// @brief Number of possible parameter types.
		TPBRT_PARAM_TYPE_MAX_NUM   = 16,
	} tpbrt_param_type_t;

	/**
	 * @brief Parameter value type.
	 * @ingroup param-private
	 */
	typedef enum tpbrt_param_value_type_t {
		/// @brief Single value.
		TPBRT_PARAM_VALUE_TYPE_SINGLE = 0,
		/// @brief Array value.
		TPBRT_PARAM_VALUE_TYPE_ARRAY  = 1,
		/// @brief Quoted string value.
		TPBRT_PARAM_VALUE_TYPE_STRING = 2,
	} tpbrt_param_value_type_t;

	/**
	 * @brief Parameter data.
	 * @ingroup param-private
	 */
	typedef struct tpbrt_param_t {
		/// @brief Parameter name.
		tpbrt_string_t name;
		/// @brief Parameter value.
		tpbrt_string_t value;
		/// @brief Parameter type.
		tpbrt_param_type_t type				: 6;
		/// @brief Parameter value type.
		tpbrt_param_value_type_t value_type : 3;
	} tpbrt_param_t;

	/**
	 * @brief Returns parameter type based on string value.
	 * @param type_str String which is convertible to parameter type.
	 * @param type_out Output parameter type value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_type_from_string(tpbrt_string_t type_str, tpbrt_param_type_t* type_out);

	/**
	 * @brief Initializes parameter data.
	 * @param declaration String containing type and name of parameter.
	 * @param value_type Value type.
	 * @param value_str Parameter value.
	 * @param param Parameter data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_init(tpbrt_string_t declaration, tpbrt_param_value_type_t value_type, tpbrt_string_t value_str,
	  tpbrt_param_t* param);

#pragma region STRING
	/**
	 * @brief Read parameter value as array of strings.
	 * @param param Parameter data.
	 * @param out_vals Strings array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_STRING, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_strings(const tpbrt_param_t* param, tpbrt_string_array_t* out_vals);
	/**
	 * @brief Read parameter value as array of strings of desired size.
	 * @param param Parameter data.
	 * @param desired_size Desired array size.
	 * @param out_vals Strings array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_STRING, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_strings_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_string_array_t* out_vals);
	/**
	 * @brief Read parameter value as single string.
	 * @param param Parameter data.
	 * @param out_val String value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_STRING, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_string(const tpbrt_param_t* param, tpbrt_string_t* out_val);
#pragma endregion

#pragma region BOOL
	/**
	 * @brief Read parameter value as array of bools.
	 * @param param Parameter data.
	 * @param out_vals Bools array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_BOOL, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_bools(const tpbrt_param_t* param, tpbrt_bool_array_t* out_vals);
	/**
	 * @brief Read parameter value as array of bools of desired size.
	 * @param param Parameter data.
	 * @param desired_size Desired array size.
	 * @param out_vals Bools array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_BOOL, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_bools_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_bool_array_t* out_vals);
	/**
	 * @brief Read parameter value as single bool.
	 * @param param Parameter data.
	 * @param out_val Bool value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_BOOL, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_bool(const tpbrt_param_t* param, tpbrt_bool_t* out_val);
#pragma endregion

#pragma region FLOAT
	/**
	 * @brief Read parameter value as array of floats.
	 * @param param Parameter data.
	 * @param out_vals Floats array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_floats(const tpbrt_param_t* param, tpbrt_float_array_t* out_vals);
	/**
	 * @brief Read parameter value as array of floats of desired size.
	 * @param param Parameter data.
	 * @param desired_size Desired array size.
	 * @param out_vals Floats array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_floats_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_float_array_t* out_vals);
	/**
	 * @brief Read parameter value as single float.
	 * @param param Parameter data.
	 * @param out_val Float value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_float(const tpbrt_param_t* param, tpbrt_float_t* out_val);
#pragma endregion

#pragma region INT
	/**
	 * @brief Read parameter value as array of ints.
	 * @param param Parameter data.
	 * @param out_vals Ints array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_INT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_ints(const tpbrt_param_t* param, tpbrt_int_array_t* out_vals);
	/**
	 * @brief Read parameter value as array of ints of desired size.
	 * @param param Parameter data.
	 * @param desired_size Desired array size.
	 * @param out_vals Ints array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_INT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_ints_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size, tpbrt_int_array_t* out_vals);
	/**
	 * @brief Read parameter value as single int.
	 * @param param Parameter data.
	 * @param out_val Int value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_INT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_int(const tpbrt_param_t* param, tpbrt_int_t* out_val);
#pragma endregion

#pragma region UINT
	/**
	 * @brief Read parameter value as array of uints.
	 * @param param Parameter data.
	 * @param out_vals Uints array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_UINT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_uints(const tpbrt_param_t* param, tpbrt_uint_array_t* out_vals);
	/**
	 * @brief Read parameter value as array of uints of desired size.
	 * @param param Parameter data.
	 * @param desired_size Desired array size.
	 * @param out_vals Uints array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_UINT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_uints_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_uint_array_t* out_vals);
	/**
	 * @brief Read parameter value as single uint.
	 * @param param Parameter data.
	 * @param out_val Uint value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_UINT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_uint(const tpbrt_param_t* param, tpbrt_uint_t* out_val);
#pragma endregion

#pragma region RGB
	/**
	 * @brief Read parameter value as array of rgbs.
	 * @param param Parameter data.
	 * @param out_vals Rgbs array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_rgbs(const tpbrt_param_t* param, tpbrt_rgb_array_t* out_vals);
	/**
	 * @brief Read parameter value as array of rgbs of desired size.
	 * @param param Parameter data.
	 * @param desired_size Desired array size.
	 * @param out_vals Rgbs array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_rgbs_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size, tpbrt_rgb_array_t* out_vals);
	/**
	 * @brief Read parameter value as single rgb.
	 * @param param Parameter data.
	 * @param out_val Rgb value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_rgb(const tpbrt_param_t* param, tpbrt_rgb_t* out_val);
#pragma endregion

#pragma region WAVELENGTH
	/**
	 * @brief Read parameter value as array of wavelengths.
	 * @param param Parameter data.
	 * @param out_vals Wavelengths array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_wavelengths(const tpbrt_param_t* param, tpbrt_wavelength_array_t* out_vals);
	/**
	 * @brief Read parameter value as array of wavelengths of desired size.
	 * @param param Parameter data.
	 * @param desired_size Desired array size.
	 * @param out_vals Wavelengths array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_wavelengths_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_wavelength_array_t* out_vals);
#pragma endregion

#pragma region SPECTRUM
	/**
	 * @brief Read parameter value as single spectrum.
	 * @param param Parameter data.
	 * @param out_val Spectrum value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_spectrum(const tpbrt_param_t* param, tpbrt_spectrum_t* out_val);
#pragma endregion

#pragma region VEC2
	/**
	 * @brief Read parameter value as array of vec2s.
	 * @param param Parameter data.
	 * @param out_vals Vec2s array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_vec2s(const tpbrt_param_t* param, tpbrt_vec2_array_t* out_vals);
	/**
	 * @brief Read parameter value as array of vec2s of desired size.
	 * @param param Parameter data.
	 * @param desired_size Desired array size.
	 * @param out_vals Vec2s array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_vec2s_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_vec2_array_t* out_vals);
	/**
	 * @brief Read parameter value as single vec2.
	 * @param param Parameter data.
	 * @param out_val Vec2 value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_vec2(const tpbrt_param_t* param, tpbrt_vec2_t* out_val);

	/**
	 * @brief Read parameter value as array of point2s.
	 * @param param Parameter data.
	 * @param out_vals Point2s array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_point2s(const tpbrt_param_t* param, tpbrt_point2_array_t* out_vals);
	/**
	 * @brief Read parameter value as array of point2s of desired size.
	 * @param param Parameter data.
	 * @param desired_size Desired array size.
	 * @param out_vals Point2s array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_point2s_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_point2_array_t* out_vals);
	/**
	 * @brief Read parameter value as single point2.
	 * @param param Parameter data.
	 * @param out_val Point2 value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_point2(const tpbrt_param_t* param, tpbrt_point2_t* out_val);
#pragma endregion

#pragma region VEC3
	/**
	 * @brief Read parameter value as array of vec3s.
	 * @param param Parameter data.
	 * @param out_vals Vec3s array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_vec3s(const tpbrt_param_t* param, tpbrt_vec3_array_t* out_vals);
	/**
	 * @brief Read parameter value as array of vec3s of desired size.
	 * @param param Parameter data.
	 * @param desired_size Desired array size.
	 * @param out_vals Vec3s array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_vec3s_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_vec3_array_t* out_vals);
	/**
	 * @brief Read parameter value as single vec3.
	 * @param param Parameter data.
	 * @param out_val Vec3 value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_vec3(const tpbrt_param_t* param, tpbrt_vec3_t* out_val);

	/**
	 * @brief Read parameter value as array of vecs.
	 * @param param Parameter data.
	 * @param out_vals Vecs array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_vecs(const tpbrt_param_t* param, tpbrt_vec_array_t* out_vals);
	/**
	 * @brief Read parameter value as array of vecs of desired size.
	 * @param param Parameter data.
	 * @param desired_size Desired array size.
	 * @param out_vals Vecs array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_vecs_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size, tpbrt_vec_array_t* out_vals);
	/**
	 * @brief Read parameter value as single vec.
	 * @param param Parameter data.
	 * @param out_val Vec value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_vec(const tpbrt_param_t* param, tpbrt_vec_t* out_val);

	/**
	 * @brief Read parameter value as array of point3s.
	 * @param param Parameter data.
	 * @param out_vals Point3s array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_point3s(const tpbrt_param_t* param, tpbrt_point3_array_t* out_vals);
	/**
	 * @brief Read parameter value as array of point3s of desired size.
	 * @param param Parameter data.
	 * @param desired_size Desired array size.
	 * @param out_vals Point3s array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_point3s_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_point3_array_t* out_vals);
	/**
	 * @brief Read parameter value as single point3.
	 * @param param Parameter data.
	 * @param out_val Point3 value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_point3(const tpbrt_param_t* param, tpbrt_point3_t* out_val);

	/**
	 * @brief Read parameter value as array of points.
	 * @param param Parameter data.
	 * @param out_vals Points array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_points(const tpbrt_param_t* param, tpbrt_point_array_t* out_vals);
	/**
	 * @brief Read parameter value as array of points of desired size.
	 * @param param Parameter data.
	 * @param desired_size Desired array size.
	 * @param out_vals Points array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_points_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_point_array_t* out_vals);
	/**
	 * @brief Read parameter value as single point.
	 * @param param Parameter data.
	 * @param out_val Point value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_point(const tpbrt_param_t* param, tpbrt_point_t* out_val);

	/**
	 * @brief Read parameter value as array of normal3s.
	 * @param param Parameter data.
	 * @param out_vals Normal3s array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_normal3s(const tpbrt_param_t* param, tpbrt_normal3_array_t* out_vals);
	/**
	 * @brief Read parameter value as array of normal3s of desired size.
	 * @param param Parameter data.
	 * @param desired_size Desired array size.
	 * @param out_vals Normal3s array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_normal3s_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_normal3_array_t* out_vals);
	/**
	 * @brief Read parameter value as single normal3.
	 * @param param Parameter data.
	 * @param out_val Normal3 value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_normal3(const tpbrt_param_t* param, tpbrt_normal3_t* out_val);

	/**
	 * @brief Read parameter value as array of normals.
	 * @param param Parameter data.
	 * @param out_vals Normals array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_normals(const tpbrt_param_t* param, tpbrt_normal_array_t* out_vals);
	/**
	 * @brief Read parameter value as array of normals of desired size.
	 * @param param Parameter data.
	 * @param desired_size Desired array size.
	 * @param out_vals Normals array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_normals_of_size(const tpbrt_param_t* param, tpbrt_size_t desired_size,
	  tpbrt_normal_array_t* out_vals);
	/**
	 * @brief Read parameter value as single normal.
	 * @param param Parameter data.
	 * @param out_val Normal value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_normal(const tpbrt_param_t* param, tpbrt_normal_t* out_val);
#pragma endregion

#pragma region TEXTURE
	/**
	 * @brief Read parameter value as single texture.
	 * @param param Parameter data.
	 * @param textures_list Textures list.
	 * @param out_val Texture handle.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_STRING, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_texture(const tpbrt_param_t* param, const tpbrt_textures_list_t* textures_list,
	  tpbrt_texture_handle_t* out_val);
	/**
	 * @brief Read parameter value as single texture of type.
	 * @param param Parameter data.
	 * @param textures_list Textures list.
	 * @param texture_type Desired texture type.
	 * @param out_val String value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_STRING, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_param_as_texture_of_type(const tpbrt_param_t* param, const tpbrt_textures_list_t* textures_list,
	  tpbrt_texture_type_t texture_type, tpbrt_texture_handle_t* out_val);
#pragma endregion
#pragma endregion

#pragma region PARAMS_LIST

	/**
	 * @brief Parameters list.
	 * @ingroup param-private
	 */
	typedef struct tpbrt_params_list_t {
		/// @brief Parameters.
		tpbrt_param_t* params;
		/// @brief Number of parameters.
		tpbrt_size_t count;
		/// @brief Parameters list capacity.
		tpbrt_size_t capacity;
	} tpbrt_params_list_t;

	/**
	 * @brief Initializes parameters list.
	 * @param params_list Parameters list data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_init(tpbrt_params_list_t* params_list);
	/**
	 * @brief Reserves space for more parameters.
	 * @param params_list Parameters list data.
	 * @param new_capacity New list capacity.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_reserve(tpbrt_params_list_t* params_list, tpbrt_size_t new_capacity);
	/**
	 * @brief Resizes parameters list.
	 * @param params_list Parameters list data.
	 * @param new_size New list size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_resize(tpbrt_params_list_t* params_list, tpbrt_size_t new_size);
	/**
	 * @brief Add parameter to list.
	 * @param params_list Parameters list data.
	 * @param param Parameter to add.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_append(tpbrt_params_list_t* params_list, const tpbrt_param_t* param);
	/**
	 * @brief Adds all parameters from other parameters list. Replaces existing parameters.
	 * @param dst_params_list Destination parameters list.
	 * @param src_params_list Source parameters list.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_extend(tpbrt_params_list_t* dst_params_list, const tpbrt_params_list_t* src_params_list);
	/**
	 * @brief Adds all parameters from other parameters list. Adds only parameters which doesn't exist in destination list.
	 * @param dst_params_list Destination parameters list.
	 * @param src_params_list Source parameters list.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_extend_missing(tpbrt_params_list_t* dst_params_list,
	  const tpbrt_params_list_t* src_params_list);
	/**
	 * @brief Returns parameter of given name from list.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param param Output parameter data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name, tpbrt_param_t** param);
	/**
	 * @brief Returns const parameter of given name from list.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param param Output parameter data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_const(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  const tpbrt_param_t** param);
	/**
	 * @brief Frees parameters list data.
	 * @param params_list Parameters list data to free.
	 * @ingroup param-private
	 */
	void tpbrt_params_list_free(tpbrt_params_list_t* params_list);

	/**
	 * @brief Checks if parameters list is empty.
	 * @param params_list Parameters list data to check.
	 * @return TPBRT_TRUE if parameters list is empty else TPBRT_FALSE.
	 * @ingroup param-private
	 */
	tpbrt_bool_t tpbrt_params_list_empty(const tpbrt_params_list_t* params_list);

#pragma region STRING
	/**
	 * @brief Returns parameter from list as strings array.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_vals Output strings array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_STRING, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_strings(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_string_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as strings array of desired size.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param desired_size Desired array size.
	 * @param out_vals Output strings array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_STRING, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_strings_of_size(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_string_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as single string. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param default_val Default string value.
	 * @param out_val Output string.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_STRING, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_string(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_string_t default_val, tpbrt_string_t* out_val);
	/**
	 * @brief Returns parameter from list as single string.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_val Output string.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_STRING, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_string_required(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_string_t* out_val);
#pragma endregion

#pragma region BOOL
	/**
	 * @brief Returns parameter from list as bools array.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_vals Output bools array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_BOOL, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_bools(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_bool_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as bools array of desired size.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param desired_size Desired array size.
	 * @param out_vals Output bools array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_BOOL, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_bools_of_size(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_bool_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as single bool. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param default_val Default bool value.
	 * @param out_val Output bool.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_BOOL, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_bool(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_bool_t default_val, tpbrt_bool_t* out_val);
#pragma endregion

#pragma region FLOAT
	/**
	 * @brief Returns parameter from list as floats array.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_vals Output floats array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_floats(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_float_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as floats array of desired size.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param desired_size Desired array size.
	 * @param out_vals Output floats array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_floats_of_size(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_float_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as single float. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param default_val Default float value.
	 * @param out_val Output float.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_float(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_float_t default_val, tpbrt_float_t* out_val);
	/**
	 * @brief Returns parameter from list as single optional float. If not found empty optional float is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_val Output float.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_opt_float(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_opt_float_t* out_val);
#pragma endregion

#pragma region INT
	/**
	 * @brief Returns parameter from list as ints array.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_vals Output ints array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_INT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_ints(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_int_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as ints array of desired size.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param desired_size Desired array size.
	 * @param out_vals Output ints array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_INT, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_ints_of_size(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_int_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as single int. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param default_val Default int value.
	 * @param out_val Output int.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_INT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_int(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_int_t default_val, tpbrt_int_t* out_val);
#pragma endregion

#pragma region UINT
	/**
	 * @brief Returns parameter from list as uints array.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_vals Output uints array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_UINT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_uints(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_uint_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as uints array of desired size.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param desired_size Desired array size.
	 * @param out_vals Output uints array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_UINT, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_uints_of_size(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_uint_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as single uint. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param default_val Default string value.
	 * @param out_val Output uint.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_UINT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_uint(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_uint_t default_val, tpbrt_uint_t* out_val);
#pragma endregion

#pragma region RGB
	/**
	 * @brief Returns parameter from list as rgbs array.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_vals Output rgbs array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_rgbs(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_rgb_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as rgbs array of desired size.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param desired_size Desired array size.
	 * @param out_vals Output rgbs array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_rgbs_of_size(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_rgb_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as single rgb. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param default_val Default rgb value.
	 * @param out_val Output rgb.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_rgb(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_rgb_t default_val, tpbrt_rgb_t* out_val);
#pragma endregion

#pragma region WAVELENGTH
	/**
	 * @brief Returns parameter from list as wavelengths array.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_vals Output wavelengths array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_wavelengths(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_wavelength_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as wavelengths array of desired size.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param desired_size Desired array size.
	 * @param out_vals Output wavelengths array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_wavelengths_of_size(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_wavelength_array_t* out_vals);
#pragma endregion

#pragma region SPECTRUM
	/**
	 * @brief Returns parameter from list as single spectrum. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param default_val Default spectrum value.
	 * @param out_val Output spectrum.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_spectrum(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  const tpbrt_spectrum_t* default_val, tpbrt_spectrum_t* out_val);
	/**
	 * @brief Returns parameter from list as single spectrum.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_val Output spectrum.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_spectrum_required(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_spectrum_t* out_val);
#pragma endregion

#pragma region VEC2
	/**
	 * @brief Returns parameter from list as vec2s array.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_vals Output vec2s array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_vec2s(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_vec2_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as vec2s array of desired size.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param desired_size Desired array size.
	 * @param out_vals Output vec2s array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_vec2s_of_size(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_vec2_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as single vec2. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param default_val Default vec2 value.
	 * @param out_val Output vec2.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_vec2(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_vec2_t default_val, tpbrt_vec2_t* out_val);

	/**
	 * @brief Returns parameter from list as point2s array.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_vals Output point2s array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_point2s(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_point2_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as point2s array of desired size.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param desired_size Desired array size.
	 * @param out_vals Output point2s array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_point2s_of_size(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_point2_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as single point2. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param default_val Default point2 value.
	 * @param out_val Output point2.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_point2(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_point2_t default_val, tpbrt_point2_t* out_val);
#pragma endregion

#pragma region VEC3
	/**
	 * @brief Returns parameter from list as vec3s array.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_vals Output vec3s array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_vec3s(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_vec3_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as vec3s array of desired size.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param desired_size Desired array size.
	 * @param out_vals Output vec3s array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_vec3s_of_size(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_vec3_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as single vec3. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param default_val Default vec3 value.
	 * @param out_val Output vec3.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_vec3(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_vec3_t default_val, tpbrt_vec3_t* out_val);

	/**
	 * @brief Returns parameter from list as vecs array.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_vals Output vecs array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_vecs(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_vec_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as vecs array of desired size.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param desired_size Desired array size.
	 * @param out_vals Output vecs array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_vecs_of_size(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_vec_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as single vec. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param default_val Default vec value.
	 * @param out_val Output vec.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_vec(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_vec_t default_val, tpbrt_vec_t* out_val);

	/**
	 * @brief Returns parameter from list as point3s array.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_vals Output point3s array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_point3s(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_point3_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as point3s array of desired size.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param desired_size Desired array size.
	 * @param out_vals Output point3s array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_point3s_of_size(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_point3_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as single point3. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param default_val Default point3 value.
	 * @param out_val Output point3.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_point3(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_point3_t default_val, tpbrt_point3_t* out_val);

	/**
	 * @brief Returns parameter from list as points array.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_vals Output points array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_points(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_point_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as points array of desired size.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param desired_size Desired array size.
	 * @param out_vals Output points array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_points_of_size(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_point_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as single point. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param default_val Default point value.
	 * @param out_val Output point.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_point(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_point_t default_val, tpbrt_point_t* out_val);

	/**
	 * @brief Returns parameter from list as normal3s array.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_vals Output normal3s array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_normal3s(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_normal3_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as normal3s array of desired size.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param desired_size Desired array size.
	 * @param out_vals Output normal3s array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_normal3s_of_size(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_normal3_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as single normal3. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param default_val Default normal3 value.
	 * @param out_val Output normal3.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_normal3(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_normal3_t default_val, tpbrt_normal3_t* out_val);

	/**
	 * @brief Returns parameter from list as normals array.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param out_vals Output normals array.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_normals(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_normal_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as normals array of desired size.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param desired_size Desired array size.
	 * @param out_vals Output normals array of desired size.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_TOO_MANY_VALUES, TPBRT_ERROR_MISSING_REQUIRED_PARAMETER, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_normals_of_size(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_size_t desired_size, tpbrt_normal_array_t* out_vals);
	/**
	 * @brief Returns parameter from list as single normal. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param default_val Default normal value.
	 * @param out_val Output normal.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_FLOAT, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_normal(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  tpbrt_normal_t default_val, tpbrt_normal_t* out_val);
#pragma endregion

#pragma region TEXTURE
	/**
	 * @brief Returns parameter from list as single texture. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param textures_list Textures list data.
	 * @param default_val Default texture value.
	 * @param out_val Output texture.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_STRING, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_texture(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  const tpbrt_textures_list_t* textures_list, const tpbrt_texture_handle_t* default_val, tpbrt_texture_handle_t* out_val);
	/**
	 * @brief Returns parameter from list as single texture of type. If not found default value is returned.
	 * @param params_list Parameters list data.
	 * @param param_name Parameter name.
	 * @param textures_list Textures list data.
	 * @param texture_type Texture type.
	 * @param default_val Default texture value.
	 * @param out_val Output texture of type.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_PARAM_NAME, TPBRT_ERROR_INVALID_PARAM_TYPE, TPBRT_ERROR_PARSE_STRING, TPBRT_ERROR_NONE).
	 * @ingroup param-private
	 */
	tpbrt_error_t tpbrt_params_list_get_texture_of_type(const tpbrt_params_list_t* params_list, tpbrt_string_t param_name,
	  const tpbrt_textures_list_t* textures_list, tpbrt_texture_type_t texture_type, const tpbrt_texture_handle_t* default_val,
	  tpbrt_texture_handle_t* out_val);
#pragma endregion
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_PARAM_INTERNAL_H_
