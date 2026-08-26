#pragma once
#ifndef _TINYPBRT_INTEGRATOR_INTERNAL_H_
#define _TINYPBRT_INTEGRATOR_INTERNAL_H_

#include <tinypbrt/detail/integrator.h>

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_create_integrator(const tpbrt_string_t* type_str, const tpbrt_params_list_t* params,
	  tpbrt_integrator_t* integrator);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_INTEGRATOR_INTERNAL_H_
