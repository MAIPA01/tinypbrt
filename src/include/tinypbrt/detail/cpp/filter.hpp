#pragma once
#ifndef _TINYPBRT_FILTER_H_
#define _TINYPBRT_FILTER_H_

#include <tinypbrt/detail/fwd.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup filter-public Filter
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Filter type.
	 * @ingroup filter-public
	 */
	typedef enum tpbrt_filter_type_t {
		/// @brief Box filter.
		TPBRT_FILTER_TYPE_BOX	   = 0,
		/// @brief Gaussian filter.
		TPBRT_FILTER_TYPE_GAUSSIAN = 1,
		/// @brief Mitchell filter.
		TPBRT_FILTER_TYPE_MITCHELL = 2,
		/// @brief Sinc filter.
		TPBRT_FILTER_TYPE_SINC	   = 3,
		/// @brief Triangle filter.
		TPBRT_FILTER_TYPE_TRIANGLE = 4,
		/// @brief Count of available filter types.
		TPBRT_FILTER_TYPE_MAX_NUM  = 5,
	} tpbrt_filter_type_t;

	/**
	 * @brief Gaussian filter parameters.
	 * @ingroup filter-public
	 */
	typedef struct tpbrt_filter_gaussian_params_t {
		/// @brief Standard deviation of the Gaussian distribution. Larger values give a filter with broader support.
		tpbrt_float_t sigma;
	} tpbrt_filter_gaussian_params_t;

	/**
	 * @brief Mitchell filter parameters. These parameters control the shape of the Mitchell filter. The best results are generally obtained when B+2C=1.
	 * @ingroup filter-public
	 */
	typedef struct tpbrt_filter_mitchell_params_t {
		/// @brief B parameter.
		tpbrt_float_t b;
		/// @brief C parameter.
		tpbrt_float_t c;
	} tpbrt_filter_mitchell_params_t;

	/**
	 * @brief Sinc filter parameters.
	 * @ingroup filter-public
	 */
	typedef struct tpbrt_filter_sinc_params_t {
		/// @brief Tau controls how many cycles the sinc function passes through before it is clamped to zero by the windowing function.
		tpbrt_float_t tau;
	} tpbrt_filter_sinc_params_t;

	/**
	 * @brief PBRT filter representation.
	 * @ingroup filter-public
	 */
	typedef struct tpbrt_filter_t {
		/// @brief The width of the filter in the x direction.
		tpbrt_float_t x_radius;
		/// @brief The width of the filter in the y direction.
		tpbrt_float_t y_radius;

		/// @brief Value dependent on type value.
		union {
			/// @brief Gaussian parameters.
			tpbrt_filter_gaussian_params_t gaussian;
			/// @brief Mitchell parameters.
			tpbrt_filter_mitchell_params_t mitchell;
			/// @brief Sinc parameters.
			tpbrt_filter_sinc_params_t sinc;
		} as;

		/// @brief filter type.
		tpbrt_filter_type_t type : 4;
	} tpbrt_filter_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_FILTER_H_
