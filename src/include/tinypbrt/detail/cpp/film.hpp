#pragma once
#ifndef _TINYPBRT_FILM_H_
#define _TINYPBRT_FILM_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/coord.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup film-public Film
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Film value type.
	 * @ingroup film-public
	 */
	typedef enum tpbrt_film_type_t {
		/// @brief Stores RGB images using the current color space when the Film directive is encountered.
		TPBRT_FILM_TYPE_RGB		 = 0,
		/// @brief In addition to RGB, stores multiple additional channels that encode information about the visible geometry in each pixel.
		TPBRT_FILM_TYPE_GBUFFER	 = 1,
		/// @brief  stores a discretized spectral distribution at each pixel, in addition to RGB (for convenience when viewing images.).
		TPBRT_FILM_TYPE_SPECTRAL = 2,
		/// @brief Count of available film types.
		TPBRT_FILM_TYPE_MAX_NUM	 = 3,
	} tpbrt_film_type_t;

	/**
	 * @brief GBuffer film type parameters.
	 * @ingroup film-public
	 */
	typedef struct tpbrt_film_gbuffer_params_t {
		/// @brief Tells in which coordinate system geometric data is stored.
		tpbrt_coord_system_type_t coordinate_system : 3;
	} tpbrt_film_gbuffer_params_t;

	/**
	 * @brief Spectral film type parameters.
	 * @ingroup film-public
	 */
	typedef struct tpbrt_film_spectral_params_t {
		/// @brief Number of buckets that the spectral range is discretized into.
		tpbrt_uint_t nbuckets;
		/// @brief Minimum wavelength for spectral range.
		tpbrt_float_t lambda_min;
		/// @brief Maximum wavelength for spectral range.
		tpbrt_float_t lambda_max;
	} tpbrt_film_spectral_params_t;

	/**
	 * @brief Film sensor type.
	 * @ingroup film-public
	 */
	typedef enum tpbrt_film_sensor_t {
		/// @brief The CIE 1931 spectral response curves.
		TPBRT_FILM_SENSOR_CIE_1931			  = 0,
		/// @brief Canon EOS 100D
		TPBRT_FILM_SENSOR_CANON_EOS_100D	  = 1,
		/// @brief Canon EOS 1DX MK II
		TPBRT_FILM_SENSOR_CANON_EOS_1DX_MKII  = 2,
		/// @brief Canon EOS 200D
		TPBRT_FILM_SENSOR_CANON_EOS_200D	  = 3,
		/// @brief Canon EOS 200D MK II
		TPBRT_FILM_SENSOR_CANON_EOS_200D_MKII = 4,
		/// @brief Canon EOS 5D
		TPBRT_FILM_SENSOR_CANON_EOS_5D		  = 5,
		/// @brief Canon EOS 5D MK II
		TPBRT_FILM_SENSOR_CANON_EOS_5D_MKII	  = 6,
		/// @brief Canon EOS 5D MK III
		TPBRT_FILM_SENSOR_CANON_EOS_5D_MKIII  = 7,
		/// @brief Canon EOS 5D MK IV
		TPBRT_FILM_SENSOR_CANON_EOS_5D_MKIV	  = 8,
		/// @brief Canon EOS 5DS
		TPBRT_FILM_SENSOR_CANON_EOS_5DS		  = 9,
		/// @brief Canon EOS M
		TPBRT_FILM_SENSOR_CANON_EOS_M		  = 10,
		/// @brief Hasselblad L1D 20C
		TPBRT_FILM_SENSOR_HASSELBLAD_L1D_20C  = 11,
		/// @brief Nikon D810
		TPBRT_FILM_SENSOR_NIKON_D810		  = 12,
		/// @brief Nikon D850
		TPBRT_FILM_SENSOR_NIKON_D850		  = 13,
		/// @brief Sony ILCE 6400
		TPBRT_FILM_SENSOR_SONY_ILCE_6400	  = 14,
		/// @brief Sony ILCE 7M3
		TPBRT_FILM_SENSOR_SONY_ILCE_7M3		  = 15,
		/// @brief Sony ILCE 7RM3
		TPBRT_FILM_SENSOR_SONY_ILCE_7RM3	  = 16,
		/// @brief Sony ILCE 9
		TPBRT_FILM_SENSOR_SONY_ILCE_9		  = 17,
		/// @brief Count of available film sensor types.
		TPBRT_FILM_SENSOR_MAX_NUM			  = 18,
	} tpbrt_film_sensor_t;

	/**
	 * @brief PBRT film representation.
	 * @ingroup film-public
	 */
	typedef struct tpbrt_film_t {
		/// @brief The sub-region of the image to render. The four values specified should be fractions in the range [0,1], and they represent x_min, x_max, y_min, and y_max, respectively. These values are in normalized device coordinates, with (0,0) in the upper-left corner of the image.
		tpbrt_float_t crop_window[4];
		/// @brief A sub-region of the image to render, specified using pixel coordinates.
		tpbrt_uint_t pixel_bounds[4];
		/// @brief The output filename.
		tpbrt_string_t file_name;
		/// @brief The number of pixels in the x direction.
		tpbrt_uint_t x_resolution;
		/// @brief The number of pixels in the y direction.
		tpbrt_uint_t y_resolution;
		/// @brief Diagonal length of the film, in mm. (This value is only used when the RealisticCamera is used.)
		tpbrt_float_t diagonal;
		/// @brief Film sensitivity to light; final pixel values are scaled by the "iso" value divided by 100.
		tpbrt_float_t iso;
		/// @brief If non-zero, this gives a temperature in degrees kelvin that is used as the reference color temperature used for whitebalancing.
		tpbrt_float_t white_balance;
		/// @brief Image sample values with luminance greater than this value are clamped to have this luminance. (This is a hack, but can be useful for eliminating large variance spikes in scenes with difficult light transport.)
		tpbrt_float_t max_component_value;

		/// @brief value dependent on type.
		union {
			/// @brief GBuffer film type parameters.
			tpbrt_film_gbuffer_params_t gbuffer;
			/// @brief Spectral film type parameters.
			tpbrt_film_spectral_params_t spectral;
		} as;

		/// @brief Film type.
		tpbrt_film_type_t type	   : 3;
		/// @brief Characterizes the sensor's response for red, green, and blue colors.
		tpbrt_film_sensor_t sensor : 6;
		/// @brief Whether 16-bit floating point values (as opposed to 32-bit floating point values) should be used when saving images in OpenEXR format.
		tpbrt_bool_t save_fp16	   : 1;
	} tpbrt_film_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_FILM_H_
