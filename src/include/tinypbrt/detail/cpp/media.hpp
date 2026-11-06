#pragma once
#ifndef _TINYPBRT_MEDIA_H_
#define _TINYPBRT_MEDIA_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/color.h>
#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/math.h>
#include <tinypbrt/detail/scattering.h>
#include <tinypbrt/detail/transform.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup media-public Media
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Media type.
	 * @ingroup media-public
	 */
	typedef enum tpbrt_media_type_t {
		/// @brief Cloud.
		TPBRT_MEDIA_TYPE_CLOUD		  = 0,
		/// @brief Homogeneous.
		TPBRT_MEDIA_TYPE_HOMOGENEOUS  = 1,
		/// @brief Nano vdb.
		TPBRT_MEDIA_TYPE_NANO_VDB	  = 2,
		/// @brief Rgb grid.
		TPBRT_MEDIA_TYPE_RGB_GRID	  = 3,
		/// @brief Uniform grid.
		TPBRT_MEDIA_TYPE_UNIFORM_GRID = 4,
		/// @brief Count of available media types.
		TPBRT_MEDIA_TYPE_MAX_NUM	  = 5,
	} tpbrt_media_type_t;

	/**
	 * @brief Cloud media parameters.
	 * @ingroup media-public
	 */
	typedef struct tpbrt_media_cloud_params_t {
		/// @brief Additional scale factor applied to the computed cloud density.
		tpbrt_float_t density;
		/// @brief Parameter that controls the frequency at which the cloud's density varies.
		tpbrt_float_t frequency;
		/// @brief Extent of the cloud in the medium's space.
		tpbrt_point3_t p0;
		/// @brief The medium's Henyey-Greenstein phase function asymmetry parameter.
		tpbrt_float_t g;
		/// @brief Extent of the cloud in the medium's space.
		tpbrt_point3_t p1;
		/// @brief Parameter that controls how wispy the cloud density is.
		tpbrt_float_t wispiness;
		/// @brief The cloud's absorption cross section.
		tpbrt_spectrum_t sigma_a;
		/// @brief The cloud's scattering cross section.
		tpbrt_spectrum_t sigma_s;
	} tpbrt_media_cloud_params_t;

	/**
	 * @brief Media scattering value type.
	 * @ingroup media-public
	 */
	typedef enum tpbrt_media_scattering_value_t {
		/// @brief Uses sigma_s and sigma_a values.
		TPBRT_MEDIA_SCATTERING_VALUE_SIGMA	 = 0,
		/// @brief Uses preset as sigma_s and sigma_a values.
		TPBRT_MEDIA_SCATTERING_VALUE_PRESET	 = 1,
		/// @brief Count of available media scattering values.
		TPBRT_MEDIA_SCATTERING_VALUE_MAX_NUM = 2,
	} tpbrt_media_scattering_value_t;

	/**
	 * @brief Media scattering.
	 * @ingroup media-public
	 */
	typedef struct tpbrt_media_scattering_t {
		/// @brief Scattering value dependent on type value.
		union {
			struct {
				/// @brief The absorption cross section.
				tpbrt_spectrum_t a;
				/// @brief The scattering cross section.
				tpbrt_spectrum_t s;
			} sigma;

			/// @brief Named measured scattering medium properties to use.
			tpbrt_scattering_builtin_t preset;
		} as;

		/// @brief Scattering value type.
		tpbrt_media_scattering_value_t type : 3;
	} tpbrt_media_scattering_t;

	/**
	 * @brief Homogeneous media parameters.
	 * @ingroup media-public
	 */
	typedef struct tpbrt_media_homogeneous_params_t {
		/// @brief The medium's Henyey-Greenstein phase function asymmetry parameter.
		tpbrt_float_t g;
		/// @brief Scale factor applied to the medium's emitted radiance.
		tpbrt_float_t le_scale;
		/// @brief Spectral distribution of the medium's emitted radiance, if any.
		tpbrt_spectrum_t le;
		/// @brief Homogeneous scattering value.
		tpbrt_media_scattering_t scattering;
		/// @brief Scale factor applied to the absorption and scattering cross sections.
		tpbrt_float_t scale;
	} tpbrt_media_homogeneous_params_t;

	/**
	 * @brief Nano vdb media parameters.
	 * @ingroup media-public
	 */
	typedef struct tpbrt_media_nano_vdb_params_t {
		/// @brief The medium's Henyey-Greenstein phase function asymmetry parameter.
		tpbrt_float_t g;
		/// @brief Scale factor applied to the medium's emitted radiance.
		tpbrt_float_t le_scale;
		/// @brief The absorption cross section; this value is scaled by the NanoVDB volume's density at each point.
		tpbrt_spectrum_t sigma_a;
		/// @brief The scattering cross section; this value is scaled by the NanoVDB volume's density at each point.
		tpbrt_spectrum_t sigma_s;
		/// @brief Filename of the NanoVDB volume.
		tpbrt_string_t file_name;
		/// @brief Offset value that is added to the emission temperature, if present.
		tpbrt_float_t temperature_offset;
		/// @brief Scale factor applied to the ofsetted emission temperature to get a temperature in degrees kelvin before the blackbody emission is determined.
		tpbrt_float_t temperature_scale;
		/// @brief Additional scale factor applied to the absorption and scattering cross sections.
		tpbrt_float_t scale;
	} tpbrt_media_nano_vdb_params_t;

	/**
	 * @brief Rgb grid media parameters.
	 * @ingroup media-public
	 */
	typedef struct tpbrt_media_rgb_grid_params_t {
		/// @brief nx*ny*nz RGB colors that specify a grid of emission spectra.
		tpbrt_rgb_array_t le;
		/// @brief Extent of the density grid in the medium's space: the coordinates of the lookup point are found with respect to these bounds before the density interpolant weights are computed.
		tpbrt_point3_t p0;
		/// @brief Scale factor applied to the medium's emitted radiance.
		tpbrt_float_t le_scale;
		/// @brief Extent of the density grid in the medium's space: the coordinates of the lookup point are found with respect to these bounds before the density interpolant weights are computed.
		tpbrt_point3_t p1;
		/// @brief The medium's Henyey-Greenstein phase function asymmetry parameter.
		tpbrt_float_t g;
		/// @brief nx*ny*nz RGB colors that specify the grid of absorption cross sections.
		tpbrt_rgb_array_t sigma_a;
		/// @brief nx*ny*nz RGB colors that specify the grid of scattering cross sections.
		tpbrt_rgb_array_t sigma_s;
		/// @brief Scale factor applied to the absorption and scattering cross sections.
		tpbrt_float_t scale;
		/// @brief Grid size in x-coordinates.
		tpbrt_uint_t nx;
		/// @brief Grid size in y-coordinates.
		tpbrt_uint_t ny;
		/// @brief Grid size in z-coordinates.
		tpbrt_uint_t nz;
	} tpbrt_media_rgb_grid_params_t;

	/**
	 * @brief Uniform grid media parameters.
	 * @ingroup media-public
	 */
	typedef struct tpbrt_media_uniform_grid_params_t {
		/// @brief nx*ny*nz density values, in row-major order.
		tpbrt_float_array_t density;
		/// @brief Spectral distribution of the medium's emitted radiance.
		tpbrt_spectrum_t le;
		/// @brief Extent of the density grid in the medium's space: the coordinates of the lookup point are found with respect to these bounds before the density interpolant weights are computed.
		tpbrt_point3_t p0;
		/// @brief Scale factor applied to the medium's emitted radiance.
		tpbrt_float_t le_scale;
		/// @brief Extent of the density grid in the medium's space: the coordinates of the lookup point are found with respect to these bounds before the density interpolant weights are computed.
		tpbrt_point3_t p1;
		/// @brief Grid size in x-coordinates.
		tpbrt_uint_t nx;
		/// @brief Grid size in y-coordinates.
		tpbrt_uint_t ny;
		/// @brief Grid size in z-coordinates.
		tpbrt_uint_t nz;
		/// @brief Scattering value.
		tpbrt_media_scattering_t scattering;
		/// @brief Scale factor applied to the absorption and scattering cross sections.
		tpbrt_float_t scale;
		/// @brief The medium's Henyey-Greenstein phase function asymmetry parameter.
		tpbrt_float_t g;
		/// @brief nx*ny*nz blackbody temperature values in kelvin, in row-major order. If specified, these are trilinearly interpolated and then converted to blackbody emission spectra to describe the medium's emission.
		tpbrt_float_array_t temperature;
		/// @brief Offset value that is added to the emission temperature, if present.
		tpbrt_float_t temperature_offset;
		/// @brief Scale factor applied to the ofsetted emission temperature to get a temperature in degrees kelvin before the blackbody emission is determined.
		tpbrt_float_t temperature_scale;
	} tpbrt_media_uniform_grid_params_t;

	/**
	 * @brief PBRT media representation.
	 * @ingroup media-public
	 */
	typedef struct tpbrt_media_t {
		/// @brief Media index.
		tpbrt_media_idx_t idx;
		/// @brief Transform handle.
		tpbrt_transform_handle_t transform;
		/// @brief Media name.
		tpbrt_string_t name;

		/// @brief Value dependent on type value.
		union {
			/// @brief Cloud media parameters.
			tpbrt_media_cloud_params_t cloud;
			/// @brief Homogeneous media parameters.
			tpbrt_media_homogeneous_params_t homogeneous;
			/// @brief Uniform grid media parameters.
			tpbrt_media_uniform_grid_params_t uniform_grid;
			/// @brief Rgb grid media parameters.
			tpbrt_media_rgb_grid_params_t rgb_grid;
			/// @brief Nano vdb media parameters.
			tpbrt_media_nano_vdb_params_t nano_vdb;
		} as;

		/// @brief Media value type.
		tpbrt_media_type_t type : 4;
	} tpbrt_media_t;

	/**
	 * @brief Media handle.
	 * @ingroup media-public
	 */
	typedef struct tpbrt_media_handle_t {
		/// @brief Media index.
		tpbrt_media_idx_t media_idx;
	} tpbrt_media_handle_t;

/**
 * @brief Invalid media handle initialization.
 * @ingroup media-public
 */
#define TPBRT_MEDIA_HANDLE_INVALID_INIT { .media_idx = TPBRT_MEDIA_IDX_INVALID }
/**
 * @brief Invalid media handle initialization with type conversion.
 * @ingroup media-public
 */
#define TPBRT_MEDIA_HANDLE_INVALID (tpbrt_media_handle_t) TPBRT_MEDIA_HANDLE_INVALID_INIT

	/**
	 * @brief Medias list.
	 * @ingroup media-public
	 */
	typedef struct tpbrt_medias_list_t {
		/// @brief Medias list items.
		tpbrt_media_t* medias;
		/// @brief Number of media items in list.
		tpbrt_size_t count;
		/// @brief Names indexes.
		tpbrt_name_index_t names;
	} tpbrt_medias_list_t;

	/**
	 * @brief Returns pointer to media with given name.
	 * @param medias List of medias in which searched media should be.
	 * @param name Name of the searched object.
	 * @param media Pointer in which pointer to media will be stored (if not found this will be TPBRT_NULL).
	 * @return Returns error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup media-public
	 */
	TPBRT_API tpbrt_error_t tpbrt_get_media_by_name(const tpbrt_medias_list_t* medias, tpbrt_string_t name,
	  const tpbrt_media_t** media);
	/**
	 * @brief Returns pointer to media to which handle is provided.
	 * @param medias List of medias in which media given by handle should be.
	 * @param handle Handle to media.
	 * @param media Pointer in which pointer to media will be stored (if not found this will be TPBRT_NULL).
	 * @return Returns error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup media-public
	 */
	TPBRT_API tpbrt_error_t tpbrt_get_media_by_handle(const tpbrt_medias_list_t* medias, tpbrt_media_handle_t handle,
	  const tpbrt_media_t** media);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MEDIA_H_
