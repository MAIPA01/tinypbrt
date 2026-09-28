#pragma once
#ifndef _TINYPBRT_COORD_H_
#define _TINYPBRT_COORD_H_

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum tpbrt_coord_system_type_t : uint8_t {
		TPBRT_COORD_SYSTEM_TYPE_CAMERA_WORLD = 0,
		TPBRT_COORD_SYSTEM_TYPE_CAMERA		 = 1,
		TPBRT_COORD_SYSTEM_TYPE_WORLD		 = 2,
		TPBRT_COORD_SYSTEM_TYPE_MAX_NUM		 = 3,
	} tpbrt_coord_system_type_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_COORD_H_
