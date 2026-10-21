#include <tinypbrt/pch.h>

#include <tinypbrt/detail/math_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_uint_t tpbrt_next_power_of_two(tpbrt_uint_t n) {
			if (n == 0) { return 1; }

		--n;
		n |= n >> (1 << 0);
		n |= n >> (1 << 1);
		n |= n >> (1 << 2);
		n |= n >> (1 << 3);
		n |= n >> (1 << 4);
		++n;

		return n;
	}

	tpbrt_float_t tpbrt_vec3_dot(const tpbrt_vec3_t a, const tpbrt_vec3_t b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); }

	tpbrt_vec3_t tpbrt_vec3_cross(const tpbrt_vec3_t a, const tpbrt_vec3_t b) {
		return TPBRT_VEC3((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
	}

	tpbrt_float_t tpbrt_vec3_length(const tpbrt_vec3_t a) { return sqrtf(tpbrt_vec3_dot(a, a)); }

	tpbrt_vec3_t tpbrt_vec3_normalize(tpbrt_vec3_t a) {
		const tpbrt_float_t len = tpbrt_vec3_length(a);

			if (len <= TPBRT_EPSILON) { return TPBRT_VEC3(0.0f, 0.0f, 0.0f); }

		a.x /= len;
		a.y /= len;
		a.z /= len;

		return a;
	}

	tpbrt_vec3_t tpbrt_vec3_subtract(const tpbrt_vec3_t a, const tpbrt_vec3_t b) {
		return TPBRT_VEC3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	tpbrt_bool_t tpbrt_vec3_equals(const tpbrt_vec3_t a, const tpbrt_vec3_t b) {
		return a.x == b.x && a.y == b.y && a.z == b.z ? TPBRT_TRUE : TPBRT_FALSE;
	}

	void tpbrt_mat4_identity(tpbrt_mat4_t* const m) {
			if (m == TPBRT_NULL) { return; }

		// NOLINTBEGIN(readability-magic-numbers)
			for (tpbrt_uint_t i = 0; i < TPBRT_MAT4_ARRAY_SIZE; ++i) { m->m[i] = i % 5u == 0 ? 1.0f : 0.0f; }
		// NOLINTEND(readability-magic-numbers)
	}

	void tpbrt_mat4_from_array(tpbrt_mat4_t* const m, const tpbrt_float_t arr[TPBRT_MAT4_ARRAY_SIZE]) {
			if (m == TPBRT_NULL) { return; }
		memcpy(m, arr, TPBRT_MAT4_ARRAY_SIZE * sizeof(tpbrt_float_t));
	}

	void tpbrt_mat4_from_translation(tpbrt_mat4_t* const m, const tpbrt_vec3_t vec) {
			if (m == TPBRT_NULL) { return; }
		tpbrt_mat4_identity(m);

		// NOLINTBEGIN(readability-magic-numbers)

		m->m[3]	 = vec.x;
		m->m[7]	 = vec.y;
		m->m[11] = vec.z;

		// NOLINTEND(readability-magic-numbers)
	}

	void tpbrt_mat4_from_scale(tpbrt_mat4_t* const m, const tpbrt_vec3_t vec) {
			if (m == TPBRT_NULL) { return; }
		tpbrt_mat4_identity(m);

		// NOLINTBEGIN(readability-magic-numbers)

		m->m[0]	 = vec.x;
		m->m[5]	 = vec.y;
		m->m[10] = vec.z;
		m->m[15] = 1.0f;

		// NOLINTEND(readability-magic-numbers)
	}

	void tpbrt_mat4_from_axis_angle(tpbrt_mat4_t* const m, tpbrt_vec3_t axis, const tpbrt_float_t angle) {
			if (m == TPBRT_NULL) { return; }
		const tpbrt_float_t rad = angle * (TPBRT_PI / 180.0f);

		const tpbrt_float_t c	= cosf(rad);
		const tpbrt_float_t s	= sinf(rad);
		const tpbrt_float_t t	= 1.0f - c;

		axis					= tpbrt_vec3_normalize(axis);
			if (tpbrt_vec3_equals(axis, TPBRT_VEC3(0.0f, 0.0f, 0.0f))) {
				tpbrt_mat4_identity(m);
				return;
			}

		// NOLINTBEGIN(readability-magic-numbers)

		m->m[0]	 = (t * axis.x * axis.x) + c;
		m->m[1]	 = (t * axis.x * axis.y) - (s * axis.z);
		m->m[2]	 = (t * axis.x * axis.z) + (s * axis.y);
		m->m[3]	 = 0.0f;

		m->m[4]	 = (t * axis.x * axis.y) + (s * axis.z);
		m->m[5]	 = (t * axis.y * axis.y) + c;
		m->m[6]	 = (t * axis.y * axis.z) - (s * axis.x);
		m->m[7]	 = 0.0f;

		m->m[8]	 = (t * axis.x * axis.z) - (s * axis.y);
		m->m[9]	 = (t * axis.y * axis.z) + (s * axis.x);
		m->m[10] = (t * axis.z * axis.z) + c;
		m->m[11] = 0.0f;

		m->m[12] = 0.0f;
		m->m[13] = 0.0f;
		m->m[14] = 0.0f;
		m->m[15] = 1.0f;

		// NOLINTEND(readability-magic-numbers)
	}

	void tpbrt_mat4_look_at_lh(tpbrt_mat4_t* const m, const tpbrt_vec3_t eye, const tpbrt_vec3_t look_at, const tpbrt_vec3_t up) {
			if (m == TPBRT_NULL) { return; }
		const tpbrt_vec3_t dir	  = tpbrt_vec3_normalize(tpbrt_vec3_subtract(look_at, eye));

		const tpbrt_vec3_t right  = tpbrt_vec3_normalize(tpbrt_vec3_cross(up, dir));

		const tpbrt_vec3_t new_up = tpbrt_vec3_cross(dir, right);

		// NOLINTBEGIN(readability-magic-numbers)

		// X - right
		m->m[0]					  = right.x;
		m->m[1]					  = right.y;
		m->m[2]					  = right.z;
		m->m[3]					  = -tpbrt_vec3_dot(right, eye);

		// Y - up
		m->m[4]					  = new_up.x;
		m->m[5]					  = new_up.y;
		m->m[6]					  = new_up.z;
		m->m[7]					  = -tpbrt_vec3_dot(new_up, eye);

		// Z - forward
		m->m[8]					  = dir.x;
		m->m[9]					  = dir.y;
		m->m[10]				  = dir.z;
		m->m[11]				  = -tpbrt_vec3_dot(dir, eye);

		m->m[12]				  = 0.0f;
		m->m[13]				  = 0.0f;
		m->m[14]				  = 0.0f;
		m->m[15]				  = 1.0f;

		// NOLINTEND(readability-magic-numbers)
	}

	void tpbrt_mat4_multiply(tpbrt_mat4_t* const out_m, const tpbrt_mat4_t* a, const tpbrt_mat4_t* b) {
			if (out_m == TPBRT_NULL || a == TPBRT_NULL || b == TPBRT_NULL) { return; }

		const tpbrt_float_t* const a_f = (const tpbrt_float_t*)a;
		const tpbrt_float_t* const b_f = (const tpbrt_float_t*)b;
			for (tpbrt_uint_t c = 0u; c < 4u; ++c) {
					for (tpbrt_uint_t r = 0u; r < 4u; ++r) {
						tpbrt_float_t sum = 0.0f;
							for (tpbrt_uint_t k = 0; k < 4; ++k) { sum += a_f[(r * 4u) + k] * b_f[(k * 4u) + c]; }
						out_m->m[(r * 4u) + c] = sum;
					}
			}
	}

	void tpbrt_mat4_inverse(tpbrt_mat4_t* const out_m, const tpbrt_mat4_t* const in_m) {
			if (out_m == TPBRT_NULL || in_m == TPBRT_NULL) { return; }

		tpbrt_mat4_t inv_m;

		tpbrt_float_t* const inv	 = inv_m.m;
		tpbrt_float_t* const out	 = out_m->m;
		const tpbrt_float_t* const m = in_m->m;

		// NOLINTBEGIN(readability-magic-numbers)

		inv[0]	= (m[5] * m[10] * m[15]) - (m[5] * m[11] * m[14]) - (m[9] * m[6] * m[15]) + (m[9] * m[7] * m[14]) +
				  (m[13] * m[6] * m[11]) - (m[13] * m[7] * m[10]);
		inv[4]	= (-m[4] * m[10] * m[15]) + (m[4] * m[11] * m[14]) + (m[8] * m[6] * m[15]) - (m[8] * m[7] * m[14]) -
				  (m[12] * m[6] * m[11]) + (m[12] * m[7] * m[10]);
		inv[8]	= (m[4] * m[9] * m[15]) - (m[4] * m[11] * m[13]) - (m[8] * m[5] * m[15]) + (m[8] * m[7] * m[13]) +
				  (m[12] * m[5] * m[11]) - (m[12] * m[7] * m[9]);
		inv[12] = (-m[4] * m[9] * m[14]) + (m[4] * m[10] * m[13]) + (m[8] * m[5] * m[14]) - (m[8] * m[6] * m[13]) -
				  (m[12] * m[5] * m[10]) + (m[12] * m[6] * m[9]);

		inv[1]	= (-m[1] * m[10] * m[15]) + (m[1] * m[11] * m[14]) + (m[9] * m[2] * m[15]) - (m[9] * m[3] * m[14]) -
				  (m[13] * m[2] * m[11]) + (m[13] * m[3] * m[10]);
		inv[5]	= (m[0] * m[10] * m[15]) - (m[0] * m[11] * m[14]) - (m[8] * m[2] * m[15]) + (m[8] * m[3] * m[14]) +
				  (m[12] * m[2] * m[11]) - (m[12] * m[3] * m[10]);
		inv[9]	= (-m[0] * m[9] * m[15]) + (m[0] * m[11] * m[13]) + (m[8] * m[1] * m[15]) - (m[8] * m[3] * m[13]) -
				  (m[12] * m[1] * m[11]) + (m[12] * m[3] * m[9]);
		inv[13] = (m[0] * m[9] * m[14]) - (m[0] * m[10] * m[13]) - (m[8] * m[1] * m[14]) + (m[8] * m[2] * m[13]) +
				  (m[12] * m[1] * m[10]) - (m[12] * m[2] * m[9]);

		inv[2]	= (m[1] * m[6] * m[15]) - (m[1] * m[7] * m[14]) - (m[5] * m[2] * m[15]) + (m[5] * m[3] * m[14]) +
				  (m[13] * m[2] * m[7]) - (m[13] * m[3] * m[6]);
		inv[6]	= (-m[0] * m[6] * m[15]) + (m[0] * m[7] * m[14]) + (m[4] * m[2] * m[15]) - (m[4] * m[3] * m[14]) -
				  (m[12] * m[2] * m[7]) + (m[12] * m[3] * m[6]);
		inv[10] = (m[0] * m[5] * m[15]) - (m[0] * m[7] * m[13]) - (m[4] * m[1] * m[15]) + (m[4] * m[3] * m[13]) +
				  (m[12] * m[1] * m[7]) - (m[12] * m[3] * m[5]);
		inv[14] = (-m[0] * m[5] * m[14]) + (m[0] * m[6] * m[13]) + (m[4] * m[1] * m[14]) - (m[4] * m[2] * m[13]) -
				  (m[12] * m[1] * m[6]) + (m[12] * m[2] * m[5]);

		inv[3]	= (-m[1] * m[6] * m[11]) + (m[1] * m[7] * m[10]) + (m[5] * m[2] * m[11]) - (m[5] * m[3] * m[10]) -
				  (m[9] * m[2] * m[7]) + (m[9] * m[3] * m[6]);
		inv[7]	= (m[0] * m[6] * m[11]) - (m[0] * m[7] * m[10]) - (m[4] * m[2] * m[11]) + (m[4] * m[3] * m[10]) +
				  (m[8] * m[2] * m[7]) - (m[8] * m[3] * m[6]);
		inv[11] = (-m[0] * m[5] * m[11]) + (m[0] * m[7] * m[9]) + (m[4] * m[1] * m[11]) - (m[4] * m[3] * m[9]) -
				  (m[8] * m[1] * m[7]) + (m[8] * m[3] * m[5]);
		inv[15] = (m[0] * m[5] * m[10]) - (m[0] * m[6] * m[9]) - (m[4] * m[1] * m[10]) + (m[4] * m[2] * m[9]) +
				  (m[8] * m[1] * m[6]) - (m[8] * m[2] * m[5]);

		const tpbrt_float_t det = (m[0] * inv[0]) + (m[1] * inv[4]) + (m[2] * inv[8]) + (m[3] * inv[12]);

		// NOLINTEND(readability-magic-numbers)

			if (fabsf(det) < TPBRT_EPSILON) {
				tpbrt_mat4_identity(out_m);
				return;
			}

		const tpbrt_float_t inv_det = 1.0f / det;
			for (tpbrt_uint_t i = 0; i < TPBRT_MAT4_ARRAY_SIZE; ++i) { out[i] = inv[i] * inv_det; }
	}

	void tpbrt_mat4_transpose(tpbrt_mat4_t* const out_m, const tpbrt_mat4_t* const in_m) {
			if (out_m == TPBRT_NULL || in_m == TPBRT_NULL) { return; }

			for (tpbrt_uint_t r = 0u; r < 4u; ++r) {
					for (tpbrt_uint_t c = 0u; c < 4u; ++c) { out_m->m[(r * 4u) + c] = in_m->m[(c * 4u) + r]; }
			}
	}

	tpbrt_uint_t tpbrt_mat4_hash(const tpbrt_mat4_t* const m) {
			if (m == TPBRT_NULL) { return 0; }
		const tpbrt_uchar_t* const bytes = (const tpbrt_uchar_t*)m;

		/* clang-format off */
		static const tpbrt_uint_t HASH_START = 2166136261u;
		static const tpbrt_uint_t HASH_MULTIPLIER = 16777619u;
		/* clang-format on */

		tpbrt_uint_t hash				 = HASH_START;
			for (tpbrt_size_t i = 0u; i < sizeof(tpbrt_mat4_t); ++i) {
				hash ^= bytes[i];
				hash *= HASH_MULTIPLIER;
			}
		return hash;
	}

	tpbrt_bool_t tpbrt_mat4_equals(const tpbrt_mat4_t* const a, const tpbrt_mat4_t* const b) {
			if (a == TPBRT_NULL || b == TPBRT_NULL) { return TPBRT_FALSE; }
		return memcmp(a, b, sizeof(tpbrt_mat4_t)) == 0 ? TPBRT_TRUE : TPBRT_FALSE;
	}

	void tpbrt_bool_array_free(tpbrt_bool_array_t* array) {
			if (array == TPBRT_NULL || array->data == TPBRT_NULL) { return; }

		free(array->data);
		*array = TPBRT_EMPTY_BOOL_ARRAY;
	}

	void tpbrt_int_array_free(tpbrt_int_array_t* array) {
			if (array == TPBRT_NULL || array->data == TPBRT_NULL) { return; }

		free(array->data);
		*array = TPBRT_EMPTY_INT_ARRAY;
	}

	void tpbrt_uint_array_free(tpbrt_uint_array_t* array) {
			if (array == TPBRT_NULL || array->data == TPBRT_NULL) { return; }

		free(array->data);
		*array = TPBRT_EMPTY_UINT_ARRAY;
	}

	void tpbrt_float_array_free(tpbrt_float_array_t* array) {
			if (array == TPBRT_NULL || array->data == TPBRT_NULL) { return; }

		free(array->data);
		*array = TPBRT_EMPTY_FLOAT_ARRAY;
	}

	void tpbrt_vec2_array_free(tpbrt_vec2_array_t* array) {
			if (array == TPBRT_NULL || array->data == TPBRT_NULL) { return; }

		free(array->data);
		*array = TPBRT_EMPTY_VEC2_ARRAY;
	}

	void tpbrt_vec3_array_free(tpbrt_vec3_array_t* array) {
			if (array == TPBRT_NULL || array->data == TPBRT_NULL) { return; }

		free(array->data);
		*array = TPBRT_EMPTY_VEC3_ARRAY;
	}

#ifdef __cplusplus
}
#endif