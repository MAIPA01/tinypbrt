#pragma once
#ifndef _TINYPBRT_OPTIONS_INTERNAL_H_
#define _TINYPBRT_OPTIONS_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/error.h>

#include <tinypbrt/detail/options.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup options-private Options
	 * @ingroup tinypbrt-private
	 */

	/**
	 * @brief Initializes default options.
	 * @param options Options data.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup options-private
	 */
	tpbrt_error_t tpbrt_options_init(tpbrt_options_t* options);
	/**
	 * @brief Applies option value.
	 * @param options Options data.
	 * @param param Option value.
	 * @return Error code (possible errors TPBRT_ERROR_INVALID_OPTION, TPBRT_ERROR_NONE).
	 * @ingroup options-private
	 */
	tpbrt_error_t tpbrt_options_apply(tpbrt_options_t* options, const tpbrt_param_t* param);
	/**
	 * @brief Frees options data.
	 * @param options Options data to free.
	 * @ingroup options-private
	 */
	void tpbrt_options_free(tpbrt_options_t* options);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_OPTIONS_INTERNAL_H_
