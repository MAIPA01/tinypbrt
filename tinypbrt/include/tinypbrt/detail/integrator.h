#pragma once
#ifndef _TINYPBRT_INTEGRATOR_H_
#define _TINYPBRT_INTEGRATOR_H_

#include "../param.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_INTEGRATOR_AMBIENT_OCCLUSION = 0,
		TPBRT_INTEGRATOR_BDPT			   = 1,
		TPBRT_INTEGRATOR_LIGHT_PATH		   = 2,
		TPBRT_INTEGRATOR_MLT			   = 3,
		TPBRT_INTEGRATOR_PATH			   = 4,
		TPBRT_INTEGRATOR_RANDOM_WALK	   = 5,
		TPBRT_INTEGRATOR_SIMPLE_PATH	   = 6,
		TPBRT_INTEGRATOR_SIMPLE_VOL_PATH   = 7,
		TPBRT_INTEGRATOR_SPPM			   = 8,
		TPBRT_INTEGRATOR_VOL_PATH		   = 9,
	} tpbrt_integrator_type_t;

	typedef struct {
		tpbrt_bool_t cos_sample;
		float max_distance;
	} tpbrt_integrator_ambient_occlusion_params_t;

	typedef struct {
		tpbrt_bool_t visualize_strategies;
		tpbrt_bool_t visualize_weights;
	} tpbrt_integrator_bdpt_params_t;

	typedef struct {
		uint32_t bootstrap_samples;
		uint32_t chains;
		uint32_t mutations_per_pixel;
		float large_step_probability;
		float sigma;
	} tpbrt_integrator_mlt_params_t;

	typedef struct {
		tpbrt_bool_t sample_bsdf;
		tpbrt_bool_t sample_lights;
	} tpbrt_integrator_simple_path_params_t;

	typedef struct {
		int32_t photons_per_iteration;
		float radius;
		uint32_t seed;
	} tpbrt_integrator_sppm_params_t;

	typedef struct {
		tpbrt_integrator_type_t type;
		uint32_t max_depth;
		tpbrt_string_t light_sampler;
		tpbrt_bool_t regularize;
		union {
			tpbrt_integrator_ambient_occlusion_params_t ambient_occlusion_params;
			tpbrt_integrator_bdpt_params_t bdpt_params;
			tpbrt_integrator_mlt_params_t mlt_params;
			tpbrt_integrator_simple_path_params_t simple_path_params;
			tpbrt_integrator_sppm_params_t sppm_params;
		};
	} tpbrt_integrator_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_INTEGRATOR_H_
