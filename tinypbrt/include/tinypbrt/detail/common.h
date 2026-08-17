#pragma once
#ifndef _TINYPBRT_TYPES_H_
#define _TINYPBRT_TYPES_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		tpbrt_char_t* chars;
		tpbrt_size_t size;
	} tpbrt_string_t;

	typedef struct {
		tpbrt_string_t* values;
		tpbrt_size_t count;
	} tpbrt_string_array_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_TYPES_H_
