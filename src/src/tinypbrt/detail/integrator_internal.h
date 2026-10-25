#pragma once
#ifndef _TINYPBRT_INTEGRATOR_INTERNAL_H_
#define _TINYPBRT_INTEGRATOR_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/integrator.h>

#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_integrator_type_from_string(tpbrt_string_t type_str, tpbrt_integrator_type_t* type);

	tpbrt_error_t tpbrt_integrator_light_sampler_from_string(tpbrt_string_t sampler_str,
	  tpbrt_integrator_light_sampler_t* sampler);

	tpbrt_error_t tpbrt_integrator_init_default(tpbrt_integrator_t* integrator);

	tpbrt_error_t tpbrt_integrator_init(tpbrt_string_t type_str, const tpbrt_params_list_t* params,
	  tpbrt_integrator_t* integrator);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_INTEGRATOR_INTERNAL_H_
