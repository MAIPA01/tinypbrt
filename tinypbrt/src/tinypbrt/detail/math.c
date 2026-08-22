#include <tinypbrt/pch.h>

#include <tinypbrt/detail/math_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	void tpbrt_mat4_identity(tpbrt_mat4_t* const m) {
		tpbrt_float_t* out = (tpbrt_float_t*)m;
			for (int i = 0; i < 16; i++) { out[i] = (i % 5 == 0) ? 1.0f : 0.0f; }
	}

	void tpbrt_mat4_multiply(tpbrt_mat4_t* out, const tpbrt_mat4_t* a, const tpbrt_mat4_t* b) {
		tpbrt_float_t* o		 = (tpbrt_float_t*)out;
		const tpbrt_float_t* a_f = (const tpbrt_float_t*)a;
		const tpbrt_float_t* b_f = (const tpbrt_float_t*)b;
			for (int c = 0; c < 4; c++) {
					for (int r = 0; r < 4; r++) {
						o[c * 4 + r] = a_f[0 * 4 + r] * b_f[c * 4 + 0] + a_f[1 * 4 + r] * b_f[c * 4 + 1] +
									   a_f[2 * 4 + r] * b_f[c * 4 + 2] + a_f[3 * 4 + r] * b_f[c * 4 + 3];
					}
			}
	}

	void tpbrt_mat4_from_array(tpbrt_mat4_t* m, const tpbrt_float_t arr[16]) { memcpy(m, arr, 16 * sizeof(tpbrt_float_t)); }

	void tpbrt_mat4_inverse(float out_m[16], const float m[16]) {
		float inv[16];

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

		const float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

			if (fabsf(det) < 1e-6f) {
					for (int i = 0; i < 16; i++) { out_m[i] = (i % 5 == 0) ? 1.0f : 0.0f; }
				return;
			}

		const float inv_det = 1.0f / det;
			for (int i = 0; i < 16; i++) { out_m[i] = inv[i] * inv_det; }
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