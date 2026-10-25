#include <tinypbrt/pch.h>

#include <tinypbrt/detail/options_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/coord_internal.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

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

	static tpbrt_error_t tpbrt_options_apply_disable_pixel_jitter(tpbrt_options_t* const options, const tpbrt_param_t* param) {
			if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_bool_t disable_pixel_jitter = TPBRT_FALSE;
		const tpbrt_error_t err			  = tpbrt_param_as_bool(param, &disable_pixel_jitter);
		options->disable_pixel_jitter	  = disable_pixel_jitter;
		return err;
	}

	static tpbrt_error_t tpbrt_options_apply_disable_texture_filtering(tpbrt_options_t* const options,
	  const tpbrt_param_t* param) {
			if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_bool_t disable_texture_filtering = TPBRT_FALSE;
		const tpbrt_error_t err				   = tpbrt_param_as_bool(param, &disable_texture_filtering);
		options->disable_texture_filtering	   = disable_texture_filtering;
		return err;
	}

	static tpbrt_error_t tpbrt_options_apply_disable_wave_length_jitter(tpbrt_options_t* const options,
	  const tpbrt_param_t* param) {
			if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_bool_t disable_wave_length_jitter = TPBRT_FALSE;
		const tpbrt_error_t err					= tpbrt_param_as_bool(param, &disable_wave_length_jitter);
		options->disable_wave_length_jitter		= disable_wave_length_jitter;
		return err;
	}

	static tpbrt_error_t tpbrt_options_apply_displacement_edge_scale(tpbrt_options_t* const options, const tpbrt_param_t* param) {
			if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		return tpbrt_param_as_float(param, &options->displacement_edge_scale);
	}

	static tpbrt_error_t tpbrt_options_apply_mse_reference_image(tpbrt_options_t* const options, const tpbrt_param_t* param) {
			if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		return tpbrt_param_as_string(param, &options->mse_reference_image);
	}

	static tpbrt_error_t tpbrt_options_apply_mse_reference_out(tpbrt_options_t* const options, const tpbrt_param_t* param) {
			if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		return tpbrt_param_as_string(param, &options->mse_reference_out);
	}

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

	static tpbrt_error_t tpbrt_options_apply_seed(tpbrt_options_t* const options, const tpbrt_param_t* param) {
			if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		return tpbrt_param_as_int(param, &options->seed);
	}

	static tpbrt_error_t tpbrt_options_apply_force_diffuse(tpbrt_options_t* const options, const tpbrt_param_t* param) {
			if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_bool_t force_diffuse = TPBRT_FALSE;
		const tpbrt_error_t err	   = tpbrt_param_as_bool(param, &force_diffuse);
		options->force_diffuse	   = force_diffuse;
		return err;
	}

	static tpbrt_error_t tpbrt_options_apply_pixel_stats(tpbrt_options_t* const options, const tpbrt_param_t* param) {
			if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_bool_t pixel_stats = TPBRT_FALSE;
		const tpbrt_error_t err	 = tpbrt_param_as_bool(param, &pixel_stats);
		options->pixel_stats	 = pixel_stats;
		return err;
	}

	static tpbrt_error_t tpbrt_options_apply_wavefront(tpbrt_options_t* const options, const tpbrt_param_t* param) {
			if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_bool_t wavefront	= TPBRT_FALSE;
		const tpbrt_error_t err = tpbrt_param_as_bool(param, &wavefront);
		options->wavefront		= wavefront;
		return err;
	}

	tpbrt_error_t tpbrt_options_apply(tpbrt_options_t* const options, const tpbrt_param_t* param) {
			if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t DISABLE_PIXEL_JITTER	   = TPBRT_STRING_INIT("disablepixeljitter");
		static const tpbrt_string_t DISABLE_TEXTURE_FILTERING  = TPBRT_STRING_INIT("disabletexturefiltering");
		static const tpbrt_string_t DISABLE_WAVE_LENGTH_JITTER = TPBRT_STRING_INIT("disablewavelengthjitter");
		static const tpbrt_string_t DISPLACEMENT_EDGE_SCALE	   = TPBRT_STRING_INIT("displacementedgescale");
		static const tpbrt_string_t MSE_REFERENCE_IMAGE		   = TPBRT_STRING_INIT("msereferenceimage");
		static const tpbrt_string_t MSE_REFERENCE_OUT		   = TPBRT_STRING_INIT("msereferenceout");
		static const tpbrt_string_t RENDER_COORD_SYS		   = TPBRT_STRING_INIT("rendercoordsys");
		static const tpbrt_string_t SEED					   = TPBRT_STRING_INIT("seed");
		static const tpbrt_string_t FORCE_DIFFUSE			   = TPBRT_STRING_INIT("forcediffuse");
		static const tpbrt_string_t PIXEL_STATS				   = TPBRT_STRING_INIT("pixelstats");
		static const tpbrt_string_t WAVEFRONT				   = TPBRT_STRING_INIT("wavefront");

			if (tpbrt_string_equals(param->name, DISABLE_PIXEL_JITTER)) {
				return tpbrt_options_apply_disable_pixel_jitter(options, param);
			}

			if (tpbrt_string_equals(param->name, DISABLE_TEXTURE_FILTERING)) {
				return tpbrt_options_apply_disable_texture_filtering(options, param);
			}

			if (tpbrt_string_equals(param->name, DISABLE_WAVE_LENGTH_JITTER)) {
				return tpbrt_options_apply_disable_wave_length_jitter(options, param);
			}

			if (tpbrt_string_equals(param->name, DISPLACEMENT_EDGE_SCALE)) {
				return tpbrt_options_apply_displacement_edge_scale(options, param);
			}


			if (tpbrt_string_equals(param->name, MSE_REFERENCE_IMAGE)) {
				return tpbrt_options_apply_mse_reference_image(options, param);
			}


			if (tpbrt_string_equals(param->name, MSE_REFERENCE_OUT)) {
				return tpbrt_options_apply_mse_reference_out(options, param);
			}


			if (tpbrt_string_equals(param->name, RENDER_COORD_SYS)) {
				return tpbrt_options_apply_render_coord_sys(options, param);
			}


			if (tpbrt_string_equals(param->name, SEED)) { return tpbrt_options_apply_seed(options, param); }


			if (tpbrt_string_equals(param->name, FORCE_DIFFUSE)) { return tpbrt_options_apply_force_diffuse(options, param); }


			if (tpbrt_string_equals(param->name, PIXEL_STATS)) { return tpbrt_options_apply_pixel_stats(options, param); }


			if (tpbrt_string_equals(param->name, WAVEFRONT)) { return tpbrt_options_apply_wavefront(options, param); }

		return TPBRT_ERROR_INVALID_OPTION;
	}

	void tpbrt_options_free(tpbrt_options_t* const options) {
			if (options == TPBRT_NULL) { return; }

		tpbrt_string_free(&options->mse_reference_image);
		tpbrt_string_free(&options->mse_reference_out);
	}

#ifdef __cplusplus
}
#endif
