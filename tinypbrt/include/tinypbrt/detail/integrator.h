#pragma once
#ifndef _TINYPBRT_INTEGRATOR_H_
#define _TINYPBRT_INTEGRATOR_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_INTEGRATOR_TYPE_AMBIENT_OCCLUSION = 0,
		TPBRT_INTEGRATOR_TYPE_BDPT				= 1,
		TPBRT_INTEGRATOR_TYPE_LIGHT_PATH		= 2,
		TPBRT_INTEGRATOR_TYPE_MLT				= 3,
		TPBRT_INTEGRATOR_TYPE_PATH				= 4,
		TPBRT_INTEGRATOR_TYPE_RANDOM_WALK		= 5,
		TPBRT_INTEGRATOR_TYPE_SIMPLE_PATH		= 6,
		TPBRT_INTEGRATOR_TYPE_SIMPLE_VOL_PATH	= 7,
		TPBRT_INTEGRATOR_TYPE_SPPM				= 8,
		TPBRT_INTEGRATOR_TYPE_VOL_PATH			= 9,
		TPBRT_INTEGRATOR_TYPE_MAX_NUM			= 10,
	} tpbrt_integrator_type_t;

	typedef enum {
		TPBRT_INTEGRATOR_LIGHT_SAMPLER_BVH	   = 0,
		TPBRT_INTEGRATOR_LIGHT_SAMPLER_UNIFORM = 1,
		TPBRT_INTEGRATOR_LIGHT_SAMPLER_POWER   = 2,
		TPBRT_INTEGRATOR_LIGHT_SAMPLER_MAX_NUM = 3,
	} tpbrt_integrator_light_sampler_t;

	typedef struct {
		tpbrt_bool_t cos_sample;
		tpbrt_float_t max_distance;
	} tpbrt_integrator_ambient_occlusion_params_t;

	typedef struct {
		tpbrt_uint_t max_depth;
		tpbrt_bool_t regularize;
		tpbrt_bool_t visualize_strategies;
		tpbrt_bool_t visualize_weights;
	} tpbrt_integrator_bdpt_params_t;

	typedef struct {
		tpbrt_uint_t max_depth;
	} tpbrt_integrator_light_path_params_t;

	typedef struct {
		tpbrt_uint_t max_depth;
		tpbrt_bool_t regularize;
		tpbrt_uint_t bootstrap_samples;
		tpbrt_uint_t chains;
		tpbrt_uint_t mutations_per_pixel;
		tpbrt_float_t large_step_probability;
		tpbrt_float_t sigma;
	} tpbrt_integrator_mlt_params_t;

	typedef struct {
		tpbrt_uint_t max_depth;
		tpbrt_integrator_light_sampler_t light_sampler;
		tpbrt_bool_t regularize;
	} tpbrt_integrator_path_params_t;

	typedef struct {
		tpbrt_uint_t max_depth;
	} tpbrt_integrator_random_walk_params_t;

	typedef struct {
		tpbrt_uint_t max_depth;
		tpbrt_bool_t sample_bsdf;
		tpbrt_bool_t sample_lights;
	} tpbrt_integrator_simple_path_params_t;

	typedef struct {
		tpbrt_uint_t max_depth;
		tpbrt_bool_t regularize;
	} tpbrt_integrator_simple_vol_path_params_t;

	typedef struct {
		tpbrt_uint_t max_depth;
		tpbrt_int_t photons_per_iteration;
		tpbrt_float_t radius;
		tpbrt_int_t seed;
	} tpbrt_integrator_sppm_params_t;

	typedef struct {
		tpbrt_uint_t max_depth;
		tpbrt_integrator_light_sampler_t light_sampler;
	} tpbrt_integrator_vol_path_params_t;

	typedef struct {
		tpbrt_integrator_type_t type;

		union {
			tpbrt_integrator_ambient_occlusion_params_t ambient_occlusion;
			tpbrt_integrator_bdpt_params_t bdpt;
			tpbrt_integrator_light_path_params_t light_path;
			tpbrt_integrator_mlt_params_t mlt;
			tpbrt_integrator_path_params_t path;
			tpbrt_integrator_random_walk_params_t random_walk;
			tpbrt_integrator_simple_path_params_t simple_path;
			tpbrt_integrator_simple_vol_path_params_t simple_vol_path;
			tpbrt_integrator_sppm_params_t sppm;
			tpbrt_integrator_vol_path_params_t vol_path;
		} as;
	} tpbrt_integrator_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_INTEGRATOR_H_
