#include <tinypbrt/pch.h>

#include <tinypbrt/detail/sampler_internal.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	static tpbrt_error_t tpbrt_sampler_type_from_string(const tpbrt_string_t* const type_str, tpbrt_sampler_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_SAMPLER_TYPE_MAX_NUM] = {
			TPBRT_STRING("halton"),
			TPBRT_STRING("independent"),
			TPBRT_STRING("paddedsobol"),
			TPBRT_STRING("sobol"),
			TPBRT_STRING("stratified"),
			TPBRT_STRING("zsobol"),
		};

			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_sampler_type_t t = 0; t < TPBRT_SAMPLER_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS + t)) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_SAMPLER_TYPE;
	}

	static tpbrt_error_t tpbrt_sampler_random_from_string(const tpbrt_string_t* const random_str,
	  tpbrt_sampler_random_t* const random) {
		static const tpbrt_string_t RANDOMS_STRS[TPBRT_SAMPLER_RANDOM_MAX_NUM] = {
			TPBRT_STRING("none"),
			TPBRT_STRING("permutedigits"),
			TPBRT_STRING("owen"),
			TPBRT_STRING("fastowen"),
		};

			if (random_str == TPBRT_NULL || random_str->data == TPBRT_NULL || random == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_sampler_random_t r = 0; r < TPBRT_SAMPLER_RANDOM_MAX_NUM; ++r) {
					if (tpbrt_string_equals(random_str, RANDOMS_STRS + r)) {
						*random = r;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_SAMPLER_RANDOM;
	}

	static tpbrt_uint_t tpbrt_next_power_of_two(tpbrt_uint_t n) {
			if (n == 0) { return 1; }

		--n;
		n |= n >> (1 << 0);
		n |= n >> (1 << 1);
		n |= n >> (1 << 2);
		n |= n >> (1 << 3);
		n |= n >> (1 << 4);
		++n;

		return n;
	}

	tpbrt_error_t tpbrt_create_sampler(const tpbrt_string_t* type_str, const tpbrt_params_list_t* params,
	  tpbrt_sampler_t* sampler) {
			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || params == TPBRT_NULL || sampler == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (memset(sampler, 0, sizeof(tpbrt_sampler_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t SEED_STR = TPBRT_STRING("seed");

		tpbrt_error_t err					 = tpbrt_params_list_get_int(params, &SEED_STR, 0, &sampler->seed);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_sampler_type_from_string(type_str, &sampler->type);
			if (err != TPBRT_ERROR_NONE) { return err; }

		static const tpbrt_string_t PIXEL_SAMPLES_STR	= TPBRT_STRING("pixelsamples");
		static const tpbrt_string_t RANDOMIZATION_STR	= TPBRT_STRING("randomization");

		static const tpbrt_uint_t PIXEL_SAMPLES_DEFAULT = 16u;

			switch (sampler->type) {
			default:
				case TPBRT_SAMPLER_TYPE_HALTON: {
					tpbrt_sampler_halton_params_t* sampler_params = &sampler->as.halton;

					err = tpbrt_params_list_get_uint(params, &PIXEL_SAMPLES_STR, PIXEL_SAMPLES_DEFAULT,
					  &sampler_params->pixel_samples);
						if (err != TPBRT_ERROR_NONE) { return err; }

					tpbrt_string_t random_str;
					err = tpbrt_params_list_get_string(params, &RANDOMIZATION_STR, &random_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

						if (err == TPBRT_ERROR_NOT_FOUND) { sampler_params->randomization = TPBRT_SAMPLER_RANDOM_PERMUTE_DIGITS; }
						else {
							err = tpbrt_sampler_random_from_string(&random_str, &sampler_params->randomization);
							tpbrt_free_string(&random_str);
								if (err != TPBRT_ERROR_NONE) { return err; }
						}
					break;
				}
				case TPBRT_SAMPLER_TYPE_INDEPENDENT: {
					tpbrt_sampler_independent_params_t* sampler_params = &sampler->as.independent;

					err = tpbrt_params_list_get_uint(params, &PIXEL_SAMPLES_STR, PIXEL_SAMPLES_DEFAULT,
					  &sampler_params->pixel_samples);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
				case TPBRT_SAMPLER_TYPE_PADDED_SOBOL: {
					tpbrt_sampler_padded_sobol_params_t* sampler_params = &sampler->as.padded_sobol;

					err = tpbrt_params_list_get_uint(params, &PIXEL_SAMPLES_STR, PIXEL_SAMPLES_DEFAULT,
					  &sampler_params->pixel_samples);
						if (err != TPBRT_ERROR_NONE) { return err; }
					sampler_params->pixel_samples = tpbrt_next_power_of_two(sampler_params->pixel_samples);

					tpbrt_string_t random_str;
					err = tpbrt_params_list_get_string(params, &RANDOMIZATION_STR, &random_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

						if (err == TPBRT_ERROR_NOT_FOUND) { sampler_params->randomization = TPBRT_SAMPLER_RANDOM_FAST_OWEN; }
						else {
							err = tpbrt_sampler_random_from_string(&random_str, &sampler_params->randomization);
							tpbrt_free_string(&random_str);
								if (err != TPBRT_ERROR_NONE) { return err; }
						}
					break;
				}
				case TPBRT_SAMPLER_TYPE_SOBOL: {
					tpbrt_sampler_sobol_params_t* sampler_params = &sampler->as.sobol;

					err = tpbrt_params_list_get_uint(params, &PIXEL_SAMPLES_STR, PIXEL_SAMPLES_DEFAULT,
					  &sampler_params->pixel_samples);
						if (err != TPBRT_ERROR_NONE) { return err; }
					sampler_params->pixel_samples = tpbrt_next_power_of_two(sampler_params->pixel_samples);

					tpbrt_string_t random_str;
					err = tpbrt_params_list_get_string(params, &RANDOMIZATION_STR, &random_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

						if (err == TPBRT_ERROR_NOT_FOUND) { sampler_params->randomization = TPBRT_SAMPLER_RANDOM_FAST_OWEN; }
						else {
							err = tpbrt_sampler_random_from_string(&random_str, &sampler_params->randomization);
							tpbrt_free_string(&random_str);
								if (err != TPBRT_ERROR_NONE) { return err; }
						}
					break;
				}
				case TPBRT_SAMPLER_TYPE_STRATIFIED: {
					static const tpbrt_string_t JITTER_STR			  = TPBRT_STRING("jitter");
					static const tpbrt_string_t X_SAMPLES_STR		  = TPBRT_STRING("xsamples");
					static const tpbrt_string_t Y_SAMPLES_STR		  = TPBRT_STRING("ysamples");

					tpbrt_sampler_stratified_params_t* sampler_params = &sampler->as.stratified;

					err = tpbrt_params_list_get_bool(params, &JITTER_STR, TPBRT_TRUE, &sampler_params->jitter);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err = tpbrt_params_list_get_uint(params, &X_SAMPLES_STR, 4u, &sampler_params->x_samples);
						if (err != TPBRT_ERROR_NONE) { return err; }

					err = tpbrt_params_list_get_uint(params, &Y_SAMPLES_STR, 4u, &sampler_params->y_samples);
						if (err != TPBRT_ERROR_NONE) { return err; }
					break;
				}
				case TPBRT_SAMPLER_TYPE_Z_SOBOL: {
					tpbrt_sampler_z_sobol_params_t* sampler_params = &sampler->as.z_sobol;

					err = tpbrt_params_list_get_uint(params, &PIXEL_SAMPLES_STR, PIXEL_SAMPLES_DEFAULT,
					  &sampler_params->pixel_samples);
						if (err != TPBRT_ERROR_NONE) { return err; }
					sampler_params->pixel_samples = tpbrt_next_power_of_two(sampler_params->pixel_samples);

					tpbrt_string_t random_str;
					err = tpbrt_params_list_get_string(params, &RANDOMIZATION_STR, &random_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

						if (err == TPBRT_ERROR_NOT_FOUND) { sampler_params->randomization = TPBRT_SAMPLER_RANDOM_FAST_OWEN; }
						else {
							err = tpbrt_sampler_random_from_string(&random_str, &sampler_params->randomization);
							tpbrt_free_string(&random_str);
								if (err != TPBRT_ERROR_NONE) { return err; }
						}
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

#ifdef __cplusplus
}
#endif