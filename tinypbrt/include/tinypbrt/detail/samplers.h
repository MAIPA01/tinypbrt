#pragma once
#ifndef _TINYPBRT_SAMPLERS_H_
#define _TINYPBRT_SAMPLERS_H_

#include <tinypbrt/detail/common.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_SAMPLER_HALTON	   = 0,
		TPBRT_SAMPLER_INDEPENDENT  = 1,
		TPBRT_SAMPLER_PADDED_SOBOL = 2,
		TPBRT_SAMPLER_SOBOL		   = 3,
		TPBRT_SAMPLER_STRATIFIED   = 4,
		TPBRT_SAMPLER_Z_SOBOL	   = 5,
	} tpbrt_sampler_type_t;

	typedef enum : uint8_t {
		TPBRT_SAMPLER_RANDOM_NONE = 0,
		TPBRT_SAMPLER_RANDOM_PERMUTE_DIGITS = 1,
		TPBRT_SAMPLER_RANDOM_OWEN = 2,
		TPBRT_SAMPLER_RANDOM_FAST_OWEN = 3,
	} tpbrt_sampler_random_t;

	typedef struct {
		tpbrt_sampler_type_t type;
		tpbrt_int_t seed;
		tpbrt_uint_t pixel_samples;
		tpbrt_sampler_random_t randomization;
		tpbrt_bool_t jitter;
		tpbrt_uint_t x_samples;
		tpbrt_uint_t y_samples;
	} tpbrt_sampler_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SAMPLERS_H_
