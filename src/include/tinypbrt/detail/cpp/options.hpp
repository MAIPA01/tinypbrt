#pragma once
#ifndef _TINYPBRT_OPTIONS_HPP_
#define _TINYPBRT_OPTIONS_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/options.h>

#include <tinypbrt/detail/cpp/common.hpp>
#include <tinypbrt/detail/cpp/coord.hpp>

namespace tpbrt {
	/**
	 * @defgroup options-cpp-public Options
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @brief C++ wrapper for tpbrt_options_t.
	 * @ingroup options-cpp-public
	 */
	struct options : wrapper<options, tpbrt_options_t> {
		/// @brief Type used for reflection.
		using reflect_type =
		  std::tuple<string, string, int_t, float_t, coord_system_type, bool_t, bool_t, bool_t, bool_t, bool_t, bool_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns mse_reference_image value.
		[[nodiscard]] constexpr string mse_reference_image() const noexcept { return _native.mse_reference_image; }

		/// @brief Returns mse_reference_out value.
		[[nodiscard]] constexpr string mse_reference_out() const noexcept { return _native.mse_reference_out; }

		/// @brief Returns seed value.
		[[nodiscard]] constexpr int_t seed() const noexcept { return _native.seed; }

		/// @brief Returns displacement_edge_scale value.
		[[nodiscard]] constexpr float_t displacement_edge_scale() const noexcept { return _native.displacement_edge_scale; }

		/// @brief Returns render_coord_sys value.
		[[nodiscard]] constexpr coord_system_type render_coord_sys() const noexcept {
			return static_cast<coord_system_type>(_native.render_coord_sys);
		}

		/// @brief Returns disable_pixel_jitter value.
		[[nodiscard]] constexpr bool_t disable_pixel_jitter() const noexcept { return _native.disable_pixel_jitter; }

		/// @brief Returns disable_texture_filtering value.
		[[nodiscard]] constexpr bool_t disable_texture_filtering() const noexcept { return _native.disable_texture_filtering; }

		/// @brief Returns disable_wave_length_jitter value.
		[[nodiscard]] constexpr bool_t disable_wave_length_jitter() const noexcept { return _native.disable_wave_length_jitter; }

		/// @brief Returns force_diffuse value.
		[[nodiscard]] constexpr bool_t force_diffuse() const noexcept { return _native.force_diffuse; }

		/// @brief Returns pixel_stats value.
		[[nodiscard]] constexpr bool_t pixel_stats() const noexcept { return _native.pixel_stats; }

		/// @brief Returns wavefront value.
		[[nodiscard]] constexpr bool_t wavefront() const noexcept { return _native.wavefront; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(mse_reference_image(), mse_reference_out(), seed(), displacement_edge_scale(),
			  render_coord_sys(), disable_pixel_jitter(), disable_texture_filtering(), disable_wave_length_jitter(),
			  force_diffuse(), pixel_stats(), wavefront());
		}
	};
} // namespace tpbrt
#endif // _TINYPBRT_OPTIONS_HPP_
