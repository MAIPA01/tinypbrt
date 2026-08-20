#include <tinypbrt/pch.h>

#include <tinypbrt/detail/texture_internal.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	static tpbrt_error_t tpbrt_texture_type_from_string(const tpbrt_string_t* const type_str, tpbrt_texture_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_TEXTURE_TYPE_MAX_NUM] = {
			{ .chars = "float",	.size = 5 },
			{ .chars = "spectrum", .size = 8 },
		};

			if (type_str == TPBRT_NULL || type_str->chars == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_texture_type_t t = 0; t < TPBRT_TEXTURE_TYPE_MAX_NUM; ++t) {
					if (type_str->size == TYPES_STRS[t].size &&
						strncmp(type_str->chars, TYPES_STRS[t].chars, TYPES_STRS[t].size) == 0) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_TEXTURE_TYPE;
	}

	static tpbrt_error_t tpbrt_texture_class_from_string(const tpbrt_string_t* const class_str,
	  tpbrt_texture_class_t* const class) {
		static const tpbrt_string_t CLASSES_STRS[TPBRT_TEXTURE_CLASS_MAX_NUM] = {
			{ .chars = "bilerp",		 .size = 6  },
			{ .chars = "checkerboard", .size = 12 },
			{ .chars = "constant",	   .size = 8	 },
			{ .chars = "directionmix", .size = 12 },
			{ .chars = "dots",		   .size = 4	 },
			{ .chars = "fbm",		  .size = 3	},
			{ .chars = "imagemap",	   .size = 8	 },
			{ .chars = "marble",		 .size = 6  },
			{ .chars = "mix",		  .size = 3	},
			{ .chars = "ptex",		   .size = 4	 },
			{ .chars = "scale",		.size = 5  },
			{ .chars = "windy",		.size = 5  },
			{ .chars = "wrinkled",	   .size = 8	 },
		};

			if (class_str == TPBRT_NULL || class_str->chars == TPBRT_NULL || class == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_texture_class_t c = 0; c < TPBRT_TEXTURE_CLASS_MAX_NUM; ++c) {
					if (class_str->size == CLASSES_STRS[c].size &&
						strncmp(class_str->chars, CLASSES_STRS[c].chars, CLASSES_STRS[c].size) == 0) {
						*class = c;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_TEXTURE_CLASS;
	}

	static tpbrt_error_t tpbrt_texture_wrap_from_string(const tpbrt_string_t* const wrap_str, tpbrt_texture_wrap_t* const wrap) {
		static const tpbrt_string_t WRAPS_STRS[TPBRT_TEXTURE_WRAP_MAX_NUM] = {
			{ .chars = "repeat", .size = 6 },
			{ .chars = "black",	.size = 5 },
			{ .chars = "clamp",	.size = 5 },
		};

			if (wrap_str == TPBRT_NULL || wrap_str->chars == TPBRT_NULL || wrap == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_texture_wrap_t w = 0; w < TPBRT_TEXTURE_WRAP_MAX_NUM; ++w) {
					if (wrap_str->size == WRAPS_STRS[w].size &&
						strncmp(wrap_str->chars, WRAPS_STRS[w].chars, WRAPS_STRS[w].size) == 0) {
						*wrap = w;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_TEXTURE_WRAP;
	}

	static tpbrt_error_t tpbrt_texture_filter_from_string(const tpbrt_string_t* const filter_str,
	  tpbrt_texture_filter_t* const filter) {
		static const tpbrt_string_t FILTERS_STRS[TPBRT_TEXTURE_FILTER_MAX_NUM] = {
			{ .chars = "bilinear",  .size = 8 },
			{ .chars = "ewa",		  .size = 3 },
			{ .chars = "trilinear", .size = 9 },
			{ .chars = "point",		.size = 5 },
		};

			if (filter_str == TPBRT_NULL || filter_str->chars == TPBRT_NULL || filter == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_texture_filter_t f = 0; f < TPBRT_TEXTURE_FILTER_MAX_NUM; ++f) {
					if (filter_str->size == FILTERS_STRS[f].size &&
						strncmp(filter_str->chars, FILTERS_STRS[f].chars, FILTERS_STRS[f].size) == 0) {
						*filter = f;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_TEXTURE_FILTER;
	}

	static tpbrt_error_t tpbrt_texture_encoding_from_string(const tpbrt_string_t* const encoding_str,
	  tpbrt_texture_encoding_t* const encoding, tpbrt_opt_float_t* const gamma_val) {
		static const tpbrt_string_t ENCODINGS_STRS[TPBRT_TEXTURE_ENCODING_MAX_NUM - 1] = {
			{ .chars = "sRGB",   .size = 4 },
			{ .chars = "linear", .size = 6 },
		};
		static const tpbrt_string_t GAMMA_STR = { .chars = "gamma", .size = 5 };

			if (encoding_str == TPBRT_NULL || encoding_str->chars == TPBRT_NULL || encoding == TPBRT_NULL ||
				gamma_val == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_texture_encoding_t e = 0; e < TPBRT_TEXTURE_ENCODING_MAX_NUM - 1; ++e) {
					if (encoding_str->size == ENCODINGS_STRS[e].size &&
						strncmp(encoding_str->chars, ENCODINGS_STRS[e].chars, ENCODINGS_STRS[e].size) == 0) {
						*encoding			 = e;
						gamma_val->has_value = TPBRT_FALSE;
						return TPBRT_ERROR_NONE;
					}
			}

		const tpbrt_char_t* curr;
			if ((curr = strstr(encoding_str->chars, GAMMA_STR.chars)) == TPBRT_NULL) {
				gamma_val->has_value = TPBRT_FALSE;
				return TPBRT_ERROR_UNKNOWN_TEXTURE_ENCODING;
			}

			if (curr - encoding_str->chars >= GAMMA_STR.size) {
				gamma_val->has_value = TPBRT_FALSE;
				return TPBRT_ERROR_UNKNOWN_TEXTURE_ENCODING;
			}

		curr += GAMMA_STR.size;

		tpbrt_size_t len;
		curr = tpbrt_next_token(curr, encoding_str->chars + encoding_str->size, &len);
			if (len == 0) {
				gamma_val->has_value = TPBRT_FALSE;
				return TPBRT_ERROR_PARSE_FLOAT;
			}

			if (!tpbrt_parse_float_token(curr, len, &gamma_val->value)) {
				gamma_val->has_value = TPBRT_FALSE;
				return TPBRT_ERROR_PARSE_FLOAT;
			}

		gamma_val->has_value = TPBRT_TRUE;
		*encoding			 = TPBRT_TEXTURE_ENCODING_GAMMA_VAL;
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_texture_mapping_from_string(const tpbrt_string_t* const mapping_str,
	  tpbrt_texture_mapping_t* const mapping) {
		static const tpbrt_string_t MAPPINGS_STRS[TPBRT_TEXTURE_MAPPING_MAX_NUM] = {
			{ .chars = "uv",			 .size = 2  },
			{ .chars = "spherical",	.size = 9  },
			{ .chars = "cylindrical", .size = 11 },
			{ .chars = "planar",		 .size = 6  },
		};

			if (mapping_str == TPBRT_NULL || mapping_str->chars == TPBRT_NULL || mapping == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_texture_mapping_t m = 0; m < TPBRT_TEXTURE_MAPPING_MAX_NUM; ++m) {
					if (mapping_str->size == MAPPINGS_STRS[m].size &&
						strncmp(mapping_str->chars, MAPPINGS_STRS[m].chars, MAPPINGS_STRS[m].size) == 0) {
						*mapping = m;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_TEXTURE_MAPPING;
	}

#pragma region TEXTURE

	tpbrt_error_t tpbrt_create_texture(const tpbrt_string_t* const name, const tpbrt_string_t* const type_str,
	  const tpbrt_string_t* const class_str, const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures,
	  const tpbrt_mat4_t* ctm, tpbrt_texture_t** const texture) {
			if (name == TPBRT_NULL || name->chars == TPBRT_NULL || type_str == TPBRT_NULL || type_str->chars == TPBRT_NULL ||
				class_str == TPBRT_NULL || class_str->chars == TPBRT_NULL || params == TPBRT_NULL || textures == TPBRT_NULL ||
				ctm == TPBRT_NULL || texture == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		*texture = malloc(sizeof(tpbrt_texture_t));
			if (*texture == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		(*texture)->idx	  = ~(tpbrt_size_t)0;

		tpbrt_error_t err = tpbrt_copy_string(&(*texture)->name, name);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_texture(texture);
				return err;
			}

		err = tpbrt_texture_type_from_string(type_str, &(*texture)->type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_texture(texture);
				return err;
			}

		err = tpbrt_texture_class_from_string(class_str, &(*texture)->class);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_texture(texture);
				return err;
			}

		static const tpbrt_string_t MAPPING_STR = { .chars = "mapping", .size = 7 };

		tpbrt_string_t mapping_str;
		err = tpbrt_params_list_get_string(params, &MAPPING_STR, &mapping_str);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
				tpbrt_free_texture(texture);
				return err;
			}

			if (err == TPBRT_ERROR_NOT_FOUND) { (*texture)->mapping = TPBRT_TEXTURE_MAPPING_UV; }
			else {
				err = tpbrt_texture_mapping_from_string(&mapping_str, &(*texture)->mapping);
				free(mapping_str.chars);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_free_texture(texture);
						return err;
					}
			}

			// mapping
			switch ((*texture)->mapping) {
			default:
				case TPBRT_TEXTURE_MAPPING_UV: {
					static const tpbrt_string_t U_SCALE_STR = { .chars = "uscale", .size = 6 };
					static const tpbrt_string_t V_SCALE_STR = { .chars = "vscale", .size = 6 };
					static const tpbrt_string_t U_DELTA_STR = { .chars = "udelta", .size = 6 };
					static const tpbrt_string_t V_DELTA_STR = { .chars = "vdelta", .size = 6 };

					err = tpbrt_params_list_get_float(params, &U_SCALE_STR, 1.0f, &(*texture)->uv_params.u_scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &V_SCALE_STR, 1.0f, &(*texture)->uv_params.v_scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &U_DELTA_STR, 0.0f, &(*texture)->uv_params.u_delta);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &V_DELTA_STR, 0.0f, &(*texture)->uv_params.v_delta);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}
					break;
				}
				case TPBRT_TEXTURE_MAPPING_SPHERICAL: {
					(*texture)->spherical_params.transform = *ctm;
				}
				case TPBRT_TEXTURE_MAPPING_CYLINDRICAL: {
					(*texture)->cylindrical_params.transform = *ctm;
				}
				case TPBRT_TEXTURE_MAPPING_PLANAR: {
					static const tpbrt_string_t U_DELTA_STR = { .chars = "udelta", .size = 6 };
					static const tpbrt_string_t V_DELTA_STR = { .chars = "vdelta", .size = 6 };
					static const tpbrt_string_t V1_STR		= { .chars = "v1", .size = 2 };
					static const tpbrt_string_t V2_STR		= { .chars = "v2", .size = 2 };

					err = tpbrt_params_list_get_float(params, &U_DELTA_STR, 0.0f, &(*texture)->planar_params.u_delta);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &V_DELTA_STR, 0.0f, &(*texture)->planar_params.v_delta);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_vec(params, &V1_STR, &(*texture)->planar_params.v1);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->planar_params.v1.x = 1.0f;
							(*texture)->planar_params.v1.y = 0.0f;
							(*texture)->planar_params.v1.z = 0.0f;
						}

					err = tpbrt_params_list_get_vec(params, &V2_STR, &(*texture)->planar_params.v2);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->planar_params.v2.x = 0.0f;
							(*texture)->planar_params.v2.y = 1.0f;
							(*texture)->planar_params.v2.z = 0.0f;
						}
					break;
				}
			}

			// class
			switch ((*texture)->class) {
			default:
				case TPBRT_TEXTURE_CLASS_BILERP: {
					static const tpbrt_string_t V00_STR = { .chars = "v00", .size = 3 };
					static const tpbrt_string_t V01_STR = { .chars = "v01", .size = 3 };
					static const tpbrt_string_t V10_STR = { .chars = "v10", .size = 3 };
					static const tpbrt_string_t V11_STR = { .chars = "v11", .size = 3 };

					tpbrt_string_t texture_name;
					err = tpbrt_params_list_get_string(params, &V00_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->bilinear_params.v00.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
							(*texture)->bilinear_params.v00.single_value = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 0.0f,
							  &(*texture)->bilinear_params.v00);
							free(texture_name.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &V01_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->bilinear_params.v01.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
							(*texture)->bilinear_params.v01.single_value = 1.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 1.0f,
							  &(*texture)->bilinear_params.v01);
							free(texture_name.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &V10_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->bilinear_params.v10.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
							(*texture)->bilinear_params.v10.single_value = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 0.0f,
							  &(*texture)->bilinear_params.v10);
							free(texture_name.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &V11_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->bilinear_params.v11.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
							(*texture)->bilinear_params.v11.single_value = 1.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 1.0f,
							  &(*texture)->bilinear_params.v11);
							free(texture_name.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_CHECKER_BOARD: {
					static const tpbrt_string_t DIMENSION_STR = { .chars = "dimension", .size = 9 };
					static const tpbrt_string_t TEX1_STR	  = { .chars = "tex1", .size = 4 };
					static const tpbrt_string_t TEX2_STR	  = { .chars = "tex2", .size = 4 };

					err = tpbrt_params_list_get_uint(params, &DIMENSION_STR, 2u, &(*texture)->checkboard_params.dimension);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					tpbrt_string_t texture_name;
					err = tpbrt_params_list_get_string(params, &TEX1_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->checkboard_params.tex1.value_type	= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
							(*texture)->checkboard_params.tex1.single_value = 1.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 1.0f,
							  &(*texture)->checkboard_params.tex1);
							free(texture_name.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &TEX2_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->checkboard_params.tex2.value_type	= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
							(*texture)->checkboard_params.tex2.single_value = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 0.0f,
							  &(*texture)->checkboard_params.tex2);
							free(texture_name.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_CONSTANT: {
					static const tpbrt_string_t VALUE_STR = { .chars = "value", .size = 5 };

					tpbrt_string_t texture_name;
					err = tpbrt_params_list_get_string(params, &VALUE_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->constant_params.value.value_type   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
							(*texture)->constant_params.value.single_value = 1.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 1.0f,
							  &(*texture)->constant_params.value);
							free(texture_name.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_DIRECTION_MIX: {
					static const tpbrt_string_t TEX1_STR = { .chars = "tex1", .size = 4 };
					static const tpbrt_string_t TEX2_STR = { .chars = "tex2", .size = 4 };
					static const tpbrt_string_t DIR_STR	 = { .chars = "dir", .size = 3 };

					tpbrt_string_t texture_name;
					err = tpbrt_params_list_get_string(params, &TEX1_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->direction_mix_params.tex1.value_type   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
							(*texture)->direction_mix_params.tex1.single_value = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 0.0f,
							  &(*texture)->direction_mix_params.tex1);
							free(texture_name.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &TEX2_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->direction_mix_params.tex2.value_type   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
							(*texture)->direction_mix_params.tex2.single_value = 1.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 1.0f,
							  &(*texture)->direction_mix_params.tex2);
							free(texture_name.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_vec(params, &DIR_STR, &(*texture)->direction_mix_params.dir);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->direction_mix_params.dir.x = 0.0f;
							(*texture)->direction_mix_params.dir.y = 1.0f;
							(*texture)->direction_mix_params.dir.z = 0.0f;
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_DOTS: {
					static const tpbrt_string_t INSIDE_STR	= { .chars = "inside", .size = 6 };
					static const tpbrt_string_t OUTSIDE_STR = { .chars = "outside", .size = 7 };

					tpbrt_string_t texture_name;
					err = tpbrt_params_list_get_string(params, &INSIDE_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->dot_params.inside.value_type   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
							(*texture)->dot_params.inside.single_value = 1.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 1.0f,
							  &(*texture)->dot_params.inside);
							free(texture_name.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &OUTSIDE_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->dot_params.outside.value_type	= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
							(*texture)->dot_params.outside.single_value = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 0.0f,
							  &(*texture)->dot_params.outside);
							free(texture_name.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}
					break;
				}
			case TPBRT_TEXTURE_CLASS_FBM:
			case TPBRT_TEXTURE_CLASS_WINDY:
				case TPBRT_TEXTURE_CLASS_WRINKLED: {
					static const tpbrt_string_t OCTAVES_STR	  = { .chars = "octaves", .size = 7 };
					static const tpbrt_string_t ROUGHNESS_STR = { .chars = "roughness", .size = 9 };

					static const tpbrt_uint_t OCTAVES_DEFAULT = 8u;
					err										  = tpbrt_params_list_get_uint(params, &OCTAVES_STR, OCTAVES_DEFAULT,
					  &(*texture)->fbm_wrinkled_windy_params.octaves);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					static const tpbrt_float_t ROUGHNESS_DEFAULT = 0.5f;
					err = tpbrt_params_list_get_float(params, &ROUGHNESS_STR, ROUGHNESS_DEFAULT,
					  &(*texture)->fbm_wrinkled_windy_params.roughness);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_IMAGE_MAP: {
					static const tpbrt_string_t FILE_NAME_STR	   = { .chars = "filename", .size = 8 };
					static const tpbrt_string_t WRAP_STR		   = { .chars = "wrap", .size = 4 };
					static const tpbrt_string_t MAX_ANISOTROPY_STR = { .chars = "maxanisotropy", .size = 13 };
					static const tpbrt_string_t FILTER_STR		   = { .chars = "filter", .size = 6 };
					static const tpbrt_string_t ENCODING_STR	   = { .chars = "encoding", .size = 8 };
					static const tpbrt_string_t SCALE_STR		   = { .chars = "scale", .size = 5 };
					static const tpbrt_string_t INVERT_STR		   = { .chars = "invert", .size = 6 };

					err = tpbrt_params_list_get_string(params, &FILE_NAME_STR, &(*texture)->image_map_params.file_name);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					tpbrt_string_t temp_str;
					err = tpbrt_params_list_get_string(params, &WRAP_STR, &temp_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) { (*texture)->image_map_params.wrap = TPBRT_TEXTURE_WRAP_REPEAT; }
						else {
							err = tpbrt_texture_wrap_from_string(&temp_str, &(*texture)->image_map_params.wrap);
							free(temp_str.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					static const tpbrt_float_t MAX_ANISOTROPY_DEFAULT = 8.0f;
					err = tpbrt_params_list_get_float(params, &MAX_ANISOTROPY_STR, MAX_ANISOTROPY_DEFAULT,
					  &(*texture)->image_map_params.max_anisotropy);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_string(params, &FILTER_STR, &temp_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) { (*texture)->image_map_params.filter = TPBRT_TEXTURE_FILTER_BILINEAR; }
						else {
							err = tpbrt_texture_filter_from_string(&temp_str, &(*texture)->image_map_params.filter);
							free(temp_str.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &ENCODING_STR, &temp_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) { (*texture)->image_map_params.encoding = TPBRT_TEXTURE_ENCODING_SRGB; }
						else {
							err = tpbrt_texture_encoding_from_string(&temp_str, &(*texture)->image_map_params.encoding,
							  &(*texture)->image_map_params.gamma);
							free(temp_str.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_float(params, &SCALE_STR, 1.0f, &(*texture)->image_map_params.scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_bool(params, &INVERT_STR, TPBRT_FALSE, &(*texture)->image_map_params.invert);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_MARBLE: {
					static const tpbrt_string_t OCTAVES_STR	  = { .chars = "octaves", .size = 7 };
					static const tpbrt_string_t ROUGHNESS_STR = { .chars = "roughness", .size = 9 };
					static const tpbrt_string_t SCALE_STR	  = { .chars = "scale", .size = 5 };
					static const tpbrt_string_t VARIATION_STR = { .chars = "variation", .size = 9 };

					static const tpbrt_uint_t OCTAVES_DEFAULT = 8u;
					err = tpbrt_params_list_get_uint(params, &OCTAVES_STR, OCTAVES_DEFAULT, &(*texture)->marble_params.octaves);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					static const tpbrt_float_t ROUGHNESS_DEFAULT = 0.5f;
					err = tpbrt_params_list_get_float(params, &ROUGHNESS_STR, ROUGHNESS_DEFAULT,
					  &(*texture)->marble_params.roughness);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &SCALE_STR, 1.0f, &(*texture)->marble_params.scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					static const tpbrt_float_t VARIATION_DEFAULT = 0.2f;
					err = tpbrt_params_list_get_float(params, &VARIATION_STR, VARIATION_DEFAULT,
					  &(*texture)->marble_params.variation);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_MIX: {
					static const tpbrt_string_t TEX1_STR   = { .chars = "tex1", .size = 4 };
					static const tpbrt_string_t TEX2_STR   = { .chars = "tex2", .size = 4 };
					static const tpbrt_string_t AMOUNT_STR = { .chars = "amount", .size = 6 };

					tpbrt_string_t texture_name;
					err = tpbrt_params_list_get_string(params, &TEX1_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->mix_params.tex1.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
							(*texture)->mix_params.tex1.single_value = 0.0f;
						}
						else {
							err =
							  tpbrt_textures_list_get_texture_handle(textures, &texture_name, 0.0f, &(*texture)->mix_params.tex1);
							free(texture_name.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &TEX2_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->mix_params.tex2.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
							(*texture)->mix_params.tex2.single_value = 1.0f;
						}
						else {
							err =
							  tpbrt_textures_list_get_texture_handle(textures, &texture_name, 1.0f, &(*texture)->mix_params.tex2);
							free(texture_name.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &AMOUNT_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

					static const tpbrt_float_t AMOUNT_DEFAULT = 0.5f;
						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->mix_params.amount.value_type   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
							(*texture)->mix_params.amount.single_value = AMOUNT_DEFAULT;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, AMOUNT_DEFAULT, &(*texture)->mix_params.amount);
							free(texture_name.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_PTEX: {
					static const tpbrt_string_t ENCODING_STR  = { .chars = "encoding", .size = 8 };
					static const tpbrt_string_t FILE_NAME_STR = { .chars = "filename", .size = 8 };
					static const tpbrt_string_t SCALE_STR	  = { .chars = "scale", .size = 5 };

					tpbrt_string_t temp_str;
					err = tpbrt_params_list_get_string(params, &ENCODING_STR, &temp_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							static const tpbrt_float_t GAMMA_DEFAULT = 2.2f;

							(*texture)->ptex_params.encoding		 = TPBRT_TEXTURE_ENCODING_GAMMA_VAL;
							(*texture)->ptex_params.gamma.has_value	 = TPBRT_TRUE;
							(*texture)->ptex_params.gamma.value		 = GAMMA_DEFAULT;
						}
						else {
							err = tpbrt_texture_encoding_from_string(&temp_str, &(*texture)->ptex_params.encoding,
							  &(*texture)->ptex_params.gamma);
							free(temp_str.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &FILE_NAME_STR, &(*texture)->ptex_params.file_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &SCALE_STR, 1.0f, &(*texture)->ptex_params.scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_SCALE: {
					static const tpbrt_string_t TEX_STR	  = { .chars = "tex", .size = 3 };
					static const tpbrt_string_t SCALE_STR = { .chars = "scale", .size = 5 };

					tpbrt_string_t texture_name;
					err = tpbrt_params_list_get_string(params, &TEX_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->scale_params.tex.value_type	  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
							(*texture)->scale_params.tex.single_value = 1.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 1.0f,
							  &(*texture)->scale_params.tex);
							free(texture_name.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &SCALE_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							(*texture)->scale_params.scale.value_type	= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
							(*texture)->scale_params.scale.single_value = 1.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 1.0f, &(*texture)->scale_params.scale);
							free(texture_name.chars);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_texture(tpbrt_texture_t** const texture) {
			if (texture == TPBRT_NULL || *texture == TPBRT_NULL) { return; }

			switch ((*texture)->class) {
				case TPBRT_TEXTURE_CLASS_IMAGE_MAP: {
						if ((*texture)->image_map_params.file_name.chars != TPBRT_NULL) {
							free((*texture)->image_map_params.file_name.chars);
						}
				}
				case TPBRT_TEXTURE_CLASS_PTEX: {
						if ((*texture)->ptex_params.file_name.chars != TPBRT_NULL) {
							free((*texture)->ptex_params.file_name.chars);
						}
				}
			default: break;
			}

		free(*texture);
		*texture = TPBRT_NULL;
	}

#pragma endregion

#pragma region TEXTURES_LIST

	tpbrt_error_t tpbrt_create_empty_textures_list(tpbrt_textures_list_t** const textures_list) {
			if (textures_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*textures_list = malloc(sizeof(tpbrt_textures_list_t));
			if (*textures_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		(*textures_list)->textures = TPBRT_NULL;
		(*textures_list)->count	   = 0;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_textures_list_add_texture(tpbrt_textures_list_t* const textures_list,
	  const tpbrt_texture_t* const texture) {
			if (texture == TPBRT_NULL || textures_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (textures_list->textures == TPBRT_NULL) {
				textures_list->textures = malloc(sizeof(tpbrt_texture_t));
					if (textures_list->textures == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

				textures_list->textures[0]	   = *texture;
				textures_list->textures[0].idx = 0;
				textures_list->count		   = 1;
				return TPBRT_ERROR_NONE;
			}

			for (tpbrt_size_t i = 0; i < textures_list->count; i++) {
					if (textures_list->textures[i].name.size == texture->name.size &&
						strncmp(textures_list->textures[i].name.chars, texture->name.chars, texture->name.size) == 0) {
						return TPBRT_ERROR_DUPLICATE_TEXTURE_NAME;
					}
			}

		tpbrt_texture_t* new_list = malloc(sizeof(tpbrt_texture_t) * (textures_list->count + 1));
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

			for (tpbrt_size_t i = 0; i < textures_list->count; i++) { new_list[i] = textures_list->textures[i]; }
		new_list[textures_list->count]	   = *texture;
		new_list[textures_list->count].idx = textures_list->count;
		++textures_list->count;
		free(textures_list->textures);
		textures_list->textures = new_list;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_textures_list_get_texture_by_name(const tpbrt_textures_list_t* const textures_list,
	  const tpbrt_string_t* const texture_name, const tpbrt_texture_t** const texture) {
			if (textures_list == TPBRT_NULL || texture_name == TPBRT_NULL || texture_name->chars == TPBRT_NULL ||
				texture == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_size_t i = 0; i < textures_list->count; i++) {
					if (textures_list->textures[i].name.size == texture_name->size &&
						strncmp(textures_list->textures[i].name.chars, texture_name->chars, texture_name->size) == 0) {
						*texture = &textures_list->textures[i];
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_NOT_FOUND;
	}

	tpbrt_error_t tpbrt_textures_list_get_texture_handle(const tpbrt_textures_list_t* const textures_list,
	  const tpbrt_string_t* const texture_name, const tpbrt_float_t default_val, tpbrt_texture_handle_t* const texture_handle) {
			if (textures_list == TPBRT_NULL || texture_name == TPBRT_NULL || texture_name->chars == TPBRT_NULL ||
				texture_handle == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_texture_t* texture;
		const tpbrt_error_t err = tpbrt_textures_list_get_texture_by_name(textures_list, texture_name, &texture);
			if (err == TPBRT_ERROR_NOT_FOUND) {
				texture_handle->value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
				texture_handle->single_value = default_val;
				return TPBRT_ERROR_NONE;
			}

			if (err != TPBRT_ERROR_NONE) { return err; }

		texture_handle->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE;
		texture_handle->tex_idx	   = texture->idx;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_textures_list_get_texture_handle_of_type(const tpbrt_textures_list_t* const textures_list,
	  const tpbrt_string_t* const texture_name, const tpbrt_texture_type_t type, const tpbrt_float_t default_val,
	  tpbrt_texture_handle_t* const texture_handle) {
			if (textures_list == TPBRT_NULL || texture_name == TPBRT_NULL || texture_name->chars == TPBRT_NULL ||
				texture_handle == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_texture_t* texture;
		const tpbrt_error_t err = tpbrt_textures_list_get_texture_by_name(textures_list, texture_name, &texture);
			if (err == TPBRT_ERROR_NOT_FOUND) {
				texture_handle->value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SINGLE;
				texture_handle->single_value = default_val;
				return TPBRT_ERROR_NONE;
			}

			if (err != TPBRT_ERROR_NONE) { return err; }

			if (texture->type != type) { return TPBRT_ERROR_INVALID_TEXTURE_TYPE; }

		texture_handle->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE;
		texture_handle->tex_idx	   = texture->idx;
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_textures_list(tpbrt_textures_list_t** const textures_list) {
			if (textures_list == TPBRT_NULL || *textures_list == TPBRT_NULL) { return; }

			if ((*textures_list)->textures != TPBRT_NULL) {
					for (tpbrt_size_t i = 0; i < (*textures_list)->count; ++i) {
						tpbrt_texture_t* texture = &(*textures_list)->textures[i];
						tpbrt_free_texture(&texture);
					}

				free((*textures_list)->textures);
			}

		free(*textures_list);
		*textures_list = TPBRT_NULL;
	}

	tpbrt_size_t tpbrt_textures_list_size(const tpbrt_textures_list_t* const textures_list) {
		return textures_list != TPBRT_NULL ? textures_list->count : 0;
	}

	tpbrt_bool_t tpbrt_textures_list_is_empty(const tpbrt_textures_list_t* const textures_list) {
		return tpbrt_textures_list_size(textures_list) == 0;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif