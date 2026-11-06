#pragma once
#ifndef _TINYPBRT_OBJECT_H_
#define _TINYPBRT_OBJECT_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/shape.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup object-public Object
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief PBRT object representation.
	 * @ingroup object-public
	 */
	typedef struct tpbrt_object_t {
		/// @brief Object name.
		tpbrt_string_t name;
		/// @brief Objects shapes.
		tpbrt_shapes_list_t children;
		/// @brief Object index.
		tpbrt_object_idx_t idx;
	} tpbrt_object_t;

	/**
	 * @brief List of objects.
	 * @ingroup object-public
	 */
	typedef struct tpbrt_objects_list_t {
		/// @brief Objects in list.
		tpbrt_object_t* objects;
		/// @brief Number of objects in list.
		tpbrt_size_t count;
		/// @brief Names indexes.
		tpbrt_name_index_t names;
	} tpbrt_objects_list_t;

	/**
	 * @brief Object handle.
	 * @ingroup object-public
	 */
	typedef struct tpbrt_object_handle_t {
		/// @brief Object index.
		tpbrt_object_idx_t object_idx;
	} tpbrt_object_handle_t;

/**
 * @brief Invalid object handle initialization.
 * @ingroup object-public
 */
#define TPBRT_OBJECT_HANDLE_INVALID_INIT { .object_idx = TPBRT_OBJECT_IDX_INVALID }
/**
 * @brief Invalid object handle initialization with type conversion.
 * @ingroup object-public
 */
#define TPBRT_OBJECT_HANDLE_INVALID (tpbrt_object_handle_t) TPBRT_OBJECT_HANDLE_INVALID_INIT

	/**
	 * @brief Returns pointer to object with given name.
	 * @param objects List of objects in which searched object should be.
	 * @param name Name of the searched object.
	 * @param object Pointer in which pointer to object will be stored (if not found this will be TPBRT_NULL).
	 * @return Returns error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup object-public
	 */
	TPBRT_API tpbrt_error_t tpbrt_get_object_by_name(const tpbrt_objects_list_t* objects, tpbrt_string_t name,
	  const tpbrt_object_t** object);
	/**
	 * @brief Returns pointer to object to which handle is provided.
	 * @param objects List of objects in which object given by handle should be.
	 * @param handle Handle to object.
	 * @param object Pointer in which pointer to object will be stored (if not found this will be TPBRT_NULL).
	 * @return Returns error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup object-public
	 */
	TPBRT_API tpbrt_error_t tpbrt_get_object_by_handle(const tpbrt_objects_list_t* objects, tpbrt_object_handle_t handle,
	  const tpbrt_object_t** object);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_OBJECT_H_
