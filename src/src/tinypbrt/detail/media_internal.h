#pragma once
#ifndef _TINYPBRT_MEDIA_INTERNAL_H_
#define _TINYPBRT_MEDIA_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/name_index_internal.h>

#include <tinypbrt/detail/media.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup media-private Media
	 * @ingroup tinypbrt-private
	 */

#pragma region MEDIA
/**
 * @brief Invalid media handle initialization.
 * @ingroup media-private
 */
#define TPBRT_MEDIA_HANDLE_INVALID_INIT { .media_idx = TPBRT_MEDIA_IDX_INVALID }
/**
 * @brief Invalid media handle initialization with type conversion.
 * @ingroup media-private
 */
#define TPBRT_MEDIA_HANDLE_INVALID (tpbrt_media_handle_t) TPBRT_MEDIA_HANDLE_INVALID_INIT

	/**
	 * @brief Returns media type based on string value.
	 * @param type_str String which is convertable to media type.
	 * @param type Output media type value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_MEDIA_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup media-private
	 */
	tpbrt_error_t tpbrt_media_type_from_string(tpbrt_string_t type_str, tpbrt_media_type_t* type);

	/**
	 * @brief Initializes media.
	 * @param name Media name.
	 * @param type_str Media type string.
	 * @param params Parameters list.
	 * @param transform Media transform.
	 * @param media Output media data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_MEDIA_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup media-private
	 */
	tpbrt_error_t tpbrt_media_init(tpbrt_string_t name, tpbrt_string_t type_str, const tpbrt_params_list_t* params,
	  tpbrt_transform_handle_t transform, tpbrt_media_t* media);
	/**
	 * @brief Frees media data.
	 * @param media Media data to free.
	 * @ingroup media-private
	 */
	void tpbrt_media_free(tpbrt_media_t* media);
#pragma endregion

#pragma region MEDIAS_LIST
	/**
	 * @brief Initializes medias list.
	 * @param medias_list Medias list.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup media-private
	 */
	tpbrt_error_t tpbrt_medias_list_init(tpbrt_medias_list_t* medias_list);
	/**
	 * @brief Adds object instance to list.
	 * @param medias_list Medias list.
	 * @param media Media to add to list.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup media-private
	 */
	tpbrt_error_t tpbrt_medias_list_append(tpbrt_medias_list_t* medias_list, const tpbrt_media_t* media);
	/**
	 * @brief Returns media with given name from medias list.
	 * @param medias_list Medias list.
	 * @param media_name Media name.
	 * @param media Output media data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup media-private
	 */
	tpbrt_error_t tpbrt_medias_list_get(const tpbrt_medias_list_t* medias_list, tpbrt_string_t media_name,
	  const tpbrt_media_t** media);
	/**
	 * @brief Returns media handle to media with given name from medias list.
	 * @param medias_list Medias list.
	 * @param media_name Media name.
	 * @param media_handle Output media handle.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup media-private
	 */
	tpbrt_error_t tpbrt_medias_list_get_handle(const tpbrt_medias_list_t* medias_list, tpbrt_string_t media_name,
	  tpbrt_media_handle_t* media_handle);
	/**
	 * @brief Shrinks medias list to minimum size.
	 * @param medias_list Medias list to shrink.
	 * @ingroup media-private
	 */
	tpbrt_error_t tpbrt_medias_list_shrink_to_fit(tpbrt_medias_list_t* medias_list);
	/**
	 * @brief Frees medias list.
	 * @param medias_list Medias list to free.
	 * @ingroup media-private
	 */
	void tpbrt_medias_list_free(tpbrt_medias_list_t* medias_list);

	/**
	 * @brief Checks if medias list is empty.
	 * @param medias_list Medias list to check.
	 * @return TPBRT_TRUE if medias list is empty else TPBRT_FALSE.
	 * @ingroup media-private
	 */
	tpbrt_bool_t tpbrt_medias_list_empty(const tpbrt_medias_list_t* medias_list);
#pragma endregion

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MEDIA_INTERNAL_H_
