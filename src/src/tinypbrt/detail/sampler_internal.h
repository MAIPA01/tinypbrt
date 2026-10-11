#pragma once
#ifndef _TINYPBRT_SAMPLER_INTERNAL_H_
#define _TINYPBRT_SAMPLER_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/sampler.h>

#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_sampler_type_from_string(tpbrt_string_t type_str, tpbrt_sampler_type_t* type);

	tpbrt_error_t tpbrt_sampler_random_from_string(tpbrt_string_t random_str, tpbrt_sampler_random_t* random);

	tpbrt_error_t tpbrt_sampler_init_default(tpbrt_sampler_t* sampler);

	tpbrt_error_t tpbrt_sampler_init(tpbrt_string_t type_str, tpbrt_params_list_t params, tpbrt_sampler_t* sampler);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SAMPLER_INTERNAL_H_
