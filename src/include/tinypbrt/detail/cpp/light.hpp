#pragma once
#ifndef _TINYPBRT_LIGHT_HPP_
#define _TINYPBRT_LIGHT_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/light.h>

#include <string_view>
#include <tuple>

#include <tinypbrt/detail/cpp/color.hpp>
#include <tinypbrt/detail/cpp/common.hpp>
#include <tinypbrt/detail/cpp/math.hpp>
#include <tinypbrt/detail/cpp/media.hpp>
#include <tinypbrt/detail/cpp/texture.hpp>
#include <tinypbrt/detail/cpp/transform.hpp>

namespace tpbrt {
	/**
	 * @defgroup light-cpp-public Light
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @copybrief tpbrt_light_source_type_t
	 * @ingroup light-cpp-public
	 */
	enum class light_source_type : uchar_t {
		/// @copybrief TPBRT_LIGHT_SOURCE_TYPE_DISTANT
		DISTANT		= TPBRT_LIGHT_SOURCE_TYPE_DISTANT,
		/// @copybrief TPBRT_LIGHT_SOURCE_TYPE_GONIOMETRIC
		GONIOMETRIC = TPBRT_LIGHT_SOURCE_TYPE_GONIOMETRIC,
		/// @copybrief TPBRT_LIGHT_SOURCE_TYPE_INFINITE
		INFINITE	= TPBRT_LIGHT_SOURCE_TYPE_INFINITE,
		/// @copybrief TPBRT_LIGHT_SOURCE_TYPE_POINT
		POINT		= TPBRT_LIGHT_SOURCE_TYPE_POINT,
		/// @copybrief TPBRT_LIGHT_SOURCE_TYPE_PROJECTION
		PROJECTION	= TPBRT_LIGHT_SOURCE_TYPE_PROJECTION,
		/// @copybrief TPBRT_LIGHT_SOURCE_TYPE_SPOT
		SPOT		= TPBRT_LIGHT_SOURCE_TYPE_SPOT,
		/// @copybrief TPBRT_LIGHT_SOURCE_TYPE_MAX_NUM
		MAX_NUM		= TPBRT_LIGHT_SOURCE_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts light_source_type enum value to string.
	 * @param type Light source type.
	 * @return String value of light source type.
	 * @ingroup light-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const light_source_type type) noexcept {
		using enum light_source_type;
			switch (type) {
			case DISTANT:	  return "distant";
			case GONIOMETRIC: return "goniometric";
			case INFINITE:	  return "infinite";
			case POINT:		  return "point";
			case PROJECTION:  return "projection";
			case SPOT:		  return "spot";
			case MAX_NUM:	  return "maxnum";
			default:		  return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_light_source_distant_params_t.
	 * @ingroup light-cpp-public
	 */
	struct light_source_distant_params : detail::wrapper<light_source_distant_params, tpbrt_light_source_distant_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns l value.
		[[nodiscard]] constexpr spectrum l() const noexcept { return _native->l; }

		/// @brief Returns p value.
		[[nodiscard]] constexpr point from() const noexcept { return _native->from; }

		/// @brief Returns to value.
		[[nodiscard]] constexpr point to() const noexcept { return _native->to; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(l(), from(), to()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_light_source_goniometric_params_t.
	 * @ingroup light-cpp-public
	 */
	struct light_source_goniometric_params
		: detail::wrapper<light_source_goniometric_params, tpbrt_light_source_goniometric_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns file_name value.
		[[nodiscard]] constexpr string file_name() const noexcept { return _native->file_name; }

		/// @brief Returns i value.
		[[nodiscard]] constexpr spectrum i() const noexcept { return _native->i; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(file_name(), i()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_light_source_infinite_params_t.
	 * @ingroup light-cpp-public
	 */
	struct light_source_infinite_params : detail::wrapper<light_source_infinite_params, tpbrt_light_source_infinite_params_t> {
		/// @brief Used to read portal array.
		using portal_type = array<tpbrt_point3_t[4], point3>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns file_name value.
		[[nodiscard]] constexpr string file_name() const noexcept { return _native->file_name; }

		/// @brief Returns portal value.
		[[nodiscard]] constexpr portal_type portal() const noexcept { return _native->portal; }

		/// @brief Returns l value.
		[[nodiscard]] constexpr spectrum l() const noexcept { return _native->l; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(file_name(), portal(), l()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_light_source_point_params_t.
	 * @ingroup light-cpp-public
	 */
	struct light_source_point_params : detail::wrapper<light_source_point_params, tpbrt_light_source_point_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns i value.
		[[nodiscard]] constexpr spectrum i() const noexcept { return _native->i; }

		/// @brief Returns from value.
		[[nodiscard]] constexpr point from() const noexcept { return _native->from; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(i(), from()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_light_source_projection_params_t.
	 * @ingroup light-cpp-public
	 */
	struct light_source_projection_params
		: detail::wrapper<light_source_projection_params, tpbrt_light_source_projection_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns i value.
		[[nodiscard]] constexpr spectrum i() const noexcept { return _native->i; }

		/// @brief Returns file_name value.
		[[nodiscard]] constexpr string file_name() const noexcept { return _native->file_name; }

		/// @brief Returns fov value.
		[[nodiscard]] constexpr float_t fov() const noexcept { return _native->fov; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(i(), file_name(), fov()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_light_source_spot_params_t.
	 * @ingroup light-cpp-public
	 */
	struct light_source_spot_params : detail::wrapper<light_source_spot_params, tpbrt_light_source_spot_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns i value.
		[[nodiscard]] constexpr spectrum i() const noexcept { return _native->i; }

		/// @brief Returns from value.
		[[nodiscard]] constexpr point from() const noexcept { return _native->from; }

		/// @brief Returns to value.
		[[nodiscard]] constexpr point to() const noexcept { return _native->to; }

		/// @brief Returns cone_angle value.
		[[nodiscard]] constexpr float_t cone_angle() const noexcept { return _native->cone_angle; }

		/// @brief Returns cone_delta_angle value.
		[[nodiscard]] constexpr float_t cone_delta_angle() const noexcept { return _native->cone_delta_angle; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept {
			return std::make_tuple(i(), from(), to(), cone_angle(), cone_delta_angle());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_light_source_t.
	 * @ingroup light-cpp-public
	 */
	struct light_source
		: detail::wrapper<light_source, tpbrt_light_source_t>,
		  detail::with_variant<light_source, light_source_distant_params, light_source_goniometric_params,
			light_source_infinite_params, light_source_point_params, light_source_projection_params, light_source_spot_params> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns transform value.
		[[nodiscard]] constexpr transform_handle transform() const noexcept { return _native->transform; }

		/// @brief Returns scale value.
		[[nodiscard]] constexpr float_t scale() const noexcept { return _native->scale; }

		/// @brief Returns power_illuminance value.
		[[nodiscard]] constexpr opt_float power_illuminance() const noexcept { return _native->power_illuminance; }

		/// @brief Returns interior_media_handle value.
		[[nodiscard]] constexpr media_handle interior_media_handle() const noexcept { return _native->interior_media_handle; }

		/// @brief Returns exterior_media_handle value.
		[[nodiscard]] constexpr media_handle exterior_media_handle() const noexcept { return _native->exterior_media_handle; }

		/// @brief Returns type value.
		[[nodiscard]] constexpr light_source_type type() const noexcept { return static_cast<light_source_type>(_native->type); }

		/// @brief Returns true if type is distant.
		[[nodiscard]] constexpr bool_t is_distant() const noexcept { return type() == light_source_type::DISTANT; }

		/// @brief Returns true if type is goniometric.
		[[nodiscard]] constexpr bool_t is_goniometric() const noexcept { return type() == light_source_type::GONIOMETRIC; }

		/// @brief Returns true if type is infinite.
		[[nodiscard]] constexpr bool_t is_infinite() const noexcept { return type() == light_source_type::INFINITE; }

		/// @brief Returns true if type is point.
		[[nodiscard]] constexpr bool_t is_point() const noexcept { return type() == light_source_type::POINT; }

		/// @brief Returns true if type is projection.
		[[nodiscard]] constexpr bool_t is_projection() const noexcept { return type() == light_source_type::PROJECTION; }

		/// @brief Returns true if type is spot.
		[[nodiscard]] constexpr bool_t is_spot() const noexcept { return type() == light_source_type::SPOT; }

		/// @brief Returns light parameters as distant.
		[[nodiscard]] constexpr std::optional<light_source_distant_params> as_distant() const noexcept {
				if (!is_distant()) { return std::nullopt; }
			return _native->as.distant;
		}

		/// @brief Returns light parameters as goniometric.
		[[nodiscard]] constexpr std::optional<light_source_goniometric_params> as_goniometric() const noexcept {
				if (!is_goniometric()) { return std::nullopt; }
			return _native->as.goniometric;
		}

		/// @brief Returns light parameters as infinite.
		[[nodiscard]] constexpr std::optional<light_source_infinite_params> as_infinite() const noexcept {
				if (!is_infinite()) { return std::nullopt; }
			return _native->as.infinite;
		}

		/// @brief Returns light parameters as point.
		[[nodiscard]] constexpr std::optional<light_source_point_params> as_point() const noexcept {
				if (!is_point()) { return std::nullopt; }
			return _native->as.point;
		}

		/// @brief Returns light parameters as projection.
		[[nodiscard]] constexpr std::optional<light_source_projection_params> as_projection() const noexcept {
				if (!is_projection()) { return std::nullopt; }
			return _native->as.projection;
		}

		/// @brief Returns light parameters as spot.
		[[nodiscard]] constexpr std::optional<light_source_spot_params> as_spot() const noexcept {
				if (!is_spot()) { return std::nullopt; }
			return _native->as.spot;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum light_source_type;
				switch (type()) {
				case DISTANT:	  return _native->as.distant;
				case GONIOMETRIC: return _native->as.goniometric;
				case INFINITE:	  return _native->as.infinite;
				case POINT:		  return _native->as.point;
				case PROJECTION:  return _native->as.projection;
				case SPOT:		  return _native->as.spot;
				case MAX_NUM:	  [[fallthrough]];
				default:		  break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept {
			return std::make_tuple(transform(), scale(), power_illuminance(), interior_media_handle(), exterior_media_handle(),
			  to_variant(), type());
		}
	};

	/**
	 * @copybrief tpbrt_area_light_type_t
	 * @ingroup light-cpp-public
	 */
	enum class area_light_type : uchar_t {
		/// @copybrief TPBRT_AREA_LIGHT_TYPE_DIFFUSE
		DIFFUSE = TPBRT_AREA_LIGHT_TYPE_DIFFUSE,
		/// @copybrief TPBRT_AREA_LIGHT_TYPE_MAX_NUM
		MAX_NUM = TPBRT_AREA_LIGHT_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts area_light_type enum value to string.
	 * @param type Area light type.
	 * @return String value of area light type.
	 * @ingroup light-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const area_light_type type) noexcept {
		using enum area_light_type;
			switch (type) {
			case DIFFUSE: return "diffuse";
			case MAX_NUM: return "maxnum";
			default:	  return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_area_light_diffuse_params_t.
	 * @ingroup light-cpp-public
	 */
	struct area_light_diffuse_params : detail::wrapper<area_light_diffuse_params, tpbrt_area_light_diffuse_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns file_name value.
		[[nodiscard]] constexpr string file_name() const noexcept { return _native->file_name; }

		/// @brief Returns l value.
		[[nodiscard]] constexpr spectrum l() const noexcept { return _native->l; }

		/// @brief Returns two_sided value.
		[[nodiscard]] constexpr bool_t two_sided() const noexcept { return _native->two_sided; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(file_name(), l(), two_sided()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_area_light_t.
	 * @ingroup light-cpp-public
	 */
	struct area_light : detail::wrapper<area_light, tpbrt_area_light_t>,
						detail::with_variant<area_light, area_light_diffuse_params> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns idx value.
		[[nodiscard]] constexpr area_light_idx_t idx() const noexcept { return _native->idx; }

		/// @brief Returns scale value.
		[[nodiscard]] constexpr float_t scale() const noexcept { return _native->scale; }

		/// @brief Returns power value.
		[[nodiscard]] constexpr opt_float power() const noexcept { return _native->power; }

		/// @brief Returns type value.
		[[nodiscard]] constexpr area_light_type type() const noexcept { return static_cast<area_light_type>(_native->type); }

		/// @brief Returns true if type is diffuse.
		[[nodiscard]] constexpr bool_t is_diffuse() const noexcept { return type() == area_light_type::DIFFUSE; }

		/// @brief Returns diffuse parameters (std::nullopt if the type is different).
		[[nodiscard]] constexpr std::optional<area_light_diffuse_params> as_diffuse() const noexcept {
				if (!is_diffuse()) { return std::nullopt; }
			return _native->as.diffuse;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum area_light_type;
				switch (type()) {
				case DIFFUSE: return _native->as.diffuse;
				case MAX_NUM: [[fallthrough]];
				default:	  break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept {
			return std::make_tuple(idx(), scale(), power(), to_variant(), type());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_area_light_handle_t.
	 * @ingroup light-cpp-public
	 */
	struct area_light_handle : detail::wrapper<area_light_handle, tpbrt_area_light_handle_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns area_light_idx value.
		[[nodiscard]] constexpr area_light_idx_t area_light_idx() const noexcept { return _native->area_light_idx; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(area_light_idx()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_lights_sources_list_t.
	 * @ingroup light-cpp-public
	 */
	using lights_sources_list = array<tpbrt_lights_sources_list_t, light_source, &tpbrt_lights_sources_list_t::lights>;

	/**
	 * @brief C++ wrapper for tpbrt_area_lights_list_t.
	 * @details Index access (operator[], at(size_t), iteration) works as in tpbrt::array, at(handle) use
	 * the C lookups and return error::NOT_FOUND (or the C error code) when there is no such area light.
	 * @ingroup light-cpp-public
	 */
	struct area_lights_list : array<tpbrt_area_lights_list_t, area_light, &tpbrt_area_lights_list_t::lights> {
		/// @brief Base list constructor.
		using array::array;

		/// @brief Index based access from tpbrt::array.
		using array::at;

		/// @brief Operator for index based access from tpbrt::array.
		using array::operator[];

		/// @brief Returns area light pointed by given handle.
		[[nodiscard]] result<element_type> at(const area_light_handle handle) const noexcept {
			return detail::find_in<element_type, tpbrt_area_light_t>(tpbrt_get_area_light_by_handle, _native, handle.native());
		}

		/// @brief Returns element pointed by given handle.
		[[nodiscard]] result<element_type> operator[](const area_light_handle handle) const noexcept { return at(handle); }
	};

	/**
	 * @brief Returns area light to which handle is provided.
	 * @param list List of area lights in which area light given by handle should be.
	 * @param handle Handle to area light.
	 * @return Result with the area light or with error code (most likely error::NOT_FOUND).
	 * @ingroup light-cpp-public
	 */
	[[nodiscard]] inline result<area_light> get_area_light(const area_lights_list list, const area_light_handle handle) noexcept {
		return list.at(handle);
	}

	/**
	 * @brief C++ wrapper for tpbrt_lights_list_t.
	 * @ingroup light-cpp-public
	 */
	struct lights_list : detail::wrapper<lights_list, tpbrt_lights_list_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns lights_sources value.
		[[nodiscard]] constexpr lights_sources_list lights_sources() const noexcept { return _native->lights_sources; }

		/// @brief Returns area_lights value.
		[[nodiscard]] constexpr area_lights_list area_lights() const noexcept { return _native->area_lights; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(lights_sources(), area_lights()); }
	};
} // namespace tpbrt
#endif // _TINYPBRT_LIGHT_HPP_
