#pragma once
#ifndef _TINYPBRT_SCATTERING_INTERNAL_H_
#define _TINYPBRT_SCATTERING_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/scattering.h>

#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_scattering_builtin_from_string(tpbrt_string_t builtin_str, tpbrt_scattering_builtin_t* builtin);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SCATTERING_INTERNAL_H_
