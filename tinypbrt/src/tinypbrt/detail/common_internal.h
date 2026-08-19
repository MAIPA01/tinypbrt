#pragma once
#ifndef _TINYPBRT_COMMON_INTERNAL_H_
#define _TINYPBRT_COMMON_INTERNAL_H_

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_copy_string(tpbrt_string_t* dst, const tpbrt_string_t* src);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_COMMON_INTERNAL_H_