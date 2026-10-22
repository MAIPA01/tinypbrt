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

		static const tpbrt_uint_t STEP_SIZE = 5u;
			for (tpbrt_uint_t i = 0; i < TPBRT_MAT4_ARRAY_SIZE; ++i) { m->m[i] = i % STEP_SIZE == 0 ? 1.0f : 0.0f; }
	}

	void tpbrt_mat4_from_array(tpbrt_mat4_t* const m, const tpbrt_float_t arr[TPBRT_MAT4_ARRAY_SIZE]) {
			if (m == TPBRT_NULL) { return; }
		memcpy(m, arr, TPBRT_MAT4_ARRAY_SIZE * sizeof(tpbrt_float_t));
	}

	void tpbrt_mat4_from_translation(tpbrt_mat4_t* const m, const tpbrt_vec3_t vec) {
			if (m == TPBRT_NULL) { return; }
		tpbrt_mat4_identity(m);

		m->m03 = vec.x;
		m->m13 = vec.y;
		m->m23 = vec.z;
	}

	void tpbrt_mat4_from_scale(tpbrt_mat4_t* const m, const tpbrt_vec3_t vec) {
			if (m == TPBRT_NULL) { return; }
		tpbrt_mat4_identity(m);

		m->m00 = vec.x;
		m->m11 = vec.y;
		m->m22 = vec.z;
		m->m33 = 1.0f;
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

		m->row0.x = (t * axis.x * axis.x) + c;
		m->row0.y = (t * axis.x * axis.y) - (s * axis.z);
		m->row0.z = (t * axis.x * axis.z) + (s * axis.y);
		m->row0.w = 0.0f;

		m->row1.x = (t * axis.x * axis.y) + (s * axis.z);
		m->row1.y = (t * axis.y * axis.y) + c;
		m->row1.z = (t * axis.y * axis.z) - (s * axis.x);
		m->row1.w = 0.0f;

		m->row2.x = (t * axis.x * axis.z) - (s * axis.y);
		m->row2.y = (t * axis.y * axis.z) + (s * axis.x);
		m->row2.z = (t * axis.z * axis.z) + c;
		m->row2.w = 0.0f;

		m->row3	  = TPBRT_VEC4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	void tpbrt_mat4_look_at_lh(tpbrt_mat4_t* const m, const tpbrt_vec3_t eye, const tpbrt_vec3_t look_at, const tpbrt_vec3_t up) {
			if (m == TPBRT_NULL) { return; }
		const tpbrt_vec3_t dir	  = tpbrt_vec3_normalize(tpbrt_vec3_subtract(look_at, eye));

		const tpbrt_vec3_t right  = tpbrt_vec3_normalize(tpbrt_vec3_cross(up, dir));

		const tpbrt_vec3_t new_up = tpbrt_vec3_cross(dir, right);

		// X - right
		m->row0					  = TPBRT_VEC4(right.x, right.y, right.z, -tpbrt_vec3_dot(right, eye));

		// Y - up
		m->row1					  = TPBRT_VEC4(new_up.x, new_up.y, new_up.z, -tpbrt_vec3_dot(new_up, eye));

		// Z - forward
		m->row2					  = TPBRT_VEC4(dir.x, dir.y, dir.z, -tpbrt_vec3_dot(dir, eye));

		m->row3					  = TPBRT_VEC4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	void tpbrt_mat4_multiply(tpbrt_mat4_t* const out_m, const tpbrt_mat4_t* a, const tpbrt_mat4_t* b) {
			if (out_m == TPBRT_NULL || a == TPBRT_NULL || b == TPBRT_NULL) { return; }

		const tpbrt_mat4_row_t* const a_r = a->rows;
		const tpbrt_mat4_row_t* const b_r = b->rows;
			for (tpbrt_uint_t c = 0u; c < 4u; ++c) {
					for (tpbrt_uint_t r = 0u; r < 4u; ++r) {
						tpbrt_float_t sum = 0.0f;
							for (tpbrt_uint_t k = 0u; k < 4u; ++k) { sum += a_r[r].v[k] * b_r[k].v[c]; }
						out_m->rows[r].v[c] = sum;
					}
			}
	}

	void tpbrt_mat4_inverse(tpbrt_mat4_t* const out_m, const tpbrt_mat4_t* const in_m) {
			if (out_m == TPBRT_NULL || in_m == TPBRT_NULL) { return; }

		tpbrt_mat4_t inv_m;

		tpbrt_mat4_row_t* const inv		= inv_m.rows;
		tpbrt_mat4_row_t* const out		= out_m->rows;
		const tpbrt_mat4_row_t* const m = in_m->rows;

		inv[0].x						= (m[1].y * m[2].z * m[3].w) - (m[1].y * m[2].w * m[3].z) - (m[2].y * m[1].z * m[3].w) +
										  (m[2].y * m[1].w * m[3].z) + (m[3].y * m[1].z * m[2].w) - (m[3].y * m[1].w * m[2].z);
		inv[1].x						= (-m[1].x * m[2].z * m[3].w) + (m[1].x * m[2].w * m[3].z) + (m[2].x * m[1].z * m[3].w) -
										  (m[2].x * m[1].w * m[3].z) - (m[3].x * m[1].z * m[2].w) + (m[3].x * m[1].w * m[2].z);
		inv[2].x						= (m[1].x * m[2].y * m[3].w) - (m[1].x * m[2].w * m[3].y) - (m[2].x * m[1].y * m[3].w) +
										  (m[2].x * m[1].w * m[3].y) + (m[3].x * m[1].y * m[2].w) - (m[3].x * m[1].w * m[2].y);
		inv[3].x						= (-m[1].x * m[2].y * m[3].z) + (m[1].x * m[2].z * m[3].y) + (m[2].x * m[1].y * m[3].z) -
										  (m[2].x * m[1].z * m[3].y) - (m[3].x * m[1].y * m[2].z) + (m[3].x * m[1].z * m[2].y);

		inv[0].y						= (-m[0].y * m[2].z * m[3].w) + (m[0].y * m[2].w * m[3].z) + (m[2].y * m[0].z * m[3].w) -
										  (m[2].y * m[0].w * m[3].z) - (m[3].y * m[0].z * m[2].w) + (m[3].y * m[0].w * m[2].z);
		inv[1].y						= (m[0].x * m[2].z * m[3].w) - (m[0].x * m[2].w * m[3].z) - (m[2].x * m[0].z * m[3].w) +
										  (m[2].x * m[0].w * m[3].z) + (m[3].x * m[0].z * m[2].w) - (m[3].x * m[0].w * m[2].z);
		inv[2].y						= (-m[0].x * m[2].y * m[3].w) + (m[0].x * m[2].w * m[3].y) + (m[2].x * m[0].y * m[3].w) -
										  (m[2].x * m[0].w * m[3].y) - (m[3].x * m[0].y * m[2].w) + (m[3].x * m[0].w * m[2].y);
		inv[3].y						= (m[0].x * m[2].y * m[3].z) - (m[0].x * m[2].z * m[3].y) - (m[2].x * m[0].y * m[3].z) +
										  (m[2].x * m[0].z * m[3].y) + (m[3].x * m[0].y * m[2].z) - (m[3].x * m[0].z * m[2].y);

		inv[0].z						= (m[0].y * m[1].z * m[3].w) - (m[0].y * m[1].w * m[3].z) - (m[1].y * m[0].z * m[3].w) +
										  (m[1].y * m[0].w * m[3].z) + (m[3].y * m[0].z * m[1].w) - (m[3].y * m[0].w * m[1].z);
		inv[1].z						= (-m[0].x * m[1].z * m[3].w) + (m[0].x * m[1].w * m[3].z) + (m[1].x * m[0].z * m[3].w) -
										  (m[1].x * m[0].w * m[3].z) - (m[3].x * m[0].z * m[1].w) + (m[3].x * m[0].w * m[1].z);
		inv[2].z						= (m[0].x * m[1].y * m[3].w) - (m[0].x * m[1].w * m[3].y) - (m[1].x * m[0].y * m[3].w) +
										  (m[1].x * m[0].w * m[3].y) + (m[3].x * m[0].y * m[1].w) - (m[3].x * m[0].w * m[1].y);
		inv[3].z						= (-m[0].x * m[1].y * m[3].z) + (m[0].x * m[1].z * m[3].y) + (m[1].x * m[0].y * m[3].z) -
										  (m[1].x * m[0].z * m[3].y) - (m[3].x * m[0].y * m[1].z) + (m[3].x * m[0].z * m[1].y);

		inv[0].w						= (-m[0].y * m[1].z * m[2].w) + (m[0].y * m[1].w * m[2].z) + (m[1].y * m[0].z * m[2].w) -
										  (m[1].y * m[0].w * m[2].z) - (m[2].y * m[0].z * m[1].w) + (m[2].y * m[0].w * m[1].z);
		inv[1].w						= (m[0].x * m[1].z * m[2].w) - (m[0].x * m[1].w * m[2].z) - (m[1].x * m[0].z * m[2].w) +
										  (m[1].x * m[0].w * m[2].z) + (m[2].x * m[0].z * m[1].w) - (m[2].x * m[0].w * m[1].z);
		inv[2].w						= (-m[0].x * m[1].y * m[2].w) + (m[0].x * m[1].w * m[2].y) + (m[1].x * m[0].y * m[2].w) -
										  (m[1].x * m[0].w * m[2].y) - (m[2].x * m[0].y * m[1].w) + (m[2].x * m[0].w * m[1].y);
		inv[3].w						= (m[0].x * m[1].y * m[2].z) - (m[0].x * m[1].z * m[2].y) - (m[1].x * m[0].y * m[2].z) +
										  (m[1].x * m[0].z * m[2].y) + (m[2].x * m[0].y * m[1].z) - (m[2].x * m[0].z * m[1].y);

		const tpbrt_float_t det			= (m[0].x * inv[0].x) + (m[0].y * inv[1].x) + (m[0].z * inv[2].x) + (m[0].w * inv[3].x);

			if (fabsf(det) < TPBRT_EPSILON) {
				tpbrt_mat4_identity(out_m);
				return;
			}

		const tpbrt_float_t inv_det = 1.0f / det;
			for (tpbrt_uint_t r = 0u; r < 4u; ++r) {
					for (tpbrt_uint_t c = 0u; c < 4u; ++c) { out[r].v[c] = inv[r].v[c] * inv_det; }
			}
	}

	void tpbrt_mat4_transpose(tpbrt_mat4_t* const out_m, const tpbrt_mat4_t* const in_m) {
			if (out_m == TPBRT_NULL || in_m == TPBRT_NULL) { return; }

			for (tpbrt_uint_t r = 0u; r < 4u; ++r) {
					for (tpbrt_uint_t c = 0u; c < 4u; ++c) { out_m->rows[r].v[c] = in_m->rows[c].v[r]; }
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
