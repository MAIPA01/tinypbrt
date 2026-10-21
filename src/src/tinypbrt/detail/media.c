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

		static const tpbrt_string_t DENSITY_STR		  = TPBRT_STRING("density");
		static const tpbrt_string_t FREQUENCY_STR	  = TPBRT_STRING("frequency");
		static const tpbrt_string_t G_STR			  = TPBRT_STRING("g");
		static const tpbrt_string_t P0_STR			  = TPBRT_STRING("p0");
		static const tpbrt_string_t P1_STR			  = TPBRT_STRING("p1");
		static const tpbrt_string_t SIGMA_A_STR		  = TPBRT_STRING("sigma_a");
		static const tpbrt_string_t SIGMA_S_STR		  = TPBRT_STRING("sigma_s");
		static const tpbrt_string_t WISPINESS_STR	  = TPBRT_STRING("wispiness");

		static const tpbrt_float_t DENSITY_DEFAULT	  = 1.0f;
		static const tpbrt_float_t FREQUENCY_DEFAULT  = 5.0f;
		static const tpbrt_float_t G_DEFAULT		  = 0.0f;
		static const tpbrt_vec3_t P0_DEFAULT		  = TPBRT_VEC3(0.0f, 0.0f, 0.0f);
		static const tpbrt_vec3_t P1_DEFAULT		  = TPBRT_VEC3(1.0f, 1.0f, 1.0f);
		static const tpbrt_spectrum_t SIGMA_A_DEFAULT = TPBRT_SPECTRUM_CONSTANT(1.0f);
		static const tpbrt_spectrum_t SIGMA_S_DEFAULT = TPBRT_SPECTRUM_CONSTANT(1.0f);
		static const tpbrt_float_t WISPINESS_DEFAULT  = 1.0f;

		tpbrt_error_t err = tpbrt_params_list_get_float(params, DENSITY_STR, DENSITY_DEFAULT, &cloud->density);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, FREQUENCY_STR, FREQUENCY_DEFAULT, &cloud->frequency);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, G_STR, G_DEFAULT, &cloud->g);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_point3(params, P0_STR, P0_DEFAULT, &cloud->p0);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_point3(params, P1_STR, P1_DEFAULT, &cloud->p1);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_spectrum(params, SIGMA_A_STR, SIGMA_A_DEFAULT, &cloud->sigma_a);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_spectrum(params, SIGMA_S_STR, SIGMA_S_DEFAULT, &cloud->sigma_s);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, WISPINESS_STR, WISPINESS_DEFAULT, &cloud->wispiness);
	}

	static tpbrt_error_t tpbrt_media_read_homogeneous(tpbrt_media_homogeneous_params_t* const homogeneous,
	  const tpbrt_params_list_t params) {
			if (homogeneous == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t G_STR			  = TPBRT_STRING("g");
		static const tpbrt_string_t LE_STR			  = TPBRT_STRING("Le");
		static const tpbrt_string_t LE_SCALE_STR	  = TPBRT_STRING("Lescale");
		static const tpbrt_string_t PRESET_STR		  = TPBRT_STRING("preset");
		static const tpbrt_string_t SIGMA_A_STR		  = TPBRT_STRING("sigma_a");
		static const tpbrt_string_t SIGMA_S_STR		  = TPBRT_STRING("sigma_s");
		static const tpbrt_string_t SCALE_STR		  = TPBRT_STRING("scale");

		static const tpbrt_float_t G_DEFAULT		  = 0.0f;
		static const tpbrt_spectrum_t LE_DEFAULT	  = TPBRT_SPECTRUM_CONSTANT(0.0f);
		static const tpbrt_float_t LE_SCALE_DEFAULT	  = 1.0f;
		static const tpbrt_string_t PRESET_DEFAULT	  = TPBRT_EMPTY_STRING;
		static const tpbrt_spectrum_t SIGMA_A_DEFAULT = TPBRT_SPECTRUM_CONSTANT(1.0f);
		static const tpbrt_spectrum_t SIGMA_S_DEFAULT = TPBRT_SPECTRUM_CONSTANT(1.0f);
		static const tpbrt_float_t SCALE_DEFAULT	  = 1.0f;

		tpbrt_error_t err							  = tpbrt_params_list_get_float(params, G_STR, G_DEFAULT, &homogeneous->g);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_spectrum(params, LE_STR, LE_DEFAULT, &homogeneous->le);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, LE_SCALE_STR, LE_SCALE_DEFAULT, &homogeneous->le_scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_string(params, PRESET_STR, PRESET_DEFAULT, &homogeneous->preset);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_spectrum(params, SIGMA_A_STR, SIGMA_A_DEFAULT, &homogeneous->sigma_a);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_spectrum(params, SIGMA_S_STR, SIGMA_S_DEFAULT, &homogeneous->sigma_s);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, SCALE_STR, SCALE_DEFAULT, &homogeneous->scale);
	}

	static tpbrt_error_t tpbrt_media_read_nano_vdb(tpbrt_media_nano_vdb_params_t* const nano_vdb,
	  const tpbrt_params_list_t params) {
			if (nano_vdb == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t G_STR					  = TPBRT_STRING("g");
		static const tpbrt_string_t LE_SCALE_STR			  = TPBRT_STRING("Lescale");
		static const tpbrt_string_t SIGMA_A_STR				  = TPBRT_STRING("sigma_a");
		static const tpbrt_string_t SIGMA_S_STR				  = TPBRT_STRING("sigma_s");
		static const tpbrt_string_t SCALE_STR				  = TPBRT_STRING("scale");
		static const tpbrt_string_t FILE_NAME_STR			  = TPBRT_STRING("filename");
		static const tpbrt_string_t TEMPERATURE_OFFSET_STR	  = TPBRT_STRING("temperatureoffset");
		static const tpbrt_string_t TEMPERATURE_SCALE_STR	  = TPBRT_STRING("temperaturescale");

		static const tpbrt_float_t G_DEFAULT				  = 0.0f;
		static const tpbrt_float_t LE_SCALE_DEFAULT			  = 1.0f;
		static const tpbrt_spectrum_t SIGMA_A_DEFAULT		  = TPBRT_SPECTRUM_CONSTANT(1.0f);
		static const tpbrt_spectrum_t SIGMA_S_DEFAULT		  = TPBRT_SPECTRUM_CONSTANT(1.0f);
		static const tpbrt_float_t SCALE_DEFAULT			  = 1.0f;
		static const tpbrt_string_t FILE_NAME_DEFAULT		  = TPBRT_EMPTY_STRING;
		static const tpbrt_float_t TEMPERATURE_OFFSET_DEFAULT = 0.0f;
		static const tpbrt_float_t TEMPERATURE_SCALE_DEFAULT  = 1.0f;

		tpbrt_error_t err = tpbrt_params_list_get_float(params, G_STR, G_DEFAULT, &nano_vdb->g);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, LE_SCALE_STR, LE_SCALE_DEFAULT, &nano_vdb->le_scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_spectrum(params, SIGMA_A_STR, SIGMA_A_DEFAULT, &nano_vdb->sigma_a);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_spectrum(params, SIGMA_S_STR, SIGMA_S_DEFAULT, &nano_vdb->sigma_s);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, SCALE_STR, SCALE_DEFAULT, &nano_vdb->scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_string(params, FILE_NAME_STR, FILE_NAME_DEFAULT, &nano_vdb->file_name);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err =
		  tpbrt_params_list_get_float(params, TEMPERATURE_OFFSET_STR, TEMPERATURE_OFFSET_DEFAULT, &nano_vdb->temperature_offset);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, TEMPERATURE_SCALE_STR, TEMPERATURE_SCALE_DEFAULT,
		  &nano_vdb->temperature_scale);
	}

	static tpbrt_error_t tpbrt_media_read_rgb_grid(tpbrt_media_rgb_grid_params_t* const rgb_grid,
	  const tpbrt_params_list_t params) {
			if (rgb_grid == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t G_STR			= TPBRT_STRING("g");
		static const tpbrt_string_t LE_STR			= TPBRT_STRING("Le");
		static const tpbrt_string_t LE_SCALE_STR	= TPBRT_STRING("Lescale");
		static const tpbrt_string_t P0_STR			= TPBRT_STRING("p0");
		static const tpbrt_string_t P1_STR			= TPBRT_STRING("p1");
		static const tpbrt_string_t SIGMA_A_STR		= TPBRT_STRING("sigma_a");
		static const tpbrt_string_t SIGMA_S_STR		= TPBRT_STRING("sigma_s");
		static const tpbrt_string_t SCALE_STR		= TPBRT_STRING("scale");
		static const tpbrt_string_t NX_STR			= TPBRT_STRING("nx");
		static const tpbrt_string_t NY_STR			= TPBRT_STRING("ny");
		static const tpbrt_string_t NZ_STR			= TPBRT_STRING("nz");

		static const tpbrt_float_t G_DEFAULT		= 0.0f;
		static const tpbrt_float_t LE_SCALE_DEFAULT = 1.0f;
		static const tpbrt_vec3_t P0_DEFAULT		= TPBRT_VEC3(0.0f, 0.0f, 0.0f);
		static const tpbrt_vec3_t P1_DEFAULT		= TPBRT_VEC3(1.0f, 1.0f, 1.0f);
		static const tpbrt_float_t SCALE_DEFAULT	= 1.0f;
		static const tpbrt_uint_t NX_DEFAULT		= 1u;
		static const tpbrt_uint_t NY_DEFAULT		= 1u;
		static const tpbrt_uint_t NZ_DEFAULT		= 1u;

		tpbrt_error_t err							= tpbrt_params_list_get_float(params, G_STR, G_DEFAULT, &rgb_grid->g);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_rgbs(params, LE_STR, &rgb_grid->le);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_float(params, LE_SCALE_STR, LE_SCALE_DEFAULT, &rgb_grid->le_scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_point3(params, P0_STR, P0_DEFAULT, &rgb_grid->p0);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_point3(params, P1_STR, P1_DEFAULT, &rgb_grid->p1);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_rgbs(params, SIGMA_A_STR, &rgb_grid->sigma_a);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_rgbs(params, SIGMA_S_STR, &rgb_grid->sigma_s);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_float(params, SCALE_STR, SCALE_DEFAULT, &rgb_grid->scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_uint(params, NX_STR, NX_DEFAULT, &rgb_grid->nx);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_uint(params, NY_STR, NY_DEFAULT, &rgb_grid->ny);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_uint(params, NZ_STR, NZ_DEFAULT, &rgb_grid->nz);
			if (err != TPBRT_ERROR_NONE) { return err; }

		const tpbrt_size_t voxels = (tpbrt_size_t)rgb_grid->nx * rgb_grid->ny * rgb_grid->nz;
			if ((rgb_grid->le.count != 0u && rgb_grid->le.count != voxels) ||
				(rgb_grid->sigma_a.count != 0u && rgb_grid->sigma_a.count != voxels) ||
				(rgb_grid->sigma_s.count != 0u && rgb_grid->sigma_s.count != voxels)) {
				return TPBRT_ERROR_INDEX_OUT_OF_RANGE;
			}
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_media_read_uniform_grid(tpbrt_media_uniform_grid_params_t* const uniform_grid,
	  const tpbrt_params_list_t params) {
			if (uniform_grid == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t DENSITY_STR				  = TPBRT_STRING("density");
		static const tpbrt_string_t G_STR					  = TPBRT_STRING("g");
		static const tpbrt_string_t LE_STR					  = TPBRT_STRING("Le");
		static const tpbrt_string_t LE_SCALE_STR			  = TPBRT_STRING("Lescale");
		static const tpbrt_string_t P0_STR					  = TPBRT_STRING("p0");
		static const tpbrt_string_t P1_STR					  = TPBRT_STRING("p1");
		static const tpbrt_string_t NX_STR					  = TPBRT_STRING("nx");
		static const tpbrt_string_t NY_STR					  = TPBRT_STRING("ny");
		static const tpbrt_string_t NZ_STR					  = TPBRT_STRING("nz");
		static const tpbrt_string_t PRESET_STR				  = TPBRT_STRING("preset");
		static const tpbrt_string_t SIGMA_A_STR				  = TPBRT_STRING("sigma_a");
		static const tpbrt_string_t SIGMA_S_STR				  = TPBRT_STRING("sigma_s");
		static const tpbrt_string_t SCALE_STR				  = TPBRT_STRING("scale");
		static const tpbrt_string_t TEMPERATURE_STR			  = TPBRT_STRING("temperature");
		static const tpbrt_string_t TEMPERATURE_OFFSET_STR	  = TPBRT_STRING("temperatureoffset");
		static const tpbrt_string_t TEMPERATURE_SCALE_STR	  = TPBRT_STRING("temperaturescale");

		static const tpbrt_float_t G_DEFAULT				  = 0.0f;
		static const tpbrt_spectrum_t LE_DEFAULT			  = TPBRT_SPECTRUM_CONSTANT(0.0f);
		static const tpbrt_float_t LE_SCALE_DEFAULT			  = 1.0f;
		static const tpbrt_vec3_t P0_DEFAULT				  = TPBRT_VEC3(0.0f, 0.0f, 0.0f);
		static const tpbrt_vec3_t P1_DEFAULT				  = TPBRT_VEC3(1.0f, 1.0f, 1.0f);
		static const tpbrt_uint_t NX_DEFAULT				  = 1u;
		static const tpbrt_uint_t NY_DEFAULT				  = 1u;
		static const tpbrt_uint_t NZ_DEFAULT				  = 1u;
		static const tpbrt_string_t PRESET_DEFAULT			  = TPBRT_EMPTY_STRING;
		static const tpbrt_spectrum_t SIGMA_A_DEFAULT		  = TPBRT_SPECTRUM_CONSTANT(1.0f);
		static const tpbrt_spectrum_t SIGMA_S_DEFAULT		  = TPBRT_SPECTRUM_CONSTANT(1.0f);
		static const tpbrt_float_t SCALE_DEFAULT			  = 1.0f;
		static const tpbrt_float_t TEMPERATURE_OFFSET_DEFAULT = 0.0f;
		static const tpbrt_float_t TEMPERATURE_SCALE_DEFAULT  = 1.0f;

		tpbrt_error_t err = tpbrt_params_list_get_floats(params, DENSITY_STR, &uniform_grid->density);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_float(params, G_STR, G_DEFAULT, &uniform_grid->g);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_spectrum(params, LE_STR, LE_DEFAULT, &uniform_grid->le);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, LE_SCALE_STR, LE_SCALE_DEFAULT, &uniform_grid->le_scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_point3(params, P0_STR, P0_DEFAULT, &uniform_grid->p0);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_point3(params, P1_STR, P1_DEFAULT, &uniform_grid->p1);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_uint(params, NX_STR, NX_DEFAULT, &uniform_grid->nx);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_uint(params, NY_STR, NY_DEFAULT, &uniform_grid->ny);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_uint(params, NZ_STR, NZ_DEFAULT, &uniform_grid->nz);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_string(params, PRESET_STR, PRESET_DEFAULT, &uniform_grid->preset);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_spectrum(params, SIGMA_A_STR, SIGMA_A_DEFAULT, &uniform_grid->sigma_a);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_spectrum(params, SIGMA_S_STR, SIGMA_S_DEFAULT, &uniform_grid->sigma_s);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, SCALE_STR, SCALE_DEFAULT, &uniform_grid->scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_floats(params, TEMPERATURE_STR, &uniform_grid->temperature);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_float(params, TEMPERATURE_OFFSET_STR, TEMPERATURE_OFFSET_DEFAULT,
		  &uniform_grid->temperature_offset);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err =
		  tpbrt_params_list_get_float(params, TEMPERATURE_SCALE_STR, TEMPERATURE_SCALE_DEFAULT, &uniform_grid->temperature_scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		const tpbrt_size_t voxels = (tpbrt_size_t)uniform_grid->nx * uniform_grid->ny * uniform_grid->nz;
			if ((uniform_grid->density.count != 0u && uniform_grid->density.count != voxels) ||
				(uniform_grid->temperature.count != 0u && uniform_grid->temperature.count != voxels)) {
				return TPBRT_ERROR_INDEX_OUT_OF_RANGE;
			}

		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_media_init(const tpbrt_string_t name, const tpbrt_string_t type_str, const tpbrt_params_list_t params,
	  const tpbrt_transform_handle_t transform, tpbrt_media_t* const media) {
			if (media == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(media, 0, sizeof(tpbrt_media_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		media->transform  = transform;

		tpbrt_error_t err = tpbrt_string_copy(&media->name, name);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_media_free(media);
				return err;
			}

		tpbrt_string_t temp_string	  = type_str;
		tpbrt_bool_t owns_temp_string = TPBRT_FALSE;
			if (tpbrt_string_empty(temp_string) == TPBRT_TRUE) {
				static const tpbrt_string_t TYPE_STR = TPBRT_STRING("type");
				err									 = tpbrt_params_list_get_string_required(params, TYPE_STR, &temp_string);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_media_free(media);
						return err;
					}
				owns_temp_string = TPBRT_TRUE;
			}

		tpbrt_media_type_t type;
		err = tpbrt_media_type_from_string(temp_string, &type);
			if (owns_temp_string == TPBRT_TRUE) { tpbrt_string_free(&temp_string); }
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

					tpbrt_spectrum_free(&params->le);
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

					tpbrt_rgb_array_free(&params->le);
					tpbrt_rgb_array_free(&params->sigma_a);
					tpbrt_rgb_array_free(&params->sigma_s);
					break;
				}
				case TPBRT_MEDIA_TYPE_UNIFORM_GRID: {
					tpbrt_media_uniform_grid_params_t* const params = &media->as.uniform_grid;

					tpbrt_float_array_free(&params->density);

					tpbrt_spectrum_free(&params->le);

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
#define TPBRT_EMPTY_MEDIAS_LIST (tpbrt_medias_list_t) { .medias = TPBRT_NULL, .count = 0u }

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
			if (media == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

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

			for (tpbrt_size_t i = 0u; i < medias_list->count; ++i) { tpbrt_media_free(medias_list->medias + i); }

		free(medias_list->medias);
		*medias_list = TPBRT_EMPTY_MEDIAS_LIST;
	}

	tpbrt_bool_t tpbrt_medias_list_empty(const tpbrt_medias_list_t medias_list) {
		return medias_list.medias == TPBRT_NULL || medias_list.count == 0u;
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