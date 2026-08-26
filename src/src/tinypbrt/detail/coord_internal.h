#pragma once
#ifndef _TINYPBRT_COORD_INTERNAL_H_
#define _TINYPBRT_COORD_INTERNAL_H_

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/coord.h>
#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_coordinate_system_from_string(const tpbrt_string_t* coord_str, tpbrt_coordinate_system_t* coord);

#ifdef __cplusplus
}
#endif
#endif // TINYPBRT_COORD_INTERNAL_H
