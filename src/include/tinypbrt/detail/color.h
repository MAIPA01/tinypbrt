#pragma once
#ifndef _TINYPBRT_COLOR_H_
#define _TINYPBRT_COLOR_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/math.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum tpbrt_color_space_t : uint8_t {
		TPBRT_COLOR_SPACE_ACES_2065_1 = 0,
		TPBRT_COLOR_SPACE_REC_2020	  = 1,
		TPBRT_COLOR_SPACE_DCI_P3	  = 2,
		TPBRT_COLOR_SPACE_SRGB		  = 3,
		TPBRT_COLOR_SPACE_MAX_NUM	  = 4,
	} tpbrt_color_space_t;

	typedef enum tpbrt_spectrum_type_t : uint8_t {
		TPBRT_SPECTRUM_TYPE_WAVELENGTH = 0,
		TPBRT_SPECTRUM_TYPE_RGB		   = 1,
		TPBRT_SPECTRUM_TYPE_BLACKBODY  = 2,
		TPBRT_SPECTRUM_TYPE_FILE	   = 3,
		TPBRT_SPECTRUM_TYPE_BUILTIN	   = 4,
		TPBRT_SPECTRUM_TYPE_CONSTANT   = 5,
		TPBRT_SPECTRUM_TYPE_MAX_NUM	   = 6,
	} tpbrt_spectrum_type_t;

	typedef struct tpbrt_wavelength_t {
		tpbrt_float_t wavelength;
		tpbrt_float_t value;
	} tpbrt_wavelength_t;

	typedef struct tpbrt_wavelength_array_t {
		tpbrt_wavelength_t* data;
		tpbrt_size_t count;
	} tpbrt_wavelength_array_t;

	typedef enum tpbrt_spectrum_builtin_t : uint8_t {
		TPBRT_SPECTRUM_BUILTIN_GLASS_BK7	   = 0,
		TPBRT_SPECTRUM_BUILTIN_GLASS_BAF10	   = 1,
		TPBRT_SPECTRUM_BUILTIN_GLASS_FK51A	   = 2,
		TPBRT_SPECTRUM_BUILTIN_GLASS_LASF9	   = 3,
		TPBRT_SPECTRUM_BUILTIN_GLASS_F5		   = 4,
		TPBRT_SPECTRUM_BUILTIN_GLASS_F10	   = 5,
		TPBRT_SPECTRUM_BUILTIN_GLASS_F11	   = 6,
		TPBRT_SPECTRUM_BUILTIN_METAL_AG_ETA	   = 7,
		TPBRT_SPECTRUM_BUILTIN_METAL_AG_K	   = 8,
		TPBRT_SPECTRUM_BUILTIN_METAL_AL_ETA	   = 9,
		TPBRT_SPECTRUM_BUILTIN_METAL_AL_K	   = 10,
		TPBRT_SPECTRUM_BUILTIN_METAL_AU_ETA	   = 11,
		TPBRT_SPECTRUM_BUILTIN_METAL_AU_K	   = 12,
		TPBRT_SPECTRUM_BUILTIN_METAL_CU_ETA	   = 13,
		TPBRT_SPECTRUM_BUILTIN_METAL_CU_K	   = 14,
		TPBRT_SPECTRUM_BUILTIN_METAL_CU_ZN_ETA = 15,
		TPBRT_SPECTRUM_BUILTIN_METAL_CU_ZN_K   = 16,
		TPBRT_SPECTRUM_BUILTIN_METAL_MG_O_ETA  = 17,
		TPBRT_SPECTRUM_BUILTIN_METAL_MG_O_K	   = 18,
		TPBRT_SPECTRUM_BUILTIN_METAL_TI_O2_ETA = 19,
		TPBRT_SPECTRUM_BUILTIN_METAL_TI_O2_K   = 20,
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_A	   = 21,
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D50   = 22,
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D65   = 23,
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F1	   = 24,
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F2	   = 25,
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F3	   = 26,
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F4	   = 27,
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F5	   = 28,
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F6	   = 29,
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F7	   = 30,
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F8	   = 31,
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F9	   = 32,
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F10   = 33,
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F11   = 34,
		TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F12   = 35,
		TPBRT_SPECTRUM_BUILTIN_ILLUM_ACES_D60  = 36,
		TPBRT_SPECTRUM_BUILTIN_MAX_NUM		   = 37,
	} tpbrt_spectrum_builtin_t;

	typedef struct tpbrt_spectrum_t {
		union {
			tpbrt_wavelength_array_t wavelengths;
			tpbrt_rgb_t rgb;
			tpbrt_blackbody_t blackbody;
			tpbrt_string_t file_name;
			tpbrt_spectrum_builtin_t builtin : 6;
			tpbrt_uint_t constant;
		} as;

		tpbrt_spectrum_type_t type : 3;
	} tpbrt_spectrum_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_COLOR_H_
