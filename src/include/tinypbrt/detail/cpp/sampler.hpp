#pragma once
#ifndef _TINYPBRT_SAMPLERS_H_
#define _TINYPBRT_SAMPLERS_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup sampler-public Sampler
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Sampler types.
	 * @ingroup sampler-public
	 */
	typedef enum tpbrt_sampler_type_t {
		/// @brief Halton sampler.
		TPBRT_SAMPLER_TYPE_HALTON		= 0,
		/// @brief Independent sampler.
		TPBRT_SAMPLER_TYPE_INDEPENDENT	= 1,
		/// @brief Padded sobol sampler.
		TPBRT_SAMPLER_TYPE_PADDED_SOBOL = 2,
		/// @brief Sobol sampler.
		TPBRT_SAMPLER_TYPE_SOBOL		= 3,
		/// @brief Stratified sampler.
		TPBRT_SAMPLER_TYPE_STRATIFIED	= 4,
		/// @brief Z-Sobol sampler.
		TPBRT_SAMPLER_TYPE_Z_SOBOL		= 5,
		/// @brief Count of available sampler types.
		TPBRT_SAMPLER_TYPE_MAX_NUM		= 6,
	} tpbrt_sampler_type_t;

	/**
	 * @brief Sampler random type.
	 * @ingroup sampler-public
	 */
	typedef enum tpbrt_sampler_random_t {
		/// @brief Disables randomization.
		TPBRT_SAMPLER_RANDOM_NONE			= 0,
		/// @brief Applies a random permutation to the digits of the sample.
		TPBRT_SAMPLER_RANDOM_PERMUTE_DIGITS = 1,
		/// @brief Applies Owen scrambling.
		TPBRT_SAMPLER_RANDOM_OWEN			= 2,
		/// @brief Applies an efficient approximation to Owen scrambling.
		TPBRT_SAMPLER_RANDOM_FAST_OWEN		= 3,
		/// @brief Count of available random types.
		TPBRT_SAMPLER_RANDOM_MAX_NUM		= 4,
	} tpbrt_sampler_random_t;

	/**
	 * @brief Halton sampler parameters.
	 * @ingroup sampler-public
	 */
	typedef struct tpbrt_sampler_halton_params_t {
		/// @brief The number of samples to take in each pixel.
		tpbrt_uint_t pixel_samples;
		/// @brief Specifies a randomization algorithm to use.
		tpbrt_sampler_random_t randomization : 4;
	} tpbrt_sampler_halton_params_t;

	/**
	 * @brief Independent sampler parameters.
	 * @ingroup sampler-public
	 */
	typedef struct tpbrt_sampler_independent_params_t {
		/// @brief The number of samples to take in each pixel.
		tpbrt_uint_t pixel_samples;
	} tpbrt_sampler_independent_params_t;

	/**
	 * @brief Padded sobol sampler parameters.
	 * @ingroup sampler-public
	 */
	typedef struct tpbrt_sampler_padded_sobol_params_t {
		/// @brief The number of samples to take in each pixel.
		tpbrt_uint_t pixel_samples;
		/// @brief Specifies a randomization algorithm to use.
		tpbrt_sampler_random_t randomization : 4;
	} tpbrt_sampler_padded_sobol_params_t;

	/**
	 * @brief Sobol sampler parameters.
	 * @ingroup sampler-public
	 */
	typedef struct tpbrt_sampler_sobol_params_t {
		/// @brief The number of samples to take in each pixel.
		tpbrt_uint_t pixel_samples;
		/// @brief Specifies a randomization algorithm to use.
		tpbrt_sampler_random_t randomization : 4;
	} tpbrt_sampler_sobol_params_t;

	/**
	 * @brief Stratified sampler parameters.
	 * @ingroup sampler-public
	 */
	typedef struct tpbrt_sampler_stratified_params_t {
		/// @brief The number of samples per pixel to take in the x direction.
		tpbrt_uint_t x_samples;
		/// @brief The number of samples per pixel to take in the y direction. In general, "xsamples" and "ysamples" should be set to the same value for best results.
		tpbrt_uint_t y_samples;
		/// @brief Whether or not the generated samples should be jittered inside each stratum; this is generally only worth setting to "false" for comparisons between jittered and uniform sampling—uniform sampling will almost always give a worse result.
		tpbrt_bool_t jitter : 1;
	} tpbrt_sampler_stratified_params_t;

	/**
	 * @brief Z-Sobol sampler parameters.
	 * @ingroup sampler-public
	 */
	typedef struct tpbrt_sampler_z_sobol_params_t {
		/// @brief The number of samples to take in each pixel.
		tpbrt_uint_t pixel_samples;
		/// @brief Specifies a randomization algorithm to use.
		tpbrt_sampler_random_t randomization : 4;
	} tpbrt_sampler_z_sobol_params_t;

	/**
	 * @brief PBRT sampler representation.
	 * @ingroup sampler-public
	 */
	typedef struct tpbrt_sampler_t {
		/// @brief A seed to use for the pseudorandom number generator. The default seed value is zero, though the --seed command-line argument can be used to specify a different seed. If a seed is specified in the scene description file, it overrides a value specified on the command-line.
		tpbrt_int_t seed;

		/// @brief Value dependent on type value.
		union {
			/// @brief Halton sampler parameters.
			tpbrt_sampler_halton_params_t halton;
			/// @brief Independent sampler parameters.
			tpbrt_sampler_independent_params_t independent;
			/// @brief Padded sobol sampler parameters.
			tpbrt_sampler_padded_sobol_params_t padded_sobol;
			/// @brief Sobol sampler parameters.
			tpbrt_sampler_sobol_params_t sobol;
			/// @brief Stratified sampler parameters.
			tpbrt_sampler_stratified_params_t stratified;
			/// @brief Z-Sobol sampler parameters.
			tpbrt_sampler_z_sobol_params_t z_sobol;
		} as;

		/// @brief Sampler value type.
		tpbrt_sampler_type_t type : 4;
	} tpbrt_sampler_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SAMPLERS_H_
