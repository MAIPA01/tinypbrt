#include <tinypbrt/pch.h>

#include <tinypbrt/detail/color_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_color_space_from_string(const tpbrt_string_t color_space_str,
	  tpbrt_color_space_t* const color_space_out) {
		static const tpbrt_string_t COLOR_SPACES_STRS[TPBRT_COLOR_SPACE_MAX_NUM] = {
			TPBRT_STRING_INIT("aces2065-1"),
			TPBRT_STRING_INIT("rec2020"),
			TPBRT_STRING_INIT("dci-p3"),
			TPBRT_STRING_INIT("srgb"),
		};

			if (color_space_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(color_space_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_COLOR_SPACE; }

			for (tpbrt_color_space_t cs = 0; cs < TPBRT_COLOR_SPACE_MAX_NUM; ++cs) {
					if (tpbrt_string_equals(color_space_str, COLOR_SPACES_STRS[cs]) == TPBRT_TRUE) {
						*color_space_out = cs;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_COLOR_SPACE;
	}

	tpbrt_error_t tpbrt_spectrum_builtin_from_string(const tpbrt_string_t builtin_str,
	  tpbrt_spectrum_builtin_t* const builtin_out) {
		static const tpbrt_string_t BUILTINS_STRS[TPBRT_SPECTRUM_BUILTIN_MAX_NUM] = {
			TPBRT_STRING_INIT("glass-BK7"),
			TPBRT_STRING_INIT("glass-BAF10"),
			TPBRT_STRING_INIT("glass-FK51A"),
			TPBRT_STRING_INIT("glass-LASF9"),
			TPBRT_STRING_INIT("glass-F5"),
			TPBRT_STRING_INIT("glass-F10"),
			TPBRT_STRING_INIT("glass-F11"),
			TPBRT_STRING_INIT("metal-Ag-eta"),
			TPBRT_STRING_INIT("metal-Ag-k"),
			TPBRT_STRING_INIT("metal-Al-eta"),
			TPBRT_STRING_INIT("metal-Al-k"),
			TPBRT_STRING_INIT("metal-Au-eta"),
			TPBRT_STRING_INIT("metal-Au-k"),
			TPBRT_STRING_INIT("metal-Cu-eta"),
			TPBRT_STRING_INIT("metal-Cu-k"),
			TPBRT_STRING_INIT("metal-CuZn-eta"),
			TPBRT_STRING_INIT("metal-CuZn-k"),
			TPBRT_STRING_INIT("metal-MgO-eta"),
			TPBRT_STRING_INIT("metal-MgO-k"),
			TPBRT_STRING_INIT("metal-TiO2-eta"),
			TPBRT_STRING_INIT("metal-TiO2-k"),
			TPBRT_STRING_INIT("stdillum-A"),
			TPBRT_STRING_INIT("stdillum-D50"),
			TPBRT_STRING_INIT("stdillum-D65"),
			TPBRT_STRING_INIT("stdillum-F1"),
			TPBRT_STRING_INIT("stdillum-F2"),
			TPBRT_STRING_INIT("stdillum-F3"),
			TPBRT_STRING_INIT("stdillum-F4"),
			TPBRT_STRING_INIT("stdillum-F5"),
			TPBRT_STRING_INIT("stdillum-F6"),
			TPBRT_STRING_INIT("stdillum-F7"),
			TPBRT_STRING_INIT("stdillum-F8"),
			TPBRT_STRING_INIT("stdillum-F9"),
			TPBRT_STRING_INIT("stdillum-F10"),
			TPBRT_STRING_INIT("stdillum-F11"),
			TPBRT_STRING_INIT("stdillum-F12"),
			TPBRT_STRING_INIT("illum-acesD60"),
		};

			if (builtin_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(builtin_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_SPECTRUM_BUILTIN; }

			for (tpbrt_spectrum_builtin_t b = 0; b < TPBRT_SPECTRUM_BUILTIN_MAX_NUM; ++b) {
					if (tpbrt_string_equals(builtin_str, BUILTINS_STRS[b]) == TPBRT_TRUE) {
						*builtin_out = b;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_SPECTRUM_BUILTIN;
	}

	void tpbrt_rgb_array_free(tpbrt_rgb_array_t* const array) { tpbrt_vec3_array_free(array); }

	tpbrt_error_t tpbrt_wavelengths_array_copy(tpbrt_wavelength_array_t* const dst, const tpbrt_wavelength_array_t src) {
			if (dst == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (src.data == TPBRT_NULL || src.count == 0) {
				*dst = TPBRT_EMPTY_WAVELENGTHS_ARRAY;
				return TPBRT_ERROR_NONE;
			}

		dst->data = malloc(sizeof(tpbrt_wavelength_t) * src.count);
			if (dst->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		memcpy(dst->data, src.data, sizeof(tpbrt_wavelength_t) * src.count);
		dst->count = src.count;
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_wavelength_array_free(tpbrt_wavelength_array_t* const wavelength_array) {
			if (wavelength_array == TPBRT_NULL || wavelength_array->data == TPBRT_NULL) { return; }

		free(wavelength_array->data);
		*wavelength_array = TPBRT_EMPTY_WAVELENGTHS_ARRAY;
	}

	tpbrt_error_t tpbrt_spectrum_copy(tpbrt_spectrum_t* const dst, const tpbrt_spectrum_t src) {
			if (dst == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		dst->type = src.type;

			switch (src.type) {
				case TPBRT_SPECTRUM_TYPE_WAVELENGTHS: {
					const tpbrt_error_t err = tpbrt_wavelengths_array_copy(&dst->as.wavelengths, src.as.wavelengths);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
				case TPBRT_SPECTRUM_TYPE_RGB: {
					dst->as.rgb = src.as.rgb;
					break;
				}
				case TPBRT_SPECTRUM_TYPE_BLACKBODY: {
					dst->as.blackbody = src.as.blackbody;
					break;
				}
				case TPBRT_SPECTRUM_TYPE_FILE: {
					const tpbrt_error_t err = tpbrt_string_copy(&dst->as.file_name, src.as.file_name);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
				case TPBRT_SPECTRUM_TYPE_BUILTIN: {
					dst->as.builtin = src.as.builtin;
					break;
				}
				case TPBRT_SPECTRUM_TYPE_CONSTANT: {
					dst->as.constant = src.as.constant;
					break;
				}
				default: {
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_spectrum_free(tpbrt_spectrum_t* const spectrum) {
			if (spectrum == TPBRT_NULL) { return; }

			switch (spectrum->type) {
				case TPBRT_SPECTRUM_TYPE_WAVELENGTHS: {
					tpbrt_wavelength_array_free(&spectrum->as.wavelengths);
					break;
				}
				case TPBRT_SPECTRUM_TYPE_FILE: {
					tpbrt_string_free(&spectrum->as.file_name);
					break;
				}
				default: {
					break;
				}
			}
	}

#ifdef __cplusplus
}
#endif
