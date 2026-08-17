#pragma once
#ifndef _TINYPBRT_PARAM_INTERNAL_H_
#define _TINYPBRT_PARAM_INTERNAL_H_

#include <tinypbrt/detail/param.h>
#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_param_type_t_from_string(const tpbrt_string_t* type_str, tpbrt_param_type_t* type_out);

	tpbrt_error_t tpbrt_fill_param(const tpbrt_string_t* type_and_name, const tpbrt_string_t* value);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_PARAM_INTERNAL_H_
