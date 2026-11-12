#pragma once
#ifndef _TINYPBRT_COLOR_INTERNAL_H_
#define _TINYPBRT_COLOR_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/color.h>

#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup color-private Color
	 * @ingroup tinypbrt-private
	 */

/**
 * @brief Initializes RGB value.
 * @param r_val Red value.
 * @param g_val Green value.
 * @param b_val Blue value.
 * @ingroup color-private
 */
#define TPBRT_RGB_INIT(r_val, g_val, b_val) { .r = (r_val), .g = (g_val), .b = (b_val) }
/**
 * @brief Initializes RGB value with type conversion.
 * @param r_val Red value.
 * @param g_val Green value.
 * @param b_val Blue value.
 * @ingroup color-private
 */
#define TPBRT_RGB(r_val, g_val, b_val) (tpbrt_rgb_t) TPBRT_RGB_INIT(r_val, g_val, b_val)

/**
 * @brief Initializes empty RGB array.
 * @ingroup color-private
 */
#define TPBRT_EMPTY_RGB_ARRAY_INIT { .data = TPBRT_NULL, .count = 0 }
/**
 * @brief Initializes empty RGB array with type conversion.
 * @ingroup color-private
 */
#define TPBRT_EMPTY_RGB_ARRAY (tpbrt_rgb_array_t) TPBRT_EMPTY_RGB_ARRAY_INIT
/**
 * @brief Initializes empty wavelengths array.
 * @ingroup color-private
 */
#define TPBRT_EMPTY_WAVELENGTHS_ARRAY_INIT { .data = TPBRT_NULL, .count = 0 }
/**
 * @brief Initializes empty wavelengths array with type conversion.
 * @ingroup color-private
 */
#define TPBRT_EMPTY_WAVELENGTHS_ARRAY (tpbrt_wavelength_array_t) TPBRT_EMPTY_WAVELENGTHS_ARRAY_INIT

/**
 * @brief Initializes spectrum with empty wavelengths.
 * @ingroup color-private
 */
#define TPBRT_SPECTRUM_WAVELENGTHS_INIT                        \
	{                                                          \
		.as.wavelengths = { .data = TPBRT_NULL, .count = 0u }, \
		.type			= TPBRT_SPECTRUM_TYPE_WAVELENGTHS      \
}
/**
 * @brief Initializes spectrum with empty wavelengths with type conversion.
 * @ingroup color-private
 */
#define TPBRT_SPECTRUM_WAVELENGTHS (tpbrt_spectrum_t) TPBRT_SPECTRUM_WAVELENGTHS_INIT
/**
 * @brief Initializes spectrum with rgb.
 * @param r_val Red value.
 * @param g_val Green value.
 * @param b_val Blue value.
 * @ingroup color-private
 */
#define TPBRT_SPECTRUM_RGB_INIT(r_val, g_val, b_val)            \
	{                                                           \
		.as.rgb = { .r = (r_val), .g = (g_val), .b = (b_val) }, \
		.type	= TPBRT_SPECTRUM_TYPE_RGB                       \
	}
/**
 * @brief Initializes spectrum with rgb with type conversion.
 * @param r_val Red value.
 * @param g_val Green value.
 * @param b_val Blue value.
 * @ingroup color-private
 */
#define TPBRT_SPECTRUM_RGB(r_val, g_val, b_val) (tpbrt_spectrum_t) TPBRT_SPECTRUM_RGB_INIT(r_val, g_val, b_val)
/**
 * @brief Initializes spectrum with blackbody temperature.
 * @param value Blackbody temperature value.
 * @ingroup color-private
 */
#define TPBRT_SPECTRUM_BLACKBODY_INIT(value) { .as.blackbody = (value), .type = TPBRT_SPECTRUM_TYPE_BLACKBODY }
/**
 * @brief Initializes spectrum with blackbody temperature with type conversion.
 * @param value Blackbody temperature value.
 * @ingroup color-private
 */
#define TPBRT_SPECTRUM_BLACKBODY(value) (tpbrt_spectrum_t) TPBRT_SPECTRUM_BLACKBODY_INIT(value)
/**
 * @brief Initializes spectrum with empty file name to wavelengths file.
 * @ingroup color-private
 */
#define TPBRT_SPECTRUM_FILE_NAME_INIT                       \
	{                                                       \
		.as.file_name = { .data = TPBRT_NULL, .size = 0u }, \
		.type		  = TPBRT_SPECTRUM_TYPE_FILE            \
	}
/**
 * @brief Initializes spectrum with empty file name to wavelengths file with type conversion.
 * @ingroup color-private
 */
#define TPBRT_SPECTRUM_FILE_NAME (tpbrt_spectrum_t) TPBRT_SPECTRUM_FILE_NAME_INIT
/**
 * @brief Initializes spectrum with builtin value.
 * @param builtin_value Builtin value.
 * @ingroup color-private
 */
#define TPBRT_SPECTRUM_BUILTIN_INIT(builtin_value) { .as.builtin = (builtin_value), .type = TPBRT_SPECTRUM_TYPE_BUILTIN }
/**
 * @brief Initializes spectrum with builtin value with type conversion.
 * @param builtin_value Builtin value.
 * @ingroup color-private
 */
#define TPBRT_SPECTRUM_BUILTIN(builtin_value) (tpbrt_spectrum_t) TPBRT_SPECTRUM_BUILTIN_INIT(builtin_value)
/**
 * @brief Initializes spectrum with constant value.
 * @param value Constant value.
 * @ingroup color-private
 */
#define TPBRT_SPECTRUM_CONSTANT_INIT(value) { .as.constant = (value), .type = TPBRT_SPECTRUM_TYPE_CONSTANT }
/**
 * @brief Initializes spectrum with constant value with type conversion.
 * @param value Constant value.
 * @ingroup color-private
 */
#define TPBRT_SPECTRUM_CONSTANT(value) (tpbrt_spectrum_t) TPBRT_SPECTRUM_CONSTANT_INIT(value)

	/**
	 * @brief Returns color space based on string value.
	 * @param color_space_str String which is convertible to color space.
	 * @param color_space_out Output color space value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_COLOR_SPACE, TPBRT_ERROR_NONE).
	 * @ingroup color-private
	 */
	tpbrt_error_t tpbrt_color_space_from_string(tpbrt_string_t color_space_str, tpbrt_color_space_t* color_space_out);
	/**
	 * @brief Returns spectrum builtin based on string value.
	 * @param builtin_str String which is convertible to spectrum builtin.
	 * @param builtin_out Output spectrum builtin value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_SPECTRUM_BUILTIN, TPBRT_ERROR_NONE).
	 * @ingroup color-private
	 */
	tpbrt_error_t tpbrt_spectrum_builtin_from_string(tpbrt_string_t builtin_str, tpbrt_spectrum_builtin_t* builtin_out);

	/**
	 * @brief Frees rgb arrays.
	 * @param array Array to free.
	 * @ingroup color-private
	 */
	void tpbrt_rgb_array_free(tpbrt_rgb_array_t* array);

	/**
	 * @brief Copies wavelengths array.
	 * @param dst Destination array.
	 * @param src Source array.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup color-private
	 */
	tpbrt_error_t tpbrt_wavelengths_array_copy(tpbrt_wavelength_array_t* dst, tpbrt_wavelength_array_t src);
	/**
	 * @brief Frees wavelengths arrays.
	 * @param wavelength_array Array to free.
	 * @ingroup color-private
	 */
	void tpbrt_wavelength_array_free(tpbrt_wavelength_array_t* wavelength_array);

	/**
	 * @brief Copies spectrum.
	 * @param dst Destination array.
	 * @param src Source array.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup color-private
	 */
	tpbrt_error_t tpbrt_spectrum_copy(tpbrt_spectrum_t* dst, const tpbrt_spectrum_t* src);
	/**
	 * @brief Frees spectrum.
	 * @param spectrum Spectrum to free.
	 * @ingroup color-private
	 */
	void tpbrt_spectrum_free(tpbrt_spectrum_t* spectrum);

#ifdef __cplusplus
}
#endif
#endif // TINYPBRT_COLOR_INTERNAL_H
