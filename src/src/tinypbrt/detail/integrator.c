#include <tinypbrt/pch.h>

#include <tinypbrt/detail/integrator_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/param_internal.h>

tpbrt_error_t tpbrt_integrator_type_from_string(const tpbrt_string_t type_str, tpbrt_integrator_type_t* const type) {
	static const tpbrt_string_t TypesStrs[TPBRT_INTEGRATOR_TYPE_MAX_NUM] = {
		TPBRT_STRING_INIT("ambientocclusion"),
		TPBRT_STRING_INIT("bdpt"),
		TPBRT_STRING_INIT("lightpath"),
		TPBRT_STRING_INIT("mlt"),
		TPBRT_STRING_INIT("path"),
		TPBRT_STRING_INIT("randomwalk"),
		TPBRT_STRING_INIT("simplepath"),
		TPBRT_STRING_INIT("simplevolpath"),
		TPBRT_STRING_INIT("sppm"),
		TPBRT_STRING_INIT("volpath"),
	};

		if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (tpbrt_string_empty(type_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_INTEGRATOR_TYPE; }

		for (tpbrt_integrator_type_t t = 0; t < TPBRT_INTEGRATOR_TYPE_MAX_NUM; ++t) {
				if (tpbrt_string_equals(type_str, TypesStrs[t]) == TPBRT_TRUE) {
					*type = t;
					return TPBRT_ERROR_NONE;
				}
		}

	return TPBRT_ERROR_INVALID_INTEGRATOR_TYPE;
}

tpbrt_error_t tpbrt_integrator_light_sampler_from_string(const tpbrt_string_t sampler_str,
  tpbrt_integrator_light_sampler_t* const sampler) {
	static const tpbrt_string_t SamplersStrs[TPBRT_INTEGRATOR_LIGHT_SAMPLER_MAX_NUM] = {
		TPBRT_STRING_INIT("bvh"),
		TPBRT_STRING_INIT("uniform"),
		TPBRT_STRING_INIT("power"),
	};

		if (sampler == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (tpbrt_string_empty(sampler_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_INTEGRATOR_LIGHT_SAMPLER; }

		for (tpbrt_integrator_light_sampler_t s = 0; s < TPBRT_INTEGRATOR_LIGHT_SAMPLER_MAX_NUM; ++s) {
				if (tpbrt_string_equals(sampler_str, SamplersStrs[s]) == TPBRT_TRUE) {
					*sampler = s;
					return TPBRT_ERROR_NONE;
				}
		}

	return TPBRT_ERROR_INVALID_INTEGRATOR_LIGHT_SAMPLER;
}

/**
 * @brief Reads Max depth parameters.
 * @param max_depth Output max depth.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup integrator-private
 */
static tpbrt_error_t tpbrt_integrator_read_max_depth(tpbrt_uint_t* const max_depth, const tpbrt_params_list_t* const params) {
		if (max_depth == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t MaxDepthStr	  = TPBRT_STRING_INIT("maxdepth");

	static const tpbrt_uint_t MaxDepthDefault = 5u;

	return tpbrt_params_list_get_uint(params, MaxDepthStr, MaxDepthDefault, max_depth);
}

/**
 * @brief Reads regularize parameters.
 * @param regularize Regularize data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup integrator-private
 */
static tpbrt_error_t tpbrt_integrator_read_regularize(tpbrt_bool_t* const regularize, const tpbrt_params_list_t* const params) {
		if (regularize == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t RegularizeStr	= TPBRT_STRING_INIT("regularize");

	static const tpbrt_bool_t RegularizeDefault = TPBRT_FALSE;

	return tpbrt_params_list_get_bool(params, RegularizeStr, RegularizeDefault, regularize);
}

/**
 * @brief Reads light sampler parameters.
 * @param light_sampler Light sampler data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup integrator-private
 */
static tpbrt_error_t tpbrt_integrator_read_light_sampler(tpbrt_integrator_light_sampler_t* const light_sampler,
  const tpbrt_params_list_t* const params) {
		if (light_sampler == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t LightSamplerStr		= TPBRT_STRING_INIT("lightsampler");

	static const tpbrt_string_t LightSamplerDefault = TPBRT_EMPTY_STRING_INIT;

	tpbrt_string_t sampler_str;
	tpbrt_error_t err = tpbrt_params_list_get_string(params, LightSamplerStr, LightSamplerDefault, &sampler_str);
		if (err != TPBRT_ERROR_NONE) { return err; }

		if (tpbrt_string_empty(sampler_str) == TPBRT_TRUE) {
			*light_sampler = TPBRT_INTEGRATOR_LIGHT_SAMPLER_BVH;
			return TPBRT_ERROR_NONE;
		}

	err = tpbrt_integrator_light_sampler_from_string(sampler_str, light_sampler);
	tpbrt_string_free(&sampler_str);
	return err;
}

/**
 * @brief Reads ambient occlusion parameters.
 * @param ambient_occlusion Ambient occlusion data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup integrator-private
 */
static tpbrt_error_t tpbrt_integrator_read_ambient_occlusion(tpbrt_integrator_ambient_occlusion_params_t* const ambient_occlusion,
  const tpbrt_params_list_t* const params) {
		if (ambient_occlusion == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t CosSampleStr	  = TPBRT_STRING_INIT("cossample");
	static const tpbrt_string_t MaxDistanceStr	  = TPBRT_STRING_INIT("maxdistance");

	static const tpbrt_bool_t CosSampleDefault	  = TPBRT_TRUE;
	static const tpbrt_float_t MaxDistanceDefault = TPBRT_INFINITY;

	tpbrt_bool_t cos_sample;
	const tpbrt_error_t err = tpbrt_params_list_get_bool(params, CosSampleStr, CosSampleDefault, &cos_sample);
		if (err != TPBRT_ERROR_NONE) { return err; }
	ambient_occlusion->cos_sample = cos_sample;

	return tpbrt_params_list_get_float(params, MaxDistanceStr, MaxDistanceDefault, &ambient_occlusion->max_distance);
}

/**
 * @brief Reads bdpt parameters.
 * @param bdpt Bdpt data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup integrator-private
 */
static tpbrt_error_t tpbrt_integrator_read_bdpt(tpbrt_integrator_bdpt_params_t* const bdpt,
  const tpbrt_params_list_t* const params) {
		if (bdpt == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t VisualizeStrategiesStr	 = TPBRT_STRING_INIT("visualizestrategies");
	static const tpbrt_string_t VisualizeWeightsStr		 = TPBRT_STRING_INIT("visualizeweights");

	static const tpbrt_bool_t VisualizeStrategiesDefault = TPBRT_FALSE;
	static const tpbrt_bool_t VisualizeWeightsDefault	 = TPBRT_FALSE;

	tpbrt_error_t err									 = tpbrt_integrator_read_max_depth(&bdpt->max_depth, params);
		if (err != TPBRT_ERROR_NONE) { return err; }

	tpbrt_bool_t temp;
	err = tpbrt_integrator_read_regularize(&temp, params);
		if (err != TPBRT_ERROR_NONE) { return err; }
	bdpt->regularize = temp;

	err				 = tpbrt_params_list_get_bool(params, VisualizeStrategiesStr, VisualizeStrategiesDefault, &temp);
		if (err != TPBRT_ERROR_NONE) { return err; }
	bdpt->visualize_strategies = temp;

	err						   = tpbrt_params_list_get_bool(params, VisualizeWeightsStr, VisualizeWeightsDefault, &temp);
	bdpt->visualize_weights	   = temp;

	return err;
}

/**
 * @brief Reads light path parameters.
 * @param light_path Light path data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup integrator-private
 */
static tpbrt_error_t tpbrt_integrator_read_light_path(tpbrt_integrator_light_path_params_t* const light_path,
  const tpbrt_params_list_t* const params) {
		if (light_path == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	return tpbrt_integrator_read_max_depth(&light_path->max_depth, params);
}

/**
 * @brief Reads mlt parameters.
 * @param mlt Mlt data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup integrator-private
 */
static tpbrt_error_t tpbrt_integrator_read_mlt(tpbrt_integrator_mlt_params_t* const mlt,
  const tpbrt_params_list_t* const params) {
		if (mlt == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t BootstrapSamplesStr		   = TPBRT_STRING_INIT("bootstrapsamples");
	static const tpbrt_string_t ChainsStr				   = TPBRT_STRING_INIT("chains");
	static const tpbrt_string_t MutationsPerPixelStr	   = TPBRT_STRING_INIT("mutationsperpixel");
	static const tpbrt_string_t LargeStepProbabilityStr	   = TPBRT_STRING_INIT("largestepprobability");
	static const tpbrt_string_t SigmaStr				   = TPBRT_STRING_INIT("sigma");

	static const tpbrt_uint_t BootstrapSamplesDefault	   = 100u * 1000u;
	static const tpbrt_uint_t ChainsDefault				   = 1000u;
	static const tpbrt_uint_t MutationsPerPixelDefault	   = 100u;
	static const tpbrt_float_t LargeStepProbabilityDefault = 0.3f;
	static const tpbrt_float_t SigmaDefault				   = 0.01f;

	tpbrt_error_t err									   = tpbrt_integrator_read_max_depth(&mlt->max_depth, params);
		if (err != TPBRT_ERROR_NONE) { return err; }

	tpbrt_bool_t regularize;
	err = tpbrt_integrator_read_regularize(&regularize, params);
		if (err != TPBRT_ERROR_NONE) { return err; }
	mlt->regularize = regularize;

	err				= tpbrt_params_list_get_uint(params, BootstrapSamplesStr, BootstrapSamplesDefault, &mlt->bootstrap_samples);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_uint(params, ChainsStr, ChainsDefault, &mlt->chains);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_uint(params, MutationsPerPixelStr, MutationsPerPixelDefault, &mlt->mutations_per_pixel);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, LargeStepProbabilityStr, LargeStepProbabilityDefault, &mlt->large_step_probability);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_float(params, SigmaStr, SigmaDefault, &mlt->sigma);
}

/**
 * @brief Reads path parameters.
 * @param path Path data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup integrator-private
 */
static tpbrt_error_t tpbrt_integrator_read_path(tpbrt_integrator_path_params_t* const path,
  const tpbrt_params_list_t* const params) {
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

/**
 * @brief Reads random walk parameters.
 * @param random_walk Random walk data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup integrator-private
 */
static tpbrt_error_t tpbrt_integrator_read_random_walk(tpbrt_integrator_random_walk_params_t* const random_walk,
  const tpbrt_params_list_t* const params) {
		if (random_walk == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	return tpbrt_integrator_read_max_depth(&random_walk->max_depth, params);
}

/**
 * @brief Reads simple path parameters.
 * @param simple_path Simple path data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup integrator-private
 */
static tpbrt_error_t tpbrt_integrator_read_simple_path(tpbrt_integrator_simple_path_params_t* const simple_path,
  const tpbrt_params_list_t* const params) {
		if (simple_path == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t SampleBsdfStr	  = TPBRT_STRING_INIT("samplebsdf");
	static const tpbrt_string_t SampleLightsStr	  = TPBRT_STRING_INIT("samplelights");

	static const tpbrt_bool_t SampleBsdfDefault	  = TPBRT_TRUE;
	static const tpbrt_bool_t SampleLightsDefault = TPBRT_TRUE;

	tpbrt_error_t err							  = tpbrt_integrator_read_max_depth(&simple_path->max_depth, params);
		if (err != TPBRT_ERROR_NONE) { return err; }

	tpbrt_bool_t temp;
	err = tpbrt_params_list_get_bool(params, SampleBsdfStr, SampleBsdfDefault, &temp);
		if (err != TPBRT_ERROR_NONE) { return err; }
	simple_path->sample_bsdf   = temp;

	err						   = tpbrt_params_list_get_bool(params, SampleLightsStr, SampleLightsDefault, &temp);
	simple_path->sample_lights = temp;

	return err;
}

/**
 * @brief Reads simple vol path parameters.
 * @param simple_vol_path Simple vol path data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup integrator-private
 */
static tpbrt_error_t tpbrt_integrator_read_simple_vol_path(tpbrt_integrator_simple_vol_path_params_t* const simple_vol_path,
  const tpbrt_params_list_t* const params) {
		if (simple_vol_path == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	return tpbrt_integrator_read_max_depth(&simple_vol_path->max_depth, params);
}

/**
 * @brief Reads sppm parameters.
 * @param sppm Sppm data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup integrator-private
 */
static tpbrt_error_t tpbrt_integrator_read_sppm(tpbrt_integrator_sppm_params_t* const sppm,
  const tpbrt_params_list_t* const params) {
		if (sppm == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t PhotonsPerIterationStr	= TPBRT_STRING_INIT("photonsperiteration");
	static const tpbrt_string_t RadiusStr				= TPBRT_STRING_INIT("radius");
	static const tpbrt_string_t SeedStr					= TPBRT_STRING_INIT("seed");

	static const tpbrt_int_t PhotonsPerIterationDefault = -1;
	static const tpbrt_float_t RadiusDefault			= 1.0f;
	static const tpbrt_int_t SeedDefault				= 0;

	tpbrt_error_t err									= tpbrt_integrator_read_max_depth(&sppm->max_depth, params);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_int(params, PhotonsPerIterationStr, PhotonsPerIterationDefault, &sppm->photons_per_iteration);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, RadiusStr, RadiusDefault, &sppm->radius);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_int(params, SeedStr, SeedDefault, &sppm->seed);
}

/**
 * @brief Reads vol path parameters.
 * @param vol_path Vol path data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup integrator-private
 */
static tpbrt_error_t tpbrt_integrator_read_vol_path(tpbrt_integrator_vol_path_params_t* const vol_path,
  const tpbrt_params_list_t* const params) {
		if (vol_path == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	tpbrt_error_t err = tpbrt_integrator_read_max_depth(&vol_path->max_depth, params);
		if (err != TPBRT_ERROR_NONE) { return err; }

	tpbrt_integrator_light_sampler_t light_sampler;
	err = tpbrt_integrator_read_light_sampler(&light_sampler, params);
		if (err != TPBRT_ERROR_NONE) { return err; }
	vol_path->light_sampler = light_sampler;

	tpbrt_bool_t regularize = TPBRT_FALSE;
	err						= tpbrt_integrator_read_regularize(&regularize, params);
	vol_path->regularize	= regularize;

	return err;
}

tpbrt_error_t tpbrt_integrator_init_default(tpbrt_integrator_t* const integrator) {
		if (integrator == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (memset(integrator, 0, sizeof(tpbrt_integrator_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_uint_t MaxDepthDefault = 5u;

	integrator->type						  = TPBRT_INTEGRATOR_TYPE_VOL_PATH;

	integrator->as.vol_path.max_depth		  = MaxDepthDefault;
	integrator->as.vol_path.light_sampler	  = TPBRT_INTEGRATOR_LIGHT_SAMPLER_BVH;

	return TPBRT_ERROR_NONE;
}

tpbrt_error_t tpbrt_integrator_init(const tpbrt_string_t type_str, const tpbrt_params_list_t* const params,
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
