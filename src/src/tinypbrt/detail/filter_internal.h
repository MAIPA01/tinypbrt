#pragma once
#ifndef _TINYPBRT_FILTER_INTERNAL_H_
#define _TINYPBRT_FILTER_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/filter.h>

#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup filter-private Filter
	 * @ingroup tinypbrt-private
	 */

	/**
	 * @brief Returns filter type based on string value.
	 * @param type_str String which is convertible to filter type.
	 * @param type Output filter type value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_FILTER_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup filter-private
	 */
	tpbrt_error_t tpbrt_filter_type_from_string(tpbrt_string_t type_str, tpbrt_filter_type_t* type);

	/**
	 * @brief Initializes default filter.
	 * @param filter Output filter data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup filter-private
	 */
	tpbrt_error_t tpbrt_filter_init_default(tpbrt_filter_t* filter);

	/**
	 * @brief Initializes filter.
	 * @param type_str Filter type string.
	 * @param params Parameters list.
	 * @param filter Output filter data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_FILTER_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup filter-private
	 */
	tpbrt_error_t tpbrt_filter_init(tpbrt_string_t type_str, const tpbrt_params_list_t* params, tpbrt_filter_t* filter);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_FILTER_INTERNAL_H_
