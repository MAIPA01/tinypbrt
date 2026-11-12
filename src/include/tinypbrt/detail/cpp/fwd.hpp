#pragma once
#ifndef _TINYPBRT_FWD_HPP_
#define _TINYPBRT_FWD_HPP_

#include <tinypbrt/detail/fwd.h>

#include <cstdint>

namespace tpbrt {
#pragma region COMMON_DEFINES
	/**
	 * @copybrief TPBRT_FALSE
	 * @ingroup tinypbrt-cpp-public
	 */
	inline constexpr auto False = TPBRT_FALSE;
	/**
	 * @copybrief TPBRT_TRUE
	 * @ingroup tinypbrt-cpp-public
	 */
	inline constexpr auto True	= TPBRT_TRUE;

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

	namespace detail {
		/**
		 * @brief Base of every C++ wrapper: a non owning view on a C struct (stores a pointer, so it is cheap to copy).
		 * @tparam Derived Derived C++ wrapper type.
		 * @tparam Native Native C library type.
		 */
		template<class Derived, class Native>
		class wrapper;

		/**
		 * @brief Adds variant visitor and type.
		 * @tparam Derived Derived C++ wrapper.
		 * @tparam Ts Variant types (must be distinct).
		 */
		template<class Derived, class... Ts>
		struct with_variant;

		/**
		 * @brief Base to all generic C++ array traits.
		 * @tparam Native Type of C library native array type.
		 * @tparam DataField Pointer to data field of array.
		 */
		template<class Native, auto DataField = nullptr>
		struct array_traits;
	} // namespace detail

	/**
	 * @brief Generic C++ array view with type conversion.
	 * @tparam Native Type of C library native array type.
	 * @tparam Element Desired array element type (if not the same as the native element type, transformation is performed).
	 * @tparam DataField Pointer to field with data.
	 * @ingroup common-cpp-public
	 */
	template<class Native, class Element = void, auto DataField = nullptr>
	struct array;

	struct string;

	struct string_array;
#pragma endregion

#pragma region MATH
	/**
	 * @copybrief tpbrt_bool_t
	 * @ingroup math-cpp-public
	 */
	using bool_t					 = bool;
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
	inline constexpr auto InvalidIdx = TPBRT_IDX_INVALID;


	struct bool_array;

	struct int_array;

	struct uint_array;

	struct float_array;


	struct vec2;
	/**
	 * @brief C++ wrapper for tpbrt_point2_t.
	 * @ingroup math-cpp-public
	 */
	using point2 = vec2;

	struct vec3;
	/**
	 * @brief C++ wrapper for tpbrt_vec_t.
	 * @ingroup math-cpp-public
	 */
	using vec	  = vec3;
	/**
	 * @brief C++ wrapper for tpbrt_point3_t.
	 * @ingroup math-cpp-public
	 */
	using point3  = vec3;
	/**
	 * @brief C++ wrapper for tpbrt_point_t.
	 * @ingroup math-cpp-public
	 */
	using point	  = point3;
	/**
	 * @brief C++ wrapper for tpbrt_normal3_t.
	 * @ingroup math-cpp-public
	 */
	using normal3 = vec3;
	/**
	 * @brief C++ wrapper for tpbrt_normal_t.
	 * @ingroup math-cpp-public
	 */
	using normal  = normal3;


	struct vec2_array;

	/**
	 * @brief C++ wrapper for tpbrt_point2_array_t.
	 * @ingroup math-cpp-public
	 */
	using point2_array = vec2_array;


	struct vec3_array;

	/**
	 * @brief C++ wrapper for tpbrt_vec_array_t.
	 * @ingroup math-cpp-public
	 */
	using vec_array		= vec3_array;
	/**
	 * @brief C++ wrapper for tpbrt_point3_array_t.
	 * @ingroup math-cpp-public
	 */
	using point3_array	= vec3_array;
	/**
	 * @brief C++ wrapper for tpbrt_point_array_t.
	 * @ingroup math-cpp-public
	 */
	using point_array	= point3_array;
	/**
	 * @brief C++ wrapper for tpbrt_normal3_array_t.
	 * @ingroup math-cpp-public
	 */
	using normal3_array = vec3_array;
	/**
	 * @brief C++ wrapper for tpbrt_normal_array_t.
	 * @ingroup math-cpp-public
	 */
	using normal_array	= normal3_array;


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
	inline constexpr auto TransformInvalidIdx = TPBRT_TRANSFORM_IDX_INVALID;

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
	inline constexpr auto TextureInvalidIdx = TPBRT_TEXTURE_IDX_INVALID;

	struct texture_handle;

	struct texture;

	struct textures_list;
#pragma endregion

#pragma region SHAPE
	struct shape;

	struct shapes_list;
#pragma endregion

#pragma region SCENE
	struct scene_view;

	class scene;
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
	inline constexpr auto ObjectInvalidIdx = TPBRT_OBJECT_IDX_INVALID;

	struct object_handle;

	struct object;

	struct objects_list;
#pragma endregion

#pragma region OBJECT_INSTANCE
	struct object_instance;

	struct objects_instances_list;
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
	inline constexpr auto MediaInvalidIdx = TPBRT_MEDIA_IDX_INVALID;

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
	inline constexpr auto MaterialInvalidIdx = TPBRT_MATERIAL_IDX_INVALID;

	struct material_handle;

	struct material;

	struct materials_list;
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
	inline constexpr auto AreaLightInvalidIdx = TPBRT_AREA_LIGHT_IDX_INVALID;


	struct area_light_handle;

	struct area_light;

	struct area_lights_list;

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

	struct blackbody;

	/**
	 * @brief C++ wrapper for tpbrt_rgb_t.
	 * @ingroup color-cpp-public
	 */
	using rgb = vec3;

	struct spectrum;

	/**
	 * @brief C++ wrapper for tpbrt_wavelength_array_t.
	 * @ingroup color-cpp-public
	 */
	using wavelength_array = array<tpbrt_wavelength_array_t, wavelength>;

	/**
	 * @brief C++ wrapper for tpbrt_rgb_array_t.
	 * @ingroup color-cpp-public
	 */
	using rgb_array		   = array<tpbrt_rgb_array_t, rgb>;
#pragma endregion

#pragma region CAMERA
	struct camera;
#pragma endregion

#pragma region ACCELERATOR
	struct accelerator;
#pragma endregion
} // namespace tpbrt
#endif // _TINYPBRT_FWD_HPP_
