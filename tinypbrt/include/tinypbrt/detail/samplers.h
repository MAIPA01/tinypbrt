#pragma once
#ifndef _TINYPBRT_SAMPLERS_H_
#define _TINYPBRT_SAMPLERS_H_

#include "param.h"

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

	typedef struct {
		tpbrt_sampler_type_t type;
		int32_t seed;
		uint32_t pixel_samples;
		tpbrt_string_t randomization;
		tpbrt_bool_t jitter;
		uint32_t x_samples;
		uint32_t y_samples;
	} tpbrt_sampler_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SAMPLERS_H_
