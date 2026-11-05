#pragma once
#ifndef _TINYPBRT_CAMERA_INTERNAL_H_
#define _TINYPBRT_CAMERA_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/camera.h>

#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup camera-private Camera
	 * @ingroup tinypbrt-private
	 */

	/**
	 * @brief Camera create info.
	 * @ingroup camera-private
	 */
	typedef struct tpbrt_camera_create_info_t {
		/// @brief Camera parameters list.
		tpbrt_params_list_t params;
		/// @brief Camera animated transform.
		tpbrt_transform_animated_t camera_transform;
		/// @brief Interior media handle.
		tpbrt_media_handle_t interior_media_handle;
		/// @brief Exterior media handle.
		tpbrt_media_handle_t exterior_media_handle;
		/// @brief Camera type.
		tpbrt_camera_type_t type : 4;
	} tpbrt_camera_create_info_t;

	/**
	 * @brief Returns camera type based on string value.
	 * @param type_str String which is convertable to camera type.
	 * @param type Output camera type value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_CAMERA_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup camera-private
	 */
	tpbrt_error_t tpbrt_camera_type_from_string(tpbrt_string_t type_str, tpbrt_camera_type_t* type);

	/**
	 * @brief Returns camera spherical mapping based on string value.
	 * @param mapping_str String which is convertable to camera spherical mapping.
	 * @param mapping Output camera spherical mapping value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_CAMERA_SPHERICAL_MAPPING, TPBRT_ERROR_NONE).
	 * @ingroup camera-private
	 */
	tpbrt_error_t tpbrt_camera_spherical_mapping_from_string(tpbrt_string_t mapping_str,
	  tpbrt_camera_spherical_mapping_t* mapping);

	/**
	 * @brief Returns camera realistic aperture builtin based on string value.
	 * @param builtin_str String which is convertable to camera realistic aperture builtin.
	 * @param builtin Output camera realistic aperture builtin value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_CAMERA_REALISTIC_APERTURE_BUILTIN, TPBRT_ERROR_NONE).
	 * @ingroup camera-private
	 */
	tpbrt_error_t tpbrt_camera_realistic_aperture_builtin_from_string(tpbrt_string_t builtin_str,
	  tpbrt_camera_realistic_aperture_builtin_t* builtin);

	/**
	 * @brief Initializes default camera create info.
	 * @param info Output camera create info data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup camera-private
	 */
	tpbrt_error_t tpbrt_camera_create_info_init_default(tpbrt_camera_create_info_t* info);

	/**
	 * @brief Initializes camera create info.
	 * @param type_str camera type string.
	 * @param params Parameters list.
	 * @param transform Animated transform.
	 * @param interior_media_handle Interior media handle.
	 * @param exterior_media_handle Exterior media handle.
	 * @param info Output camera create info data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_CAMERA_TYPE, TPBRT_ERROR_INVALID_CAMERA_SPHERICAL_MAPPING, TPBRT_ERROR_INVALID_CAMERA_REALISTIC_APERTURE_BUILTIN, TPBRT_ERROR_NONE).
	 * @ingroup camera-private
	 */
	tpbrt_error_t tpbrt_camera_create_info_init(tpbrt_string_t type_str, const tpbrt_params_list_t* params,
	  tpbrt_transform_animated_t transform, tpbrt_media_handle_t interior_media_handle,
	  tpbrt_media_handle_t exterior_media_handle, tpbrt_camera_create_info_t* info);

	/**
	 * @brief Frees all camera create info resources.
	 * @param info Camera create info data containing resources to free.
	 * @ingroup camera-private
	 */
	void tpbrt_camera_create_info_free(tpbrt_camera_create_info_t* info);

	/**
	 * @brief Initializes camera.
	 * @param info Camera create info.
	 * @param film Current film data.
	 * @param camera Output camera data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup camera-private
	 */
	tpbrt_error_t tpbrt_camera_init(const tpbrt_camera_create_info_t* info, const tpbrt_film_t* film, tpbrt_camera_t* camera);

	/**
	 * @brief Frees all camera resources.
	 * @param camera Camera data containing resources to free.
	 * @ingroup camera-private
	 */
	void tpbrt_camera_free(tpbrt_camera_t* camera);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_CAMERA_INTERNAL_H_
