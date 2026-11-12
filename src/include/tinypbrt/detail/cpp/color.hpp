#pragma once
#ifndef _TINYPBRT_COLOR_HPP_
#define _TINYPBRT_COLOR_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/color.h>

#include <optional>
#include <tuple>

#include <tinypbrt/detail/cpp/common.hpp>
#include <tinypbrt/detail/cpp/math.hpp>

namespace tpbrt {
	/**
	 * @defgroup color-cpp-public Color
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @copybrief tpbrt_color_space_t
	 * @ingroup color-cpp-public
	 */
	enum class color_space : uchar_t {
		/// @copybrief TPBRT_COLOR_SPACE_ACES_2065_1
		ACES_2065_1 = TPBRT_COLOR_SPACE_ACES_2065_1,
		/// @copybrief TPBRT_COLOR_SPACE_REC_2020
		REC_2020	= TPBRT_COLOR_SPACE_REC_2020,
		/// @copybrief TPBRT_COLOR_SPACE_DCI_P3
		DCI_P3		= TPBRT_COLOR_SPACE_DCI_P3,
		/// @copybrief TPBRT_COLOR_SPACE_SRGB
		SRGB		= TPBRT_COLOR_SPACE_SRGB,
		/// @copybrief TPBRT_COLOR_SPACE_MAX_NUM
		MAX_NUM		= TPBRT_COLOR_SPACE_MAX_NUM,
	};

	/**
	 * @brief Converts color_space enum value to string.
	 * @param space Color space.
	 * @return String value of color space.
	 * @ingroup color-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const color_space space) noexcept {
		using enum color_space;
			switch (space) {
			case ACES_2065_1: return "aces2065-1";
			case REC_2020:	  return "rec2020";
			case DCI_P3:	  return "dci-p3";
			case SRGB:		  return "srgb";
			case MAX_NUM:	  return "maxnum";
			default:		  return "UNKNOWN";
			}
	}

	/**
	 * @copybrief tpbrt_spectrum_type_t
	 * @ingroup color-cpp-public
	 */
	enum class spectrum_type : uchar_t {
		/// @copybrief TPBRT_SPECTRUM_TYPE_WAVELENGTHS
		WAVELENGTHS = TPBRT_SPECTRUM_TYPE_WAVELENGTHS,
		/// @copybrief TPBRT_SPECTRUM_TYPE_RGB
		RGB			= TPBRT_SPECTRUM_TYPE_RGB,
		/// @copybrief TPBRT_SPECTRUM_TYPE_BLACKBODY
		BLACKBODY	= TPBRT_SPECTRUM_TYPE_BLACKBODY,
		/// @copybrief TPBRT_SPECTRUM_TYPE_FILE
		FILE		= TPBRT_SPECTRUM_TYPE_FILE,
		/// @copybrief TPBRT_SPECTRUM_TYPE_BUILTIN
		BUILTIN		= TPBRT_SPECTRUM_TYPE_BUILTIN,
		/// @copybrief TPBRT_SPECTRUM_TYPE_CONSTANT
		CONSTANT	= TPBRT_SPECTRUM_TYPE_CONSTANT,
		/// @copybrief TPBRT_SPECTRUM_TYPE_MAX_NUM
		MAX_NUM		= TPBRT_SPECTRUM_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts spectrum_type enum value to string.
	 * @param type Spectrum type.
	 * @return String value of spectrum type.
	 * @ingroup color-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const spectrum_type type) noexcept {
		using enum spectrum_type;
			switch (type) {
			case WAVELENGTHS: return "wavelengths";
			case RGB:		  return "rgb";
			case BLACKBODY:	  return "blackbody";
			case FILE:		  return "file";
			case BUILTIN:	  return "builtin";
			case CONSTANT:	  return "constant";
			case MAX_NUM:	  return "maxnum";
			default:		  return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_wavelength_t.
	 * @ingroup color-cpp-public
	 */
	struct wavelength : detail::wrapper<wavelength, tpbrt_wavelength_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns wavelength value.
		[[nodiscard]] constexpr float_t lambda() const noexcept { return _native->wavelength; }

		/// @brief Returns value value.
		[[nodiscard]] constexpr float_t value() const noexcept { return _native->value; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(lambda(), value()); }
	};

	/**
	 * @copybrief tpbrt_spectrum_builtin_t
	 * @ingroup color-cpp-public
	 */
	enum class spectrum_builtin : uchar_t {
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_GLASS_BK7
		GLASS_BK7		= TPBRT_SPECTRUM_BUILTIN_GLASS_BK7,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_GLASS_BAF10
		GLASS_BAF10		= TPBRT_SPECTRUM_BUILTIN_GLASS_BAF10,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_GLASS_FK51A
		GLASS_FK51A		= TPBRT_SPECTRUM_BUILTIN_GLASS_FK51A,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_GLASS_LASF9
		GLASS_LASF9		= TPBRT_SPECTRUM_BUILTIN_GLASS_LASF9,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_GLASS_F5
		GLASS_F5		= TPBRT_SPECTRUM_BUILTIN_GLASS_F5,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_GLASS_F10
		GLASS_F10		= TPBRT_SPECTRUM_BUILTIN_GLASS_F10,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_GLASS_F11
		GLASS_F11		= TPBRT_SPECTRUM_BUILTIN_GLASS_F11,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_METAL_AG_ETA
		METAL_AG_ETA	= TPBRT_SPECTRUM_BUILTIN_METAL_AG_ETA,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_METAL_AG_K
		METAL_AG_K		= TPBRT_SPECTRUM_BUILTIN_METAL_AG_K,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_METAL_AL_ETA
		METAL_AL_ETA	= TPBRT_SPECTRUM_BUILTIN_METAL_AL_ETA,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_METAL_AL_K
		METAL_AL_K		= TPBRT_SPECTRUM_BUILTIN_METAL_AL_K,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_METAL_AU_ETA
		METAL_AU_ETA	= TPBRT_SPECTRUM_BUILTIN_METAL_AU_ETA,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_METAL_AU_K
		METAL_AU_K		= TPBRT_SPECTRUM_BUILTIN_METAL_AU_K,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_METAL_CU_ETA
		METAL_CU_ETA	= TPBRT_SPECTRUM_BUILTIN_METAL_CU_ETA,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_METAL_CU_K
		METAL_CU_K		= TPBRT_SPECTRUM_BUILTIN_METAL_CU_K,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_METAL_CU_ZN_ETA
		METAL_CU_ZN_ETA = TPBRT_SPECTRUM_BUILTIN_METAL_CU_ZN_ETA,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_METAL_CU_ZN_K
		METAL_CU_ZN_K	= TPBRT_SPECTRUM_BUILTIN_METAL_CU_ZN_K,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_METAL_MG_O_ETA
		METAL_MG_O_ETA	= TPBRT_SPECTRUM_BUILTIN_METAL_MG_O_ETA,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_METAL_MG_O_K
		METAL_MG_O_K	= TPBRT_SPECTRUM_BUILTIN_METAL_MG_O_K,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_METAL_TI_O2_ETA
		METAL_TI_O2_ETA = TPBRT_SPECTRUM_BUILTIN_METAL_TI_O2_ETA,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_METAL_TI_O2_K
		METAL_TI_O2_K	= TPBRT_SPECTRUM_BUILTIN_METAL_TI_O2_K,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_A
		STD_ILLUM_A		= TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_A,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D50
		STD_ILLUM_D50	= TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D50,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D65
		STD_ILLUM_D65	= TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D65,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F1
		STD_ILLUM_F1	= TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F1,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F2
		STD_ILLUM_F2	= TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F2,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F3
		STD_ILLUM_F3	= TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F3,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F4
		STD_ILLUM_F4	= TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F4,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F5
		STD_ILLUM_F5	= TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F5,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F6
		STD_ILLUM_F6	= TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F6,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F7
		STD_ILLUM_F7	= TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F7,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F8
		STD_ILLUM_F8	= TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F8,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F9
		STD_ILLUM_F9	= TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F9,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F10
		STD_ILLUM_F10	= TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F10,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F11
		STD_ILLUM_F11	= TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F11,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F12
		STD_ILLUM_F12	= TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F12,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_ILLUM_ACES_D60
		ILLUM_ACES_D60	= TPBRT_SPECTRUM_BUILTIN_ILLUM_ACES_D60,
		/// @copybrief TPBRT_SPECTRUM_BUILTIN_MAX_NUM
		MAX_NUM			= TPBRT_SPECTRUM_BUILTIN_MAX_NUM,
	};

	/**
	 * @brief Converts spectrum_builtin enum value to string.
	 * @param builtin Spectrum builtin.
	 * @return String value of spectrum builtin.
	 * @ingroup color-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const spectrum_builtin builtin) noexcept {
		using enum spectrum_builtin;
			switch (builtin) {
			case GLASS_BK7:		  return "glass-BK7";
			case GLASS_BAF10:	  return "glass-BAF10";
			case GLASS_FK51A:	  return "glass-FK51A";
			case GLASS_LASF9:	  return "glass-LASF9";
			case GLASS_F5:		  return "glass-F5";
			case GLASS_F10:		  return "glass-F10";
			case GLASS_F11:		  return "glass-F11";
			case METAL_AG_ETA:	  return "metal-Ag-eta";
			case METAL_AG_K:	  return "metal-Ag-k";
			case METAL_AL_ETA:	  return "metal-Al-eta";
			case METAL_AL_K:	  return "metal-Al-k";
			case METAL_AU_ETA:	  return "metal-Au-eta";
			case METAL_AU_K:	  return "metal-Au-k";
			case METAL_CU_ETA:	  return "metal-Cu-eta";
			case METAL_CU_K:	  return "metal-Cu-k";
			case METAL_CU_ZN_ETA: return "metal-CuZn-eta";
			case METAL_CU_ZN_K:	  return "metal-CuZn-k";
			case METAL_MG_O_ETA:  return "metal-MgO-eta";
			case METAL_MG_O_K:	  return "metal-MgO-k";
			case METAL_TI_O2_ETA: return "metal-TiO2-eta";
			case METAL_TI_O2_K:	  return "metal-TiO2-k";
			case STD_ILLUM_A:	  return "stdillum-A";
			case STD_ILLUM_D50:	  return "stdillum-D50";
			case STD_ILLUM_D65:	  return "stdillum-D65";
			case STD_ILLUM_F1:	  return "stdillum-F1";
			case STD_ILLUM_F2:	  return "stdillum-F2";
			case STD_ILLUM_F3:	  return "stdillum-F3";
			case STD_ILLUM_F4:	  return "stdillum-F4";
			case STD_ILLUM_F5:	  return "stdillum-F5";
			case STD_ILLUM_F6:	  return "stdillum-F6";
			case STD_ILLUM_F7:	  return "stdillum-F7";
			case STD_ILLUM_F8:	  return "stdillum-F8";
			case STD_ILLUM_F9:	  return "stdillum-F9";
			case STD_ILLUM_F10:	  return "stdillum-F10";
			case STD_ILLUM_F11:	  return "stdillum-F11";
			case STD_ILLUM_F12:	  return "stdillum-F12";
			case ILLUM_ACES_D60:  return "illum-acesD60";
			case MAX_NUM:		  return "maxnum";
			default:			  return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_blackbody_t (a separate type, so a blackbody spectrum is not confused with a constant one
	 * when visiting the spectrum variant - in C both are tpbrt_float_t).
	 * @ingroup color-cpp-public
	 */
	struct blackbody : detail::wrapper<blackbody, tpbrt_blackbody_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns blackbody temperature in Kelvin.
		[[nodiscard]] constexpr float_t temperature() const noexcept { return *_native; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(temperature()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_spectrum_t.
	 * @ingroup color-cpp-public
	 */
	struct spectrum : detail::wrapper<spectrum, tpbrt_spectrum_t>,
					  detail::with_variant<spectrum, wavelength_array, rgb, blackbody, string, spectrum_builtin, float_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns type value.
		[[nodiscard]] constexpr spectrum_type type() const noexcept { return static_cast<spectrum_type>(_native->type); }

		/// @brief Returns true if type is wavelengths.
		[[nodiscard]] constexpr bool_t is_wavelengths() const noexcept { return type() == spectrum_type::WAVELENGTHS; }

		/// @brief Returns true if type is rgb.
		[[nodiscard]] constexpr bool_t is_rgb() const noexcept { return type() == spectrum_type::RGB; }

		/// @brief Returns true if type is blackbody.
		[[nodiscard]] constexpr bool_t is_blackbody() const noexcept { return type() == spectrum_type::BLACKBODY; }

		/// @brief Returns true if type is file.
		[[nodiscard]] constexpr bool_t is_file() const noexcept { return type() == spectrum_type::FILE; }

		/// @brief Returns true if type is builtin.
		[[nodiscard]] constexpr bool_t is_builtin() const noexcept { return type() == spectrum_type::BUILTIN; }

		/// @brief Returns true if type is constant.
		[[nodiscard]] constexpr bool_t is_constant() const noexcept { return type() == spectrum_type::CONSTANT; }

		/// @brief Returns spectrum value as wavelengths.
		[[nodiscard]] constexpr std::optional<wavelength_array> as_wavelengths() const noexcept {
				if (!is_wavelengths()) { return std::nullopt; }
			return _native->as.wavelengths;
		}

		/// @brief Returns spectrum value as rgb.
		[[nodiscard]] constexpr std::optional<rgb> as_rgb() const noexcept {
				if (!is_rgb()) { return std::nullopt; }
			return _native->as.rgb;
		}

		/// @brief Returns spectrum value as blackbody.
		[[nodiscard]] constexpr std::optional<blackbody> as_blackbody() const noexcept {
				if (!is_blackbody()) { return std::nullopt; }
			return _native->as.blackbody;
		}

		/// @brief Returns spectrum value as file.
		[[nodiscard]] constexpr std::optional<string> as_file() const noexcept {
				if (!is_file()) { return std::nullopt; }
			return _native->as.file_name;
		}

		/// @brief Returns spectrum value as builtin.
		[[nodiscard]] constexpr std::optional<spectrum_builtin> as_builtin() const noexcept {
				if (!is_builtin()) { return std::nullopt; }
			return static_cast<spectrum_builtin>(_native->as.builtin);
		}

		/// @brief Returns spectrum value as constant.
		[[nodiscard]] constexpr std::optional<float_t> as_constant() const noexcept {
				if (!is_constant()) { return std::nullopt; }
			return _native->as.constant;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum spectrum_type;
				switch (type()) {
				case WAVELENGTHS: return _native->as.wavelengths;
				case RGB:		  return _native->as.rgb;
				case BLACKBODY:
					return blackbody(
					  _native->as.blackbody
					); // float in C: without the explicit type the variant would pick float_t
				case FILE:	   return _native->as.file_name;
				case BUILTIN:  return static_cast<spectrum_builtin>(_native->as.builtin);
				case CONSTANT: return _native->as.constant;
				case MAX_NUM:  [[fallthrough]];
				default:	   break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(to_variant(), type()); }
	};
} // namespace tpbrt
#endif // _TINYPBRT_COLOR_HPP_
