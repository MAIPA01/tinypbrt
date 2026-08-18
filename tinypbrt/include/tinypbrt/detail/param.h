#pragma once
#ifndef _TINYPBRT_PARAM_H_
#define _TINYPBRT_PARAM_H_

#include <tinypbrt/detail/common.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_PARAM_TYPE_INTEGER   = 0,
		TPBRT_PARAM_TYPE_FLOAT	   = 1,
		TPBRT_PARAM_TYPE_POINT2	   = 2,
		TPBRT_PARAM_TYPE_VECTOR2   = 3,
		TPBRT_PARAM_TYPE_POINT3	   = 4,
		TPBRT_PARAM_TYPE_POINT	   = TPBRT_PARAM_TYPE_POINT3,
		TPBRT_PARAM_TYPE_VECTOR3   = 5,
		TPBRT_PARAM_TYPE_VECTOR	   = TPBRT_PARAM_TYPE_VECTOR3,
		TPBRT_PARAM_TYPE_NORMAL3   = 6,
		TPBRT_PARAM_TYPE_NORMAL	   = TPBRT_PARAM_TYPE_NORMAL3,
		TPBRT_PARAM_TYPE_SPECTRUM  = 7,
		TPBRT_PARAM_TYPE_RGB	   = 8,
		TPBRT_PARAM_TYPE_BLACKBODY = 9,
		TPBRT_PARAM_TYPE_BOOL	   = 10,
		TPBRT_PARAM_TYPE_STRING	   = 11,
		TPBRT_PARAM_TYPE_TEXTURE   = 12,
	} tpbrt_param_type_t;

	typedef enum : uint8_t {
		TPBRT_PARAM_VALUE_TYPE_SINGLE = 0,
		TPBRT_PARAM_VALUE_TYPE_ARRAY  = 1,
		TPBRT_PARAM_VALUE_TYPE_STRING = 2,
	} tpbrt_param_value_type_t;

	typedef struct {
		tpbrt_string_t name;
		tpbrt_param_type_t type;
		tpbrt_param_value_type_t value_type;
		tpbrt_string_t value;
	} tpbrt_param_t;

	typedef struct {
		tpbrt_param_t* params;
		tpbrt_size_t count;
	} tpbrt_params_list_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_PARAM_H_
