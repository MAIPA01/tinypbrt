#pragma once
#ifndef _TINYPBRT_FILTER_HPP_
#define _TINYPBRT_FILTER_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/filter.h>

#include <tinypbrt/detail/cpp/common.hpp>

namespace tpbrt {
	/**
	 * @defgroup filter-cpp-public Filter
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @copybrief tpbrt_filter_type_t
	 * @ingroup filter-cpp-public
	 */
	enum class filter_type : uchar_t {
		/// @copybrief TPBRT_FILTER_TYPE_BOX
		BOX		 = TPBRT_FILTER_TYPE_BOX,
		/// @copybrief TPBRT_FILTER_TYPE_GAUSSIAN
		GAUSSIAN = TPBRT_FILTER_TYPE_GAUSSIAN,
		/// @copybrief TPBRT_FILTER_TYPE_MITCHELL
		MITCHELL = TPBRT_FILTER_TYPE_MITCHELL,
		/// @copybrief TPBRT_FILTER_TYPE_SINC
		SINC	 = TPBRT_FILTER_TYPE_SINC,
		/// @copybrief TPBRT_FILTER_TYPE_TRIANGLE
		TRIANGLE = TPBRT_FILTER_TYPE_TRIANGLE,
		/// @copybrief TPBRT_FILTER_TYPE_MAX_NUM
		MAX_NUM	 = TPBRT_FILTER_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts filter_type enum value to string.
	 * @param type Filter type.
	 * @return String value of filter type.
	 * @ingroup filter-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const filter_type type) noexcept {
		using enum filter_type;
			switch (type) {
			case BOX:	   return "box";
			case GAUSSIAN: return "gaussian";
			case MITCHELL: return "mitchell";
			case SINC:	   return "sinc";
			case TRIANGLE: return "triangle";
			case MAX_NUM:  return "maxnum";
			default:	   return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_filter_gaussian_params_t.
	 * @ingroup filter-cpp-public
	 */
	struct filter_gaussian_params : detail::wrapper<filter_gaussian_params, tpbrt_filter_gaussian_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns sigma value.
		[[nodiscard]] constexpr float_t sigma() const noexcept { return _native->sigma; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(sigma()); }
	};

	/**
	 * @brief  C++ wrapper for tpbrt_filter_mitchell_params_t.
	 * @ingroup filter-cpp-public
	 */
	struct filter_mitchell_params : detail::wrapper<filter_mitchell_params, tpbrt_filter_mitchell_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns b value.
		[[nodiscard]] constexpr float_t b() const noexcept { return _native->b; }

		/// @brief Returns c value.
		[[nodiscard]] constexpr float_t c() const noexcept { return _native->c; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(b(), c()); }
	};

	/**
	 * @brief  C++ wrapper for tpbrt_filter_sinc_params_t.
	 * @ingroup filter-cpp-public
	 */
	struct filter_sinc_params : detail::wrapper<filter_sinc_params, tpbrt_filter_sinc_params_t> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns tau value.
		[[nodiscard]] constexpr float_t tau() const noexcept { return _native->tau; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept { return std::make_tuple(tau()); }
	};

	/**
	 * @brief  C++ wrapper for tpbrt_filter_t.
	 * @ingroup filter-cpp-public
	 */
	struct filter : detail::wrapper<filter, tpbrt_filter_t>,
					detail::with_variant<filter, filter_gaussian_params, filter_mitchell_params, filter_sinc_params> {
		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns x_radius value.
		[[nodiscard]] constexpr float_t x_radius() const noexcept { return _native->x_radius; }

		/// @brief Returns y_radius value.
		[[nodiscard]] constexpr float_t y_radius() const noexcept { return _native->y_radius; }

		/// @brief Returns type value.
		[[nodiscard]] constexpr filter_type type() const noexcept { return static_cast<filter_type>(_native->type); }

		/// @brief Returns true if type is gaussian.
		[[nodiscard]] constexpr bool_t is_gaussian() const noexcept { return type() == filter_type::GAUSSIAN; }

		/// @brief Returns true if type is mitchell.
		[[nodiscard]] constexpr bool_t is_mitchell() const noexcept { return type() == filter_type::MITCHELL; }

		/// @brief Returns true if type is sinc.
		[[nodiscard]] constexpr bool_t is_sinc() const noexcept { return type() == filter_type::SINC; }

		/// @brief Returns gaussian parameters (std::nullopt if the type is different).
		[[nodiscard]] constexpr std::optional<filter_gaussian_params> as_gaussian() const noexcept {
				if (!is_gaussian()) { return std::nullopt; }
			return _native->as.gaussian;
		}

		/// @brief Returns mitchell parameters (std::nullopt if the type is different).
		[[nodiscard]] constexpr std::optional<filter_mitchell_params> as_mitchell() const noexcept {
				if (!is_mitchell()) { return std::nullopt; }
			return _native->as.mitchell;
		}

		/// @brief Returns sinc parameters (std::nullopt if the type is different).
		[[nodiscard]] constexpr std::optional<filter_sinc_params> as_sinc() const noexcept {
				if (!is_sinc()) { return std::nullopt; }
			return _native->as.sinc;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum filter_type;
				switch (type()) {
				case GAUSSIAN: return _native->as.gaussian;
				case MITCHELL: return _native->as.mitchell;
				case SINC:	   return _native->as.sinc;
				case MAX_NUM:  [[fallthrough]];
				default:	   break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr auto reflect() const noexcept {
			return std::make_tuple(x_radius(), y_radius(), to_variant(), type());
		}
	};
} // namespace tpbrt
#endif // _TINYPBRT_FILTER_HPP_
