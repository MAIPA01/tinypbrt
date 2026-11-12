#pragma once
#ifndef _TINYPBRT_FILM_HPP_
#define _TINYPBRT_FILM_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/film.h>

#include <tinypbrt/detail/cpp/common.hpp>
#include <tinypbrt/detail/cpp/coord.hpp>

namespace tpbrt {
	/**
	 * @defgroup film-cpp-public Film
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @copybrief tpbrt_film_type_t
	 * @ingroup film-cpp-public
	 */
	enum class film_type : uchar_t {
		/// @copybrief TPBRT_FILM_TYPE_RGB
		RGB		 = TPBRT_FILM_TYPE_RGB,
		/// @copybrief TPBRT_FILM_TYPE_GBUFFER
		GBUFFER	 = TPBRT_FILM_TYPE_GBUFFER,
		/// @copybrief TPBRT_FILM_TYPE_SPECTRAL
		SPECTRAL = TPBRT_FILM_TYPE_SPECTRAL,
		/// @copybrief TPBRT_FILM_TYPE_MAX_NUM
		MAX_NUM	 = TPBRT_FILM_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts film_type enum value to string.
	 * @param type Film type.
	 * @return String value of film type.
	 * @ingroup film-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const film_type type) noexcept {
		using enum film_type;
			switch (type) {
			case RGB:	   return "rgb";
			case GBUFFER:  return "gbuffer";
			case SPECTRAL: return "spectral";
			case MAX_NUM:  return "maxnum";
			default:	   return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_film_gbuffer_params_t.
	 * @ingroup film-cpp-public
	 */
	struct film_gbuffer_params : detail::wrapper<film_gbuffer_params, tpbrt_film_gbuffer_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns coordinate_system value.
		[[nodiscard]] constexpr coord_system_type coordinate_system() const noexcept {
			return static_cast<coord_system_type>(_native->coordinate_system);
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(coordinate_system()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_film_spectral_params_t.
	 * @ingroup film-cpp-public
	 */
	struct film_spectral_params : detail::wrapper<film_spectral_params, tpbrt_film_spectral_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns nbuckets value.
		[[nodiscard]] constexpr uint_t nbuckets() const noexcept { return _native->nbuckets; }

		/// @brief Returns lambda_min value.
		[[nodiscard]] constexpr float_t lambda_min() const noexcept { return _native->lambda_min; }

		/// @brief Returns lambda_max value.
		[[nodiscard]] constexpr float_t lambda_max() const noexcept { return _native->lambda_max; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(nbuckets(), lambda_min(), lambda_max()); }
	};

	/**
	 * @copybrief tpbrt_film_sensor_t
	 * @ingroup film-cpp-public
	 */
	enum class film_sensor : uchar_t {
		/// @copybrief TPBRT_FILM_SENSOR_CIE_1931
		CIE_1931			= TPBRT_FILM_SENSOR_CIE_1931,
		/// @copybrief TPBRT_FILM_SENSOR_CANON_EOS_100D
		CANON_EOS_100D		= TPBRT_FILM_SENSOR_CANON_EOS_100D,
		/// @copybrief TPBRT_FILM_SENSOR_CANON_EOS_1DX_MKII
		CANON_EOS_1DX_MKII	= TPBRT_FILM_SENSOR_CANON_EOS_1DX_MKII,
		/// @copybrief TPBRT_FILM_SENSOR_CANON_EOS_200D
		CANON_EOS_200D		= TPBRT_FILM_SENSOR_CANON_EOS_200D,
		/// @copybrief TPBRT_FILM_SENSOR_CANON_EOS_200D_MKII
		CANON_EOS_200D_MKII = TPBRT_FILM_SENSOR_CANON_EOS_200D_MKII,
		/// @copybrief TPBRT_FILM_SENSOR_CANON_EOS_5D
		CANON_EOS_5D		= TPBRT_FILM_SENSOR_CANON_EOS_5D,
		/// @copybrief TPBRT_FILM_SENSOR_CANON_EOS_5D_MKII
		CANON_EOS_5D_MKII	= TPBRT_FILM_SENSOR_CANON_EOS_5D_MKII,
		/// @copybrief TPBRT_FILM_SENSOR_CANON_EOS_5D_MKIII
		CANON_EOS_5D_MKIII	= TPBRT_FILM_SENSOR_CANON_EOS_5D_MKIII,
		/// @copybrief TPBRT_FILM_SENSOR_CANON_EOS_5D_MKIV
		CANON_EOS_5D_MKIV	= TPBRT_FILM_SENSOR_CANON_EOS_5D_MKIV,
		/// @copybrief TPBRT_FILM_SENSOR_CANON_EOS_5DS
		CANON_EOS_5DS		= TPBRT_FILM_SENSOR_CANON_EOS_5DS,
		/// @copybrief TPBRT_FILM_SENSOR_CANON_EOS_M
		CANON_EOS_M			= TPBRT_FILM_SENSOR_CANON_EOS_M,
		/// @copybrief TPBRT_FILM_SENSOR_HASSELBLAD_L1D_20C
		HASSELBLAD_L1D_20C	= TPBRT_FILM_SENSOR_HASSELBLAD_L1D_20C,
		/// @copybrief TPBRT_FILM_SENSOR_NIKON_D810
		NIKON_D810			= TPBRT_FILM_SENSOR_NIKON_D810,
		/// @copybrief TPBRT_FILM_SENSOR_NIKON_D850
		NIKON_D850			= TPBRT_FILM_SENSOR_NIKON_D850,
		/// @copybrief TPBRT_FILM_SENSOR_SONY_ILCE_6400
		SONY_ILCE_6400		= TPBRT_FILM_SENSOR_SONY_ILCE_6400,
		/// @copybrief TPBRT_FILM_SENSOR_SONY_ILCE_7M3
		SONY_ILCE_7M3		= TPBRT_FILM_SENSOR_SONY_ILCE_7M3,
		/// @copybrief TPBRT_FILM_SENSOR_SONY_ILCE_7RM3
		SONY_ILCE_7RM3		= TPBRT_FILM_SENSOR_SONY_ILCE_7RM3,
		/// @copybrief TPBRT_FILM_SENSOR_SONY_ILCE_9
		SONY_ILCE_9			= TPBRT_FILM_SENSOR_SONY_ILCE_9,
		/// @copybrief TPBRT_FILM_SENSOR_MAX_NUM
		MAX_NUM				= TPBRT_FILM_SENSOR_MAX_NUM,
	};

	/**
	 * @brief Converts film_sensor enum value to string.
	 * @param type Film sensor.
	 * @return String value of film sensor.
	 * @ingroup film-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const film_sensor type) noexcept {
		using enum film_sensor;
			switch (type) {
			case CIE_1931:			  return "cie1931";
			case CANON_EOS_100D:	  return "canon_eos_100d";
			case CANON_EOS_1DX_MKII:  return "canon_eos_1dx_mkii";
			case CANON_EOS_200D:	  return "canon_eos_200d";
			case CANON_EOS_200D_MKII: return "canon_eos_200d_mkii";
			case CANON_EOS_5D:		  return "canon_eos_5d";
			case CANON_EOS_5D_MKII:	  return "canon_eos_5d_mkii";
			case CANON_EOS_5D_MKIII:  return "canon_eos_5d_mkiii";
			case CANON_EOS_5D_MKIV:	  return "canon_eos_5d_mkiv";
			case CANON_EOS_5DS:		  return "canon_eos_5ds";
			case CANON_EOS_M:		  return "canon_eos_m";
			case HASSELBLAD_L1D_20C:  return "hasselblad_l1d_20c";
			case NIKON_D810:		  return "nikon_d810";
			case NIKON_D850:		  return "nikon_d850";
			case SONY_ILCE_6400:	  return "sony_ilce_6400";
			case SONY_ILCE_7M3:		  return "sony_ilce_7m3";
			case SONY_ILCE_7RM3:	  return "sony_ilce_7rm3";
			case SONY_ILCE_9:		  return "sony_ilce_9";
			case MAX_NUM:			  return "maxnum";
			default:				  return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_film_t.
	 * @ingroup film-cpp-public
	 */
	struct film : detail::wrapper<film, tpbrt_film_t>,
				  detail::with_variant<film, film_gbuffer_params, film_spectral_params> {
		/// @brief Type used to read crop_window parameter.
		using crop_window_type	= array<tpbrt_float_t[4], float_t>;

		/// @brief Type used to read pixel_bounds parameter.
		using pixel_bounds_type = array<tpbrt_uint_t[4], uint_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns crop_window value.
		[[nodiscard]] constexpr crop_window_type crop_window() const noexcept { return _native->crop_window; }

		/// @brief Returns pixel_bounds value.
		[[nodiscard]] constexpr pixel_bounds_type pixel_bounds() const noexcept { return _native->pixel_bounds; }

		/// @brief Returns file_name value.
		[[nodiscard]] constexpr string file_name() const noexcept { return _native->file_name; }

		/// @brief Returns x_resolution value.
		[[nodiscard]] constexpr uint_t x_resolution() const noexcept { return _native->x_resolution; }

		/// @brief Returns y_resolution value.
		[[nodiscard]] constexpr uint_t y_resolution() const noexcept { return _native->y_resolution; }

		/// @brief Returns diagonal value.
		[[nodiscard]] constexpr float_t diagonal() const noexcept { return _native->diagonal; }

		/// @brief Returns iso value.
		[[nodiscard]] constexpr float_t iso() const noexcept { return _native->iso; }

		/// @brief Returns white_balance value.
		[[nodiscard]] constexpr float_t white_balance() const noexcept { return _native->white_balance; }

		/// @brief Returns max_component_value value.
		[[nodiscard]] constexpr float_t max_component_value() const noexcept { return _native->max_component_value; }

		/// @brief Returns sensor value.
		[[nodiscard]] constexpr film_sensor sensor() const noexcept { return static_cast<film_sensor>(_native->sensor); }

		/// @brief Returns save_fp16 value.
		[[nodiscard]] constexpr bool_t save_fp16() const noexcept { return _native->save_fp16 == True; }

		/// @brief Returns type value.
		[[nodiscard]] constexpr film_type type() const noexcept { return static_cast<film_type>(_native->type); }

		/// @brief Returns true if type is gbuffer.
		[[nodiscard]] constexpr bool_t is_gbuffer() const noexcept { return type() == film_type::GBUFFER; }

		/// @brief Returns true if type is spectral.
		[[nodiscard]] constexpr bool_t is_spectral() const noexcept { return type() == film_type::SPECTRAL; }

		/// @brief Returns film parameters as gbuffer.
		[[nodiscard]] constexpr std::optional<film_gbuffer_params> as_gbuffer() const noexcept {
				if (!is_gbuffer()) { return std::nullopt; }
			return _native->as.gbuffer;
		}

		/// @brief Returns film parameters as spectral.
		[[nodiscard]] constexpr std::optional<film_spectral_params> as_spectral() const noexcept {
				if (!is_spectral()) { return std::nullopt; }
			return _native->as.spectral;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum film_type;
				switch (type()) {
				case GBUFFER:  return _native->as.gbuffer;
				case SPECTRAL: return _native->as.spectral;
				case MAX_NUM:  [[fallthrough]];
				default:	   break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept {
			return std::make_tuple(crop_window(), pixel_bounds(), file_name(), x_resolution(), y_resolution(), diagonal(), iso(),
			  white_balance(), max_component_value(), to_variant(), type(), sensor(), save_fp16());
		}
	};
} // namespace tpbrt
#endif // _TINYPBRT_FILM_HPP_
