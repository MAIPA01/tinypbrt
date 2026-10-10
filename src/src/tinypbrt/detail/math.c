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

	tpbrt_float_t tpbrt_vec3_dot(const tpbrt_vec3_t a, const tpbrt_vec3_t b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

	tpbrt_vec3_t tpbrt_vec3_cross(const tpbrt_vec3_t a, const tpbrt_vec3_t b) {
		return TPBRT_VEC3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}

	tpbrt_float_t tpbrt_vec3_length(const tpbrt_vec3_t a) { return sqrtf(tpbrt_vec3_dot(a, a)); }

	tpbrt_vec3_t tpbrt_vec3_normalize(tpbrt_vec3_t a) {
		const tpbrt_float_t len = tpbrt_vec3_length(a);

			if (len > TPBRT_EPSILON) {
				a.x /= len;
				a.y /= len;
				a.z /= len;
			}
			else {
				a.x = 0.0f;
				a.y = 0.0f;
				a.z = 0.0f;
			}

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

			for (int i = 0; i < 16; i++) { m->m[i] = i % 5 == 0 ? 1.0f : 0.0f; }
	}

	void tpbrt_mat4_from_array(tpbrt_mat4_t* const m, const tpbrt_float_t arr[16]) {
			if (m == TPBRT_NULL) { return; }
		memcpy(m, arr, 16u * sizeof(tpbrt_float_t));
	}

	void tpbrt_mat4_from_translation(tpbrt_mat4_t* const m, const tpbrt_vec3_t vec) {
			if (m == TPBRT_NULL) { return; }
		tpbrt_mat4_identity(m);

		m->m[3]	 = vec.x;
		m->m[7]	 = vec.y;
		m->m[11] = vec.z;
	}

	void tpbrt_mat4_from_scale(tpbrt_mat4_t* const m, const tpbrt_vec3_t vec) {
			if (m == TPBRT_NULL) { return; }
		tpbrt_mat4_identity(m);

		m->m[0]	 = vec.x;
		m->m[5]	 = vec.y;
		m->m[10] = vec.z;
		m->m[15] = 1.0f;
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

		m->m[0]	 = t * axis.x * axis.x + c;
		m->m[1]	 = t * axis.x * axis.y - s * axis.z;
		m->m[2]	 = t * axis.x * axis.z + s * axis.y;
		m->m[3]	 = 0.0f;

		m->m[4]	 = t * axis.x * axis.y + s * axis.z;
		m->m[5]	 = t * axis.y * axis.y + c;
		m->m[6]	 = t * axis.y * axis.z - s * axis.x;
		m->m[7]	 = 0.0f;

		m->m[8]	 = t * axis.x * axis.z - s * axis.y;
		m->m[9]	 = t * axis.y * axis.z + s * axis.x;
		m->m[10] = t * axis.z * axis.z + c;
		m->m[11] = 0.0f;

		m->m[12] = 0.0f;
		m->m[13] = 0.0f;
		m->m[14] = 0.0f;
		m->m[15] = 1.0f;
	}

	void tpbrt_mat4_look_at_lh(tpbrt_mat4_t* const m, const tpbrt_vec3_t eye, const tpbrt_vec3_t look_at, const tpbrt_vec3_t up) {
			if (m == TPBRT_NULL) { return; }
		const tpbrt_vec3_t dir	  = tpbrt_vec3_normalize(tpbrt_vec3_subtract(look_at, eye));

		const tpbrt_vec3_t right  = tpbrt_vec3_normalize(tpbrt_vec3_cross(up, dir));

		const tpbrt_vec3_t new_up = tpbrt_vec3_cross(dir, right);

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
	}

	void tpbrt_mat4_multiply(tpbrt_mat4_t* const out_m, const tpbrt_mat4_t* a, const tpbrt_mat4_t* b) {
			if (out_m == TPBRT_NULL || a == TPBRT_NULL || b == TPBRT_NULL) { return; }

		const tpbrt_float_t* a_f = (const tpbrt_float_t*)a;
		const tpbrt_float_t* b_f = (const tpbrt_float_t*)b;
			for (int c = 0; c < 4; c++) {
					for (int r = 0; r < 4; r++) {
						out_m->m[c * 4 + r] = a_f[0 * 4 + r] * b_f[c * 4 + 0] + a_f[1 * 4 + r] * b_f[c * 4 + 1] +
											  a_f[2 * 4 + r] * b_f[c * 4 + 2] + a_f[3 * 4 + r] * b_f[c * 4 + 3];
					}
			}
	}

	void tpbrt_mat4_inverse(tpbrt_mat4_t* const out_m, const tpbrt_mat4_t* const in_m) {
			if (out_m == TPBRT_NULL || in_m == TPBRT_NULL) { return; }

		tpbrt_mat4_t inv_m;

		tpbrt_float_t* const inv	 = inv_m.m;
		tpbrt_float_t* const out	 = out_m->m;
		const tpbrt_float_t* const m = in_m->m;

		inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] -
				 m[13] * m[7] * m[10];
		inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] +
				 m[12] * m[7] * m[10];
		inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] -
				 m[12] * m[7] * m[9];
		inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] +
				  m[12] * m[6] * m[9];

		inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] +
				 m[13] * m[3] * m[10];
		inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] -
				 m[12] * m[3] * m[10];
		inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] +
				 m[12] * m[3] * m[9];
		inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] -
				  m[12] * m[2] * m[9];

		inv[2]	= m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] -
				  m[13] * m[3] * m[6];
		inv[6]	= -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] +
				  m[12] * m[3] * m[6];
		inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] -
				  m[12] * m[3] * m[5];
		inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] +
				  m[12] * m[2] * m[5];

		inv[3]	= -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] +
				  m[9] * m[3] * m[6];
		inv[7]	= m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] -
				  m[8] * m[3] * m[6];
		inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] +
				  m[8] * m[3] * m[5];
		inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] -
				  m[8] * m[2] * m[5];

		const tpbrt_float_t det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

			if (fabsf(det) < TPBRT_EPSILON) {
					for (tpbrt_uint_t i = 0; i < 16; ++i) { out[i] = (i % 5 == 0) ? 1.0f : 0.0f; }
				return;
			}

		const tpbrt_float_t inv_det = 1.0f / det;
			for (tpbrt_uint_t i = 0; i < 16; ++i) { out[i] = inv[i] * inv_det; }
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