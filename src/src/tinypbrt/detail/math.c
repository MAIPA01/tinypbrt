#include <tinypbrt/pch.h>

#include <tinypbrt/detail/math_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	void tpbrt_mat4_identity(tpbrt_mat4_t* const m) {
			if (m == TPBRT_NULL) { return; }

		tpbrt_float_t* out = (tpbrt_float_t*)m;
			for (int i = 0; i < 16; i++) { out[i] = (i % 5 == 0) ? 1.0f : 0.0f; }
	}

	void tpbrt_mat4_from_array(tpbrt_mat4_t* const m, const tpbrt_float_t arr[16]) {
			if (m == TPBRT_NULL) { return; }
		memcpy(m, arr, 16 * sizeof(tpbrt_float_t));
	}

	void tpbrt_mat4_from_translation(tpbrt_mat4_t* const m, const tpbrt_float_t vec[3]) {
			if (m == TPBRT_NULL) { return; }
		tpbrt_mat4_identity(m);

		tpbrt_float_t* out = (tpbrt_float_t*)m;
		out[3]			   = vec[0];
		out[7]			   = vec[1];
		out[11]			   = vec[2];
	}

	void tpbrt_mat4_from_scale(tpbrt_mat4_t* const m, const tpbrt_float_t vec[3]) {
			if (m == TPBRT_NULL) { return; }
		tpbrt_mat4_identity(m);

		tpbrt_float_t* out = (tpbrt_float_t*)m;
		out[0]			   = vec[0];
		out[5]			   = vec[1];
		out[10]			   = vec[2];
		out[15]			   = 1.0f;
	}

	void tpbrt_mat4_from_axis_angle(tpbrt_mat4_t* const m, const tpbrt_float_t axis[3], const tpbrt_float_t angle) {
			if (m == TPBRT_NULL) { return; }
		const tpbrt_float_t rad = angle * (M_PI / 180.0f);

		const tpbrt_float_t c	= cosf(rad);
		const tpbrt_float_t s	= sinf(rad);
		const tpbrt_float_t t	= 1.0f - c;

		const tpbrt_float_t len = sqrtf(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]);
		tpbrt_float_t x = axis[0], y = axis[1], z = axis[2];

			if (len > 1e-6f) {
				x /= len;
				y /= len;
				z /= len;
			}
			else {
				tpbrt_mat4_identity(m);
				return;
			}

		tpbrt_float_t* out = (tpbrt_float_t*)m;
		out[0]			   = t * x * x + c;
		out[1]			   = t * x * y - s * z;
		out[2]			   = t * x * z + s * y;
		out[3]			   = 0.0f;

		out[4]			   = t * x * y + s * z;
		out[5]			   = t * y * y + c;
		out[6]			   = t * y * z - s * x;
		out[7]			   = 0.0f;

		out[8]			   = t * x * z - s * y;
		out[9]			   = t * y * z + s * x;
		out[10]			   = t * z * z + c;
		out[11]			   = 0.0f;

		out[12]			   = 0.0f;
		out[13]			   = 0.0f;
		out[14]			   = 0.0f;
		out[15]			   = 1.0f;
	}

	void tpbrt_mat4_look_at_lh(tpbrt_mat4_t* const m, const tpbrt_float_t eye[3], const tpbrt_float_t look_at[3],
	  const tpbrt_float_t up[3]) {
			if (m == TPBRT_NULL) { return; }
		tpbrt_float_t dir[3]		= { look_at[0] - eye[0], look_at[1] - eye[1], look_at[2] - eye[2] };
		const tpbrt_float_t dir_len = sqrtf(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
			if (dir_len > 1e-6f) {
				dir[0] /= dir_len;
				dir[1] /= dir_len;
				dir[2] /= dir_len;
			}

		tpbrt_float_t right[3]		  = { up[1] * dir[2] - up[2] * dir[1], up[2] * dir[0] - up[0] * dir[2],
			up[0] * dir[1] - up[1] * dir[0] };
		const tpbrt_float_t right_len = sqrtf(right[0] * right[0] + right[1] * right[1] + right[2] * right[2]);
			if (right_len > 1e-6f) {
				right[0] /= right_len;
				right[1] /= right_len;
				right[2] /= right_len;
			}

		const tpbrt_float_t new_up[3] = { dir[1] * right[2] - dir[2] * right[1], dir[2] * right[0] - dir[0] * right[2],
			dir[0] * right[1] - dir[1] * right[0] };

		tpbrt_float_t* out			  = (tpbrt_float_t*)m;
		// X - right
		out[0]						  = right[0];
		out[1]						  = right[1];
		out[2]						  = right[2];
		out[3]						  = -(right[0] * eye[0] + right[1] * eye[1] + right[2] * eye[2]);

		// Y - up
		out[4]						  = new_up[0];
		out[5]						  = new_up[1];
		out[6]						  = new_up[2];
		out[7]						  = -(new_up[0] * eye[0] + new_up[1] * eye[1] + new_up[2] * eye[2]);

		// Z - forward
		out[8]						  = dir[0];
		out[9]						  = dir[1];
		out[10]						  = dir[2];
		out[11]						  = -(dir[0] * eye[0] + dir[1] * eye[1] + dir[2] * eye[2]);

		out[12]						  = 0.0f;
		out[13]						  = 0.0f;
		out[14]						  = 0.0f;
		out[15]						  = 1.0f;
	}

	void tpbrt_mat4_multiply(tpbrt_mat4_t* out_m, const tpbrt_mat4_t* a, const tpbrt_mat4_t* b) {
			if (out_m == TPBRT_NULL || a == TPBRT_NULL || b == TPBRT_NULL) { return; }

		tpbrt_float_t* out		 = (tpbrt_float_t*)out_m;
		const tpbrt_float_t* a_f = (const tpbrt_float_t*)a;
		const tpbrt_float_t* b_f = (const tpbrt_float_t*)b;
			for (int c = 0; c < 4; c++) {
					for (int r = 0; r < 4; r++) {
						out[c * 4 + r] = a_f[0 * 4 + r] * b_f[c * 4 + 0] + a_f[1 * 4 + r] * b_f[c * 4 + 1] +
										 a_f[2 * 4 + r] * b_f[c * 4 + 2] + a_f[3 * 4 + r] * b_f[c * 4 + 3];
					}
			}
	}

	void tpbrt_mat4_inverse(tpbrt_mat4_t* const out_m, const tpbrt_mat4_t* const in_m) {
			if (out_m == TPBRT_NULL || in_m == TPBRT_NULL) { return; }

		tpbrt_mat4_t inv_m;

		tpbrt_float_t* inv	   = (tpbrt_float_t*)&inv_m;
		tpbrt_float_t* out	   = (tpbrt_float_t*)out_m;
		const tpbrt_float_t* m = (const tpbrt_float_t*)in_m;

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

			if (fabsf(det) < 1e-6f) {
					for (tpbrt_uint_t i = 0; i < 16; ++i) { out[i] = (i % 5 == 0) ? 1.0f : 0.0f; }
				return;
			}

		const tpbrt_float_t inv_det = 1.0f / det;
			for (tpbrt_uint_t i = 0; i < 16; ++i) { out[i] = inv[i] * inv_det; }
	}

	void tpbrt_free_bool_array(tpbrt_bool_array_t* array) {
			if (array == TPBRT_NULL || array->data == TPBRT_NULL) { return; }
		free(array->data);
		array->data	 = TPBRT_NULL;
		array->count = 0;
	}

	void tpbrt_free_int_array(tpbrt_int_array_t* array) {
			if (array == TPBRT_NULL || array->data == TPBRT_NULL) { return; }
		free(array->data);
		array->data	 = TPBRT_NULL;
		array->count = 0;
	}

	void tpbrt_free_uint_array(tpbrt_uint_array_t* array) {
			if (array == TPBRT_NULL || array->data == TPBRT_NULL) { return; }
		free(array->data);
		array->data	 = TPBRT_NULL;
		array->count = 0;
	}

	void tpbrt_free_float_array(tpbrt_float_array_t* array) {
			if (array == TPBRT_NULL || array->data == TPBRT_NULL) { return; }
		free(array->data);
		array->data	 = TPBRT_NULL;
		array->count = 0;
	}

	void tpbrt_free_vec2_array(tpbrt_vec2_array_t* array) {
			if (array == TPBRT_NULL || array->data == TPBRT_NULL) { return; }
		free(array->data);
		array->data	 = TPBRT_NULL;
		array->count = 0;
	}

	void tpbrt_free_vec3_array(tpbrt_vec3_array_t* array) {
			if (array == TPBRT_NULL || array->data == TPBRT_NULL) { return; }
		free(array->data);
		array->data	 = TPBRT_NULL;
		array->count = 0;
	}

#ifdef __cplusplus
}
#endif