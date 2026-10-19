#include <tinypbrt/pch.h>

#include <tinypbrt/detail/sampler_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_sampler_type_from_string(const tpbrt_string_t type_str, tpbrt_sampler_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_SAMPLER_TYPE_MAX_NUM] = {
			TPBRT_STRING("halton"),
			TPBRT_STRING("independent"),
			TPBRT_STRING("paddedsobol"),
			TPBRT_STRING("sobol"),
			TPBRT_STRING("stratified"),
			TPBRT_STRING("zsobol"),
		};

			if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(type_str)) { return TPBRT_ERROR_INVALID_SAMPLER_TYPE; }

			for (tpbrt_sampler_type_t t = 0; t < TPBRT_SAMPLER_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS[t])) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_SAMPLER_TYPE;
	}

	tpbrt_error_t tpbrt_sampler_random_from_string(const tpbrt_string_t random_str, tpbrt_sampler_random_t* const random) {
		static const tpbrt_string_t RANDOMS_STRS[TPBRT_SAMPLER_RANDOM_MAX_NUM] = {
			TPBRT_STRING("none"),
			TPBRT_STRING("permutedigits"),
			TPBRT_STRING("owen"),
			TPBRT_STRING("fastowen"),
		};

			if (random == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(random_str)) { return TPBRT_ERROR_INVALID_SAMPLER_RANDOM; }

			for (tpbrt_sampler_random_t r = 0; r < TPBRT_SAMPLER_RANDOM_MAX_NUM; ++r) {
					if (tpbrt_string_equals(random_str, RANDOMS_STRS[r])) {
						*random = r;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_SAMPLER_RANDOM;
	}

	static tpbrt_error_t tpbrt_sampler_read_pixel_samples(tpbrt_uint_t* const pixel_samples, const tpbrt_params_list_t params) {
			if (pixel_samples == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t PIXEL_SAMPLES_STR	= TPBRT_STRING("pixelsamples");

		static const tpbrt_uint_t PIXEL_SAMPLES_DEFAULT = 16u;

		return tpbrt_params_list_get_uint(params, PIXEL_SAMPLES_STR, PIXEL_SAMPLES_DEFAULT, pixel_samples);
	}

	static tpbrt_error_t tpbrt_sampler_read_randomization(tpbrt_sampler_random_t* const randomization,
	  const tpbrt_params_list_t params, const tpbrt_sampler_random_t default_random) {
			if (randomization == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t RANDOMIZATION_STR = TPBRT_STRING("randomization");

		tpbrt_string_t random_str;
		tpbrt_error_t err = tpbrt_params_list_get_string(params, RANDOMIZATION_STR, TPBRT_EMPTY_STRING, &random_str);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (tpbrt_string_empty(random_str) == TPBRT_TRUE) {
				*randomization = default_random;
				return TPBRT_ERROR_NONE;
			}

		err = tpbrt_sampler_random_from_string(random_str, randomization);
		tpbrt_string_free(&random_str);
		return err;
	}

	static tpbrt_error_t tpbrt_sampler_read_halton(tpbrt_sampler_halton_params_t* const halton,
	  const tpbrt_params_list_t params) {
			if (halton == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_error_t err = tpbrt_sampler_read_pixel_samples(&halton->pixel_samples, params);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_sampler_random_t randomization;
		err = tpbrt_sampler_read_randomization(&randomization, params, TPBRT_SAMPLER_RANDOM_PERMUTE_DIGITS);
			if (err != TPBRT_ERROR_NONE) { return err; }
		halton->randomization = randomization;

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_sampler_read_independent(tpbrt_sampler_independent_params_t* const independent,
	  const tpbrt_params_list_t params) {
			if (independent == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		return tpbrt_sampler_read_pixel_samples(&independent->pixel_samples, params);
	}

	static tpbrt_error_t tpbrt_sampler_read_padded_sobol(tpbrt_sampler_padded_sobol_params_t* const padded_sobol,
	  const tpbrt_params_list_t params) {
			if (padded_sobol == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_error_t err = tpbrt_sampler_read_pixel_samples(&padded_sobol->pixel_samples, params);
			if (err != TPBRT_ERROR_NONE) { return err; }
		padded_sobol->pixel_samples = tpbrt_next_power_of_two(padded_sobol->pixel_samples);

		tpbrt_sampler_random_t randomization;
		err = tpbrt_sampler_read_randomization(&randomization, params, TPBRT_SAMPLER_RANDOM_FAST_OWEN);
			if (err != TPBRT_ERROR_NONE) { return err; }
		padded_sobol->randomization = randomization;

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_sampler_read_sobol(tpbrt_sampler_sobol_params_t* const sobol, const tpbrt_params_list_t params) {
			if (sobol == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_error_t err = tpbrt_sampler_read_pixel_samples(&sobol->pixel_samples, params);
			if (err != TPBRT_ERROR_NONE) { return err; }
		sobol->pixel_samples = tpbrt_next_power_of_two(sobol->pixel_samples);

		tpbrt_sampler_random_t randomization;
		err = tpbrt_sampler_read_randomization(&randomization, params, TPBRT_SAMPLER_RANDOM_FAST_OWEN);
			if (err != TPBRT_ERROR_NONE) { return err; }
		sobol->randomization = randomization;

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_sampler_read_stratified(tpbrt_sampler_stratified_params_t* const stratified,
	  const tpbrt_params_list_t params) {
			if (stratified == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t JITTER_STR	  = TPBRT_STRING("jitter");
		static const tpbrt_string_t X_SAMPLES_STR = TPBRT_STRING("xsamples");
		static const tpbrt_string_t Y_SAMPLES_STR = TPBRT_STRING("ysamples");

		tpbrt_bool_t jitter;
		tpbrt_error_t err = tpbrt_params_list_get_bool(params, JITTER_STR, TPBRT_TRUE, &jitter);
			if (err != TPBRT_ERROR_NONE) { return err; }
		stratified->jitter = jitter;

		err				   = tpbrt_params_list_get_uint(params, X_SAMPLES_STR, 4u, &stratified->x_samples);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_uint(params, Y_SAMPLES_STR, 4u, &stratified->y_samples);
	}

	static tpbrt_error_t tpbrt_sampler_read_z_sobol(tpbrt_sampler_z_sobol_params_t* const z_sobol,
	  const tpbrt_params_list_t params) {
			if (z_sobol == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_error_t err = tpbrt_sampler_read_pixel_samples(&z_sobol->pixel_samples, params);
			if (err != TPBRT_ERROR_NONE) { return err; }
		z_sobol->pixel_samples = tpbrt_next_power_of_two(z_sobol->pixel_samples);

		tpbrt_sampler_random_t randomization;
		err = tpbrt_sampler_read_randomization(&randomization, params, TPBRT_SAMPLER_RANDOM_FAST_OWEN);
			if (err != TPBRT_ERROR_NONE) { return err; }
		z_sobol->randomization = randomization;

		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_sampler_init_default(tpbrt_sampler_t* const sampler) {
			if (sampler == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(sampler, 0, sizeof(tpbrt_sampler_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		sampler->seed					  = 0;
		sampler->type					  = TPBRT_SAMPLER_TYPE_Z_SOBOL;
		sampler->as.z_sobol.pixel_samples = 16u;
		sampler->as.z_sobol.randomization = TPBRT_SAMPLER_RANDOM_FAST_OWEN;

		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_sampler_init(const tpbrt_string_t type_str, const tpbrt_params_list_t params, tpbrt_sampler_t* sampler) {
			if (sampler == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(sampler, 0, sizeof(tpbrt_sampler_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t SEED_STR = TPBRT_STRING("seed");

		tpbrt_error_t err					 = tpbrt_params_list_get_int(params, SEED_STR, 0, &sampler->seed);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_sampler_type_t type;
		err = tpbrt_sampler_type_from_string(type_str, &type);
			if (err != TPBRT_ERROR_NONE) { return err; }
		sampler->type = type;

			switch (sampler->type) {
			default:
				case TPBRT_SAMPLER_TYPE_HALTON: {
					err = tpbrt_sampler_read_halton(&sampler->as.halton, params);
					break;
				}
				case TPBRT_SAMPLER_TYPE_INDEPENDENT: {
					err = tpbrt_sampler_read_independent(&sampler->as.independent, params);
					break;
				}
				case TPBRT_SAMPLER_TYPE_PADDED_SOBOL: {
					err = tpbrt_sampler_read_padded_sobol(&sampler->as.padded_sobol, params);
					break;
				}
				case TPBRT_SAMPLER_TYPE_SOBOL: {
					err = tpbrt_sampler_read_sobol(&sampler->as.sobol, params);
					break;
				}
				case TPBRT_SAMPLER_TYPE_STRATIFIED: {
					err = tpbrt_sampler_read_stratified(&sampler->as.stratified, params);
					break;
				}
				case TPBRT_SAMPLER_TYPE_Z_SOBOL: {
					err = tpbrt_sampler_read_z_sobol(&sampler->as.z_sobol, params);
					break;
				}
			}

		return err;
	}

#ifdef __cplusplus
}
#endif