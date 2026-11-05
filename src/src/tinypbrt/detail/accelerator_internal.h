#pragma once
#ifndef _TINYPBRT_ACCELERATOR_INTERNAL_H_
#define _TINYPBRT_ACCELERATOR_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/accelerator.h>

#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup accelerator-private Accelerator
	 * @ingroup tinypbrt-private
	 */

	/**
	 * @brief Returns accelerator type based on string value.
	 * @param type_str String which is convertable to accelerator type.
	 * @param type Output accelerator type value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_ACCELERATOR_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup accelerator-private
	 */
	tpbrt_error_t tpbrt_accelerator_type_from_string(tpbrt_string_t type_str, tpbrt_accelerator_type_t* type);

	/**
	 * @brief Returns bvh split method based on string value.
	 * @param method_str String which is convertable to bvh split method.
	 * @param method Output bvh split method value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_ACCELERATOR_BVH_SPLIT_METHOD, TPBRT_ERROR_NONE).
	 * @ingroup accelerator-private
	 */
	tpbrt_error_t tpbrt_accelerator_bvh_split_method_from_string(tpbrt_string_t method_str,
	  tpbrt_accelerator_bvh_split_method_t* method);

	/**
	 * @brief Initializes default accelerator.
	 * @param accelerator Output accelerator data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup accelerator-private
	 */
	tpbrt_error_t tpbrt_accelerator_init_default(tpbrt_accelerator_t* accelerator);

	/**
	 * @brief Initializes accelerator.
	 * @param type_str Accelerator type string.
	 * @param params Parameters list.
	 * @param accelerator Output accelerator data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_ACCELERATOR_BVH_SPLIT_METHOD, TPBRT_ERROR_INVALID_ACCELERATOR_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup accelerator-private
	 */
	tpbrt_error_t tpbrt_accelerator_init(tpbrt_string_t type_str, const tpbrt_params_list_t* params,
	  tpbrt_accelerator_t* accelerator);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_ACCELERATOR_INTERNAL_H_
