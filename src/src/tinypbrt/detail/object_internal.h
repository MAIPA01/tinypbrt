#pragma once
#ifndef _TINYPBRT_OBJECT_INTERNAL_H_
#define _TINYPBRT_OBJECT_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/name_index_internal.h>

#include <tinypbrt/detail/object.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup object-private Object
	 * @ingroup tinypbrt-private
	 */

#pragma region OBJECT
/**
 * @brief Invalid object handle initialization.
 * @ingroup object-private
 */
#define TPBRT_OBJECT_HANDLE_INVALID_INIT { .object_idx = TPBRT_OBJECT_IDX_INVALID }
/**
 * @brief Invalid object handle initialization with type conversion.
 * @ingroup object-private
 */
#define TPBRT_OBJECT_HANDLE_INVALID (tpbrt_object_handle_t) TPBRT_OBJECT_HANDLE_INVALID_INIT

	/**
	 * @brief Initializes object data.
	 * @param name Object name.
	 * @param object Output object data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup object-private
	 */
	tpbrt_error_t tpbrt_object_init(tpbrt_string_t name, tpbrt_object_t* object);
	/**
	 * @brief Adds shape to object data.
	 * @param object Object data.
	 * @param shape Shape data to add.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup object-private
	 */
	tpbrt_error_t tpbrt_object_add_shape(tpbrt_object_t* object, const tpbrt_shape_t* shape);
	/**
	 * @brief Frees object data.
	 * @param object Object data.
	 * @ingroup object-private
	 */
	void tpbrt_object_free(tpbrt_object_t* object);
#pragma endregion

#pragma region OBJECTS_LIST
	/**
	 * @brief Initializes objects list.
	 * @param objects_list Objects list.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup object-private
	 */
	tpbrt_error_t tpbrt_objects_list_init(tpbrt_objects_list_t* objects_list);
	/**
	 * @brief Appends object to list.
	 * @param objects_list Objects list.
	 * @param object Object data to append.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup object-private
	 */
	tpbrt_error_t tpbrt_objects_list_append(tpbrt_objects_list_t* objects_list, const tpbrt_object_t* object);
	/**
	 * @brief Returns object of given name.
	 * @param objects_list Objects list.
	 * @param object_name Object name.
	 * @param object Output object data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup object-private
	 */
	tpbrt_error_t tpbrt_objects_list_get(const tpbrt_objects_list_t* objects_list, tpbrt_string_t object_name,
	  const tpbrt_object_t** object);
	/**
	 * @brief Returns object of given name handle.
	 * @param objects_list Objects list.
	 * @param object_name Object name.
	 * @param object_handle Output object handle.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup object-private
	 */
	tpbrt_error_t tpbrt_objects_list_get_handle(const tpbrt_objects_list_t* objects_list, tpbrt_string_t object_name,
	  tpbrt_object_handle_t* object_handle);
	/**
	 * @brief Checks if objects list is empty.
	 * @param objects_list Objects list.
	 * @return TPBRT_TRUE if objects list is empty else TPBRT_FALSE.
	 * @ingroup object-private
	 */
	tpbrt_bool_t tpbrt_objects_list_empty(const tpbrt_objects_list_t* objects_list);
	/**
	 * @brief Shrinks objects list data to minimum.
	 * @param objects_list Objects list.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup object-private
	 */
	tpbrt_error_t tpbrt_objects_list_shrink_to_fit(tpbrt_objects_list_t* objects_list);
	/**
	 * @brief Frees objects list.
	 * @param objects_list Objects list to free.
	 * @ingroup object-private
	 */
	void tpbrt_objects_list_free(tpbrt_objects_list_t* objects_list);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_OBJECT_INTERNAL_H_
