#include <tinypbrt/pch.h>

#include <tinypbrt/detail/texture_internal.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region TEXTURE

	tpbrt_error_t tpbrt_texture_type_from_string(const tpbrt_string_t type_str, tpbrt_texture_type_t* const type_out) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_TEXTURE_TYPE_MAX_NUM] = {
			TPBRT_STRING("float"),
			TPBRT_STRING("spectrum"),
		};

			if (type_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(type_str)) { return TPBRT_ERROR_INVALID_TEXTURE_TYPE; }

			for (tpbrt_texture_type_t t = 0; t < TPBRT_TEXTURE_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS[t])) {
						*type_out = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_TEXTURE_TYPE;
	}

	tpbrt_error_t tpbrt_texture_class_from_string(const tpbrt_string_t class_str, tpbrt_texture_class_t* const class_out) {
		static const tpbrt_string_t CLASSES_STRS[TPBRT_TEXTURE_CLASS_MAX_NUM] = {
			TPBRT_STRING("bilerp"),
			TPBRT_STRING("checkerboard"),
			TPBRT_STRING("constant"),
			TPBRT_STRING("directionmix"),
			TPBRT_STRING("dots"),
			TPBRT_STRING("fbm"),
			TPBRT_STRING("imagemap"),
			TPBRT_STRING("marble"),
			TPBRT_STRING("mix"),
			TPBRT_STRING("ptex"),
			TPBRT_STRING("scale"),
			TPBRT_STRING("windy"),
			TPBRT_STRING("wrinkled"),
		};

			if (class_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(class_str)) { return TPBRT_ERROR_INVALID_TEXTURE_CLASS; }

			for (tpbrt_texture_class_t c = 0; c < TPBRT_TEXTURE_CLASS_MAX_NUM; ++c) {
					if (tpbrt_string_equals(class_str, CLASSES_STRS[c])) {
						*class_out = c;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_TEXTURE_CLASS;
	}

	tpbrt_error_t tpbrt_texture_wrap_from_string(const tpbrt_string_t wrap_str, tpbrt_texture_wrap_t* const wrap_out) {
		static const tpbrt_string_t WRAPS_STRS[TPBRT_TEXTURE_WRAP_MAX_NUM] = {
			TPBRT_STRING("repeat"),
			TPBRT_STRING("black"),
			TPBRT_STRING("clamp"),
		};

			if (wrap_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(wrap_str)) { return TPBRT_ERROR_INVALID_TEXTURE_WRAP; }

			for (tpbrt_texture_wrap_t w = 0; w < TPBRT_TEXTURE_WRAP_MAX_NUM; ++w) {
					if (tpbrt_string_equals(wrap_str, WRAPS_STRS[w])) {
						*wrap_out = w;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_TEXTURE_WRAP;
	}

	tpbrt_error_t tpbrt_texture_filter_from_string(const tpbrt_string_t filter_str, tpbrt_texture_filter_t* const filter_out) {
		static const tpbrt_string_t FILTERS_STRS[TPBRT_TEXTURE_FILTER_MAX_NUM] = {
			TPBRT_STRING("bilinear"),
			TPBRT_STRING("ewa"),
			TPBRT_STRING("trilinear"),
			TPBRT_STRING("point"),
		};

			if (filter_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(filter_str)) { return TPBRT_ERROR_INVALID_TEXTURE_FILTER; }

			for (tpbrt_texture_filter_t f = 0; f < TPBRT_TEXTURE_FILTER_MAX_NUM; ++f) {
					if (tpbrt_string_equals(filter_str, FILTERS_STRS[f])) {
						*filter_out = f;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_TEXTURE_FILTER;
	}

	tpbrt_error_t tpbrt_texture_encoding_from_string(const tpbrt_string_t encoding_str,
	  tpbrt_texture_encoding_t* const encoding_out, tpbrt_opt_float_t* const gamma_val_out) {
		static const tpbrt_string_t ENCODINGS_STRS[TPBRT_TEXTURE_ENCODING_MAX_NUM - 1] = {
			TPBRT_STRING("sRGB"),
			TPBRT_STRING("linear"),
		};
		static const tpbrt_string_t GAMMA_STR = TPBRT_STRING("gamma");

			if (encoding_out == TPBRT_NULL || gamma_val_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(encoding_str)) { return TPBRT_ERROR_INVALID_TEXTURE_ENCODING; }

			for (tpbrt_texture_encoding_t e = 0; e < TPBRT_TEXTURE_ENCODING_MAX_NUM - 1; ++e) {
					if (tpbrt_string_equals(encoding_str, ENCODINGS_STRS[e])) {
						*encoding_out			 = e;
						gamma_val_out->has_value = TPBRT_FALSE;
						return TPBRT_ERROR_NONE;
					}
			}

		tpbrt_string_t token;
			if ((token.data = strstr(encoding_str.data, GAMMA_STR.data)) == TPBRT_NULL) {
				gamma_val_out->has_value = TPBRT_FALSE;
				return TPBRT_ERROR_INVALID_TEXTURE_ENCODING;
			}

			if (token.data - encoding_str.data >= GAMMA_STR.size) {
				gamma_val_out->has_value = TPBRT_FALSE;
				return TPBRT_ERROR_INVALID_TEXTURE_ENCODING;
			}

		token.data += GAMMA_STR.size;


			if ((token = tpbrt_next_value(token.data, encoding_str.data + encoding_str.size)).data == TPBRT_NULL) {
				gamma_val_out->has_value = TPBRT_FALSE;
				return TPBRT_ERROR_PARSE_FLOAT;
			}

			if (!tpbrt_read_float_value(token, &gamma_val_out->value)) {
				gamma_val_out->has_value = TPBRT_FALSE;
				return TPBRT_ERROR_PARSE_FLOAT;
			}

		gamma_val_out->has_value = TPBRT_TRUE;
		*encoding_out			 = TPBRT_TEXTURE_ENCODING_GAMMA;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_texture_mapping_from_string(const tpbrt_string_t mapping_str,
	  tpbrt_texture_mapping_t* const mapping_out) {
		static const tpbrt_string_t MAPPINGS_STRS[TPBRT_TEXTURE_MAPPING_MAX_NUM] = {
			TPBRT_STRING("uv"),
			TPBRT_STRING("spherical"),
			TPBRT_STRING("cylindrical"),
			TPBRT_STRING("planar"),
		};

			if (mapping_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(mapping_str)) { return TPBRT_ERROR_INVALID_TEXTURE_MAPPING; }

			for (tpbrt_texture_mapping_t m = 0; m < TPBRT_TEXTURE_MAPPING_MAX_NUM; ++m) {
					if (tpbrt_string_equals(mapping_str, MAPPINGS_STRS[m])) {
						*mapping_out = m;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_TEXTURE_MAPPING;
	}

	static tpbrt_error_t tpbrt_texture_read_mapping(tpbrt_texture_t* const texture, const tpbrt_params_list_t params,
	  const tpbrt_mat4_animated_t* ctm) {
		tpbrt_error_t err;

			switch (texture->mapping) {
			default:
				case TPBRT_TEXTURE_MAPPING_UV: {
					static const tpbrt_string_t U_SCALE_STR = TPBRT_STRING("uscale");
					static const tpbrt_string_t V_SCALE_STR = TPBRT_STRING("vscale");
					static const tpbrt_string_t U_DELTA_STR = TPBRT_STRING("udelta");
					static const tpbrt_string_t V_DELTA_STR = TPBRT_STRING("vdelta");

					err = tpbrt_params_list_get_float(params, U_SCALE_STR, 1.0f, &texture->mapping_as.uv.u_scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, V_SCALE_STR, 1.0f, &texture->mapping_as.uv.v_scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, U_DELTA_STR, 0.0f, &texture->mapping_as.uv.u_delta);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, V_DELTA_STR, 0.0f, &texture->mapping_as.uv.v_delta);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}
					break;
				}
				case TPBRT_TEXTURE_MAPPING_SPHERICAL: {
					texture->mapping_as.spherical.transform = *ctm;
					break;
				}
				case TPBRT_TEXTURE_MAPPING_CYLINDRICAL: {
					texture->mapping_as.cylindrical.transform = *ctm;
					break;
				}
				case TPBRT_TEXTURE_MAPPING_PLANAR: {
					static const tpbrt_string_t U_DELTA_STR = TPBRT_STRING("udelta");
					static const tpbrt_string_t V_DELTA_STR = TPBRT_STRING("vdelta");
					static const tpbrt_string_t V1_STR		= TPBRT_STRING("v1");
					static const tpbrt_string_t V2_STR		= TPBRT_STRING("v2");

					err = tpbrt_params_list_get_float(params, U_DELTA_STR, 0.0f, &texture->mapping_as.planar.u_delta);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, V_DELTA_STR, 0.0f, &texture->mapping_as.planar.v_delta);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}

					err = tpbrt_params_list_get_vec(params, V1_STR, &texture->mapping_as.planar.v1);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->mapping_as.planar.v1.x = 1.0f;
							texture->mapping_as.planar.v1.y = 0.0f;
							texture->mapping_as.planar.v1.z = 0.0f;
						}

					err = tpbrt_params_list_get_vec(params, V2_STR, &texture->mapping_as.planar.v2);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->mapping_as.planar.v2.x = 0.0f;
							texture->mapping_as.planar.v2.y = 1.0f;
							texture->mapping_as.planar.v2.z = 0.0f;
						}
					break;
				}
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_texture_init(const tpbrt_string_t name, const tpbrt_string_t type_str, const tpbrt_string_t class_str,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures, const tpbrt_mat4_animated_t* ctm,
	  tpbrt_texture_t* const texture) {
			if (ctm == TPBRT_NULL || texture == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(texture, 0, sizeof(tpbrt_texture_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		texture->idx	  = TPBRT_TEXTURE_IDX_INVALID;

		tpbrt_error_t err = tpbrt_string_copy(&texture->name, name);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_texture_free(texture);
				return err;
			}

		err = tpbrt_texture_type_from_string(type_str, &texture->type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_texture_free(texture);
				return err;
			}

		err = tpbrt_texture_class_from_string(class_str, &texture->tex_class);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_texture_free(texture);
				return err;
			}

		static const tpbrt_string_t MAPPING_STR = TPBRT_STRING("mapping");

		tpbrt_string_t mapping_str;
		err = tpbrt_params_list_get_string(params, MAPPING_STR, &mapping_str);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
				tpbrt_texture_free(texture);
				return err;
			}

			if (err == TPBRT_ERROR_NOT_FOUND) { texture->mapping = TPBRT_TEXTURE_MAPPING_UV; }
			else {
				err = tpbrt_texture_mapping_from_string(mapping_str, &texture->mapping);
				tpbrt_string_free(&mapping_str);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_texture_free(texture);
						return err;
					}
			}

		err = tpbrt_texture_read_mapping(texture, params, ctm);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_texture_free(texture);
				return err;
			}

			// class
			switch (texture->tex_class) {
			default:
				case TPBRT_TEXTURE_CLASS_BILERP: {
					static const tpbrt_string_t V00_STR = TPBRT_STRING("v00");
					static const tpbrt_string_t V01_STR = TPBRT_STRING("v01");
					static const tpbrt_string_t V10_STR = TPBRT_STRING("v10");
					static const tpbrt_string_t V11_STR = TPBRT_STRING("v11");

					err = tpbrt_params_list_get_texture(params, V00_STR, textures, &texture->as.bilinear.v00);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.bilinear.v00.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.bilinear.v00.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_texture(params, V01_STR, textures, &texture->as.bilinear.v01);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.bilinear.v01.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.bilinear.v01.as.float_value = 1.0f;
						}

					err = tpbrt_params_list_get_texture(params, V10_STR, textures, &texture->as.bilinear.v10);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.bilinear.v10.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.bilinear.v10.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_texture(params, V11_STR, textures, &texture->as.bilinear.v11);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.bilinear.v11.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.bilinear.v11.as.float_value = 1.0f;
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_CHECKER_BOARD: {
					static const tpbrt_string_t DIMENSION_STR = TPBRT_STRING("dimension");
					static const tpbrt_string_t TEX1_STR	  = TPBRT_STRING("tex1");
					static const tpbrt_string_t TEX2_STR	  = TPBRT_STRING("tex2");

					err = tpbrt_params_list_get_uint(params, DIMENSION_STR, 2u, &texture->as.checkboard.dimension);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}

					err = tpbrt_params_list_get_texture(params, TEX1_STR, textures, &texture->as.checkboard.tex1);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.checkboard.tex1.value_type	   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.checkboard.tex1.as.float_value = 1.0f;
						}

					err = tpbrt_params_list_get_texture(params, TEX2_STR, textures, &texture->as.checkboard.tex2);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.checkboard.tex2.value_type	   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.checkboard.tex2.as.float_value = 0.0f;
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_CONSTANT: {
					static const tpbrt_string_t VALUE_STR = TPBRT_STRING("value");

					err = tpbrt_params_list_get_texture(params, VALUE_STR, textures, &texture->as.constant.value);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.constant.value.value_type	  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.constant.value.as.float_value = 1.0f;
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_DIRECTION_MIX: {
					static const tpbrt_string_t TEX1_STR = TPBRT_STRING("tex1");
					static const tpbrt_string_t TEX2_STR = TPBRT_STRING("tex2");
					static const tpbrt_string_t DIR_STR	 = TPBRT_STRING("dir");

					err = tpbrt_params_list_get_texture(params, TEX1_STR, textures, &texture->as.direction_mix.tex1);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.direction_mix.tex1.value_type	  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.direction_mix.tex1.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_texture(params, TEX2_STR, textures, &texture->as.direction_mix.tex2);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.direction_mix.tex2.value_type	  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.direction_mix.tex2.as.float_value = 1.0f;
						}

					err = tpbrt_params_list_get_vec(params, DIR_STR, &texture->as.direction_mix.dir);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.direction_mix.dir.x = 0.0f;
							texture->as.direction_mix.dir.y = 1.0f;
							texture->as.direction_mix.dir.z = 0.0f;
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_DOTS: {
					static const tpbrt_string_t INSIDE_STR	= TPBRT_STRING("inside");
					static const tpbrt_string_t OUTSIDE_STR = TPBRT_STRING("outside");

					err = tpbrt_params_list_get_texture(params, INSIDE_STR, textures, &texture->as.dot.inside);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.dot.inside.value_type	  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.dot.inside.as.float_value = 1.0f;
						}

					err = tpbrt_params_list_get_texture(params, OUTSIDE_STR, textures, &texture->as.dot.outside);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.dot.outside.value_type	   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.dot.outside.as.float_value = 0.0f;
						}
					break;
				}
			case TPBRT_TEXTURE_CLASS_FBM:
			case TPBRT_TEXTURE_CLASS_WINDY:
				case TPBRT_TEXTURE_CLASS_WRINKLED: {
					static const tpbrt_string_t OCTAVES_STR	  = TPBRT_STRING("octaves");
					static const tpbrt_string_t ROUGHNESS_STR = TPBRT_STRING("roughness");

					texture->as.fbm_wrinkled_windy.transform  = *ctm;

					static const tpbrt_uint_t OCTAVES_DEFAULT = 8u;
					err =
					  tpbrt_params_list_get_uint(params, OCTAVES_STR, OCTAVES_DEFAULT, &texture->as.fbm_wrinkled_windy.octaves);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}

					static const tpbrt_float_t ROUGHNESS_DEFAULT = 0.5f;
					err = tpbrt_params_list_get_float(params, ROUGHNESS_STR, ROUGHNESS_DEFAULT,
					  &texture->as.fbm_wrinkled_windy.roughness);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_IMAGE_MAP: {
					static const tpbrt_string_t FILE_NAME_STR	   = TPBRT_STRING("filename");
					static const tpbrt_string_t WRAP_STR		   = TPBRT_STRING("wrap");
					static const tpbrt_string_t MAX_ANISOTROPY_STR = TPBRT_STRING("maxanisotropy");
					static const tpbrt_string_t FILTER_STR		   = TPBRT_STRING("filter");
					static const tpbrt_string_t ENCODING_STR	   = TPBRT_STRING("encoding");
					static const tpbrt_string_t SCALE_STR		   = TPBRT_STRING("scale");
					static const tpbrt_string_t INVERT_STR		   = TPBRT_STRING("invert");

					err = tpbrt_params_list_get_string(params, FILE_NAME_STR, &texture->as.image_map.file_name);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}

					tpbrt_string_t temp_str;
					err = tpbrt_params_list_get_string(params, WRAP_STR, &temp_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) { texture->as.image_map.wrap = TPBRT_TEXTURE_WRAP_REPEAT; }
						else {
							err = tpbrt_texture_wrap_from_string(temp_str, &texture->as.image_map.wrap);
							tpbrt_string_free(&temp_str);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_texture_free(texture);
									return err;
								}
						}

					static const tpbrt_float_t MAX_ANISOTROPY_DEFAULT = 8.0f;
					err = tpbrt_params_list_get_float(params, MAX_ANISOTROPY_STR, MAX_ANISOTROPY_DEFAULT,
					  &texture->as.image_map.max_anisotropy);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}

					err = tpbrt_params_list_get_string(params, FILTER_STR, &temp_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) { texture->as.image_map.filter = TPBRT_TEXTURE_FILTER_BILINEAR; }
						else {
							err = tpbrt_texture_filter_from_string(temp_str, &texture->as.image_map.filter);
							tpbrt_string_free(&temp_str);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_texture_free(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, ENCODING_STR, &temp_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) { texture->as.image_map.encoding = TPBRT_TEXTURE_ENCODING_SRGB; }
						else {
							err = tpbrt_texture_encoding_from_string(temp_str, &texture->as.image_map.encoding,
							  &texture->as.image_map.gamma);
							tpbrt_string_free(&temp_str);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_texture_free(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_float(params, SCALE_STR, 1.0f, &texture->as.image_map.scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}

					err = tpbrt_params_list_get_bool(params, INVERT_STR, TPBRT_FALSE, &texture->as.image_map.invert);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_MARBLE: {
					static const tpbrt_string_t OCTAVES_STR	  = TPBRT_STRING("octaves");
					static const tpbrt_string_t ROUGHNESS_STR = TPBRT_STRING("roughness");
					static const tpbrt_string_t SCALE_STR	  = TPBRT_STRING("scale");
					static const tpbrt_string_t VARIATION_STR = TPBRT_STRING("variation");

					static const tpbrt_uint_t OCTAVES_DEFAULT = 8u;
					err = tpbrt_params_list_get_uint(params, OCTAVES_STR, OCTAVES_DEFAULT, &texture->as.marble.octaves);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}

					static const tpbrt_float_t ROUGHNESS_DEFAULT = 0.5f;
					err = tpbrt_params_list_get_float(params, ROUGHNESS_STR, ROUGHNESS_DEFAULT, &texture->as.marble.roughness);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, SCALE_STR, 1.0f, &texture->as.marble.scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}

					static const tpbrt_float_t VARIATION_DEFAULT = 0.2f;
					err = tpbrt_params_list_get_float(params, VARIATION_STR, VARIATION_DEFAULT, &texture->as.marble.variation);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_MIX: {
					static const tpbrt_string_t TEX1_STR   = TPBRT_STRING("tex1");
					static const tpbrt_string_t TEX2_STR   = TPBRT_STRING("tex2");
					static const tpbrt_string_t AMOUNT_STR = TPBRT_STRING("amount");

					err = tpbrt_params_list_get_texture(params, TEX1_STR, textures, &texture->as.mix.tex1);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.mix.tex1.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.mix.tex1.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_texture(params, TEX2_STR, textures, &texture->as.mix.tex2);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.mix.tex2.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.mix.tex2.as.float_value = 1.0f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, AMOUNT_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &texture->as.mix.amount);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.mix.amount.value_type	  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.mix.amount.as.float_value = 0.5f;
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_PTEX: {
					static const tpbrt_string_t ENCODING_STR  = TPBRT_STRING("encoding");
					static const tpbrt_string_t FILE_NAME_STR = TPBRT_STRING("filename");
					static const tpbrt_string_t SCALE_STR	  = TPBRT_STRING("scale");

					tpbrt_string_t temp_str;
					err = tpbrt_params_list_get_string(params, ENCODING_STR, &temp_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							static const tpbrt_float_t GAMMA_DEFAULT = 2.2f;

							texture->as.ptex.encoding				 = TPBRT_TEXTURE_ENCODING_GAMMA;
							texture->as.ptex.gamma.has_value		 = TPBRT_TRUE;
							texture->as.ptex.gamma.value			 = GAMMA_DEFAULT;
						}
						else {
							err =
							  tpbrt_texture_encoding_from_string(temp_str, &texture->as.ptex.encoding, &texture->as.ptex.gamma);
							tpbrt_string_free(&temp_str);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_texture_free(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, FILE_NAME_STR, &texture->as.ptex.file_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, SCALE_STR, 1.0f, &texture->as.ptex.scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_texture_free(texture);
							return err;
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_SCALE: {
					static const tpbrt_string_t TEX_STR	  = TPBRT_STRING("tex");
					static const tpbrt_string_t SCALE_STR = TPBRT_STRING("scale");

					err = tpbrt_params_list_get_texture(params, TEX_STR, textures, &texture->as.scale.tex);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.scale.tex.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.scale.tex.as.float_value = 1.0f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, SCALE_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &texture->as.scale.scale);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_texture_free(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.scale.scale.value_type	   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.scale.scale.as.float_value = 1.0f;
						}
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_texture_free(tpbrt_texture_t* const texture) {
			if (texture == TPBRT_NULL) { return; }

			switch (texture->tex_class) {
				case TPBRT_TEXTURE_CLASS_IMAGE_MAP: {
					tpbrt_string_free(&texture->as.image_map.file_name);
					break;
				}
				case TPBRT_TEXTURE_CLASS_PTEX: {
					tpbrt_string_free(&texture->as.ptex.file_name);
					break;
				}
			default: break;
			}
	}

#pragma endregion

#pragma region TEXTURES_LIST
#define TPBRT_EMPTY_TEXTURES_LIST (tpbrt_textures_list_t) { .textures = TPBRT_NULL, .count = 0 }

	tpbrt_error_t tpbrt_textures_list_init(tpbrt_textures_list_t* const textures_list) {
			if (textures_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*textures_list = TPBRT_EMPTY_TEXTURES_LIST;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_textures_list_append(tpbrt_textures_list_t* const textures_list, const tpbrt_texture_t* const texture) {
			if (texture == TPBRT_NULL || textures_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_size_t new_count = textures_list->count + 1;

		tpbrt_texture_t* new_list	 = realloc(textures_list->textures, sizeof(tpbrt_texture_t) * new_count);
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		new_list[textures_list->count]	   = *texture;
		new_list[textures_list->count].idx = textures_list->count;
		*textures_list					   = (tpbrt_textures_list_t) { .textures = new_list, .count = new_count };
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_textures_list_get(const tpbrt_textures_list_t textures_list, const tpbrt_string_t texture_name,
	  const tpbrt_texture_t** const texture) {
			if (texture == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			for (tpbrt_size_t i = 0; i < textures_list.count; i++) {
					if (tpbrt_string_equals(textures_list.textures[i].name, texture_name)) {
						*texture = &textures_list.textures[i];
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_NOT_FOUND;
	}

	tpbrt_error_t tpbrt_textures_list_get_handle(const tpbrt_textures_list_t textures_list, const tpbrt_string_t texture_name,
	  tpbrt_texture_handle_t* const texture_handle) {
			if (texture_handle == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_texture_t* texture;
		const tpbrt_error_t err = tpbrt_textures_list_get(textures_list, texture_name, &texture);
			if (err != TPBRT_ERROR_NONE) {
				texture_handle->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE;
				return err;
			}

		texture_handle->value_type	   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE;
		texture_handle->as.texture_idx = texture->idx;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_textures_list_get_handle_with_type(const tpbrt_textures_list_t textures_list,
	  const tpbrt_string_t texture_name, const tpbrt_texture_type_t type, tpbrt_texture_handle_t* const texture_handle) {
			if (texture_handle == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_texture_t* texture;
		const tpbrt_error_t err = tpbrt_textures_list_get(textures_list, texture_name, &texture);
			if (err != TPBRT_ERROR_NONE) {
				texture_handle->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE;
				return err;
			}

			if (texture->type != type) {
				texture_handle->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE;
				return TPBRT_ERROR_WRONG_TEXTURE_TYPE;
			}

		texture_handle->value_type	   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE;
		texture_handle->as.texture_idx = texture->idx;
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_textures_list_free(tpbrt_textures_list_t* const textures_list) {
			if (textures_list == TPBRT_NULL) { return; }

			if (textures_list->textures != TPBRT_NULL) {
					for (tpbrt_size_t i = 0; i < textures_list->count; ++i) { tpbrt_texture_free(textures_list->textures + i); }

				free(textures_list->textures);
			}

		*textures_list = TPBRT_EMPTY_TEXTURES_LIST;
	}

	tpbrt_bool_t tpbrt_textures_list_empty(const tpbrt_textures_list_t textures_list) {
		return textures_list.textures == TPBRT_NULL || textures_list.count == 0 ? TPBRT_TRUE : TPBRT_FALSE;
	}

#pragma endregion

#pragma region API

	tpbrt_error_t tpbrt_get_texture_by_name(const tpbrt_textures_list_t textures, const tpbrt_string_t name,
	  const tpbrt_texture_t** const texture) {
		return tpbrt_textures_list_get(textures, name, texture);
	}

	tpbrt_error_t tpbrt_get_texture_by_handle(const tpbrt_textures_list_t textures, const tpbrt_texture_handle_t handle,
	  const tpbrt_texture_t** const texture) {
			if (texture == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (handle.value_type != TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE) {
				*texture = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_HANDLE;
			}

			if (handle.as.texture_idx >= textures.count) {
				*texture = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_HANDLE;
			}

		*texture = &textures.textures[handle.as.texture_idx];
		return TPBRT_ERROR_NONE;
	}

#pragma endregion
#ifdef __cplusplus
}
#endif