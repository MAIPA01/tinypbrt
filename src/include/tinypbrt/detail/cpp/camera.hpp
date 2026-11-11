#pragma once
#ifndef _TINYPBRT_CAMERA_HPP_
#define _TINYPBRT_CAMERA_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/camera.h>

#include <string_view>

#include <tinypbrt/detail/cpp/common.hpp>
#include <tinypbrt/detail/cpp/media.hpp>
#include <tinypbrt/detail/cpp/transform.hpp>

namespace tpbrt {
	/**
	 * @defgroup camera-cpp-public Camera
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @copybrief tpbrt_camera_type_t
	 * @ingroup camera-cpp-public
	 */
	enum class camera_type : char_t {
		/// @copybrief TPBRT_CAMERA_TYPE_ORTHOGRAPHIC
		ORTHOGRAPHIC = TPBRT_CAMERA_TYPE_ORTHOGRAPHIC,
		/// @copybrief TPBRT_CAMERA_TYPE_PERSPECTIVE
		PERSPECTIVE	 = TPBRT_CAMERA_TYPE_PERSPECTIVE,
		/// @copybrief TPBRT_CAMERA_TYPE_REALISTIC
		REALISTIC	 = TPBRT_CAMERA_TYPE_REALISTIC,
		/// @copybrief TPBRT_CAMERA_TYPE_SPHERICAL
		SPHERICAL	 = TPBRT_CAMERA_TYPE_SPHERICAL,
		/// @copybrief TPBRT_CAMERA_TYPE_MAX_NUM
		MAX_NUM		 = TPBRT_CAMERA_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts camera_type enum value to string.
	 * @param type Camera type.
	 * @return String value of camera type.
	 * @ingroup camera-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const camera_type type) noexcept {
		using enum camera_type;
			switch (type) {
			case ORTHOGRAPHIC: return "orthographic";
			case PERSPECTIVE:  return "perspective";
			case REALISTIC:	   return "realistic";
			case SPHERICAL:	   return "spherical";
			case MAX_NUM:	   return "maxnum";
			default:		   return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_camera_orthographic_params_t.
	 * @ingroup camera-cpp-public
	 */
	struct camera_orthographic_params : wrapper<camera_orthographic_params, tpbrt_camera_orthographic_params_t> {
		/// @brief Type used to read screen_window parameter.
		using screen_window_array_type = array<tpbrt_float_t[4], float_t>;

		/// @brief Type used for reflection.
		using reflect_type			   = std::tuple<float_t, screen_window_array_type, float_t, float_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns frame_aspect_ratio value.
		[[nodiscard]] constexpr float_t frame_aspect_ratio() const noexcept { return _native.frame_aspect_ratio; }

		/// @brief Returns screen_window value.
		[[nodiscard]] constexpr screen_window_array_type screen_window() const noexcept { return _native.screen_window; }

		/// @brief Returns lens_radius value.
		[[nodiscard]] constexpr float_t lens_radius() const noexcept { return _native.lens_radius; }

		/// @brief Returns focal_distance value.s
		[[nodiscard]] constexpr float_t focal_distance() const noexcept { return _native.focal_distance; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(frame_aspect_ratio(), screen_window(), lens_radius(), focal_distance());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_camera_perspective_params_t.
	 * @ingroup camera-cpp-public
	 */
	struct camera_perspective_params : wrapper<camera_perspective_params, tpbrt_camera_perspective_params_t> {
		/// @brief Type used to read screen_window parameter.
		using screen_window_array_type = array<tpbrt_float_t[4], float_t>;

		/// @brief Type used for reflection.
		using reflect_type			   = std::tuple<float_t, screen_window_array_type, float_t, float_t, float_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns frame_aspect_ratio value.
		[[nodiscard]] constexpr float_t frame_aspect_ratio() const noexcept { return _native.frame_aspect_ratio; }

		/// @brief Returns screen_window value.
		[[nodiscard]] constexpr screen_window_array_type screen_window() const noexcept { return _native.screen_window; }

		/// @brief Returns lens_radius value.
		[[nodiscard]] constexpr float_t lens_radius() const noexcept { return _native.lens_radius; }

		/// @brief Returns focal_distance value.
		[[nodiscard]] constexpr float_t focal_distance() const noexcept { return _native.focal_distance; }

		/// @brief Returns fov value.
		[[nodiscard]] constexpr float_t fov() const noexcept { return _native.fov; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(frame_aspect_ratio(), screen_window(), lens_radius(), focal_distance(), fov());
		}
	};

	/**
	 * @copybrief tpbrt_camera_spherical_mapping_t
	 * @ingroup camera-cpp-public
	 */
	enum class camera_spherical_mapping : char_t {
		/// @copybrief TPBRT_CAMERA_SPHERICAL_MAPPING_EQUAL_AREA
		EQUAL_AREA		 = TPBRT_CAMERA_SPHERICAL_MAPPING_EQUAL_AREA,
		/// @copybrief TPBRT_CAMERA_SPHERICAL_MAPPING_EQUI_RECTANGULAR
		EQUI_RECTANGULAR = TPBRT_CAMERA_SPHERICAL_MAPPING_EQUI_RECTANGULAR,
		/// @copybrief TPBRT_CAMERA_SPHERICAL_MAPPING_MAX_NUM
		MAX_NUM			 = TPBRT_CAMERA_SPHERICAL_MAPPING_MAX_NUM,
	};

	/**
	 * @brief Converts camera_spherical_mapping enum value to string.
	 * @param mapping Camera spherical mapping.
	 * @return String value of camera spherical mapping.
	 * @ingroup camera-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const camera_spherical_mapping mapping) noexcept {
		using enum camera_spherical_mapping;
			switch (mapping) {
			case EQUAL_AREA:	   return "equalarea";
			case EQUI_RECTANGULAR: return "equirectangular";
			case MAX_NUM:		   return "maxnum";
			default:			   return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_camera_spherical_params_t.
	 * @ingroup camera-cpp-public
	 */
	struct camera_spherical_params : wrapper<camera_spherical_mapping, tpbrt_camera_spherical_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<camera_spherical_mapping>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns mapping value.
		[[nodiscard]] constexpr camera_spherical_mapping mapping() const noexcept {
			return static_cast<camera_spherical_mapping>(_native.mapping);
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(mapping()); }
	};

	/**
	 * @copybrief tpbrt_camera_realistic_aperture_type_t
	 * @ingroup camera-cpp-public
	 */
	enum class camera_realistic_aperture_type : char_t {
		/// @copybrief TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_FILE_NAME
		FILE_NAME = TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_FILE_NAME,
		/// @copybrief TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_BUILTIN
		BUILTIN	  = TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_BUILTIN,
		/// @copybrief TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_MAX_NUM
		MAX_NUM	  = TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts camera_realistic_aperture_type enum value to string.
	 * @param type Camera realistic aperture type.
	 * @return String value of camera realistic aperture type.
	 * @ingroup camera-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const camera_realistic_aperture_type type) noexcept {
		using enum camera_realistic_aperture_type;
			switch (type) {
			case FILE_NAME: return "filename";
			case BUILTIN:	return "builtin";
			case MAX_NUM:	return "maxnum";
			default:		return "UNKNOWN";
			}
	}

	/**
	 * @copybrief tpbrt_camera_realistic_aperture_builtin_t
	 * @ingroup camera-cpp-public
	 */
	enum class camera_realistic_aperture_builtin : char_t {
		/// @copybrief TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_CIRCULAR
		CIRCULAR = TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_CIRCULAR,
		/// @copybrief TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_GAUSSIAN
		GAUSSIAN = TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_GAUSSIAN,
		/// @copybrief TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_SQUARE
		SQUARE	 = TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_SQUARE,
		/// @copybrief TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_PENTAGON
		PENTAGON = TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_PENTAGON,
		/// @copybrief TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_STAR
		STAR	 = TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_STAR,
		/// @copybrief TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_MAX_NUM
		MAX_NUM	 = TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_MAX_NUM,
	};

	/**
	 * @brief Converts camera_realistic_aperture_builtin enum value to string.
	 * @param builtin Camera realistic aperture builtin.
	 * @return String value of camera realistic aperture builtin.
	 * @ingroup camera-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const camera_realistic_aperture_builtin builtin) noexcept {
		using enum camera_realistic_aperture_builtin;
			switch (builtin) {
			case CIRCULAR: return "circular";
			case GAUSSIAN: return "gaussian";
			case SQUARE:   return "square";
			case PENTAGON: return "pentagon";
			case STAR:	   return "star";
			case MAX_NUM:  return "maxnum";
			default:	   return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_camera_realistic_aperture_t.
	 * @ingroup camera-cpp-public
	 */
	struct camera_realistic_aperture : wrapper<camera_realistic_aperture, tpbrt_camera_realistic_aperture_t>,
									   with_variant<camera_realistic_aperture, camera_realistic_aperture_builtin, string> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<variant_type, camera_realistic_aperture_type>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns type value.
		[[nodiscard]] constexpr camera_realistic_aperture_type type() const noexcept {
			return static_cast<camera_realistic_aperture_type>(_native.type);
		}

		/// @brief Returns if type is file name.
		[[nodiscard]] constexpr bool_t is_file_name() const noexcept { return type() == camera_realistic_aperture_type::FILE_NAME; }

		/// @brief Returns if type is builtin.
		[[nodiscard]] constexpr bool_t is_builtin() const noexcept { return type() == camera_realistic_aperture_type::BUILTIN; }

		/// @brief Returns camera realistic aperture as file name.
		[[nodiscard]] constexpr std::optional<string> as_file_name() const noexcept {
				if (!is_file_name()) { return std::nullopt; }
			return _native.as.file_name;
		}

		/// @brief Returns camera realistic aperture as builtin.
		[[nodiscard]] constexpr std::optional<camera_realistic_aperture_builtin> as_builtin() const noexcept {
				if (!is_builtin()) { return std::nullopt; }
			return static_cast<camera_realistic_aperture_builtin>(_native.as.builtin);
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum camera_realistic_aperture_type;
				switch (type()) {
				case FILE_NAME: return _native.as.file_name;
				case BUILTIN:	return static_cast<camera_realistic_aperture_builtin>(_native.as.builtin);
				case MAX_NUM:	[[fallthrough]];
				default:		break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(to_variant(), type()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_camera_realistic_params_t.
	 * @ingroup camera-cpp-public
	 */
	struct camera_realistic_params : wrapper<camera_realistic_params, tpbrt_camera_realistic_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<string, float_t, float_t, camera_realistic_aperture>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns lens_file value.
		[[nodiscard]] constexpr string lens_file() const noexcept { return _native.lens_file; }

		/// @brief Returns aperture_diameter value.
		[[nodiscard]] constexpr float_t aperture_diameter() const noexcept { return _native.aperture_diameter; }

		/// @brief Returns focus_distance value.
		[[nodiscard]] constexpr float_t focus_distance() const noexcept { return _native.focus_distance; }

		/// @brief Returns aperture value.
		[[nodiscard]] constexpr camera_realistic_aperture aperture() const noexcept { return _native.aperture; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(lens_file(), aperture_diameter(), focus_distance(), aperture());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_camera_t.
	 * @ingroup camera-cpp-public
	 */
	struct camera : wrapper<camera, tpbrt_camera_t>,
					with_variant<camera, camera_orthographic_params, camera_perspective_params, camera_spherical_params,
					  camera_realistic_params> {
		/// @brief Type used for reflection.
		using reflect_type =
		  std::tuple<float_t, float_t, transform_animated, media_handle, media_handle, variant_type, camera_type>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns shutter_open value.
		[[nodiscard]] constexpr float_t shutter_open() const noexcept { return _native.shutter_open; }

		/// @brief Returns shutter_close value.
		[[nodiscard]] constexpr float_t shutter_close() const noexcept { return _native.shutter_close; }

		/// @brief Returns transform value.
		[[nodiscard]] constexpr transform_animated transform() const noexcept { return _native.transform; }

		/// @brief Returns interior_media_handle value.
		[[nodiscard]] constexpr media_handle interior_media_handle() const noexcept { return _native.interior_media_handle; }

		/// @brief Returns exterior_media_handle value.
		[[nodiscard]] constexpr media_handle exterior_media_handle() const noexcept { return _native.exterior_media_handle; }

		/// @brief Returns type value.
		[[nodiscard]] constexpr camera_type type() const noexcept { return static_cast<camera_type>(_native.type); }

		/// @brief Returns if type is orthographic.
		[[nodiscard]] constexpr bool_t is_orthographic() const noexcept { return type() == camera_type::ORTHOGRAPHIC; }

		/// @brief Returns if type is perspective.
		[[nodiscard]] constexpr bool_t is_perspective() const noexcept { return type() == camera_type::PERSPECTIVE; }

		/// @brief Returns if type is spherical.
		[[nodiscard]] constexpr bool_t is_spherical() const noexcept { return type() == camera_type::SPHERICAL; }

		/// @brief Returns if type is realistic.
		[[nodiscard]] constexpr bool_t is_realistic() const noexcept { return type() == camera_type::REALISTIC; }

		/// @brief Returns camera parameters as orthographic.
		[[nodiscard]] constexpr std::optional<camera_orthographic_params> as_orthographic() const noexcept {
				if (!is_orthographic()) { return std::nullopt; }
			return _native.as.orthographic;
		}

		/// @brief Returns camera parameters as perspective.
		[[nodiscard]] constexpr std::optional<camera_perspective_params> as_perspective() const noexcept {
				if (!is_perspective()) { return std::nullopt; }
			return _native.as.perspective;
		}

		/// @brief Returns camera parameters as spherical.
		[[nodiscard]] constexpr std::optional<camera_spherical_params> as_spherical() const noexcept {
				if (!is_spherical()) { return std::nullopt; }
			return _native.as.spherical;
		}

		/// @brief Returns camera parameters as realistic.
		[[nodiscard]] constexpr std::optional<camera_realistic_params> as_realistic() const noexcept {
				if (!is_realistic()) { return std::nullopt; }
			return _native.as.realistic;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum camera_type;
				switch (type()) {
				case ORTHOGRAPHIC: return _native.as.orthographic;
				case PERSPECTIVE:  return _native.as.perspective;
				case SPHERICAL:	   return _native.as.spherical;
				case REALISTIC:	   return _native.as.realistic;
				case MAX_NUM:	   [[fallthrough]];
				default:		   break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(shutter_open(), shutter_close(), transform(), interior_media_handle(), exterior_media_handle(),
			  to_variant(), type());
		}
	};
} // namespace tpbrt
#endif // _TINYPBRT_CAMERA_HPP_
