#pragma once
#ifndef _TINYPBRT_ACCELERATOR_INTERNAL_H_
#define _TINYPBRT_ACCELERATOR_INTERNAL_H_

#include <tinypbrt/detail/accelerator.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_create_accelerator(const tpbrt_string_t* type_str, const tpbrt_params_list_t* params,
	  tpbrt_accelerator_t* accelerator);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_ACCELERATOR_INTERNAL_H_
