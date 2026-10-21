#include <tinypbrt/pch.h>

#include <tinypbrt/detail/film_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/coord_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	static const tpbrt_uint_t tpbrt_film_x_res_default				  = 1280;
	static const tpbrt_uint_t tpbrt_film_y_res_default				  = 720;
	static const tpbrt_float_t tpbrt_film_diagonal_default			  = 35.0f;
	static const tpbrt_string_t tpbrt_film_file_name_default		  = TPBRT_STRING("pbrt.exr");
	static const tpbrt_bool_t tpbrt_film_save_fp16_default			  = TPBRT_TRUE;
	static const tpbrt_float_t tpbrt_film_iso_default				  = 100.0f;
	static const tpbrt_float_t tpbrt_film_white_balance_default		  = 0.0f;
	static const tpbrt_film_sensor_t tpbrt_film_sensor_default		  = TPBRT_FILM_SENSOR_CIE_1931;
	static const tpbrt_float_t tpbrt_film_max_component_value_default = INFINITY;

	tpbrt_error_t tpbrt_film_type_from_string(const tpbrt_string_t type_str, tpbrt_film_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_FILM_TYPE_MAX_NUM] = {
			TPBRT_STRING("rgb"),
			TPBRT_STRING("gbuffer"),
			TPBRT_STRING("spectral"),
		};

			if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(type_str)) { return TPBRT_ERROR_INVALID_FILM_TYPE; }

			for (tpbrt_film_type_t t = 0; t < TPBRT_FILM_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS[t])) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_FILM_TYPE;
	}

	tpbrt_error_t tpbrt_film_sensor_from_string(const tpbrt_string_t type_str, tpbrt_film_sensor_t* const sensor) {
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

			if (sensor == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(type_str)) { return TPBRT_ERROR_INVALID_FILM_SENSOR; }

			for (tpbrt_film_sensor_t s = 0; s < TPBRT_FILM_SENSOR_MAX_NUM; ++s) {
					if (tpbrt_string_equals(type_str, SENSORS_STRS[s])) {
						*sensor = s;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_FILM_SENSOR;
	}

	tpbrt_error_t tpbrt_film_init_default(tpbrt_film_t* film) {
			if (film == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*film = (tpbrt_film_t) {
			.crop_window		 = { 0.0f, 1.0f,					 0.0f, 1.0f						},
			.pixel_bounds		 = { 0,	tpbrt_film_x_res_default, 0,	 tpbrt_film_y_res_default },
			.file_name			 = TPBRT_EMPTY_STRING,
			.sensor				 = tpbrt_film_sensor_default,
			.x_resolution		 = tpbrt_film_x_res_default,
			.y_resolution		 = tpbrt_film_y_res_default,
			.diagonal			 = tpbrt_film_diagonal_default,
			.save_fp16			 = tpbrt_film_save_fp16_default,
			.iso				 = tpbrt_film_iso_default,
			.white_balance		 = tpbrt_film_white_balance_default,
			.max_component_value = tpbrt_film_max_component_value_default,
			.type				 = TPBRT_FILM_TYPE_RGB,
		};

		const tpbrt_error_t err = tpbrt_string_copy(&film->file_name, tpbrt_film_file_name_default);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_film_free(film);
				return err;
			}

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_film_read_gbuffer(tpbrt_film_gbuffer_params_t* const gbuffer, const tpbrt_params_list_t params) {
			if (gbuffer == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t COORD_SYS_STR				 = TPBRT_STRING("coordinatesystem");

		static const tpbrt_string_t COORD_SYS_DEFAULT_STR		 = TPBRT_EMPTY_STRING;
		static const tpbrt_coord_system_type_t COORD_SYS_DEFAULT = TPBRT_COORD_SYSTEM_TYPE_CAMERA;

		tpbrt_string_t coordinate_system_str;
		tpbrt_error_t err = tpbrt_params_list_get_string(params, COORD_SYS_STR, COORD_SYS_DEFAULT_STR, &coordinate_system_str);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (tpbrt_string_empty(coordinate_system_str) == TPBRT_TRUE) {
				gbuffer->coordinate_system = COORD_SYS_DEFAULT;
				return TPBRT_ERROR_NONE;
			}

		tpbrt_coord_system_type_t coordinate_system;
		err = tpbrt_coord_system_type_from_string(coordinate_system_str, &coordinate_system);
		tpbrt_string_free(&coordinate_system_str);
		gbuffer->coordinate_system = coordinate_system;

		return err;
	}

	static tpbrt_error_t tpbrt_film_read_spectral(tpbrt_film_spectral_params_t* const spectral,
	  const tpbrt_params_list_t params) {
			if (spectral == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t N_BUCKETS_STR	  = TPBRT_STRING("nbuckets");
		static const tpbrt_string_t LAMBDA_MIN_STR	  = TPBRT_STRING("lambdamin");
		static const tpbrt_string_t LAMBDA_MAX_STR	  = TPBRT_STRING("lambdamax");

		static const tpbrt_uint_t N_BUCKETS_DEFAULT	  = 16u;
		static const tpbrt_float_t LAMBDA_MIN_DEFAULT = 360.0f;
		static const tpbrt_float_t LAMBDA_MAX_DEFAULT = 830.0f;

		tpbrt_error_t err = tpbrt_params_list_get_uint(params, N_BUCKETS_STR, N_BUCKETS_DEFAULT, &spectral->nbuckets);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, LAMBDA_MIN_STR, LAMBDA_MIN_DEFAULT, &spectral->lambda_min);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, LAMBDA_MAX_STR, LAMBDA_MAX_DEFAULT, &spectral->lambda_max);
	}

	tpbrt_error_t tpbrt_film_init(const tpbrt_string_t type_str, const tpbrt_params_list_t params, tpbrt_film_t* const film) {
			if (film == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(film, 0, sizeof(tpbrt_film_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t TPBRT_FILM_X_RESOULUTION_STR	   = TPBRT_STRING("xresolution");
		static const tpbrt_string_t TPBRT_FILM_Y_RESOULUTION_STR	   = TPBRT_STRING("yresolution");
		static const tpbrt_string_t TPBRT_FILM_CROP_WINDOW_STR		   = TPBRT_STRING("cropwindow");
		static const tpbrt_string_t TPBRT_FILM_PIXEL_BOUNDS_STR		   = TPBRT_STRING("pixelbounds");
		static const tpbrt_string_t TPBRT_FILM_DIAGONAL_STR			   = TPBRT_STRING("diagonal");
		static const tpbrt_string_t TPBRT_FILM_FILE_NAME_STR		   = TPBRT_STRING("filename");
		static const tpbrt_string_t TPBRT_FILM_SAVE_FP16_STR		   = TPBRT_STRING("savefp16");
		static const tpbrt_string_t TPBRT_FILM_ISO_STR				   = TPBRT_STRING("iso");
		static const tpbrt_string_t TPBRT_FILM_WHITE_BALANCE_STR	   = TPBRT_STRING("whitebalance");
		static const tpbrt_string_t TPBRT_FILM_SENSOR_STR			   = TPBRT_STRING("sensor");
		static const tpbrt_string_t TPBRT_FILM_MAX_COMPONENT_VALUE_STR = TPBRT_STRING("maxcomponentvalue");

		tpbrt_error_t err											   = TPBRT_ERROR_NONE;
		film->type													   = TPBRT_FILM_TYPE_RGB;
			if (tpbrt_string_empty(type_str) == TPBRT_FALSE) {
				tpbrt_film_type_t type;
				err		   = tpbrt_film_type_from_string(type_str, &type);
				film->type = type;
			}
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_film_free(film);
				return err;
			}

			switch (film->type) {
			default:
				case TPBRT_FILM_TYPE_RGB: {
					err = TPBRT_ERROR_NONE;
					break;
				}
				case TPBRT_FILM_TYPE_GBUFFER: {
					err = tpbrt_film_read_gbuffer(&film->as.gbuffer, params);
					break;
				}
				case TPBRT_FILM_TYPE_SPECTRAL: {
					err = tpbrt_film_read_spectral(&film->as.spectral, params);
					break;
				}
			}

			if (err != TPBRT_ERROR_NONE) {
				tpbrt_film_free(film);
				return err;
			}

		err = tpbrt_params_list_get_uint(params, TPBRT_FILM_X_RESOULUTION_STR, tpbrt_film_x_res_default, &film->x_resolution);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_film_free(film);
				return err;
			}

		err = tpbrt_params_list_get_uint(params, TPBRT_FILM_Y_RESOULUTION_STR, tpbrt_film_y_res_default, &film->y_resolution);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_film_free(film);
				return err;
			}

		tpbrt_float_array_t crop_window;
		err = tpbrt_params_list_get_floats_of_size(params, TPBRT_FILM_CROP_WINDOW_STR, 4u, &crop_window);

			if (err == TPBRT_ERROR_NOT_FOUND) {
				film->crop_window[0] = 0.0f;
				film->crop_window[1] = 1.0f;
				film->crop_window[2] = 0.0f;
				film->crop_window[3] = 1.0f;
			}
			else if (err == TPBRT_ERROR_NONE) {
				memcpy(film->crop_window, crop_window.data, sizeof(tpbrt_float_t) * 4u);
				tpbrt_float_array_free(&crop_window);
			}
			else {
				tpbrt_film_free(film);
				return err;
			}

		tpbrt_uint_array_t pixel_bounds;
		err = tpbrt_params_list_get_uints_of_size(params, TPBRT_FILM_PIXEL_BOUNDS_STR, 4u, &pixel_bounds);

			if (err == TPBRT_ERROR_NOT_FOUND) {
				film->pixel_bounds[0] = 0.0f;
				film->pixel_bounds[1] = film->x_resolution;
				film->pixel_bounds[2] = 0.0f;
				film->pixel_bounds[3] = film->y_resolution;
			}
			else if (err == TPBRT_ERROR_NONE) {
				memcpy(film->pixel_bounds, pixel_bounds.data, sizeof(tpbrt_uint_t) * 4u);
				tpbrt_uint_array_free(&pixel_bounds);
			}
			else {
				tpbrt_film_free(film);
				return err;
			}

		err = tpbrt_params_list_get_float(params, TPBRT_FILM_DIAGONAL_STR, tpbrt_film_diagonal_default, &film->diagonal);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_film_free(film);
				return err;
			}

		err = tpbrt_params_list_get_string(params, TPBRT_FILM_FILE_NAME_STR, TPBRT_EMPTY_STRING, &film->file_name);
			if (tpbrt_string_empty(film->file_name) == TPBRT_TRUE) {
				err = tpbrt_string_copy(&film->file_name, tpbrt_film_file_name_default);
			}

			if (err != TPBRT_ERROR_NONE) {
				tpbrt_film_free(film);
				return err;
			}

		tpbrt_bool_t save_fp16;
		err = tpbrt_params_list_get_bool(params, TPBRT_FILM_SAVE_FP16_STR, tpbrt_film_save_fp16_default, &save_fp16);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_film_free(film);
				return err;
			}
		film->save_fp16 = save_fp16;

		err				= tpbrt_params_list_get_float(params, TPBRT_FILM_ISO_STR, tpbrt_film_iso_default, &film->iso);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_film_free(film);
				return err;
			}

		err = tpbrt_params_list_get_float(params, TPBRT_FILM_WHITE_BALANCE_STR, tpbrt_film_white_balance_default,
		  &film->white_balance);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_film_free(film);
				return err;
			}

		tpbrt_string_t sensor_str;
		err = tpbrt_params_list_get_string(params, TPBRT_FILM_SENSOR_STR, TPBRT_EMPTY_STRING, &sensor_str);
			if (tpbrt_string_empty(sensor_str) == TPBRT_TRUE) { film->sensor = tpbrt_film_sensor_default; }
			else if (err == TPBRT_ERROR_NONE) {
				tpbrt_film_sensor_t sensor;
				err = tpbrt_film_sensor_from_string(sensor_str, &sensor);
				tpbrt_string_free(&sensor_str);
				film->sensor = sensor;
			}

			if (err != TPBRT_ERROR_NONE) {
				tpbrt_film_free(film);
				return err;
			}

		err = tpbrt_params_list_get_float(params, TPBRT_FILM_MAX_COMPONENT_VALUE_STR, tpbrt_film_max_component_value_default,
		  &film->max_component_value);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_film_free(film);
				return err;
			}
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_film_free(tpbrt_film_t* const film) {
			if (film == TPBRT_NULL) { return; }

		tpbrt_string_free(&film->file_name);
	}

#ifdef __cplusplus
}
#endif