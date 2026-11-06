#pragma once
#ifndef _TINYPBRT_NAME_INDEX_INTERNAL_H_
#define _TINYPBRT_NAME_INDEX_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup name-index-private Name Index
	 * @ingroup tinypbrt-private
	 */

	/**
	 * @brief View on list elements. *(tpbrt_string_t*)((char*)elements + i * stride + name_offset)
	 * @ingroup name-index-private
	 */
	typedef struct tpbrt_name_index_view_t {
		/// @brief Elements pointer.
		const void* elements;
		/// @brief Number of elements.
		tpbrt_size_t count;
		/// @brief Elements stride.
		tpbrt_size_t stride;
		/// @brief Name offset.
		tpbrt_size_t name_offset;
	} tpbrt_name_index_view_t;

	/**
	 * @brief Initializes name index view.
	 * @param list_elements Elements list.
	 * @param list_count Number of elements in list.
	 * @param element_type Type of elements in list.
	 * @ingroup name-index-private
	 */
#define TPBRT_NAME_INDEX_VIEW(list_elements, list_count, element_type)                      \
	(tpbrt_name_index_view_t) {                                                             \
		.elements = (list_elements), .count = (list_count), .stride = sizeof(element_type), \
		.name_offset = offsetof(element_type, name)                                         \
	}

	/**
	 * @brief Additional search condition. If TPBRT_NULL it is ignored.
	 * @ingroup name-index-private
	 */
	typedef tpbrt_bool_t (*tpbrt_name_index_filter_t)(const void* element, const void* context);

	/**
	 * @brief Saves element_idx in element's index.
	 * @param index Name index.
	 * @param view Element name view.
	 * @param element_idx Element index.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup name-index-private
	 */
	tpbrt_error_t tpbrt_name_index_insert(tpbrt_name_index_t* index, const tpbrt_name_index_view_t* view,
	  tpbrt_idx_t element_idx);

	/**
	 * @brief Finds first element with given name which also if not TPBRT_NULL passes additional search condition.
	 * @param index Name indexes table.
	 * @param view Name variable view.
	 * @param name Searched name.
	 * @param filter Additional search condition.
	 * @param context All elements.
	 * @return TPBRT_IDX_INVALID if not found else valid index to element.
	 * @ingroup name-index-private
	 */
	tpbrt_idx_t tpbrt_name_index_find(const tpbrt_name_index_t* index, const tpbrt_name_index_view_t* view, tpbrt_string_t name,
	  tpbrt_name_index_filter_t filter, const void* context);

	/**
	 * @brief Shrinks data to min required.
	 * @param index Name indexes table.
	 * @param view View to name variable.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_OUT_OF_MEMORY, TPBRT_ERROR_NONE).
	 * @ingroup name-index-private
	 */
	tpbrt_error_t tpbrt_name_index_shrink(tpbrt_name_index_t* index, const tpbrt_name_index_view_t* view);

	/**
	 * @brief Frees name indexes table.
	 * @param index Name indexes table.
	 * @ingroup name-index-private
	 */
	void tpbrt_name_index_free(tpbrt_name_index_t* index);

	/**
	 * @brief Appends new element and reserves place for more.
	 * @param data Current pointer to data.
	 * @param count Number of elements in data.
	 * @param element_size Size of one element in data.
	 * @return New data or TPBRT_NULL if any error occurred.
	 * @ingroup name-index-private
	 */
	void* tpbrt_array_reserve_append(void* data, tpbrt_size_t count, tpbrt_size_t element_size);
	// Zwraca tablicę dokładnie na `count` elementów (NULL dla count == 0); przy nieudanym realloc - starą tablicę.
	/**
	 * @brief Shrinks array to hold only 'count' elements.
	 * @param data Pointer to data.
	 * @param count Number of elements in data.
	 * @param element_size Size of one element in data.
	 * @return Pointer to new data or old one if any error occurred. TPBRT_NULL if count == 0.
	 * @ingroup name-index-private
	 */
	void* tpbrt_array_shrink_to_fit(void* data, tpbrt_size_t count, tpbrt_size_t element_size);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_NAME_INDEX_INTERNAL_H_
