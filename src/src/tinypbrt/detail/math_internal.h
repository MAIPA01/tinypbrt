#pragma once
#ifndef _TINYPBRT_MATH_INTERNAL_H_
#define _TINYPBRT_MATH_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/math.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup math-private Math
	 * @ingroup tinypbrt-private
	 */

/**
 * @brief Initializes vec2.
 * @param x_val X component value.
 * @param y_val Y component value.
 * @ingroup math-private
 */
#define TPBRT_VEC2_INIT(x_val, y_val) { .x = (x_val), .y = (y_val) }
/**
 * @brief Initializes vec2 with type conversion.
 * @param x_val X component value.
 * @param y_val Y component value.
 * @ingroup math-private
 */
#define TPBRT_VEC2(x_val, y_val) (tpbrt_vec2_t) TPBRT_VEC2_INIT(x_val, y_val)

/**
 * @brief Initializes vec3.
 * @param x_val X component value.
 * @param y_val Y component value.
 * @param z_val Z component value.
 * @ingroup math-private
 */
#define TPBRT_VEC3_INIT(x_val, y_val, z_val) { .x = (x_val), .y = (y_val), .z = (z_val) }
/**
 * @brief Initializes vec3 with type conversion.
 * @param x_val X component value.
 * @param y_val Y component value.
 * @param z_val Z component value.
 * @ingroup math-private
 */
#define TPBRT_VEC3(x_val, y_val, z_val) (tpbrt_vec3_t) TPBRT_VEC3_INIT(x_val, y_val, z_val)

/**
 * @brief Initializes vec4.
 * @param x_val X component value.
 * @param y_val Y component value.
 * @param z_val Z component value.
 * @param w_val W component value.
 * @ingroup math-private
 */
#define TPBRT_VEC4(x_val, y_val, z_val, w_val)                                \
	(tpbrt_vec4_t) { .x = (x_val), .y = (y_val), .z = (z_val), .w = (w_val) }

/**
 * @brief Initializes optional float with value.
 * @param float_value Float value.
 * @ingroup math-private
 */
#define TPBRT_OPT_FLOAT_VALUE_INIT(float_value) { .value = (float_value), .has_value = TPBRT_TRUE }
/**
 * @brief Initializes optional float with value with type conversion.
 * @param float_value Float value.
 * @ingroup math-private
 */
#define TPBRT_OPT_FLOAT_VALUE(float_value) (tpbrt_opt_float_t) TPBRT_OPT_FLOAT_VALUE_INIT(float_value)
/**
 * @brief Initializes optional float with no value.
 * @ingroup math-private
 */
#define TPBRT_OPT_FLOAT_NONE_INIT { .value = 0.0f, .has_value = TPBRT_FALSE }
/**
 * @brief Initializes optional float with no value with type conversion.
 * @ingroup math-private
 */
#define TPBRT_OPT_FLOAT_NONE (tpbrt_opt_float_t) TPBRT_OPT_FLOAT_NONE_INIT

	/**
	 * @brief Calculates nearest power of two greater or equal to n.
	 * @param n Any number.
	 * @return Next power of two.
	 * @ingroup math-private
	 */
	tpbrt_uint_t tpbrt_next_power_of_two(tpbrt_uint_t n);

	/**
	 * @brief Dot product of two 3 component vectors.
	 * @param a First 3 component vector.
	 * @param b Second 3 component vector.
	 * @return Scalar -> dot product result.
	 * @ingroup math-private
	 */
	tpbrt_float_t tpbrt_vec3_dot(tpbrt_vec3_t a, tpbrt_vec3_t b);
	/**
	 * @brief Cross product of two 3 component vectors.
	 * @param a First 3 component vector.
	 * @param b Second 3 component vector.
	 * @return 3 component vector -> cross product result.
	 * @ingroup math-private
	 */
	tpbrt_vec3_t tpbrt_vec3_cross(tpbrt_vec3_t a, tpbrt_vec3_t b);
	/**
	 * @brief 3 component vector length.
	 * @param a 3 component vector which length we want to calculate.
	 * @return Length of 3 component vector.
	 * @ingroup math-private
	 */
	tpbrt_float_t tpbrt_vec3_length(tpbrt_vec3_t a);
	/**
	 * @brief Calculates normalized vector from 3 component vector.
	 * @param a 3 component vector to normalize.
	 * @return Normalized 3 component vector.
	 * @ingroup math-private
	 */
	tpbrt_vec3_t tpbrt_vec3_normalize(tpbrt_vec3_t a);
	/**
	 * @brief Subtracts values from two 3 component vectors.
	 * @param a First 3 component vector.
	 * @param b Second 3 component vector.
	 * @return Result of subtracting two 3 component vectors.
	 * @ingroup math-private
	 */
	tpbrt_vec3_t tpbrt_vec3_subtract(tpbrt_vec3_t a, tpbrt_vec3_t b);
	/**
	 * @brief Checks if two 3 component vectors are equal.
	 * @param a First 3 component vector.
	 * @param b Second 3 component vector.
	 * @return TPBRT_TRUE if they are equal else TPBRT_FALSE.
	 * @ingroup math-private
	 */
	tpbrt_bool_t tpbrt_vec3_equals(tpbrt_vec3_t a, tpbrt_vec3_t b);

	/**
	 * @brief 4 by 4 matrix identity.
	 * @param m Output matrix data.
	 * @ingroup math-private
	 */
	void tpbrt_mat4_identity(tpbrt_mat4_t* m);
	// NOLINTBEGIN(readability-magic-numbers)
	/**
	 * @brief Creates 4 by 4 matrix from array of 16 values.
	 * @param m Output matrix data.
	 * @param arr Array of values.
	 * @ingroup math-private
	 */
	void tpbrt_mat4_from_array(tpbrt_mat4_t* m, const tpbrt_float_t arr[16u]);
	// NOLINTEND(readability-magic-numbers)
	/**
	 * @brief Creates 4 by 4 translation matrix from 3 component vector.
	 * @param m Output matrix data.
	 * @param vec Translation 3 component vector.
	 * @ingroup math-private
	 */
	void tpbrt_mat4_from_translation(tpbrt_mat4_t* m, tpbrt_vec3_t vec);
	/**
	 * @brief Creates 4 by 4 scale matrix from 3 component vector.
	 * @param m Output matrix data.
	 * @param vec Scale 3 component vector.
	 * @ingroup math-private
	 */
	void tpbrt_mat4_from_scale(tpbrt_mat4_t* m, tpbrt_vec3_t vec);
	/**
	 * @brief Creates 4 by 4 rotation matrix from axis and angle.
	 * @param m Output matrix data.
	 * @param axis Rotation axis 3 component vector.
	 * @param angle Rotation angle.
	 * @ingroup math-private
	 */
	void tpbrt_mat4_from_axis_angle(tpbrt_mat4_t* m, tpbrt_vec3_t axis, tpbrt_float_t angle);
	/**
	 * @brief Creates 4 by 4 look at matrix.
	 * @param m Output matrix data.
	 * @param eye Eye position.
	 * @param look_at Object to look at position.
	 * @param up Up position.
	 * @ingroup math-private
	 */
	void tpbrt_mat4_look_at_lh(tpbrt_mat4_t* m, tpbrt_vec3_t eye, tpbrt_vec3_t look_at, tpbrt_vec3_t up);
	/**
	 * @brief Multiplies two 4 by 4 matrices.
	 * @param out_m Output matrix data.
	 * @param a First 4 by 4 matrix.
	 * @param b Second 4 by 4 matrix.
	 * @ingroup math-private
	 */
	void tpbrt_mat4_multiply(tpbrt_mat4_t* out_m, const tpbrt_mat4_t* a, const tpbrt_mat4_t* b);
	/**
	 * @brief Inverses 4 by 4 matrix.
	 * @param out_m Output matrix data.
	 * @param in_m Matrix to inverse.
	 * @ingroup math-private
	 */
	void tpbrt_mat4_inverse(tpbrt_mat4_t* out_m, const tpbrt_mat4_t* in_m);
	/**
	 * @brief Transposes 4 by 4 matrix.
	 * @param out_m Output matrix data.
	 * @param in_m Matrix to transpose.
	 * @ingroup math-private
	 */
	void tpbrt_mat4_transpose(tpbrt_mat4_t* out_m, const tpbrt_mat4_t* in_m);
	/**
	 * @brief Calculates matrix hash.
	 * @param m Matrix to calculate hash.
	 * @return Hash value of matrix.
	 * @ingroup math-private
	 */
	tpbrt_uint_t tpbrt_mat4_hash(const tpbrt_mat4_t* m);
	/**
	 * @brief Checks if two matrices are equal.
	 * @param a First matrix.
	 * @param b Second matrix.
	 * @return TPBRT_TRUE if matrices are equal else TPBRT_FALSE.
	 * @ingroup math-private
	 */
	tpbrt_bool_t tpbrt_mat4_equals(const tpbrt_mat4_t* a, const tpbrt_mat4_t* b);

	/**
	 * @brief Initializes empty array.
	 * @ingroup math-private
	 */
#define TPBRT_EMPTY_ARRAY_INIT { .data = TPBRT_NULL, .count = 0u }

	/**
	 * @brief Initializes empty bool array.
	 * @ingroup math-private
	 */
#define TPBRT_EMPTY_BOOL_ARRAY_INIT TPBRT_EMPTY_ARRAY_INIT
	/**
	 * @brief Initializes empty int array.
	 * @ingroup math-private
	 */
#define TPBRT_EMPTY_INT_ARRAY_INIT TPBRT_EMPTY_ARRAY_INIT
	/**
	 * @brief Initializes empty uint array.
	 * @ingroup math-private
	 */
#define TPBRT_EMPTY_UINT_ARRAY_INIT TPBRT_EMPTY_ARRAY_INIT
	/**
	 * @brief Initializes empty float array.
	 * @ingroup math-private
	 */
#define TPBRT_EMPTY_FLOAT_ARRAY_INIT TPBRT_EMPTY_ARRAY_INIT
	/**
	 * @brief Initializes empty vec2 array.
	 * @ingroup math-private
	 */
#define TPBRT_EMPTY_VEC2_ARRAY_INIT TPBRT_EMPTY_ARRAY_INIT
	/**
	 * @brief Initializes empty vec3 array.
	 * @ingroup math-private
	 */
#define TPBRT_EMPTY_VEC3_ARRAY_INIT TPBRT_EMPTY_ARRAY_INIT

	/**
	 * @brief Initializes empty bool array with type conversion.
	 * @ingroup math-private
	 */
#define TPBRT_EMPTY_BOOL_ARRAY (tpbrt_bool_array_t) TPBRT_EMPTY_BOOL_ARRAY_INIT
	/**
	 * @brief Initializes empty int array with type conversion.
	 * @ingroup math-private
	 */
#define TPBRT_EMPTY_INT_ARRAY (tpbrt_int_array_t) TPBRT_EMPTY_INT_ARRAY_INIT
	/**
	 * @brief Initializes empty uint array with type conversion.
	 * @ingroup math-private
	 */
#define TPBRT_EMPTY_UINT_ARRAY (tpbrt_uint_array_t) TPBRT_EMPTY_UINT_ARRAY_INIT
	/**
	 * @brief Initializes empty float array with type conversion.
	 * @ingroup math-private
	 */
#define TPBRT_EMPTY_FLOAT_ARRAY (tpbrt_float_array_t) TPBRT_EMPTY_FLOAT_ARRAY_INIT
	/**
	 * @brief Initializes empty vec2 array with type conversion.
	 * @ingroup math-private
	 */
#define TPBRT_EMPTY_VEC2_ARRAY (tpbrt_vec2_array_t) TPBRT_EMPTY_VEC2_ARRAY_INIT
	/**
	 * @brief Initializes empty vec3 array with type conversion.
	 * @ingroup math-private
	 */
#define TPBRT_EMPTY_VEC3_ARRAY (tpbrt_vec3_array_t) TPBRT_EMPTY_VEC3_ARRAY_INIT

	/**
	 * @brief Frees bool array.
	 * @param array Bool array to free.
	 * @ingroup math-private
	 */
	void tpbrt_bool_array_free(tpbrt_bool_array_t* array);
	/**
	 * @brief Frees int array.
	 * @param array Int array to free.
	 * @ingroup math-private
	 */
	void tpbrt_int_array_free(tpbrt_int_array_t* array);
	/**
	 * @brief Frees uint array.
	 * @param array Uint array to free.
	 * @ingroup math-private
	 */
	void tpbrt_uint_array_free(tpbrt_uint_array_t* array);
	/**
	 * @brief Frees float array.
	 * @param array Float array to free.
	 * @ingroup math-private
	 */
	void tpbrt_float_array_free(tpbrt_float_array_t* array);
	/**
	 * @brief Frees vec2 array.
	 * @param array Vec2 array to free.
	 * @ingroup math-private
	 */
	void tpbrt_vec2_array_free(tpbrt_vec2_array_t* array);
	/**
	 * @brief Frees vec3 array.
	 * @param array Vec3 array to free.
	 * @ingroup math-private
	 */
	void tpbrt_vec3_array_free(tpbrt_vec3_array_t* array);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MATH_INTERNAL_H_
