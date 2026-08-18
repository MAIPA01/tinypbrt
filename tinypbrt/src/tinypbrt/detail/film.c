#include <tinypbrt/pch.h>

#include <tinypbrt/detail/film_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	static const tpbrt_uint_t TPBRT_FILM_X_RES_DEFAULT		 = 1280;
	static const tpbrt_uint_t TPBRT_FILM_Y_RES_DEFAULT		 = 720;
	static const tpbrt_float_t TPBRT_FILM_DIAGONAL_DEFAULT	 = 35.0f;
	static const tpbrt_string_t TPBRT_FILM_FILE_NAME_DEFAULT = { .chars = "pbrt.exr", .size = 8 };

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
		(*film)->save_fp16			 = TPBRT_TRUE;
		(*film)->iso				 = 100.0f;
		(*film)->white_balance		 = 0.0f;
		(*film)->sensor				 = TPBRT_FILM_SENSOR_CIE_1931;
		(*film)->max_component_value = INFINITY;
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

	tpbrt_error_t tpbrt_create_film(const tpbrt_string_t* const type_str, const tpbrt_params_list_t* const params,
	  tpbrt_film_t** film) {
			if (params == TPBRT_NULL || film == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_film_type_t type = TPBRT_FILM_TYPE_RGB;
			if (type_str != TPBRT_NULL && type_str->chars != TPBRT_NULL) {
				const tpbrt_error_t error = tpbrt_film_type_from_string(type_str, &type);
					if (error != TPBRT_ERROR_NONE) { return error; }
			}
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