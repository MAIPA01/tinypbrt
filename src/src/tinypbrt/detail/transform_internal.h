#pragma once
#ifndef _TINYPBRT_TRANSFORM_INTERNAL_H_
#define _TINYPBRT_TRANSFORM_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/transform.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup transform-private Transform
	 * @ingroup tinypbrt-private
	 */

/**
 * @brief Invalid transform handle initialization.
 * @ingroup transform-private
 */
#define TPBRT_TRANSFORM_HANDLE_INVALID_INIT { .transform_idx = TPBRT_TRANSFORM_IDX_INVALID }
/**
 * @brief Invalid transform handle initialization with type conversion.
 * @ingroup transform-private
 */
#define TPBRT_TRANSFORM_HANDLE_INVALID (tpbrt_transform_handle_t) TPBRT_TRANSFORM_HANDLE_INVALID_INIT

#pragma region TRANSFORMS_LIST
	/**
	 * @brief Initializes transforms list.
	 * @param transforms_list Transforms list data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup transform-private
	 */
	tpbrt_error_t tpbrt_transforms_list_init(tpbrt_transforms_list_t* transforms_list);
	/**
	 * @brief Appends transform to list.
	 * @param transforms_list Transforms list data.
	 * @param transform Transform to append.
	 * @param out_handle Output transform handle.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup transform-private
	 */
	tpbrt_error_t tpbrt_transforms_list_append(tpbrt_transforms_list_t* transforms_list, const tpbrt_mat4_t* transform,
	  tpbrt_transform_handle_t* out_handle);
	/**
	 * @brief Returns transform based on given handle.
	 * @param transforms_list Transforms list data.
	 * @param transform_handle Transform handle.
	 * @param transform Output transform.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_HANDLE, TPBRT_ERROR_NONE).
	 * @ingroup transform-private
	 */
	tpbrt_error_t tpbrt_transforms_list_get(const tpbrt_transforms_list_t* transforms_list,
	  tpbrt_transform_handle_t transform_handle, const tpbrt_mat4_t** transform);
	/**
	 * @brief Frees transform list.
	 * @param transforms_list Transforms list data.
	 * @ingroup transform-private
	 */
	void tpbrt_transforms_list_free(tpbrt_transforms_list_t* transforms_list);

	/**
	 * @brief Checks if transforms list is empty.
	 * @param transforms_list Transforms list data.
	 * @return TPBRT_TRUE if transform list is empty else TPBRT_FALSE.
	 * @ingroup transform-private
	 */
	tpbrt_bool_t tpbrt_transforms_list_empty(const tpbrt_transforms_list_t* transforms_list);
#pragma endregion

#pragma region TRANSFORMS_LIST_BUILDER

	/**
	 * @brief Transforms list builder data.
	 * @ingroup transform-private
	 */
	typedef struct tpbrt_transforms_list_builder_t {
		/// @brief Transforms list.
		tpbrt_transforms_list_t* list;
		/// @brief Current transforms list capacity.
		tpbrt_size_t capacity;
		/// @brief Transforms list buckets.
		tpbrt_transform_idx_t* buckets;
		/// @brief Transforms list buckets count.
		tpbrt_size_t buckets_count;
	} tpbrt_transforms_list_builder_t;

	/**
	 * @brief Initializes transforms list builder.
	 * @param builder Transforms list builder.
	 * @param list Transforms list data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup transform-private
	 */
	tpbrt_error_t tpbrt_transforms_list_builder_init(tpbrt_transforms_list_builder_t* builder, tpbrt_transforms_list_t* list);
	/**
	 * @brief Appends transform to transforms list.
	 * @param builder Transforms list builder.
	 * @param transform Transform to add.
	 * @param out_handle Output transform handle.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup transform-private
	 */
	tpbrt_error_t tpbrt_transforms_list_builder_append(tpbrt_transforms_list_builder_t* builder, const tpbrt_mat4_t* transform,
	  tpbrt_transform_handle_t* out_handle);
	/**
	 * @brief Appends animated transform to transforms list.
	 * @param builder Transforms list builder.
	 * @param transform Animated transform to add.
	 * @param out_transform Output animated transform.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup transform-private
	 */
	tpbrt_error_t tpbrt_transforms_list_builder_append_animated(tpbrt_transforms_list_builder_t* builder,
	  const tpbrt_mat4_animated_t* transform, tpbrt_transform_animated_t* out_transform);
	/**
	 * @brief Frees transforms list builder.
	 * @param builder Transforms list builder to free.
	 * @ingroup transform-private
	 */
	void tpbrt_transforms_list_builder_finish(tpbrt_transforms_list_builder_t* builder);

#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_TRANSFORM_INTERNAL_H_
