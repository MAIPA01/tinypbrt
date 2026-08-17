#pragma once
#ifndef _TINYPBRT_FILTER_H_
#define _TINYPBRT_FILTER_H_
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_FILTER_BOX	  = 0,
		TPBRT_FILTER_GAUSSIAN = 1,
		TPBRT_FILTER_MITCHELL = 2,
		TPBRT_FILTER_SINC	  = 3,
		TPBRT_FILTER_TRIANGLE = 4,
	} tpbrt_filter_type_t;

	typedef struct {
		float sigma;
	} tpbrt_filter_gaussian_params_t;

	typedef struct {
		float B;
		float C;
	} tpbrt_filter_mitchell_params_t;

	typedef struct {
		float tau;
	} tpbrt_filter_sinc_params_t;

	typedef struct {
		tpbrt_filter_type_t type;
		float x_radius;
		float y_radius;

		union {
			tpbrt_filter_gaussian_params_t gaussian_params;
			tpbrt_filter_mitchell_params_t mitchell_params;
			tpbrt_filter_sinc_params_t sinc_params;
		};
	} tpbrt_filter_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_FILTER_H_
