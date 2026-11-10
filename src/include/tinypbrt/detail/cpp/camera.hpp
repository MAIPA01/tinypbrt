#pragma once
#ifndef _TINYPBRT_CAMERA_HPP_
#define _TINYPBRT_CAMERA_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/camera.h>

#include <tinypbrt/detail/cpp/common.hpp>
#include <tinypbrt/detail/cpp/media.hpp>
#include <tinypbrt/detail/cpp/transform.hpp>

#include <algorithm>
#include <array>
#include <span>
#include <string_view>
#include <type_traits>

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
	struct camera_orthographic_params {
		/// @brief Native C library type for this struct.
		using native_type  = tpbrt_camera_orthographic_params_t;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<float_t, std::span<const float_t, 4>, float_t, float_t>;

	private:
		/// @brief C bvh parameters struct.
		const native_type& _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr camera_orthographic_params(const native_type& native) noexcept : _native(native) {} // NOLINT

		/// @brief Returns frame_aspect_ratio value.
		[[nodiscard]] constexpr float_t frame_aspect_ratio() const noexcept { return _native.frame_aspect_ratio; }

		/// @brief Returns screen_window value.
		[[nodiscard]] constexpr std::span<const float_t, 4> screen_window() const noexcept {
			return std::span(_native.screen_window);
		}

		/// @brief Returns lens_radius value.
		[[nodiscard]] constexpr float_t lens_radius() const noexcept { return _native.lens_radius; }

		/// @brief Returns focal_distance value.s
		[[nodiscard]] constexpr float_t focal_distance() const noexcept { return _native.focal_distance; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(frame_aspect_ratio(), screen_window(), lens_radius(), focal_distance());
		}

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to pointer to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const camera_orthographic_params& rhs) const noexcept {
			auto to_cmp_tuple = [](const camera_orthographic_params& params) noexcept {
				return std::make_tuple(params.frame_aspect_ratio(), std::to_array(params._native.screen_window),
				  params.lens_radius(), params.focal_distance());
			};

			return to_cmp_tuple(*this) <=> to_cmp_tuple(rhs);
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const camera_orthographic_params& rhs) const noexcept {
			return frame_aspect_ratio() == rhs.frame_aspect_ratio() && lens_radius() == rhs.lens_radius() &&
				   focal_distance() == rhs.focal_distance() &&
				   std::ranges::equal(_native.screen_window, rhs._native.screen_window);
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_camera_perspective_params_t.
	 * @ingroup camera-cpp-public
	 */
	struct camera_perspective_params {
		/// @brief Native C library type for this struct.
		using native_type  = tpbrt_camera_perspective_params_t;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<float_t, std::span<const float_t, 4>, float_t, float_t, float_t>;

	private:
		/// @brief C native struct.
		const native_type& _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr camera_perspective_params(const native_type& native) noexcept // NOLINT
			: _native(native) {}

		/// @brief Returns frame_aspect_ratio value.
		[[nodiscard]] constexpr float_t frame_aspect_ratio() const noexcept { return _native.frame_aspect_ratio; }

		/// @brief Returns screen_window value.
		[[nodiscard]] constexpr std::span<const float_t, 4> screen_window() const noexcept {
			return std::span(_native.screen_window);
		}

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

		/// @brief Conversion operator to native type.
		[[nodiscard]] operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to pointer to native type.
		[[nodiscard]] operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const camera_perspective_params& rhs) const noexcept {
			auto to_cmp_tuple = [](const camera_perspective_params& params) noexcept {
				return std::make_tuple(params.frame_aspect_ratio(), std::to_array(params._native.screen_window),
				  params.lens_radius(), params.focal_distance());
			};

			return to_cmp_tuple(*this) <=> to_cmp_tuple(rhs);
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const camera_perspective_params& rhs) const noexcept {
			return frame_aspect_ratio() == rhs.frame_aspect_ratio() && lens_radius() == rhs.lens_radius() &&
				   focal_distance() == rhs.focal_distance() && fov() == rhs.fov() &&
				   std::ranges::equal(_native.screen_window, rhs._native.screen_window);
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
	struct camera_spherical_params {
		/// @brief Native C library type for this struct.
		using native_type  = tpbrt_camera_spherical_params_t;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<camera_spherical_mapping>;

	private:
		/// @brief C native struct.
		const native_type& _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr camera_spherical_params(const tpbrt_camera_spherical_params_t& native) noexcept : _native(native) {} // NOLINT

		/// @brief Returns mapping value.
		[[nodiscard]] constexpr camera_spherical_mapping mapping() const noexcept {
			return static_cast<camera_spherical_mapping>(_native.mapping);
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(mapping()); }

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to pointer to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const camera_spherical_params& rhs) const noexcept {
			return mapping() <=> rhs.mapping();
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const camera_spherical_params& rhs) const noexcept {
			return mapping() == rhs.mapping();
		}
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
	struct camera_realistic_aperture {
		/// @brief Native C library type for this struct.
		using native_type  = tpbrt_camera_realistic_aperture_t;

		/// @brief Type used for better reading tagged union of parameters.
		using variant_type = detail::optional_variant<camera_realistic_aperture_builtin, string>;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<variant_type, camera_realistic_aperture_type>;

	private:
		/// @brief C native struct.
		const native_type& _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr camera_realistic_aperture(const native_type& native) noexcept : _native(native) {} // NOLINT

		/// @brief Returns type value.
		[[nodiscard]] constexpr camera_realistic_aperture_type type() const noexcept {
			return static_cast<camera_realistic_aperture_type>(_native.type);
		}

		/// @brief Returns if type is file name.
		[[nodiscard]] constexpr bool is_file_name() const noexcept { return type() == camera_realistic_aperture_type::FILE_NAME; }

		/// @brief Returns if type is builtin.
		[[nodiscard]] constexpr bool is_builtin() const noexcept { return type() == camera_realistic_aperture_type::BUILTIN; }

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

		/// @brief Visits camera realistic aperture parameters.
		constexpr auto visit(const auto& visitor) const noexcept {
			const variant_type res = to_variant();

			using return_type	   = decltype(std::visit(visitor, res.value()));

				if constexpr (std::is_void_v<return_type>) {
						if (res.has_value()) { return std::visit(visitor, res.value()); }
				}
				else {
						if (res.has_value()) { return std::optional<return_type>(std::visit(visitor, res.value())); }
					return std::optional<return_type>(std::nullopt);
				}
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(to_variant(), type()); }

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to pointer to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const camera_realistic_aperture& rhs) const noexcept {
			return to_variant() <=> rhs.to_variant();
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const camera_realistic_aperture& rhs) const noexcept {
				if (type() != rhs.type()) { return false; }
			return to_variant() == rhs.to_variant();
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_camera_realistic_params_t.
	 * @ingroup camera-cpp-public
	 */
	struct camera_realistic_params {
		/// @brief Native C library type for this struct.
		using native_type  = tpbrt_camera_realistic_params_t;

		/// @brief Type used for reflection.
		using reflect_type = std::tuple<string, float_t, float_t, camera_realistic_aperture>;

	private:
		/// @brief C native struct.
		const native_type& _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr camera_realistic_params(const native_type& native) noexcept : _native(native) {} // NOLINT

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

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to pointer to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const camera_realistic_params& rhs) const noexcept {
			return reflect() <=> rhs.reflect();
		}

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const camera_realistic_params& rhs) const noexcept {
			return lens_file() == rhs.lens_file() && aperture_diameter() == rhs.aperture_diameter() &&
				   focus_distance() == rhs.focus_distance() && aperture() == rhs.aperture();
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_camera_t.
	 * @ingroup camera-cpp-public
	 */
	struct camera {
		/// @brief Native C library type for this struct.
		using native_type  = tpbrt_camera_t;

		/// @brief Type used for better reading tagged union of parameters.
		using variant_type = detail::optional_variant<camera_orthographic_params, camera_perspective_params,
		  camera_spherical_params, camera_realistic_params>;

		/// @brief Type used for reflection.
		using reflect_type =
		  std::tuple<float_t, float_t, transform_animated, media_handle, media_handle, variant_type, camera_type>;

	private:
		/// @brief C native struct.
		const native_type& _native;

	public:
		/// @brief C to C++ wrapper conversion constructor.
		constexpr camera(const native_type& native) noexcept : _native(native) {} // NOLINT

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
		[[nodiscard]] constexpr bool is_orthographic() const noexcept { return type() == camera_type::ORTHOGRAPHIC; }

		/// @brief Returns if type is perspective.
		[[nodiscard]] constexpr bool is_perspective() const noexcept { return type() == camera_type::PERSPECTIVE; }

		/// @brief Returns if type is spherical.
		[[nodiscard]] constexpr bool is_spherical() const noexcept { return type() == camera_type::SPHERICAL; }

		/// @brief Returns if type is realistic.
		[[nodiscard]] constexpr bool is_realistic() const noexcept { return type() == camera_type::REALISTIC; }

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

		/// @brief Visits camera parameters.
		constexpr auto visit(const auto& visitor) const noexcept {
			const variant_type res = to_variant();

			using return_type	   = decltype(std::visit(visitor, res.value()));

				if constexpr (std::is_void_v<return_type>) {
						if (res.has_value()) { return std::visit(visitor, res.value()); }
				}
				else {
						if (res.has_value()) { return std::optional<return_type>(std::visit(visitor, res.value())); }
					return std::optional<return_type>(std::nullopt);
				}
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(shutter_open(), shutter_close(), transform(), interior_media_handle(), exterior_media_handle(),
			  to_variant(), type());
		}

		/// @brief Conversion operator to native type.
		[[nodiscard]] constexpr operator const native_type&() const noexcept { return _native; } // NOLINT

		/// @brief Conversion operator to pointer to native type.
		[[nodiscard]] constexpr operator const native_type*() const noexcept { return &_native; } // NOLINT

		/// @brief Space ship operator.
		[[nodiscard]] constexpr auto operator<=>(const camera& rhs) const noexcept { return reflect() <=> rhs.reflect(); }

		/// @brief Equal operator.
		[[nodiscard]] constexpr bool operator==(const camera& rhs) const noexcept {
			return type() == rhs.type() && shutter_open() == rhs.shutter_open() && shutter_close() == rhs.shutter_close() &&
				   transform() == rhs.transform() && interior_media_handle() == rhs.interior_media_handle() &&
				   exterior_media_handle() == rhs.exterior_media_handle() && to_variant() == rhs.to_variant();
		}
	};
} // namespace tpbrt
#endif // _TINYPBRT_CAMERA_HPP_
