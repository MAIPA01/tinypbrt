#pragma once
#ifndef _TINYPBRT_FWD_H_
#define _TINYPBRT_FWD_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TPBRT_NULL NULL

#define TPBRT_FALSE 0
#define TPBRT_TRUE	1

	typedef uint8_t tpbrt_bool_t;
	typedef int32_t tpbrt_int_t;
	typedef uint32_t tpbrt_uint_t;
	typedef float tpbrt_float_t;
	typedef size_t tpbrt_size_t;

	struct tpbrt_bool_array_t;
	struct tpbrt_int_array_t;
	struct tpbrt_uint_array_t;
	struct tpbrt_float_array_t;

	struct tpbrt_vec2_t;
	struct tpbrt_vec3_t;

	struct tpbrt_vec2_array_t;
	struct tpbrt_vec3_array_t;

	struct tpbrt_mat4_t;
	struct tpbrt_mat4_animated_t;

	typedef char tpbrt_char_t;
	struct tpbrt_string_t;

	struct tpbrt_string_array_t;

	struct tpbrt_wavelength_t;
	typedef uint32_t tpbrt_blackbody_t;
	struct tpbrt_rgb_t;
	struct tpbrt_spectrum_t;

	struct tpbrt_wavelength_array_t;
	struct tpbrt_rgb_array_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_FWD_H_