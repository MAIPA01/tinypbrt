#pragma once
#ifndef _TINYPBRT_MEDIA_HPP_
#define _TINYPBRT_MEDIA_HPP_

#include <tinypbrt/detail/cpp/fwd.hpp>

#include <tinypbrt/detail/media.h>

#include <tuple>

#include <tinypbrt/detail/cpp/color.hpp>
#include <tinypbrt/detail/cpp/common.hpp>
#include <tinypbrt/detail/cpp/scattering.hpp>
#include <tinypbrt/detail/cpp/transform.hpp>

namespace tpbrt {
	/**
	 * @defgroup media-cpp-public Media
	 * @ingroup tinypbrt-cpp-public
	 */

	/**
	 * @copybrief tpbrt_media_type_t
	 * @ingroup media-cpp-public
	 */
	enum class media_type : char_t {
		/// @copybrief TPBRT_MEDIA_TYPE_CLOUD
		CLOUD		 = TPBRT_MEDIA_TYPE_CLOUD,
		/// @copybrief TPBRT_MEDIA_TYPE_HOMOGENEOUS
		HOMOGENEOUS	 = TPBRT_MEDIA_TYPE_HOMOGENEOUS,
		/// @copybrief TPBRT_MEDIA_TYPE_NANO_VDB
		NANO_VDB	 = TPBRT_MEDIA_TYPE_NANO_VDB,
		/// @copybrief TPBRT_MEDIA_TYPE_RGB_GRID
		RGB_GRID	 = TPBRT_MEDIA_TYPE_RGB_GRID,
		/// @copybrief TPBRT_MEDIA_TYPE_UNIFORM_GRID
		UNIFORM_GRID = TPBRT_MEDIA_TYPE_UNIFORM_GRID,
		/// @copybrief TPBRT_MEDIA_TYPE_MAX_NUM
		MAX_NUM		 = TPBRT_MEDIA_TYPE_MAX_NUM,
	};

	/**
	 * @brief Converts media_type enum value to string.
	 * @param type Media type.
	 * @return String value of media type.
	 * @ingroup media-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const media_type type) noexcept {
		using enum media_type;
			switch (type) {
			case CLOUD:		   return "cloud";
			case HOMOGENEOUS:  return "homogeneous";
			case NANO_VDB:	   return "nanovdb";
			case RGB_GRID:	   return "rgbgrid";
			case UNIFORM_GRID: return "uniformgrid";
			case MAX_NUM:	   return "maxnum";
			default:		   return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_media_cloud_params_t.
	 * @ingroup media-cpp-public
	 */
	struct media_cloud_params : wrapper<media_cloud_params, tpbrt_media_cloud_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<float_t, float_t, point3, float_t, point3, float_t, spectrum, spectrum>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns density value.
		[[nodiscard]] constexpr float_t density() const noexcept { return _native.density; }

		/// @brief Returns frequency value.
		[[nodiscard]] constexpr float_t frequency() const noexcept { return _native.frequency; }

		/// @brief Returns p0 value.
		[[nodiscard]] constexpr point3 p0() const noexcept { return _native.p0; }

		/// @brief Returns g value.
		[[nodiscard]] constexpr float_t g() const noexcept { return _native.g; }

		/// @brief Returns p0 value.
		[[nodiscard]] constexpr point3 p1() const noexcept { return _native.p1; }

		/// @brief Returns wispiness value.
		[[nodiscard]] constexpr float_t wispiness() const noexcept { return _native.wispiness; }

		/// @brief Returns sigma_a value.
		[[nodiscard]] constexpr spectrum sigma_a() const noexcept { return _native.sigma_a; }

		/// @brief Returns sigma_s value.
		[[nodiscard]] constexpr spectrum sigma_s() const noexcept { return _native.sigma_s; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(density(), frequency(), p0(), g(), p1(), wispiness(), sigma_a(), sigma_s());
		}
	};

	/**
	 * @copybrief tpbrt_media_scattering_value_t
	 * @ingroup media-cpp-public
	 */
	enum class media_scattering_value : char_t {
		/// @copybrief TPBRT_MEDIA_SCATTERING_VALUE_SIGMA
		SIGMA	= TPBRT_MEDIA_SCATTERING_VALUE_SIGMA,
		/// @copybrief TPBRT_MEDIA_SCATTERING_VALUE_PRESET
		PRESET	= TPBRT_MEDIA_SCATTERING_VALUE_PRESET,
		/// @copybrief TPBRT_MEDIA_SCATTERING_VALUE_MAX_NUM
		MAX_NUM = TPBRT_MEDIA_SCATTERING_VALUE_MAX_NUM,
	};

	/**
	 * @brief Converts media_scattering_value enum value to string.
	 * @param scattering_value Media scattering value.
	 * @return String value of media scattering value.
	 * @ingroup media-cpp-public
	 */
	[[nodiscard]] constexpr std::string_view to_string(const media_scattering_value scattering_value) noexcept {
		using enum media_scattering_value;
			switch (scattering_value) {
			case SIGMA:	  return "sigma";
			case PRESET:  return "preset";
			case MAX_NUM: return "maxnum";
			default:	  return "UNKNOWN";
			}
	}

	/**
	 * @brief C++ wrapper for tpbrt_media_scattering_t.
	 * @ingroup media-cpp-public
	 */
	struct media_scattering : wrapper<media_scattering, tpbrt_media_scattering_t>,
							  with_variant<media_scattering, std::pair<spectrum, spectrum>, scattering_builtin> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<variant_type, media_scattering_value>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns type value.
		[[nodiscard]] constexpr media_scattering_value type() const noexcept {
			return static_cast<media_scattering_value>(_native.type);
		}

		/// @brief Returns if type is sigma.
		[[nodiscard]] constexpr bool_t is_sigma() const noexcept { return type() == media_scattering_value::SIGMA; }

		/// @brief Returns if type is preset.
		[[nodiscard]] constexpr bool_t is_preset() const noexcept { return type() == media_scattering_value::PRESET; }

		/// @brief Returns media scattering value as sigma.
		[[nodiscard]] constexpr std::optional<std::pair<spectrum, spectrum>> as_sigma() const noexcept {
				if (!is_sigma()) { return std::nullopt; }
			return std::pair<spectrum, spectrum> { _native.as.sigma.a, _native.as.sigma.s };
		}

		/// @brief Returns media scattering value as preset.
		[[nodiscard]] constexpr std::optional<scattering_builtin> as_preset() const noexcept {
				if (!is_preset()) { return std::nullopt; }
			return static_cast<scattering_builtin>(_native.as.preset);
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum media_scattering_value;
				switch (type()) {
				case SIGMA:	  return std::pair<spectrum, spectrum> { _native.as.sigma.a, _native.as.sigma.s };
				case PRESET:  return static_cast<scattering_builtin>(_native.as.preset);
				case MAX_NUM: [[fallthrough]];
				default:	  break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(to_variant(), type()); }
	};

	/**
	 * @brief C++ wrapper for tpbrt_media_homogeneous_params_t.
	 * @ingroup media-cpp-public
	 */
	struct media_homogeneous_params : wrapper<media_homogeneous_params, tpbrt_media_homogeneous_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<float_t, float_t, spectrum, media_scattering, float_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns g value.
		[[nodiscard]] constexpr float_t g() const noexcept { return _native.g; }

		/// @brief Returns le_scale value.
		[[nodiscard]] constexpr float_t le_scale() const noexcept { return _native.le_scale; }

		/// @brief Returns le value.
		[[nodiscard]] constexpr spectrum le() const noexcept { return _native.le; }

		/// @brief Returns scattering value.
		[[nodiscard]] constexpr media_scattering scattering() const noexcept { return _native.scattering; }

		/// @brief Returns scale value.
		[[nodiscard]] constexpr float_t scale() const noexcept { return _native.scale; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(g(), le_scale(), le(), scattering(), scale());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_media_nano_vdb_params_t.
	 * @ingroup media-cpp-public
	 */
	struct media_nano_vdb_params : wrapper<media_nano_vdb_params, tpbrt_media_nano_vdb_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<float_t, float_t, spectrum, spectrum, string, float_t, float_t, float_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns g value.
		[[nodiscard]] constexpr float_t g() const noexcept { return _native.g; }

		/// @brief Returns le_scale value.
		[[nodiscard]] constexpr float_t le_scale() const noexcept { return _native.le_scale; }

		/// @brief Returns sigma_a value.
		[[nodiscard]] constexpr spectrum sigma_a() const noexcept { return _native.sigma_a; }

		/// @brief Returns sigma_s value.
		[[nodiscard]] constexpr spectrum sigma_s() const noexcept { return _native.sigma_s; }

		/// @brief Returns file_name value.
		[[nodiscard]] constexpr string file_name() const noexcept { return _native.file_name; }

		/// @brief Returns temperature_offset value.
		[[nodiscard]] constexpr float_t temperature_offset() const noexcept { return _native.temperature_offset; }

		/// @brief Returns temperature_scale value.
		[[nodiscard]] constexpr float_t temperature_scale() const noexcept { return _native.temperature_scale; }

		/// @brief Returns scale value.
		[[nodiscard]] constexpr float_t scale() const noexcept { return _native.scale; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(g(), le_scale(), sigma_a(), sigma_s(), file_name(), temperature_offset(), temperature_scale(),
			  scale());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_media_rgb_grid_params_t.
	 * @ingroup media-cpp-public
	 */
	struct media_rgb_grid_params : wrapper<media_rgb_grid_params, tpbrt_media_rgb_grid_params_t> {
		/// @brief Type used for reflection.
		using reflect_type =
		  std::tuple<rgb_array, point3, float_t, point3, float_t, rgb_array, rgb_array, float_t, uint_t, uint_t, uint_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns le value.
		[[nodiscard]] constexpr rgb_array le() const noexcept { return _native.le; }

		/// @brief Returns p0 value.
		[[nodiscard]] constexpr point3 p0() const noexcept { return _native.p0; }

		/// @brief Returns le_scale value.
		[[nodiscard]] constexpr float_t le_scale() const noexcept { return _native.le_scale; }

		/// @brief Returns p1 value.
		[[nodiscard]] constexpr point3 p1() const noexcept { return _native.p1; }

		/// @brief Returns g value.
		[[nodiscard]] constexpr float_t g() const noexcept { return _native.g; }

		/// @brief Returns sigma_a value.
		[[nodiscard]] constexpr rgb_array sigma_a() const noexcept { return _native.sigma_a; }

		/// @brief Returns sigma_s value.
		[[nodiscard]] constexpr rgb_array sigma_s() const noexcept { return _native.sigma_s; }

		/// @brief Returns scale value.
		[[nodiscard]] constexpr float_t scale() const noexcept { return _native.scale; }

		/// @brief Returns nx value.
		[[nodiscard]] constexpr uint_t nx() const noexcept { return _native.nx; }

		/// @brief Returns ny value.
		[[nodiscard]] constexpr uint_t ny() const noexcept { return _native.ny; }

		/// @brief Returns nz value.
		[[nodiscard]] constexpr uint_t nz() const noexcept { return _native.nz; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(le(), p0(), le_scale(), p1(), g(), sigma_a(), sigma_s(), scale(), nx(), ny(), nz());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_media_uniform_grid_params_t.
	 * @ingroup media-cpp-public
	 */
	struct media_uniform_grid_params : wrapper<media_uniform_grid_params, tpbrt_media_uniform_grid_params_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<float_array, spectrum, point3, float_t, point3, uint_t, uint_t, uint_t, media_scattering,
		  float_t, float_t, float_array, float_t, float_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns density value.
		[[nodiscard]] constexpr float_array density() const noexcept { return _native.density; }

		/// @brief Returns le value.
		[[nodiscard]] constexpr spectrum le() const noexcept { return _native.le; }

		/// @brief Returns p0 value.
		[[nodiscard]] constexpr point3 p0() const noexcept { return _native.p0; }

		/// @brief Returns le_scale value.
		[[nodiscard]] constexpr float_t le_scale() const noexcept { return _native.le_scale; }

		/// @brief Returns p1 value.
		[[nodiscard]] constexpr point3 p1() const noexcept { return _native.p1; }

		/// @brief Returns nx value.
		[[nodiscard]] constexpr uint_t nx() const noexcept { return _native.nx; }

		/// @brief Returns ny value.
		[[nodiscard]] constexpr uint_t ny() const noexcept { return _native.ny; }

		/// @brief Returns nz value.
		[[nodiscard]] constexpr uint_t nz() const noexcept { return _native.nz; }

		/// @brief Returns scattering value.
		[[nodiscard]] constexpr media_scattering scattering() const noexcept { return _native.scattering; }

		/// @brief Returns scale value.
		[[nodiscard]] constexpr float_t scale() const noexcept { return _native.scale; }

		/// @brief Returns g value.
		[[nodiscard]] constexpr float_t g() const noexcept { return _native.g; }

		/// @brief Returns temperature value.
		[[nodiscard]] constexpr float_array temperature() const noexcept { return _native.temperature; }

		/// @brief Returns temperature_offset value.
		[[nodiscard]] constexpr float_t temperature_offset() const noexcept { return _native.temperature_offset; }

		/// @brief Returns temperature_scale value.
		[[nodiscard]] constexpr float_t temperature_scale() const noexcept { return _native.temperature_scale; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(density(), le(), p0(), le_scale(), p1(), nx(), ny(), nz(), scattering(), scale(), g(),
			  temperature(), temperature_offset(), temperature_scale());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_media_t.
	 * @ingroup media-cpp-public
	 */
	struct media : wrapper<media, tpbrt_media_t>,
				   with_variant<media, media_cloud_params, media_homogeneous_params, media_uniform_grid_params,
					 media_rgb_grid_params, media_nano_vdb_params> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<media_idx_t, transform_handle, string, variant_type, media_type>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns idx value.
		[[nodiscard]] constexpr media_idx_t idx() const noexcept { return _native.idx; }

		/// @brief Returns transform value.
		[[nodiscard]] constexpr transform_handle transform() const noexcept { return _native.transform; }

		/// @brief Returns name value.
		[[nodiscard]] constexpr string name() const noexcept { return _native.name; }

		/// @brief Returns type value.
		[[nodiscard]] constexpr media_type type() const noexcept { return static_cast<media_type>(_native.type); }

		/// @brief Returns if type is cloud.
		[[nodiscard]] constexpr bool_t is_cloud() const noexcept { return type() == media_type::CLOUD; }

		/// @brief Returns if type is homogeneous.
		[[nodiscard]] constexpr bool_t is_homogeneous() const noexcept { return type() == media_type::HOMOGENEOUS; }

		/// @brief Returns if type is uniform_grid.
		[[nodiscard]] constexpr bool_t is_uniform_grid() const noexcept { return type() == media_type::UNIFORM_GRID; }

		/// @brief Returns if type is rgb_grid.
		[[nodiscard]] constexpr bool_t is_rgb_grid() const noexcept { return type() == media_type::RGB_GRID; }

		/// @brief Returns if type is nano_vdb.
		[[nodiscard]] constexpr bool_t is_nano_vdb() const noexcept { return type() == media_type::NANO_VDB; }

		/// @brief Returns media parameters as cloud.
		[[nodiscard]] constexpr std::optional<media_cloud_params> as_cloud() const noexcept {
				if (!is_cloud()) { return std::nullopt; }
			return _native.as.cloud;
		}

		/// @brief Returns media parameters as homogeneous.
		[[nodiscard]] constexpr std::optional<media_homogeneous_params> as_homogeneous() const noexcept {
				if (!is_homogeneous()) { return std::nullopt; }
			return _native.as.homogeneous;
		}

		/// @brief Returns media parameters as uniform_grid.
		[[nodiscard]] constexpr std::optional<media_uniform_grid_params> as_uniform_grid() const noexcept {
				if (!is_uniform_grid()) { return std::nullopt; }
			return _native.as.uniform_grid;
		}

		/// @brief Returns media parameters as rgb_grid.
		[[nodiscard]] constexpr std::optional<media_rgb_grid_params> as_rgb_grid() const noexcept {
				if (!is_rgb_grid()) { return std::nullopt; }
			return _native.as.rgb_grid;
		}

		/// @brief Returns media parameters as nano_vdb.
		[[nodiscard]] constexpr std::optional<media_nano_vdb_params> as_nano_vdb() const noexcept {
				if (!is_nano_vdb()) { return std::nullopt; }
			return _native.as.nano_vdb;
		}

		/// @brief Conversion to variant.
		[[nodiscard]] constexpr variant_type to_variant() const noexcept {
			using enum media_type;
				switch (type()) {
				case CLOUD:		   return _native.as.cloud;
				case HOMOGENEOUS:  return _native.as.homogeneous;
				case UNIFORM_GRID: return _native.as.uniform_grid;
				case RGB_GRID:	   return _native.as.rgb_grid;
				case NANO_VDB:	   return _native.as.nano_vdb;
				case MAX_NUM:	   [[fallthrough]];
				default:		   break;
				}
			return std::nullopt;
		}

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept {
			return std::make_tuple(idx(), transform(), name(), to_variant(), type());
		}
	};

	/**
	 * @brief C++ wrapper for tpbrt_media_handle_t.
	 * @ingroup media-cpp-public
	 */
	struct media_handle : wrapper<media_handle, tpbrt_media_handle_t> {
		/// @brief Type used for reflection.
		using reflect_type = std::tuple<media_idx_t>;

		/// @brief Default wrapper constructor.
		using wrapper::wrapper;

		/// @brief Returns media_idx value.
		[[nodiscard]] constexpr media_idx_t media_idx() const noexcept { return _native.media_idx; }

		/// @brief Conversion to tuple.
		[[nodiscard]] constexpr reflect_type reflect() const noexcept { return std::make_tuple(media_idx()); }
	};

	/**
	 * @brief Returns media to which handle is provided.
	 * @param medias_list List of medias in which media given by handle should be.
	 * @param handle Handle to media.
	 * @return Returns result with error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE) or with media.
	 * @ingroup media-cpp-public
	 */
	[[nodiscard]] constexpr result<media> get_media(medias_list medias_list, media_handle handle) noexcept;

	/**
	 * @brief Returns media with given name.
	 * @param medias_list List of medias in which searched media should be.
	 * @param name Name of the searched object.
	 * @return Returns result with error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE) or with media.
	 * @ingroup media-cpp-public
	 */
	[[nodiscard]] constexpr result<media> get_media(medias_list medias_list, string name) noexcept;

	/**
	 * @brief C++ wrapper for tpbrt_medias_list_t.
	 * @ingroup media-cpp-public
	 */
	struct medias_list : array<tpbrt_medias_list_t, media, &tpbrt_medias_list_t::medias> {
		/// @brief Base list constructor.
		using array::array;

		/// @brief Returns element pointed by given handle.
		[[nodiscard]] constexpr result<element_type> at(const media_handle handle) const noexcept {
			return get_media(*this, handle);
		}

		/// @brief Returns element pointed by given name.
		[[nodiscard]] constexpr result<element_type> at(const string name) const noexcept { return get_media(*this, name); }

		/// @brief Returns element pointed by given handle.
		[[nodiscard]] constexpr result<element_type> operator[](const media_handle handle) const noexcept { return at(handle); }

		/// @brief Returns element pointed by given name.
		[[nodiscard]] constexpr result<element_type> operator[](const string name) const noexcept { return at(name); }
	};

	constexpr result<media> get_media(const medias_list medias_list, const media_handle handle) noexcept {
		const tpbrt_media_t* c_media = nullptr;

		const auto err				 = static_cast<error>(tpbrt_get_media_by_handle(medias_list, handle, &c_media));

			if (err == error::NONE && c_media != nullptr) { return media(*c_media); }

		return err;
	}

	constexpr result<media> get_media(const medias_list medias_list, const string name) noexcept {
		const tpbrt_media_t* c_media = nullptr;

		const auto err				 = static_cast<error>(tpbrt_get_media_by_name(medias_list, name, &c_media));

			if (err == error::NONE && c_media != nullptr) { return media(*c_media); }

		return err;
	}
} // namespace tpbrt
#endif // _TINYPBRT_MEDIA_HPP_
