#pragma once
#ifndef _TINYPBRT_INTEGRATOR_HPP_
#define _TINYPBRT_INTEGRATOR_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/integrator.h>

#include <tinypbrt/detail/cpp/common.hpp>

namespace tpbrt {
	/**
	 * @defgroup integrator-cpp-public Integrator
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @copybrief tpbrt_integrator_type_t
	 * @ingroup integrator-cpp-public
	 */
	enum class integrator_type : char_t {
		/// @copybrief TPBRT_INTEGRATOR_TYPE_AMBIENT_OCCLUSION
		AMBIENT_OCCLUSION = TPBRT_INTEGRATOR_TYPE_AMBIENT_OCCLUSION,
		/// @copybrief TPBRT_INTEGRATOR_TYPE_BDPT
		BDPT			  = TPBRT_INTEGRATOR_TYPE_BDPT,
		/// @copybrief TPBRT_INTEGRATOR_TYPE_LIGHT_PATH
		LIGHT_PATH		  = TPBRT_INTEGRATOR_TYPE_LIGHT_PATH,
		/// @copybrief TPBRT_INTEGRATOR_TYPE_MLT
		MLT				  = TPBRT_INTEGRATOR_TYPE_MLT,
		/// @copybrief TPBRT_INTEGRATOR_TYPE_PATH
		PATH			  = TPBRT_INTEGRATOR_TYPE_PATH,
		/// @copybrief TPBRT_INTEGRATOR_TYPE_RANDOM_WALK
		RANDOM_WALK		  = TPBRT_INTEGRATOR_TYPE_RANDOM_WALK,
		/// @copybrief TPBRT_INTEGRATOR_TYPE_SIMPLE_PATH
		SIMPLE_PATH		  = TPBRT_INTEGRATOR_TYPE_SIMPLE_PATH,
		/// @copybrief TPBRT_INTEGRATOR_TYPE_SIMPLE_VOL_PATH
		SIMPLE_VOL_PATH	  = TPBRT_INTEGRATOR_TYPE_SIMPLE_VOL_PATH,
		/// @copybrief TPBRT_INTEGRATOR_TYPE_SPPM
		SPPM			  = TPBRT_INTEGRATOR_TYPE_SPPM,
		/// @copybrief TPBRT_INTEGRATOR_TYPE_VOL_PATH
		VOL_PATH		  = TPBRT_INTEGRATOR_TYPE_VOL_PATH,
		/// @copybrief TPBRT_INTEGRATOR_TYPE_MAX_NUM
		MAX_NUM			  = TPBRT_INTEGRATOR_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts integrator_type enum value to string.
	 * @param type Integrator type.
	 * @return String value of integrator type.
	 * @ingroup integrator-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const integrator_type type) noexcept {
		using enum integrator_type;
			switch (type) {
			case AMBIENT_OCCLUSION: return "ambientocclusion";
			case BDPT:				return "bdpt";
			case LIGHT_PATH:		return "lightpath";
			case MLT:				return "mlt";
			case PATH:				return "path";
			case RANDOM_WALK:		return "randomwalk";
			case SIMPLE_PATH:		return "simplepath";
			case SIMPLE_VOL_PATH:	return "simplevolpath";
			case SPPM:				return "volpath";
			case VOL_PATH:			return "preset";
			case MAX_NUM:			return "maxnum";
			default:				return "UNKNOWN";
			}
	}

	/**
	 * @copybrief tpbrt_integrator_light_sampler_t
	 * @ingroup integrator-cpp-public
	 */
	enum class integrator_light_sampler : char_t {
		/// @copybrief TPBRT_INTEGRATOR_LIGHT_SAMPLER_BVH
		BVH		= TPBRT_INTEGRATOR_LIGHT_SAMPLER_BVH,
		/// @copybrief TPBRT_INTEGRATOR_LIGHT_SAMPLER_UNIFORM
		UNIFORM = TPBRT_INTEGRATOR_LIGHT_SAMPLER_UNIFORM,
		/// @copybrief TPBRT_INTEGRATOR_LIGHT_SAMPLER_POWER
		POWER	= TPBRT_INTEGRATOR_LIGHT_SAMPLER_POWER,
		/// @copybrief TPBRT_INTEGRATOR_LIGHT_SAMPLER_MAX_NUM
		MAX_NUM = TPBRT_INTEGRATOR_LIGHT_SAMPLER_MAX_NUM,
	};

	/**
	 * @brief Converts integrator_light_sampler enum value to string.
	 * @param sampler Integrator light sampler.
	 * @return String value of integrator light sampler.
	 * @ingroup integrator-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const integrator_light_sampler sampler) noexcept {
		using enum integrator_light_sampler;
			switch (sampler) {
			case BVH:	  return "bvh";
			case UNIFORM: return "uniform";
			case POWER:	  return "power";
			case MAX_NUM: return "maxnum";
			default:	  return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_integrator_ambient_occlusion_params_t.
	 * @ingroup integrator-cpp-public
	 */
	struct integrator_ambient_occlusion_params
		: wrapper<integrator_ambient_occlusion_params, tpbrt_integrator_ambient_occlusion_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<float_t, bool_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns max_distance value.
		[[nodiscard]] constexpr float_t max_distance() const noexcept { return _native.max_distance; }

		/// @brief Returns cos_sample value.
		[[nodiscard]] constexpr bool_t cos_sample() const noexcept { return _native.cos_sample; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(max_distance(), cos_sample()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_integrator_bdpt_params_t.
	 * @ingroup integrator-cpp-public
	 */
	struct integrator_bdpt_params : wrapper<integrator_bdpt_params, tpbrt_integrator_bdpt_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t, bool_t, bool_t, bool_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns max_depth value.
		[[nodiscard]] constexpr uint_t max_depth() const noexcept { return _native.max_depth; }

		/// @brief Returns regularize value.
		[[nodiscard]] constexpr bool_t regularize() const noexcept { return _native.regularize; }

		/// @brief Returns visualize_strategies value.
		[[nodiscard]] constexpr bool_t visualize_strategies() const noexcept { return _native.visualize_strategies; }

		/// @brief Returns visualize_weights value.
		[[nodiscard]] constexpr bool_t visualize_weights() const noexcept { return _native.visualize_weights; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(max_depth(), regularize(), visualize_strategies(), visualize_weights());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_integrator_light_path_params_t.
	 * @ingroup integrator-cpp-public
	 */
	struct integrator_light_path_params : wrapper<integrator_light_path_params, tpbrt_integrator_light_path_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns max_depth value.
		[[nodiscard]] constexpr uint_t max_depth() const noexcept { return _native.max_depth; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(max_depth()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_integrator_mlt_params_t.
	 * @ingroup integrator-cpp-public
	 */
	struct integrator_mlt_params : wrapper<integrator_mlt_params, tpbrt_integrator_mlt_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t, uint_t, uint_t, uint_t, float_t, float_t, bool_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns max_depth value.
		[[nodiscard]] constexpr uint_t max_depth() const noexcept { return _native.max_depth; }

		/// @brief Returns bootstrap_samples value.
		[[nodiscard]] constexpr uint_t bootstrap_samples() const noexcept { return _native.bootstrap_samples; }

		/// @brief Returns chains value.
		[[nodiscard]] constexpr uint_t chains() const noexcept { return _native.chains; }

		/// @brief Returns mutations_per_pixel value.
		[[nodiscard]] constexpr uint_t mutations_per_pixel() const noexcept { return _native.mutations_per_pixel; }

		/// @brief Returns large_step_probability value.
		[[nodiscard]] constexpr float_t large_step_probability() const noexcept { return _native.large_step_probability; }

		/// @brief Returns sigma value.
		[[nodiscard]] constexpr float_t sigma() const noexcept { return _native.sigma; }

		/// @brief Returns regularize value.
		[[nodiscard]] constexpr bool_t regularize() const noexcept { return _native.regularize; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(max_depth(), bootstrap_samples(), chains(), mutations_per_pixel(), large_step_probability(),
			  sigma(), regularize());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_integrator_path_params_t.
	 * @ingroup integrator-cpp-public
	 */
	struct integrator_path_params : wrapper<integrator_path_params, tpbrt_integrator_path_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t, integrator_light_sampler, bool_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns max_depth value.
		[[nodiscard]] constexpr uint_t max_depth() const noexcept { return _native.max_depth; }

		/// @brief Returns light_sampler value.
		[[nodiscard]] constexpr integrator_light_sampler light_sampler() const noexcept {
			return static_cast<integrator_light_sampler>(_native.light_sampler);
		}

		/// @brief Returns regularize value.
		[[nodiscard]] constexpr bool_t regularize() const noexcept { return _native.regularize; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(max_depth(), light_sampler(), regularize());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_integrator_random_walk_params_t.
	 * @ingroup integrator-cpp-public
	 */
	struct integrator_random_walk_params : wrapper<integrator_random_walk_params, tpbrt_integrator_random_walk_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns max_depth value.
		[[nodiscard]] constexpr uint_t max_depth() const noexcept { return _native.max_depth; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(max_depth()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_integrator_simple_path_params_t.
	 * @ingroup integrator-cpp-public
	 */
	struct integrator_simple_path_params : wrapper<integrator_simple_path_params, tpbrt_integrator_simple_path_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t, bool_t, bool_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns max_depth value.
		[[nodiscard]] constexpr uint_t max_depth() const noexcept { return _native.max_depth; }

		/// @brief Returns sample_bsdf value.
		[[nodiscard]] constexpr bool_t sample_bsdf() const noexcept { return _native.sample_bsdf; }

		/// @brief Returns sample_lights value.
		[[nodiscard]] constexpr bool_t sample_lights() const noexcept { return _native.sample_lights; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(max_depth(), sample_bsdf(), sample_lights());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_integrator_simple_vol_path_params_t.
	 * @ingroup integrator-cpp-public
	 */
	struct integrator_simple_vol_path_params
		: wrapper<integrator_simple_vol_path_params, tpbrt_integrator_simple_vol_path_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns max_depth value.
		[[nodiscard]] constexpr uint_t max_depth() const noexcept { return _native.max_depth; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(max_depth()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_integrator_sppm_params_t.
	 * @ingroup integrator-cpp-public
	 */
	struct integrator_sppm_params : wrapper<integrator_sppm_params, tpbrt_integrator_sppm_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t, int_t, float_t, int_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns max_depth value.
		[[nodiscard]] constexpr uint_t max_depth() const noexcept { return _native.max_depth; }

		/// @brief Returns photons_per_iteration value.
		[[nodiscard]] constexpr int_t photons_per_iteration() const noexcept { return _native.photons_per_iteration; }

		/// @brief Returns radius value.
		[[nodiscard]] constexpr float_t radius() const noexcept { return _native.radius; }

		/// @brief Returns seed value.
		[[nodiscard]] constexpr int_t seed() const noexcept { return _native.seed; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(max_depth(), photons_per_iteration(), radius(), seed());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_integrator_vol_path_params_t.
	 * @ingroup integrator-cpp-public
	 */
	struct integrator_vol_path_params : wrapper<integrator_vol_path_params, tpbrt_integrator_vol_path_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<uint_t, integrator_light_sampler, bool_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns max_depth value.
		[[nodiscard]] constexpr uint_t max_depth() const noexcept { return _native.max_depth; }

		/// @brief Returns light_sampler value.
		[[nodiscard]] constexpr integrator_light_sampler light_sampler() const noexcept {
			return static_cast<integrator_light_sampler>(_native.light_sampler);
		}

		/// @brief Returns regularize value.
		[[nodiscard]] constexpr bool_t regularize() const noexcept { return _native.regularize; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(max_depth(), light_sampler(), regularize());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_integrator_t.
	 * @ingroup integrator-cpp-public
	 */
	struct integrator
		: wrapper<integrator, tpbrt_integrator_t>,
		  with_variant<integrator, integrator_ambient_occlusion_params, integrator_bdpt_params, integrator_light_path_params,
			integrator_mlt_params, integrator_path_params, integrator_random_walk_params, integrator_simple_path_params,
			integrator_simple_vol_path_params, integrator_sppm_params, integrator_vol_path_params> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<variant_type, integrator_type>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns type value.
		[[nodiscard]] constexpr integrator_type type() const noexcept { return static_cast<integrator_type>(_native.type); }

		/// @brief Returns if type is ambient_occlusion.
		[[nodiscard]] constexpr bool_t is_ambient_occlusion() const noexcept {
			return type() == integrator_type::AMBIENT_OCCLUSION;
		}

		/// @brief Returns if type is bdpt.
		[[nodiscard]] constexpr bool_t is_bdpt() const noexcept { return type() == integrator_type::BDPT; }

		/// @brief Returns if type is light_path.
		[[nodiscard]] constexpr bool_t is_light_path() const noexcept { return type() == integrator_type::LIGHT_PATH; }

		/// @brief Returns if type is mlt.
		[[nodiscard]] constexpr bool_t is_mlt() const noexcept { return type() == integrator_type::MLT; }

		/// @brief Returns if type is path.
		[[nodiscard]] constexpr bool_t is_path() const noexcept { return type() == integrator_type::PATH; }

		/// @brief Returns if type is random_walk.
		[[nodiscard]] constexpr bool_t is_random_walk() const noexcept { return type() == integrator_type::RANDOM_WALK; }

		/// @brief Returns if type is simple_path.
		[[nodiscard]] constexpr bool_t is_simple_path() const noexcept { return type() == integrator_type::SIMPLE_PATH; }

		/// @brief Returns if type is simple_vol_path.
		[[nodiscard]] constexpr bool_t is_simple_vol_path() const noexcept { return type() == integrator_type::SIMPLE_VOL_PATH; }

		/// @brief Returns if type is sppm.
		[[nodiscard]] constexpr bool_t is_sppm() const noexcept { return type() == integrator_type::SPPM; }

		/// @brief Returns if type is vol_path.
		[[nodiscard]] constexpr bool_t is_vol_path() const noexcept { return type() == integrator_type::VOL_PATH; }

		/// @brief Returns integrator parameters as ambient_occlusion.
		[[nodiscard]] constexpr std::optional<integrator_ambient_occlusion_params> as_ambient_occlusion() const noexcept {
				if (!is_ambient_occlusion()) { return std::nullopt; }
			return _native.as.ambient_occlusion;
		}

		/// @brief Returns integrator parameters as bdpt.
		[[nodiscard]] constexpr std::optional<integrator_bdpt_params> as_bdpt() const noexcept {
				if (!is_bdpt()) { return std::nullopt; }
			return _native.as.bdpt;
		}

		/// @brief Returns integrator parameters as light_path.
		[[nodiscard]] constexpr std::optional<integrator_light_path_params> as_light_path() const noexcept {
				if (!is_light_path()) { return std::nullopt; }
			return _native.as.light_path;
		}

		/// @brief Returns integrator parameters as mlt.
		[[nodiscard]] constexpr std::optional<integrator_mlt_params> as_mlt() const noexcept {
				if (!is_mlt()) { return std::nullopt; }
			return _native.as.mlt;
		}

		/// @brief Returns integrator parameters as path.
		[[nodiscard]] constexpr std::optional<integrator_path_params> as_path() const noexcept {
				if (!is_path()) { return std::nullopt; }
			return _native.as.path;
		}

		/// @brief Returns integrator parameters as random_walk.
		[[nodiscard]] constexpr std::optional<integrator_random_walk_params> as_random_walk() const noexcept {
				if (!is_random_walk()) { return std::nullopt; }
			return _native.as.random_walk;
		}

		/// @brief Returns integrator parameters as simple_path.
		[[nodiscard]] constexpr std::optional<integrator_simple_path_params> as_simple_path() const noexcept {
				if (!is_simple_path()) { return std::nullopt; }
			return _native.as.simple_path;
		}

		/// @brief Returns integrator parameters as simple_vol_path.
		[[nodiscard]] constexpr std::optional<integrator_simple_vol_path_params> as_simple_vol_path() const noexcept {
				if (!is_simple_vol_path()) { return std::nullopt; }
			return _native.as.simple_vol_path;
		}

		/// @brief Returns integrator parameters as sppm.
		[[nodiscard]] constexpr std::optional<integrator_sppm_params> as_sppm() const noexcept {
				if (!is_sppm()) { return std::nullopt; }
			return _native.as.sppm;
		}

		/// @brief Returns integrator parameters as vol_path.
		[[nodiscard]] constexpr std::optional<integrator_vol_path_params> as_vol_path() const noexcept {
				if (!is_vol_path()) { return std::nullopt; }
			return _native.as.vol_path;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum integrator_type;
				switch (type()) {
				case AMBIENT_OCCLUSION: return _native.as.ambient_occlusion;
				case BDPT:				return _native.as.bdpt;
				case LIGHT_PATH:		return _native.as.light_path;
				case MLT:				return _native.as.mlt;
				case PATH:				return _native.as.path;
				case RANDOM_WALK:		return _native.as.random_walk;
				case SIMPLE_PATH:		return _native.as.simple_path;
				case SIMPLE_VOL_PATH:	return _native.as.simple_vol_path;
				case SPPM:				return _native.as.sppm;
				case VOL_PATH:			return _native.as.vol_path;
				case MAX_NUM:			[[fallthrough]];
				default:				break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(to_variant(), type()); }
	};
} // namespace tpbrt
#endif // _TINYPBRT_INTEGRATOR_HPP_
