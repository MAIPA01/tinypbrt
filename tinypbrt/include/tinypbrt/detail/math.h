#pragma once
#ifndef _TINYPBRT_MATH_H_
#define _TINYPBRT_MATH_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		tpbrt_bool_t* values;
		tpbrt_size_t count;
	} tpbrt_bool_array_t;

	typedef struct {
		tpbrt_int_t* values;
		tpbrt_size_t count;
	} tpbrt_int_array_t;

	typedef struct {
		tpbrt_uint_t* values;
		tpbrt_size_t count;
	} tpbrt_uint_array_t;

	typedef struct {
		tpbrt_float_t* values;
		tpbrt_size_t count;
	} tpbrt_float_array_t;

	typedef struct {
		tpbrt_float_t x, y;
	} tpbrt_vec2_t;

	typedef struct {
		tpbrt_float_t x, y, z;
	} tpbrt_vec3_t;

	typedef struct {
		tpbrt_vec2_t* values;
		tpbrt_size_t count;
	} tpbrt_vec2_array_t;

	typedef struct {
		tpbrt_vec3_t* values;
		tpbrt_size_t count;
	} tpbrt_vec3_array_t;

	typedef struct {
		tpbrt_float_t m[16];
	} tpbrt_mat4_t;

	typedef struct {
		tpbrt_mat4_t start, end;
	} tpbrt_mat4_animated_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MATH_H_
