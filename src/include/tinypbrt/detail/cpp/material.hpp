#pragma once
#ifndef _TINYPBRT_MATERIAL_HPP_
#define _TINYPBRT_MATERIAL_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/material.h>

#include <tinypbrt/detail/cpp/texture.hpp>

namespace tpbrt {
	/**
	 * @defgroup material-cpp-public Material
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @copybrief tpbrt_material_type_t
	 * @ingroup material-cpp-public
	 */
	enum class material_type : char_t {
		/// @copybrief TPBRT_MATERIAL_TYPE_COATED_DIFFUSE
		COATED_DIFFUSE		 = TPBRT_MATERIAL_TYPE_COATED_DIFFUSE,
		/// @copybrief TPBRT_MATERIAL_TYPE_COATED_CONDUCTOR
		COATED_CONDUCTOR	 = TPBRT_MATERIAL_TYPE_COATED_CONDUCTOR,
		/// @copybrief TPBRT_MATERIAL_TYPE_CONDUCTOR
		CONDUCTOR			 = TPBRT_MATERIAL_TYPE_CONDUCTOR,
		/// @copybrief TPBRT_MATERIAL_TYPE_DIELECTRIC
		DIELECTRIC			 = TPBRT_MATERIAL_TYPE_DIELECTRIC,
		/// @copybrief TPBRT_MATERIAL_TYPE_DIFFUSE
		DIFFUSE				 = TPBRT_MATERIAL_TYPE_DIFFUSE,
		/// @copybrief TPBRT_MATERIAL_TYPE_DIFFUSE_TRANSMISSION
		DIFFUSE_TRANSMISSION = TPBRT_MATERIAL_TYPE_DIFFUSE_TRANSMISSION,
		/// @copybrief TPBRT_MATERIAL_TYPE_HAIR
		HAIR				 = TPBRT_MATERIAL_TYPE_HAIR,
		/// @copybrief TPBRT_MATERIAL_TYPE_INTERFACE
		INTERFACE			 = TPBRT_MATERIAL_TYPE_INTERFACE,
		/// @copybrief TPBRT_MATERIAL_TYPE_MEASURED
		MEASURED			 = TPBRT_MATERIAL_TYPE_MEASURED,
		/// @copybrief TPBRT_MATERIAL_TYPE_MIX
		MIX					 = TPBRT_MATERIAL_TYPE_MIX,
		/// @copybrief TPBRT_MATERIAL_TYPE_SUBSURFACE
		SUBSURFACE			 = TPBRT_MATERIAL_TYPE_SUBSURFACE,
		/// @copybrief TPBRT_MATERIAL_TYPE_THIN_DIELECTRIC
		THIN_DIELECTRIC		 = TPBRT_MATERIAL_TYPE_THIN_DIELECTRIC,
		/// @copybrief TPBRT_MATERIAL_TYPE_MAX_NUM
		MAX_NUM				 = TPBRT_MATERIAL_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts material_type enum value to string.
	 * @param type Material type.
	 * @return String value of material type.
	 * @ingroup material-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const material_type type) noexcept {
		using enum material_type;
			switch (type) {
			case COATED_DIFFUSE:	   return "coateddiffuse";
			case COATED_CONDUCTOR:	   return "coatedconductor";
			case CONDUCTOR:			   return "conductor";
			case DIELECTRIC:		   return "dielectric";
			case DIFFUSE:			   return "diffuse";
			case DIFFUSE_TRANSMISSION: return "diffusetransmission";
			case HAIR:				   return "hair";
			case INTERFACE:			   return "interface";
			case MEASURED:			   return "measured";
			case MIX:				   return "mix";
			case SUBSURFACE:		   return "subsurface";
			case THIN_DIELECTRIC:	   return "thindielectric";
			case MAX_NUM:			   return "maxnum";
			default:				   return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_material_handle_t.
	 * @ingroup material-cpp-public
	 */
	struct material_handle : wrapper<material_handle, tpbrt_material_handle_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<material_idx_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns material_idx value.
		[[nodiscard]] constexpr media_idx_t material_idx() const noexcept { return _native.material_idx; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(material_idx()); }
	};

	/**
	 * @copybrief tpbrt_material_bump_map_type_t
	 * @ingroup material-cpp-public
	 */
	enum class material_bump_map_type : char_t {
		/// @copybrief TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE
		NONE		 = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE,
		/// @copybrief TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT
		DISPLACEMENT = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT,
		/// @copybrief TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP
		NORMAL_MAP	 = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP,
		/// @copybrief TPBRT_MATERIAL_BUMP_MAP_TYPE_MAX_NUM
		MAX_NUM		 = TPBRT_MATERIAL_BUMP_MAP_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts material_bump_map_type enum value to string.
	 * @param type Material bump map type.
	 * @return String value of material bump map type.
	 * @ingroup material-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const material_bump_map_type type) noexcept {
		using enum material_bump_map_type;
			switch (type) {
			case NONE:		   return "none";
			case DISPLACEMENT: return "displacement";
			case NORMAL_MAP:   return "normalmap";
			case MAX_NUM:	   return "maxnum";
			default:		   return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_material_bump_map_params_t.
	 * @ingroup material-cpp-public
	 */
	struct material_bump_map_params : wrapper<material_bump_map_params, tpbrt_material_bump_map_params_t>,
									  with_variant<material_bump_map_params, texture_handle, string> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<variant_type, material_bump_map_type>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns type value.
		[[nodiscard]] constexpr material_bump_map_type type() const noexcept {
			return static_cast<material_bump_map_type>(_native.type);
		}

		/// @brief Returns if type is displacement.
		[[nodiscard]] constexpr bool_t is_displacement() const noexcept { return type() == material_bump_map_type::DISPLACEMENT; }

		/// @brief Returns if type is normal_map.
		[[nodiscard]] constexpr bool_t is_normal_map() const noexcept { return type() == material_bump_map_type::NORMAL_MAP; }

		/// @brief Returns material bump map parameters as displacement.
		[[nodiscard]] constexpr std::optional<texture_handle> as_displacement() const noexcept {
				if (!is_displacement()) { return std::nullopt; }
			return _native.as.displacement;
		}

		/// @brief Returns material bump map parameters as normal_map.
		[[nodiscard]] constexpr std::optional<string> as_normal_map() const noexcept {
				if (!is_normal_map()) { return std::nullopt; }
			return _native.as.normal_map;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum material_bump_map_type;
				switch (type()) {
				case DISPLACEMENT: return _native.as.displacement;
				case NORMAL_MAP:   return _native.as.normal_map;
				case MAX_NUM:	   [[fallthrough]];
				default:		   break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(to_variant(), type()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_material_roughness_params_t.
	 * @ingroup material-cpp-public
	 */
	struct material_roughness_params : wrapper<material_roughness_params, tpbrt_material_roughness_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<texture_handle, texture_handle, texture_handle, bool_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns roughness value.
		[[nodiscard]] constexpr texture_handle roughness() const noexcept { return _native.roughness; }

		/// @brief Returns u_roughness value.
		[[nodiscard]] constexpr texture_handle u_roughness() const noexcept { return _native.u_roughness; }

		/// @brief Returns v_roughness value.
		[[nodiscard]] constexpr texture_handle v_roughness() const noexcept { return _native.v_roughness; }

		/// @brief Returns remap_roughness value.
		[[nodiscard]] constexpr bool_t remap_roughness() const noexcept { return _native.remap_roughness; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(roughness(), u_roughness(), v_roughness(), remap_roughness());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_material_coated_diffuse_params_t.
	 * @ingroup material-cpp-public
	 */
	struct material_coated_diffuse_params : wrapper<material_coated_diffuse_params, tpbrt_material_coated_diffuse_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<material_bump_map_params, texture_handle, texture_handle, texture_handle, uint_t, uint_t,
		  texture_handle, material_roughness_params, float_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns bump_map value.
		[[nodiscard]] constexpr material_bump_map_params bump_map() const noexcept { return _native.bump_map; }

		/// @brief Returns albedo value.
		[[nodiscard]] constexpr texture_handle albedo() const noexcept { return _native.albedo; }

		/// @brief Returns g value.
		[[nodiscard]] constexpr texture_handle g() const noexcept { return _native.g; }

		/// @brief Returns eta value.
		[[nodiscard]] constexpr texture_handle eta() const noexcept { return _native.eta; }

		/// @brief Returns max_depth value.
		[[nodiscard]] constexpr uint_t max_depth() const noexcept { return _native.max_depth; }

		/// @brief Returns n_samples value.
		[[nodiscard]] constexpr uint_t n_samples() const noexcept { return _native.n_samples; }

		/// @brief Returns reflectance value.
		[[nodiscard]] constexpr texture_handle reflectance() const noexcept { return _native.reflectance; }

		/// @brief Returns roughness_params value.
		[[nodiscard]] constexpr material_roughness_params roughness_params() const noexcept { return _native.roughness_params; }

		/// @brief Returns thickness value.
		[[nodiscard]] constexpr float_t thickness() const noexcept { return _native.thickness; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(bump_map(), albedo(), g(), eta(), max_depth(), n_samples(), reflectance(), roughness_params(),
			  thickness());
		}
	};

	/**
	 * @copybrief tpbrt_material_conductor_value_type_t
	 * @ingroup material-cpp-public
	 */
	enum class material_conductor_value_type : char_t {
		/// @copybrief TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_ETA_K
		ETA_K		= TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_ETA_K,
		/// @copybrief TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_REFLECTANCE
		REFLECTANCE = TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_REFLECTANCE,
		/// @copybrief TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_MAX_NUM
		MAX_NUM		= TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts material_conductor_value_type enum value to string.
	 * @param type Material conductor value type.
	 * @return String value of material conductor value type.
	 * @ingroup material-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const material_conductor_value_type type) noexcept {
		using enum material_conductor_value_type;
			switch (type) {
			case ETA_K:		  return "etak";
			case REFLECTANCE: return "reflectance";
			case MAX_NUM:	  return "maxnum";
			default:		  return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_material_coated_conductor_params_t.
	 * @ingroup material-cpp-public
	 */
	struct material_coated_conductor_params
		: wrapper<material_coated_conductor_params, tpbrt_material_coated_conductor_params_t>,
		  with_variant<material_coated_conductor_params, std::pair<spectrum, spectrum>, spectrum> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<material_bump_map_params, texture_handle, texture_handle, uint_t, uint_t, variant_type,
		  material_roughness_params, material_roughness_params, float_t, material_conductor_value_type>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns bump_map value.
		[[nodiscard]] constexpr material_bump_map_params bump_map() const noexcept { return _native.bump_map; }

		/// @brief Returns albedo value.
		[[nodiscard]] constexpr texture_handle albedo() const noexcept { return _native.albedo; }

		/// @brief Returns g value.
		[[nodiscard]] constexpr texture_handle g() const noexcept { return _native.g; }

		/// @brief Returns max_depth value.
		[[nodiscard]] constexpr uint_t max_depth() const noexcept { return _native.max_depth; }

		/// @brief Returns n_samples value.
		[[nodiscard]] constexpr uint_t n_samples() const noexcept { return _native.n_samples; }

		/// @brief Returns conductor_roughness_params value.
		[[nodiscard]] constexpr material_roughness_params conductor_roughness_params() const noexcept {
			return _native.conductor_roughness_params;
		}

		/// @brief Returns interface_roughness_params value.
		[[nodiscard]] constexpr material_roughness_params interface_roughness_params() const noexcept {
			return _native.interface_roughness_params;
		}

		/// @brief Returns thickness value.
		[[nodiscard]] constexpr float_t thickness() const noexcept { return _native.thickness; }

		/// @brief Returns value_type value.
		[[nodiscard]] constexpr material_conductor_value_type value_type() const noexcept {
			return static_cast<material_conductor_value_type>(_native.value_type);
		}

		/// @brief Returns if type is eta_k.
		[[nodiscard]] constexpr bool_t is_eta_k() const noexcept { return value_type() == material_conductor_value_type::ETA_K; }

		/// @brief Returns if type is reflectance.
		[[nodiscard]] constexpr bool_t is_reflectance() const noexcept {
			return value_type() == material_conductor_value_type::REFLECTANCE;
		}

		/// @brief Returns media parameters as eta_k.
		[[nodiscard]] constexpr std::optional<std::pair<spectrum, spectrum>> as_eta_k() const noexcept {
				if (!is_eta_k()) { return std::nullopt; }
			return std::pair<spectrum, spectrum> { _native.as.conductor.eta, _native.as.conductor.k };
		}

		/// @brief Returns media parameters as reflectance.
		[[nodiscard]] constexpr std::optional<spectrum> as_reflectance() const noexcept {
				if (!is_reflectance()) { return std::nullopt; }
			return _native.as.reflectance;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum material_conductor_value_type;
				switch (value_type()) {
				case ETA_K:		  return std::pair<spectrum, spectrum> { _native.as.conductor.eta, _native.as.conductor.k };
				case REFLECTANCE: return _native.as.reflectance;
				case MAX_NUM:	  [[fallthrough]];
				default:		  break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(bump_map(), albedo(), g(), max_depth(), n_samples(), to_variant(),
			  conductor_roughness_params(), interface_roughness_params(), thickness(), value_type());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_material_conductor_params_t.
	 * @ingroup material-cpp-public
	 */
	struct material_conductor_params
		: wrapper<material_conductor_params, tpbrt_material_conductor_params_t>,
		  with_variant<material_conductor_params, std::pair<texture_handle, texture_handle>, texture_handle> {
		/// @brief Type used for reflection.
		using reflect_type =
		  std::tuple<material_bump_map_params, variant_type, material_roughness_params, material_conductor_value_type>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns bump_map value.
		[[nodiscard]] constexpr material_bump_map_params bump_map() const noexcept { return _native.bump_map; }

		/// @brief Returns roughness_params value.
		[[nodiscard]] constexpr material_roughness_params roughness_params() const noexcept { return _native.roughness_params; }

		/// @brief Returns value_type value.
		[[nodiscard]] constexpr material_conductor_value_type value_type() const noexcept {
			return static_cast<material_conductor_value_type>(_native.value_type);
		}

		/// @brief Returns if type is eta_k.
		[[nodiscard]] constexpr bool_t is_eta_k() const noexcept { return value_type() == material_conductor_value_type::ETA_K; }

		/// @brief Returns if type is reflectance.
		[[nodiscard]] constexpr bool_t is_reflectance() const noexcept {
			return value_type() == material_conductor_value_type::REFLECTANCE;
		}

		/// @brief Returns media parameters as eta_k.
		[[nodiscard]] constexpr std::optional<std::pair<texture_handle, texture_handle>> as_eta_k() const noexcept {
				if (!is_eta_k()) { return std::nullopt; }
			return std::pair<texture_handle, texture_handle> { _native.as.conductor.eta, _native.as.conductor.k };
		}

		/// @brief Returns media parameters as reflectance.
		[[nodiscard]] constexpr std::optional<texture_handle> as_reflectance() const noexcept {
				if (!is_reflectance()) { return std::nullopt; }
			return _native.as.reflectance;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum material_conductor_value_type;
				switch (value_type()) {
				case ETA_K:		  return std::pair<texture_handle, texture_handle> { _native.as.conductor.eta, _native.as.conductor.k };
				case REFLECTANCE: return _native.as.reflectance;
				case MAX_NUM:	  [[fallthrough]];
				default:		  break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(bump_map(), to_variant(), roughness_params(), value_type());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_material_dielectric_params_t.
	 * @ingroup material-cpp-public
	 */
	struct material_dielectric_params : wrapper<material_dielectric_params, tpbrt_material_dielectric_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<material_bump_map_params, texture_handle, material_roughness_params>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns bump_map value.
		[[nodiscard]] constexpr material_bump_map_params bump_map() const noexcept { return _native.bump_map; }

		/// @brief Returns eta value.
		[[nodiscard]] constexpr texture_handle eta() const noexcept { return _native.eta; }

		/// @brief Returns roughness_params value.
		[[nodiscard]] constexpr material_roughness_params roughness_params() const noexcept { return _native.roughness_params; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(bump_map(), eta(), roughness_params());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_material_diffuse_params_t.
	 * @ingroup material-cpp-public
	 */
	struct material_diffuse_params : wrapper<material_diffuse_params, tpbrt_material_diffuse_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<material_bump_map_params, texture_handle>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns bump_map value.
		[[nodiscard]] constexpr material_bump_map_params bump_map() const noexcept { return _native.bump_map; }

		/// @brief Returns reflectance value.
		[[nodiscard]] constexpr texture_handle reflectance() const noexcept { return _native.reflectance; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(bump_map(), reflectance()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_material_diffuse_transmission_params_t.
	 * @ingroup material-cpp-public
	 */
	struct material_diffuse_transmission_params
		: wrapper<material_diffuse_transmission_params, tpbrt_material_diffuse_transmission_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<material_bump_map_params, texture_handle, texture_handle, texture_handle>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns bump_map value.
		[[nodiscard]] constexpr material_bump_map_params bump_map() const noexcept { return _native.bump_map; }

		/// @brief Returns reflectance value.
		[[nodiscard]] constexpr texture_handle reflectance() const noexcept { return _native.reflectance; }

		/// @brief Returns transmittance value.
		[[nodiscard]] constexpr texture_handle transmittance() const noexcept { return _native.transmittance; }

		/// @brief Returns scale value.
		[[nodiscard]] constexpr texture_handle scale() const noexcept { return _native.scale; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(bump_map(), reflectance(), transmittance(), scale());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_material_hair_params_t.
	 * @ingroup material-cpp-public
	 */
	struct material_hair_params : wrapper<material_hair_params, tpbrt_material_hair_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<material_bump_map_params, texture_handle, texture_handle, texture_handle, texture_handle,
		  texture_handle, texture_handle, texture_handle, texture_handle>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns bump_map value.
		[[nodiscard]] constexpr material_bump_map_params bump_map() const noexcept { return _native.bump_map; }

		/// @brief Returns sigma_a value.
		[[nodiscard]] constexpr texture_handle sigma_a() const noexcept { return _native.sigma_a; }

		/// @brief Returns reflectance value.
		[[nodiscard]] constexpr texture_handle reflectance() const noexcept { return _native.reflectance; }

		/// @brief Returns eumelanin value.
		[[nodiscard]] constexpr texture_handle eumelanin() const noexcept { return _native.eumelanin; }

		/// @brief Returns pheomelanin value.
		[[nodiscard]] constexpr texture_handle pheomelanin() const noexcept { return _native.pheomelanin; }

		/// @brief Returns eta value.
		[[nodiscard]] constexpr texture_handle eta() const noexcept { return _native.eta; }

		/// @brief Returns beta_m value.
		[[nodiscard]] constexpr texture_handle beta_m() const noexcept { return _native.beta_m; }

		/// @brief Returns beta_n value.
		[[nodiscard]] constexpr texture_handle beta_n() const noexcept { return _native.beta_n; }

		/// @brief Returns alpha value.
		[[nodiscard]] constexpr texture_handle alpha() const noexcept { return _native.alpha; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(bump_map(), sigma_a(), reflectance(), eumelanin(), pheomelanin(), eta(), beta_m(), beta_n(),
			  alpha());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_material_measured_params_t.
	 * @ingroup material-cpp-public
	 */
	struct material_measured_params : wrapper<material_measured_params, tpbrt_material_measured_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<material_bump_map_params, string>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns bump_map value.
		[[nodiscard]] constexpr material_bump_map_params bump_map() const noexcept { return _native.bump_map; }

		/// @brief Returns file_name value.
		[[nodiscard]] constexpr string file_name() const noexcept { return _native.file_name; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(bump_map(), file_name()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_material_mix_params_t.
	 * @ingroup material-cpp-public
	 */
	struct material_mix_params : wrapper<material_mix_params, tpbrt_material_mix_params_t> {
		/// @brief Type used to read materials parameter.
		using materials_type = array<tpbrt_material_handle_t[2], material_handle>;

		/// @brief Type used for reflection.
		using reflect_type	 = std::tuple<materials_type, texture_handle>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns materials value.
		[[nodiscard]] constexpr materials_type materials() const noexcept { return _native.materials; }

		/// @brief Returns amount value.
		[[nodiscard]] constexpr texture_handle amount() const noexcept { return _native.amount; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(materials(), amount()); }
	};

	/**
	 * @copybrief tpbrt_material_subsurface_scattering_value_t
	 * @ingroup material-cpp-public
	 */
	enum class material_subsurface_scattering_value : char_t {
		/// @copybrief TPBRT_MATERIAL_SUBSURFACE_SCATTERING_VALUE_SIGMA
		SIGMA	= TPBRT_MATERIAL_SUBSURFACE_SCATTERING_VALUE_SIGMA,
		/// @copybrief TPBRT_MATERIAL_SUBSURFACE_SCATTERING_VALUE_BUILTIN
		BUILTIN = TPBRT_MATERIAL_SUBSURFACE_SCATTERING_VALUE_BUILTIN,
		/// @copybrief TPBRT_MATERIAL_SUBSURFACE_SCATTERING_VALUE_MAX_NUM
		MAX_NUM = TPBRT_MATERIAL_SUBSURFACE_SCATTERING_VALUE_MAX_NUM,
	};

	/**
	 * @brief Converts material_subsurface_scattering_value enum value to string.
	 * @param scattering_value Material scattering value.
	 * @return String value of material scattering value.
	 * @ingroup material-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const material_subsurface_scattering_value scattering_value) noexcept {
		using enum material_subsurface_scattering_value;
			switch (scattering_value) {
			case SIGMA:	  return "sigma";
			case BUILTIN: return "builtin";
			case MAX_NUM: return "maxnum";
			default:	  return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_material_subsurface_scattering_t.
	 * @ingroup material-cpp-public
	 */
	struct material_subsurface_scattering
		: wrapper<material_subsurface_scattering, tpbrt_material_subsurface_scattering_t>,
		  with_variant<material_subsurface_scattering, std::pair<texture_handle, texture_handle>, scattering_builtin> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<variant_type, material_subsurface_scattering_value>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns type value.
		[[nodiscard]] constexpr material_subsurface_scattering_value type() const noexcept {
			return static_cast<material_subsurface_scattering_value>(_native.type);
		}

		/// @brief Returns if type is sigma.
		[[nodiscard]] constexpr bool_t is_sigma() const noexcept { return type() == material_subsurface_scattering_value::SIGMA; }

		/// @brief Returns if type is builtin.
		[[nodiscard]] constexpr bool_t is_builtin() const noexcept {
			return type() == material_subsurface_scattering_value::BUILTIN;
		}

		/// @brief Returns material scattering value as sigma.
		[[nodiscard]] constexpr std::optional<std::pair<texture_handle, texture_handle>> as_sigma() const noexcept {
				if (!is_sigma()) { return std::nullopt; }
			return std::pair<texture_handle, texture_handle> { _native.as.sigma.a, _native.as.sigma.s };
		}

		/// @brief Returns media parameters as builtin.
		[[nodiscard]] constexpr std::optional<scattering_builtin> as_builtin() const noexcept {
				if (!is_builtin()) { return std::nullopt; }
			return static_cast<scattering_builtin>(_native.as.builtin);
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum material_subsurface_scattering_value;
				switch (type()) {
				case SIGMA:	  return std::pair<texture_handle, texture_handle> { _native.as.sigma.a, _native.as.sigma.s };
				case BUILTIN: return static_cast<scattering_builtin>(_native.as.builtin);
				case MAX_NUM: [[fallthrough]];
				default:	  break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(to_variant(), type()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_material_subsurface_params_t.
	 * @ingroup material-cpp-public
	 */
	struct material_subsurface_params : wrapper<material_subsurface_params, tpbrt_material_subsurface_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<material_bump_map_params, texture_handle, texture_handle, texture_handle,
		  material_subsurface_scattering, texture_handle, material_roughness_params, float_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns bump_map value.
		[[nodiscard]] constexpr material_bump_map_params bump_map() const noexcept { return _native.bump_map; }

		/// @brief Returns eta value.
		[[nodiscard]] constexpr texture_handle eta() const noexcept { return _native.eta; }

		/// @brief Returns g value.
		[[nodiscard]] constexpr texture_handle g() const noexcept { return _native.g; }

		/// @brief Returns mfp value.
		[[nodiscard]] constexpr texture_handle mfp() const noexcept { return _native.mfp; }

		/// @brief Returns scattering value.
		[[nodiscard]] constexpr material_subsurface_scattering scattering() const noexcept { return _native.scattering; }

		/// @brief Returns reflectance value.
		[[nodiscard]] constexpr texture_handle reflectance() const noexcept { return _native.reflectance; }

		/// @brief Returns roughness_params value.
		[[nodiscard]] constexpr material_roughness_params roughness_params() const noexcept { return _native.roughness_params; }

		/// @brief Returns scale value.
		[[nodiscard]] constexpr float_t scale() const noexcept { return _native.scale; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(bump_map(), eta(), g(), mfp(), scattering(), reflectance(), roughness_params(), scale());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_material_thin_dielectric_params_t.
	 * @ingroup material-cpp-public
	 */
	struct material_thin_dielectric_params : wrapper<material_thin_dielectric_params, tpbrt_material_thin_dielectric_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<material_bump_map_params, texture_handle>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns bump_map value.
		[[nodiscard]] constexpr material_bump_map_params bump_map() const noexcept { return _native.bump_map; }

		/// @brief Returns eta value.
		[[nodiscard]] constexpr texture_handle eta() const noexcept { return _native.eta; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(bump_map(), eta()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_material_t.
	 * @ingroup material-cpp-public
	 */
	struct material
		: wrapper<material, tpbrt_material_t>,
		  with_variant<material, material_coated_diffuse_params, material_coated_conductor_params, material_conductor_params,
			material_dielectric_params, material_diffuse_params, material_diffuse_transmission_params, material_hair_params,
			material_measured_params, material_mix_params, material_subsurface_params, material_thin_dielectric_params> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<material_idx_t, string, variant_type, material_type>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns idx value.
		[[nodiscard]] constexpr material_idx_t idx() const noexcept { return _native.idx; }

		/// @brief Returns name value.
		[[nodiscard]] constexpr string name() const noexcept { return _native.name; }

		/// @brief Returns type value.
		[[nodiscard]] constexpr material_type type() const noexcept { return static_cast<material_type>(_native.type); }

		/// @brief Returns if type is coated_diffuse.
		[[nodiscard]] constexpr bool_t is_coated_diffuse() const noexcept { return type() == material_type::COATED_DIFFUSE; }

		/// @brief Returns if type is coated_conductor.
		[[nodiscard]] constexpr bool_t is_coated_conductor() const noexcept { return type() == material_type::COATED_CONDUCTOR; }

		/// @brief Returns if type is conductor.
		[[nodiscard]] constexpr bool_t is_conductor() const noexcept { return type() == material_type::CONDUCTOR; }

		/// @brief Returns if type is dielectric.
		[[nodiscard]] constexpr bool_t is_dielectric() const noexcept { return type() == material_type::DIELECTRIC; }

		/// @brief Returns if type is diffuse.
		[[nodiscard]] constexpr bool_t is_diffuse() const noexcept { return type() == material_type::DIFFUSE; }

		/// @brief Returns if type is diffuse_transmission.
		[[nodiscard]] constexpr bool_t is_diffuse_transmission() const noexcept {
			return type() == material_type::DIFFUSE_TRANSMISSION;
		}

		/// @brief Returns if type is hair.
		[[nodiscard]] constexpr bool_t is_hair() const noexcept { return type() == material_type::HAIR; }

		/// @brief Returns if type is measured.
		[[nodiscard]] constexpr bool_t is_measured() const noexcept { return type() == material_type::MEASURED; }

		/// @brief Returns if type is mix.
		[[nodiscard]] constexpr bool_t is_mix() const noexcept { return type() == material_type::MIX; }

		/// @brief Returns if type is subsurface.
		[[nodiscard]] constexpr bool_t is_subsurface() const noexcept { return type() == material_type::SUBSURFACE; }

		/// @brief Returns if type is thin_dielectric.
		[[nodiscard]] constexpr bool_t is_thin_dielectric() const noexcept { return type() == material_type::THIN_DIELECTRIC; }

		/// @brief Returns material parameters as coated_diffuse.
		[[nodiscard]] constexpr std::optional<material_coated_diffuse_params> as_coated_diffuse() const noexcept {
				if (!is_coated_diffuse()) { return std::nullopt; }
			return _native.as.coated_diffuse;
		}

		/// @brief Returns material parameters as coated_conductor.
		[[nodiscard]] constexpr std::optional<material_coated_conductor_params> as_coated_conductor() const noexcept {
				if (!is_coated_conductor()) { return std::nullopt; }
			return _native.as.coated_conductor;
		}

		/// @brief Returns material parameters as conductor.
		[[nodiscard]] constexpr std::optional<material_conductor_params> as_conductor() const noexcept {
				if (!is_conductor()) { return std::nullopt; }
			return _native.as.conductor;
		}

		/// @brief Returns material parameters as dielectric.
		[[nodiscard]] constexpr std::optional<material_dielectric_params> as_dielectric() const noexcept {
				if (!is_dielectric()) { return std::nullopt; }
			return _native.as.dielectric;
		}

		/// @brief Returns material parameters as diffuse.
		[[nodiscard]] constexpr std::optional<material_diffuse_params> as_diffuse() const noexcept {
				if (!is_diffuse()) { return std::nullopt; }
			return _native.as.diffuse;
		}

		/// @brief Returns material parameters as diffuse_transmission.
		[[nodiscard]] constexpr std::optional<material_diffuse_transmission_params> as_diffuse_transmission() const noexcept {
				if (!is_diffuse_transmission()) { return std::nullopt; }
			return _native.as.diffuse_transmission;
		}

		/// @brief Returns material parameters as hair.
		[[nodiscard]] constexpr std::optional<material_hair_params> as_hair() const noexcept {
				if (!is_hair()) { return std::nullopt; }
			return _native.as.hair;
		}

		/// @brief Returns material parameters as measured.
		[[nodiscard]] constexpr std::optional<material_measured_params> as_measured() const noexcept {
				if (!is_measured()) { return std::nullopt; }
			return _native.as.measured;
		}

		/// @brief Returns material parameters as mix.
		[[nodiscard]] constexpr std::optional<material_mix_params> as_mix() const noexcept {
				if (!is_mix()) { return std::nullopt; }
			return _native.as.mix;
		}

		/// @brief Returns material parameters as subsurface.
		[[nodiscard]] constexpr std::optional<material_subsurface_params> as_subsurface() const noexcept {
				if (!is_subsurface()) { return std::nullopt; }
			return _native.as.subsurface;
		}

		/// @brief Returns material parameters as thin_dielectric.
		[[nodiscard]] constexpr std::optional<material_thin_dielectric_params> as_thin_dielectric() const noexcept {
				if (!is_thin_dielectric()) { return std::nullopt; }
			return _native.as.thin_dielectric;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum material_type;
				switch (type()) {
				case COATED_DIFFUSE:	   return _native.as.coated_diffuse;
				case COATED_CONDUCTOR:	   return _native.as.coated_conductor;
				case CONDUCTOR:			   return _native.as.conductor;
				case DIELECTRIC:		   return _native.as.dielectric;
				case DIFFUSE:			   return _native.as.diffuse;
				case DIFFUSE_TRANSMISSION: return _native.as.diffuse_transmission;
				case HAIR:				   return _native.as.hair;
				case MEASURED:			   return _native.as.measured;
				case MIX:				   return _native.as.mix;
				case SUBSURFACE:		   return _native.as.subsurface;
				case THIN_DIELECTRIC:	   return _native.as.thin_dielectric;
				case MAX_NUM:			   [[fallthrough]];
				default:				   break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(idx(), name(), to_variant(), type());
		}
	};

	/**
	 * @brief Returns material to which handle is provided.
	 * @param materials_list List of materials in which material given by handle should be.
	 * @param handle Handle to material.
	 * @return Returns result with error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE) or with material.
	 * @ingroup material-cpp-public
	 */
	[[nodiscard]] constexpr result<material> get_material(materials_list materials_list, material_handle handle) noexcept;

	/**
	 * @brief Returns material with given name.
	 * @param materials_list List of materials in which searched material should be.
	 * @param name Name of the searched material.
	 * @return Returns result with error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE) or with material.
	 * @ingroup material-cpp-public
	 */
	[[nodiscard]] constexpr result<material> get_material(materials_list materials_list, string name) noexcept;

	/**
	 * @brief C++ wrapper for tpbrt_materials_list_t.
	 * @ingroup material-cpp-public
	 */
	struct materials_list : array<tpbrt_materials_list_t, material, &tpbrt_materials_list_t::materials> {
		/// @brief Base list constructor.
		using array::array;

		/// @brief Returns element pointed by given handle.
		[[nodiscard]] constexpr result<element_type> at(const material_handle handle) const noexcept {
			return get_material(*this, handle);
		}

		/// @brief Returns element pointed by given name.
		[[nodiscard]] constexpr result<element_type> at(const string name) const noexcept { return get_material(*this, name); }

		/// @brief Returns element pointed by given handle.
		[[nodiscard]] constexpr result<element_type> operator[](const material_handle handle) const noexcept {
			return at(handle);
		}

		/// @brief Returns element pointed by given name.
		[[nodiscard]] constexpr result<element_type> operator[](const string name) const noexcept { return at(name); }
	};

	constexpr result<material> get_material(const materials_list materials_list, const material_handle handle) noexcept {
		const tpbrt_material_t* c_material = nullptr;

		const auto err = static_cast<error>(tpbrt_get_material_by_handle(materials_list, handle, &c_material));

			if (err == error::NONE && c_material != nullptr) { return material(*c_material); }

		return err;
	}

	constexpr result<material> get_material(const materials_list materials_list, const string name) noexcept {
		const tpbrt_material_t* c_material = nullptr;

		const auto err					   = static_cast<error>(tpbrt_get_material_by_name(materials_list, name, &c_material));

			if (err == error::NONE && c_material != nullptr) { return material(*c_material); }

		return err;
	}
} // namespace tpbrt
#endif // _TINYPBRT_MATERIAL_HPP_
