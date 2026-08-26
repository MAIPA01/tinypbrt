#pragma once
#ifndef _TINYPBRT_COORD_H_
#define _TINYPBRT_COORD_H_

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum tpbrt_coordinate_system_t : uint8_t {
		TPBRT_COORDINATE_SYSTEM_CAMERA_WORLD = 0,
		TPBRT_COORDINATE_SYSTEM_CAMERA		 = 1,
		TPBRT_COORDINATE_SYSTEM_WORLD		 = 2,
		TPBRT_COORDINATE_SYSTEM_MAX_NUM		 = 3,
	} tpbrt_coordinate_system_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_COORD_H_
