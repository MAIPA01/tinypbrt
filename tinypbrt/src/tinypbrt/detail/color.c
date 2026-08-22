#include <tinypbrt/pch.h>

#include <tinypbrt/detail/color_internal.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_color_space_from_string(const tpbrt_string_t* const color_space_str,
	  tpbrt_color_space_t* const color_space_out) {
		static const tpbrt_string_t COLOR_SPACES_STRS[TPBRT_COLOR_SPACE_MAX_NUM] = {
			TPBRT_STRING("aces2065-1"),
			TPBRT_STRING("rec2020"),
			TPBRT_STRING("dci-p3"),
			TPBRT_STRING("srgb"),
		};

			if (color_space_str == TPBRT_NULL || color_space_str->data == TPBRT_NULL || color_space_out == NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_color_space_t cs = 0; cs < TPBRT_COLOR_SPACE_MAX_NUM; ++cs) {
					if (tpbrt_string_equals(color_space_str, COLOR_SPACES_STRS + cs)) {
						*color_space_out = cs;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_COLOR_SPACE;
	}

	tpbrt_error_t tpbrt_spectrum_builtin_from_string(const tpbrt_string_t* const builtin_str,
	  tpbrt_spectrum_builtin_t* const builtin_out) {
		static const tpbrt_string_t BUILTINS_STRS[TPBRT_SPECTRUM_BUILTIN_MAX_NUM] = {
			TPBRT_STRING("glass-BK7"),
			TPBRT_STRING("glass-BAF10"),
			TPBRT_STRING("glass-FK51A"),
			TPBRT_STRING("glass-LASF9"),
			TPBRT_STRING("glass-F5"),
			TPBRT_STRING("glass-F10"),
			TPBRT_STRING("glass-F11"),
			TPBRT_STRING("metal-Ag-eta"),
			TPBRT_STRING("metal-Al-eta"),
			TPBRT_STRING("metal-Au-eta"),
			TPBRT_STRING("metal-Cu-eta"),
			TPBRT_STRING("metal-CuZn-eta"),
			TPBRT_STRING("metal-MgO-eta"),
			TPBRT_STRING("metal-TiO2-eta"),
			TPBRT_STRING("stdillum-A"),
			TPBRT_STRING("stdillum-D50"),
			TPBRT_STRING("stdillum-D65"),
			TPBRT_STRING("stdillum-F1"),
			TPBRT_STRING("stdillum-F2"),
			TPBRT_STRING("stdillum-F3"),
			TPBRT_STRING("stdillum-F4"),
			TPBRT_STRING("stdillum-F5"),
			TPBRT_STRING("stdillum-F6"),
			TPBRT_STRING("stdillum-F7"),
			TPBRT_STRING("stdillum-F8"),
			TPBRT_STRING("stdillum-F9"),
			TPBRT_STRING("stdillum-F10"),
			TPBRT_STRING("stdillum-F11"),
			TPBRT_STRING("stdillum-F12"),
			TPBRT_STRING("illum-acesD60"),
		};

		static const tpbrt_string_t
		  METALS_KS_BUILTINS_STRS[TPBRT_SPECTRUM_BUILTIN_METAL_TI_O2_K - TPBRT_SPECTRUM_BUILTIN_METAL_AG_K + 1] = {
			  TPBRT_STRING("metal-Ag-k"),
			  TPBRT_STRING("metal-Al-k"),
			  TPBRT_STRING("metal-Au-k"),
			  TPBRT_STRING("metal-Cu-k"),
			  TPBRT_STRING("metal-CuZn-k"),
			  TPBRT_STRING("metal-MgO-k"),
			  TPBRT_STRING("metal-TiO2-k"),
		  };

			if (builtin_str == TPBRT_NULL || builtin_str->data == TPBRT_NULL || builtin_out == NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_spectrum_builtin_t b = 0; b < TPBRT_SPECTRUM_BUILTIN_MAX_NUM; ++b) {
					if (tpbrt_string_equals(builtin_str, BUILTINS_STRS + b)) {
						*builtin_out = b;
						return TPBRT_ERROR_NONE;
					}
			}

			for (tpbrt_spectrum_builtin_t b = TPBRT_SPECTRUM_BUILTIN_METAL_AG_K; b <= TPBRT_SPECTRUM_BUILTIN_METAL_TI_O2_K; ++b) {
					if (tpbrt_string_equals(builtin_str, METALS_KS_BUILTINS_STRS + b)) {
						*builtin_out = b;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_SPECTRUM_BUILTIN;
	}

	void tpbrt_free_rgb_array(tpbrt_rgb_array_t* const array) {
			if (array == TPBRT_NULL || array->data == TPBRT_NULL) { return; }

		free(array->data);
		array->data	 = TPBRT_NULL;
		array->count = 0;
	}

	tpbrt_error_t tpbrt_copy_wavelengths_array(tpbrt_wavelength_array_t* const dst, const tpbrt_wavelength_array_t* const src) {
			if (dst == TPBRT_NULL || src == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (src->data == TPBRT_NULL || src->count == 0) {
				dst->data  = TPBRT_NULL;
				dst->count = 0;
				return TPBRT_ERROR_NONE;
			}

		dst->data = malloc(sizeof(tpbrt_wavelength_t) * src->count);
			if (dst->data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		memcpy(dst->data, src->data, sizeof(tpbrt_wavelength_t) * src->count);
		dst->count = src->count;
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_wavelength_array(tpbrt_wavelength_array_t* const wavelength_array) {
			if (wavelength_array == TPBRT_NULL || wavelength_array->data == TPBRT_NULL) { return; }

		free(wavelength_array->data);
		wavelength_array->data	= TPBRT_NULL;
		wavelength_array->count = 0;
	}

	tpbrt_error_t tpbrt_copy_spectrum(tpbrt_spectrum_t* const dst, const tpbrt_spectrum_t* const src) {
			if (dst == TPBRT_NULL || src == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		dst->type = src->type;

			if (src->type == TPBRT_SPECTRUM_TYPE_WAVELENGTH) {
				const tpbrt_error_t err = tpbrt_copy_wavelengths_array(&dst->as.wavelengths, &src->as.wavelengths);
					if (err != TPBRT_ERROR_NONE) { return err; }
			}
			else if (src->type == TPBRT_SPECTRUM_TYPE_RGB) {
				dst->as.rgb.r = src->as.rgb.r;
				dst->as.rgb.g = src->as.rgb.g;
				dst->as.rgb.b = src->as.rgb.b;
			}
			else if (src->type == TPBRT_SPECTRUM_TYPE_BLACKBODY) { dst->as.blackbody = src->as.blackbody; }
			else if (src->type == TPBRT_SPECTRUM_TYPE_FILE) {
				const tpbrt_error_t err = tpbrt_copy_string(&dst->as.file_name, &src->as.file_name);
					if (err != TPBRT_ERROR_NONE) { return err; }
			}
			else if (src->type == TPBRT_SPECTRUM_TYPE_BUILTIN) { dst->as.builtin = src->as.builtin; }

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_spectrum(tpbrt_spectrum_t* const spectrum) {
			if (spectrum == TPBRT_NULL) { return; }

			switch (spectrum->type) {
				case TPBRT_SPECTRUM_TYPE_WAVELENGTH: {
					tpbrt_free_wavelength_array(&spectrum->as.wavelengths);
				}
				case TPBRT_SPECTRUM_TYPE_FILE: {
					tpbrt_free_string(&spectrum->as.file_name);
				}
				default: {
					break;
				}
			}
	}

#ifdef __cplusplus
}
#endif