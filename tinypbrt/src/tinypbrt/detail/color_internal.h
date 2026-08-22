#pragma once
#ifndef _TINYPBRT_COLOR_INTERNAL_H_
#define _TINYPBRT_COLOR_INTERNAL_H_

#include <tinypbrt/detail/color.h>
#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_color_space_from_string(const tpbrt_string_t* color_space_str, tpbrt_color_space_t* color_space_out);
	tpbrt_error_t tpbrt_spectrum_builtin_from_string(const tpbrt_string_t* builtin_str, tpbrt_spectrum_builtin_t* builtin_out);

	void tpbrt_free_rgb_array(tpbrt_rgb_array_t* array);

	tpbrt_error_t tpbrt_copy_wavelengths_array(tpbrt_wavelength_array_t* dst, const tpbrt_wavelength_array_t* src);
	void tpbrt_free_wavelength_array(tpbrt_wavelength_array_t* wavelength_array);

	tpbrt_error_t tpbrt_copy_spectrum(tpbrt_spectrum_t* dst, const tpbrt_spectrum_t* src);
	void tpbrt_free_spectrum(tpbrt_spectrum_t* spectrum);

#ifdef __cplusplus
}
#endif
#endif // TINYPBRT_COLOR_INTERNAL_H
