#pragma once
#ifndef _TINYPBRT_MATH_INTERNAL_H_
#define _TINYPBRT_MATH_INTERNAL_H_

#include <tinypbrt/detail/math.h>

#ifdef __cplusplus
extern "C" {
#endif

	void tpbrt_mat4_identity(tpbrt_mat4_t* m);
	void tpbrt_mat4_multiply(tpbrt_mat4_t* out, const tpbrt_mat4_t* a, const tpbrt_mat4_t* b);
	void tpbrt_mat4_from_array(tpbrt_mat4_t* m, const tpbrt_float_t arr[16]);
	void tpbrt_mat4_inverse(float out_m[16], const float m[16]);

	void tpbrt_free_int_array(tpbrt_int_array_t* array);
	void tpbrt_free_uint_array(tpbrt_uint_array_t* array);
	void tpbrt_free_float_array(tpbrt_float_array_t* array);
	void tpbrt_free_vec2_array(tpbrt_vec2_array_t* array);
	void tpbrt_free_vec3_array(tpbrt_vec3_array_t* array);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MATH_INTERNAL_H_
