#pragma once
#ifndef _TINYPBRT_INTEGRATOR_H_
#define _TINYPBRT_INTEGRATOR_H_

#include <tinypbrt/detail/fwd.h>

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

	typedef enum {
		TPBRT_INTEGRATOR_LIGHT_SAMPLER_BVH	   = 0,
		TPBRT_INTEGRATOR_LIGHT_SAMPLER_UNIFORM = 1,
		TPBRT_INTEGRATOR_LIGHT_SAMPLER_POWER   = 2,
	} tpbrt_integrator_light_sampler_t;

	typedef struct {
		tpbrt_bool_t cos_sample;
		tpbrt_float_t max_distance;
	} tpbrt_integrator_ambient_occlusion_params_t;

	typedef struct {
		tpbrt_bool_t visualize_strategies;
		tpbrt_bool_t visualize_weights;
	} tpbrt_integrator_bdpt_params_t;

	typedef struct {
		tpbrt_uint_t bootstrap_samples;
		tpbrt_uint_t chains;
		tpbrt_uint_t mutations_per_pixel;
		tpbrt_float_t large_step_probability;
		tpbrt_float_t sigma;
	} tpbrt_integrator_mlt_params_t;

	typedef struct {
		tpbrt_bool_t sample_bsdf;
		tpbrt_bool_t sample_lights;
	} tpbrt_integrator_simple_path_params_t;

	typedef struct {
		tpbrt_int_t photons_per_iteration;
		tpbrt_float_t radius;
		tpbrt_uint_t seed;
	} tpbrt_integrator_sppm_params_t;

	typedef struct {
		tpbrt_integrator_type_t type;
		tpbrt_uint_t max_depth;
		tpbrt_integrator_light_sampler_t light_sampler;
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
