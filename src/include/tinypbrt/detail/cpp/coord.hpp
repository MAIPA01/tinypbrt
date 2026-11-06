#pragma once
#ifndef _TINYPBRT_COORD_H_
#define _TINYPBRT_COORD_H_

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup coord-public Coordinates
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Coordinate System type.
	 * @ingroup coord-public
	 */
	typedef enum tpbrt_coord_system_type_t {
		/// @brief Translates the scene so that the camera is at the origin.
		TPBRT_COORD_SYSTEM_TYPE_CAMERA_WORLD = 0,
		/// @brief Uses camera space (performance may suffer due to the scene being rotated).
		TPBRT_COORD_SYSTEM_TYPE_CAMERA		 = 1,
		/// @brief Uses world space (accuracy may suffer due to floating-point precision).
		TPBRT_COORD_SYSTEM_TYPE_WORLD		 = 2,
		/// @brief Count of available coordinate system types.
		TPBRT_COORD_SYSTEM_TYPE_MAX_NUM		 = 3,
	} tpbrt_coord_system_type_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_COORD_H_
