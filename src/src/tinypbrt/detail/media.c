#include <tinypbrt/pch.h>

#include <tinypbrt/detail/media_internal.h>

#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region MEDIA

	tpbrt_error_t tpbrt_media_type_from_string(const tpbrt_string_t type_str, tpbrt_media_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_MEDIA_TYPE_MAX_NUM] = {
			TPBRT_STRING("cloud"),
			TPBRT_STRING("homogeneous"),
			TPBRT_STRING("nanovdb"),
			TPBRT_STRING("rgbgrid"),
			TPBRT_STRING("uniformgrid"),
		};

			if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(type_str)) { return TPBRT_ERROR_INVALID_MEDIA_TYPE; }

			for (tpbrt_media_type_t t = 0; t < TPBRT_MEDIA_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS[t])) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_MEDIA_TYPE;
	}

	static tpbrt_error_t tpbrt_media_read_cloud(tpbrt_media_cloud_params_t* const cloud, const tpbrt_params_list_t params) {
			if (cloud == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t DENSITY_STR	  = TPBRT_STRING("density");
		static const tpbrt_string_t FREQUENCY_STR = TPBRT_STRING("frequency");
		static const tpbrt_string_t G_STR		  = TPBRT_STRING("g");
		static const tpbrt_string_t P0_STR		  = TPBRT_STRING("p0");
		static const tpbrt_string_t P1_STR		  = TPBRT_STRING("p1");
		static const tpbrt_string_t SIGMA_A_STR	  = TPBRT_STRING("sigma_a");
		static const tpbrt_string_t SIGMA_S_STR	  = TPBRT_STRING("sigma_s");
		static const tpbrt_string_t WISPINESS_STR = TPBRT_STRING("wispiness");

		tpbrt_error_t err						  = tpbrt_params_list_get_float(params, DENSITY_STR, 1.0f, &cloud->density);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, FREQUENCY_STR, 5.0f, &cloud->frequency);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, G_STR, 0.0f, &cloud->g);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_point3(params, P0_STR, &cloud->p0);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { cloud->p0 = TPBRT_VEC3(0.0f, 0.0f, 0.0f); }

		err = tpbrt_params_list_get_point3(params, P1_STR, &cloud->p1);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { cloud->p1 = TPBRT_VEC3(1.0f, 1.0f, 1.0f); }

		err = tpbrt_params_list_get_spectrum(params, SIGMA_A_STR, &cloud->sigma_a);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { cloud->sigma_a = TPBRT_SPECTRUM_BLACKBODY(1u); }

		err = tpbrt_params_list_get_spectrum(params, SIGMA_S_STR, &cloud->sigma_s);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { cloud->sigma_s = TPBRT_SPECTRUM_BLACKBODY(1u); }

		return tpbrt_params_list_get_float(params, WISPINESS_STR, 1.0f, &cloud->wispiness);
	}

	static tpbrt_error_t tpbrt_media_read_homogeneous(tpbrt_media_homogeneous_params_t* const homogeneous,
	  const tpbrt_params_list_t params) {
			if (homogeneous == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t G_STR		 = TPBRT_STRING("g");
		static const tpbrt_string_t LE_STR		 = TPBRT_STRING("Le");
		static const tpbrt_string_t LE_SCALE_STR = TPBRT_STRING("Lescale");
		static const tpbrt_string_t PRESET_STR	 = TPBRT_STRING("preset");
		static const tpbrt_string_t SIGMA_A_STR	 = TPBRT_STRING("sigma_a");
		static const tpbrt_string_t SIGMA_S_STR	 = TPBRT_STRING("sigma_s");
		static const tpbrt_string_t SCALE_STR	 = TPBRT_STRING("scale");

		tpbrt_error_t err						 = tpbrt_params_list_get_float(params, G_STR, 0.0f, &homogeneous->g);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_spectrum(params, LE_STR, &homogeneous->Le);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { homogeneous->Le = TPBRT_SPECTRUM_BLACKBODY(0u); }

		err = tpbrt_params_list_get_float(params, LE_SCALE_STR, 1.0f, &homogeneous->Le_scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_string(params, PRESET_STR, &homogeneous->preset);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_spectrum(params, SIGMA_A_STR, &homogeneous->sigma_a);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { homogeneous->sigma_a = TPBRT_SPECTRUM_BLACKBODY(1u); }

		err = tpbrt_params_list_get_spectrum(params, SIGMA_S_STR, &homogeneous->sigma_s);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { homogeneous->sigma_s = TPBRT_SPECTRUM_BLACKBODY(1u); }

		return tpbrt_params_list_get_float(params, SCALE_STR, 1.0f, &homogeneous->scale);
	}

	static tpbrt_error_t tpbrt_media_read_nano_vdb(tpbrt_media_nano_vdb_params_t* const nano_vdb,
	  const tpbrt_params_list_t params) {
			if (nano_vdb == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t G_STR				   = TPBRT_STRING("g");
		static const tpbrt_string_t LE_SCALE_STR		   = TPBRT_STRING("Lescale");
		static const tpbrt_string_t SIGMA_A_STR			   = TPBRT_STRING("sigma_a");
		static const tpbrt_string_t SIGMA_S_STR			   = TPBRT_STRING("sigma_s");
		static const tpbrt_string_t SCALE_STR			   = TPBRT_STRING("scale");
		static const tpbrt_string_t FILE_NAME_STR		   = TPBRT_STRING("filename");
		static const tpbrt_string_t TEMPERATURE_OFFSET_STR = TPBRT_STRING("temepratureoffset");
		static const tpbrt_string_t TEMPERATURE_SCALE_STR  = TPBRT_STRING("temperaturescale");

		tpbrt_error_t err								   = tpbrt_params_list_get_float(params, G_STR, 0.0f, &nano_vdb->g);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, LE_SCALE_STR, 1.0f, &nano_vdb->Le_scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_spectrum(params, SIGMA_A_STR, &nano_vdb->sigma_a);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { nano_vdb->sigma_a = TPBRT_SPECTRUM_BLACKBODY(1u); }

		err = tpbrt_params_list_get_spectrum(params, SIGMA_S_STR, &nano_vdb->sigma_s);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { nano_vdb->sigma_s = TPBRT_SPECTRUM_BLACKBODY(1u); }

		err = tpbrt_params_list_get_float(params, SCALE_STR, 1.0f, &nano_vdb->scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_string(params, FILE_NAME_STR, &nano_vdb->file_name);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_float(params, TEMPERATURE_OFFSET_STR, 0.0f, &nano_vdb->temperature_offset);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, TEMPERATURE_SCALE_STR, 1.0f, &nano_vdb->temperature_scale);
	}

	static tpbrt_error_t tpbrt_media_read_rgb_grid(tpbrt_media_rgb_grid_params_t* const rgb_grid,
	  const tpbrt_params_list_t params) {
			if (rgb_grid == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t G_STR		 = TPBRT_STRING("g");
		static const tpbrt_string_t LE_STR		 = TPBRT_STRING("Le");
		static const tpbrt_string_t LE_SCALE_STR = TPBRT_STRING("Lescale");
		static const tpbrt_string_t P0_STR		 = TPBRT_STRING("p0");
		static const tpbrt_string_t P1_STR		 = TPBRT_STRING("p1");
		static const tpbrt_string_t SIGMA_A_STR	 = TPBRT_STRING("sigma_a");
		static const tpbrt_string_t SIGMA_S_STR	 = TPBRT_STRING("sigma_s");
		static const tpbrt_string_t SCALE_STR	 = TPBRT_STRING("scale");

		tpbrt_error_t err						 = tpbrt_params_list_get_float(params, G_STR, 0.0f, &rgb_grid->g);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_rgbs(params, LE_STR, &rgb_grid->Le);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_float(params, LE_SCALE_STR, 1.0f, &rgb_grid->Le_scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_point3(params, P0_STR, &rgb_grid->p0);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { rgb_grid->p0 = TPBRT_VEC3(0.0f, 0.0f, 0.0f); }

		err = tpbrt_params_list_get_point3(params, P1_STR, &rgb_grid->p1);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { rgb_grid->p1 = TPBRT_VEC3(1.0f, 1.0f, 1.0f); }

		err = tpbrt_params_list_get_rgbs(params, SIGMA_A_STR, &rgb_grid->sigma_a);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_rgbs(params, SIGMA_S_STR, &rgb_grid->sigma_s);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		return tpbrt_params_list_get_float(params, SCALE_STR, 1.0f, &rgb_grid->scale);
	}

	static tpbrt_error_t tpbrt_media_read_uniform_grid(tpbrt_media_uniform_grid_params_t* const uniform_grid,
	  const tpbrt_params_list_t params) {
			if (uniform_grid == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t DENSITY_STR			   = TPBRT_STRING("density");
		static const tpbrt_string_t G_STR				   = TPBRT_STRING("g");
		static const tpbrt_string_t LE_STR				   = TPBRT_STRING("Le");
		static const tpbrt_string_t LE_SCALE_STR		   = TPBRT_STRING("Lescale");
		static const tpbrt_string_t P0_STR				   = TPBRT_STRING("p0");
		static const tpbrt_string_t P1_STR				   = TPBRT_STRING("p1");
		static const tpbrt_string_t NX_STR				   = TPBRT_STRING("nx");
		static const tpbrt_string_t NY_STR				   = TPBRT_STRING("ny");
		static const tpbrt_string_t NZ_STR				   = TPBRT_STRING("nz");
		static const tpbrt_string_t PRESET_STR			   = TPBRT_STRING("preset");
		static const tpbrt_string_t SIGMA_A_STR			   = TPBRT_STRING("sigma_a");
		static const tpbrt_string_t SIGMA_S_STR			   = TPBRT_STRING("sigma_s");
		static const tpbrt_string_t SCALE_STR			   = TPBRT_STRING("scale");
		static const tpbrt_string_t TEMPERATURE_STR		   = TPBRT_STRING("temperature");
		static const tpbrt_string_t TEMPERATURE_OFFSET_STR = TPBRT_STRING("temepratureoffset");
		static const tpbrt_string_t TEMPERATURE_SCALE_STR  = TPBRT_STRING("temperaturescale");

		tpbrt_error_t err = tpbrt_params_list_get_floats(params, DENSITY_STR, &uniform_grid->density);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_float(params, G_STR, 0.0f, &uniform_grid->g);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_spectrum(params, LE_STR, &uniform_grid->Le);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { uniform_grid->Le = TPBRT_SPECTRUM_BLACKBODY(0u); }

		err = tpbrt_params_list_get_float(params, LE_SCALE_STR, 1.0f, &uniform_grid->Le_scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_point3(params, P0_STR, &uniform_grid->p0);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { uniform_grid->p0 = TPBRT_VEC3(0.0f, 0.0f, 0.0f); }

		err = tpbrt_params_list_get_point3(params, P1_STR, &uniform_grid->p0);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { uniform_grid->p1 = TPBRT_VEC3(1.0f, 1.0f, 1.0f); }

		err = tpbrt_params_list_get_uint(params, NX_STR, 1u, &uniform_grid->nx);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_uint(params, NY_STR, 1u, &uniform_grid->ny);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_uint(params, NZ_STR, 1u, &uniform_grid->nz);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_string(params, PRESET_STR, &uniform_grid->preset);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_spectrum(params, SIGMA_A_STR, &uniform_grid->sigma_a);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { uniform_grid->sigma_a = TPBRT_SPECTRUM_BLACKBODY(1u); }

		err = tpbrt_params_list_get_spectrum(params, SIGMA_S_STR, &uniform_grid->sigma_s);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { uniform_grid->sigma_s = TPBRT_SPECTRUM_BLACKBODY(1u); }

		err = tpbrt_params_list_get_float(params, SCALE_STR, 1.0f, &uniform_grid->scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_floats(params, TEMPERATURE_STR, &uniform_grid->temperature);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_float(params, TEMPERATURE_OFFSET_STR, 0.0f, &uniform_grid->temperature_offset);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, TEMPERATURE_SCALE_STR, 1.0f, &uniform_grid->temperature_scale);
	}

	tpbrt_error_t tpbrt_media_init(const tpbrt_string_t name, const tpbrt_string_t type_str, const tpbrt_params_list_t params,
	  tpbrt_media_t* const media) {
			if (media == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(media, 0, sizeof(tpbrt_media_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_error_t err = tpbrt_string_copy(&media->name, name);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_media_free(media);
				return err;
			}

		tpbrt_string_t temp_string = type_str;
			if (tpbrt_string_empty(temp_string) == TPBRT_TRUE) {
				static const tpbrt_string_t TYPE_STR = TPBRT_STRING("type");
				err									 = tpbrt_params_list_get_string(params, TYPE_STR, &temp_string);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_media_free(media);
						return err;
					}
			}

		tpbrt_media_type_t type;
		err = tpbrt_media_type_from_string(temp_string, &type);
		tpbrt_string_free(&temp_string);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_media_free(media);
				return err;
			}
		media->type = type;

			switch (media->type) {
			default:
				case TPBRT_MEDIA_TYPE_CLOUD: {
					err = tpbrt_media_read_cloud(&media->as.cloud, params);
					break;
				}
				case TPBRT_MEDIA_TYPE_HOMOGENEOUS: {
					err = tpbrt_media_read_homogeneous(&media->as.homogeneous, params);
					break;
				}
				case TPBRT_MEDIA_TYPE_NANO_VDB: {
					err = tpbrt_media_read_nano_vdb(&media->as.nano_vdb, params);
					break;
				}
				case TPBRT_MEDIA_TYPE_RGB_GRID: {
					err = tpbrt_media_read_rgb_grid(&media->as.rgb_grid, params);
					break;
				}
				case TPBRT_MEDIA_TYPE_UNIFORM_GRID: {
					err = tpbrt_media_read_uniform_grid(&media->as.uniform_grid, params);
					break;
				}
			}

			if (err != TPBRT_ERROR_NONE) {
				tpbrt_media_free(media);
				return err;
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_media_free(tpbrt_media_t* const media) {
			if (media == TPBRT_NULL) { return; }

		tpbrt_string_free(&media->name);

			switch (media->type) {
				case TPBRT_MEDIA_TYPE_CLOUD: {
					tpbrt_media_cloud_params_t* const params = &media->as.cloud;

					tpbrt_spectrum_free(&params->sigma_a);
					tpbrt_spectrum_free(&params->sigma_s);
					break;
				}
				case TPBRT_MEDIA_TYPE_HOMOGENEOUS: {
					tpbrt_media_homogeneous_params_t* const params = &media->as.homogeneous;

					tpbrt_spectrum_free(&params->Le);
					tpbrt_spectrum_free(&params->sigma_a);
					tpbrt_spectrum_free(&params->sigma_s);

					tpbrt_string_free(&params->preset);
					break;
				}
				case TPBRT_MEDIA_TYPE_NANO_VDB: {
					tpbrt_media_nano_vdb_params_t* const params = &media->as.nano_vdb;

					tpbrt_spectrum_free(&params->sigma_a);
					tpbrt_spectrum_free(&params->sigma_s);

					tpbrt_string_free(&params->file_name);
					break;
				}
				case TPBRT_MEDIA_TYPE_RGB_GRID: {
					tpbrt_media_rgb_grid_params_t* const params = &media->as.rgb_grid;

					tpbrt_rgb_array_free(&params->Le);
					tpbrt_rgb_array_free(&params->sigma_a);
					tpbrt_rgb_array_free(&params->sigma_s);
					break;
				}
				case TPBRT_MEDIA_TYPE_UNIFORM_GRID: {
					tpbrt_media_uniform_grid_params_t* const params = &media->as.uniform_grid;

					tpbrt_float_array_free(&params->density);

					tpbrt_spectrum_free(&params->Le);

					tpbrt_string_free(&params->preset);

					tpbrt_spectrum_free(&params->sigma_a);
					tpbrt_spectrum_free(&params->sigma_s);

					tpbrt_float_array_free(&params->temperature);
					break;
				}
			default: break;
			}
	}

#pragma endregion

#pragma region MEDIAS_LIST
#define TPBRT_EMPTY_MEDIAS_LIST (tpbrt_medias_list_t) { .medias = TPBRT_NULL, .count = 0 }

	tpbrt_error_t tpbrt_medias_list_init(tpbrt_medias_list_t* const medias_list) {
			if (medias_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*medias_list = TPBRT_EMPTY_MEDIAS_LIST;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_medias_list_append(tpbrt_medias_list_t* const medias_list, const tpbrt_media_t* const media) {
			if (media == TPBRT_NULL || medias_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_size_t new_count = medias_list->count + 1;

		tpbrt_media_t* new_list		 = realloc(medias_list->medias, sizeof(tpbrt_media_t) * new_count);
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		medias_list->medias						= new_list;
		medias_list->medias[medias_list->count] = *media;
		medias_list->count						= new_count;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_medias_list_get(const tpbrt_medias_list_t medias_list, const tpbrt_string_t media_name,
	  const tpbrt_media_t** const media) {
			if (media == TPBRT_NULL) {
				*media = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_size_t i = 0; i < medias_list.count; i++) {
					if (tpbrt_string_equals(medias_list.medias[i].name, media_name)) {
						*media = &medias_list.medias[i];
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_NOT_FOUND;
	}

	tpbrt_error_t tpbrt_medias_list_get_handle(const tpbrt_medias_list_t medias_list, const tpbrt_string_t media_name,
	  tpbrt_media_handle_t* const media_handle) {
			if (media_handle == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_media_t* object;
		const tpbrt_error_t err = tpbrt_medias_list_get(medias_list, media_name, &object);

			if (err != TPBRT_ERROR_NONE) {
				*media_handle = TPBRT_MEDIA_HANDLE_INVALID;
				return err;
			}

		media_handle->media_idx = object->idx;
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_medias_list_free(tpbrt_medias_list_t* const medias_list) {
			if (medias_list == TPBRT_NULL || medias_list->medias == TPBRT_NULL) { return; }

			for (tpbrt_size_t i = 0; i < medias_list->count; ++i) { tpbrt_media_free(medias_list->medias + i); }

		free(medias_list->medias);
		*medias_list = TPBRT_EMPTY_MEDIAS_LIST;
	}

	tpbrt_bool_t tpbrt_medias_list_empty(const tpbrt_medias_list_t medias_list) {
		return medias_list.medias == TPBRT_NULL || medias_list.count == 0;
	}

#pragma endregion

#pragma region API

	tpbrt_error_t tpbrt_get_media_by_name(const tpbrt_medias_list_t medias, const tpbrt_string_t name,
	  const tpbrt_media_t** const media) {
		return tpbrt_medias_list_get(medias, name, media);
	}

	tpbrt_error_t tpbrt_get_media_by_handle(const tpbrt_medias_list_t medias, const tpbrt_media_handle_t handle,
	  const tpbrt_media_t** const media) {
			if (media == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (handle.media_idx == TPBRT_MEDIA_IDX_INVALID || handle.media_idx >= medias.count) {
				*media = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_HANDLE;
			}

		*media = medias.medias + handle.media_idx;
		return TPBRT_ERROR_NONE;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif