#pragma once
#ifndef _TINYPBRT_COLOR_H_
#define _TINYPBRT_COLOR_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/math.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup color-public Color
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Color space types.
	 * @ingroup color-public
	 */
	typedef enum tpbrt_color_space_t {
		/// @brief The standard color space defined in the Academy Color Encoding System.
		TPBRT_COLOR_SPACE_ACES_2065_1 = 0,
		/// @brief The ITU-R Recommendation BT.2020 color space.
		TPBRT_COLOR_SPACE_REC_2020	  = 1,
		/// @brief The DCI-P3 color space, widely used in current displays.
		TPBRT_COLOR_SPACE_DCI_P3	  = 2,
		/// @brief The venerable sRGB color space; it has the smallest gamut of pbrt-v4's color spaces, but is still widely used.
		TPBRT_COLOR_SPACE_SRGB		  = 3,
		/// @brief Count of available color spaces.
		TPBRT_COLOR_SPACE_MAX_NUM	  = 4,
	} tpbrt_color_space_t;

	/**
	 * @brief Spectrum value type.
	 * @ingroup color-public
	 */
	typedef enum tpbrt_spectrum_type_t {
		/// @brief Spectrum defined as list of pairs (wavelength, value).
		TPBRT_SPECTRUM_TYPE_WAVELENGTHS = 0,
		/// @brief Spectrum defined as RGB value.
		TPBRT_SPECTRUM_TYPE_RGB			= 1,
		/// @brief Spectrum defined as Blackbody temperature in Kelvins.
		TPBRT_SPECTRUM_TYPE_BLACKBODY	= 2,
		/// @brief Spectrum defined as file which contains wavelengths.
		TPBRT_SPECTRUM_TYPE_FILE		= 3,
		/// @brief Spectrum defined as builtin value.
		TPBRT_SPECTRUM_TYPE_BUILTIN		= 4,
		/// @brief Spectrum defined as single constant float value.
		TPBRT_SPECTRUM_TYPE_CONSTANT	= 5,
		/// @brief Count of available spectrum value types.
		TPBRT_SPECTRUM_TYPE_MAX_NUM		= 6,
	} tpbrt_spectrum_type_t;

	/**
	 * @brief Representation of wavelength pair.
	 * @ingroup color-public
	 */
	typedef struct tpbrt_wavelength_t {
		tpbrt_float_t wavelength;
		tpbrt_float_t value;
	} tpbrt_wavelength_t;

	/**
	 * @brief Array of wavelengths pairs.
	 * @ingroup color-public
	 */
	typedef struct tpbrt_wavelength_array_t {
		/// @brief Array data.
		tpbrt_wavelength_t* data;
		/// @brief Array items count.
		tpbrt_size_t count;
	} tpbrt_wavelength_array_t;

	/**
	 * @brief Spectrum builtins.
	 * @ingroup color-public
	 */
	typedef enum tpbrt_spectrum_builtin_t {
		/// @brief Index of refraction for BK7 glass
		TPBRT_SPECTRUM_BUILTIN_GLASS_BK7	   = 0,
		/// @brief Index of refraction for BAF10 glass
		TPBRT_SPECTRUM_BUILTIN_GLASS_BAF10	   = 1,
		/// @brief Index of refraction for FK51A glass
		TPBRT_SPECTRUM_BUILTIN_GLASS_FK51A	   = 2,
		/// @brief Index of refraction for LASF9 glass
		TPBRT_SPECTRUM_BUILTIN_GLASS_LASF9	   = 3,
		/// @brief Index of refraction for F5 glass
		TPBRT_SPECTRUM_BUILTIN_GLASS_F5		   = 4,
		/// @brief Index of refraction for F10 glass
		TPBRT_SPECTRUM_BUILTIN_GLASS_F10	   = 5,
		/// @brief Index of refraction for F11 glass
		TPBRT_SPECTRUM_BUILTIN_GLASS_F11	   = 6,
		/// @brief Index of refraction for silver.
		TPBRT_SPECTRUM_BUILTIN_METAL_AG_ETA	   = 7,
		/// @brief Extinction coefficient for silver.
		TPBRT_SPECTRUM_BUILTIN_METAL_AG_K	   = 8,
		/// @brief Index of refraction for aluminum.
		TPBRT_SPECTRUM_BUILTIN_METAL_AL_ETA	   = 9,
		/// @brief Extinction coefficient for aluminum.
		TPBRT_SPECTRUM_BUILTIN_METAL_AL_K	   = 10,
		/// @brief Index of refraction for gold.
		TPBRT_SPECTRUM_BUILTIN_METAL_AU_ETA	   = 11,
		/// @brief Extinction coefficient for gold.
		TPBRT_SPECTRUM_BUILTIN_METAL_AU_K	   = 12,
		/// @brief Index of refraction for copper.
		TPBRT_SPECTRUM_BUILTIN_METAL_CU_ETA	   = 13,
		/// @brief Extinction coefficient for copper.
		TPBRT_SPECTRUM_BUILTIN_METAL_CU_K	   = 14,
		/// @brief Index of refraction for copper zinc alloy.
		TPBRT_SPECTRUM_BUILTIN_METAL_CU_ZN_ETA = 15,
		/// @brief Extinction coefficient for copper zinc alloy.
		TPBRT_SPECTRUM_BUILTIN_METAL_CU_ZN_K   = 16,
		/// @brief Index of refraction for magnesium oxide.
		TPBRT_SPECTRUM_BUILTIN_METAL_MG_O_ETA  = 17,
		/// @brief Extinction coefficient for magnesium oxide.
		TPBRT_SPECTRUM_BUILTIN_METAL_MG_O_K	   = 18,
		/// @brief Index of refraction for titanium dioxide.
		TPBRT_SPECTRUM_BUILTIN_METAL_TI_O2_ETA = 19,
		/// @brief Extinction coefficient for titanium dioxide.
		TPBRT_SPECTRUM_BUILTIN_METAL_TI_O2_K   = 20,
		/// @brief CIE standard illuminant A.
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_A	   = 21,
		/// @brief CIE standard illuminant D50.
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D50   = 22,
		/// @brief CIE standard illuminant D65.
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D65   = 23,
		/// @brief CIE standard illuminant F1.
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F1	   = 24,
		/// @brief CIE standard illuminant F2.
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F2	   = 25,
		/// @brief CIE standard illuminant F3.
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F3	   = 26,
		/// @brief CIE standard illuminant F4.
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F4	   = 27,
		/// @brief CIE standard illuminant F5.
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F5	   = 28,
		/// @brief CIE standard illuminant F6.
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F6	   = 29,
		/// @brief CIE standard illuminant F7.
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F7	   = 30,
		/// @brief CIE standard illuminant F8.
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F8	   = 31,
		/// @brief CIE standard illuminant F9.
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F9	   = 32,
		/// @brief CIE standard illuminant F10.
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F10   = 33,
		/// @brief CIE standard illuminant F11.
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F11   = 34,
		/// @brief CIE standard illuminant F12.
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F12   = 35,
		/// @brief D60 illuminant from ACES.
		TPBRT_SPECTRUM_BUILTIN_ILLUM_ACES_D60  = 36,
		/// @brief Count of available spectrum builtins.
		TPBRT_SPECTRUM_BUILTIN_MAX_NUM		   = 37,
	} tpbrt_spectrum_builtin_t;

	/**
	 * @brief PBRT Spectrum representation.
	 * @ingroup color-public
	 */
	typedef struct tpbrt_spectrum_t {
		/// @brief Value dependent on type value.
		union {
			/// @brief Array of pairs (wavelength, value).
			tpbrt_wavelength_array_t wavelengths;
			/// @brief RGB value.
			tpbrt_rgb_t rgb;
			/// @brief Blackbody temperature in Kelvins.
			tpbrt_blackbody_t blackbody;
			/// @brief Filename of file which contains wavelengths array.
			tpbrt_string_t file_name;
			/// @brief Builtin value.
			tpbrt_spectrum_builtin_t builtin : 7;
			/// @brief Constant value.
			tpbrt_float_t constant;
		} as;

		/// @brief value type.
		tpbrt_spectrum_type_t type : 4;
	} tpbrt_spectrum_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_COLOR_H_
