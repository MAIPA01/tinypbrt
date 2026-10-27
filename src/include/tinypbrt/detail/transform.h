#pragma once
#ifndef _TINYPBRT_TRANSFORM_H_
#define _TINYPBRT_TRANSFORM_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/math.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup transform-public Transform
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Transform handle.
	 * @ingroup transform-public
	 */
	typedef struct tpbrt_transform_handle_t {
		tpbrt_transform_idx_t transform_idx;
	} tpbrt_transform_handle_t;

/**
 * @brief Invalid transform handle initialization.
 * @ingroup transform-public
 */
#define TPBRT_TRANSFORM_HANDLE_INVALID_INIT { .transform_idx = TPBRT_TRANSFORM_IDX_INVALID }
/**
 * @brief Invalid transform handle initialization with type conversion.
 * @ingroup transform-public
 */
#define TPBRT_TRANSFORM_HANDLE_INVALID (tpbrt_transform_handle_t) TPBRT_TRANSFORM_HANDLE_INVALID_INIT

	/**
	 * @brief Animated transform.
	 * @ingroup transform-public
	 */
	typedef struct tpbrt_transform_animated_t {
		/// @brief Handle to starting transform.
		tpbrt_transform_handle_t start_transform;
		/// @brief Handle to ending transform.
		tpbrt_transform_handle_t end_transform;
	} tpbrt_transform_animated_t;

	/**
	 * @brief List of transforms.
	 * @ingroup transform-public
	 */
	typedef struct tpbrt_transforms_list_t {
		/// @brief Transforms matrices list data.
		tpbrt_mat4_t* transforms;
		/// @brief Number of transform matrices in list.
		tpbrt_size_t count;
	} tpbrt_transforms_list_t;

	/**
	 * @brief Returns pointer to transform to which handle is provided.
	 * @param transforms List of transforms in which transform given by handle should be.
	 * @param handle Handle to transform.
	 * @param transform Pointer in which pointer to transform will be stored (if not found this will be TPBRT_NULL).
	 * @return Returns error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup transform-public
	 */
	TPBRT_API tpbrt_error_t tpbrt_get_transform(const tpbrt_transforms_list_t* transforms, tpbrt_transform_handle_t handle,
	  const tpbrt_mat4_t** transform);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_TRANSFORM_H_
