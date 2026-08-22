#include <tinypbrt/pch.h>

#include <tinypbrt/detail/integrator_internal.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	static tpbrt_error_t tpbrt_integrator_type_from_string(const tpbrt_string_t* const type_str,
	  tpbrt_integrator_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_INTEGRATOR_TYPE_MAX_NUM] = {
			TPBRT_STRING("ambientocclusion"),
			TPBRT_STRING("bdpt"),
			TPBRT_STRING("lightpath"),
			TPBRT_STRING("mlt"),
			TPBRT_STRING("path"),
			TPBRT_STRING("randomwalk"),
			TPBRT_STRING("simplepath"),
			TPBRT_STRING("simplevolpath"),
			TPBRT_STRING("sppm"),
			TPBRT_STRING("volpath"),
		};

			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_integrator_type_t t = 0; t < TPBRT_INTEGRATOR_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS + t)) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_INTEGRATOR_TYPE;
	}

	static tpbrt_error_t tpbrt_integrator_light_sampler_from_string(const tpbrt_string_t* const sampler_str,
	  tpbrt_integrator_light_sampler_t* const sampler) {
		static const tpbrt_string_t SAMPLERS_STRS[TPBRT_INTEGRATOR_LIGHT_SAMPLER_MAX_NUM] = {
			TPBRT_STRING("bvh"),
			TPBRT_STRING("uniform"),
			TPBRT_STRING("power"),
		};

			if (sampler_str == TPBRT_NULL || sampler_str->data == TPBRT_NULL || sampler == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_integrator_light_sampler_t s = 0; s < TPBRT_INTEGRATOR_LIGHT_SAMPLER_MAX_NUM; ++s) {
					if (tpbrt_string_equals(sampler_str, SAMPLERS_STRS + s)) {
						*sampler = s;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_INTEGRATOR_LIGHT_SAMPLER;
	}

	tpbrt_error_t tpbrt_create_integrator(const tpbrt_string_t* type_str, const tpbrt_params_list_t* params,
	  tpbrt_integrator_t* integrator) {
			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || params == TPBRT_NULL || integrator == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		tpbrt_error_t err = tpbrt_integrator_type_from_string(type_str, &integrator->type);
			if (err != TPBRT_ERROR_NONE) { return err; }

		static const tpbrt_string_t MAX_DEPTH_STR	  = TPBRT_STRING("maxdepth");
		static const tpbrt_uint_t MAX_DEPTH_DEFAULT	  = 5u;

		static const tpbrt_string_t LIGHT_SAMPLER_STR = TPBRT_STRING("lightsampler");
		static const tpbrt_string_t REGULARIZE_STR	  = TPBRT_STRING("regularize");

			switch (integrator->type) {
			default:
				case TPBRT_INTEGRATOR_TYPE_AMBIENT_OCCLUSION: {
					static const tpbrt_string_t COS_SAMPLE_STR	 = TPBRT_STRING("cossample");
					static const tpbrt_string_t MAX_DISTANCE_STR = TPBRT_STRING("maxdistance");

					err											 = tpbrt_params_list_get_bool(params, &COS_SAMPLE_STR, TPBRT_TRUE,
					  &integrator->as.ambient_occlusion.cos_sample);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err = tpbrt_params_list_get_float(params, &MAX_DISTANCE_STR, INFINITY,
					  &integrator->as.ambient_occlusion.max_distance);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_BDPT: {
					static const tpbrt_string_t VISUALIZE_STRATEGIES_STR = TPBRT_STRING("visualizestrategies");
					static const tpbrt_string_t VISUALIZE_WEIGHTS_STR	 = TPBRT_STRING("visualizeweights");

					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT, &integrator->as.bdpt.max_depth);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err = tpbrt_params_list_get_bool(params, &REGULARIZE_STR, TPBRT_FALSE, &integrator->as.bdpt.regularize);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err = tpbrt_params_list_get_bool(params, &VISUALIZE_STRATEGIES_STR, TPBRT_FALSE,
					  &integrator->as.bdpt.visualize_strategies);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err = tpbrt_params_list_get_bool(params, &VISUALIZE_WEIGHTS_STR, TPBRT_FALSE,
					  &integrator->as.bdpt.visualize_weights);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_LIGHT_PATH: {
					err =
					  tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT, &integrator->as.light_path.max_depth);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_MLT: {
					static const tpbrt_string_t BOOTSTRAP_SAMPLES_STR	   = TPBRT_STRING("bootstrapsamples");
					static const tpbrt_string_t CHAINS_STR				   = TPBRT_STRING("chains");
					static const tpbrt_string_t MUTATIONS_PER_PIXEL_STR	   = TPBRT_STRING("mutationsperpixel");
					static const tpbrt_string_t LARGE_STEP_PROBABILITY_STR = TPBRT_STRING("largestepprobability");
					static const tpbrt_string_t SIGMA_STR				   = TPBRT_STRING("sigma");

					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT, &integrator->as.mlt.max_depth);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err = tpbrt_params_list_get_bool(params, &REGULARIZE_STR, TPBRT_FALSE, &integrator->as.mlt.regularize);
						if (err != TPBRT_ERROR_NONE) { return err; }

					static const tpbrt_uint_t BOOTSTRAP_SAMPLES_DEFAULT = 100u * 1000u;
					err = tpbrt_params_list_get_uint(params, &BOOTSTRAP_SAMPLES_STR, BOOTSTRAP_SAMPLES_DEFAULT,
					  &integrator->as.mlt.bootstrap_samples);
						if (err != TPBRT_ERROR_NONE) { return err; }

					static const tpbrt_uint_t CHAINS_DEFAULT = 1000u;
					err = tpbrt_params_list_get_uint(params, &CHAINS_STR, CHAINS_DEFAULT, &integrator->as.mlt.chains);
						if (err != TPBRT_ERROR_NONE) { return err; }

					static const tpbrt_uint_t MUTATIONS_PER_PIXEL_DEFAULT = 100u;
					err = tpbrt_params_list_get_uint(params, &MUTATIONS_PER_PIXEL_STR, MUTATIONS_PER_PIXEL_DEFAULT,
					  &integrator->as.mlt.mutations_per_pixel);
						if (err != TPBRT_ERROR_NONE) { return err; }

					static const tpbrt_float_t LARGE_STEP_PROBABILITY_DEFAULT = 0.03f;
					err = tpbrt_params_list_get_float(params, &LARGE_STEP_PROBABILITY_STR, LARGE_STEP_PROBABILITY_DEFAULT,
					  &integrator->as.mlt.large_step_probability);
						if (err != TPBRT_ERROR_NONE) { return err; }

					static const tpbrt_float_t SIGMA_DEFAULT = 0.01f;
					err = tpbrt_params_list_get_float(params, &SIGMA_STR, SIGMA_DEFAULT, &integrator->as.mlt.sigma);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_PATH: {
					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT, &integrator->as.path.max_depth);
						if (err != TPBRT_ERROR_NONE) { return err; }

					tpbrt_string_t sampler_str;
					err = tpbrt_params_list_get_string(params, &LIGHT_SAMPLER_STR, &sampler_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

						if (err == TPBRT_ERROR_NOT_FOUND) {
							integrator->as.path.light_sampler = TPBRT_INTEGRATOR_LIGHT_SAMPLER_BVH;
						}
						else {
							err = tpbrt_integrator_light_sampler_from_string(&sampler_str, &integrator->as.path.light_sampler);
							tpbrt_free_string(&sampler_str);
								if (err != TPBRT_ERROR_NONE) { return err; }
						}

					err = tpbrt_params_list_get_bool(params, &REGULARIZE_STR, TPBRT_FALSE, &integrator->as.path.regularize);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_RANDOM_WALK: {
					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT,
					  &integrator->as.random_walk.max_depth);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_SIMPLE_PATH: {
					static const tpbrt_string_t SAMPLE_BSDF_STR	  = TPBRT_STRING("samplebsdf");
					static const tpbrt_string_t SAMPLE_LIGHTS_STR = TPBRT_STRING("samplelights");

					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT,
					  &integrator->as.simple_path.max_depth);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err =
					  tpbrt_params_list_get_bool(params, &SAMPLE_BSDF_STR, TPBRT_TRUE, &integrator->as.simple_path.sample_bsdf);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err = tpbrt_params_list_get_bool(params, &SAMPLE_LIGHTS_STR, TPBRT_TRUE,
					  &integrator->as.simple_path.sample_lights);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_SIMPLE_VOL_PATH: {
					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT,
					  &integrator->as.simple_vol_path.max_depth);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err = tpbrt_params_list_get_bool(params, &REGULARIZE_STR, TPBRT_FALSE,
					  &integrator->as.simple_vol_path.regularize);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_SPPM: {
					static const tpbrt_string_t PHOTONS_PER_ITERATION_STR = TPBRT_STRING("photonsperiteration");
					static const tpbrt_string_t RADIUS_STR				  = TPBRT_STRING("radius");
					static const tpbrt_string_t SEED_STR				  = TPBRT_STRING("seed");

					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT, &integrator->as.sppm.max_depth);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err = tpbrt_params_list_get_int(params, &PHOTONS_PER_ITERATION_STR, -1,
					  &integrator->as.sppm.photons_per_iteration);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err = tpbrt_params_list_get_float(params, &RADIUS_STR, 1.0f, &integrator->as.sppm.radius);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err = tpbrt_params_list_get_int(params, &SEED_STR, 0, &integrator->as.sppm.seed);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_VOL_PATH: {
					err =
					  tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT, &integrator->as.vol_path.max_depth);
						if (err != TPBRT_ERROR_NONE) { return err; }

					tpbrt_string_t sampler_str;
					err = tpbrt_params_list_get_string(params, &LIGHT_SAMPLER_STR, &sampler_str);
						if (err != TPBRT_ERROR_NONE || err != TPBRT_ERROR_NOT_FOUND) { return err; }

						if (err == TPBRT_ERROR_NOT_FOUND) {
							integrator->as.vol_path.light_sampler = TPBRT_INTEGRATOR_LIGHT_SAMPLER_BVH;
						}
						else {
							err =
							  tpbrt_integrator_light_sampler_from_string(&sampler_str, &integrator->as.vol_path.light_sampler);
							tpbrt_free_string(&sampler_str);
								if (err != TPBRT_ERROR_NONE) { return err; }
						}
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

#ifdef __cplusplus
}
#endif