#pragma once
#ifndef _TINYPBRT_FWD_HPP_
#define _TINYPBRT_FWD_HPP_

#include <tinypbrt/detail/export.h>
#include <tinypbrt/detail/fwd.h>

#include <cstddef>
#include <cstdint>
#include <optional>
#include <variant>

namespace tpbrt {
#pragma region COMMON_DEFINES
	/**
	 * @copybrief TPBRT_FALSE
	 * @ingroup tinypbrt-cpp-public
	 */
	static constexpr auto False = TPBRT_FALSE;
	/**
	 * @copybrief TPBRT_TRUE
	 * @ingroup tinypbrt-cpp-public
	 */
	static constexpr auto True	= TPBRT_TRUE;

	namespace detail {
		template<class T, class... Us>
		constexpr bool is_type_in_v = (std::is_same_v<std::decay_t<T>, std::decay_t<Us>> || ...); // NOLINT

		template<class TupleIn, class TupleOut>
		struct _unique_types_impl;

		template<class... Us>
		struct _unique_types_impl<std::tuple<>, std::tuple<Us...>> {
			using type = std::tuple<Us...>;
		};

		template<class T, class... Ts, class... Us>
		struct _unique_types_impl<std::tuple<T, Ts...>, std::tuple<Us...>>
			: _unique_types_impl<std::tuple<Ts...>,
				std::conditional_t<is_type_in_v<T, Us...>, std::tuple<Us...>, std::tuple<Us..., std::decay_t<T>>>> {};

		template<class T, class... Ts>
		struct _unique_types_impl<std::tuple<T, Ts...>, std::tuple<>>
			: _unique_types_impl<std::tuple<Ts...>, std::tuple<std::decay_t<T>>> {};

		template<class... Ts>
		using unique_types_t = _unique_types_impl<std::tuple<Ts...>, std::tuple<>>::type;

		template<class T>
		struct _optional_variant;

		template<class... Ts>
		struct _optional_variant<std::tuple<Ts...>> {
			using type = std::optional<std::variant<Ts...>>;
		};

		/**
		 * @brief Optional variant.
		 * @tparam Ts Variant types.
		 * @ingroup tinypbrt-cpp-public
		 */
		template<class... Ts>
		using optional_variant = _optional_variant<unique_types_t<Ts...>>::type;
	} // namespace detail

#pragma endregion

#pragma region COMMON
	/**
	 * @copybrief tpbrt_char_t
	 * @ingroup common-cpp-public
	 */
	using char_t  = tpbrt_char_t;
	/**
	 * @copybrief tpbrt_uchar_t
	 * @ingroup common-cpp-public
	 */
	using uchar_t = tpbrt_uchar_t;
#pragma endregion

#pragma region MATH
	/**
	 * @copybrief tpbrt_bool_t
	 * @ingroup math-cpp-public
	 */
	using bool_t					 = tpbrt_bool_t;
	/**
	 * @copybrief tpbrt_int_t
	 * @ingroup math-cpp-public
	 */
	using int_t						 = tpbrt_int_t;
	/**
	 * @copybrief tpbrt_uint_t
	 * @ingroup math-cpp-public
	 */
	using uint_t					 = tpbrt_uint_t;
	/**
	 * @copybrief tpbrt_float_t
	 * @ingroup math-cpp-public
	 */
	using float_t					 = tpbrt_float_t;
	/**
	 * @copybrief tpbrt_double_t
	 * @ingroup math-cpp-public
	 */
	using double_t					 = tpbrt_double_t;
	/**
	 * @copybrief tpbrt_size_t
	 * @ingroup math-cpp-public
	 */
	using size_t					 = tpbrt_size_t;
	/**
	 * @copybrief tpbrt_long_t
	 * @ingroup math-cpp-public
	 */
	using long_t					 = tpbrt_long_t;
	/**
	 * @copybrief tpbrt_ulong_t
	 * @ingroup math-cpp-public
	 */
	using ulong_t					 = tpbrt_ulong_t;
	/**
	 * @copybrief tpbrt_longlong_t
	 * @ingroup math-cpp-public
	 */
	using longlong_t				 = tpbrt_longlong_t;
	/**
	 * @copybrief tpbrt_ulonglong_t
	 * @ingroup math-cpp-public
	 */
	using ulonglong_t				 = tpbrt_ulonglong_t;

	/**
	 * @copybrief tpbrt_idx_t
	 * @ingroup math-cpp-public
	 */
	using idx_t						 = tpbrt_idx_t;
	/**
	 * @copybrief TPBRT_IDX_INVALID
	 * @ingroup math-cpp-public
	 */
	static constexpr auto InvalidIdx = TPBRT_IDX_INVALID;


	struct vec2;
	/**
	 * @brief C++ wrapper for tpbrt_point2_t
	 * @ingroup math-cpp-public
	 */
	using point2 = vec2;


	struct vec3;
	/**
	 * @brief C++ wrapper for tpbrt_vec_t
	 * @ingroup math-cpp-public
	 */
	using vec	  = vec3;
	/**
	 * @brief C++ wrapper for tpbrt_point3_t
	 * @ingroup math-cpp-public
	 */
	using point3  = vec3;
	/**
	 * @brief C++ wrapper for tpbrt_point_t
	 * @ingroup math-cpp-public
	 */
	using point	  = point3;
	/**
	 * @brief C++ wrapper for tpbrt_normal3_t
	 * @ingroup math-cpp-public
	 */
	using normal3 = vec3;
	/**
	 * @brief C++ wrapper for tpbrt_normal_t
	 * @ingroup math-cpp-public
	 */
	using normal  = normal3;


	struct vec4;

	struct mat4;

	struct mat4_animated;
#pragma endregion

#pragma region TRANSFORM
	/**
	 * @copybrief tpbrt_transform_idx_t.
	 * @ingroup transform-cpp-public
	 */
	using transform_idx_t					  = tpbrt_transform_idx_t;
	/**
	 * @copybrief TPBRT_TRANSFORM_IDX_INVALID.
	 * @ingroup transform-cpp-public
	 */
	static constexpr auto TransformInvalidIdx = TPBRT_TRANSFORM_IDX_INVALID;

	struct transforms_list;

	struct transform_handle;

	struct transform_animated;
#pragma endregion

#pragma region TEXTURE
	/**
	 * @copybrief tpbrt_texture_idx_t
	 * @ingroup texture-cpp-public
	 */
	using texture_idx_t						= tpbrt_texture_idx_t;
	/**
	 * @copybrief TPBRT_TEXTURE_IDX_INVALID
	 * @ingroup texture-cpp-public
	 */
	static constexpr auto TextureInvalidIdx = TPBRT_TEXTURE_IDX_INVALID;

	struct texture_handle;

	struct texture;
#pragma endregion

#pragma region SHAPE
	struct shape;
#pragma endregion

#pragma region SCENE
	struct scene;
#pragma endregion

#pragma region SAMPLER
	struct sampler;
#pragma endregion

#pragma region OPTIONS
	struct options;
#pragma endregion

#pragma region OBJECT
	/**
	 * @copybrief tpbrt_object_idx_t
	 * @ingroup object-cpp-public
	 */
	using object_idx_t					   = tpbrt_object_idx_t;
	/**
	 * @copybrief TPBRT_OBJECT_IDX_INVALID
	 * @ingroup object-cpp-public
	 */
	static constexpr auto ObjectInvalidIdx = TPBRT_OBJECT_IDX_INVALID;

	struct object_handle;

	struct object;
#pragma endregion

#pragma region OBJECT_INSTANCE
	struct object_instance;
#pragma endregion

#pragma region MEDIA
	/**
	 * @copybrief tpbrt_media_idx_t
	 * @ingroup media-cpp-public
	 */
	using media_idx_t					  = tpbrt_media_idx_t;
	/**
	 * @copybrief TPBRT_MEDIA_IDX_INVALID
	 * @ingroup media-cpp-public
	 */
	static constexpr auto MediaInvalidIdx = TPBRT_MEDIA_IDX_INVALID;

	struct medias_list;

	struct media_handle;

	struct media;
#pragma endregion

#pragma region MATERIAL
	/**
	 * @copybrief tpbrt_material_idx_t
	 * @ingroup material-cpp-public
	 */
	using material_idx_t					 = tpbrt_material_idx_t;
	/**
	 * @copybrief TPBRT_MATERIAL_IDX_INVALID
	 * @ingroup material-cpp-public
	 */
	static constexpr auto MaterialInvalidIdx = TPBRT_MATERIAL_IDX_INVALID;

	struct material_handle;

	struct material;
#pragma endregion

#pragma region LIGHT
	struct light_source;

	/**
	 * @copybrief tpbrt_area_light_idx_t
	 * @ingroup light-cpp-public
	 */
	using area_light_idx_t					  = tpbrt_area_light_idx_t;
	/**
	 * @copybrief TPBRT_AREA_LIGHT_IDX_INVALID
	 * @ingroup light-cpp-public
	 */
	static constexpr auto AreaLightInvalidIdx = TPBRT_AREA_LIGHT_IDX_INVALID;


	struct area_light_handle;

	struct area_light;

	struct lights_list;
#pragma endregion

#pragma region INTEGRATOR
	struct integrator;
#pragma endregion

#pragma region FILTER
	struct filter;
#pragma endregion

#pragma region FILM
	struct film;
#pragma endregion

#pragma region COLOR
	struct wavelength;

	/**
	 * @copybrief tpbrt_blackbody_t
	 * @ingroup color-cpp-public
	 */
	using blackbody = tpbrt_blackbody_t;

	/**
	 * @brief C++ wrapper for tpbrt_rgb_t
	 * @ingroup color-cpp-public
	 */
	using rgb		= vec3;

	struct spectrum;
#pragma endregion

#pragma region CAMERA
	struct camera;
#pragma endregion

#pragma region ACCELERATOR
	struct accelerator;
#pragma endregion
} // namespace tpbrt
#endif // _TINYPBRT_FWD_HPP_
