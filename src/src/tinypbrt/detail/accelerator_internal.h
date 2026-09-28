#pragma once
#ifndef _TINYPBRT_ACCELERATOR_INTERNAL_H_
#define _TINYPBRT_ACCELERATOR_INTERNAL_H_

#include <tinypbrt/detail/accelerator.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_accelerator_type_from_string(tpbrt_string_t type_str, tpbrt_accelerator_type_t* type);

	tpbrt_error_t tpbrt_accelerator_bvh_split_method_from_string(tpbrt_string_t method_str,
	  tpbrt_accelerator_bvh_split_method_t* method);

	tpbrt_error_t tpbrt_accelerator_init(tpbrt_string_t type_str, tpbrt_params_list_t params, tpbrt_accelerator_t* accelerator);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_ACCELERATOR_INTERNAL_H_
