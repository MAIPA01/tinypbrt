#pragma once
#ifndef _TINYPBRT_SAMPLER_INTERNAL_H_
#define _TINYPBRT_SAMPLER_INTERNAL_H_

#include <tinypbrt/detail/fwd_internal.h>

#include <tinypbrt/detail/sampler.h>

#include <tinypbrt/detail/error.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup sampler-private Sampler
	 * @ingroup tinypbrt-private
	 */

	/**
	 * @brief Returns sampler type based on string value.
	 * @param type_str String which is convertable to sampler type.
	 * @param type Output sampler type value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_SAMPLER_TYPE, TPBRT_ERROR_NONE).
	 * @ingroup sampler-private
	 */
	tpbrt_error_t tpbrt_sampler_type_from_string(tpbrt_string_t type_str, tpbrt_sampler_type_t* type);

	/**
	 * @brief Returns sampler random based on string value.
	 * @param random_str String which is convertable to sampler random.
	 * @param random Output sampler random value.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_SAMPLER_RANDOM, TPBRT_ERROR_NONE).
	 * @ingroup sampler-private
	 */
	tpbrt_error_t tpbrt_sampler_random_from_string(tpbrt_string_t random_str, tpbrt_sampler_random_t* random);

	/**
	 * @brief Initializes default sampler.
	 * @param sampler Output sampler data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
	 * @ingroup sampler-private
	 */
	tpbrt_error_t tpbrt_sampler_init_default(tpbrt_sampler_t* sampler);

	/**
	 * @brief Initializes sampler.
	 * @param type_str Sampler type string.
	 * @param params Parameters list.
	 * @param sampler Output sampler data.
	 * @return Error code (most possible TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_SAMPLER_TYPE, TPBRT_ERROR_INVALID_SAMPLER_RANDOM, TPBRT_ERROR_NONE).
	 * @ingroup sampler-private
	 */
	tpbrt_error_t tpbrt_sampler_init(tpbrt_string_t type_str, const tpbrt_params_list_t* params, tpbrt_sampler_t* sampler);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SAMPLER_INTERNAL_H_
