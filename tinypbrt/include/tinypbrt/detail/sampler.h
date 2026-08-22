#pragma once
#ifndef _TINYPBRT_SAMPLERS_H_
#define _TINYPBRT_SAMPLERS_H_

#include <tinypbrt/detail/common.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_SAMPLER_TYPE_HALTON		= 0,
		TPBRT_SAMPLER_TYPE_INDEPENDENT	= 1,
		TPBRT_SAMPLER_TYPE_PADDED_SOBOL = 2,
		TPBRT_SAMPLER_TYPE_SOBOL		= 3,
		TPBRT_SAMPLER_TYPE_STRATIFIED	= 4,
		TPBRT_SAMPLER_TYPE_Z_SOBOL		= 5,
		TPBRT_SAMPLER_TYPE_MAX_NUM		= 6,
	} tpbrt_sampler_type_t;

	typedef enum : uint8_t {
		TPBRT_SAMPLER_RANDOM_NONE			= 0,
		TPBRT_SAMPLER_RANDOM_PERMUTE_DIGITS = 1,
		TPBRT_SAMPLER_RANDOM_OWEN			= 2,
		TPBRT_SAMPLER_RANDOM_FAST_OWEN		= 3,
		TPBRT_SAMPLER_RANDOM_MAX_NUM		= 4,
	} tpbrt_sampler_random_t;

	typedef struct {
		tpbrt_uint_t pixel_samples;
		tpbrt_sampler_random_t randomization;
	} tpbrt_sampler_halton_params_t;

	typedef struct {
		tpbrt_uint_t pixel_samples;
	} tpbrt_sampler_independent_params_t;

	typedef struct {
		tpbrt_uint_t pixel_samples;
		tpbrt_sampler_random_t randomization;
	} tpbrt_sampler_padded_sobol_params_t;

	typedef struct {
		tpbrt_uint_t pixel_samples;
		tpbrt_sampler_random_t randomization;
	} tpbrt_sampler_sobol_params_t;

	typedef struct {
		tpbrt_bool_t jitter;
		tpbrt_uint_t x_samples;
		tpbrt_uint_t y_samples;
	} tpbrt_sampler_stratified_params_t;

	typedef struct {
		tpbrt_uint_t pixel_samples;
		tpbrt_sampler_random_t randomization;
	} tpbrt_sampler_z_sobol_params_t;

	typedef struct {
		tpbrt_sampler_type_t type;
		tpbrt_int_t seed;

		union {
			tpbrt_sampler_halton_params_t halton;
			tpbrt_sampler_independent_params_t independent;
			tpbrt_sampler_padded_sobol_params_t padded_sobol;
			tpbrt_sampler_sobol_params_t sobol;
			tpbrt_sampler_stratified_params_t stratified;
			tpbrt_sampler_z_sobol_params_t z_sobol;
		} as;
	} tpbrt_sampler_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SAMPLERS_H_
