#include <tinypbrt/pch.h>

#include <tinypbrt/detail/texture_internal.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	static tpbrt_error_t tpbrt_texture_type_from_string(const tpbrt_string_t* const type_str, tpbrt_texture_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_TEXTURE_TYPE_MAX_NUM] = {
			TPBRT_STRING("float"),
			TPBRT_STRING("spectrum"),
		};

			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_texture_type_t t = 0; t < TPBRT_TEXTURE_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS + t)) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_TEXTURE_TYPE;
	}

	static tpbrt_error_t tpbrt_texture_class_from_string(const tpbrt_string_t* const class_str,
	  tpbrt_texture_class_t* const class) {
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

			if (class_str == TPBRT_NULL || class_str->data == TPBRT_NULL || class == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_texture_class_t c = 0; c < TPBRT_TEXTURE_CLASS_MAX_NUM; ++c) {
					if (tpbrt_string_equals(class_str, CLASSES_STRS + c)) {
						*class = c;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_TEXTURE_CLASS;
	}

	static tpbrt_error_t tpbrt_texture_wrap_from_string(const tpbrt_string_t* const wrap_str, tpbrt_texture_wrap_t* const wrap) {
		static const tpbrt_string_t WRAPS_STRS[TPBRT_TEXTURE_WRAP_MAX_NUM] = {
			TPBRT_STRING("repeat"),
			TPBRT_STRING("black"),
			TPBRT_STRING("clamp"),
		};

			if (wrap_str == TPBRT_NULL || wrap_str->data == TPBRT_NULL || wrap == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_texture_wrap_t w = 0; w < TPBRT_TEXTURE_WRAP_MAX_NUM; ++w) {
					if (tpbrt_string_equals(wrap_str, WRAPS_STRS + w)) {
						*wrap = w;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_TEXTURE_WRAP;
	}

	static tpbrt_error_t tpbrt_texture_filter_from_string(const tpbrt_string_t* const filter_str,
	  tpbrt_texture_filter_t* const filter) {
		static const tpbrt_string_t FILTERS_STRS[TPBRT_TEXTURE_FILTER_MAX_NUM] = {
			TPBRT_STRING("bilinear"),
			TPBRT_STRING("ewa"),
			TPBRT_STRING("trilinear"),
			TPBRT_STRING("point"),
		};

			if (filter_str == TPBRT_NULL || filter_str->data == TPBRT_NULL || filter == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_texture_filter_t f = 0; f < TPBRT_TEXTURE_FILTER_MAX_NUM; ++f) {
					if (tpbrt_string_equals(filter_str, FILTERS_STRS + f)) {
						*filter = f;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_TEXTURE_FILTER;
	}

	static tpbrt_error_t tpbrt_texture_encoding_from_string(const tpbrt_string_t* const encoding_str,
	  tpbrt_texture_encoding_t* const encoding, tpbrt_opt_float_t* const gamma_val) {
		static const tpbrt_string_t ENCODINGS_STRS[TPBRT_TEXTURE_ENCODING_MAX_NUM - 1] = {
			TPBRT_STRING("sRGB"),
			TPBRT_STRING("linear"),
		};
		static const tpbrt_string_t GAMMA_STR = TPBRT_STRING("gamma");

			if (encoding_str == TPBRT_NULL || encoding_str->data == TPBRT_NULL || encoding == TPBRT_NULL ||
				gamma_val == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_texture_encoding_t e = 0; e < TPBRT_TEXTURE_ENCODING_MAX_NUM - 1; ++e) {
					if (tpbrt_string_equals(encoding_str, ENCODINGS_STRS + e)) {
						*encoding			 = e;
						gamma_val->has_value = TPBRT_FALSE;
						return TPBRT_ERROR_NONE;
					}
			}

		const tpbrt_char_t* curr;
			if ((curr = strstr(encoding_str->data, GAMMA_STR.data)) == TPBRT_NULL) {
				gamma_val->has_value = TPBRT_FALSE;
				return TPBRT_ERROR_UNKNOWN_TEXTURE_ENCODING;
			}

			if (curr - encoding_str->data >= GAMMA_STR.size) {
				gamma_val->has_value = TPBRT_FALSE;
				return TPBRT_ERROR_UNKNOWN_TEXTURE_ENCODING;
			}

		curr += GAMMA_STR.size;

		tpbrt_size_t len;
		curr = tpbrt_next_token(curr, encoding_str->data + encoding_str->size, &len);
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
			TPBRT_STRING("uv"),
			TPBRT_STRING("spherical"),
			TPBRT_STRING("cylindrical"),
			TPBRT_STRING("planar"),
		};

			if (mapping_str == TPBRT_NULL || mapping_str->data == TPBRT_NULL || mapping == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_texture_mapping_t m = 0; m < TPBRT_TEXTURE_MAPPING_MAX_NUM; ++m) {
					if (tpbrt_string_equals(mapping_str, MAPPINGS_STRS + m)) {
						*mapping = m;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_TEXTURE_MAPPING;
	}

#pragma region TEXTURE

	tpbrt_error_t tpbrt_create_texture(const tpbrt_string_t* const name, const tpbrt_string_t* const type_str,
	  const tpbrt_string_t* const class_str, const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures,
	  const tpbrt_mat4_animated_t* ctm, tpbrt_texture_t* const texture) {
			if (name == TPBRT_NULL || name->data == TPBRT_NULL || type_str == TPBRT_NULL || type_str->data == TPBRT_NULL ||
				class_str == TPBRT_NULL || class_str->data == TPBRT_NULL || params == TPBRT_NULL || textures == TPBRT_NULL ||
				ctm == TPBRT_NULL || texture == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (memset(texture, 0, sizeof(tpbrt_texture_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		texture->idx	  = ~(tpbrt_size_t)0;

		tpbrt_error_t err = tpbrt_copy_string(&texture->name, name);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_texture(texture);
				return err;
			}

		err = tpbrt_texture_type_from_string(type_str, &texture->type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_texture(texture);
				return err;
			}

		err = tpbrt_texture_class_from_string(class_str, &texture->tex_class);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_texture(texture);
				return err;
			}

		static const tpbrt_string_t MAPPING_STR = TPBRT_STRING("mapping");

		tpbrt_string_t mapping_str;
		err = tpbrt_params_list_get_string(params, &MAPPING_STR, &mapping_str);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
				tpbrt_free_texture(texture);
				return err;
			}

			if (err == TPBRT_ERROR_NOT_FOUND) { texture->mapping = TPBRT_TEXTURE_MAPPING_UV; }
			else {
				err = tpbrt_texture_mapping_from_string(&mapping_str, &texture->mapping);
				tpbrt_free_string(&mapping_str);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_free_texture(texture);
						return err;
					}
			}

			// mapping
			switch (texture->mapping) {
			default:
				case TPBRT_TEXTURE_MAPPING_UV: {
					static const tpbrt_string_t U_SCALE_STR = TPBRT_STRING("uscale");
					static const tpbrt_string_t V_SCALE_STR = TPBRT_STRING("vscale");
					static const tpbrt_string_t U_DELTA_STR = TPBRT_STRING("udelta");
					static const tpbrt_string_t V_DELTA_STR = TPBRT_STRING("vdelta");

					err = tpbrt_params_list_get_float(params, &U_SCALE_STR, 1.0f, &texture->mapping_as.uv.u_scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &V_SCALE_STR, 1.0f, &texture->mapping_as.uv.v_scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &U_DELTA_STR, 0.0f, &texture->mapping_as.uv.u_delta);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &V_DELTA_STR, 0.0f, &texture->mapping_as.uv.v_delta);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}
					break;
				}
				case TPBRT_TEXTURE_MAPPING_SPHERICAL: {
					texture->mapping_as.spherical.transform = *ctm;
				}
				case TPBRT_TEXTURE_MAPPING_CYLINDRICAL: {
					texture->mapping_as.cylindrical.transform = *ctm;
				}
				case TPBRT_TEXTURE_MAPPING_PLANAR: {
					static const tpbrt_string_t U_DELTA_STR = TPBRT_STRING("udelta");
					static const tpbrt_string_t V_DELTA_STR = TPBRT_STRING("vdelta");
					static const tpbrt_string_t V1_STR		= TPBRT_STRING("v1");
					static const tpbrt_string_t V2_STR		= TPBRT_STRING("v2");

					err = tpbrt_params_list_get_float(params, &U_DELTA_STR, 0.0f, &texture->mapping_as.planar.u_delta);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &V_DELTA_STR, 0.0f, &texture->mapping_as.planar.v_delta);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_vec(params, &V1_STR, &texture->mapping_as.planar.v1);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->mapping_as.planar.v1.x = 1.0f;
							texture->mapping_as.planar.v1.y = 0.0f;
							texture->mapping_as.planar.v1.z = 0.0f;
						}

					err = tpbrt_params_list_get_vec(params, &V2_STR, &texture->mapping_as.planar.v2);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
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

			// class
			switch (texture->tex_class) {
			default:
				case TPBRT_TEXTURE_CLASS_BILERP: {
					static const tpbrt_string_t V00_STR = TPBRT_STRING("v00");
					static const tpbrt_string_t V01_STR = TPBRT_STRING("v01");
					static const tpbrt_string_t V10_STR = TPBRT_STRING("v10");
					static const tpbrt_string_t V11_STR = TPBRT_STRING("v11");

					tpbrt_string_t texture_name;
					err = tpbrt_params_list_get_string(params, &V00_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.bilinear.v00.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.bilinear.v00.as.f32		= 0.0f;
						}
						else {
							err =
							  tpbrt_textures_list_get_texture_handle(textures, &texture_name, 0.0f, &texture->as.bilinear.v00);
							tpbrt_free_string(&texture_name);
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
							texture->as.bilinear.v01.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.bilinear.v01.as.f32		= 1.0f;
						}
						else {
							err =
							  tpbrt_textures_list_get_texture_handle(textures, &texture_name, 1.0f, &texture->as.bilinear.v01);
							tpbrt_free_string(&texture_name);
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
							texture->as.bilinear.v10.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.bilinear.v10.as.f32		= 0.0f;
						}
						else {
							err =
							  tpbrt_textures_list_get_texture_handle(textures, &texture_name, 0.0f, &texture->as.bilinear.v10);
							tpbrt_free_string(&texture_name);
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
							texture->as.bilinear.v11.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.bilinear.v11.as.f32		= 1.0f;
						}
						else {
							err =
							  tpbrt_textures_list_get_texture_handle(textures, &texture_name, 1.0f, &texture->as.bilinear.v11);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_CHECKER_BOARD: {
					static const tpbrt_string_t DIMENSION_STR = TPBRT_STRING("dimension");
					static const tpbrt_string_t TEX1_STR	  = TPBRT_STRING("tex1");
					static const tpbrt_string_t TEX2_STR	  = TPBRT_STRING("tex2");

					err = tpbrt_params_list_get_uint(params, &DIMENSION_STR, 2u, &texture->as.checkboard.dimension);
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
							texture->as.checkboard.tex1.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.checkboard.tex1.as.f32	   = 1.0f;
						}
						else {
							err =
							  tpbrt_textures_list_get_texture_handle(textures, &texture_name, 1.0f, &texture->as.checkboard.tex1);
							tpbrt_free_string(&texture_name);
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
							texture->as.checkboard.tex2.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.checkboard.tex2.as.f32	   = 0.0f;
						}
						else {
							err =
							  tpbrt_textures_list_get_texture_handle(textures, &texture_name, 0.0f, &texture->as.checkboard.tex2);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_CONSTANT: {
					static const tpbrt_string_t VALUE_STR = TPBRT_STRING("value");

					tpbrt_string_t texture_name;
					err = tpbrt_params_list_get_string(params, &VALUE_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.constant.value.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.constant.value.as.f32	  = 1.0f;
						}
						else {
							err =
							  tpbrt_textures_list_get_texture_handle(textures, &texture_name, 1.0f, &texture->as.constant.value);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_DIRECTION_MIX: {
					static const tpbrt_string_t TEX1_STR = TPBRT_STRING("tex1");
					static const tpbrt_string_t TEX2_STR = TPBRT_STRING("tex2");
					static const tpbrt_string_t DIR_STR	 = TPBRT_STRING("dir");

					tpbrt_string_t texture_name;
					err = tpbrt_params_list_get_string(params, &TEX1_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.direction_mix.tex1.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.direction_mix.tex1.as.f32	  = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 0.0f,
							  &texture->as.direction_mix.tex1);
							tpbrt_free_string(&texture_name);
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
							texture->as.direction_mix.tex2.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.direction_mix.tex2.as.f32	  = 1.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 1.0f,
							  &texture->as.direction_mix.tex2);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_vec(params, &DIR_STR, &texture->as.direction_mix.dir);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
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

					tpbrt_string_t texture_name;
					err = tpbrt_params_list_get_string(params, &INSIDE_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.dot.inside.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.dot.inside.as.f32	  = 1.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 1.0f, &texture->as.dot.inside);
							tpbrt_free_string(&texture_name);
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
							texture->as.dot.outside.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.dot.outside.as.f32	   = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 0.0f, &texture->as.dot.outside);
							tpbrt_free_string(&texture_name);
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
					static const tpbrt_string_t OCTAVES_STR	  = TPBRT_STRING("octaves");
					static const tpbrt_string_t ROUGHNESS_STR = TPBRT_STRING("roughness");

					texture->as.fbm_wrinkled_windy.transform  = *ctm;

					static const tpbrt_uint_t OCTAVES_DEFAULT = 8u;
					err =
					  tpbrt_params_list_get_uint(params, &OCTAVES_STR, OCTAVES_DEFAULT, &texture->as.fbm_wrinkled_windy.octaves);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					static const tpbrt_float_t ROUGHNESS_DEFAULT = 0.5f;
					err = tpbrt_params_list_get_float(params, &ROUGHNESS_STR, ROUGHNESS_DEFAULT,
					  &texture->as.fbm_wrinkled_windy.roughness);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
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

					err = tpbrt_params_list_get_string(params, &FILE_NAME_STR, &texture->as.image_map.file_name);
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

						if (err == TPBRT_ERROR_NOT_FOUND) { texture->as.image_map.wrap = TPBRT_TEXTURE_WRAP_REPEAT; }
						else {
							err = tpbrt_texture_wrap_from_string(&temp_str, &texture->as.image_map.wrap);
							tpbrt_free_string(&temp_str);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					static const tpbrt_float_t MAX_ANISOTROPY_DEFAULT = 8.0f;
					err = tpbrt_params_list_get_float(params, &MAX_ANISOTROPY_STR, MAX_ANISOTROPY_DEFAULT,
					  &texture->as.image_map.max_anisotropy);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_string(params, &FILTER_STR, &temp_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) { texture->as.image_map.filter = TPBRT_TEXTURE_FILTER_BILINEAR; }
						else {
							err = tpbrt_texture_filter_from_string(&temp_str, &texture->as.image_map.filter);
							tpbrt_free_string(&temp_str);
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

						if (err == TPBRT_ERROR_NOT_FOUND) { texture->as.image_map.encoding = TPBRT_TEXTURE_ENCODING_SRGB; }
						else {
							err = tpbrt_texture_encoding_from_string(&temp_str, &texture->as.image_map.encoding,
							  &texture->as.image_map.gamma);
							tpbrt_free_string(&temp_str);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_float(params, &SCALE_STR, 1.0f, &texture->as.image_map.scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_bool(params, &INVERT_STR, TPBRT_FALSE, &texture->as.image_map.invert);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
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
					err = tpbrt_params_list_get_uint(params, &OCTAVES_STR, OCTAVES_DEFAULT, &texture->as.marble.octaves);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					static const tpbrt_float_t ROUGHNESS_DEFAULT = 0.5f;
					err = tpbrt_params_list_get_float(params, &ROUGHNESS_STR, ROUGHNESS_DEFAULT, &texture->as.marble.roughness);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &SCALE_STR, 1.0f, &texture->as.marble.scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}

					static const tpbrt_float_t VARIATION_DEFAULT = 0.2f;
					err = tpbrt_params_list_get_float(params, &VARIATION_STR, VARIATION_DEFAULT, &texture->as.marble.variation);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_MIX: {
					static const tpbrt_string_t TEX1_STR   = TPBRT_STRING("tex1");
					static const tpbrt_string_t TEX2_STR   = TPBRT_STRING("tex2");
					static const tpbrt_string_t AMOUNT_STR = TPBRT_STRING("amount");

					tpbrt_string_t texture_name;
					err = tpbrt_params_list_get_string(params, &TEX1_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.mix.tex1.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.mix.tex1.as.f32		= 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 0.0f, &texture->as.mix.tex1);
							tpbrt_free_string(&texture_name);
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
							texture->as.mix.tex2.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.mix.tex2.as.f32		= 1.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 1.0f, &texture->as.mix.tex2);
							tpbrt_free_string(&texture_name);
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
							texture->as.mix.amount.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.mix.amount.as.f32	  = AMOUNT_DEFAULT;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, AMOUNT_DEFAULT, &texture->as.mix.amount);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_PTEX: {
					static const tpbrt_string_t ENCODING_STR  = TPBRT_STRING("encoding");
					static const tpbrt_string_t FILE_NAME_STR = TPBRT_STRING("filename");
					static const tpbrt_string_t SCALE_STR	  = TPBRT_STRING("scale");

					tpbrt_string_t temp_str;
					err = tpbrt_params_list_get_string(params, &ENCODING_STR, &temp_str);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							static const tpbrt_float_t GAMMA_DEFAULT = 2.2f;

							texture->as.ptex.encoding				 = TPBRT_TEXTURE_ENCODING_GAMMA_VAL;
							texture->as.ptex.gamma.has_value		 = TPBRT_TRUE;
							texture->as.ptex.gamma.value			 = GAMMA_DEFAULT;
						}
						else {
							err =
							  tpbrt_texture_encoding_from_string(&temp_str, &texture->as.ptex.encoding, &texture->as.ptex.gamma);
							tpbrt_free_string(&temp_str);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_texture(texture);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &FILE_NAME_STR, &texture->as.ptex.file_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &SCALE_STR, 1.0f, &texture->as.ptex.scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_texture(texture);
							return err;
						}
					break;
				}
				case TPBRT_TEXTURE_CLASS_SCALE: {
					static const tpbrt_string_t TEX_STR	  = TPBRT_STRING("tex");
					static const tpbrt_string_t SCALE_STR = TPBRT_STRING("scale");

					tpbrt_string_t texture_name;
					err = tpbrt_params_list_get_string(params, &TEX_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_texture(texture);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							texture->as.scale.tex.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.scale.tex.as.f32	 = 1.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle(textures, &texture_name, 1.0f, &texture->as.scale.tex);
							tpbrt_free_string(&texture_name);
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
							texture->as.scale.scale.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							texture->as.scale.scale.as.f32	   = 1.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 1.0f, &texture->as.scale.scale);
							tpbrt_free_string(&texture_name);
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

	void tpbrt_free_texture(tpbrt_texture_t* const texture) {
			if (texture == TPBRT_NULL) { return; }

			switch (texture->tex_class) {
				case TPBRT_TEXTURE_CLASS_IMAGE_MAP: {
					tpbrt_free_string(&texture->as.image_map.file_name);
				}
				case TPBRT_TEXTURE_CLASS_PTEX: {
					tpbrt_free_string(&texture->as.ptex.file_name);
				}
			default: break;
			}
	}

#pragma endregion

#pragma region TEXTURES_LIST

	tpbrt_error_t tpbrt_init_textures_list(tpbrt_textures_list_t* const textures_list) {
			if (textures_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		textures_list->textures = TPBRT_NULL;
		textures_list->count	= 0;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_textures_list_add_texture(tpbrt_textures_list_t* const textures_list,
	  const tpbrt_texture_t* const texture) {
			if (texture == TPBRT_NULL || textures_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_size_t new_count = textures_list->count + 1;

		tpbrt_texture_t* new_list	 = realloc(textures_list->textures, sizeof(tpbrt_texture_t) * new_count);
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		textures_list->textures							  = new_list;
		textures_list->textures[textures_list->count]	  = *texture;
		textures_list->textures[textures_list->count].idx = new_count;
		textures_list->count							  = new_count;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_textures_list_get_texture_by_name(const tpbrt_textures_list_t* const textures_list,
	  const tpbrt_string_t* const texture_name, const tpbrt_texture_t** const texture) {
			if (textures_list == TPBRT_NULL || texture_name == TPBRT_NULL || texture_name->data == TPBRT_NULL ||
				texture == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_size_t i = 0; i < textures_list->count; i++) {
					if (tpbrt_string_equals(&textures_list->textures[i].name, texture_name)) {
						*texture = &textures_list->textures[i];
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_NOT_FOUND;
	}

	tpbrt_error_t tpbrt_textures_list_get_texture_handle(const tpbrt_textures_list_t* const textures_list,
	  const tpbrt_string_t* const texture_name, const tpbrt_float_t default_val, tpbrt_texture_handle_t* const texture_handle) {
			if (textures_list == TPBRT_NULL || texture_name == TPBRT_NULL || texture_name->data == TPBRT_NULL ||
				texture_handle == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_texture_t* texture;
		const tpbrt_error_t err = tpbrt_textures_list_get_texture_by_name(textures_list, texture_name, &texture);
			if (err == TPBRT_ERROR_NOT_FOUND) {
				texture_handle->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
				texture_handle->as.f32	   = default_val;
				return TPBRT_ERROR_NONE;
			}

			if (err != TPBRT_ERROR_NONE) { return err; }

		texture_handle->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE;
		texture_handle->as.tex_idx = texture->idx;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_textures_list_get_opt_texture_handle_of_type(const tpbrt_textures_list_t* const textures_list,
	  const tpbrt_string_t* const texture_name, const tpbrt_texture_type_t type, tpbrt_texture_handle_t* const texture_handle) {
			if (textures_list == TPBRT_NULL || texture_name == TPBRT_NULL || texture_name->data == TPBRT_NULL ||
				texture_handle == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_texture_t* texture;
		const tpbrt_error_t err = tpbrt_textures_list_get_texture_by_name(textures_list, texture_name, &texture);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (texture->type != type) { return TPBRT_ERROR_UNKNOWN_TEXTURE_TYPE; }

		texture_handle->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE;
		texture_handle->as.tex_idx = texture->idx;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_textures_list_get_texture_handle_of_type(const tpbrt_textures_list_t* const textures_list,
	  const tpbrt_string_t* const texture_name, const tpbrt_texture_type_t type, const tpbrt_float_t default_val,
	  tpbrt_texture_handle_t* const texture_handle) {
			if (textures_list == TPBRT_NULL || texture_name == TPBRT_NULL || texture_name->data == TPBRT_NULL ||
				texture_handle == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_error_t err =
		  tpbrt_textures_list_get_opt_texture_handle_of_type(textures_list, texture_name, type, texture_handle);
			if (err == TPBRT_ERROR_NOT_FOUND) {
				texture_handle->value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
				texture_handle->as.f32	   = default_val;
				return TPBRT_ERROR_NONE;
			}

		return err;
	}

	void tpbrt_free_textures_list(tpbrt_textures_list_t* const textures_list) {
			if (textures_list == TPBRT_NULL) { return; }

			if (textures_list->textures != TPBRT_NULL) {
					for (tpbrt_size_t i = 0; i < textures_list->count; ++i) { tpbrt_free_texture(textures_list->textures + i); }

				free(textures_list->textures);
			}
	}

	tpbrt_size_t tpbrt_textures_list_size(const tpbrt_textures_list_t* const textures_list) {
		return textures_list != TPBRT_NULL ? textures_list->count : 0;
	}

	tpbrt_bool_t tpbrt_textures_list_is_empty(const tpbrt_textures_list_t* const textures_list) {
		return tpbrt_textures_list_size(textures_list) == 0;
	}

#pragma endregion

#pragma region API

	tpbrt_error_t tpbrt_get_texture_by_name(const tpbrt_textures_list_t* const textures, const tpbrt_string_t* const name,
	  const tpbrt_texture_t** const texture) {
		return tpbrt_textures_list_get_texture_by_name(textures, name, texture);
	}

	tpbrt_error_t tpbrt_get_texture_by_handle(const tpbrt_textures_list_t* const textures,
	  const tpbrt_texture_handle_t* const handle, const tpbrt_texture_t** const texture) {
			if (textures == TPBRT_NULL || handle == TPBRT_NULL || texture == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (handle->value_type != TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE) {
				*texture = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_HANDLE;
			}

			if (handle->as.tex_idx >= textures->count) {
				*texture = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_HANDLE;
			}

		*texture = &textures->textures[handle->as.tex_idx];
		return TPBRT_ERROR_NONE;
	}

#pragma endregion
#ifdef __cplusplus
}
#endif