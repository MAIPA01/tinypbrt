#pragma once
#ifndef _TINYPBRT_MATH_H_
#define _TINYPBRT_MATH_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup math-public Math
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Optional float value.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_opt_float_t {
		/// @brief Value set only when has_value is TPBRT_TRUE.
		tpbrt_float_t value;
		/// @brief Indicates if value is set.
		tpbrt_bool_t has_value : 1;
	} tpbrt_opt_float_t;

	/**
	 * @brief Array of bool values.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_bool_array_t {
		/// @brief Array data.
		tpbrt_bool_t* data;
		/// @brief Array items count.
		tpbrt_size_t count;
	} tpbrt_bool_array_t;

	/**
	 * @brief Array of int values.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_int_array_t {
		/// @brief Array data.
		tpbrt_int_t* data;
		/// @brief Array items count.
		tpbrt_size_t count;
	} tpbrt_int_array_t;

	/**
	 * @brief Array of uint values.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_uint_array_t {
		/// @brief Array data.
		tpbrt_uint_t* data;
		/// @brief Array items count.
		tpbrt_size_t count;
	} tpbrt_uint_array_t;

	/**
	 * @brief Array of float values.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_float_array_t {
		/// @brief Array data.
		tpbrt_float_t* data;
		/// @brief Array items count.
		tpbrt_size_t count;
	} tpbrt_float_array_t;

	/**
	 * @brief 2 component vector.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_vec2_t {
		union {
			struct {
				union {
					/// @brief First vector component.
					tpbrt_float_t x, r;
				};

				union {
					/// @brief Second vector component.
					tpbrt_float_t y, g;
				};
			};

			/// @brief vector as array of 2 float values.
			tpbrt_float_t v[2];
		};
	} tpbrt_vec2_t;

	/**
	 * @brief 3 component vector.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_vec3_t {
		union {
			struct {
				union {
					/// @brief First vector component.
					tpbrt_float_t x, r;
				};

				union {
					/// @brief Second vector component.
					tpbrt_float_t y, g;
				};

				union {
					/// @brief Third vector component.
					tpbrt_float_t z, b;
				};
			};

			/// @brief vector as array of 3 float values.
			tpbrt_float_t v[3];
		};
	} tpbrt_vec3_t;

	/**
	 * @brief 4 component vector.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_vec4_t {
		union {
			struct {
				union {
					/// @brief First vector component.
					tpbrt_float_t x, r;
				};

				union {
					/// @brief Second vector component.
					tpbrt_float_t y, g;
				};

				union {
					/// @brief Third vector component.
					tpbrt_float_t z, b;
				};

				union {
					/// @brief Fourth vector component.
					tpbrt_float_t w, a;
				};
			};

			/// @brief vector as array of 4 float values.
			tpbrt_float_t v[4];
		};
	} tpbrt_vec4_t;

	/**
	 * @brief Array of 2 component vectors.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_vec2_array_t {
		/// @brief Array data.
		tpbrt_vec2_t* data;
		/// @brief Array items count.
		tpbrt_size_t count;
	} tpbrt_vec2_array_t;

	/**
	 * @brief Array of 3 component vectors.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_vec3_array_t {
		/// @brief Array data.
		tpbrt_vec3_t* data;
		/// @brief Array items count.
		tpbrt_size_t count;
	} tpbrt_vec3_array_t;

	/**
	 * @brief 4 by 4 row-major matrix.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_mat4_t {
		union {
			struct {
				/// @brief First row.
				tpbrt_mat4_row_t row0;
				/// @brief Second row.
				tpbrt_mat4_row_t row1;
				/// @brief Third row.
				tpbrt_mat4_row_t row2;
				/// @brief Fourth row.
				tpbrt_mat4_row_t row3;
			};

			/// @brief Array of 4 rows.
			tpbrt_mat4_row_t rows[4];

			struct {
				/// @brief First row elements.
				tpbrt_float_t m00, m01, m02, m03;
				/// @brief Second row elements.
				tpbrt_float_t m10, m11, m12, m13;
				/// @brief Third row elements.
				tpbrt_float_t m20, m21, m22, m23;
				/// @brief Fourth row elements.
				tpbrt_float_t m30, m31, m32, m33;
			};

			/// @brief Array of 16 elements.
			tpbrt_float_t m[TPBRT_MAT4_ARRAY_SIZE];
		};
	} tpbrt_mat4_t;

	/**
	 * @brief Two 4 by 4 row-major matrices used for animations.
	 * @ingroup math-public
	 */
	typedef struct tpbrt_mat4_animated_t {
		/// @brief Starting transform matrix.
		tpbrt_mat4_t start;
		/// @brief Ending transform matrix.
		tpbrt_mat4_t end;
	} tpbrt_mat4_animated_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MATH_H_
