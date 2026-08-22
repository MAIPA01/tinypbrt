#pragma once
#ifndef _TINYPBRT_MATH_H_
#define _TINYPBRT_MATH_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		tpbrt_bool_t* data;
		tpbrt_size_t count;
	} tpbrt_bool_array_t;

	typedef struct {
		tpbrt_int_t* data;
		tpbrt_size_t count;
	} tpbrt_int_array_t;

	typedef struct {
		tpbrt_uint_t* data;
		tpbrt_size_t count;
	} tpbrt_uint_array_t;

	typedef struct {
		tpbrt_float_t* data;
		tpbrt_size_t count;
	} tpbrt_float_array_t;

	typedef struct {
		tpbrt_float_t x, y;
	} tpbrt_vec2_t;

	typedef struct {
		tpbrt_float_t x, y, z;
	} tpbrt_vec3_t;

	typedef struct {
		tpbrt_vec2_t* data;
		tpbrt_size_t count;
	} tpbrt_vec2_array_t;

	typedef struct {
		tpbrt_vec3_t* data;
		tpbrt_size_t count;
	} tpbrt_vec3_array_t;

	typedef struct {
		tpbrt_float_t m[16];
	} tpbrt_mat4_t;

	typedef struct {
		tpbrt_mat4_t start, end;
	} tpbrt_mat4_animated_t;

	typedef tpbrt_vec2_t tpbrt_point2_t;
	typedef tpbrt_vec3_t tpbrt_vec_t;
	typedef tpbrt_vec3_t tpbrt_point3_t;
	typedef tpbrt_point3_t tpbrt_point_t;
	typedef tpbrt_vec3_t tpbrt_normal3_t;
	typedef tpbrt_normal3_t tpbrt_normal_t;

	typedef tpbrt_vec2_array_t tpbrt_point2_array_t;
	typedef tpbrt_vec3_array_t tpbrt_vec_array_t;
	typedef tpbrt_vec3_array_t tpbrt_point3_array_t;
	typedef tpbrt_point3_array_t tpbrt_point_array_t;
	typedef tpbrt_vec3_array_t tpbrt_normal3_array_t;
	typedef tpbrt_normal3_array_t tpbrt_normal_array_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MATH_H_
