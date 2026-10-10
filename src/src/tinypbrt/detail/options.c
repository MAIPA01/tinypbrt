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

	tpbrt_error_t tpbrt_options_apply(tpbrt_options_t* const options, const tpbrt_param_t* param) {
			if (options == TPBRT_NULL || param == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t DISABLE_PIXEL_JITTER = TPBRT_STRING("disablepixeljitter");
			if (tpbrt_string_equals(param->name, DISABLE_PIXEL_JITTER)) {
				tpbrt_bool_t disable_pixel_jitter;
				const tpbrt_error_t err = tpbrt_param_as_bool(param, &disable_pixel_jitter);

					if (err != TPBRT_ERROR_NONE) { return err; }

				options->disable_pixel_jitter = disable_pixel_jitter;
				return TPBRT_ERROR_NONE;
			}

		static const tpbrt_string_t DISABLE_TEXTURE_FILTERING = TPBRT_STRING("disabletexturefiltering");
			if (tpbrt_string_equals(param->name, DISABLE_TEXTURE_FILTERING)) {
				tpbrt_bool_t disable_texture_filtering;
				const tpbrt_error_t err = tpbrt_param_as_bool(param, &disable_texture_filtering);

					if (err != TPBRT_ERROR_NONE) { return err; }

				options->disable_texture_filtering = disable_texture_filtering;
				return TPBRT_ERROR_NONE;
			}

		static const tpbrt_string_t DISABLE_WAVE_LENGTH_JITTER = TPBRT_STRING("disablewavelengthjitter");
			if (tpbrt_string_equals(param->name, DISABLE_WAVE_LENGTH_JITTER)) {
				tpbrt_bool_t disable_wave_length_jitter;
				const tpbrt_error_t err = tpbrt_param_as_bool(param, &disable_wave_length_jitter);

					if (err != TPBRT_ERROR_NONE) { return err; }

				options->disable_wave_length_jitter = disable_wave_length_jitter;
				return TPBRT_ERROR_NONE;
			}

		static const tpbrt_string_t DISPLACEMENT_EDGE_SCALE = TPBRT_STRING("displacementedgescale");
			if (tpbrt_string_equals(param->name, DISPLACEMENT_EDGE_SCALE)) {
				return tpbrt_param_as_float(param, &options->displacement_edge_scale);
			}

		static const tpbrt_string_t MSE_REFERENCE_IMAGE = TPBRT_STRING("msereferenceimage");
			if (tpbrt_string_equals(param->name, MSE_REFERENCE_IMAGE)) {
				return tpbrt_param_as_string(param, &options->mse_reference_image);
			}

		static const tpbrt_string_t MSE_REFERENCE_OUT = TPBRT_STRING("msereferenceout");
			if (tpbrt_string_equals(param->name, MSE_REFERENCE_OUT)) {
				return tpbrt_param_as_string(param, &options->mse_reference_out);
			}

		static const tpbrt_string_t RENDER_COORD_SYS = TPBRT_STRING("rendercoordsys");
			if (tpbrt_string_equals(param->name, RENDER_COORD_SYS)) {
				tpbrt_string_t coord_str;
				tpbrt_error_t err = tpbrt_param_as_string(param, &coord_str);
					if (err != TPBRT_ERROR_NONE) { return err; }

				tpbrt_coord_system_type_t render_coord_sys;
				err = tpbrt_coord_system_type_from_string(coord_str, &render_coord_sys);
				tpbrt_string_free(&coord_str);

					if (err != TPBRT_ERROR_NONE) { return err; }

				options->render_coord_sys = render_coord_sys;
				return TPBRT_ERROR_NONE;
			}

		static const tpbrt_string_t SEED = TPBRT_STRING("seed");
			if (tpbrt_string_equals(param->name, SEED)) { return tpbrt_param_as_int(param, &options->seed); }

		static const tpbrt_string_t FORCE_DIFFUSE = TPBRT_STRING("forcediffuse");
			if (tpbrt_string_equals(param->name, FORCE_DIFFUSE)) {
				tpbrt_bool_t force_diffuse;
				const tpbrt_error_t err = tpbrt_param_as_bool(param, &force_diffuse);

					if (err != TPBRT_ERROR_NONE) { return err; }

				options->force_diffuse = force_diffuse;
				return TPBRT_ERROR_NONE;
			}

		static const tpbrt_string_t PIXEL_STATS = TPBRT_STRING("pixelstats");
			if (tpbrt_string_equals(param->name, PIXEL_STATS)) {
				tpbrt_bool_t pixel_stats;
				const tpbrt_error_t err = tpbrt_param_as_bool(param, &pixel_stats);

					if (err != TPBRT_ERROR_NONE) { return err; }

				options->pixel_stats = pixel_stats;
				return TPBRT_ERROR_NONE;
			}

		static const tpbrt_string_t WAVEFRONT = TPBRT_STRING("wavefront");
			if (tpbrt_string_equals(param->name, WAVEFRONT)) {
				tpbrt_bool_t wavefront;
				const tpbrt_error_t err = tpbrt_param_as_bool(param, &wavefront);

					if (err != TPBRT_ERROR_NONE) { return err; }

				options->wavefront = wavefront;
				return TPBRT_ERROR_NONE;
			}

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