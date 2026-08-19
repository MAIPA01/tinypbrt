#include <tinypbrt/pch.h>

#include <tinypbrt/detail/film_internal.h>

#include <tinypbrt/detail/coord_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	static const tpbrt_coordinate_system_t TPBRT_FILM_GBUFFER_COORD_SYS_DEFAULT = TPBRT_COORDINATE_SYSTEM_CAMERA;
	static const tpbrt_uint_t TPBRT_FILM_SPECTRAL_N_BUCKETS_DEFAULT				= 16u;
	static const tpbrt_float_t TPBRT_FILM_SPECTRAL_LAMBDA_MIN_DEFAULT			= 360.0f;
	static const tpbrt_float_t TPBRT_FILM_SPECTRAL_LAMBDA_MAX_DEFAULT			= 830.0f;
	static const tpbrt_uint_t TPBRT_FILM_X_RES_DEFAULT							= 1280;
	static const tpbrt_uint_t TPBRT_FILM_Y_RES_DEFAULT							= 720;
	static const tpbrt_float_t TPBRT_FILM_DIAGONAL_DEFAULT						= 35.0f;
	static const tpbrt_string_t TPBRT_FILM_FILE_NAME_DEFAULT					= { .chars = "pbrt.exr", .size = 8 };
	static const tpbrt_bool_t TPBRT_FILM_SAVE_FP16_DEFAULT						= TPBRT_TRUE;
	static const tpbrt_float_t TPBRT_FILM_ISO_DEFAULT							= 100.0f;
	static const tpbrt_float_t TPBRT_FILM_WHITE_BALANCE_DEFAULT					= 0.0f;
	static const tpbrt_film_sensor_t TPBRT_FILM_SENSOR_DEFAULT					= TPBRT_FILM_SENSOR_CIE_1931;
	static const tpbrt_float_t TPBRT_FILM_MAX_COMPONENT_VALUE_DEFAULT			= INFINITY;

	static const tpbrt_string_t TPBRT_FILM_GBUFFER_COORD_SYS_STR				= { .chars = "coordinatesystem", .size = 16 };
	static const tpbrt_string_t TPBRT_FILM_SPECTRAL_N_BUCKETS_STR				= { .chars = "nbuckets", .size = 8 };
	static const tpbrt_string_t TPBRT_FILM_SPECTRAL_LAMBDA_MIN_STR				= { .chars = "lambdamin", .size = 9 };
	static const tpbrt_string_t TPBRT_FILM_SPECTRAL_LAMBDA_MAX_STR				= { .chars = "lambdamax", .size = 9 };
	static const tpbrt_string_t TPBRT_FILM_X_RESOULUTION_STR					= { .chars = "xresolution", .size = 11 };
	static const tpbrt_string_t TPBRT_FILM_Y_RESOULUTION_STR					= { .chars = "yresolution", .size = 11 };
	static const tpbrt_string_t TPBRT_FILM_CROP_WINDOW_STR						= { .chars = "cropwindow", .size = 10 };
	static const tpbrt_string_t TPBRT_FILM_PIXEL_BOUNDS_STR						= { .chars = "pixelbounds", .size = 11 };
	static const tpbrt_string_t TPBRT_FILM_DIAGONAL_STR							= { .chars = "diagonal", .size = 8 };
	static const tpbrt_string_t TPBRT_FILM_FILE_NAME_STR						= { .chars = "filename", .size = 8 };
	static const tpbrt_string_t TPBRT_FILM_SAVE_FP16_STR						= { .chars = "savefp16", .size = 8 };
	static const tpbrt_string_t TPBRT_FILM_ISO_STR								= { .chars = "iso", .size = 3 };
	static const tpbrt_string_t TPBRT_FILM_WHITE_BALANCE_STR					= { .chars = "whitebalance", .size = 12 };
	static const tpbrt_string_t TPBRT_FILM_SENSOR_STR							= { .chars = "sensor", .size = 6 };
	static const tpbrt_string_t TPBRT_FILM_MAX_COMPONENT_VALUE_STR				= { .chars = "maxcomponentvalue", .size = 17 };

	tpbrt_error_t tpbrt_create_default_film(tpbrt_film_t** film) {
			if (film == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*film = malloc(sizeof(tpbrt_film_t));
			if (*film == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		(*film)->type				 = TPBRT_FILM_TYPE_RGB;
		(*film)->x_resolution		 = TPBRT_FILM_X_RES_DEFAULT;
		(*film)->y_resolution		 = TPBRT_FILM_Y_RES_DEFAULT;
		(*film)->crop_window[0]		 = 0.0f;
		(*film)->crop_window[1]		 = 1.0f;
		(*film)->crop_window[2]		 = 0.0f;
		(*film)->crop_window[3]		 = 1.0f;
		(*film)->pixel_bounds[0]	 = 0;
		(*film)->pixel_bounds[1]	 = (*film)->x_resolution;
		(*film)->pixel_bounds[2]	 = 0;
		(*film)->pixel_bounds[3]	 = (*film)->y_resolution;
		(*film)->diagonal			 = TPBRT_FILM_DIAGONAL_DEFAULT;
		(*film)->file_name.chars	 = TPBRT_FILM_FILE_NAME_DEFAULT.chars;
		(*film)->file_name.size		 = TPBRT_FILM_FILE_NAME_DEFAULT.size;
		(*film)->save_fp16			 = TPBRT_FILM_SAVE_FP16_DEFAULT;
		(*film)->iso				 = TPBRT_FILM_ISO_DEFAULT;
		(*film)->white_balance		 = TPBRT_FILM_WHITE_BALANCE_DEFAULT;
		(*film)->sensor				 = TPBRT_FILM_SENSOR_CIE_1931;
		(*film)->max_component_value = TPBRT_FILM_MAX_COMPONENT_VALUE_DEFAULT;
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_film_type_from_string(const tpbrt_string_t* const type_str, tpbrt_film_type_t* const type) {
		static const tpbrt_string_t RGB_STR		 = { .chars = "rgb", .size = 3 };
		static const tpbrt_string_t GBUFFER_STR	 = { .chars = "gbuffer", .size = 7 };
		static const tpbrt_string_t SPECTRAL_STR = { .chars = "spectral", .size = 8 };

			if (type_str == TPBRT_NULL || type_str->chars == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (type_str->size == RGB_STR.size && strncmp(type_str->chars, RGB_STR.chars, RGB_STR.size) == 0) {
				*type = TPBRT_FILM_TYPE_RGB;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == GBUFFER_STR.size && strncmp(type_str->chars, GBUFFER_STR.chars, GBUFFER_STR.size) == 0) {
				*type = TPBRT_FILM_TYPE_GBUFFER;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == SPECTRAL_STR.size && strncmp(type_str->chars, SPECTRAL_STR.chars, SPECTRAL_STR.size) == 0) {
				*type = TPBRT_FILM_TYPE_SPECTRAL;
				return TPBRT_ERROR_NONE;
			}

		return TPBRT_ERROR_INVALID_FILM_TYPE;
	}

	static tpbrt_error_t tpbrt_film_sensor_from_string(const tpbrt_string_t* const type_str, tpbrt_film_sensor_t* const sensor) {
		static const tpbrt_string_t CIE_1931_STR			= { .chars = "cie1931", .size = 7 };
		static const tpbrt_string_t CANON_EOS_100D_STR		= { .chars = "canon_eos_100d", .size = 14 };
		static const tpbrt_string_t CANON_EOS_1DX_MKII_STR	= { .chars = "canon_eos_1dx_mkii", .size = 18 };
		static const tpbrt_string_t CANON_EOS_200D_STR		= { .chars = "canon_eos_200d", .size = 14 };
		static const tpbrt_string_t CANON_EOS_200D_MKII_STR = { .chars = "canon_eos_200d_mkii", .size = 19 };
		static const tpbrt_string_t CANON_EOS_5D_STR		= { .chars = "canon_eos_5d", .size = 12 };
		static const tpbrt_string_t CANON_EOS_5D_MKII_STR	= { .chars = "canon_eos_5d_mkii", .size = 17 };
		static const tpbrt_string_t CANON_EOS_5D_MKIII_STR	= { .chars = "canon_eos_5d_mkiii", .size = 18 };
		static const tpbrt_string_t CANON_EOS_5D_MKIV_STR	= { .chars = "canon_eos_5d_mkiv", .size = 17 };
		static const tpbrt_string_t CANON_EOS_5DS_STR		= { .chars = "canon_eos_5ds", .size = 13 };
		static const tpbrt_string_t CANON_EOS_M_STR			= { .chars = "canon_eos_m", .size = 11 };
		static const tpbrt_string_t HASSELBLAD_L1D_20C_STR	= { .chars = "hasselblad_l1d_20c", .size = 18 };
		static const tpbrt_string_t NIKON_D810_STR			= { .chars = "nikon_d810", .size = 10 };
		static const tpbrt_string_t NIKON_D850_STR			= { .chars = "nikon_d850", .size = 10 };
		static const tpbrt_string_t SONY_ILCE_6400_STR		= { .chars = "sony_ilce_6400", .size = 14 };
		static const tpbrt_string_t SONY_ILCE_7M3_STR		= { .chars = "sony_ilce_7m3", .size = 13 };
		static const tpbrt_string_t SONY_ILCE_7RM3_STR		= { .chars = "sony_ilce_7rm3", .size = 14 };
		static const tpbrt_string_t SONY_ILCE_9_STR			= { .chars = "sony_ilce_9", .size = 11 };

			if (type_str->size == CIE_1931_STR.size && strncmp(type_str->chars, CIE_1931_STR.chars, CIE_1931_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_CIE_1931;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == CANON_EOS_100D_STR.size &&
				strncmp(type_str->chars, CANON_EOS_100D_STR.chars, CANON_EOS_100D_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_CANON_EOS_100D;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == CANON_EOS_1DX_MKII_STR.size &&
				strncmp(type_str->chars, CANON_EOS_1DX_MKII_STR.chars, CANON_EOS_1DX_MKII_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_CANON_EOS_1DX_MKII;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == CANON_EOS_200D_STR.size &&
				strncmp(type_str->chars, CANON_EOS_200D_STR.chars, CANON_EOS_200D_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_CANON_EOS_200D;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == CANON_EOS_200D_MKII_STR.size &&
				strncmp(type_str->chars, CANON_EOS_200D_MKII_STR.chars, CANON_EOS_200D_MKII_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_CANON_EOS_200D_MKII;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == CANON_EOS_5D_STR.size &&
				strncmp(type_str->chars, CANON_EOS_5D_STR.chars, CANON_EOS_5D_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_CANON_EOS_5D;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == CANON_EOS_5D_MKII_STR.size &&
				strncmp(type_str->chars, CANON_EOS_5D_MKII_STR.chars, CANON_EOS_5D_MKII_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_CANON_EOS_5D_MKII;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == CANON_EOS_5D_MKIII_STR.size &&
				strncmp(type_str->chars, CANON_EOS_5D_MKIII_STR.chars, CANON_EOS_5D_MKIII_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_CANON_EOS_5D_MKIII;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == CANON_EOS_5D_MKIV_STR.size &&
				strncmp(type_str->chars, CANON_EOS_5D_MKIV_STR.chars, CANON_EOS_5D_MKIV_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_CANON_EOS_5D_MKIV;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == CANON_EOS_5DS_STR.size &&
				strncmp(type_str->chars, CANON_EOS_5DS_STR.chars, CANON_EOS_5DS_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_CANON_EOS_5DS;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == CANON_EOS_M_STR.size &&
				strncmp(type_str->chars, CANON_EOS_M_STR.chars, CANON_EOS_M_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_CANON_EOS_M;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == HASSELBLAD_L1D_20C_STR.size &&
				strncmp(type_str->chars, HASSELBLAD_L1D_20C_STR.chars, HASSELBLAD_L1D_20C_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_HASSELBLAD_L1D_20C;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == NIKON_D810_STR.size &&
				strncmp(type_str->chars, NIKON_D810_STR.chars, NIKON_D810_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_NIKON_D810;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == NIKON_D850_STR.size &&
				strncmp(type_str->chars, NIKON_D850_STR.chars, NIKON_D850_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_NIKON_D850;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == SONY_ILCE_6400_STR.size &&
				strncmp(type_str->chars, SONY_ILCE_6400_STR.chars, SONY_ILCE_6400_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_SONY_ILCE_6400;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == SONY_ILCE_7M3_STR.size &&
				strncmp(type_str->chars, SONY_ILCE_7M3_STR.chars, SONY_ILCE_7M3_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_SONY_ILCE_7M3;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == SONY_ILCE_7RM3_STR.size &&
				strncmp(type_str->chars, SONY_ILCE_7RM3_STR.chars, SONY_ILCE_7RM3_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_SONY_ILCE_7RM3;
				return TPBRT_ERROR_NONE;
			}

			if (type_str->size == SONY_ILCE_9_STR.size &&
				strncmp(type_str->chars, SONY_ILCE_9_STR.chars, SONY_ILCE_9_STR.size) == 0) {
				*sensor = TPBRT_FILM_SENSOR_SONY_ILCE_9;
				return TPBRT_ERROR_NONE;
			}

		return TPBRT_ERROR_UNKNOWN_FILM_SENSOR;
	}

	tpbrt_error_t tpbrt_create_film(const tpbrt_string_t* const type_str, const tpbrt_params_list_t* const params,
	  tpbrt_film_t** const film) {
			if (params == TPBRT_NULL || film == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_error_t err;
		tpbrt_film_type_t type = TPBRT_FILM_TYPE_RGB;
			if (type_str != TPBRT_NULL && type_str->chars != TPBRT_NULL) {
				err = tpbrt_film_type_from_string(type_str, &type);
					if (err != TPBRT_ERROR_NONE) { return err; }
			}

		*film = malloc(sizeof(tpbrt_film_t));
			if (*film == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

			switch (type) {
			default:
				case TPBRT_FILM_TYPE_RGB: {
					break;
				}
				case TPBRT_FILM_TYPE_GBUFFER: {
					tpbrt_string_t coordinate_system_str;
					err = tpbrt_params_list_get_string(params, &TPBRT_FILM_GBUFFER_COORD_SYS_STR, &coordinate_system_str);
						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*film)->gbuffer_params.coordinate_system = TPBRT_FILM_GBUFFER_COORD_SYS_DEFAULT;
							break;
						}

						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_film(film);
							return err;
						}

					err = tpbrt_coordinate_system_from_string(&coordinate_system_str, &(*film)->gbuffer_params.coordinate_system);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_film(film);
							return err;
						}
					break;
				}
				case TPBRT_FILM_TYPE_SPECTRAL: {
					err = tpbrt_params_list_get_uint(params, &TPBRT_FILM_SPECTRAL_N_BUCKETS_STR,
					  TPBRT_FILM_SPECTRAL_N_BUCKETS_DEFAULT, &(*film)->spectral_params.nbuckets);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_film(film);
							return err;
						}


					err = tpbrt_params_list_get_float(params, &TPBRT_FILM_SPECTRAL_LAMBDA_MIN_STR,
					  TPBRT_FILM_SPECTRAL_LAMBDA_MIN_DEFAULT, &(*film)->spectral_params.lambda_min);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_film(film);
							return err;
						}


					err = tpbrt_params_list_get_float(params, &TPBRT_FILM_SPECTRAL_LAMBDA_MAX_STR,
					  TPBRT_FILM_SPECTRAL_LAMBDA_MAX_DEFAULT, &(*film)->spectral_params.lambda_max);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_film(film);
							return err;
						}

					break;
				}
			}

		err = tpbrt_params_list_get_uint(params, &TPBRT_FILM_X_RESOULUTION_STR, TPBRT_FILM_X_RES_DEFAULT, &(*film)->x_resolution);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_film(film);
				return err;
			}

		err = tpbrt_params_list_get_uint(params, &TPBRT_FILM_Y_RESOULUTION_STR, TPBRT_FILM_Y_RES_DEFAULT, &(*film)->y_resolution);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_film(film);
				return err;
			}

		tpbrt_float_array_t crop_window;
		err = tpbrt_params_list_get_floats(params, &TPBRT_FILM_CROP_WINDOW_STR, &crop_window);
			if (err != TPBRT_ERROR_NOT_FOUND) {
					if (err != TPBRT_ERROR_NONE) {
							if (crop_window.values != TPBRT_NULL) { free(crop_window.values); }
						tpbrt_free_film(film);
						return err;
					}

					if (crop_window.count < 4) {
							if (crop_window.values != TPBRT_NULL) { free(crop_window.values); }
						tpbrt_free_film(film);
						return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
					}

					if (crop_window.count > 4) {
						free(crop_window.values);
						tpbrt_free_film(film);
						return TPBRT_ERROR_TOO_MANY_VALUES;
					}

				(*film)->crop_window[0] = crop_window.values[0];
				(*film)->crop_window[1] = crop_window.values[1];
				(*film)->crop_window[2] = crop_window.values[2];
				(*film)->crop_window[3] = crop_window.values[3];
				free(crop_window.values);
			}
			else {
				(*film)->crop_window[0] = 0.0f;
				(*film)->crop_window[1] = 1.0f;
				(*film)->crop_window[2] = 0.0f;
				(*film)->crop_window[3] = 1.0f;
			}

		tpbrt_uint_array_t pixel_bounds;
		err = tpbrt_params_list_get_uints(params, &TPBRT_FILM_PIXEL_BOUNDS_STR, &pixel_bounds);
			if (err != TPBRT_ERROR_NOT_FOUND) {
					if (err != TPBRT_ERROR_NONE) {
							if (pixel_bounds.values != TPBRT_NULL) { free(pixel_bounds.values); }
						tpbrt_free_film(film);
						return err;
					}

					if (pixel_bounds.count < 4) {
							if (pixel_bounds.values != TPBRT_NULL) { free(pixel_bounds.values); }
						tpbrt_free_film(film);
						return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
					}

					if (pixel_bounds.count > 4) {
						free(pixel_bounds.values);
						tpbrt_free_film(film);
						return TPBRT_ERROR_TOO_MANY_VALUES;
					}

				(*film)->pixel_bounds[0] = pixel_bounds.values[0];
				(*film)->pixel_bounds[1] = pixel_bounds.values[1];
				(*film)->pixel_bounds[2] = pixel_bounds.values[2];
				(*film)->pixel_bounds[3] = pixel_bounds.values[3];
				free(pixel_bounds.values);
			}
			else {
				(*film)->pixel_bounds[0] = 0.0f;
				(*film)->pixel_bounds[1] = (*film)->x_resolution;
				(*film)->pixel_bounds[2] = 0.0f;
				(*film)->pixel_bounds[3] = (*film)->y_resolution;
			}

		err = tpbrt_params_list_get_float(params, &TPBRT_FILM_DIAGONAL_STR, TPBRT_FILM_DIAGONAL_DEFAULT, &(*film)->diagonal);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_film(film);
				return err;
			}

		err = tpbrt_params_list_get_string(params, &TPBRT_FILM_FILE_NAME_STR, &(*film)->file_name);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
				tpbrt_free_film(film);
				return err;
			}
			if (err == TPBRT_ERROR_NOT_FOUND) { (*film)->file_name = TPBRT_FILM_FILE_NAME_DEFAULT; }

		err = tpbrt_params_list_get_bool(params, &TPBRT_FILM_SAVE_FP16_STR, TPBRT_FILM_SAVE_FP16_DEFAULT, &(*film)->save_fp16);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_film(film);
				return err;
			}

		err = tpbrt_params_list_get_float(params, &TPBRT_FILM_ISO_STR, TPBRT_FILM_ISO_DEFAULT, &(*film)->iso);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_film(film);
				return err;
			}

		err = tpbrt_params_list_get_float(params, &TPBRT_FILM_WHITE_BALANCE_STR, TPBRT_FILM_WHITE_BALANCE_DEFAULT,
		  &(*film)->white_balance);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_film(film);
				return err;
			}

		tpbrt_string_t sensor_str;
		err = tpbrt_params_list_get_string(params, &TPBRT_FILM_SENSOR_STR, &sensor_str);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
				tpbrt_free_film(film);
				return err;
			}
			if (err == TPBRT_ERROR_NOT_FOUND) { (*film)->sensor = TPBRT_FILM_SENSOR_DEFAULT; }
			else {
				err = tpbrt_film_sensor_from_string(&sensor_str, &(*film)->sensor);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_free_film(film);
						return err;
					}
			}

		err = tpbrt_params_list_get_float(params, &TPBRT_FILM_MAX_COMPONENT_VALUE_STR, TPBRT_FILM_MAX_COMPONENT_VALUE_DEFAULT,
		  &(*film)->max_component_value);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_film(film);
				return err;
			}
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_film(tpbrt_film_t** film) {
			if (film == TPBRT_NULL || *film == TPBRT_NULL) { return; }

			if ((*film)->file_name.chars != TPBRT_FILM_FILE_NAME_DEFAULT.chars && (*film)->file_name.chars != TPBRT_NULL) {
				free((*film)->file_name.chars);
			}

		free(*film);
		*film = TPBRT_NULL;
	}

#ifdef __cplusplus
}
#endif