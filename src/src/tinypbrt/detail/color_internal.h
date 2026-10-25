#pragma once
#ifndef _TINYPBRT_COLOR_INTERNAL_H_
#define _TINYPBRT_COLOR_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/color.h>

#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TPBRT_RGB_INIT(r_val, g_val, b_val) { .r = (r_val), .g = (g_val), .b = (b_val) }
#define TPBRT_RGB(r_val, g_val, b_val)		(tpbrt_rgb_t) TPBRT_RGB_INIT(r_val, g_val, b_val)

#define TPBRT_EMPTY_RGB_ARRAY_INIT		   { .data = TPBRT_NULL, .count = 0 }
#define TPBRT_EMPTY_RGB_ARRAY			   (tpbrt_rgb_array_t) TPBRT_EMPTY_RGB_ARRAY_INIT
#define TPBRT_EMPTY_WAVELENGTHS_ARRAY_INIT { .data = TPBRT_NULL, .count = 0 }
#define TPBRT_EMPTY_WAVELENGTHS_ARRAY	   (tpbrt_wavelength_array_t) TPBRT_EMPTY_WAVELENGTHS_ARRAY_INIT

#define TPBRT_SPECTRUM_WAVELENGTHS_INIT                        \
	{                                                          \
		.as.wavelengths = { .data = TPBRT_NULL, .count = 0u }, \
		.type			= TPBRT_SPECTRUM_TYPE_WAVELENGTHS      \
}
#define TPBRT_SPECTRUM_WAVELENGTHS (tpbrt_spectrum_t) TPBRT_SPECTRUM_WAVELENGTHS_INIT
#define TPBRT_SPECTRUM_RGB_INIT(r_val, g_val, b_val)            \
	{                                                           \
		.as.rgb = { .r = (r_val), .g = (g_val), .b = (b_val) }, \
		.type	= TPBRT_SPECTRUM_TYPE_RGB                       \
	}
#define TPBRT_SPECTRUM_RGB(r_val, g_val, b_val) (tpbrt_spectrum_t) TPBRT_SPECTRUM_RGB_INIT(r_val, g_val, b_val)
#define TPBRT_SPECTRUM_BLACKBODY_INIT(value)	{ .as.blackbody = (value), .type = TPBRT_SPECTRUM_TYPE_BLACKBODY }
#define TPBRT_SPECTRUM_BLACKBODY(value)			(tpbrt_spectrum_t) TPBRT_SPECTRUM_BLACKBODY_INIT(value)
#define TPBRT_SPECTRUM_FILE_NAME_INIT                       \
	{                                                       \
		.as.file_name = { .data = TPBRT_NULL, .size = 0u }, \
		.type		  = TPBRT_SPECTRUM_TYPE_FILE            \
	}
#define TPBRT_SPECTRUM_FILE_NAME				   (tpbrt_spectrum_t) TPBRT_SPECTRUM_FILE_NAME_INIT
#define TPBRT_SPECTRUM_BUILTIN_INIT(builtin_value) { .as.builtin = (builtin_value), .type = TPBRT_SPECTRUM_TYPE_BUILTIN }
#define TPBRT_SPECTRUM_BUILTIN(builtin_value)	   (tpbrt_spectrum_t) TPBRT_SPECTRUM_BUILTIN_INIT(builtin_value)
#define TPBRT_SPECTRUM_CONSTANT_INIT(value)		   { .as.constant = (value), .type = TPBRT_SPECTRUM_TYPE_CONSTANT }
#define TPBRT_SPECTRUM_CONSTANT(value)			   (tpbrt_spectrum_t) TPBRT_SPECTRUM_CONSTANT_INIT(value)

	tpbrt_error_t tpbrt_color_space_from_string(tpbrt_string_t color_space_str, tpbrt_color_space_t* color_space_out);
	tpbrt_error_t tpbrt_spectrum_builtin_from_string(tpbrt_string_t builtin_str, tpbrt_spectrum_builtin_t* builtin_out);

	void tpbrt_rgb_array_free(tpbrt_rgb_array_t* array);

	tpbrt_error_t tpbrt_wavelengths_array_copy(tpbrt_wavelength_array_t* dst, tpbrt_wavelength_array_t src);
	void tpbrt_wavelength_array_free(tpbrt_wavelength_array_t* wavelength_array);

	tpbrt_error_t tpbrt_spectrum_copy(tpbrt_spectrum_t* dst, const tpbrt_spectrum_t* src);
	void tpbrt_spectrum_free(tpbrt_spectrum_t* spectrum);

#ifdef __cplusplus
}
#endif
#endif // TINYPBRT_COLOR_INTERNAL_H
