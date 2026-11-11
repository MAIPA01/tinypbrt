#pragma once
#ifndef _TINYPBRT_SAMPLERS_HPP_
#define _TINYPBRT_SAMPLERS_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/sampler.h>

#include <tinypbrt/detail/cpp/common.hpp>

namespace tpbrt {
	/**
	 * @defgroup sampler-cpp-public Sampler
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @copybrief tpbrt_sampler_type_t
	 * @ingroup sampler-cpp-public
	 */
	enum class sampler_type : char_t {
		/// @copybrief TPBRT_SAMPLER_TYPE_HALTON
		HALTON		 = TPBRT_SAMPLER_TYPE_HALTON,
		/// @copybrief TPBRT_SAMPLER_TYPE_INDEPENDENT
		INDEPENDENT	 = TPBRT_SAMPLER_TYPE_INDEPENDENT,
		/// @copybrief TPBRT_SAMPLER_TYPE_PADDED_SOBOL
		PADDED_SOBOL = TPBRT_SAMPLER_TYPE_PADDED_SOBOL,
		/// @copybrief TPBRT_SAMPLER_TYPE_SOBOL
		SOBOL		 = TPBRT_SAMPLER_TYPE_SOBOL,
		/// @copybrief TPBRT_SAMPLER_TYPE_STRATIFIED
		STRATIFIED	 = TPBRT_SAMPLER_TYPE_STRATIFIED,
		/// @copybrief TPBRT_SAMPLER_TYPE_Z_SOBOL
		Z_SOBOL		 = TPBRT_SAMPLER_TYPE_Z_SOBOL,
		/// @copybrief TPBRT_SAMPLER_TYPE_MAX_NUM
		MAX_NUM		 = TPBRT_SAMPLER_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts sampler_type enum value to string.
	 * @param type Sampler type.
	 * @return String value of sampler type.
	 * @ingroup sampler-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const sampler_type type) noexcept {
		using enum sampler_type;
			switch (type) {
			case HALTON:	   return "halton";
			case INDEPENDENT:  return "independent";
			case PADDED_SOBOL: return "paddedsobol";
			case SOBOL:		   return "sobol";
			case STRATIFIED:   return "stratified";
			case Z_SOBOL:	   return "zsobol";
			case MAX_NUM:	   return "maxnum";
			default:		   return "UNKNOWN";
			}
	}

	/**
	 * @copybrief tpbrt_sampler_type_t
	 * @ingroup sampler-cpp-public
	 */
	enum class sampler_random : char_t {
		/// @copybrief TPBRT_SAMPLER_RANDOM_NONE
		NONE		   = TPBRT_SAMPLER_RANDOM_NONE,
		/// @copybrief TPBRT_SAMPLER_RANDOM_PERMUTE_DIGITS
		PERMUTE_DIGITS = TPBRT_SAMPLER_RANDOM_PERMUTE_DIGITS,
		/// @copybrief TPBRT_SAMPLER_RANDOM_OWEN
		OWEN		   = TPBRT_SAMPLER_RANDOM_OWEN,
		/// @copybrief TPBRT_SAMPLER_RANDOM_FAST_OWEN
		FAST_OWEN	   = TPBRT_SAMPLER_RANDOM_FAST_OWEN,
		/// @copybrief TPBRT_SAMPLER_RANDOM_MAX_NUM
		MAX_NUM		   = TPBRT_SAMPLER_RANDOM_MAX_NUM,
	};

	/**
	 * @brief Converts sampler_random enum value to string.
	 * @param random Sampler random.
	 * @return String value of sampler random.
	 * @ingroup sampler-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const sampler_random random) noexcept {
		using enum sampler_random;
			switch (random) {
			case NONE:			 return "none";
			case PERMUTE_DIGITS: return "permutedigits";
			case OWEN:			 return "owen";
			case FAST_OWEN:		 return "fastowen";
			case MAX_NUM:		 return "maxnum";
			default:			 return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_sampler_halton_params_t.
	 * @ingroup sampler-cpp-public
	 */
	struct sampler_halton_params : wrapper<sampler_halton_params, tpbrt_sampler_halton_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t, sampler_random>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns pixel_samples value.
		[[nodiscard]] constexpr uint_t pixel_samples() const noexcept { return _native.pixel_samples; }

		/// @brief Returns randomization value.
		[[nodiscard]] constexpr sampler_random randomization() const noexcept {
			return static_cast<sampler_random>(_native.randomization);
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(pixel_samples(), randomization());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_sampler_independent_params_t.
	 * @ingroup sampler-cpp-public
	 */
	struct sampler_independent_params : wrapper<sampler_independent_params, tpbrt_sampler_independent_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns pixel_samples value.
		[[nodiscard]] constexpr uint_t pixel_samples() const noexcept { return _native.pixel_samples; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(pixel_samples()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_sampler_padded_sobol_params_t.
	 * @ingroup sampler-cpp-public
	 */
	struct sampler_padded_sobol_params : wrapper<sampler_padded_sobol_params, tpbrt_sampler_padded_sobol_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t, sampler_random>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns pixel_samples value.
		[[nodiscard]] constexpr uint_t pixel_samples() const noexcept { return _native.pixel_samples; }

		/// @brief Returns randomization value.
		[[nodiscard]] constexpr sampler_random randomization() const noexcept {
			return static_cast<sampler_random>(_native.randomization);
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(pixel_samples(), randomization());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_sampler_sobol_params_t.
	 * @ingroup sampler-cpp-public
	 */
	struct sampler_sobol_params : wrapper<sampler_sobol_params, tpbrt_sampler_sobol_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t, sampler_random>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns pixel_samples value.
		[[nodiscard]] constexpr uint_t pixel_samples() const noexcept { return _native.pixel_samples; }

		/// @brief Returns randomization value.
		[[nodiscard]] constexpr sampler_random randomization() const noexcept {
			return static_cast<sampler_random>(_native.randomization);
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(pixel_samples(), randomization());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_sampler_stratified_params_t.
	 * @ingroup sampler-cpp-public
	 */
	struct sampler_stratified_params : wrapper<sampler_stratified_params, tpbrt_sampler_stratified_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t, uint_t, bool_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns x_samples value.
		[[nodiscard]] constexpr uint_t x_samples() const noexcept { return _native.x_samples; }

		/// @brief Returns y_samples value.
		[[nodiscard]] constexpr uint_t y_samples() const noexcept { return _native.y_samples; }

		/// @brief Returns jitter value.
		[[nodiscard]] constexpr bool_t jitter() const noexcept { return _native.jitter; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(x_samples(), y_samples(), jitter());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_sampler_z_sobol_params_t.
	 * @ingroup sampler-cpp-public
	 */
	struct sampler_z_sobol_params : wrapper<sampler_z_sobol_params, tpbrt_sampler_z_sobol_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t, sampler_random>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns pixel_samples value.
		[[nodiscard]] constexpr uint_t pixel_samples() const noexcept { return _native.pixel_samples; }

		/// @brief Returns randomization value.
		[[nodiscard]] constexpr sampler_random randomization() const noexcept {
			return static_cast<sampler_random>(_native.randomization);
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(pixel_samples(), randomization());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_sampler_t.
	 * @ingroup sampler-cpp-public
	 */
	struct sampler : wrapper<sampler, tpbrt_sampler_t>,
					 with_variant<sampler, sampler_halton_params, sampler_independent_params, sampler_padded_sobol_params,
					   sampler_sobol_params, sampler_stratified_params, sampler_z_sobol_params> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<int_t, variant_type, sampler_type>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns seed value.
		[[nodiscard]] constexpr int_t seed() const noexcept { return _native.seed; }

		/// @brief Returns type value.
		[[nodiscard]] constexpr sampler_type type() const noexcept { return static_cast<sampler_type>(_native.type); }

		/// @brief Returns if type is halton.
		[[nodiscard]] constexpr bool_t is_halton() const noexcept { return type() == sampler_type::HALTON; }

		/// @brief Returns if type is independent.
		[[nodiscard]] constexpr bool_t is_independent() const noexcept { return type() == sampler_type::INDEPENDENT; }

		/// @brief Returns if type is padded_sobol.
		[[nodiscard]] constexpr bool_t is_padded_sobol() const noexcept { return type() == sampler_type::PADDED_SOBOL; }

		/// @brief Returns if type is sobol.
		[[nodiscard]] constexpr bool_t is_sobol() const noexcept { return type() == sampler_type::SOBOL; }

		/// @brief Returns if type is stratified.
		[[nodiscard]] constexpr bool_t is_stratified() const noexcept { return type() == sampler_type::STRATIFIED; }

		/// @brief Returns if type is z_sobol.
		[[nodiscard]] constexpr bool_t is_z_sobol() const noexcept { return type() == sampler_type::Z_SOBOL; }

		/// @brief Returns sampler parameters as halton.
		[[nodiscard]] constexpr std::optional<sampler_halton_params> as_halton() const noexcept {
				if (!is_halton()) { return std::nullopt; }
			return _native.as.halton;
		}

		/// @brief Returns sampler parameters as independent.
		[[nodiscard]] constexpr std::optional<sampler_independent_params> as_independent() const noexcept {
				if (!is_independent()) { return std::nullopt; }
			return _native.as.independent;
		}

		/// @brief Returns sampler parameters as padded_sobol.
		[[nodiscard]] constexpr std::optional<sampler_padded_sobol_params> as_padded_sobol() const noexcept {
				if (!is_padded_sobol()) { return std::nullopt; }
			return _native.as.padded_sobol;
		}

		/// @brief Returns sampler parameters as sobol.
		[[nodiscard]] constexpr std::optional<sampler_sobol_params> as_sobol() const noexcept {
				if (!is_sobol()) { return std::nullopt; }
			return _native.as.sobol;
		}

		/// @brief Returns sampler parameters as stratified.
		[[nodiscard]] constexpr std::optional<sampler_stratified_params> as_stratified() const noexcept {
				if (!is_stratified()) { return std::nullopt; }
			return _native.as.stratified;
		}

		/// @brief Returns sampler parameters as z_sobol.
		[[nodiscard]] constexpr std::optional<sampler_z_sobol_params> as_z_sobol() const noexcept {
				if (!is_z_sobol()) { return std::nullopt; }
			return _native.as.z_sobol;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum sampler_type;
				switch (type()) {
				case HALTON:	   return _native.as.halton;
				case INDEPENDENT:  return _native.as.independent;
				case PADDED_SOBOL: return _native.as.padded_sobol;
				case SOBOL:		   return _native.as.sobol;
				case STRATIFIED:   return _native.as.stratified;
				case Z_SOBOL:	   return _native.as.z_sobol;
				case MAX_NUM:	   [[fallthrough]];
				default:		   break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(seed(), to_variant(), type()); }
	};
} // namespace tpbrt
#endif // _TINYPBRT_SAMPLERS_HPP_
