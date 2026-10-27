#pragma once
#ifndef _TINYPBRT_OPTIONS_H_
#define _TINYPBRT_OPTIONS_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/coord.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup options-public Options
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief PBRT options representation.
	 * @ingroup options-public
	 */
	typedef struct tpbrt_options_t {
		/// @brief Specifies the filename of an image to use when computing mean squared error versus the number of pixel samples taken (see "msereferenceout").
		tpbrt_string_t mse_reference_image;
		/// @brief Filename for per-sample mean squared error results. When both this and "msereferenceimage" are specified, the mean squared error of the current image and the reference image is computed after each sample is taken and the results are stored in text format in this file.
		tpbrt_string_t mse_reference_out;
		/// @brief Seed to use for pseudo-random number generation during rendering. Rendering a scene with different seed values will give independent results, which can be useful for statistical analysis.
		tpbrt_int_t seed;
		/// @brief Global scale factor applied to triangle edge lengths before evaluating the edge length test for refinement when applying displacement mapping. Increasing this value will generally reduce memory use and startup time when rendering scenes with displacement mapping.
		tpbrt_float_t displacement_edge_scale;
		/// @brief Specifies the coordinate system to use for rendering computation.
		tpbrt_coord_system_type_t render_coord_sys : 3;
		/// @brief Forces all pixel samples to be through the center of the pixel area. Enabling this can be useful when computing reference images and then computing error with respect to them in that it eliminates differences due to geometric sampling that may not be of interest.
		tpbrt_bool_t disable_pixel_jitter		   : 1;
		/// @brief Forces point sampling at the finest MIP level for all texture lookups. (Rarely useful.)
		tpbrt_bool_t disable_texture_filtering	   : 1;
		/// @brief Forces all samples within each pixel to sample the same wavelengths. RGB images will generally have objectionable color error but this can also be useful when computing error with respect to reference images when error due to random wavelength sampling shouldn't be included.
		tpbrt_bool_t disable_wave_length_jitter	   : 1;
		/// @brief Force all materials to be diffuse. (Note: not currently supported with the --wavefront and --gpu integrators.)
		tpbrt_bool_t force_diffuse				   : 1;
		/// @brief Write out images after rendering that encode per-pixel statistics including time spent rendering each pixel and any other per-pixel statistics added using STAT_PIXEL_COUNTER or STAT_PIXEL_RATIO in the pbrt source code.
		tpbrt_bool_t pixel_stats				   : 1;
		/// @brief Enables the "wavefront" integrator (i.e., the integrator used for GPU rendering, but running on the CPU.)
		tpbrt_bool_t wavefront					   : 1;
	} tpbrt_options_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_OPTIONS_H_
