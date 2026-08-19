#include <tinypbrt/pch.h>

#include <tinypbrt/detail/integrator_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	static tpbrt_error_t tpbrt_integrator_type_from_string(const tpbrt_string_t* const type_str,
	  tpbrt_integrator_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_INTEGRATOR_TYPE_MAX_NUM] = {
			{ .chars = "ambientocclusion", .size = 16 },
			{ .chars = "bdpt",			   .size = 4	 },
			{ .chars = "lightpath",		.size = 9  },
			{ .chars = "mlt",			  .size = 3	},
			{ .chars = "path",			   .size = 4	 },
			{ .chars = "randomwalk",		 .size = 10 },
			{ .chars = "simplepath",		 .size = 10 },
			{ .chars = "simplevolpath",	.size = 13 },
			{ .chars = "sppm",			   .size = 4	 },
			{ .chars = "volpath",		  .size = 7	},
		};

			if (type_str == TPBRT_NULL || type_str->chars == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_integrator_type_t t = 0; t < TPBRT_INTEGRATOR_TYPE_MAX_NUM; ++t) {
					if (type_str->size == TYPES_STRS[t].size &&
						strncmp(type_str->chars, TYPES_STRS[t].chars, TYPES_STRS[t].size) == 0) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_INTEGRATOR_TYPE;
	}

	static tpbrt_error_t tpbrt_integrator_light_sampler_from_string(const tpbrt_string_t* const sampler_str,
	  tpbrt_integrator_light_sampler_t* const sampler) {
		static const tpbrt_string_t SAMPLERS_STRS[TPBRT_INTEGRATOR_LIGHT_SAMPLER_MAX_NUM] = {
			{ .chars = "bvh",	  .size = 3 },
			{ .chars = "uniform", .size = 7 },
			{ .chars = "power",	.size = 5 },
		};

			if (sampler_str == TPBRT_NULL || sampler_str->chars == TPBRT_NULL || sampler == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_integrator_light_sampler_t s = 0; s < TPBRT_INTEGRATOR_LIGHT_SAMPLER_MAX_NUM; ++s) {
					if (sampler_str->size == SAMPLERS_STRS[s].size &&
						strncmp(sampler_str->chars, SAMPLERS_STRS[s].chars, SAMPLERS_STRS[s].size) == 0) {
						*sampler = s;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_INTEGRATOR_LIGHT_SAMPLER;
	}

	tpbrt_error_t tpbrt_create_integrator(const tpbrt_string_t* type_str, const tpbrt_params_list_t* params,
	  tpbrt_integrator_t** integrator) {
			if (type_str == TPBRT_NULL || type_str->chars == TPBRT_NULL || params == TPBRT_NULL || integrator == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		*integrator = malloc(sizeof(tpbrt_integrator_t));
			if (*integrator == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		tpbrt_error_t err = tpbrt_integrator_type_from_string(type_str, &(*integrator)->type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_integrator(integrator);
				return err;
			}

		static const tpbrt_string_t MAX_DEPTH_STR	  = { .chars = "maxdepth", .size = 8 };
		static const tpbrt_uint_t MAX_DEPTH_DEFAULT	  = 5u;

		static const tpbrt_string_t LIGHT_SAMPLER_STR = { .chars = "lightsampler", .size = 12 };
		static const tpbrt_string_t REGULARIZE_STR	  = { .chars = "regularize", .size = 10 };

			switch ((*integrator)->type) {
			default:
				case TPBRT_INTEGRATOR_TYPE_AMBIENT_OCCLUSION: {
					static const tpbrt_string_t COS_SAMPLE_STR	 = { .chars = "cossample", .size = 9 };
					static const tpbrt_string_t MAX_DISTANCE_STR = { .chars = "maxdistance", .size = 11 };

					err											 = tpbrt_params_list_get_bool(params, &COS_SAMPLE_STR, TPBRT_TRUE,
					  &(*integrator)->ambient_occlusion_params.cos_sample);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &MAX_DISTANCE_STR, INFINITY,
					  &(*integrator)->ambient_occlusion_params.max_distance);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_BDPT: {
					static const tpbrt_string_t VISUALIZE_STRATEGIES_STR = { .chars = "visualizestrategies", .size = 19 };
					static const tpbrt_string_t VISUALIZE_WEIGHTS_STR	 = { .chars = "visualizeweights", .size = 16 };

					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT,
					  &(*integrator)->bdpt_params.max_depth);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					err =
					  tpbrt_params_list_get_bool(params, &REGULARIZE_STR, TPBRT_FALSE, &(*integrator)->bdpt_params.regularize);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					err = tpbrt_params_list_get_bool(params, &VISUALIZE_STRATEGIES_STR, TPBRT_FALSE,
					  &(*integrator)->bdpt_params.visualize_strategies);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					err = tpbrt_params_list_get_bool(params, &VISUALIZE_WEIGHTS_STR, TPBRT_FALSE,
					  &(*integrator)->bdpt_params.visualize_weights);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_LIGHT_PATH: {
					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT,
					  &(*integrator)->light_path_params.max_depth);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_MLT: {
					static const tpbrt_string_t BOOTSTRAP_SAMPLES_STR	   = { .chars = "bootstrapsamples", .size = 16 };
					static const tpbrt_string_t CHAINS_STR				   = { .chars = "chains", .size = 6 };
					static const tpbrt_string_t MUTATIONS_PER_PIXEL_STR	   = { .chars = "mutationsperpixel", .size = 17 };
					static const tpbrt_string_t LARGE_STEP_PROBABILITY_STR = { .chars = "largestepprobability", .size = 20 };
					static const tpbrt_string_t SIGMA_STR				   = { .chars = "sigma", .size = 5 };

					err =
					  tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT, &(*integrator)->mlt_params.max_depth);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					err = tpbrt_params_list_get_bool(params, &REGULARIZE_STR, TPBRT_FALSE, &(*integrator)->mlt_params.regularize);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					static const tpbrt_uint_t BOOTSTRAP_SAMPLES_DEFAULT = 100u * 1000u;
					err = tpbrt_params_list_get_uint(params, &BOOTSTRAP_SAMPLES_STR, BOOTSTRAP_SAMPLES_DEFAULT,
					  &(*integrator)->mlt_params.bootstrap_samples);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					static const tpbrt_uint_t CHAINS_DEFAULT = 1000u;
					err = tpbrt_params_list_get_uint(params, &CHAINS_STR, CHAINS_DEFAULT, &(*integrator)->mlt_params.chains);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					static const tpbrt_uint_t MUTATIONS_PER_PIXEL_DEFAULT = 100u;
					err = tpbrt_params_list_get_uint(params, &MUTATIONS_PER_PIXEL_STR, MUTATIONS_PER_PIXEL_DEFAULT,
					  &(*integrator)->mlt_params.mutations_per_pixel);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					static const tpbrt_float_t LARGE_STEP_PROBABILITY_DEFAULT = 0.03f;
					err = tpbrt_params_list_get_float(params, &LARGE_STEP_PROBABILITY_STR, LARGE_STEP_PROBABILITY_DEFAULT,
					  &(*integrator)->mlt_params.large_step_probability);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					static const tpbrt_float_t SIGMA_DEFAULT = 0.01f;
					err = tpbrt_params_list_get_float(params, &SIGMA_STR, SIGMA_DEFAULT, &(*integrator)->mlt_params.sigma);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_PATH: {
					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT,
					  &(*integrator)->path_params.max_depth);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					tpbrt_string_t sampler_str;
					err = tpbrt_params_list_get_string(params, &LIGHT_SAMPLER_STR, &sampler_str);
						if (err != TPBRT_ERROR_NONE || err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_integrator(integrator);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*integrator)->path_params.light_sampler = TPBRT_INTEGRATOR_LIGHT_SAMPLER_BVH;
						}
						else {
							err =
							  tpbrt_integrator_light_sampler_from_string(&sampler_str, &(*integrator)->path_params.light_sampler);
							free(sampler_str.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_integrator(integrator);
									return err;
								}
						}

					err =
					  tpbrt_params_list_get_bool(params, &REGULARIZE_STR, TPBRT_FALSE, &(*integrator)->path_params.regularize);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_RANDOM_WALK: {
					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT,
					  &(*integrator)->random_walk_params.max_depth);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_SIMPLE_PATH: {
					static const tpbrt_string_t SAMPLE_BSDF_STR	  = { .chars = "samplebsdf", .size = 10 };
					static const tpbrt_string_t SAMPLE_LIGHTS_STR = { .chars = "samplelights", .size = 12 };

					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT,
					  &(*integrator)->simple_path_params.max_depth);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					err = tpbrt_params_list_get_bool(params, &SAMPLE_BSDF_STR, TPBRT_TRUE,
					  &(*integrator)->simple_path_params.sample_bsdf);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					err = tpbrt_params_list_get_bool(params, &SAMPLE_LIGHTS_STR, TPBRT_TRUE,
					  &(*integrator)->simple_path_params.sample_lights);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_SIMPLE_VOL_PATH: {
					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT,
					  &(*integrator)->simple_vol_path_params.max_depth);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					err = tpbrt_params_list_get_bool(params, &REGULARIZE_STR, TPBRT_FALSE,
					  &(*integrator)->simple_vol_path_params.regularize);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_SPPM: {
					static const tpbrt_string_t PHOTONS_PER_ITERATION_STR = { .chars = "photonsperiteration", .size = 19 };
					static const tpbrt_string_t RADIUS_STR				  = { .chars = "radius", .size = 6 };
					static const tpbrt_string_t SEED_STR				  = { .chars = "seed", .size = 4 };

					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT,
					  &(*integrator)->sppm_params.max_depth);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					err = tpbrt_params_list_get_int(params, &PHOTONS_PER_ITERATION_STR, -1,
					  &(*integrator)->sppm_params.photons_per_iteration);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &RADIUS_STR, 1.0f, &(*integrator)->sppm_params.radius);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					err = tpbrt_params_list_get_int(params, &SEED_STR, 0, &(*integrator)->sppm_params.seed);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_VOL_PATH: {
					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT,
					  &(*integrator)->vol_path_params.max_depth);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_integrator(integrator);
							return err;
						}

					tpbrt_string_t sampler_str;
					err = tpbrt_params_list_get_string(params, &LIGHT_SAMPLER_STR, &sampler_str);
						if (err != TPBRT_ERROR_NONE || err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_integrator(integrator);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*integrator)->vol_path_params.light_sampler = TPBRT_INTEGRATOR_LIGHT_SAMPLER_BVH;
						}
						else {
							err = tpbrt_integrator_light_sampler_from_string(&sampler_str,
							  &(*integrator)->vol_path_params.light_sampler);
							free(sampler_str.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_integrator(integrator);
									return err;
								}
						}
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_integrator(tpbrt_integrator_t** integrator) {
			if (integrator == TPBRT_NULL || *integrator == TPBRT_NULL) { return; }

		free(*integrator);
		*integrator = TPBRT_NULL;
	}

#ifdef __cplusplus
}
#endif