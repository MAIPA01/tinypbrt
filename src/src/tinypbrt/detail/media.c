#include <tinypbrt/pch.h>

#include <tinypbrt/detail/media_internal.h>

#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/name_index_internal.h>
#include <tinypbrt/detail/param_internal.h>
#include <tinypbrt/detail/scattering_internal.h>

/**
 * @defgroup media-private Media
 * @ingroup tinypbrt-private
 */

#pragma region MEDIA

tpbrt_error_t tpbrt_media_type_from_string(const tpbrt_string_t type_str, tpbrt_media_type_t* const type) {
	static const tpbrt_string_t TypesStrs[TPBRT_MEDIA_TYPE_MAX_NUM] = {
		TPBRT_STRING_INIT("cloud"),
		TPBRT_STRING_INIT("homogeneous"),
		TPBRT_STRING_INIT("nanovdb"),
		TPBRT_STRING_INIT("rgbgrid"),
		TPBRT_STRING_INIT("uniformgrid"),
	};

		if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (tpbrt_string_empty(type_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_MEDIA_TYPE; }

		for (tpbrt_media_type_t t = 0; t < TPBRT_MEDIA_TYPE_MAX_NUM; ++t) {
				if (tpbrt_string_equals(type_str, TypesStrs[t]) == TPBRT_TRUE) {
					*type = t;
					return TPBRT_ERROR_NONE;
				}
		}

	return TPBRT_ERROR_INVALID_MEDIA_TYPE;
}

/**
 * @brief Reads scattering parameters.
 * @param scattering Scattering data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_SCATTERING_BUILTIN, TPBRT_ERROR_PARAM_TYPE_MISMATCH, TPBRT_ERROR_NONE).
 * @ingroup media-private
 */
static tpbrt_error_t tpbrt_media_read_scattering(tpbrt_media_scattering_t* const scattering,
  const tpbrt_params_list_t* const params) {
		if (scattering == TPBRT_NULL || params == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t PresetStr		= TPBRT_STRING_INIT("preset");
	static const tpbrt_string_t SigmaAStr		= TPBRT_STRING_INIT("sigma_a");
	static const tpbrt_string_t SigmaSStr		= TPBRT_STRING_INIT("sigma_s");

	static const tpbrt_string_t PresetDefault	= TPBRT_EMPTY_STRING_INIT;
	static const tpbrt_spectrum_t SigmaADefault = TPBRT_SPECTRUM_CONSTANT_INIT(1.0f);
	static const tpbrt_spectrum_t SigmaSDefault = TPBRT_SPECTRUM_CONSTANT_INIT(1.0f);

	// get_string returns the default ("") with TPBRT_ERROR_NONE when "preset" is missing - like pbrt-v4,
	// an empty preset means "no preset", so sigma_a / sigma_s are read instead
	tpbrt_string_t preset_str;
	tpbrt_error_t err = tpbrt_params_list_get_string(params, PresetStr, PresetDefault, &preset_str);
		if (err != TPBRT_ERROR_NONE) { return err; }

		if (tpbrt_string_empty(preset_str) == TPBRT_FALSE) {
			scattering->type = TPBRT_MEDIA_SCATTERING_VALUE_PRESET;

			err				 = tpbrt_scattering_builtin_from_string(preset_str, &scattering->as.preset);
			tpbrt_string_free(&preset_str);
			return err;
		}
	tpbrt_string_free(&preset_str);

	scattering->type = TPBRT_MEDIA_SCATTERING_VALUE_SIGMA;

	err				 = tpbrt_params_list_get_spectrum(params, SigmaAStr, &SigmaADefault, &scattering->as.sigma.a);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_spectrum(params, SigmaSStr, &SigmaSDefault, &scattering->as.sigma.s);
}

/**
 * @brief Reads cloud parameters
 * @param cloud Cloud data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup media-private
 */
static tpbrt_error_t tpbrt_media_read_cloud(tpbrt_media_cloud_params_t* const cloud, const tpbrt_params_list_t* const params) {
		if (cloud == TPBRT_NULL || params == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t DensityStr		= TPBRT_STRING_INIT("density");
	static const tpbrt_string_t FrequencyStr	= TPBRT_STRING_INIT("frequency");
	static const tpbrt_string_t GStr			= TPBRT_STRING_INIT("g");
	static const tpbrt_string_t P0Str			= TPBRT_STRING_INIT("p0");
	static const tpbrt_string_t P1Str			= TPBRT_STRING_INIT("p1");
	static const tpbrt_string_t SigmaAStr		= TPBRT_STRING_INIT("sigma_a");
	static const tpbrt_string_t SigmaSStr		= TPBRT_STRING_INIT("sigma_s");
	static const tpbrt_string_t WispinessStr	= TPBRT_STRING_INIT("wispiness");

	static const tpbrt_float_t DensityDefault	= 1.0f;
	static const tpbrt_float_t FrequencyDefault = 5.0f;
	static const tpbrt_float_t GDefault			= 0.0f;
	static const tpbrt_vec3_t P0Default			= TPBRT_VEC3_INIT(0.0f, 0.0f, 0.0f);
	static const tpbrt_vec3_t P1Default			= TPBRT_VEC3_INIT(1.0f, 1.0f, 1.0f);
	static const tpbrt_spectrum_t SigmaADefault = TPBRT_SPECTRUM_CONSTANT_INIT(1.0f);
	static const tpbrt_spectrum_t SigmaSDefault = TPBRT_SPECTRUM_CONSTANT_INIT(1.0f);
	static const tpbrt_float_t WispinessDefault = 1.0f;

	tpbrt_error_t err = tpbrt_params_list_get_float(params, DensityStr, DensityDefault, &cloud->density);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, FrequencyStr, FrequencyDefault, &cloud->frequency);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, GStr, GDefault, &cloud->g);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_point3(params, P0Str, P0Default, &cloud->p0);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_point3(params, P1Str, P1Default, &cloud->p1);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_spectrum(params, SigmaAStr, &SigmaADefault, &cloud->sigma_a);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_spectrum(params, SigmaSStr, &SigmaSDefault, &cloud->sigma_s);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_float(params, WispinessStr, WispinessDefault, &cloud->wispiness);
}

/**
 * @brief Reads homogeneous parameters.
 * @param homogeneous Homogeneous data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup media-private
 */
static tpbrt_error_t tpbrt_media_read_homogeneous(tpbrt_media_homogeneous_params_t* const homogeneous,
  const tpbrt_params_list_t* const params) {
		if (homogeneous == TPBRT_NULL || params == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t GStr		  = TPBRT_STRING_INIT("g");
	static const tpbrt_string_t LeStr		  = TPBRT_STRING_INIT("Le");
	static const tpbrt_string_t LeScaleStr	  = TPBRT_STRING_INIT("Lescale");
	static const tpbrt_string_t ScaleStr	  = TPBRT_STRING_INIT("scale");

	static const tpbrt_float_t GDefault		  = 0.0f;
	static const tpbrt_spectrum_t LeDefault	  = TPBRT_SPECTRUM_CONSTANT_INIT(0.0f);
	static const tpbrt_float_t LeScaleDefault = 1.0f;
	static const tpbrt_float_t ScaleDefault	  = 1.0f;

	tpbrt_error_t err						  = tpbrt_params_list_get_float(params, GStr, GDefault, &homogeneous->g);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_spectrum(params, LeStr, &LeDefault, &homogeneous->le);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, LeScaleStr, LeScaleDefault, &homogeneous->le_scale);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_media_read_scattering(&homogeneous->scattering, params);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_float(params, ScaleStr, ScaleDefault, &homogeneous->scale);
}

/**
 * @brief Reads nano vdb parameters.
 * @param nano_vdb Nano vdb data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup media-private
 */
static tpbrt_error_t tpbrt_media_read_nano_vdb(tpbrt_media_nano_vdb_params_t* const nano_vdb,
  const tpbrt_params_list_t* const params) {
		if (nano_vdb == TPBRT_NULL || params == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t GStr					= TPBRT_STRING_INIT("g");
	static const tpbrt_string_t LeScaleStr				= TPBRT_STRING_INIT("Lescale");
	static const tpbrt_string_t SigmaAStr				= TPBRT_STRING_INIT("sigma_a");
	static const tpbrt_string_t SigmaSStr				= TPBRT_STRING_INIT("sigma_s");
	static const tpbrt_string_t ScaleStr				= TPBRT_STRING_INIT("scale");
	static const tpbrt_string_t FileNameStr				= TPBRT_STRING_INIT("filename");
	static const tpbrt_string_t TemperatureOffsetStr	= TPBRT_STRING_INIT("temperatureoffset");
	static const tpbrt_string_t TemperatureScaleStr		= TPBRT_STRING_INIT("temperaturescale");

	static const tpbrt_float_t GDefault					= 0.0f;
	static const tpbrt_float_t LeScaleDefault			= 1.0f;
	static const tpbrt_spectrum_t SigmaADefault			= TPBRT_SPECTRUM_CONSTANT_INIT(1.0f);
	static const tpbrt_spectrum_t SigmaSDefault			= TPBRT_SPECTRUM_CONSTANT_INIT(1.0f);
	static const tpbrt_float_t ScaleDefault				= 1.0f;
	static const tpbrt_string_t FileNameDefault			= TPBRT_EMPTY_STRING_INIT;
	static const tpbrt_float_t TemperatureOffsetDefault = 0.0f;
	static const tpbrt_float_t TemperatureScaleDefault	= 1.0f;

	tpbrt_error_t err									= tpbrt_params_list_get_float(params, GStr, GDefault, &nano_vdb->g);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, LeScaleStr, LeScaleDefault, &nano_vdb->le_scale);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_spectrum(params, SigmaAStr, &SigmaADefault, &nano_vdb->sigma_a);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_spectrum(params, SigmaSStr, &SigmaSDefault, &nano_vdb->sigma_s);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, ScaleStr, ScaleDefault, &nano_vdb->scale);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_string(params, FileNameStr, FileNameDefault, &nano_vdb->file_name);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, TemperatureOffsetStr, TemperatureOffsetDefault, &nano_vdb->temperature_offset);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_float(params, TemperatureScaleStr, TemperatureScaleDefault, &nano_vdb->temperature_scale);
}

/**
 * @brief Reads rgb grid parameters.
 * @param rgb_grid Rgb grid data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup media-private
 */
static tpbrt_error_t tpbrt_media_read_rgb_grid(tpbrt_media_rgb_grid_params_t* const rgb_grid,
  const tpbrt_params_list_t* const params) {
		if (rgb_grid == TPBRT_NULL || params == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t GStr		  = TPBRT_STRING_INIT("g");
	static const tpbrt_string_t LeStr		  = TPBRT_STRING_INIT("Le");
	static const tpbrt_string_t LeScaleStr	  = TPBRT_STRING_INIT("Lescale");
	static const tpbrt_string_t P0Str		  = TPBRT_STRING_INIT("p0");
	static const tpbrt_string_t P1Str		  = TPBRT_STRING_INIT("p1");
	static const tpbrt_string_t SigmaAStr	  = TPBRT_STRING_INIT("sigma_a");
	static const tpbrt_string_t SigmaSStr	  = TPBRT_STRING_INIT("sigma_s");
	static const tpbrt_string_t ScaleStr	  = TPBRT_STRING_INIT("scale");
	static const tpbrt_string_t NXStr		  = TPBRT_STRING_INIT("nx");
	static const tpbrt_string_t NYStr		  = TPBRT_STRING_INIT("ny");
	static const tpbrt_string_t NZStr		  = TPBRT_STRING_INIT("nz");

	static const tpbrt_float_t GDefault		  = 0.0f;
	static const tpbrt_float_t LeScaleDefault = 1.0f;
	static const tpbrt_vec3_t P0Default		  = TPBRT_VEC3_INIT(0.0f, 0.0f, 0.0f);
	static const tpbrt_vec3_t P1Default		  = TPBRT_VEC3_INIT(1.0f, 1.0f, 1.0f);
	static const tpbrt_float_t ScaleDefault	  = 1.0f;
	static const tpbrt_uint_t NXDefault		  = 1u;
	static const tpbrt_uint_t NYDefault		  = 1u;
	static const tpbrt_uint_t NZDefault		  = 1u;

	tpbrt_error_t err						  = tpbrt_params_list_get_float(params, GStr, GDefault, &rgb_grid->g);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_rgbs(params, LeStr, &rgb_grid->le);
		if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

	err = tpbrt_params_list_get_float(params, LeScaleStr, LeScaleDefault, &rgb_grid->le_scale);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_point3(params, P0Str, P0Default, &rgb_grid->p0);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_point3(params, P1Str, P1Default, &rgb_grid->p1);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_rgbs(params, SigmaAStr, &rgb_grid->sigma_a);
		if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

	err = tpbrt_params_list_get_rgbs(params, SigmaSStr, &rgb_grid->sigma_s);
		if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

	err = tpbrt_params_list_get_float(params, ScaleStr, ScaleDefault, &rgb_grid->scale);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_uint(params, NXStr, NXDefault, &rgb_grid->nx);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_uint(params, NYStr, NYDefault, &rgb_grid->ny);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_uint(params, NZStr, NZDefault, &rgb_grid->nz);
		if (err != TPBRT_ERROR_NONE) { return err; }

	const tpbrt_size_t voxels = (tpbrt_size_t)rgb_grid->nx * rgb_grid->ny * rgb_grid->nz;
		if ((rgb_grid->le.count != 0u && rgb_grid->le.count != voxels) ||
			(rgb_grid->sigma_a.count != 0u && rgb_grid->sigma_a.count != voxels) ||
			(rgb_grid->sigma_s.count != 0u && rgb_grid->sigma_s.count != voxels)) {
			return TPBRT_ERROR_INDEX_OUT_OF_RANGE;
		}
	return TPBRT_ERROR_NONE;
}

/**
 * @brief Reads uniform grid parameters.
 * @param uniform_grid Uniform grid data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup media-private
 */
static tpbrt_error_t tpbrt_media_read_uniform_grid(tpbrt_media_uniform_grid_params_t* const uniform_grid,
  const tpbrt_params_list_t* const params) {
		if (uniform_grid == TPBRT_NULL || params == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t DensityStr				= TPBRT_STRING_INIT("density");
	static const tpbrt_string_t GStr					= TPBRT_STRING_INIT("g");
	static const tpbrt_string_t LeStr					= TPBRT_STRING_INIT("Le");
	static const tpbrt_string_t LeScaleStr				= TPBRT_STRING_INIT("Lescale");
	static const tpbrt_string_t P0Str					= TPBRT_STRING_INIT("p0");
	static const tpbrt_string_t P1Str					= TPBRT_STRING_INIT("p1");
	static const tpbrt_string_t NXStr					= TPBRT_STRING_INIT("nx");
	static const tpbrt_string_t NYStr					= TPBRT_STRING_INIT("ny");
	static const tpbrt_string_t NZStr					= TPBRT_STRING_INIT("nz");
	static const tpbrt_string_t ScaleStr				= TPBRT_STRING_INIT("scale");
	static const tpbrt_string_t TemperatureStr			= TPBRT_STRING_INIT("temperature");
	static const tpbrt_string_t TemperatureOffsetStr	= TPBRT_STRING_INIT("temperatureoffset");
	static const tpbrt_string_t TemperatureScaleStr		= TPBRT_STRING_INIT("temperaturescale");

	static const tpbrt_float_t GDefault					= 0.0f;
	static const tpbrt_spectrum_t LeDefault				= TPBRT_SPECTRUM_CONSTANT_INIT(0.0f);
	static const tpbrt_float_t LeScaleDefault			= 1.0f;
	static const tpbrt_vec3_t P0Default					= TPBRT_VEC3_INIT(0.0f, 0.0f, 0.0f);
	static const tpbrt_vec3_t P1Default					= TPBRT_VEC3_INIT(1.0f, 1.0f, 1.0f);
	static const tpbrt_uint_t NXDefault					= 1u;
	static const tpbrt_uint_t NYDefault					= 1u;
	static const tpbrt_uint_t NZDefault					= 1u;
	static const tpbrt_float_t ScaleDefault				= 1.0f;
	static const tpbrt_float_t TemperatureOffsetDefault = 0.0f;
	static const tpbrt_float_t TemperatureScaleDefault	= 1.0f;

	tpbrt_error_t err = tpbrt_params_list_get_floats(params, DensityStr, &uniform_grid->density);
		if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

	err = tpbrt_params_list_get_float(params, GStr, GDefault, &uniform_grid->g);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_spectrum(params, LeStr, &LeDefault, &uniform_grid->le);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, LeScaleStr, LeScaleDefault, &uniform_grid->le_scale);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_point3(params, P0Str, P0Default, &uniform_grid->p0);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_point3(params, P1Str, P1Default, &uniform_grid->p1);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_uint(params, NXStr, NXDefault, &uniform_grid->nx);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_uint(params, NYStr, NYDefault, &uniform_grid->ny);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_uint(params, NZStr, NZDefault, &uniform_grid->nz);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_media_read_scattering(&uniform_grid->scattering, params);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, ScaleStr, ScaleDefault, &uniform_grid->scale);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_floats(params, TemperatureStr, &uniform_grid->temperature);
		if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

	err = tpbrt_params_list_get_float(params, TemperatureOffsetStr, TemperatureOffsetDefault, &uniform_grid->temperature_offset);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, TemperatureScaleStr, TemperatureScaleDefault, &uniform_grid->temperature_scale);
		if (err != TPBRT_ERROR_NONE) { return err; }

	const tpbrt_size_t voxels = (tpbrt_size_t)uniform_grid->nx * uniform_grid->ny * uniform_grid->nz;
		if ((uniform_grid->density.count != 0u && uniform_grid->density.count != voxels) ||
			(uniform_grid->temperature.count != 0u && uniform_grid->temperature.count != voxels)) {
			return TPBRT_ERROR_INDEX_OUT_OF_RANGE;
		}

	return TPBRT_ERROR_NONE;
}

tpbrt_error_t tpbrt_media_init(const tpbrt_string_t name, const tpbrt_string_t type_str, const tpbrt_params_list_t* const params,
  const tpbrt_transform_handle_t transform, tpbrt_media_t* const media) {
		if (media == TPBRT_NULL || params == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

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
			static const tpbrt_string_t TypeStr = TPBRT_STRING_INIT("type");
			err									= tpbrt_params_list_get_string_required(params, TypeStr, &temp_string);
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
					if (params->scattering.type == TPBRT_MEDIA_SCATTERING_VALUE_SIGMA) {
						tpbrt_spectrum_free(&params->scattering.as.sigma.a);
						tpbrt_spectrum_free(&params->scattering.as.sigma.s);
					}
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

					if (params->scattering.type == TPBRT_MEDIA_SCATTERING_VALUE_SIGMA) {
						tpbrt_spectrum_free(&params->scattering.as.sigma.a);
						tpbrt_spectrum_free(&params->scattering.as.sigma.s);
					}

				tpbrt_float_array_free(&params->temperature);
				break;
			}
		default: break;
		}
}

#pragma endregion

#pragma region MEDIAS_LIST
/**
 * @brief Initializes empty medias list with type conversion.
 * @ingroup media-private
 */
#define TPBRT_EMPTY_MEDIAS_LIST (tpbrt_medias_list_t) { .medias = TPBRT_NULL, .count = 0u }

tpbrt_error_t tpbrt_medias_list_init(tpbrt_medias_list_t* const medias_list) {
		if (medias_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	*medias_list = TPBRT_EMPTY_MEDIAS_LIST;
	return TPBRT_ERROR_NONE;
}

tpbrt_error_t tpbrt_medias_list_append(tpbrt_medias_list_t* const medias_list, const tpbrt_media_t* const media) {
		if (media == TPBRT_NULL || medias_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	tpbrt_media_t* const new_list = tpbrt_array_reserve_append(medias_list->medias, medias_list->count, sizeof(tpbrt_media_t));
		if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

	const tpbrt_size_t idx = medias_list->count;
	new_list[idx]		   = *media;
	new_list[idx].idx	   = (tpbrt_idx_t)idx;
	medias_list->medias	   = new_list;
	medias_list->count	   = idx + 1u;

	return tpbrt_name_index_insert(&medias_list->names, &(TPBRT_NAME_INDEX_VIEW(new_list, idx + 1u, tpbrt_media_t)),
	  (tpbrt_idx_t)idx);
}

tpbrt_error_t tpbrt_medias_list_get(const tpbrt_medias_list_t* const medias_list, const tpbrt_string_t media_name,
  const tpbrt_media_t** const media) {
		if (media == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	const tpbrt_idx_t idx = tpbrt_name_index_find(&medias_list->names,
	  &(TPBRT_NAME_INDEX_VIEW(medias_list->medias, medias_list->count, tpbrt_media_t)), media_name, TPBRT_NULL, TPBRT_NULL);
		if (idx == TPBRT_IDX_INVALID) {
			*media = TPBRT_NULL;
			return TPBRT_ERROR_NOT_FOUND;
		}

	*media = &medias_list->medias[idx];
	return TPBRT_ERROR_NONE;
}

tpbrt_error_t tpbrt_medias_list_shrink_to_fit(tpbrt_medias_list_t* const medias_list) {
		if (medias_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	medias_list->medias = tpbrt_array_shrink_to_fit(medias_list->medias, medias_list->count, sizeof(tpbrt_media_t));
	return tpbrt_name_index_shrink(&medias_list->names,
	  &(TPBRT_NAME_INDEX_VIEW(medias_list->medias, medias_list->count, tpbrt_media_t)));
}

tpbrt_error_t tpbrt_medias_list_get_handle(const tpbrt_medias_list_t* const medias_list, const tpbrt_string_t media_name,
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
		if (medias_list == TPBRT_NULL) { return; }

	tpbrt_name_index_free(&medias_list->names);

		if (medias_list->medias == TPBRT_NULL) { return; }

		for (tpbrt_size_t i = 0u; i < medias_list->count; ++i) { tpbrt_media_free(medias_list->medias + i); }

	free(medias_list->medias);
	*medias_list = TPBRT_EMPTY_MEDIAS_LIST;
}

tpbrt_bool_t tpbrt_medias_list_empty(const tpbrt_medias_list_t* const medias_list) {
	return medias_list->medias == TPBRT_NULL || medias_list->count == 0u;
}

#pragma endregion

#pragma region API

tpbrt_error_t tpbrt_get_media_by_name(const tpbrt_medias_list_t* const medias, const tpbrt_string_t name,
  const tpbrt_media_t** const media) {
	return tpbrt_medias_list_get(medias, name, media);
}

tpbrt_error_t tpbrt_get_media_by_handle(const tpbrt_medias_list_t* const medias, const tpbrt_media_handle_t handle,
  const tpbrt_media_t** const media) {
		if (media == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (handle.media_idx == TPBRT_MEDIA_IDX_INVALID || handle.media_idx >= medias->count) {
			*media = TPBRT_NULL;
			return TPBRT_ERROR_INVALID_HANDLE;
		}

	*media = medias->medias + handle.media_idx;
	return TPBRT_ERROR_NONE;
}

#pragma endregion
