#pragma once
#ifndef _TINYPBRT_LIGHT_H_
#define _TINYPBRT_LIGHT_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/color.h>
#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/math.h>
#include <tinypbrt/detail/media.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup light-public Light
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Light source type.
	 * @ingroup light-public
	 */
	typedef enum tpbrt_light_source_type_t {
		/// @brief Distant light source. (Directional)
		TPBRT_LIGHT_SOURCE_TYPE_DISTANT		= 0,
		/// @brief Goniometric light source.
		TPBRT_LIGHT_SOURCE_TYPE_GONIOMETRIC = 1,
		/// @brief Infinite light source.
		TPBRT_LIGHT_SOURCE_TYPE_INFINITE	= 2,
		/// @brief Point light source.
		TPBRT_LIGHT_SOURCE_TYPE_POINT		= 3,
		/// @brief Projection light source.
		TPBRT_LIGHT_SOURCE_TYPE_PROJECTION	= 4,
		/// @brief Spot light source.
		TPBRT_LIGHT_SOURCE_TYPE_SPOT		= 5,
		/// @brief Count of available light source types.
		TPBRT_LIGHT_SOURCE_TYPE_MAX_NUM		= 6,
	} tpbrt_light_source_type_t;

	/**
	 * @brief Distant light source parameters.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_light_source_distant_params_t {
		/// @brief Spectral distribution of the radiance emitted by the light source.
		tpbrt_spectrum_t l;
		/// @brief "from" and "to" define the direction vector along which illumination from the light arrives at the scene. The defaults give a light that shines along the z axis.
		tpbrt_point_t from;
		/// @brief "from" and "to" define the direction vector along which illumination from the light arrives at the scene. The defaults give a light that shines along the z axis.
		tpbrt_point_t to;
	} tpbrt_light_source_distant_params_t;

	/**
	 * @brief Goniometric light source parameters.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_light_source_goniometric_params_t {
		/// @brief The filename of the image file that stores a goniometric diagram to use for the lighting distribution.
		tpbrt_string_t file_name;
		/// @brief The spectral distribution of emission from the light; the radiant intensity in a particular direction is computed as the product of this value, the "scale" parameter, the appropriate value from the goniometric diagram table, and the light's power, if specified.
		tpbrt_spectrum_t i;
	} tpbrt_light_source_goniometric_params_t;

	/**
	 * @brief Infinite light source parameters.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_light_source_infinite_params_t {
		/// @brief The environment map to use for the infinite area light. If no filename is provided, the light will emit the same amount of radiance from every direction. Environment maps should be specified using the same equal-area mapping as is used for the goniometric light; see above for information about this parameterization and how to convert environment maps into this representation.
		tpbrt_string_t file_name;
		/// @brief If provided, these points should specify a planar quadrilateral through with the light source is visible (e.g., a window, or a quad that bounds multiple windows on a wall.) This information is used to only sample regions of the light that are visible from the point being shaded and generally gives lower error when it is applicable.
		tpbrt_point3_t portal[4];
		/// @brief The spectral distribution of emission from the light; the radiance intensity in a particular direction is computed as the product of this value, the "scale" parameter, and the light's power, if specified. Note that both "L" and "filename" may not be specified.
		tpbrt_spectrum_t l;
	} tpbrt_light_source_infinite_params_t;

	/**
	 * @brief Point light source parameters.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_light_source_point_params_t {
		/// @brief Spectral distribution of the light's emitted radiant intensity.
		tpbrt_spectrum_t i;
		/// @brief The location of the light.
		tpbrt_point_t from;
	} tpbrt_light_source_point_params_t;

	/**
	 * @brief Projection light source parameters.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_light_source_projection_params_t {
		/// @brief Spectral distribution of the light's emitted radiant intensity.
		tpbrt_spectrum_t i;
		/// @brief The image to project into the scene.
		tpbrt_string_t file_name;
		/// @brief The spread angle of the projected light, along the shorter image axis.
		tpbrt_float_t fov;
	} tpbrt_light_source_projection_params_t;

	/**
	 * @brief Spot light source parameters.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_light_source_spot_params_t {
		/// @brief Spectral distribution of the light's emitted radiant intensity.
		tpbrt_spectrum_t i;
		/// @brief One of two points defining the lighting vector.
		tpbrt_point_t from;
		/// @brief One of two points defining the lighting vector.
		tpbrt_point_t to;
		/// @brief The angle that the spotlight's cone makes with its primary axis. For directions up to this angle from the main axis, the full radiant intensity given by "I" is emitted. After this angle and up to "coneangle" + "conedeltaangle", illumination falls off until it is zero.
		tpbrt_float_t cone_angle;
		/// @brief The angle at which the spotlight intensity begins to fall off at the edges.
		tpbrt_float_t cone_delta_angle;
	} tpbrt_light_source_spot_params_t;

	/**
	 * @brief PBRT light source representation.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_light_source_t {
		/// @brief Transform handle.
		tpbrt_transform_handle_t transform;
		/// @brief Scale factor that modulates the amount of light that the light source emits into the scene.
		tpbrt_float_t scale;
		/// @brief Total luminous power or illuminance emitted by the light.
		tpbrt_opt_float_t power_illuminance;
		/// @brief Interior media handle.
		tpbrt_media_handle_t interior_media_handle;
		/// @brief Exterior media handle.
		tpbrt_media_handle_t exterior_media_handle;

		/// @brief Value dependent on type value.
		union {
			/// @brief Distant light source parameters.
			tpbrt_light_source_distant_params_t distant;
			/// @brief Goniometric light source parameters.
			tpbrt_light_source_goniometric_params_t goniometric;
			/// @brief Infinite light source parameters.
			tpbrt_light_source_infinite_params_t infinite;
			/// @brief Point light source parameters.
			tpbrt_light_source_point_params_t point;
			/// @brief Projection light source parameters.
			tpbrt_light_source_projection_params_t projection;
			/// @brief Spot light source parameters.
			tpbrt_light_source_spot_params_t spot;
		} as;

		/// @brief Light value type.
		tpbrt_light_source_type_t type : 4;
	} tpbrt_light_source_t;

	/**
	 * @brief Area light type.
	 * @ingroup light-public
	 */
	typedef enum tpbrt_area_light_type_t {
		/// @brief Diffuse area light.
		TPBRT_AREA_LIGHT_TYPE_DIFFUSE = 0,
		/// @brief Count of available area light types.
		TPBRT_AREA_LIGHT_TYPE_MAX_NUM = 1,
	} tpbrt_area_light_type_t;

	/**
	 * @brief Diffuse area light parameters.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_area_light_diffuse_params_t {
		/// @brief Filename for an image that describes spatially-varying emission over the surface of the emitter. The emitting shape's default (u,v) parameterization is used to map the image to the surface.
		tpbrt_string_t file_name;
		/// @brief Spectral distribution of the light's emitted radiance.
		tpbrt_spectrum_t l;
		/// @brief Determines whether the light source emits light from just the side of the surface where the surface normal points or both sides.
		tpbrt_bool_t two_sided : 1;
	} tpbrt_area_light_diffuse_params_t;

	/**
	 * @brief PBRT area light representation.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_area_light_t {
		/// @brief Area light index.
		tpbrt_area_light_idx_t idx;
		/// @brief Scale factor that modulates the amount of light that the light source emits into the scene.
		tpbrt_float_t scale;
		/// @brief Total luminous power or illuminance emitted by the light.
		tpbrt_opt_float_t power;

		/// @brief Value dependent on type value.
		union {
			/// @brief Diffuse area light parameters.
			tpbrt_area_light_diffuse_params_t diffuse;
		} as;

		/// @brief Area light value type.
		tpbrt_area_light_type_t type : 2;
	} tpbrt_area_light_t;

	/**
	 * @brief Area light handle.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_area_light_handle_t {
		/// @brief Area light index.
		tpbrt_area_light_idx_t area_light_idx;
	} tpbrt_area_light_handle_t;

/**
 * @brief Invalid area light handle initialization.
 * @ingroup light-public
 */
#define TPBRT_AREA_LIGHT_HANDLE_INVALID_INIT { .area_light_idx = TPBRT_AREA_LIGHT_IDX_INVALID }
/**
 * @brief Invalid area light handle initialization with type conversion.
 * @ingroup light-public
 */
#define TPBRT_AREA_LIGHT_HANDLE_INVALID (tpbrt_area_light_handle_t) TPBRT_AREA_LIGHT_HANDLE_INVALID_INIT

	/**
	 * @brief List of light sources.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_lights_sources_list_t {
		/// @brief Light sources.
		tpbrt_light_source_t* lights;
		/// @brief Number of light sources in list.
		tpbrt_size_t count;
	} tpbrt_lights_sources_list_t;

	/**
	 * @brief List of area lights.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_area_lights_list_t {
		/// @brief Area lights.
		tpbrt_area_light_t* lights;
		/// @brief Number of area lights in list.
		tpbrt_size_t count;
	} tpbrt_area_lights_list_t;

	/**
	 * @brief List of all types of lights.
	 * @ingroup light-public
	 */
	typedef struct tpbrt_lights_list_t {
		/// @brief List of light sources.
		tpbrt_lights_sources_list_t lights_sources;
		/// @brief List of area lights.
		tpbrt_area_lights_list_t area_lights;
	} tpbrt_lights_list_t;

	/**
	 * @brief Returns pointer to area light to which handle is provided.
	 * @param area_lights_list List of area lights in which area light given by handle should be.
	 * @param handle Handle to area light.
	 * @param area_light Pointer in which pointer to area light will be stored (if not found this will be TPBRT_NULL).
	 * @return Returns error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup light-public
	 */
	TPBRT_API tpbrt_error_t tpbrt_get_area_light_by_handle(const tpbrt_area_lights_list_t* area_lights_list,
	  tpbrt_area_light_handle_t handle, const tpbrt_area_light_t** area_light);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_LIGHT_H_
