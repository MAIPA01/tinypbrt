#pragma once
#ifndef _TINYPBRT_COORD_HPP_
#define _TINYPBRT_COORD_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/coord.h>

namespace tpbrt {
	/**
	 * @defgroup coord-cpp-public Coordinates
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @copybrief tpbrt_coord_system_type_t
	 * @ingroup coord-cpp-public
	 */
	enum class coord_system_type : char_t {
		/// @copybrief TPBRT_COORD_SYSTEM_TYPE_CAMERA_WORLD
		CAMERA_WORLD = TPBRT_COORD_SYSTEM_TYPE_CAMERA_WORLD,
		/// @copybrief TPBRT_COORD_SYSTEM_TYPE_CAMERA
		CAMERA		 = TPBRT_COORD_SYSTEM_TYPE_CAMERA,
		/// @copybrief TPBRT_COORD_SYSTEM_TYPE_WORLD
		WORLD		 = TPBRT_COORD_SYSTEM_TYPE_WORLD,
		/// @copybrief TPBRT_COORD_SYSTEM_TYPE_MAX_NUM
		MAX_NUM		 = TPBRT_COORD_SYSTEM_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts coord_system_type enum value to string.
	 * @param type Coordinate system type.
	 * @return String value of coordinate system type.
	 * @ingroup coord-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const coord_system_type type) noexcept {
		using enum coord_system_type;
			switch (type) {
			case CAMERA_WORLD: return "cameraworld";
			case CAMERA:	   return "camera";
			case WORLD:		   return "world";
			case MAX_NUM:	   return "maxnum";
			default:		   return "UNKNOWN";
			}
	}
} // namespace tpbrt
#endif // _TINYPBRT_COORD_HPP_
