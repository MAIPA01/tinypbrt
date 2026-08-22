#include <tinypbrt/pch.h>

#include <tinypbrt/detail/film_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/coord_internal.h>
#include <tinypbrt/detail/math_internal.h>

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
	static const tpbrt_string_t TPBRT_FILM_FILE_NAME_DEFAULT					= TPBRT_STRING("pbrt.exr");
	static const tpbrt_bool_t TPBRT_FILM_SAVE_FP16_DEFAULT						= TPBRT_TRUE;
	static const tpbrt_float_t TPBRT_FILM_ISO_DEFAULT							= 100.0f;
	static const tpbrt_float_t TPBRT_FILM_WHITE_BALANCE_DEFAULT					= 0.0f;
	static const tpbrt_film_sensor_t TPBRT_FILM_SENSOR_DEFAULT					= TPBRT_FILM_SENSOR_CIE_1931;
	static const tpbrt_float_t TPBRT_FILM_MAX_COMPONENT_VALUE_DEFAULT			= INFINITY;

	static const tpbrt_string_t TPBRT_FILM_GBUFFER_COORD_SYS_STR				= TPBRT_STRING("coordinatesystem");
	static const tpbrt_string_t TPBRT_FILM_SPECTRAL_N_BUCKETS_STR				= TPBRT_STRING("nbuckets");
	static const tpbrt_string_t TPBRT_FILM_SPECTRAL_LAMBDA_MIN_STR				= TPBRT_STRING("lambdamin");
	static const tpbrt_string_t TPBRT_FILM_SPECTRAL_LAMBDA_MAX_STR				= TPBRT_STRING("lambdamax");
	static const tpbrt_string_t TPBRT_FILM_X_RESOULUTION_STR					= TPBRT_STRING("xresolution");
	static const tpbrt_string_t TPBRT_FILM_Y_RESOULUTION_STR					= TPBRT_STRING("yresolution");
	static const tpbrt_string_t TPBRT_FILM_CROP_WINDOW_STR						= TPBRT_STRING("cropwindow");
	static const tpbrt_string_t TPBRT_FILM_PIXEL_BOUNDS_STR						= TPBRT_STRING("pixelbounds");
	static const tpbrt_string_t TPBRT_FILM_DIAGONAL_STR							= TPBRT_STRING("diagonal");
	static const tpbrt_string_t TPBRT_FILM_FILE_NAME_STR						= TPBRT_STRING("filename");
	static const tpbrt_string_t TPBRT_FILM_SAVE_FP16_STR						= TPBRT_STRING("savefp16");
	static const tpbrt_string_t TPBRT_FILM_ISO_STR								= TPBRT_STRING("iso");
	static const tpbrt_string_t TPBRT_FILM_WHITE_BALANCE_STR					= TPBRT_STRING("whitebalance");
	static const tpbrt_string_t TPBRT_FILM_SENSOR_STR							= TPBRT_STRING("sensor");
	static const tpbrt_string_t TPBRT_FILM_MAX_COMPONENT_VALUE_STR				= TPBRT_STRING("maxcomponentvalue");

	tpbrt_error_t tpbrt_create_default_film(tpbrt_film_t* film) {
			if (film == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		film->type				  = TPBRT_FILM_TYPE_RGB;
		film->x_resolution		  = TPBRT_FILM_X_RES_DEFAULT;
		film->y_resolution		  = TPBRT_FILM_Y_RES_DEFAULT;
		film->crop_window[0]	  = 0.0f;
		film->crop_window[1]	  = 1.0f;
		film->crop_window[2]	  = 0.0f;
		film->crop_window[3]	  = 1.0f;
		film->pixel_bounds[0]	  = 0;
		film->pixel_bounds[1]	  = film->x_resolution;
		film->pixel_bounds[2]	  = 0;
		film->pixel_bounds[3]	  = film->y_resolution;
		film->diagonal			  = TPBRT_FILM_DIAGONAL_DEFAULT;
		film->file_name.data	  = TPBRT_FILM_FILE_NAME_DEFAULT.data;
		film->file_name.size	  = TPBRT_FILM_FILE_NAME_DEFAULT.size;
		film->save_fp16			  = TPBRT_FILM_SAVE_FP16_DEFAULT;
		film->iso				  = TPBRT_FILM_ISO_DEFAULT;
		film->white_balance		  = TPBRT_FILM_WHITE_BALANCE_DEFAULT;
		film->sensor			  = TPBRT_FILM_SENSOR_CIE_1931;
		film->max_component_value = TPBRT_FILM_MAX_COMPONENT_VALUE_DEFAULT;
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_film_type_from_string(const tpbrt_string_t* const type_str, tpbrt_film_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_FILM_TYPE_MAX_NUM] = {
			TPBRT_STRING("rgb"),
			TPBRT_STRING("gbuffer"),
			TPBRT_STRING("spectral"),
		};

			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_film_type_t t = 0; t < TPBRT_FILM_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS + t)) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_FILM_TYPE;
	}

	static tpbrt_error_t tpbrt_film_sensor_from_string(const tpbrt_string_t* const type_str, tpbrt_film_sensor_t* const sensor) {
		static const tpbrt_string_t SENSORS_STRS[TPBRT_FILM_SENSOR_MAX_NUM] = {
			TPBRT_STRING("cie1931"),
			TPBRT_STRING("canon_eos_100d"),
			TPBRT_STRING("canon_eos_1dx_mkii"),
			TPBRT_STRING("canon_eos_200d"),
			TPBRT_STRING("canon_eos_200d_mkii"),
			TPBRT_STRING("canon_eos_5d"),
			TPBRT_STRING("canon_eos_5d_mkii"),
			TPBRT_STRING("canon_eos_5d_mkiii"),
			TPBRT_STRING("canon_eos_5d_mkiv"),
			TPBRT_STRING("canon_eos_5ds"),
			TPBRT_STRING("canon_eos_m"),
			TPBRT_STRING("hasselblad_l1d_20c"),
			TPBRT_STRING("nikon_d810"),
			TPBRT_STRING("nikon_d850"),
			TPBRT_STRING("sony_ilce_6400"),
			TPBRT_STRING("sony_ilce_7m3"),
			TPBRT_STRING("sony_ilce_7rm3"),
			TPBRT_STRING("sony_ilce_9"),
		};

			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || sensor == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_film_sensor_t s = 0; s < TPBRT_FILM_SENSOR_MAX_NUM; ++s) {
					if (tpbrt_string_equals(type_str, SENSORS_STRS + s)) {
						*sensor = s;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_FILM_SENSOR;
	}

	tpbrt_error_t tpbrt_create_film(const tpbrt_string_t* const type_str, const tpbrt_params_list_t* const params,
	  tpbrt_film_t* const film) {
			if (params == TPBRT_NULL || film == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_error_t err;
		film->type = TPBRT_FILM_TYPE_RGB;
			if (type_str != TPBRT_NULL && type_str->data != TPBRT_NULL) {
				err = tpbrt_film_type_from_string(type_str, &film->type);
					if (err != TPBRT_ERROR_NONE) { return err; }
			}

			switch (film->type) {
			default:
				case TPBRT_FILM_TYPE_RGB: {
					break;
				}
				case TPBRT_FILM_TYPE_GBUFFER: {
					tpbrt_string_t coordinate_system_str;
					err = tpbrt_params_list_get_string(params, &TPBRT_FILM_GBUFFER_COORD_SYS_STR, &coordinate_system_str);
						if (err == TPBRT_ERROR_NOT_FOUND) {
							film->as.gbuffer.coordinate_system = TPBRT_FILM_GBUFFER_COORD_SYS_DEFAULT;
							break;
						}

						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_film(film);
							return err;
						}

					err = tpbrt_coordinate_system_from_string(&coordinate_system_str, &film->as.gbuffer.coordinate_system);
					tpbrt_free_string(&coordinate_system_str);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_film(film);
							return err;
						}
					break;
				}
				case TPBRT_FILM_TYPE_SPECTRAL: {
					err = tpbrt_params_list_get_uint(params, &TPBRT_FILM_SPECTRAL_N_BUCKETS_STR,
					  TPBRT_FILM_SPECTRAL_N_BUCKETS_DEFAULT, &film->as.spectral.nbuckets);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_film(film);
							return err;
						}


					err = tpbrt_params_list_get_float(params, &TPBRT_FILM_SPECTRAL_LAMBDA_MIN_STR,
					  TPBRT_FILM_SPECTRAL_LAMBDA_MIN_DEFAULT, &film->as.spectral.lambda_min);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_film(film);
							return err;
						}


					err = tpbrt_params_list_get_float(params, &TPBRT_FILM_SPECTRAL_LAMBDA_MAX_STR,
					  TPBRT_FILM_SPECTRAL_LAMBDA_MAX_DEFAULT, &film->as.spectral.lambda_max);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_film(film);
							return err;
						}

					break;
				}
			}

		err = tpbrt_params_list_get_uint(params, &TPBRT_FILM_X_RESOULUTION_STR, TPBRT_FILM_X_RES_DEFAULT, &film->x_resolution);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_film(film);
				return err;
			}

		err = tpbrt_params_list_get_uint(params, &TPBRT_FILM_Y_RESOULUTION_STR, TPBRT_FILM_Y_RES_DEFAULT, &film->y_resolution);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_film(film);
				return err;
			}

		tpbrt_float_array_t crop_window;
		err = tpbrt_params_list_get_floats(params, &TPBRT_FILM_CROP_WINDOW_STR, &crop_window);
			if (err != TPBRT_ERROR_NOT_FOUND) {
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_free_film(film);
						return err;
					}

					if (crop_window.count < 4) {
						tpbrt_free_float_array(&crop_window);
						tpbrt_free_film(film);
						return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
					}

					if (crop_window.count > 4) {
						tpbrt_free_float_array(&crop_window);
						tpbrt_free_film(film);
						return TPBRT_ERROR_TOO_MANY_VALUES;
					}

				film->crop_window[0] = crop_window.data[0];
				film->crop_window[1] = crop_window.data[1];
				film->crop_window[2] = crop_window.data[2];
				film->crop_window[3] = crop_window.data[3];
				tpbrt_free_float_array(&crop_window);
			}
			else {
				film->crop_window[0] = 0.0f;
				film->crop_window[1] = 1.0f;
				film->crop_window[2] = 0.0f;
				film->crop_window[3] = 1.0f;
			}

		tpbrt_uint_array_t pixel_bounds;
		err = tpbrt_params_list_get_uints(params, &TPBRT_FILM_PIXEL_BOUNDS_STR, &pixel_bounds);
			if (err != TPBRT_ERROR_NOT_FOUND) {
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_free_film(film);
						return err;
					}

					if (pixel_bounds.count < 4) {
						tpbrt_free_uint_array(&pixel_bounds);
						tpbrt_free_film(film);
						return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
					}

					if (pixel_bounds.count > 4) {
						tpbrt_free_uint_array(&pixel_bounds);
						tpbrt_free_film(film);
						return TPBRT_ERROR_TOO_MANY_VALUES;
					}

				film->pixel_bounds[0] = pixel_bounds.data[0];
				film->pixel_bounds[1] = pixel_bounds.data[1];
				film->pixel_bounds[2] = pixel_bounds.data[2];
				film->pixel_bounds[3] = pixel_bounds.data[3];
				tpbrt_free_uint_array(&pixel_bounds);
			}
			else {
				film->pixel_bounds[0] = 0.0f;
				film->pixel_bounds[1] = film->x_resolution;
				film->pixel_bounds[2] = 0.0f;
				film->pixel_bounds[3] = film->y_resolution;
			}

		err = tpbrt_params_list_get_float(params, &TPBRT_FILM_DIAGONAL_STR, TPBRT_FILM_DIAGONAL_DEFAULT, &film->diagonal);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_film(film);
				return err;
			}

		err = tpbrt_params_list_get_string(params, &TPBRT_FILM_FILE_NAME_STR, &film->file_name);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
				tpbrt_free_film(film);
				return err;
			}
			if (err == TPBRT_ERROR_NOT_FOUND) { tpbrt_copy_string(&film->file_name, &TPBRT_FILM_FILE_NAME_DEFAULT); }

		err = tpbrt_params_list_get_bool(params, &TPBRT_FILM_SAVE_FP16_STR, TPBRT_FILM_SAVE_FP16_DEFAULT, &film->save_fp16);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_film(film);
				return err;
			}

		err = tpbrt_params_list_get_float(params, &TPBRT_FILM_ISO_STR, TPBRT_FILM_ISO_DEFAULT, &film->iso);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_film(film);
				return err;
			}

		err = tpbrt_params_list_get_float(params, &TPBRT_FILM_WHITE_BALANCE_STR, TPBRT_FILM_WHITE_BALANCE_DEFAULT,
		  &film->white_balance);
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
			if (err == TPBRT_ERROR_NOT_FOUND) { film->sensor = TPBRT_FILM_SENSOR_DEFAULT; }
			else {
				err = tpbrt_film_sensor_from_string(&sensor_str, &film->sensor);
				tpbrt_free_string(&sensor_str);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_free_film(film);
						return err;
					}
			}

		err = tpbrt_params_list_get_float(params, &TPBRT_FILM_MAX_COMPONENT_VALUE_STR, TPBRT_FILM_MAX_COMPONENT_VALUE_DEFAULT,
		  &film->max_component_value);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_film(film);
				return err;
			}
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_film(tpbrt_film_t* const film) {
			if (film == TPBRT_NULL) { return; }

		tpbrt_free_string(&film->file_name);
	}

#ifdef __cplusplus
}
#endif