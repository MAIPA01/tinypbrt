#pragma once
#ifndef _TINYPBRT_CAMERA_H_
#define _TINYPBRT_CAMERA_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/math.h>
#include <tinypbrt/detail/media.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup camera-public Camera
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Used to declare camera type.
	 * @ingroup camera-public
	 */
	typedef enum tpbrt_camera_type_t {
		/// @brief Orthographic camera.
		TPBRT_CAMERA_TYPE_ORTHOGRAPHIC = 0,
		/// @brief Perspective camera.
		TPBRT_CAMERA_TYPE_PERSPECTIVE  = 1,
		/// @brief Realistic camera.
		TPBRT_CAMERA_TYPE_REALISTIC	   = 2,
		/// @brief Spherical camera.
		TPBRT_CAMERA_TYPE_SPHERICAL	   = 3,
		/// @brief Count of available camera types.
		TPBRT_CAMERA_TYPE_MAX_NUM	   = 4,
	} tpbrt_camera_type_t;

	/**
	 * @brief Orthographic camera parameters.
	 * @ingroup camera-public
	 */
	typedef struct tpbrt_camera_orthographic_params_t {
		/// @brief The aspect ratio of the film.
		tpbrt_float_t frame_aspect_ratio;
		/// @brief The bounds of the film plane in screen space.
		tpbrt_float_t screen_window[4];
		/// @brief The radius of the lens.
		tpbrt_float_t lens_radius;
		/// @brief The focal distance of the lens. If "lensradius" is zero, this has no effect. Otherwise, it specifies the distance from the camera origin to the focal plane.
		tpbrt_float_t focal_distance;
	} tpbrt_camera_orthographic_params_t;

	/**
	 * @brief Perspective camera parameters.
	 * @ingroup camera-public
	 */
	typedef struct tpbrt_camera_perspective_params_t {
		/// @brief The aspect ratio of the film.
		tpbrt_float_t frame_aspect_ratio;
		/// @brief The bounds of the film plane in screen space.
		tpbrt_float_t screen_window[4];
		/// @brief The radius of the lens.
		tpbrt_float_t lens_radius;
		/// @brief The focal distance of the lens. If "lensradius" is zero, this has no effect. Otherwise, it specifies the distance from the camera origin to the focal plane.
		tpbrt_float_t focal_distance;
		/// @brief Specifies the field of view for the perspective camera. This is the spread angle of the viewing frustum along the narrower of the image's width and height.
		tpbrt_float_t fov;
	} tpbrt_camera_perspective_params_t;

	/**
	 * @brief Spherical camera mapping.
	 * @ingroup camera-public
	 */
	typedef enum tpbrt_camera_spherical_mapping_t {
		/// @brief An area-preserving mapping based on an octahedral encoding of the unit sphere is used.
		TPBRT_CAMERA_SPHERICAL_MAPPING_EQUAL_AREA		= 0,
		/// @brief An equirectangular mapping.
		TPBRT_CAMERA_SPHERICAL_MAPPING_EQUI_RECTANGULAR = 1,
		/// @brief Count of available spherical camera mappings.
		TPBRT_CAMERA_SPHERICAL_MAPPING_MAX_NUM			= 2,
	} tpbrt_camera_spherical_mapping_t;

	/**
	 * @brief Spherical camera parameters.
	 * @ingroup camera-public
	 */
	typedef struct tpbrt_camera_spherical_params_t {
		/// @brief mapping type.
		tpbrt_camera_spherical_mapping_t mapping : 3;
	} tpbrt_camera_spherical_params_t;

	/**
	 * @brief Realistic camera aperture value type.
	 * @ingroup camera-public
	 */
	typedef enum tpbrt_camera_realistic_aperture_type_t {
		/// @brief Aperture will be read from given file specifying an image to be used to specify the shape.
		TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_FILE_NAME = 0,
		/// @brief Aperture value is a builtin aperture.
		TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_BUILTIN   = 1,
		/// @brief Count of available realistic camera apertures.
		TPBRT_CAMERA_REALISTIC_APERTURE_TYPE_MAX_NUM   = 2,
	} tpbrt_camera_realistic_aperture_type_t;

	/**
	 * @brief Realistic camera aperture builtin type.
	 * @ingroup camera-public
	 */
	typedef enum tpbrt_camera_realistic_aperture_builtin_t {
		/// @brief Circular aperture shape.
		TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_CIRCULAR = 0,
		/// @brief Gaussian aperture shape.
		TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_GAUSSIAN = 1,
		/// @brief Square aperture shape.
		TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_SQUARE	 = 2,
		/// @brief Pentagon aperture shape.
		TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_PENTAGON = 3,
		/// @brief Star aperture shape.
		TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_STAR	 = 4,
		/// @brief Count of available realistic camera builtin apertures.
		TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_MAX_NUM	 = 5,
	} tpbrt_camera_realistic_aperture_builtin_t;

	/**
	 * @brief Realistic camera aperture. Allows specifying the shape of the camera aperture.
	 * @ingroup camera-public
	 */
	typedef struct tpbrt_camera_realistic_aperture_t {
		/// @brief Initialized value depends on type value.
		union {
			/// @brief Aperture builtin type.
			tpbrt_camera_realistic_aperture_builtin_t builtin : 4;
			/// @brief File specifying an image to be used to specify the shape.
			tpbrt_string_t file_name;
		} as;

		/// @brief Aperture value type.
		tpbrt_camera_realistic_aperture_type_t type : 3;
	} tpbrt_camera_realistic_aperture_t;

	/**
	 * @brief Realistic camera parameters.
	 * @ingroup camera-public
	 */
	typedef struct tpbrt_camera_realistic_params_t {
		/// @brief Specifies the name of a lens description file that gives the collection of lens elements in the lens system. A number of such lenses are available in the lenses directory in the pbrt-v4 scenes distribution.
		tpbrt_string_t lens_file;
		/// @brief Diameter of the lens system's aperture, specified in mm. The smaller the aperture, the less light reaches the film plane, but the greater the range of distances that are in focus.
		tpbrt_float_t aperture_diameter;
		/// @brief Distance in meters at which the lens system is focused.
		tpbrt_float_t focus_distance;
		/// @brief Allows specifying the shape of the camera aperture.
		tpbrt_camera_realistic_aperture_t aperture;
	} tpbrt_camera_realistic_params_t;

	/**
	 * @brief PBRT Camera representation.
	 * @ingroup camera-public
	 */
	typedef struct tpbrt_camera_t {
		/// @brief The time at which the virtual camera shutter opens.
		tpbrt_float_t shutter_open;
		/// @brief The time at which the virtual camera shutter closes.
		tpbrt_float_t shutter_close;
		/// @brief Transform handle.
		tpbrt_transform_animated_t transform;
		/// @brief Interior media handle.
		tpbrt_media_handle_t interior_media_handle;
		/// @brief Exterior media handle.
		tpbrt_media_handle_t exterior_media_handle;

		/// @brief Initialized value depends on current type value.
		union {
			/// @brief Orthographic camera parameters.
			tpbrt_camera_orthographic_params_t orthographic;
			/// @brief Perspective camera parameters.
			tpbrt_camera_perspective_params_t perspective;
			/// @brief Spherical camera parameters.
			tpbrt_camera_spherical_params_t spherical;
			/// @brief Realistic camera parameters.
			tpbrt_camera_realistic_params_t realistic;
		} as;

		/// @brief Camera type.
		tpbrt_camera_type_t type : 4;
	} tpbrt_camera_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_CAMERA_H_
