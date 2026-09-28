#pragma once
#ifndef _TINYPBRT_MATH_INTERNAL_H_
#define _TINYPBRT_MATH_INTERNAL_H_

#include <tinypbrt/detail/math.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_uint_t tpbrt_next_power_of_two(tpbrt_uint_t n);

	void tpbrt_mat4_identity(tpbrt_mat4_t* m);
	void tpbrt_mat4_from_array(tpbrt_mat4_t* m, const tpbrt_float_t arr[16]);
	void tpbrt_mat4_from_translation(tpbrt_mat4_t* m, const tpbrt_float_t vec[3]);
	void tpbrt_mat4_from_scale(tpbrt_mat4_t* m, const tpbrt_float_t vec[3]);
	void tpbrt_mat4_from_axis_angle(tpbrt_mat4_t* m, const tpbrt_float_t axis[3], tpbrt_float_t angle);
	void tpbrt_mat4_look_at_lh(tpbrt_mat4_t* m, const tpbrt_float_t eye[3], const tpbrt_float_t look_at[3],
	  const tpbrt_float_t up[3]);
	void tpbrt_mat4_multiply(tpbrt_mat4_t* out_m, const tpbrt_mat4_t* a, const tpbrt_mat4_t* b);
	void tpbrt_mat4_inverse(tpbrt_mat4_t* out_m, const tpbrt_mat4_t* in_m);

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
