#pragma once
#ifndef _TINYPBRT_TEXTURE_H_
#define _TINYPBRT_TEXTURE_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/color.h>
#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/math.h>
#include <tinypbrt/detail/transform.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup texture-public Texture
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Texture handle value types.
	 * @ingroup texture-public
	 */
	typedef enum tpbrt_texture_handle_value_type_t {
		/// @brief Texture index.
		TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE_IDX = 0,
		/// @brief Spectrum.
		TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM	= 1,
		/// @brief Count of available texture handle value types.
		TPBRT_TEXTURE_HANDLE_VALUE_TYPE_MAX_NUM		= 2,
	} tpbrt_texture_handle_value_type_t;

	/**
	 * @brief Texture handle.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_handle_t {
		/// @brief Value dependent on value_type value.
		union {
			/// @brief Texture index.
			tpbrt_texture_idx_t texture_idx;
			/// @brief Spectrum.
			tpbrt_spectrum_t spectrum;
		} as;

		/// @brief Texture handle value type.
		tpbrt_texture_handle_value_type_t value_type : 3;
	} tpbrt_texture_handle_t;

/**
 * @brief Invalid texture handle initialization.
 * @ingroup texture-public
 */
#define TPBRT_TEXTURE_HANDLE_INVALID_INIT                                                                      \
	{ .as.texture_idx = TPBRT_TEXTURE_IDX_INVALID, .value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE_IDX }
/**
 * @brief Invalid texture handle initialization with type conversion.
 * @ingroup texture-public
 */
#define TPBRT_TEXTURE_HANDLE_INVALID (tpbrt_texture_handle_t) TPBRT_TEXTURE_HANDLE_INVALID_INIT

	/**
	 * @brief Textures types.
	 * @ingroup texture-public
	 */
	typedef enum tpbrt_texture_type_t {
		/// @brief Float texture.
		TPBRT_TEXTURE_TYPE_FLOAT	= 0,
		/// @brief Spectrum texture.
		TPBRT_TEXTURE_TYPE_SPECTRUM = 1,
		/// @brief Count of available texture types.
		TPBRT_TEXTURE_TYPE_MAX_NUM	= 2,
	} tpbrt_texture_type_t;

	/**
	 * @brief Texture classes.
	 * @ingroup texture-public
	 */
	typedef enum tpbrt_texture_class_t {
		/// @brief Bilerp texture.
		TPBRT_TEXTURE_CLASS_BILERP		  = 0,
		/// @brief Checker board texture.
		TPBRT_TEXTURE_CLASS_CHECKER_BOARD = 1,
		/// @brief Constant texture.
		TPBRT_TEXTURE_CLASS_CONSTANT	  = 2,
		/// @brief Direction mix texture.
		TPBRT_TEXTURE_CLASS_DIRECTION_MIX = 3,
		/// @brief Dots texture.
		TPBRT_TEXTURE_CLASS_DOTS		  = 4,
		/// @brief FBM texture.
		TPBRT_TEXTURE_CLASS_FBM			  = 5,
		/// @brief Image map texture.
		TPBRT_TEXTURE_CLASS_IMAGE_MAP	  = 6,
		/// @brief Marble texture.
		TPBRT_TEXTURE_CLASS_MARBLE		  = 7,
		/// @brief Mix texture.
		TPBRT_TEXTURE_CLASS_MIX			  = 8,
		/// @brief PTEX texture.
		TPBRT_TEXTURE_CLASS_PTEX		  = 9,
		/// @brief Scale texture.
		TPBRT_TEXTURE_CLASS_SCALE		  = 10,
		/// @brief Windy texture.
		TPBRT_TEXTURE_CLASS_WINDY		  = 11,
		/// @brief Wrinkled texture.
		TPBRT_TEXTURE_CLASS_WRINKLED	  = 12,
		/// @brief Count of available texture classes.
		TPBRT_TEXTURE_CLASS_MAX_NUM		  = 13,
	} tpbrt_texture_class_t;

	/**
	 * @brief Bilerp texture parameters.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_bilerp_params_t {
		/// @brief First of the four values to be bilinearly interpolated between.
		tpbrt_texture_handle_t v00;
		/// @brief Second of the four values to be bilinearly interpolated between.
		tpbrt_texture_handle_t v01;
		/// @brief Third of the four values to be bilinearly interpolated between.
		tpbrt_texture_handle_t v10;
		/// @brief Fourth of the four values to be bilinearly interpolated between.
		tpbrt_texture_handle_t v11;
	} tpbrt_texture_bilerp_params_t;

	/**
	 * @brief Checkerboard texture parameters.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_checkerboard_params_t {
		/// @brief The texture to use for even checks.
		tpbrt_texture_handle_t tex1;
		/// @brief The texture to use for odd checks.
		tpbrt_texture_handle_t tex2;
		/// @brief Sets the dimension of the checkerboard texture. Legal values are 2 and 3.
		tpbrt_uint_t dimension;
	} tpbrt_texture_checkerboard_params_t;

	/**
	 * @brief Constant texture parameters.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_constant_params_t {
		/// @brief The constant value of this texture.
		tpbrt_texture_handle_t value;
	} tpbrt_texture_constant_params_t;

	/**
	 * @brief Direction mix texture parameters.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_direction_mix_params_t {
		/// @brief One of the two textures to be mixed.
		tpbrt_texture_handle_t tex1;
		/// @brief The other texture to be mixed. These two textures must be of the same type (float or spectrum).
		tpbrt_texture_handle_t tex2;
		/// @brief Vector with with the dot product of the surface normal is computed.
		tpbrt_vec_t dir;
	} tpbrt_texture_direction_mix_params_t;

	/**
	 * @brief Dots texture parameters.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_dots_params_t {
		/// @brief One of the textures to use for coloring the dots and the background.
		tpbrt_texture_handle_t inside;
		/// @brief One of the textures to use for coloring the dots and the background.
		tpbrt_texture_handle_t outside;
	} tpbrt_texture_dots_params_t;

	/**
	 * @brief FBM texture parameters.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_fbm_params_t {
		/// @brief The maximum number of octaves of noise to use in spectral synthesis.
		tpbrt_uint_t octaves;
		/// @brief The "bumpiness" of the resulting texture.
		tpbrt_float_t roughness;
		/// @brief CTM from when the texture was defined.
		tpbrt_transform_handle_t transform;
	} tpbrt_texture_fbm_params_t;

	/**
	 * @brief Wrinkled texture parameters.
	 * @ingroup texture-public
	 */
	typedef tpbrt_texture_fbm_params_t tpbrt_texture_wrinkled_params_t;
	/**
	 * @brief Windy texture parameters.
	 * @ingroup texture-public
	 */
	typedef tpbrt_texture_fbm_params_t tpbrt_texture_windy_params_t;

	/**
	 * @brief Texture wrap types.
	 * @ingroup texture-public
	 */
	typedef enum tpbrt_texture_wrap_t {
		/// @brief Simply tiles the texture.
		TPBRT_TEXTURE_WRAP_REPEAT  = 0,
		/// @brief Returns black when outside the legal range.
		TPBRT_TEXTURE_WRAP_BLACK   = 1,
		/// @brief Always returns the nearest border texel.
		TPBRT_TEXTURE_WRAP_CLAMP   = 2,
		/// @brief Count of available texture wrap types.
		TPBRT_TEXTURE_WRAP_MAX_NUM = 3,
	} tpbrt_texture_wrap_t;

	/**
	 * @brief Texture filter types.
	 * @ingroup texture-public
	 */
	typedef enum tpbrt_texture_filter_t {
		/// @brief Does bilinear sampling of a single MIP level.
		TPBRT_TEXTURE_FILTER_BILINEAR  = 0,
		/// @brief Uses the high quality but much slower elliptically weighted average algorithm.
		TPBRT_TEXTURE_FILTER_EWA	   = 1,
		/// @brief Does trilinear interpolation between two MIP levels.
		TPBRT_TEXTURE_FILTER_TRILINEAR = 2,
		/// @brief Point sampling.
		TPBRT_TEXTURE_FILTER_POINT	   = 3,
		/// @brief Count of available texture filter types.
		TPBRT_TEXTURE_FILTER_MAX_NUM   = 4,
	} tpbrt_texture_filter_t;

	/**
	 * @brief Texture encoding types.
	 * @ingroup texture-public
	 */
	typedef enum tpbrt_texture_encoding_t {
		/// @brief Applies the sRGB nonlinear mapping.
		TPBRT_TEXTURE_ENCODING_SRGB	   = 0,
		/// @brief Directly maps them, dividing their values by 255.
		TPBRT_TEXTURE_ENCODING_LINEAR  = 1,
		/// @brief Applies a gamma correction curve with the specified exponent.
		TPBRT_TEXTURE_ENCODING_GAMMA   = 2,
		/// @brief Count of available texture encoding types.
		TPBRT_TEXTURE_ENCODING_MAX_NUM = 3,
	} tpbrt_texture_encoding_t;

	/**
	 * @brief Image map texture parameters.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_image_map_params_t {
		/// @brief The filename of the image to load. Currently pbrt supports TGA, PFM, and EXR format images.
		tpbrt_string_t file_name;
		/// @brief The maximum elliptical eccentricity for the EWA algorithm.
		tpbrt_float_t max_anisotropy;
		/// @brief Scale factor to apply to value looked up in texture.
		tpbrt_float_t scale;
		/// @brief Gamma exponent.
		tpbrt_opt_float_t gamma;
		/// @brief What to do with texture coordinates that fall outside the legal [0,1] range.
		tpbrt_texture_wrap_t wrap		  : 3;
		/// @brief Filter function to use for anti-aliasing texture lookups.
		tpbrt_texture_filter_t filter	  : 4;
		/// @brief Specification of how to convert 8-bit color values to floating point values between 0 and 1.
		tpbrt_texture_encoding_t encoding : 3;
		/// @brief If true, then given a texture value x, returns 1-x instead.
		tpbrt_bool_t invert				  : 1;
	} tpbrt_texture_image_map_params_t;

	/**
	 * @brief Marble texture parameters.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_marble_params_t {
		/// @brief The maximum number of octaves of noise to use in spectral synthesis.
		tpbrt_uint_t octaves;
		/// @brief The "bumpiness" of the resulting texture.
		tpbrt_float_t roughness;
		/// @brief A scaling factor to apply to the noise function inputs.
		tpbrt_float_t scale;
		/// @brief A scaling factor to apply to the noise function output.
		tpbrt_float_t variation;
	} tpbrt_texture_marble_params_t;

	/**
	 * @brief Mix texture parameters.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_mix_params_t {
		/// @brief One of the two textures to be mixed.
		tpbrt_texture_handle_t tex1;
		/// @brief The other texture to be mixed. These two textures must be of the same type (float or spectrum).
		tpbrt_texture_handle_t tex2;
		/// @brief The amount to use when linearly interpolating between the two mix textures.
		tpbrt_texture_handle_t amount;
	} tpbrt_texture_mix_params_t;

	/**
	 * @brief PTEX texture parameters.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_ptex_params_t {
		/// @brief Gamma exponent.
		tpbrt_opt_float_t gamma;
		/// @brief Filename of the Ptex texture to use.
		tpbrt_string_t file_name;
		/// @brief Scale factor applied to texture values.
		tpbrt_float_t scale;
		/// @brief Specifies how to convert 8-bit color values from the texture to floating point values between 0 and 1.
		tpbrt_texture_encoding_t encoding : 3;
	} tpbrt_texture_ptex_params_t;

	/**
	 * @brief Scale texture parameters.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_scale_params_t {
		/// @brief Texture to be scaled.
		tpbrt_texture_handle_t tex;
		/// @brief Scale factor by which to multiply the values returned by "tex".
		tpbrt_texture_handle_t scale;
	} tpbrt_texture_scale_params_t;

	/**
	 * @brief Texture mapping types.
	 * @ingroup texture-public
	 */
	typedef enum tpbrt_texture_mapping_t {
		/// @brief Based on the surface's (u,v) parameterization.
		TPBRT_TEXTURE_MAPPING_UV		  = 0,
		/// @brief Spherical mapping.
		TPBRT_TEXTURE_MAPPING_SPHERICAL	  = 1,
		/// @brief Cylindrical mapping.
		TPBRT_TEXTURE_MAPPING_CYLINDRICAL = 2,
		/// @brief Planar mapping.
		TPBRT_TEXTURE_MAPPING_PLANAR	  = 3,
		/// @brief Count of available texture mapping types.
		TPBRT_TEXTURE_MAPPING_MAX_NUM	  = 4,
	} tpbrt_texture_mapping_t;

	/**
	 * @brief UV texture mapping parameters.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_mapping_uv_params_t {
		/// @brief Scaling factor to be applied to the u texture coordinates.
		tpbrt_float_t u_scale;
		/// @brief Scaling factor to be applied to the v texture coordinates.
		tpbrt_float_t v_scale;
		/// @brief An offset to be applied to the u texture coordinates. This offset is applied after any scaling from "uscale".
		tpbrt_float_t u_delta;
		/// @brief An offset to be applied to the v texture coordinates. This offset is applied after any scaling from "vscale".
		tpbrt_float_t v_delta;
	} tpbrt_texture_mapping_uv_params_t;

	/**
	 * @brief Spherical texture mapping parameters.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_mapping_spherical_params_t {
		/// @brief The current transformation matrix when the associated texture is specified gives the rendering space from texture space transformation.
		tpbrt_transform_handle_t transform;
	} tpbrt_texture_mapping_spherical_params_t;

	/**
	 * @brief Cylindrical texture mapping parameters.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_mapping_cylindrical_params_t {
		/// @brief The current transformation matrix when the associated texture is specified gives the rendering space from texture space transformation.
		tpbrt_transform_handle_t transform;
	} tpbrt_texture_mapping_cylindrical_params_t;

	/**
	 * @brief Planar texture mapping parameters.
	 * @note Note that "uscale" and "vscale" are redundant for the "planar" mapping and are therefore not provided since their effect can be had by scaling the "v1" and "v2" vectors.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_mapping_planar_params_t {
		/// @brief An offset to be applied to the u texture coordinates. This offset is applied after any scaling from "uscale".
		tpbrt_float_t u_delta;
		/// @brief An offset to be applied to the v texture coordinates. This offset is applied after any scaling from "vscale".
		tpbrt_float_t v_delta;
		/// @brief v1 is one of two vectors that define a planar mapping.
		tpbrt_vec_t v1;
		/// @brief v2 is one of two vectors that define a planar mapping.
		tpbrt_vec_t v2;
	} tpbrt_texture_mapping_planar_params_t;

	/**
	 * @brief PBRT texture representation.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_texture_t {
		/// @brief Texture index.
		tpbrt_texture_idx_t idx;
		/// @brief Texture type.
		tpbrt_texture_type_t type		: 3;
		/// @brief Texture class.
		tpbrt_texture_class_t tex_class : 5;
		/// @brief Texture mapping.
		tpbrt_texture_mapping_t mapping : 4;
		/// @brief Texture name.
		tpbrt_string_t name;

		/// @brief Value dependent on mapping value.
		union {
			/// @brief UV texture mapping parameters.
			tpbrt_texture_mapping_uv_params_t uv;
			/// @brief Spherical texture mapping parameters.
			tpbrt_texture_mapping_spherical_params_t spherical;
			/// @brief Cylindrical texture mapping parameters.
			tpbrt_texture_mapping_cylindrical_params_t cylindrical;
			/// @brief Planar texture mapping parameters.
			tpbrt_texture_mapping_planar_params_t planar;
		} mapping_as;

		/// @brief Value dependent on tex_class value.
		union {
			/// @brief Bilerp texture parameters.
			tpbrt_texture_bilerp_params_t bilerp;
			/// @brief Checkerboard texture parameters.
			tpbrt_texture_checkerboard_params_t checkerboard;
			/// @brief Constant texture parameters.
			tpbrt_texture_constant_params_t constant;
			/// @brief Direction mix texture parameters.
			tpbrt_texture_direction_mix_params_t direction_mix;
			/// @brief Dots texture parameters.
			tpbrt_texture_dots_params_t dots;
			/// @brief FBM texture parameters.
			tpbrt_texture_fbm_params_t fbm;
			/// @brief Wrinkled texture parameters.
			tpbrt_texture_wrinkled_params_t wrinkled;
			/// @brief Windy texture parameters.
			tpbrt_texture_windy_params_t windy;
			/// @brief Image map texture parameters.
			tpbrt_texture_image_map_params_t image_map;
			/// @brief Marble texture parameters.
			tpbrt_texture_marble_params_t marble;
			/// @brief Mix texture parameters.
			tpbrt_texture_mix_params_t mix;
			/// @brief PTEX texture parameters.
			tpbrt_texture_ptex_params_t ptex;
			/// @brief Scale texture parameters.
			tpbrt_texture_scale_params_t scale;
		} as;
	} tpbrt_texture_t;

	/**
	 * @brief List of textures.
	 * @ingroup texture-public
	 */
	typedef struct tpbrt_textures_list_t {
		/// @brief Textures list data.
		tpbrt_texture_t* textures;
		/// @brief Number of textures in list.
		tpbrt_size_t count;
		/// @brief Names indexes.
		tpbrt_name_index_t names;
	} tpbrt_textures_list_t;

	/**
	 * @brief Returns pointer to texture with given name.
	 * @param textures List of textures in which searched texture should be.
	 * @param name Name of the searched texture.
	 * @param texture Pointer in which pointer to texture will be stored (if not found this will be TPBRT_NULL).
	 * @return Returns error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup texture-public
	 */
	TPBRT_API tpbrt_error_t tpbrt_get_texture_by_name(const tpbrt_textures_list_t* textures, tpbrt_string_t name,
	  const tpbrt_texture_t** texture);
	/**
	 * @brief Returns pointer to texture to which handle is provided.
	 * @param textures List of textures in which texture given by handle should be.
	 * @param handle Handle to texture.
	 * @param texture Pointer in which pointer to texture will be stored (if not found this will be TPBRT_NULL).
	 * @return Returns error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup texture-public
	 */
	TPBRT_API tpbrt_error_t tpbrt_get_texture_by_handle(const tpbrt_textures_list_t* textures,
	  const tpbrt_texture_handle_t* handle, const tpbrt_texture_t** texture);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_TEXTURE_H_
