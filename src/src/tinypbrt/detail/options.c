#include <tinypbrt/pch.h>

#include <tinypbrt/detail/options_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/coord_internal.h>
#include <tinypbrt/detail/param_internal.h>

tpbrt_error_t tpbrt_options_init(tpbrt_options_t* const options) {
		if (options == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	*options = (tpbrt_options_t) {
		.disable_pixel_jitter		= TPBRT_FALSE,
		.disable_texture_filtering	= TPBRT_FALSE,
		.disable_wave_length_jitter = TPBRT_FALSE,
		.displacement_edge_scale	= 1,
		.mse_reference_image		= TPBRT_EMPTY_STRING,
		.mse_reference_out			= TPBRT_EMPTY_STRING,
		.render_coord_sys			= TPBRT_COORD_SYSTEM_TYPE_CAMERA_WORLD,
		.seed						= 0,
		.force_diffuse				= TPBRT_FALSE,
		.pixel_stats				= TPBRT_FALSE,
		.wavefront					= TPBRT_FALSE,
	};
	return TPBRT_ERROR_NONE;
}

/**
 * @brief Applies disable pixel jitter option.
 * @param options Options data.
 * @param param Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup options-private
 */
static tpbrt_error_t tpbrt_options_apply_disable_pixel_jitter(tpbrt_options_t* const options, const tpbrt_param_t* param) {
		if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	tpbrt_bool_t disable_pixel_jitter = TPBRT_FALSE;
	const tpbrt_error_t err			  = tpbrt_param_as_bool(param, &disable_pixel_jitter);
	options->disable_pixel_jitter	  = disable_pixel_jitter;
	return err;
}

/**
 * @brief Applies disable texture filtering option.
 * @param options Options data.
 * @param param Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup options-private
 */
static tpbrt_error_t tpbrt_options_apply_disable_texture_filtering(tpbrt_options_t* const options, const tpbrt_param_t* param) {
		if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	tpbrt_bool_t disable_texture_filtering = TPBRT_FALSE;
	const tpbrt_error_t err				   = tpbrt_param_as_bool(param, &disable_texture_filtering);
	options->disable_texture_filtering	   = disable_texture_filtering;
	return err;
}

/**
 * @brief Applies disable wave length jitter option.
 * @param options Options data.
 * @param param Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup options-private
 */
static tpbrt_error_t tpbrt_options_apply_disable_wave_length_jitter(tpbrt_options_t* const options, const tpbrt_param_t* param) {
		if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	tpbrt_bool_t disable_wave_length_jitter = TPBRT_FALSE;
	const tpbrt_error_t err					= tpbrt_param_as_bool(param, &disable_wave_length_jitter);
	options->disable_wave_length_jitter		= disable_wave_length_jitter;
	return err;
}

/**
 * @brief Applies displacement edge scale option.
 * @param options Options data.
 * @param param Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup options-private
 */
static tpbrt_error_t tpbrt_options_apply_displacement_edge_scale(tpbrt_options_t* const options, const tpbrt_param_t* param) {
		if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	return tpbrt_param_as_float(param, &options->displacement_edge_scale);
}

/**
 * @brief Applies mse reference image option.
 * @param options Options data.
 * @param param Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup options-private
 */
static tpbrt_error_t tpbrt_options_apply_mse_reference_image(tpbrt_options_t* const options, const tpbrt_param_t* param) {
		if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	return tpbrt_param_as_string(param, &options->mse_reference_image);
}

/**
 * @brief Applies mse reference out option.
 * @param options Options data.
 * @param param Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup options-private
 */
static tpbrt_error_t tpbrt_options_apply_mse_reference_out(tpbrt_options_t* const options, const tpbrt_param_t* param) {
		if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	return tpbrt_param_as_string(param, &options->mse_reference_out);
}

/**
 * @brief Applies render coord sys option.
 * @param options Options data.
 * @param param Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup options-private
 */
static tpbrt_error_t tpbrt_options_apply_render_coord_sys(tpbrt_options_t* const options, const tpbrt_param_t* param) {
		if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	tpbrt_string_t coord_str;
	tpbrt_error_t err = tpbrt_param_as_string(param, &coord_str);
		if (err != TPBRT_ERROR_NONE) { return err; }

	tpbrt_coord_system_type_t render_coord_sys = TPBRT_COORD_SYSTEM_TYPE_MAX_NUM;
	err										   = tpbrt_coord_system_type_from_string(coord_str, &render_coord_sys);
	tpbrt_string_free(&coord_str);
	options->render_coord_sys = render_coord_sys;
	return err;
}

/**
 * @brief Applies seed option.
 * @param options Options data.
 * @param param Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup options-private
 */
static tpbrt_error_t tpbrt_options_apply_seed(tpbrt_options_t* const options, const tpbrt_param_t* param) {
		if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	return tpbrt_param_as_int(param, &options->seed);
}

/**
 * @brief Applies force diffuse option.
 * @param options Options data.
 * @param param Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup options-private
 */
static tpbrt_error_t tpbrt_options_apply_force_diffuse(tpbrt_options_t* const options, const tpbrt_param_t* param) {
		if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	tpbrt_bool_t force_diffuse = TPBRT_FALSE;
	const tpbrt_error_t err	   = tpbrt_param_as_bool(param, &force_diffuse);
	options->force_diffuse	   = force_diffuse;
	return err;
}

/**
 * @brief Applies pixel stats option.
 * @param options Options data.
 * @param param Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup options-private
 */
static tpbrt_error_t tpbrt_options_apply_pixel_stats(tpbrt_options_t* const options, const tpbrt_param_t* param) {
		if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	tpbrt_bool_t pixel_stats = TPBRT_FALSE;
	const tpbrt_error_t err	 = tpbrt_param_as_bool(param, &pixel_stats);
	options->pixel_stats	 = pixel_stats;
	return err;
}

/**
 * @brief Applies wavefront option.
 * @param options Options data.
 * @param param Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup options-private
 */
static tpbrt_error_t tpbrt_options_apply_wavefront(tpbrt_options_t* const options, const tpbrt_param_t* param) {
		if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	tpbrt_bool_t wavefront	= TPBRT_FALSE;
	const tpbrt_error_t err = tpbrt_param_as_bool(param, &wavefront);
	options->wavefront		= wavefront;
	return err;
}

tpbrt_error_t tpbrt_options_apply(tpbrt_options_t* const options, const tpbrt_param_t* param) {
		if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t DisablePixelJitter		= TPBRT_STRING_INIT("disablepixeljitter");
	static const tpbrt_string_t DisableTextureFiltering = TPBRT_STRING_INIT("disabletexturefiltering");
	static const tpbrt_string_t DisableWaveLengthJitter = TPBRT_STRING_INIT("disablewavelengthjitter");
	static const tpbrt_string_t DisplacementEdgeScale	= TPBRT_STRING_INIT("displacementedgescale");
	static const tpbrt_string_t MSEReferenceImage		= TPBRT_STRING_INIT("msereferenceimage");
	static const tpbrt_string_t MSEReferenceOut			= TPBRT_STRING_INIT("msereferenceout");
	static const tpbrt_string_t RenderCoordSys			= TPBRT_STRING_INIT("rendercoordsys");
	static const tpbrt_string_t Seed					= TPBRT_STRING_INIT("seed");
	static const tpbrt_string_t ForceDiffuse			= TPBRT_STRING_INIT("forcediffuse");
	static const tpbrt_string_t PixelStats				= TPBRT_STRING_INIT("pixelstats");
	static const tpbrt_string_t Wavefront				= TPBRT_STRING_INIT("wavefront");

		if (tpbrt_string_equals(param->name, DisablePixelJitter)) {
			return tpbrt_options_apply_disable_pixel_jitter(options, param);
		}

		if (tpbrt_string_equals(param->name, DisableTextureFiltering)) {
			return tpbrt_options_apply_disable_texture_filtering(options, param);
		}

		if (tpbrt_string_equals(param->name, DisableWaveLengthJitter)) {
			return tpbrt_options_apply_disable_wave_length_jitter(options, param);
		}

		if (tpbrt_string_equals(param->name, DisplacementEdgeScale)) {
			return tpbrt_options_apply_displacement_edge_scale(options, param);
		}


		if (tpbrt_string_equals(param->name, MSEReferenceImage)) {
			return tpbrt_options_apply_mse_reference_image(options, param);
		}


		if (tpbrt_string_equals(param->name, MSEReferenceOut)) { return tpbrt_options_apply_mse_reference_out(options, param); }


		if (tpbrt_string_equals(param->name, RenderCoordSys)) { return tpbrt_options_apply_render_coord_sys(options, param); }


		if (tpbrt_string_equals(param->name, Seed)) { return tpbrt_options_apply_seed(options, param); }


		if (tpbrt_string_equals(param->name, ForceDiffuse)) { return tpbrt_options_apply_force_diffuse(options, param); }


		if (tpbrt_string_equals(param->name, PixelStats)) { return tpbrt_options_apply_pixel_stats(options, param); }


		if (tpbrt_string_equals(param->name, Wavefront)) { return tpbrt_options_apply_wavefront(options, param); }

	return TPBRT_ERROR_INVALID_OPTION;
}

void tpbrt_options_free(tpbrt_options_t* const options) {
		if (options == TPBRT_NULL) { return; }

	tpbrt_string_free(&options->mse_reference_image);
	tpbrt_string_free(&options->mse_reference_out);
}
