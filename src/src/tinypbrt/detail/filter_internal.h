#pragma once
#ifndef _TINYPBRT_FILTER_INTERNAL_H_
#define _TINYPBRT_FILTER_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/filter.h>

#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_filter_type_from_string(tpbrt_string_t type_str, tpbrt_filter_type_t* type);

	tpbrt_error_t tpbrt_filter_init_default(tpbrt_filter_t* filter);

	tpbrt_error_t tpbrt_filter_init(tpbrt_string_t type, const tpbrt_params_list_t* params, tpbrt_filter_t* filter);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_FILTER_INTERNAL_H_
