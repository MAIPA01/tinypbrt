#pragma once
#ifndef _TINYPBRT_COLOR_INTERNAL_H_
#define _TINYPBRT_COLOR_INTERNAL_H_

#include <tinypbrt/detail/color.h>
#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TPBRT_EMPTY_RGB_ARRAY		  (tpbrt_rgb_array_t) { .data = TPBRT_NULL, .count = 0 }
#define TPBRT_EMPTY_WAVELENGTHS_ARRAY (tpbrt_wavelength_array_t) { .data = TPBRT_NULL, .count = 0 }

	tpbrt_error_t tpbrt_color_space_from_string(tpbrt_string_t color_space_str, tpbrt_color_space_t* color_space_out);
	tpbrt_error_t tpbrt_spectrum_builtin_from_string(tpbrt_string_t builtin_str, tpbrt_spectrum_builtin_t* builtin_out);

	void tpbrt_rgb_array_free(tpbrt_rgb_array_t* array);

	tpbrt_error_t tpbrt_wavelengths_array_copy(tpbrt_wavelength_array_t* dst, tpbrt_wavelength_array_t src);
	void tpbrt_wavelength_array_free(tpbrt_wavelength_array_t* wavelength_array);

	tpbrt_error_t tpbrt_spectrum_copy(tpbrt_spectrum_t* dst, tpbrt_spectrum_t src);
	void tpbrt_spectrum_free(tpbrt_spectrum_t* spectrum);

#ifdef __cplusplus
}
#endif
#endif // TINYPBRT_COLOR_INTERNAL_H
