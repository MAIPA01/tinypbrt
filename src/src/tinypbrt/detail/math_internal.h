#pragma once
#ifndef _TINYPBRT_MATH_INTERNAL_H_
#define _TINYPBRT_MATH_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TPBRT_VEC2(x_val, y_val)                  \
	(tpbrt_vec2_t) { .x = (x_val), .y = (y_val) }
#define TPBRT_VEC3(x_val, y_val, z_val)                         \
	(tpbrt_vec3_t) { .x = (x_val), .y = (y_val), .z = (z_val) }

#define TPBRT_OPT_FLOAT_VALUE(float_value)                                  \
	(tpbrt_opt_float_t) { .value = (float_value), .has_value = TPBRT_TRUE }
#define TPBRT_OPT_FLOAT_NONE                                        \
	(tpbrt_opt_float_t) { .value = 0.0f, .has_value = TPBRT_FALSE }

	tpbrt_uint_t tpbrt_next_power_of_two(tpbrt_uint_t n);

	tpbrt_float_t tpbrt_vec3_dot(tpbrt_vec3_t a, tpbrt_vec3_t b);
	tpbrt_vec3_t tpbrt_vec3_cross(tpbrt_vec3_t a, tpbrt_vec3_t b);
	tpbrt_float_t tpbrt_vec3_length(tpbrt_vec3_t a);
	tpbrt_vec3_t tpbrt_vec3_normalize(tpbrt_vec3_t a);
	tpbrt_vec3_t tpbrt_vec3_subtract(tpbrt_vec3_t a, tpbrt_vec3_t b);
	tpbrt_bool_t tpbrt_vec3_equals(tpbrt_vec3_t a, tpbrt_vec3_t b);

	void tpbrt_mat4_identity(tpbrt_mat4_t* m);
	void tpbrt_mat4_from_array(tpbrt_mat4_t* m, const tpbrt_float_t arr[TPBRT_MAT4_ARRAY_SIZE]);
	void tpbrt_mat4_from_translation(tpbrt_mat4_t* m, tpbrt_vec3_t vec);
	void tpbrt_mat4_from_scale(tpbrt_mat4_t* m, tpbrt_vec3_t vec);
	void tpbrt_mat4_from_axis_angle(tpbrt_mat4_t* m, tpbrt_vec3_t axis, tpbrt_float_t angle);
	void tpbrt_mat4_look_at_lh(tpbrt_mat4_t* m, tpbrt_vec3_t eye, tpbrt_vec3_t look_at, tpbrt_vec3_t up);
	void tpbrt_mat4_multiply(tpbrt_mat4_t* out_m, const tpbrt_mat4_t* a, const tpbrt_mat4_t* b);
	void tpbrt_mat4_inverse(tpbrt_mat4_t* out_m, const tpbrt_mat4_t* in_m);
	void tpbrt_mat4_transpose(tpbrt_mat4_t* out_m, const tpbrt_mat4_t* in_m);
	tpbrt_uint_t tpbrt_mat4_hash(const tpbrt_mat4_t* m);
	tpbrt_bool_t tpbrt_mat4_equals(const tpbrt_mat4_t* a, const tpbrt_mat4_t* b);

#define TPBRT_EMPTY_BOOL_ARRAY	(tpbrt_bool_array_t) { .data = TPBRT_NULL, .count = 0 }
#define TPBRT_EMPTY_INT_ARRAY	(tpbrt_int_array_t) { .data = TPBRT_NULL, .count = 0 }
#define TPBRT_EMPTY_UINT_ARRAY	(tpbrt_uint_array_t) { .data = TPBRT_NULL, .count = 0 }
#define TPBRT_EMPTY_FLOAT_ARRAY (tpbrt_float_array_t) { .data = TPBRT_NULL, .count = 0 }
#define TPBRT_EMPTY_VEC2_ARRAY	(tpbrt_vec2_array_t) { .data = TPBRT_NULL, .count = 0 }
#define TPBRT_EMPTY_VEC3_ARRAY	(tpbrt_vec3_array_t) { .data = TPBRT_NULL, .count = 0 }

	void tpbrt_bool_array_free(tpbrt_bool_array_t* array);
	void tpbrt_int_array_free(tpbrt_int_array_t* array);
	void tpbrt_uint_array_free(tpbrt_uint_array_t* array);
	void tpbrt_float_array_free(tpbrt_float_array_t* array);
	void tpbrt_vec2_array_free(tpbrt_vec2_array_t* array);
	void tpbrt_vec3_array_free(tpbrt_vec3_array_t* array);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MATH_INTERNAL_H_
