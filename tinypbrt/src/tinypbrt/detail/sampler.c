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
					if (type_str->size == TYPES_STRS[t].size &&
						strncmp(type_str->data, TYPES_STRS[t].data, TYPES_STRS[t].size) == 0) {
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
					if (random_str->size == RANDOMS_STRS[r].size &&
						strncmp(random_str->data, RANDOMS_STRS[r].data, RANDOMS_STRS[r].size) == 0) {
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
	  tpbrt_sampler_t** sampler) {
			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || params == TPBRT_NULL || sampler == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		*sampler = malloc(sizeof(tpbrt_sampler_t));
			if (*sampler == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		static const tpbrt_string_t SEED_STR = TPBRT_STRING("seed");

		tpbrt_error_t err					 = tpbrt_params_list_get_int(params, &SEED_STR, 0, &(*sampler)->seed);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_sampler(sampler);
				return err;
			}

		err = tpbrt_sampler_type_from_string(type_str, &(*sampler)->type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_sampler(sampler);
				return err;
			}

		static const tpbrt_string_t PIXEL_SAMPLES_STR	= TPBRT_STRING("pixelsamples");
		static const tpbrt_string_t RANDOMIZATION_STR	= TPBRT_STRING("randomization");

		static const tpbrt_uint_t PIXEL_SAMPLES_DEFAULT = 16u;

			switch ((*sampler)->type) {
			default:
				case TPBRT_SAMPLER_TYPE_HALTON: {
					err = tpbrt_params_list_get_uint(params, &PIXEL_SAMPLES_STR, PIXEL_SAMPLES_DEFAULT,
					  &(*sampler)->as.halton.pixel_samples);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_sampler(sampler);
							return err;
						}

					tpbrt_string_t random_str;
					err = tpbrt_params_list_get_string(params, &RANDOMIZATION_STR, &random_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_sampler(sampler);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*sampler)->as.halton.randomization = TPBRT_SAMPLER_RANDOM_PERMUTE_DIGITS;
						}
						else {
							err = tpbrt_sampler_random_from_string(&random_str, &(*sampler)->as.halton.randomization);
							free(random_str.data);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_sampler(sampler);
									return err;
								}
						}
					break;
				}
				case TPBRT_SAMPLER_TYPE_INDEPENDENT: {
					err = tpbrt_params_list_get_uint(params, &PIXEL_SAMPLES_STR, PIXEL_SAMPLES_DEFAULT,
					  &(*sampler)->as.independent.pixel_samples);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_sampler(sampler);
							return err;
						}
					break;
				}
				case TPBRT_SAMPLER_TYPE_PADDED_SOBOL: {
					err = tpbrt_params_list_get_uint(params, &PIXEL_SAMPLES_STR, PIXEL_SAMPLES_DEFAULT,
					  &(*sampler)->as.padded_sobol.pixel_samples);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_sampler(sampler);
							return err;
						}
					(*sampler)->as.padded_sobol.pixel_samples =
					  tpbrt_next_power_of_two((*sampler)->as.padded_sobol.pixel_samples);

					tpbrt_string_t random_str;
					err = tpbrt_params_list_get_string(params, &RANDOMIZATION_STR, &random_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_sampler(sampler);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*sampler)->as.padded_sobol.randomization = TPBRT_SAMPLER_RANDOM_FAST_OWEN;
						}
						else {
							err = tpbrt_sampler_random_from_string(&random_str, &(*sampler)->as.padded_sobol.randomization);
							free(random_str.data);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_sampler(sampler);
									return err;
								}
						}
					break;
				}
				case TPBRT_SAMPLER_TYPE_SOBOL: {
					err = tpbrt_params_list_get_uint(params, &PIXEL_SAMPLES_STR, PIXEL_SAMPLES_DEFAULT,
					  &(*sampler)->as.sobol.pixel_samples);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_sampler(sampler);
							return err;
						}
					(*sampler)->as.sobol.pixel_samples = tpbrt_next_power_of_two((*sampler)->as.sobol.pixel_samples);

					tpbrt_string_t random_str;
					err = tpbrt_params_list_get_string(params, &RANDOMIZATION_STR, &random_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_sampler(sampler);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) { (*sampler)->as.sobol.randomization = TPBRT_SAMPLER_RANDOM_FAST_OWEN; }
						else {
							err = tpbrt_sampler_random_from_string(&random_str, &(*sampler)->as.sobol.randomization);
							free(random_str.data);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_sampler(sampler);
									return err;
								}
						}
					break;
				}
				case TPBRT_SAMPLER_TYPE_STRATIFIED: {
					static const tpbrt_string_t JITTER_STR	  = TPBRT_STRING("jitter");
					static const tpbrt_string_t X_SAMPLES_STR = TPBRT_STRING("xsamples");
					static const tpbrt_string_t Y_SAMPLES_STR = TPBRT_STRING("ysamples");

					err = tpbrt_params_list_get_bool(params, &JITTER_STR, TPBRT_TRUE, &(*sampler)->as.stratified.jitter);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_sampler(sampler);
							return err;
						}

					err = tpbrt_params_list_get_uint(params, &X_SAMPLES_STR, 4u, &(*sampler)->as.stratified.x_samples);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_sampler(sampler);
							return err;
						}

					err = tpbrt_params_list_get_uint(params, &Y_SAMPLES_STR, 4u, &(*sampler)->as.stratified.y_samples);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_sampler(sampler);
							return err;
						}
					break;
				}
				case TPBRT_SAMPLER_TYPE_Z_SOBOL: {
					err = tpbrt_params_list_get_uint(params, &PIXEL_SAMPLES_STR, PIXEL_SAMPLES_DEFAULT,
					  &(*sampler)->as.z_sobol.pixel_samples);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_sampler(sampler);
							return err;
						}
					(*sampler)->as.z_sobol.pixel_samples = tpbrt_next_power_of_two((*sampler)->as.z_sobol.pixel_samples);

					tpbrt_string_t random_str;
					err = tpbrt_params_list_get_string(params, &RANDOMIZATION_STR, &random_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_sampler(sampler);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*sampler)->as.z_sobol.randomization = TPBRT_SAMPLER_RANDOM_FAST_OWEN;
						}
						else {
							err = tpbrt_sampler_random_from_string(&random_str, &(*sampler)->as.z_sobol.randomization);
							free(random_str.data);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_sampler(sampler);
									return err;
								}
						}
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_sampler(tpbrt_sampler_t** sampler) {
			if (sampler == TPBRT_NULL || *sampler == TPBRT_NULL) { return; }

		free(*sampler);
		*sampler = TPBRT_NULL;
	}

#ifdef __cplusplus
}
#endif