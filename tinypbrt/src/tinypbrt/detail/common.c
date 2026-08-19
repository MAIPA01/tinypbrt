#include <tinypbrt/pch.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_copy_string(tpbrt_string_t* const dst, const tpbrt_string_t* const src) {
			if (dst == TPBRT_NULL || src == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (src->size == 0 || src->chars == TPBRT_NULL) {
				dst->chars = TPBRT_NULL;
				dst->size  = 0;
				return TPBRT_ERROR_NONE;
			}

		dst->chars = malloc(sizeof(tpbrt_char_t) * (src->size + 1));
			if (dst->chars == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		memcpy(dst->chars, src->chars, sizeof(tpbrt_char_t) * src->size);
		dst->chars[src->size] = '\0';
		dst->size			  = src->size;

		return TPBRT_ERROR_NONE;
	}

#ifdef __cplusplus
}
#endif