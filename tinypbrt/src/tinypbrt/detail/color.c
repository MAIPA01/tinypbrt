#include <tinypbrt/pch.h>

#include <tinypbrt/detail/color_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_color_space_from_string(const tpbrt_string_t* const str, tpbrt_color_space_t* const out) {
		static const tpbrt_string_t ACES2065_1_STR = { .chars = "aces2065-1", .size = 10 };
		static const tpbrt_string_t REC2020_STR	   = { .chars = "rec2020", .size = 7 };
		static const tpbrt_string_t DCI_P3_STR	   = { .chars = "dci-p3", .size = 6 };
		static const tpbrt_string_t SRGB_STR	   = { .chars = "srgb", .size = 4 };

			if (str == TPBRT_NULL || str->chars == TPBRT_NULL || out == NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (str->size == ACES2065_1_STR.size && strncmp(str->chars, ACES2065_1_STR.chars, ACES2065_1_STR.size) == 0) {
				*out = TPBRT_COLOR_SPACE_ACES_2065_1;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == REC2020_STR.size && strncmp(str->chars, REC2020_STR.chars, REC2020_STR.size) == 0) {
				*out = TPBRT_COLOR_SPACE_REC_2020;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == DCI_P3_STR.size && strncmp(str->chars, DCI_P3_STR.chars, DCI_P3_STR.size) == 0) {
				*out = TPBRT_COLOR_SPACE_DCI_P3;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == SRGB_STR.size && strncmp(str->chars, SRGB_STR.chars, SRGB_STR.size) == 0) {
				*out = TPBRT_COLOR_SPACE_SRGB;
				return TPBRT_ERROR_NONE;
			}

		return TPBRT_ERROR_UNKNOWN_COLOR_SPACE;
	}

	tpbrt_error_t tpbrt_spectrum_builtin_from_string(const tpbrt_string_t* const str, tpbrt_spectrum_builtin_t* const out) {
		static const tpbrt_string_t GLASS_BK7_STR		= { .chars = "glass-BK7", .size = 9 };
		static const tpbrt_string_t GLASS_BAF10_STR		= { .chars = "glass-BAF10", .size = 11 };
		static const tpbrt_string_t GLASS_FK51A_STR		= { .chars = "glass-FK51A", .size = 11 };
		static const tpbrt_string_t GLASS_LASF9_STR		= { .chars = "glass-LASF9", .size = 11 };
		static const tpbrt_string_t GLASS_F5_STR		= { .chars = "glass-F5", .size = 8 };
		static const tpbrt_string_t GLASS_F10_STR		= { .chars = "glass-F10", .size = 9 };
		static const tpbrt_string_t GLASS_F11_STR		= { .chars = "glass-F11", .size = 9 };
		static const tpbrt_string_t METAL_AG_ETA_STR	= { .chars = "metal-Ag-eta", .size = 12 };
		static const tpbrt_string_t METAL_AG_K_STR		= { .chars = "metal-Ag-k", .size = 10 };
		static const tpbrt_string_t METAL_AL_ETA_STR	= { .chars = "metal-Al-eta", .size = 12 };
		static const tpbrt_string_t METAL_AL_K_STR		= { .chars = "metal-Al-k", .size = 10 };
		static const tpbrt_string_t METAL_AU_ETA_STR	= { .chars = "metal-Au-eta", .size = 12 };
		static const tpbrt_string_t METAL_AU_K_STR		= { .chars = "metal-Au-k", .size = 10 };
		static const tpbrt_string_t METAL_CU_ETA_STR	= { .chars = "metal-Cu-eta", .size = 12 };
		static const tpbrt_string_t METAL_CU_K_STR		= { .chars = "metal-Cu-k", .size = 10 };
		static const tpbrt_string_t METAL_CU_ZN_ETA_STR = { .chars = "metal-CuZn-eta", .size = 14 };
		static const tpbrt_string_t METAL_CU_ZN_K_STR	= { .chars = "metal-CuZn-k", .size = 12 };
		static const tpbrt_string_t METAL_MG_O_ETA_STR	= { .chars = "metal-MgO-eta", .size = 13 };
		static const tpbrt_string_t METAL_MG_O_K_STR	= { .chars = "metal-MgO-k", .size = 11 };
		static const tpbrt_string_t METAL_TI_O2_ETA_STR = { .chars = "metal-TiO2-eta", .size = 14 };
		static const tpbrt_string_t METAL_TI_O2_K_STR	= { .chars = "metal-TI02-k", .size = 12 };
		static const tpbrt_string_t STD_ILLUM_A_STR		= { .chars = "stdillum-A", .size = 10 };
		static const tpbrt_string_t STD_ILLUM_D50_STR	= { .chars = "stdillum-D50", .size = 12 };
		static const tpbrt_string_t STD_ILLUM_D65_STR	= { .chars = "stdillum-D65", .size = 12 };
		static const tpbrt_string_t STD_ILLUM_F1_STR	= { .chars = "stdillum-F1", .size = 11 };
		static const tpbrt_string_t STD_ILLUM_F2_STR	= { .chars = "stdillum-F2", .size = 11 };
		static const tpbrt_string_t STD_ILLUM_F3_STR	= { .chars = "stdillum-F3", .size = 11 };
		static const tpbrt_string_t STD_ILLUM_F4_STR	= { .chars = "stdillum-F4", .size = 11 };
		static const tpbrt_string_t STD_ILLUM_F5_STR	= { .chars = "stdillum-F5", .size = 11 };
		static const tpbrt_string_t STD_ILLUM_F6_STR	= { .chars = "stdillum-F6", .size = 11 };
		static const tpbrt_string_t STD_ILLUM_F7_STR	= { .chars = "stdillum-F7", .size = 11 };
		static const tpbrt_string_t STD_ILLUM_F8_STR	= { .chars = "stdillum-F8", .size = 11 };
		static const tpbrt_string_t STD_ILLUM_F9_STR	= { .chars = "stdillum-F9", .size = 11 };
		static const tpbrt_string_t STD_ILLUM_F10_STR	= { .chars = "stdillum-F10", .size = 12 };
		static const tpbrt_string_t STD_ILLUM_F11_STR	= { .chars = "stdillum-F11", .size = 12 };
		static const tpbrt_string_t STD_ILLUM_F12_STR	= { .chars = "stdillum-F12", .size = 12 };
		static const tpbrt_string_t ILLUM_ACES_D60_STR	= { .chars = "illum-acesD60", .size = 13 };

			if (str == TPBRT_NULL || str->chars == TPBRT_NULL || out == NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (str->size == GLASS_BK7_STR.size && strncmp(str->chars, GLASS_BK7_STR.chars, GLASS_BK7_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_GLASS_BK7;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == GLASS_BAF10_STR.size && strncmp(str->chars, GLASS_BAF10_STR.chars, GLASS_BAF10_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_GLASS_BAF10;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == GLASS_FK51A_STR.size && strncmp(str->chars, GLASS_FK51A_STR.chars, GLASS_FK51A_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_GLASS_FK51A;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == GLASS_LASF9_STR.size && strncmp(str->chars, GLASS_LASF9_STR.chars, GLASS_LASF9_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_GLASS_LASF9;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == GLASS_F5_STR.size && strncmp(str->chars, GLASS_F5_STR.chars, GLASS_F5_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_GLASS_F5;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == GLASS_F10_STR.size && strncmp(str->chars, GLASS_F10_STR.chars, GLASS_F10_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_GLASS_F10;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == GLASS_F11_STR.size && strncmp(str->chars, GLASS_F11_STR.chars, GLASS_F11_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_GLASS_F11;
				return TPBRT_ERROR_NONE;
			}

			if ((str->size == METAL_AG_K_STR.size && strncmp(str->chars, METAL_AG_K_STR.chars, METAL_AG_K_STR.size) == 0) ||
				(str->size == METAL_AG_ETA_STR.size && strncmp(str->chars, METAL_AG_ETA_STR.chars, METAL_AG_ETA_STR.size) == 0)) {
				*out = TPBRT_SPECTRUM_METAL_AG_ETA;
				return TPBRT_ERROR_NONE;
			}

			if ((str->size == METAL_AL_K_STR.size && strncmp(str->chars, METAL_AL_K_STR.chars, METAL_AL_K_STR.size) == 0) ||
				(str->size == METAL_AL_ETA_STR.size && strncmp(str->chars, METAL_AL_ETA_STR.chars, METAL_AL_ETA_STR.size) == 0)) {
				*out = TPBRT_SPECTRUM_METAL_AL_ETA;
				return TPBRT_ERROR_NONE;
			}

			if ((str->size == METAL_AU_K_STR.size && strncmp(str->chars, METAL_AU_K_STR.chars, METAL_AU_K_STR.size) == 0) ||
				(str->size == METAL_AU_ETA_STR.size && strncmp(str->chars, METAL_AU_ETA_STR.chars, METAL_AU_ETA_STR.size) == 0)) {
				*out = TPBRT_SPECTRUM_METAL_AU_ETA;
				return TPBRT_ERROR_NONE;
			}

			if ((str->size == METAL_CU_K_STR.size && strncmp(str->chars, METAL_CU_K_STR.chars, METAL_CU_K_STR.size) == 0) ||
				(str->size == METAL_CU_ETA_STR.size && strncmp(str->chars, METAL_CU_ETA_STR.chars, METAL_CU_ETA_STR.size) == 0)) {
				*out = TPBRT_SPECTRUM_METAL_CU_ETA;
				return TPBRT_ERROR_NONE;
			}

			if ((str->size == METAL_CU_ZN_K_STR.size &&
				  strncmp(str->chars, METAL_CU_ZN_K_STR.chars, METAL_CU_ZN_K_STR.size) == 0) ||
				(str->size == METAL_CU_ZN_ETA_STR.size &&
				  strncmp(str->chars, METAL_CU_ZN_ETA_STR.chars, METAL_CU_ZN_ETA_STR.size) == 0)) {
				*out = TPBRT_SPECTRUM_METAL_CU_ZN_ETA;
				return TPBRT_ERROR_NONE;
			}

			if ((str->size == METAL_MG_O_K_STR.size && strncmp(str->chars, METAL_MG_O_K_STR.chars, METAL_MG_O_K_STR.size) == 0) ||
				(str->size == METAL_MG_O_ETA_STR.size &&
				  strncmp(str->chars, METAL_MG_O_ETA_STR.chars, METAL_MG_O_ETA_STR.size) == 0)) {
				*out = TPBRT_SPECTRUM_METAL_MG_O_ETA;
				return TPBRT_ERROR_NONE;
			}

			if ((str->size == METAL_TI_O2_K_STR.size &&
				  strncmp(str->chars, METAL_TI_O2_K_STR.chars, METAL_TI_O2_K_STR.size) == 0) ||
				(str->size == METAL_TI_O2_ETA_STR.size &&
				  strncmp(str->chars, METAL_TI_O2_ETA_STR.chars, METAL_TI_O2_ETA_STR.size) == 0)) {
				*out = TPBRT_SPECTRUM_METAL_TI_O2_ETA;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == STD_ILLUM_A_STR.size && strncmp(str->chars, STD_ILLUM_A_STR.chars, STD_ILLUM_A_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_STD_ILLUM_A;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == STD_ILLUM_D50_STR.size &&
				strncmp(str->chars, STD_ILLUM_D50_STR.chars, STD_ILLUM_D50_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_STD_ILLUM_D50;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == STD_ILLUM_D65_STR.size &&
				strncmp(str->chars, STD_ILLUM_D65_STR.chars, STD_ILLUM_D65_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_STD_ILLUM_D65;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == STD_ILLUM_F1_STR.size && strncmp(str->chars, STD_ILLUM_F1_STR.chars, STD_ILLUM_F1_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_STD_ILLUM_F1;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == STD_ILLUM_F2_STR.size && strncmp(str->chars, STD_ILLUM_F2_STR.chars, STD_ILLUM_F2_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_STD_ILLUM_F2;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == STD_ILLUM_F3_STR.size && strncmp(str->chars, STD_ILLUM_F3_STR.chars, STD_ILLUM_F3_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_STD_ILLUM_F3;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == STD_ILLUM_F4_STR.size && strncmp(str->chars, STD_ILLUM_F4_STR.chars, STD_ILLUM_F4_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_STD_ILLUM_F4;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == STD_ILLUM_F5_STR.size && strncmp(str->chars, STD_ILLUM_F5_STR.chars, STD_ILLUM_F5_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_STD_ILLUM_F5;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == STD_ILLUM_F6_STR.size && strncmp(str->chars, STD_ILLUM_F6_STR.chars, STD_ILLUM_F6_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_STD_ILLUM_F6;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == STD_ILLUM_F7_STR.size && strncmp(str->chars, STD_ILLUM_F7_STR.chars, STD_ILLUM_F7_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_STD_ILLUM_F7;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == STD_ILLUM_F8_STR.size && strncmp(str->chars, STD_ILLUM_F8_STR.chars, STD_ILLUM_F8_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_STD_ILLUM_F8;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == STD_ILLUM_F9_STR.size && strncmp(str->chars, STD_ILLUM_F9_STR.chars, STD_ILLUM_F9_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_STD_ILLUM_F9;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == STD_ILLUM_F10_STR.size &&
				strncmp(str->chars, STD_ILLUM_F10_STR.chars, STD_ILLUM_F10_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_STD_ILLUM_F10;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == STD_ILLUM_F11_STR.size &&
				strncmp(str->chars, STD_ILLUM_F11_STR.chars, STD_ILLUM_F11_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_STD_ILLUM_F11;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == STD_ILLUM_F12_STR.size &&
				strncmp(str->chars, STD_ILLUM_F12_STR.chars, STD_ILLUM_F12_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_STD_ILLUM_F12;
				return TPBRT_ERROR_NONE;
			}

			if (str->size == ILLUM_ACES_D60_STR.size &&
				strncmp(str->chars, ILLUM_ACES_D60_STR.chars, ILLUM_ACES_D60_STR.size) == 0) {
				*out = TPBRT_SPECTRUM_ILLUM_ACES_D60;
				return TPBRT_ERROR_NONE;
			}

		return TPBRT_ERROR_UNKNOWN_SPECTRUM_BUILTIN;
	}

#ifdef __cplusplus
}
#endif