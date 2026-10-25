#pragma once
#ifndef _TINYPBRT_MATH_H_
#define _TINYPBRT_MATH_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct tpbrt_opt_float_t {
		tpbrt_float_t value;
		tpbrt_bool_t has_value : 1;
	} tpbrt_opt_float_t;

	typedef struct tpbrt_bool_array_t {
		tpbrt_bool_t* data;
		tpbrt_size_t count;
	} tpbrt_bool_array_t;

	typedef struct tpbrt_int_array_t {
		tpbrt_int_t* data;
		tpbrt_size_t count;
	} tpbrt_int_array_t;

	typedef struct tpbrt_uint_array_t {
		tpbrt_uint_t* data;
		tpbrt_size_t count;
	} tpbrt_uint_array_t;

	typedef struct tpbrt_float_array_t {
		tpbrt_float_t* data;
		tpbrt_size_t count;
	} tpbrt_float_array_t;

	typedef struct tpbrt_vec2_t {
		union {
			struct {
				union {
					tpbrt_float_t x, r;
				};

				union {
					tpbrt_float_t y, g;
				};
			};

			tpbrt_float_t v[2];
		};
	} tpbrt_vec2_t;

	typedef struct tpbrt_vec3_t {
		union {
			struct {
				union {
					tpbrt_float_t x, r;
				};

				union {
					tpbrt_float_t y, g;
				};

				union {
					tpbrt_float_t z, b;
				};
			};

			tpbrt_float_t v[3];
		};
	} tpbrt_vec3_t;

	typedef struct tpbrt_vec4_t {
		union {
			struct {
				union {
					tpbrt_float_t x, r;
				};

				union {
					tpbrt_float_t y, g;
				};

				union {
					tpbrt_float_t z, b;
				};

				union {
					tpbrt_float_t w, a;
				};
			};

			tpbrt_float_t v[4];
		};
	} tpbrt_vec4_t;

	typedef struct tpbrt_vec2_array_t {
		tpbrt_vec2_t* data;
		tpbrt_size_t count;
	} tpbrt_vec2_array_t;

	typedef struct tpbrt_vec3_array_t {
		tpbrt_vec3_t* data;
		tpbrt_size_t count;
	} tpbrt_vec3_array_t;

	typedef struct tpbrt_mat4_t {
		union {
			struct {
				tpbrt_mat4_row_t row0;
				tpbrt_mat4_row_t row1;
				tpbrt_mat4_row_t row2;
				tpbrt_mat4_row_t row3;
			};

			tpbrt_mat4_row_t rows[4];

			struct {
				tpbrt_float_t m00, m01, m02, m03;
				tpbrt_float_t m10, m11, m12, m13;
				tpbrt_float_t m20, m21, m22, m23;
				tpbrt_float_t m30, m31, m32, m33;
			};

			tpbrt_float_t m[TPBRT_MAT4_ARRAY_SIZE];
		};
	} tpbrt_mat4_t;

	typedef struct tpbrt_mat4_animated_t {
		tpbrt_mat4_t start;
		tpbrt_mat4_t end;
	} tpbrt_mat4_animated_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MATH_H_
