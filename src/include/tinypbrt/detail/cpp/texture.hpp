#pragma once
#ifndef _TINYPBRT_TEXTURE_HPP_
#define _TINYPBRT_TEXTURE_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/texture.h>

#include <tinypbrt/detail/cpp/color.hpp>
#include <tinypbrt/detail/cpp/common.hpp>
#include <tinypbrt/detail/cpp/transform.hpp>

namespace tpbrt {
	/**
	 * @defgroup texture-cpp-public Texture
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @copybrief tpbrt_texture_handle_value_type_t
	 * @ingroup texture-cpp-public
	 */
	enum class texture_handle_value_type : uchar_t {
		/// @copybrief TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE_IDX
		TEXTURE_IDX = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE_IDX,
		/// @copybrief TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM
		SPECTRUM	= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM,
		/// @copybrief TPBRT_TEXTURE_HANDLE_VALUE_TYPE_MAX_NUM
		MAX_NUM		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts texture_handle_value_type enum value to string.
	 * @param type Texture handle value type.
	 * @return String value of texture handle value type.
	 * @ingroup texture-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const texture_handle_value_type type) noexcept {
		using enum texture_handle_value_type;
			switch (type) {
			case TEXTURE_IDX: return "textureidx";
			case SPECTRUM:	  return "spectrum";
			case MAX_NUM:	  return "maxnum";
			default:		  return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_texture_handle_t.
	 * @ingroup texture-cpp-public
	 */
	struct texture_handle : detail::wrapper<texture_handle, tpbrt_texture_handle_t>,
							detail::with_variant<texture_handle, texture_idx_t, spectrum> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns value_type value.
		[[nodiscard]] constexpr texture_handle_value_type value_type() const noexcept {
			return static_cast<texture_handle_value_type>(_native->value_type);
		}

		/// @brief Returns true if type is texture_idx.
		[[nodiscard]] constexpr bool_t is_texture_idx() const noexcept {
			return value_type() == texture_handle_value_type::TEXTURE_IDX;
		}

		/// @brief Returns true if type is spectrum.
		[[nodiscard]] constexpr bool_t is_spectrum() const noexcept {
			return value_type() == texture_handle_value_type::SPECTRUM;
		}

		/// @brief Returns texture handle value as texture_idx.
		[[nodiscard]] constexpr std::optional<texture_idx_t> as_texture_idx() const noexcept {
				if (!is_texture_idx()) { return std::nullopt; }
			return _native->as.texture_idx;
		}

		/// @brief Returns texture handle value as spectrum.
		[[nodiscard]] constexpr std::optional<spectrum> as_spectrum() const noexcept {
				if (!is_spectrum()) { return std::nullopt; }
			return _native->as.spectrum;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum texture_handle_value_type;
				switch (value_type()) {
				case TEXTURE_IDX: return _native->as.texture_idx;
				case SPECTRUM:	  return _native->as.spectrum;
				case MAX_NUM:	  [[fallthrough]];
				default:		  break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(to_variant(), value_type()); }
	};

	/**
	 * @copybrief tpbrt_texture_type_t
	 * @ingroup texture-cpp-public
	 */
	enum class texture_type : uchar_t {
		/// @copybrief TPBRT_TEXTURE_TYPE_FLOAT
		FLOAT	 = TPBRT_TEXTURE_TYPE_FLOAT,
		/// @copybrief TPBRT_TEXTURE_TYPE_SPECTRUM
		SPECTRUM = TPBRT_TEXTURE_TYPE_SPECTRUM,
		/// @copybrief TPBRT_TEXTURE_TYPE_MAX_NUM
		MAX_NUM	 = TPBRT_TEXTURE_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts texture_type enum value to string.
	 * @param type Texture type.
	 * @return String value of texture type.
	 * @ingroup texture-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const texture_type type) noexcept {
		using enum texture_type;
			switch (type) {
			case FLOAT:	   return "float";
			case SPECTRUM: return "spectrum";
			case MAX_NUM:  return "maxnum";
			default:	   return "UNKNOWN";
			}
	}

	/**
	 * @copybrief tpbrt_texture_class_t
	 * @ingroup texture-cpp-public
	 */
	enum class texture_class : uchar_t {
		/// @copybrief TPBRT_TEXTURE_CLASS_BILERP
		BILERP		  = TPBRT_TEXTURE_CLASS_BILERP,
		/// @copybrief TPBRT_TEXTURE_CLASS_CHECKER_BOARD
		CHECKER_BOARD = TPBRT_TEXTURE_CLASS_CHECKER_BOARD,
		/// @copybrief TPBRT_TEXTURE_CLASS_CONSTANT
		CONSTANT	  = TPBRT_TEXTURE_CLASS_CONSTANT,
		/// @copybrief TPBRT_TEXTURE_CLASS_DIRECTION_MIX
		DIRECTION_MIX = TPBRT_TEXTURE_CLASS_DIRECTION_MIX,
		/// @copybrief TPBRT_TEXTURE_CLASS_DOTS
		DOTS		  = TPBRT_TEXTURE_CLASS_DOTS,
		/// @copybrief TPBRT_TEXTURE_CLASS_FBM
		FBM			  = TPBRT_TEXTURE_CLASS_FBM,
		/// @copybrief TPBRT_TEXTURE_CLASS_IMAGE_MAP
		IMAGE_MAP	  = TPBRT_TEXTURE_CLASS_IMAGE_MAP,
		/// @copybrief TPBRT_TEXTURE_CLASS_MARBLE
		MARBLE		  = TPBRT_TEXTURE_CLASS_MARBLE,
		/// @copybrief TPBRT_TEXTURE_CLASS_MIX
		MIX			  = TPBRT_TEXTURE_CLASS_MIX,
		/// @copybrief TPBRT_TEXTURE_CLASS_PTEX
		PTEX		  = TPBRT_TEXTURE_CLASS_PTEX,
		/// @copybrief TPBRT_TEXTURE_CLASS_SCALE
		SCALE		  = TPBRT_TEXTURE_CLASS_SCALE,
		/// @copybrief TPBRT_TEXTURE_CLASS_WINDY
		WINDY		  = TPBRT_TEXTURE_CLASS_WINDY,
		/// @copybrief TPBRT_TEXTURE_CLASS_WRINKLED
		WRINKLED	  = TPBRT_TEXTURE_CLASS_WRINKLED,
		/// @copybrief TPBRT_TEXTURE_CLASS_MAX_NUM
		MAX_NUM		  = TPBRT_TEXTURE_CLASS_MAX_NUM,
	};

	/**
	 * @brief Converts texture_class enum value to string.
	 * @param class_val Texture class.
	 * @return String value of texture class.
	 * @ingroup texture-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const texture_class class_val) noexcept {
		using enum texture_class;
			switch (class_val) {
			case BILERP:		return "bilerp";
			case CHECKER_BOARD: return "checkerboard";
			case CONSTANT:		return "constant";
			case DIRECTION_MIX: return "directionmix";
			case DOTS:			return "dots";
			case FBM:			return "fbm";
			case IMAGE_MAP:		return "imagemap";
			case MARBLE:		return "marble";
			case MIX:			return "mix";
			case PTEX:			return "ptex";
			case SCALE:			return "scale";
			case WINDY:			return "windy";
			case WRINKLED:		return "wrinkled";
			case MAX_NUM:		return "maxnum";
			default:			return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_texture_bilerp_params_t.
	 * @ingroup texture-cpp-public
	 */
	struct texture_bilerp_params : detail::wrapper<texture_bilerp_params, tpbrt_texture_bilerp_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns v00 value.
		[[nodiscard]] constexpr texture_handle v00() const noexcept { return _native->v00; }

		/// @brief Returns v01 value.
		[[nodiscard]] constexpr texture_handle v01() const noexcept { return _native->v01; }

		/// @brief Returns v10 value.
		[[nodiscard]] constexpr texture_handle v10() const noexcept { return _native->v10; }

		/// @brief Returns v11 value.
		[[nodiscard]] constexpr texture_handle v11() const noexcept { return _native->v11; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(v00(), v01(), v10(), v11()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_texture_checkerboard_params_t.
	 * @ingroup texture-cpp-public
	 */
	struct texture_checkerboard_params : detail::wrapper<texture_checkerboard_params, tpbrt_texture_checkerboard_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns tex1 value.
		[[nodiscard]] constexpr texture_handle tex1() const noexcept { return _native->tex1; }

		/// @brief Returns tex2 value.
		[[nodiscard]] constexpr texture_handle tex2() const noexcept { return _native->tex2; }

		/// @brief Returns dimension value.
		[[nodiscard]] constexpr uint_t dimension() const noexcept { return _native->dimension; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(tex1(), tex2(), dimension()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_texture_constant_params_t.
	 * @ingroup texture-cpp-public
	 */
	struct texture_constant_params : detail::wrapper<texture_constant_params, tpbrt_texture_constant_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns value value.
		[[nodiscard]] constexpr texture_handle value() const noexcept { return _native->value; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(value()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_texture_direction_mix_params_t.
	 * @ingroup texture-cpp-public
	 */
	struct texture_direction_mix_params : detail::wrapper<texture_direction_mix_params, tpbrt_texture_direction_mix_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns tex1 value.
		[[nodiscard]] constexpr texture_handle tex1() const noexcept { return _native->tex1; }

		/// @brief Returns tex2 value.
		[[nodiscard]] constexpr texture_handle tex2() const noexcept { return _native->tex2; }

		/// @brief Returns dir value.
		[[nodiscard]] constexpr vec dir() const noexcept { return _native->dir; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(tex1(), tex2(), dir()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_texture_dots_params_t.
	 * @ingroup texture-cpp-public
	 */
	struct texture_dots_params : detail::wrapper<texture_dots_params, tpbrt_texture_dots_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns inside value.
		[[nodiscard]] constexpr texture_handle inside() const noexcept { return _native->inside; }

		/// @brief Returns outside value.
		[[nodiscard]] constexpr texture_handle outside() const noexcept { return _native->outside; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(inside(), outside()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_texture_fbm_params_t, shared by the fbm, wrinkled and windy textures.
	 * @details The texture class is a template parameter, so every texture class gets its own type (a type alias
	 * would make them the same type and std::visit could not tell fbm, wrinkled and windy apart).
	 * @tparam Class Texture class (FBM, WRINKLED or WINDY).
	 * @ingroup texture-cpp-public
	 */
	template<texture_class Class>
	struct texture_noise_params : detail::wrapper<texture_noise_params<Class>, tpbrt_texture_fbm_params_t> {
		/// @brief Texture class described by these parameters.
		static constexpr texture_class TextureClass = Class;

		/// @brief Default wrapper constructor.
		using detail::wrapper<texture_noise_params, tpbrt_texture_fbm_params_t>::wrapper;

		/// @brief Returns octaves value.
		[[nodiscard]] constexpr uint_t octaves() const noexcept { return this->_native->octaves; }

		/// @brief Returns roughness value.
		[[nodiscard]] constexpr float_t roughness() const noexcept { return this->_native->roughness; }

		/// @brief Returns transform value.
		[[nodiscard]] constexpr transform_handle transform() const noexcept { return this->_native->transform; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(octaves(), roughness(), transform()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_texture_fbm_params_t.
	 * @ingroup texture-cpp-public
	 */
	using texture_fbm_params	  = texture_noise_params<texture_class::FBM>;

	/**
	 * @brief C++ wrapper for tpbrt_texture_wrinkled_params_t.
	 * @ingroup texture-cpp-public
	 */
	using texture_wrinkled_params = texture_noise_params<texture_class::WRINKLED>;

	/**
	 * @brief C++ wrapper for tpbrt_texture_windy_params_t.
	 * @ingroup texture-cpp-public
	 */
	using texture_windy_params	  = texture_noise_params<texture_class::WINDY>;

	/**
	 * @copybrief tpbrt_texture_wrap_t
	 * @ingroup texture-cpp-public
	 */
	enum class texture_wrap : uchar_t {
		/// @copybrief TPBRT_TEXTURE_WRAP_REPEAT
		REPEAT	= TPBRT_TEXTURE_WRAP_REPEAT,
		/// @copybrief TPBRT_TEXTURE_WRAP_BLACK
		BLACK	= TPBRT_TEXTURE_WRAP_BLACK,
		/// @copybrief TPBRT_TEXTURE_WRAP_CLAMP
		CLAMP	= TPBRT_TEXTURE_WRAP_CLAMP,
		/// @copybrief TPBRT_TEXTURE_WRAP_MAX_NUM
		MAX_NUM = TPBRT_TEXTURE_WRAP_MAX_NUM,
	};

	/**
	 * @brief Converts texture_wrap enum value to string.
	 * @param wrap Texture wrap.
	 * @return String value of texture wrap.
	 * @ingroup texture-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const texture_wrap wrap) noexcept {
		using enum texture_wrap;
			switch (wrap) {
			case REPEAT:  return "repeat";
			case BLACK:	  return "black";
			case CLAMP:	  return "clamp";
			case MAX_NUM: return "maxnum";
			default:	  return "UNKNOWN";
			}
	}

	/**
	 * @copybrief tpbrt_texture_filter_t
	 * @ingroup texture-cpp-public
	 */
	enum class texture_filter : uchar_t {
		/// @copybrief TPBRT_TEXTURE_FILTER_BILINEAR
		BILINEAR  = TPBRT_TEXTURE_FILTER_BILINEAR,
		/// @copybrief TPBRT_TEXTURE_FILTER_EWA
		EWA		  = TPBRT_TEXTURE_FILTER_EWA,
		/// @copybrief TPBRT_TEXTURE_FILTER_TRILINEAR
		TRILINEAR = TPBRT_TEXTURE_FILTER_TRILINEAR,
		/// @copybrief TPBRT_TEXTURE_FILTER_POINT
		POINT	  = TPBRT_TEXTURE_FILTER_POINT,
		/// @copybrief TPBRT_TEXTURE_FILTER_MAX_NUM
		MAX_NUM	  = TPBRT_TEXTURE_FILTER_MAX_NUM,
	};

	/**
	 * @brief Converts texture_filter enum value to string.
	 * @param filter Texture filter.
	 * @return String value of texture filter.
	 * @ingroup texture-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const texture_filter filter) noexcept {
		using enum texture_filter;
			switch (filter) {
			case BILINEAR:	return "bilinear";
			case EWA:		return "ewa";
			case TRILINEAR: return "trilinear";
			case POINT:		return "point";
			case MAX_NUM:	return "maxnum";
			default:		return "UNKNOWN";
			}
	}

	/**
	 * @copybrief tpbrt_texture_encoding_t
	 * @ingroup texture-cpp-public
	 */
	enum class texture_encoding : uchar_t {
		/// @copybrief TPBRT_TEXTURE_ENCODING_SRGB
		SRGB	= TPBRT_TEXTURE_ENCODING_SRGB,
		/// @copybrief TPBRT_TEXTURE_ENCODING_LINEAR
		LINEAR	= TPBRT_TEXTURE_ENCODING_LINEAR,
		/// @copybrief TPBRT_TEXTURE_ENCODING_GAMMA
		GAMMA	= TPBRT_TEXTURE_ENCODING_GAMMA,
		/// @copybrief TPBRT_TEXTURE_ENCODING_MAX_NUM
		MAX_NUM = TPBRT_TEXTURE_ENCODING_MAX_NUM,
	};

	/**
	 * @brief Converts texture_encoding enum value to string.
	 * @param encoding Texture encoding.
	 * @return String value of texture encoding.
	 * @ingroup texture-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const texture_encoding encoding) noexcept {
		using enum texture_encoding;
			switch (encoding) {
			case SRGB:	  return "sRGB";
			case LINEAR:  return "linear";
			case GAMMA:	  return "gamma";
			case MAX_NUM: return "maxnum";
			default:	  return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_texture_image_map_params_t.
	 * @ingroup texture-cpp-public
	 */
	struct texture_image_map_params : detail::wrapper<texture_image_map_params, tpbrt_texture_image_map_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns file_name value.
		[[nodiscard]] constexpr string file_name() const noexcept { return _native->file_name; }

		/// @brief Returns max_anisotropy value.
		[[nodiscard]] constexpr float_t max_anisotropy() const noexcept { return _native->max_anisotropy; }

		/// @brief Returns scale value.
		[[nodiscard]] constexpr float_t scale() const noexcept { return _native->scale; }

		/// @brief Returns gamma value.
		[[nodiscard]] constexpr opt_float gamma() const noexcept { return _native->gamma; }

		/// @brief Returns wrap value.
		[[nodiscard]] constexpr texture_wrap wrap() const noexcept { return static_cast<texture_wrap>(_native->wrap); }

		/// @brief Returns filter value.
		[[nodiscard]] constexpr texture_filter filter() const noexcept { return static_cast<texture_filter>(_native->filter); }

		/// @brief Returns encoding value.
		[[nodiscard]] constexpr texture_encoding encoding() const noexcept {
			return static_cast<texture_encoding>(_native->encoding);
		}

		/// @brief Returns invert value.
		[[nodiscard]] constexpr bool_t invert() const noexcept { return _native->invert; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept {
			return std::make_tuple(file_name(), max_anisotropy(), scale(), gamma(), wrap(), filter(), encoding(), invert());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_texture_marble_params_t.
	 * @ingroup texture-cpp-public
	 */
	struct texture_marble_params : detail::wrapper<texture_marble_params, tpbrt_texture_marble_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns octaves value.
		[[nodiscard]] constexpr uint_t octaves() const noexcept { return _native->octaves; }

		/// @brief Returns roughness value.
		[[nodiscard]] constexpr float_t roughness() const noexcept { return _native->roughness; }

		/// @brief Returns scale value.
		[[nodiscard]] constexpr float_t scale() const noexcept { return _native->scale; }

		/// @brief Returns variation value.
		[[nodiscard]] constexpr float_t variation() const noexcept { return _native->variation; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept {
			return std::make_tuple(octaves(), roughness(), scale(), variation());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_texture_mix_params_t.
	 * @ingroup texture-cpp-public
	 */
	struct texture_mix_params : detail::wrapper<texture_mix_params, tpbrt_texture_mix_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns tex1 value.
		[[nodiscard]] constexpr texture_handle tex1() const noexcept { return _native->tex1; }

		/// @brief Returns tex2 value.
		[[nodiscard]] constexpr texture_handle tex2() const noexcept { return _native->tex2; }

		/// @brief Returns amount value.
		[[nodiscard]] constexpr texture_handle amount() const noexcept { return _native->amount; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(tex1(), tex2(), amount()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_texture_ptex_params_t.
	 * @ingroup texture-cpp-public
	 */
	struct texture_ptex_params : detail::wrapper<texture_ptex_params, tpbrt_texture_ptex_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns gamma value.
		[[nodiscard]] constexpr opt_float gamma() const noexcept { return _native->gamma; }

		/// @brief Returns file_name value.
		[[nodiscard]] constexpr string file_name() const noexcept { return _native->file_name; }

		/// @brief Returns scale value.
		[[nodiscard]] constexpr float_t scale() const noexcept { return _native->scale; }

		/// @brief Returns encoding value.
		[[nodiscard]] constexpr texture_encoding encoding() const noexcept {
			return static_cast<texture_encoding>(_native->encoding);
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept {
			return std::make_tuple(gamma(), file_name(), scale(), encoding());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_texture_scale_params_t.
	 * @ingroup texture-cpp-public
	 */
	struct texture_scale_params : detail::wrapper<texture_scale_params, tpbrt_texture_scale_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns tex value.
		[[nodiscard]] constexpr texture_handle tex() const noexcept { return _native->tex; }

		/// @brief Returns scale value.
		[[nodiscard]] constexpr texture_handle scale() const noexcept { return _native->scale; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(tex(), scale()); }
	};

	/**
	 * @copybrief tpbrt_texture_mapping_t
	 * @ingroup texture-cpp-public
	 */
	enum class texture_mapping : uchar_t {
		/// @copybrief TPBRT_TEXTURE_MAPPING_UV
		UV			= TPBRT_TEXTURE_MAPPING_UV,
		/// @copybrief TPBRT_TEXTURE_MAPPING_SPHERICAL
		SPHERICAL	= TPBRT_TEXTURE_MAPPING_SPHERICAL,
		/// @copybrief TPBRT_TEXTURE_MAPPING_CYLINDRICAL
		CYLINDRICAL = TPBRT_TEXTURE_MAPPING_CYLINDRICAL,
		/// @copybrief TPBRT_TEXTURE_MAPPING_PLANAR
		PLANAR		= TPBRT_TEXTURE_MAPPING_PLANAR,
		/// @copybrief TPBRT_TEXTURE_MAPPING_MAX_NUM
		MAX_NUM		= TPBRT_TEXTURE_MAPPING_MAX_NUM,
	};

	/**
	 * @brief Converts texture_mapping enum value to string.
	 * @param mapping Texture mapping.
	 * @return String value of texture mapping.
	 * @ingroup texture-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const texture_mapping mapping) noexcept {
		using enum texture_mapping;
			switch (mapping) {
			case UV:		  return "uv";
			case SPHERICAL:	  return "spherical";
			case CYLINDRICAL: return "cylindrical";
			case PLANAR:	  return "planar";
			case MAX_NUM:	  return "maxnum";
			default:		  return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_texture_mapping_uv_params_t.
	 * @ingroup texture-cpp-public
	 */
	struct texture_mapping_uv_params : detail::wrapper<texture_mapping_uv_params, tpbrt_texture_mapping_uv_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns u_scale value.
		[[nodiscard]] constexpr float_t u_scale() const noexcept { return _native->u_scale; }

		/// @brief Returns v_scale value.
		[[nodiscard]] constexpr float_t v_scale() const noexcept { return _native->v_scale; }

		/// @brief Returns u_delta value.
		[[nodiscard]] constexpr float_t u_delta() const noexcept { return _native->u_delta; }

		/// @brief Returns v_delta value.
		[[nodiscard]] constexpr float_t v_delta() const noexcept { return _native->v_delta; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept {
			return std::make_tuple(u_scale(), v_scale(), u_delta(), v_delta());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_texture_mapping_spherical_params_t, shared by the spherical and cylindrical mappings.
	 * @details The mapping is a template parameter, so both mappings get their own type (see texture_noise_params).
	 * @tparam Mapping Texture mapping (SPHERICAL or CYLINDRICAL).
	 * @ingroup texture-cpp-public
	 */
	template<texture_mapping Mapping>
	struct texture_mapping_transform_params
		: detail::wrapper<texture_mapping_transform_params<Mapping>, tpbrt_texture_mapping_spherical_params_t> {
		/// @brief Mapping described by these parameters.
		static constexpr texture_mapping TextureMapping = Mapping;

		/// @brief Default wrapper constructor.
		using detail::wrapper<texture_mapping_transform_params, tpbrt_texture_mapping_spherical_params_t>::wrapper;

		/// @brief Returns transform value.
		[[nodiscard]] constexpr transform_handle transform() const noexcept { return this->_native->transform; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(transform()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_texture_mapping_spherical_params_t.
	 * @ingroup texture-cpp-public
	 */
	using texture_mapping_spherical_params	 = texture_mapping_transform_params<texture_mapping::SPHERICAL>;

	/**
	 * @brief C++ wrapper for tpbrt_texture_mapping_cylindrical_params_t.
	 * @ingroup texture-cpp-public
	 */
	using texture_mapping_cylindrical_params = texture_mapping_transform_params<texture_mapping::CYLINDRICAL>;

	/**
	 * @brief C++ wrapper for tpbrt_texture_mapping_planar_params_t.
	 * @note Note that "uscale" and "vscale" are redundant for the "planar" mapping and are therefore not provided since their effect can be had by scaling the "v1" and "v2" vectors.
	 * @ingroup texture-cpp-public
	 */
	struct texture_mapping_planar_params : detail::wrapper<texture_mapping_planar_params, tpbrt_texture_mapping_planar_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns u_delta value.
		[[nodiscard]] constexpr float_t u_delta() const noexcept { return _native->u_delta; }

		/// @brief Returns v_delta value.
		[[nodiscard]] constexpr float_t v_delta() const noexcept { return _native->v_delta; }

		/// @brief Returns v1 value.
		[[nodiscard]] constexpr vec v1() const noexcept { return _native->v1; }

		/// @brief Returns v2 value.
		[[nodiscard]] constexpr vec v2() const noexcept { return _native->v2; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(u_delta(), v_delta(), v1(), v2()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_texture_t.
	 * @ingroup texture-cpp-public
	 */
	struct texture
		: detail::wrapper<texture, tpbrt_texture_t>,
		  detail::with_variant<texture, texture_bilerp_params, texture_checkerboard_params, texture_constant_params,
			texture_direction_mix_params, texture_dots_params, texture_fbm_params, texture_wrinkled_params, texture_windy_params,
			texture_image_map_params, texture_marble_params, texture_mix_params, texture_ptex_params, texture_scale_params> {
		/// @brief Type used for better reading mapping tagged union of parameters.
		using mapping_variant_type = detail::optional_variant<texture_mapping_uv_params, texture_mapping_spherical_params,
		  texture_mapping_cylindrical_params, texture_mapping_planar_params>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns idx value.
		[[nodiscard]] constexpr texture_idx_t idx() const noexcept { return _native->idx; }

		/// @brief Returns name value.
		[[nodiscard]] constexpr string name() const noexcept { return _native->name; }

		/// @brief Returns type value.
		[[nodiscard]] constexpr texture_type type() const noexcept { return static_cast<texture_type>(_native->type); }

		/// @brief Returns tex_class value.
		[[nodiscard]] constexpr texture_class tex_class() const noexcept {
			return static_cast<texture_class>(_native->tex_class);
		}

		/// @brief Returns true if type is bilerp.
		[[nodiscard]] constexpr bool_t is_bilerp() const noexcept { return tex_class() == texture_class::BILERP; }

		/// @brief Returns true if type is checkerboard.
		[[nodiscard]] constexpr bool_t is_checkerboard() const noexcept { return tex_class() == texture_class::CHECKER_BOARD; }

		/// @brief Returns true if type is constant.
		[[nodiscard]] constexpr bool_t is_constant() const noexcept { return tex_class() == texture_class::CONSTANT; }

		/// @brief Returns true if type is direction_mix.
		[[nodiscard]] constexpr bool_t is_direction_mix() const noexcept { return tex_class() == texture_class::DIRECTION_MIX; }

		/// @brief Returns true if type is dots.
		[[nodiscard]] constexpr bool_t is_dots() const noexcept { return tex_class() == texture_class::DOTS; }

		/// @brief Returns true if type is fbm.
		[[nodiscard]] constexpr bool_t is_fbm() const noexcept { return tex_class() == texture_class::FBM; }

		/// @brief Returns true if type is wrinkled.
		[[nodiscard]] constexpr bool_t is_wrinkled() const noexcept { return tex_class() == texture_class::WRINKLED; }

		/// @brief Returns true if type is windy.
		[[nodiscard]] constexpr bool_t is_windy() const noexcept { return tex_class() == texture_class::WINDY; }

		/// @brief Returns true if type is image_map.
		[[nodiscard]] constexpr bool_t is_image_map() const noexcept { return tex_class() == texture_class::IMAGE_MAP; }

		/// @brief Returns true if type is marble.
		[[nodiscard]] constexpr bool_t is_marble() const noexcept { return tex_class() == texture_class::MARBLE; }

		/// @brief Returns true if type is mix.
		[[nodiscard]] constexpr bool_t is_mix() const noexcept { return tex_class() == texture_class::MIX; }

		/// @brief Returns true if type is ptex.
		[[nodiscard]] constexpr bool_t is_ptex() const noexcept { return tex_class() == texture_class::PTEX; }

		/// @brief Returns true if type is scale.
		[[nodiscard]] constexpr bool_t is_scale() const noexcept { return tex_class() == texture_class::SCALE; }

		/// @brief Returns texture parameters as bilerp.
		[[nodiscard]] constexpr std::optional<texture_bilerp_params> as_bilerp() const noexcept {
				if (!is_bilerp()) { return std::nullopt; }
			return _native->as.bilerp;
		}

		/// @brief Returns texture parameters as checkerboard.
		[[nodiscard]] constexpr std::optional<texture_checkerboard_params> as_checkerboard() const noexcept {
				if (!is_checkerboard()) { return std::nullopt; }
			return _native->as.checkerboard;
		}

		/// @brief Returns texture parameters as constant.
		[[nodiscard]] constexpr std::optional<texture_constant_params> as_constant() const noexcept {
				if (!is_constant()) { return std::nullopt; }
			return _native->as.constant;
		}

		/// @brief Returns texture parameters as direction_mix.
		[[nodiscard]] constexpr std::optional<texture_direction_mix_params> as_direction_mix() const noexcept {
				if (!is_direction_mix()) { return std::nullopt; }
			return _native->as.direction_mix;
		}

		/// @brief Returns texture parameters as dots.
		[[nodiscard]] constexpr std::optional<texture_dots_params> as_dots() const noexcept {
				if (!is_dots()) { return std::nullopt; }
			return _native->as.dots;
		}

		/// @brief Returns texture parameters as fbm.
		[[nodiscard]] constexpr std::optional<texture_fbm_params> as_fbm() const noexcept {
				if (!is_fbm()) { return std::nullopt; }
			return _native->as.fbm;
		}

		/// @brief Returns texture parameters as wrinkled.
		[[nodiscard]] constexpr std::optional<texture_wrinkled_params> as_wrinkled() const noexcept {
				if (!is_wrinkled()) { return std::nullopt; }
			return _native->as.wrinkled;
		}

		/// @brief Returns texture parameters as windy.
		[[nodiscard]] constexpr std::optional<texture_windy_params> as_windy() const noexcept {
				if (!is_windy()) { return std::nullopt; }
			return _native->as.windy;
		}

		/// @brief Returns texture parameters as image_map.
		[[nodiscard]] constexpr std::optional<texture_image_map_params> as_image_map() const noexcept {
				if (!is_image_map()) { return std::nullopt; }
			return _native->as.image_map;
		}

		/// @brief Returns texture parameters as marble.
		[[nodiscard]] constexpr std::optional<texture_marble_params> as_marble() const noexcept {
				if (!is_marble()) { return std::nullopt; }
			return _native->as.marble;
		}

		/// @brief Returns texture parameters as mix.
		[[nodiscard]] constexpr std::optional<texture_mix_params> as_mix() const noexcept {
				if (!is_mix()) { return std::nullopt; }
			return _native->as.mix;
		}

		/// @brief Returns texture parameters as ptex.
		[[nodiscard]] constexpr std::optional<texture_ptex_params> as_ptex() const noexcept {
				if (!is_ptex()) { return std::nullopt; }
			return _native->as.ptex;
		}

		/// @brief Returns texture parameters as scale.
		[[nodiscard]] constexpr std::optional<texture_scale_params> as_scale() const noexcept {
				if (!is_scale()) { return std::nullopt; }
			return _native->as.scale;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum texture_class;
				switch (tex_class()) {
				case BILERP:		return _native->as.bilerp;
				case CHECKER_BOARD: return _native->as.checkerboard;
				case CONSTANT:		return _native->as.constant;
				case DIRECTION_MIX: return _native->as.direction_mix;
				case DOTS:			return _native->as.dots;
				case FBM:			return texture_fbm_params(_native->as.fbm);
				case WRINKLED:		return texture_wrinkled_params(_native->as.wrinkled);
				case WINDY:			return texture_windy_params(_native->as.windy);
				case IMAGE_MAP:		return _native->as.image_map;
				case MARBLE:		return _native->as.marble;
				case MIX:			return _native->as.mix;
				case PTEX:			return _native->as.ptex;
				case SCALE:			return _native->as.scale;
				case MAX_NUM:		[[fallthrough]];
				default:			break;
				}
			return std::nullopt;
		}

		/// @brief Returns mapping value.
		[[nodiscard]] constexpr texture_mapping mapping() const noexcept {
			return static_cast<texture_mapping>(_native->mapping);
		}

		/// @brief Returns true if type is uv.
		[[nodiscard]] constexpr bool_t is_mapping_uv() const noexcept { return mapping() == texture_mapping::UV; }

		/// @brief Returns true if type is spherical.
		[[nodiscard]] constexpr bool_t is_mapping_spherical() const noexcept { return mapping() == texture_mapping::SPHERICAL; }

		/// @brief Returns true if type is cylindrical.
		[[nodiscard]] constexpr bool_t is_mapping_cylindrical() const noexcept {
			return mapping() == texture_mapping::CYLINDRICAL;
		}

		/// @brief Returns true if type is planar.
		[[nodiscard]] constexpr bool_t is_mapping_planar() const noexcept { return mapping() == texture_mapping::PLANAR; }

		/// @brief Returns texture mapping parameters as uv.
		[[nodiscard]] constexpr std::optional<texture_mapping_uv_params> mapping_as_uv() const noexcept {
				if (!is_mapping_uv()) { return std::nullopt; }
			return _native->mapping_as.uv;
		}

		/// @brief Returns texture mapping parameters as spherical.
		[[nodiscard]] constexpr std::optional<texture_mapping_spherical_params> mapping_as_spherical() const noexcept {
				if (!is_mapping_spherical()) { return std::nullopt; }
			return _native->mapping_as.spherical;
		}

		/// @brief Returns texture mapping parameters as cylindrical.
		[[nodiscard]] constexpr std::optional<texture_mapping_cylindrical_params> mapping_as_cylindrical() const noexcept {
				if (!is_mapping_cylindrical()) { return std::nullopt; }
			return _native->mapping_as.cylindrical;
		}

		/// @brief Returns texture mapping parameters as planar.
		[[nodiscard]] constexpr std::optional<texture_mapping_planar_params> mapping_as_planar() const noexcept {
				if (!is_mapping_planar()) { return std::nullopt; }
			return _native->mapping_as.planar;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr mapping_variant_type mapping_to_variant() const noexcept {
			using enum texture_mapping;
				switch (mapping()) {
				case UV:		  return _native->mapping_as.uv;
				case SPHERICAL:	  return texture_mapping_spherical_params(_native->mapping_as.spherical);
				case CYLINDRICAL: return texture_mapping_cylindrical_params(_native->mapping_as.cylindrical);
				case PLANAR:	  return _native->mapping_as.planar;
				case MAX_NUM:	  [[fallthrough]];
				default:		  break;
				}
			return std::nullopt;
		}

		/// @brief Visits mapping parameters.
		template<class Visitor>
		constexpr auto visit_mapping(Visitor&& visitor) const {
			return detail::visit_optional(std::forward<Visitor>(visitor), mapping_to_variant());
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept {
			return std::make_tuple(idx(), type(), tex_class(), mapping(), name(), mapping_to_variant(), to_variant());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_textures_list_t.
	 * @details Index access (operator[], at(size_t), iteration) works as in tpbrt::array, at(handle) and at(name) use
	 * the C lookups and return error::NOT_FOUND (or the C error code) when there is no such texture.
	 * @ingroup texture-cpp-public
	 */
	struct textures_list : array<tpbrt_textures_list_t, texture, &tpbrt_textures_list_t::textures> {
		/// @brief Base list constructor.
		using array::array;

		/// @brief Index based access from tpbrt::array.
		using array::at;

		/// @brief Operator for index based access from tpbrt::array.
		using array::operator[];

		/// @brief Returns texture pointed by given handle.
		[[nodiscard]] result<element_type> at(const texture_handle handle) const noexcept {
			return detail::find_in<element_type, tpbrt_texture_t>(tpbrt_get_texture_by_handle, _native, &handle.native());
		}

		/// @brief Returns texture with given name.
		[[nodiscard]] result<element_type> at(const std::string_view name) const noexcept {
			return detail::find_in<element_type, tpbrt_texture_t>(tpbrt_get_texture_by_name, _native,
			  detail::to_native_string(name));
		}

		/// @brief Returns texture pointed by given handle.
		[[nodiscard]] result<element_type> operator[](const texture_handle handle) const noexcept { return at(handle); }

		/// @brief Returns texture with given name.
		[[nodiscard]] result<element_type> operator[](const std::string_view name) const noexcept { return at(name); }
	};

	/**
	 * @brief Returns texture to which handle is provided.
	 * @param list List of textures in which texture given by handle should be.
	 * @param handle Handle to texture.
	 * @return Result with the texture or with error code (most likely error::NOT_FOUND).
	 * @ingroup texture-cpp-public
	 */
	[[nodiscard]] inline result<texture> get_texture(const textures_list list, const texture_handle handle) noexcept {
		return list.at(handle);
	}

	/**
	 * @brief Returns texture with given name.
	 * @param list List of textures in which searched texture should be.
	 * @param name Name of the searched texture.
	 * @return Result with the texture or with error code (most likely error::NOT_FOUND).
	 * @ingroup texture-cpp-public
	 */
	[[nodiscard]] inline result<texture> get_texture(const textures_list list, const std::string_view name) noexcept {
		return list.at(name);
	}
} // namespace tpbrt
#endif // _TINYPBRT_TEXTURE_HPP_
