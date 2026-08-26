#pragma once
#ifndef _TINYPBRT_SAMPLER_INTERNAL_H_
#define _TINYPBRT_SAMPLER_INTERNAL_H_

#include <tinypbrt/detail/sampler.h>

#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_create_sampler(const tpbrt_string_t* type_str, const tpbrt_params_list_t* params,
	  tpbrt_sampler_t* sampler);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SAMPLER_INTERNAL_H_
