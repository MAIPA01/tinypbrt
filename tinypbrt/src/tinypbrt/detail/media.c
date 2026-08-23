#include <tinypbrt/pch.h>

#include <tinypbrt/detail/media_internal.h>

#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region MEDIA

	static tpbrt_error_t tpbrt_media_type_from_string(const tpbrt_string_t* const type_str, tpbrt_media_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_MEDIA_TYPE_MAX_NUM] = {
			TPBRT_STRING("cloud"),
			TPBRT_STRING("homogeneous"),
			TPBRT_STRING("nanovdb"),
			TPBRT_STRING("rgbgrid"),
			TPBRT_STRING("uniformgrid"),
		};

			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_media_type_t t = 0; t < TPBRT_MEDIA_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS + t)) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_MEDIA_TYPE;
	}

	tpbrt_error_t tpbrt_create_media(const tpbrt_string_t* const name, const tpbrt_string_t* const type_str,
	  const tpbrt_params_list_t* const params, tpbrt_media_t* const media) {
			if (name == TPBRT_NULL || name->data == TPBRT_NULL || params == TPBRT_NULL || media == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		tpbrt_error_t err = tpbrt_copy_string(&media->name, name);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_media(media);
				return err;
			}

		tpbrt_string_t temp_string = *type_str;
			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL) {
				static const tpbrt_string_t TYPE_STR = TPBRT_STRING("type");
				err									 = tpbrt_params_list_get_string(params, &TYPE_STR, &temp_string);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_free_media(media);
						return err;
					}
			}

		err = tpbrt_media_type_from_string(&temp_string, &media->type);
			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL) { tpbrt_free_string(&temp_string); }
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_media(media);
				return err;
			}

			switch (media->type) {
			default:
				case TPBRT_MEDIA_TYPE_CLOUD: {
					static const tpbrt_string_t DENSITY_STR	  = TPBRT_STRING("density");
					static const tpbrt_string_t FREQUENCY_STR = TPBRT_STRING("frequency");
					static const tpbrt_string_t G_STR		  = TPBRT_STRING("g");
					static const tpbrt_string_t P0_STR		  = TPBRT_STRING("p0");
					static const tpbrt_string_t P1_STR		  = TPBRT_STRING("p1");
					static const tpbrt_string_t SIGMA_A_STR	  = TPBRT_STRING("sigma_a");
					static const tpbrt_string_t SIGMA_S_STR	  = TPBRT_STRING("sigma_s");
					static const tpbrt_string_t WISPINESS_STR = TPBRT_STRING("wispiness");

					tpbrt_media_cloud_params_t* media_params  = &media->as.cloud;

					err = tpbrt_params_list_get_float(params, &DENSITY_STR, 1.0f, &media_params->density);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &FREQUENCY_STR, 5.0f, &media_params->frequency);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &G_STR, 0.0f, &media_params->g);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_point3(params, &P0_STR, &media_params->p0);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							media_params->p0.x = 0.0f;
							media_params->p0.y = 0.0f;
							media_params->p0.z = 0.0f;
						}

					err = tpbrt_params_list_get_point3(params, &P1_STR, &media_params->p1);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							media_params->p1.x = 1.0f;
							media_params->p1.y = 1.0f;
							media_params->p1.z = 1.0f;
						}

					err = tpbrt_params_list_get_spectrum(params, &SIGMA_A_STR, &media_params->sigma_a);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							media_params->sigma_a.type		   = TPBRT_SPECTRUM_TYPE_BLACKBODY;
							media_params->sigma_a.as.blackbody = 1u;
						}

					err = tpbrt_params_list_get_spectrum(params, &SIGMA_S_STR, &media_params->sigma_s);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							media_params->sigma_s.type		   = TPBRT_SPECTRUM_TYPE_BLACKBODY;
							media_params->sigma_s.as.blackbody = 1u;
						}

					err = tpbrt_params_list_get_float(params, &WISPINESS_STR, 1.0f, &media_params->wispiness);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					break;
				}
				case TPBRT_MEDIA_TYPE_HOMOGENEOUS: {
					static const tpbrt_string_t G_STR			   = TPBRT_STRING("g");
					static const tpbrt_string_t LE_STR			   = TPBRT_STRING("Le");
					static const tpbrt_string_t LE_SCALE_STR	   = TPBRT_STRING("Lescale");
					static const tpbrt_string_t PRESET_STR		   = TPBRT_STRING("preset");
					static const tpbrt_string_t SIGMA_A_STR		   = TPBRT_STRING("sigma_a");
					static const tpbrt_string_t SIGMA_S_STR		   = TPBRT_STRING("sigma_s");
					static const tpbrt_string_t SCALE_STR		   = TPBRT_STRING("scale");

					tpbrt_media_homogeneous_params_t* media_params = &media->as.homogeneous;

					err = tpbrt_params_list_get_float(params, &G_STR, 0.0f, &media_params->g);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_spectrum(params, &LE_STR, &media_params->Le);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							media_params->Le.type		  = TPBRT_SPECTRUM_TYPE_BLACKBODY;
							media_params->Le.as.blackbody = 0u;
						}

					err = tpbrt_params_list_get_float(params, &LE_SCALE_STR, 1.0f, &media_params->Le_scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_string(params, &PRESET_STR, &media_params->preset);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_spectrum(params, &SIGMA_A_STR, &media_params->sigma_a);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							media_params->sigma_a.type		   = TPBRT_SPECTRUM_TYPE_BLACKBODY;
							media_params->sigma_a.as.blackbody = 1u;
						}

					err = tpbrt_params_list_get_spectrum(params, &SIGMA_S_STR, &media_params->sigma_s);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							media_params->sigma_s.type		   = TPBRT_SPECTRUM_TYPE_BLACKBODY;
							media_params->sigma_s.as.blackbody = 1u;
						}

					err = tpbrt_params_list_get_float(params, &SCALE_STR, 1.0f, &media_params->scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					break;
				}
				case TPBRT_MEDIA_TYPE_NANO_VDB: {
					static const tpbrt_string_t G_STR				   = TPBRT_STRING("g");
					static const tpbrt_string_t LE_SCALE_STR		   = TPBRT_STRING("Lescale");
					static const tpbrt_string_t SIGMA_A_STR			   = TPBRT_STRING("sigma_a");
					static const tpbrt_string_t SIGMA_S_STR			   = TPBRT_STRING("sigma_s");
					static const tpbrt_string_t SCALE_STR			   = TPBRT_STRING("scale");
					static const tpbrt_string_t FILE_NAME_STR		   = TPBRT_STRING("filename");
					static const tpbrt_string_t TEMPERATURE_OFFSET_STR = TPBRT_STRING("temepratureoffset");
					static const tpbrt_string_t TEMPERATURE_SCALE_STR  = TPBRT_STRING("temperaturescale");

					tpbrt_media_nano_vdb_params_t* media_params		   = &media->as.nano_vdb;

					err = tpbrt_params_list_get_float(params, &G_STR, 0.0f, &media_params->g);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &LE_SCALE_STR, 1.0f, &media_params->Le_scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_spectrum(params, &SIGMA_A_STR, &media_params->sigma_a);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							media_params->sigma_a.type		   = TPBRT_SPECTRUM_TYPE_BLACKBODY;
							media_params->sigma_a.as.blackbody = 1u;
						}

					err = tpbrt_params_list_get_spectrum(params, &SIGMA_S_STR, &media_params->sigma_s);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							media_params->sigma_s.type		   = TPBRT_SPECTRUM_TYPE_BLACKBODY;
							media_params->sigma_s.as.blackbody = 1u;
						}

					err = tpbrt_params_list_get_float(params, &SCALE_STR, 1.0f, &media_params->scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_string(params, &FILE_NAME_STR, &media_params->file_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &TEMPERATURE_OFFSET_STR, 0.0f, &media_params->temperature_offset);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &TEMPERATURE_SCALE_STR, 1.0f, &media_params->temperature_scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					break;
				}
				case TPBRT_MEDIA_TYPE_RGB_GRID: {
					static const tpbrt_string_t G_STR			= TPBRT_STRING("g");
					static const tpbrt_string_t LE_STR			= TPBRT_STRING("Le");
					static const tpbrt_string_t LE_SCALE_STR	= TPBRT_STRING("Lescale");
					static const tpbrt_string_t P0_STR			= TPBRT_STRING("p0");
					static const tpbrt_string_t P1_STR			= TPBRT_STRING("p1");
					static const tpbrt_string_t SIGMA_A_STR		= TPBRT_STRING("sigma_a");
					static const tpbrt_string_t SIGMA_S_STR		= TPBRT_STRING("sigma_s");
					static const tpbrt_string_t SCALE_STR		= TPBRT_STRING("scale");

					tpbrt_media_rgb_grid_params_t* media_params = &media->as.rgb_grid;

					err = tpbrt_params_list_get_float(params, &G_STR, 0.0f, &media_params->g);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_rgbs(params, &LE_STR, &media_params->Le);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &LE_SCALE_STR, 1.0f, &media_params->Le_scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_point3(params, &P0_STR, &media_params->p0);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							media_params->p0.x = 0.0f;
							media_params->p0.y = 0.0f;
							media_params->p0.z = 0.0f;
						}

					err = tpbrt_params_list_get_point3(params, &P1_STR, &media_params->p1);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							media_params->p1.x = 1.0f;
							media_params->p1.y = 1.0f;
							media_params->p1.z = 1.0f;
						}

					err = tpbrt_params_list_get_rgbs(params, &SIGMA_A_STR, &media_params->sigma_a);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_rgbs(params, &SIGMA_S_STR, &media_params->sigma_s);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &SCALE_STR, 1.0f, &media_params->scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					break;
				}
				case TPBRT_MEDIA_TYPE_UNIFORM_GRID: {
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

					tpbrt_media_uniform_grid_params_t* media_params	   = &media->as.uniform_grid;

					err = tpbrt_params_list_get_floats(params, &DENSITY_STR, &media_params->density);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &G_STR, 0.0f, &media_params->g);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_spectrum(params, &LE_STR, &media_params->Le);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							media_params->Le.type		  = TPBRT_SPECTRUM_TYPE_BLACKBODY;
							media_params->Le.as.blackbody = 0u;
						}

					err = tpbrt_params_list_get_float(params, &LE_SCALE_STR, 1.0f, &media_params->Le_scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_point3(params, &P0_STR, &media_params->p0);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							media_params->p0.x = 0.0f;
							media_params->p0.y = 0.0f;
							media_params->p0.z = 0.0f;
						}

					err = tpbrt_params_list_get_point3(params, &P1_STR, &media_params->p0);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							media_params->p1.x = 1.0f;
							media_params->p1.y = 1.0f;
							media_params->p1.z = 1.0f;
						}

					err = tpbrt_params_list_get_uint(params, &NX_STR, 1u, &media_params->nx);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_uint(params, &NY_STR, 1u, &media_params->ny);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_uint(params, &NZ_STR, 1u, &media_params->nz);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_string(params, &PRESET_STR, &media_params->preset);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_spectrum(params, &SIGMA_A_STR, &media_params->sigma_a);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							media_params->sigma_a.type		   = TPBRT_SPECTRUM_TYPE_BLACKBODY;
							media_params->sigma_a.as.blackbody = 1u;
						}

					err = tpbrt_params_list_get_spectrum(params, &SIGMA_S_STR, &media_params->sigma_s);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							media_params->sigma_s.type		   = TPBRT_SPECTRUM_TYPE_BLACKBODY;
							media_params->sigma_s.as.blackbody = 1u;
						}

					err = tpbrt_params_list_get_float(params, &SCALE_STR, 1.0f, &media_params->scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_floats(params, &TEMPERATURE_STR, &media_params->temperature);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &TEMPERATURE_OFFSET_STR, 0.0f, &media_params->temperature_offset);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &TEMPERATURE_SCALE_STR, 1.0f, &media_params->temperature_scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_media(media);
							return err;
						}

					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_media(tpbrt_media_t* const media) {
			if (media == TPBRT_NULL) { return; }

		tpbrt_free_string(&media->name);

			switch (media->type) {
				case TPBRT_MEDIA_TYPE_CLOUD: {
					tpbrt_media_cloud_params_t* const params = &media->as.cloud;

					tpbrt_free_spectrum(&params->sigma_a);
					tpbrt_free_spectrum(&params->sigma_s);
				}
				case TPBRT_MEDIA_TYPE_HOMOGENEOUS: {
					tpbrt_media_homogeneous_params_t* const params = &media->as.homogeneous;

					tpbrt_free_spectrum(&params->Le);
					tpbrt_free_spectrum(&params->sigma_a);
					tpbrt_free_spectrum(&params->sigma_s);

					tpbrt_free_string(&params->preset);
				}
				case TPBRT_MEDIA_TYPE_NANO_VDB: {
					tpbrt_media_nano_vdb_params_t* const params = &media->as.nano_vdb;

					tpbrt_free_spectrum(&params->sigma_a);
					tpbrt_free_spectrum(&params->sigma_s);

					tpbrt_free_string(&params->file_name);
				}
				case TPBRT_MEDIA_TYPE_RGB_GRID: {
					tpbrt_media_rgb_grid_params_t* const params = &media->as.rgb_grid;

					tpbrt_free_rgb_array(&params->Le);
					tpbrt_free_rgb_array(&params->sigma_a);
					tpbrt_free_rgb_array(&params->sigma_s);
				}
				case TPBRT_MEDIA_TYPE_UNIFORM_GRID: {
					tpbrt_media_uniform_grid_params_t* const params = &media->as.uniform_grid;

					tpbrt_free_float_array(&params->density);

					tpbrt_free_spectrum(&params->Le);

					tpbrt_free_string(&params->preset);

					tpbrt_free_spectrum(&params->sigma_a);
					tpbrt_free_spectrum(&params->sigma_s);

					tpbrt_free_float_array(&params->temperature);
				}
			default: break;
			}
	}

#pragma endregion

#pragma region MEDIAS_LIST

	tpbrt_error_t tpbrt_init_medias_list(tpbrt_medias_list_t* const medias_list) {
			if (medias_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		medias_list->medias = TPBRT_NULL;
		medias_list->count	= 0;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_medias_list_add_media(tpbrt_medias_list_t* const medias_list, const tpbrt_media_t* const media) {
			if (media == TPBRT_NULL || medias_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (medias_list->medias == TPBRT_NULL) {
				medias_list->medias = malloc(sizeof(tpbrt_media_t));
					if (medias_list->medias == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

				medias_list->medias[0]	   = *media;
				medias_list->medias[0].idx = 0;
				medias_list->count		   = 1;
				return TPBRT_ERROR_NONE;
			}

			if (media->name.size != 0 && media->name.data != TPBRT_NULL) {
					for (tpbrt_size_t i = 0; i < medias_list->count; i++) {
							if (tpbrt_string_equals(&medias_list->medias[i].name, &media->name)) {
								return TPBRT_ERROR_DUPLICATE_TEXTURE_NAME;
							}
					}
			}

		tpbrt_media_t* new_list = malloc(sizeof(tpbrt_media_t) * (medias_list->count + 1));
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

			for (tpbrt_size_t i = 0; i < medias_list->count; i++) { new_list[i] = medias_list->medias[i]; }
		new_list[medias_list->count]	 = *media;
		new_list[medias_list->count].idx = medias_list->count;
		++medias_list->count;
		free(medias_list->medias);
		medias_list->medias = new_list;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_medias_list_get_media(const tpbrt_medias_list_t* const medias_list,
	  const tpbrt_string_t* const media_name, const tpbrt_media_t** const media) {
			if (medias_list == TPBRT_NULL || media_name == TPBRT_NULL || media_name->data == TPBRT_NULL || media == TPBRT_NULL) {
				*media = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_size_t i = 0; i < medias_list->count; i++) {
					if (tpbrt_string_equals(&medias_list->medias[i].name, media_name)) {
						*media = &medias_list->medias[i];
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_NOT_FOUND;
	}

	tpbrt_error_t tpbrt_medias_list_get_media_handle(const tpbrt_medias_list_t* const medias_list,
	  const tpbrt_string_t* const media_name, tpbrt_media_handle_t* const media_handle) {
			if (medias_list == TPBRT_NULL || media_name == TPBRT_NULL || media_name->data == TPBRT_NULL ||
				media_handle == TPBRT_NULL) {
				*media_handle = TPBRT_MEDIA_HANDLE_INVALID;
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_media_t* object;
		const tpbrt_error_t err = tpbrt_medias_list_get_media(medias_list, media_name, &object);

			if (err != TPBRT_ERROR_NONE) {
				*media_handle = TPBRT_MEDIA_HANDLE_INVALID;
				return err;
			}

		*media_handle = object->idx;
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_medias_list(tpbrt_medias_list_t* const medias_list) {
			if (medias_list == TPBRT_NULL || medias_list->medias == TPBRT_NULL) { return; }

			for (tpbrt_size_t i = 0; i < medias_list->count; ++i) { tpbrt_free_media(medias_list->medias + i); }

		free(medias_list->medias);
		medias_list->medias = TPBRT_NULL;
		medias_list->count	= 0;
	}

	tpbrt_size_t tpbrt_medias_list_size(const tpbrt_medias_list_t* const medias_list) {
		return medias_list != TPBRT_NULL ? medias_list->count : 0;
	}

	tpbrt_bool_t tpbrt_medias_list_is_empty(const tpbrt_medias_list_t* const medias_list) {
		return tpbrt_medias_list_size(medias_list) == 0;
	}

#pragma endregion

#pragma region API

	tpbrt_error_t tpbrt_get_media_by_name(const tpbrt_medias_list_t* const medias, const tpbrt_string_t* const name,
	  const tpbrt_media_t** const media) {
		return tpbrt_medias_list_get_media(medias, name, media);
	}

	tpbrt_error_t tpbrt_get_media_by_handle(const tpbrt_medias_list_t* const medias, const tpbrt_media_handle_t handle,
	  const tpbrt_media_t** const media) {
			if (medias == TPBRT_NULL || media == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (handle == TPBRT_MEDIA_HANDLE_INVALID || handle >= medias->count) {
				*media = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_HANDLE;
			}

		*media = &medias->medias[handle];
		return TPBRT_ERROR_NONE;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif