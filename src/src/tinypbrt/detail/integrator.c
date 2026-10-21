#include <tinypbrt/pch.h>

#include <tinypbrt/detail/integrator_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_integrator_type_from_string(const tpbrt_string_t type_str, tpbrt_integrator_type_t* const type) {
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

			if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(type_str)) { return TPBRT_ERROR_INVALID_INTEGRATOR_TYPE; }

			for (tpbrt_integrator_type_t t = 0; t < TPBRT_INTEGRATOR_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS[t])) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_INTEGRATOR_TYPE;
	}

	tpbrt_error_t tpbrt_integrator_light_sampler_from_string(const tpbrt_string_t sampler_str,
	  tpbrt_integrator_light_sampler_t* const sampler) {
		static const tpbrt_string_t SAMPLERS_STRS[TPBRT_INTEGRATOR_LIGHT_SAMPLER_MAX_NUM] = {
			TPBRT_STRING("bvh"),
			TPBRT_STRING("uniform"),
			TPBRT_STRING("power"),
		};

			if (sampler == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(sampler_str)) { return TPBRT_ERROR_INVALID_INTEGRATOR_LIGHT_SAMPLER; }

			for (tpbrt_integrator_light_sampler_t s = 0; s < TPBRT_INTEGRATOR_LIGHT_SAMPLER_MAX_NUM; ++s) {
					if (tpbrt_string_equals(sampler_str, SAMPLERS_STRS[s])) {
						*sampler = s;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_INTEGRATOR_LIGHT_SAMPLER;
	}

	static tpbrt_error_t tpbrt_integrator_read_max_depth(tpbrt_uint_t* const max_depth, const tpbrt_params_list_t params) {
			if (max_depth == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t MAX_DEPTH_STR	= TPBRT_STRING("maxdepth");

		static const tpbrt_uint_t MAX_DEPTH_DEFAULT = 5u;

		return tpbrt_params_list_get_uint(params, MAX_DEPTH_STR, MAX_DEPTH_DEFAULT, max_depth);
	}

	static tpbrt_error_t tpbrt_integrator_read_regularize(tpbrt_bool_t* const regularize, const tpbrt_params_list_t params) {
			if (regularize == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t REGULARIZE_STR	 = TPBRT_STRING("regularize");

		static const tpbrt_bool_t REGULARIZE_DEFAULT = TPBRT_FALSE;

		return tpbrt_params_list_get_bool(params, REGULARIZE_STR, REGULARIZE_DEFAULT, regularize);
	}

	static tpbrt_error_t tpbrt_integrator_read_light_sampler(tpbrt_integrator_light_sampler_t* const light_sampler,
	  const tpbrt_params_list_t params) {
			if (light_sampler == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t LIGHT_SAMPLER_STR	  = TPBRT_STRING("lightsampler");

		static const tpbrt_string_t LIGHT_SAMPLER_DEFAULT = TPBRT_EMPTY_STRING;

		tpbrt_string_t sampler_str;
		tpbrt_error_t err = tpbrt_params_list_get_string(params, LIGHT_SAMPLER_STR, LIGHT_SAMPLER_DEFAULT, &sampler_str);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (tpbrt_string_empty(sampler_str) == TPBRT_TRUE) {
				*light_sampler = TPBRT_INTEGRATOR_LIGHT_SAMPLER_BVH;
				return TPBRT_ERROR_NONE;
			}

		err = tpbrt_integrator_light_sampler_from_string(sampler_str, light_sampler);
		tpbrt_string_free(&sampler_str);
		return err;
	}

	static tpbrt_error_t tpbrt_integrator_read_ambient_occlusion(
	  tpbrt_integrator_ambient_occlusion_params_t* const ambient_occlusion, const tpbrt_params_list_t params
	) {
			if (ambient_occlusion == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t COS_SAMPLE_STR		= TPBRT_STRING("cossample");
		static const tpbrt_string_t MAX_DISTANCE_STR	= TPBRT_STRING("maxdistance");

		static const tpbrt_bool_t COS_SAMPLE_DEFAULT	= TPBRT_TRUE;
		static const tpbrt_float_t MAX_DISTANCE_DEFAULT = TPBRT_INFINITY;

		tpbrt_bool_t cos_sample;
		const tpbrt_error_t err = tpbrt_params_list_get_bool(params, COS_SAMPLE_STR, COS_SAMPLE_DEFAULT, &cos_sample);
			if (err != TPBRT_ERROR_NONE) { return err; }
		ambient_occlusion->cos_sample = cos_sample;

		return tpbrt_params_list_get_float(params, MAX_DISTANCE_STR, MAX_DISTANCE_DEFAULT, &ambient_occlusion->max_distance);
	}

	static tpbrt_error_t tpbrt_integrator_read_bdpt(tpbrt_integrator_bdpt_params_t* const bdpt,
	  const tpbrt_params_list_t params) {
			if (bdpt == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t VISUALIZE_STRATEGIES_STR   = TPBRT_STRING("visualizestrategies");
		static const tpbrt_string_t VISUALIZE_WEIGHTS_STR	   = TPBRT_STRING("visualizeweights");

		static const tpbrt_bool_t VISUALIZE_STRATEGIES_DEFAULT = TPBRT_FALSE;
		static const tpbrt_bool_t VISUALIZE_WEIGHTS_DEFAULT	   = TPBRT_FALSE;

		tpbrt_error_t err									   = tpbrt_integrator_read_max_depth(&bdpt->max_depth, params);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_bool_t temp;
		err = tpbrt_integrator_read_regularize(&temp, params);
			if (err != TPBRT_ERROR_NONE) { return err; }
		bdpt->regularize = temp;

		err				 = tpbrt_params_list_get_bool(params, VISUALIZE_STRATEGIES_STR, VISUALIZE_STRATEGIES_DEFAULT, &temp);
			if (err != TPBRT_ERROR_NONE) { return err; }
		bdpt->visualize_strategies = temp;

		err						   = tpbrt_params_list_get_bool(params, VISUALIZE_WEIGHTS_STR, VISUALIZE_WEIGHTS_DEFAULT, &temp);
		bdpt->visualize_weights	   = temp;

		return err;
	}

	static tpbrt_error_t tpbrt_integrator_read_light_path(tpbrt_integrator_light_path_params_t* const light_path,
	  const tpbrt_params_list_t params) {
			if (light_path == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		return tpbrt_integrator_read_max_depth(&light_path->max_depth, params);
	}

	static tpbrt_error_t tpbrt_integrator_read_mlt(tpbrt_integrator_mlt_params_t* const mlt, const tpbrt_params_list_t params) {
			if (mlt == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t BOOTSTRAP_SAMPLES_STR		  = TPBRT_STRING("bootstrapsamples");
		static const tpbrt_string_t CHAINS_STR					  = TPBRT_STRING("chains");
		static const tpbrt_string_t MUTATIONS_PER_PIXEL_STR		  = TPBRT_STRING("mutationsperpixel");
		static const tpbrt_string_t LARGE_STEP_PROBABILITY_STR	  = TPBRT_STRING("largestepprobability");
		static const tpbrt_string_t SIGMA_STR					  = TPBRT_STRING("sigma");

		static const tpbrt_uint_t BOOTSTRAP_SAMPLES_DEFAULT		  = 100u * 1000u;
		static const tpbrt_uint_t CHAINS_DEFAULT				  = 1000u;
		static const tpbrt_uint_t MUTATIONS_PER_PIXEL_DEFAULT	  = 100u;
		static const tpbrt_float_t LARGE_STEP_PROBABILITY_DEFAULT = 0.3f;
		static const tpbrt_float_t SIGMA_DEFAULT				  = 0.01f;

		tpbrt_error_t err										  = tpbrt_integrator_read_max_depth(&mlt->max_depth, params);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_bool_t regularize;
		err = tpbrt_integrator_read_regularize(&regularize, params);
			if (err != TPBRT_ERROR_NONE) { return err; }
		mlt->regularize = regularize;

		err = tpbrt_params_list_get_uint(params, BOOTSTRAP_SAMPLES_STR, BOOTSTRAP_SAMPLES_DEFAULT, &mlt->bootstrap_samples);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_uint(params, CHAINS_STR, CHAINS_DEFAULT, &mlt->chains);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_uint(params, MUTATIONS_PER_PIXEL_STR, MUTATIONS_PER_PIXEL_DEFAULT, &mlt->mutations_per_pixel);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, LARGE_STEP_PROBABILITY_STR, LARGE_STEP_PROBABILITY_DEFAULT,
		  &mlt->large_step_probability);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, SIGMA_STR, SIGMA_DEFAULT, &mlt->sigma);
	}

	static tpbrt_error_t tpbrt_integrator_read_path(tpbrt_integrator_path_params_t* const path,
	  const tpbrt_params_list_t params) {
			if (path == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_error_t err = tpbrt_integrator_read_max_depth(&path->max_depth, params);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_integrator_light_sampler_t light_sampler;
		err = tpbrt_integrator_read_light_sampler(&light_sampler, params);
			if (err != TPBRT_ERROR_NONE) { return err; }
		path->light_sampler = light_sampler;

		tpbrt_bool_t regularize;
		err = tpbrt_integrator_read_regularize(&regularize, params);
			if (err != TPBRT_ERROR_NONE) { return err; }
		path->regularize = regularize;

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_integrator_read_random_walk(tpbrt_integrator_random_walk_params_t* const random_walk,
	  const tpbrt_params_list_t params) {
			if (random_walk == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		return tpbrt_integrator_read_max_depth(&random_walk->max_depth, params);
	}

	static tpbrt_error_t tpbrt_integrator_read_simple_path(tpbrt_integrator_simple_path_params_t* const simple_path,
	  const tpbrt_params_list_t params) {
			if (simple_path == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t SAMPLE_BSDF_STR		= TPBRT_STRING("samplebsdf");
		static const tpbrt_string_t SAMPLE_LIGHTS_STR	= TPBRT_STRING("samplelights");

		static const tpbrt_bool_t SAMPLE_BSDF_DEFAULT	= TPBRT_TRUE;
		static const tpbrt_bool_t SAMPLE_LIGHTS_DEFAULT = TPBRT_TRUE;

		tpbrt_error_t err								= tpbrt_integrator_read_max_depth(&simple_path->max_depth, params);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_bool_t temp;
		err = tpbrt_params_list_get_bool(params, SAMPLE_BSDF_STR, SAMPLE_BSDF_DEFAULT, &temp);
			if (err != TPBRT_ERROR_NONE) { return err; }
		simple_path->sample_bsdf   = temp;

		err						   = tpbrt_params_list_get_bool(params, SAMPLE_LIGHTS_STR, SAMPLE_LIGHTS_DEFAULT, &temp);
		simple_path->sample_lights = temp;

		return err;
	}

	static tpbrt_error_t tpbrt_integrator_read_simple_vol_path(tpbrt_integrator_simple_vol_path_params_t* const simple_vol_path,
	  const tpbrt_params_list_t params) {
			if (simple_vol_path == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		return tpbrt_integrator_read_max_depth(&simple_vol_path->max_depth, params);
	}

	static tpbrt_error_t tpbrt_integrator_read_sppm(tpbrt_integrator_sppm_params_t* const sppm,
	  const tpbrt_params_list_t params) {
			if (sppm == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t PHOTONS_PER_ITERATION_STR  = TPBRT_STRING("photonsperiteration");
		static const tpbrt_string_t RADIUS_STR				   = TPBRT_STRING("radius");
		static const tpbrt_string_t SEED_STR				   = TPBRT_STRING("seed");

		static const tpbrt_int_t PHOTONS_PER_ITERATION_DEFAULT = -1;
		static const tpbrt_float_t RADIUS_DEFAULT			   = 1.0f;
		static const tpbrt_int_t SEED_DEFAULT				   = 0;

		tpbrt_error_t err									   = tpbrt_integrator_read_max_depth(&sppm->max_depth, params);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_int(params, PHOTONS_PER_ITERATION_STR, PHOTONS_PER_ITERATION_DEFAULT,
		  &sppm->photons_per_iteration);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, RADIUS_STR, RADIUS_DEFAULT, &sppm->radius);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_int(params, SEED_STR, SEED_DEFAULT, &sppm->seed);
	}

	static tpbrt_error_t tpbrt_integrator_read_vol_path(tpbrt_integrator_vol_path_params_t* const vol_path,
	  const tpbrt_params_list_t params) {
			if (vol_path == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_error_t err = tpbrt_integrator_read_max_depth(&vol_path->max_depth, params);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_integrator_light_sampler_t light_sampler;
		err = tpbrt_integrator_read_light_sampler(&light_sampler, params);
			if (err != TPBRT_ERROR_NONE) { return err; }
		vol_path->light_sampler = light_sampler;

		tpbrt_bool_t regularize;
		err					 = tpbrt_integrator_read_regularize(&regularize, params);
		vol_path->regularize = regularize;

		return err;
	}

	tpbrt_error_t tpbrt_integrator_init_default(tpbrt_integrator_t* const integrator) {
			if (integrator == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(integrator, 0, sizeof(tpbrt_integrator_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_uint_t MAX_DEPTH_DEFAULT = 5u;

		integrator->type							= TPBRT_INTEGRATOR_TYPE_VOL_PATH;

		integrator->as.vol_path.max_depth			= MAX_DEPTH_DEFAULT;
		integrator->as.vol_path.light_sampler		= TPBRT_INTEGRATOR_LIGHT_SAMPLER_BVH;

		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_integrator_init(const tpbrt_string_t type_str, const tpbrt_params_list_t params,
	  tpbrt_integrator_t* integrator) {
			if (integrator == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(integrator, 0, sizeof(tpbrt_integrator_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_integrator_type_t integrator_type;
		tpbrt_error_t err = tpbrt_integrator_type_from_string(type_str, &integrator_type);
			if (err != TPBRT_ERROR_NONE) { return err; }
		integrator->type = integrator_type;

			switch (integrator->type) {
			default:
				case TPBRT_INTEGRATOR_TYPE_AMBIENT_OCCLUSION: {
					err = tpbrt_integrator_read_ambient_occlusion(&integrator->as.ambient_occlusion, params);
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_BDPT: {
					err = tpbrt_integrator_read_bdpt(&integrator->as.bdpt, params);
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_LIGHT_PATH: {
					err = tpbrt_integrator_read_light_path(&integrator->as.light_path, params);
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_MLT: {
					err = tpbrt_integrator_read_mlt(&integrator->as.mlt, params);
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_PATH: {
					err = tpbrt_integrator_read_path(&integrator->as.path, params);
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_RANDOM_WALK: {
					err = tpbrt_integrator_read_random_walk(&integrator->as.random_walk, params);
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_SIMPLE_PATH: {
					err = tpbrt_integrator_read_simple_path(&integrator->as.simple_path, params);
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_SIMPLE_VOL_PATH: {
					err = tpbrt_integrator_read_simple_vol_path(&integrator->as.simple_vol_path, params);
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_SPPM: {
					err = tpbrt_integrator_read_sppm(&integrator->as.sppm, params);
					break;
				}
				case TPBRT_INTEGRATOR_TYPE_VOL_PATH: {
					err = tpbrt_integrator_read_vol_path(&integrator->as.vol_path, params);
					break;
				}
			}

		return err;
	}

#ifdef __cplusplus
}
#endif