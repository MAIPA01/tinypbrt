#pragma once
#ifndef _TINYPBRT_OPTIONS_INTERNAL_H_
#define _TINYPBRT_OPTIONS_INTERNAL_H_

#include <tinypbrt/detail/options.h>

#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_options_init(tpbrt_options_t* options);
	tpbrt_error_t tpbrt_options_apply(tpbrt_options_t* options, const tpbrt_param_t* param);
	void tpbrt_options_free(tpbrt_options_t* options);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_OPTIONS_INTERNAL_H_
