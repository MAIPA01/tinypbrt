#pragma once
#ifndef _TINYPBRT_FILTER_INTERNAL_H_
#define _TINYPBRT_FILTER_INTERNAL_H_

#include <tinypbrt/detail/filter.h>

#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_create_default_filter(tpbrt_filter_t* filter);

	tpbrt_error_t tpbrt_create_filter(const tpbrt_string_t* type, const tpbrt_params_list_t* params, tpbrt_filter_t* filter);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_FILTER_INTERNAL_H_
