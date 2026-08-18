#pragma once
#ifndef _TINYPBRT_PARAM_INTERNAL_H_
#define _TINYPBRT_PARAM_INTERNAL_H_

#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/param.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_create_param(const tpbrt_string_t* declaration, const tpbrt_string_t* value_str, tpbrt_param_t** param);

	void tpbrt_free_param(tpbrt_param_t** param);

	tpbrt_error_t tpbrt_create_empty_params_list(tpbrt_params_list_t** params_list);

	tpbrt_error_t tpbrt_add_param(tpbrt_params_list_t* params_list, const tpbrt_param_t* param);

	tpbrt_error_t tpbrt_get_param(const tpbrt_params_list_t* params_list, const tpbrt_string_t* param_name, const tpbrt_param_t** param);

	void tpbrt_free_params_list(tpbrt_params_list_t** params_list);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_PARAM_INTERNAL_H_
