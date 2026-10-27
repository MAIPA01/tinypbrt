#pragma once
#ifndef _TINYPBRT_INTEGRATOR_H_
#define _TINYPBRT_INTEGRATOR_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup integrator-public Integrator
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Integrator type.
	 * @ingroup integrator-public
	 */
	typedef enum tpbrt_integrator_type_t {
		/// @brief Ambient occlusion (accessibility over the hemisphere).
		TPBRT_INTEGRATOR_TYPE_AMBIENT_OCCLUSION = 0,
		/// @brief Bidirectional path tracing.
		TPBRT_INTEGRATOR_TYPE_BDPT				= 1,
		/// @brief Path tracing starting from the light sources.
		TPBRT_INTEGRATOR_TYPE_LIGHT_PATH		= 2,
		/// @brief Metropolis light transport using bidirectional path tracing.
		TPBRT_INTEGRATOR_TYPE_MLT				= 3,
		/// @brief Path tracing.
		TPBRT_INTEGRATOR_TYPE_PATH				= 4,
		/// @brief Rendering using a simple random walk without any explicit light sampling.
		TPBRT_INTEGRATOR_TYPE_RANDOM_WALK		= 5,
		/// @brief Path tracing with very basic sampling algorithms.
		TPBRT_INTEGRATOR_TYPE_SIMPLE_PATH		= 6,
		/// @brief Volumetric path tracing with very basic sampling algorithms.
		TPBRT_INTEGRATOR_TYPE_SIMPLE_VOL_PATH	= 7,
		/// @brief Stochastic progressive photon mapping.
		TPBRT_INTEGRATOR_TYPE_SPPM				= 8,
		/// @brief Volumetric path tracing.
		TPBRT_INTEGRATOR_TYPE_VOL_PATH			= 9,
		/// @brief Count of available integrator types.
		TPBRT_INTEGRATOR_TYPE_MAX_NUM			= 10,
	} tpbrt_integrator_type_t;

	/**
	 * @brief Integrator light sampler type.
	 * @ingroup integrator-public
	 */
	typedef enum tpbrt_integrator_light_sampler_t {
		/// @brief BVH over the lights.
		TPBRT_INTEGRATOR_LIGHT_SAMPLER_BVH	   = 0,
		/// @brief Uniform light sampling.
		TPBRT_INTEGRATOR_LIGHT_SAMPLER_UNIFORM = 1,
		/// @brief Sampling lights according to their power.
		TPBRT_INTEGRATOR_LIGHT_SAMPLER_POWER   = 2,
		/// @brief Count of available integrator light samplers.
		TPBRT_INTEGRATOR_LIGHT_SAMPLER_MAX_NUM = 3,
	} tpbrt_integrator_light_sampler_t;

	/**
	 * @brief Ambient Occlusion integrator parameters.
	 * @ingroup integrator-public
	 */
	typedef struct tpbrt_integrator_ambient_occlusion_params_t {
		/// @brief Distance after which occlusion should be ignored.
		tpbrt_float_t max_distance;
		/// @brief Controls whether uniform hemisphere sampling or cosine-weighted sampling should be used for ambient occlusion sample rays.
		tpbrt_bool_t cos_sample : 1;
	} tpbrt_integrator_ambient_occlusion_params_t;

	/**
	 * @brief BDPT integrator parameters.
	 * @ingroup integrator-public
	 */
	typedef struct tpbrt_integrator_bdpt_params_t {
		/// @brief Maximum length of a light-carrying path sampled by the integrator.
		tpbrt_uint_t max_depth;
		/// @brief Controls whether "path regularization" is performed. (i.e., a roughening of specular vertices after a non-specular scattering event which can reduce high-variance error in tricky lighting situations.).
		tpbrt_bool_t regularize			  : 1;
		/// @brief If true, an image is saved for each (s,t) bidirectional path generation strategy used by the integrator. These images can be helpful for understanding which sampling strategies are effective for sampling various types of light transport paths.
		tpbrt_bool_t visualize_strategies : 1;
		/// @brief If true, an image is saved with the multiple importance sampling weights for each (s,t) bidirectional path generation strategy. These images can be helpful for understanding which sampling strategies are effective for sampling various types of light transport paths.
		tpbrt_bool_t visualize_weights	  : 1;
	} tpbrt_integrator_bdpt_params_t;

	/**
	 * @brief Light path integrator parameters.
	 * @ingroup integrator-public
	 */
	typedef struct tpbrt_integrator_light_path_params_t {
		/// @brief Maximum length of a light-carrying path sampled by the integrator.
		tpbrt_uint_t max_depth;
	} tpbrt_integrator_light_path_params_t;

	/**
	 * @brief MLT integrator parameters.
	 * @ingroup integrator-public
	 */
	typedef struct tpbrt_integrator_mlt_params_t {
		/// @brief Maximum length of a light-carrying path sampled by the integrator.
		tpbrt_uint_t max_depth;
		/// @brief Number of samples to take during the "bootstrap" phase; some of these samples are used for initial light-carrying paths for the Metropolis algorithm.
		tpbrt_uint_t bootstrap_samples;
		/// @brief Number of unique Markov chains chains to follow with the Metropolis algorithm. (Each chain starts with a new path from the bootstrap phase.).
		tpbrt_uint_t chains;
		/// @brief Number of path mutations to apply per pixel in the image. (Note that each pixel will generally receive more or fewer path contributions, depending on how bright the pixel is. For the most part, this is the most effective parameter to increase to improve image quality.
		tpbrt_uint_t mutations_per_pixel;
		/// @brief Probability of discarding the current path and generating a new random path (versus applying a small mutation to the current path). For scenes with very difficult-to-sample light transport paths, reducing this probability may be worthwhile.
		tpbrt_float_t large_step_probability;
		/// @brief Standard deviation of the perturbation applied to random samples in [0,1] used for small path mutations.
		tpbrt_float_t sigma;
		/// @brief Controls whether "path regularization" is performed. (i.e., a roughening of specular vertices after a non-specular scattering event which can reduce high-variance error in tricky lighting situations.).
		tpbrt_bool_t regularize : 1;
	} tpbrt_integrator_mlt_params_t;

	/**
	 * @brief Path integrator parameters.
	 * @ingroup integrator-public
	 */
	typedef struct tpbrt_integrator_path_params_t {
		/// @brief Maximum length of a light-carrying path sampled by the integrator.
		tpbrt_uint_t max_depth;
		/// @brief Light sampling algorithm to be used.
		tpbrt_integrator_light_sampler_t light_sampler : 3;
		/// @brief Controls whether "path regularization" is performed. (i.e., a roughening of specular vertices after a non-specular scattering event which can reduce high-variance error in tricky lighting situations.).
		tpbrt_bool_t regularize						   : 1;
	} tpbrt_integrator_path_params_t;

	/**
	 * @brief Random walk integrator parameters.
	 * @ingroup integrator-public
	 */
	typedef struct tpbrt_integrator_random_walk_params_t {
		/// @brief Maximum length of a light-carrying path sampled by the integrator.
		tpbrt_uint_t max_depth;
	} tpbrt_integrator_random_walk_params_t;

	/**
	 * @brief Simple path integrator parameters.
	 * @ingroup integrator-public
	 */
	typedef struct tpbrt_integrator_simple_path_params_t {
		/// @brief Maximum length of a light-carrying path sampled by the integrator.
		tpbrt_uint_t max_depth;
		/// @brief Use the BSDFs' importance sampling routines to sample ray directions. (Uniform spherical sampling is used otherwise.).
		tpbrt_bool_t sample_bsdf   : 1;
		/// @brief Use the lights' importance sampling routines for direct lighting calculations. (No light sampling is used otherwise and lights must be randomly intersected by rays for illumination to be found.).
		tpbrt_bool_t sample_lights : 1;
	} tpbrt_integrator_simple_path_params_t;

	/**
	 * @brief Simple VOL path integrator parameters.
	 * @ingroup integrator-public
	 */
	typedef struct tpbrt_integrator_simple_vol_path_params_t {
		/// @brief Maximum length of a light-carrying path sampled by the integrator.
		tpbrt_uint_t max_depth;
	} tpbrt_integrator_simple_vol_path_params_t;

	/**
	 * @brief SPPM integrator parameters.
	 * @ingroup integrator-public
	 */
	typedef struct tpbrt_integrator_sppm_params_t {
		/// @brief Maximum length of a light-carrying path sampled by the integrator.
		tpbrt_uint_t max_depth;
		/// @brief Number of photons to shoot from light sources in each iteration. With the default value, -1, the number is automatically set to be equal to the number of pixels in the image.
		tpbrt_int_t photons_per_iteration;
		/// @brief Initial photon search radius. (This value will be reduced as photons are accumulated at each pixel.)
		tpbrt_float_t radius;
		/// @brief Seed for random number generator used to randomize sample generation for photon rays.
		tpbrt_int_t seed;
	} tpbrt_integrator_sppm_params_t;

	/**
	 * @brief VOL path integrator parameters.
	 * @ingroup integrator-public
	 */
	typedef struct tpbrt_integrator_vol_path_params_t {
		/// @brief Maximum length of a light-carrying path sampled by the integrator.
		tpbrt_uint_t max_depth;
		/// @brief Light sampling algorithm to be used.
		tpbrt_integrator_light_sampler_t light_sampler : 3;
		/// @brief Controls whether "path regularization" is performed. (i.e., a roughening of specular vertices after a non-specular scattering event which can reduce high-variance error in tricky lighting situations.).
		tpbrt_bool_t regularize						   : 1;
	} tpbrt_integrator_vol_path_params_t;

	/**
	 * @brief PBRT integrator representation.
	 * @ingroup integrator-public
	 */
	typedef struct tpbrt_integrator_t {
		/// @brief Value dependent on type value.
		union {
			/// @brief Ambient occlusion integrator parameters.
			tpbrt_integrator_ambient_occlusion_params_t ambient_occlusion;
			/// @brief BDPT integrator parameters.
			tpbrt_integrator_bdpt_params_t bdpt;
			/// @brief Light path integrator parameters.
			tpbrt_integrator_light_path_params_t light_path;
			/// @brief MLT integrator parameters.
			tpbrt_integrator_mlt_params_t mlt;
			/// @brief Path integrator parameters.
			tpbrt_integrator_path_params_t path;
			/// @brief Random walk integrator parameters.
			tpbrt_integrator_random_walk_params_t random_walk;
			/// @brief Simple path integrator parameters.
			tpbrt_integrator_simple_path_params_t simple_path;
			/// @brief Simple vol path integrator parameters.
			tpbrt_integrator_simple_vol_path_params_t simple_vol_path;
			/// @brief SPPM integrator parameters.
			tpbrt_integrator_sppm_params_t sppm;
			/// @brief VOL path integrator parameters.
			tpbrt_integrator_vol_path_params_t vol_path;
		} as;

		/// @brief Value type.
		tpbrt_integrator_type_t type : 5;
	} tpbrt_integrator_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_INTEGRATOR_H_
