#include <tinypbrt/pch.h>

#include <tinypbrt/detail/parser_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/param_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	void tpbrt_element_free(tpbrt_element_t* const element) {
			if (!element) { return; }

			switch (element->type) {
			case TPBRT_DIRECTIVE_FILM:
			case TPBRT_DIRECTIVE_CAMERA:
			case TPBRT_DIRECTIVE_SAMPLER:
			case TPBRT_DIRECTIVE_INTEGRATOR:
			case TPBRT_DIRECTIVE_ACCELERATOR:
			case TPBRT_DIRECTIVE_LIGHT_SOURCE:
			case TPBRT_DIRECTIVE_AREA_LIGHT_SOURCE:
			case TPBRT_DIRECTIVE_PIXEL_FILTER:
			case TPBRT_DIRECTIVE_MATERIAL:
			case TPBRT_DIRECTIVE_SHAPE:				  tpbrt_params_list_free(&element->as.generic_with_params.params); break;
			case TPBRT_DIRECTIVE_MAKE_NAMED_MEDIUM:	  tpbrt_params_list_free(&element->as.named_medium.params); break;
			case TPBRT_DIRECTIVE_ATTRIBUTE:			  tpbrt_params_list_free(&element->as.attribute.params); break;
			case TPBRT_DIRECTIVE_MAKE_NAMED_MATERIAL: tpbrt_params_list_free(&element->as.named_with_params.params); break;
			case TPBRT_DIRECTIVE_TEXTURE:			  tpbrt_params_list_free(&element->as.texture.params); break;
			default:								  break;
			}
	}

	tpbrt_error_t tpbrt_parser_init(tpbrt_parser_t* const parser, const tpbrt_string_t str) {
			if (parser == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		return tpbrt_tokenizer_init(&parser->tokenizer, str);
	}

	static tpbrt_error_t tpbrt_read_token(tpbrt_parser_t* const parser, tpbrt_token_t* const out_token) {
			if (!tpbrt_tokenizer_next(&parser->tokenizer, out_token)) { return TPBRT_ERROR_END_OF_FILE; }
			if (!tpbrt_token_is_valid(out_token)) { return TPBRT_ERROR_INVALID_TOKEN; }
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_read_string_token(tpbrt_parser_t* const parser, tpbrt_string_t* const out_str) {
		tpbrt_token_t token;
		const tpbrt_error_t err = tpbrt_read_token(parser, &token);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (tpbrt_token_is_quoted_string(&token) == TPBRT_FALSE) { return TPBRT_ERROR_INVALID_STRING; }

			if (tpbrt_read_string_value(token.value, out_str) == TPBRT_FALSE) { return TPBRT_ERROR_INVALID_STRING; }
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_read_single_token(tpbrt_parser_t* const parser, tpbrt_string_t* const out_single) {
		tpbrt_token_t token;
		const tpbrt_error_t err = tpbrt_read_token(parser, &token);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (tpbrt_token_is_single(&token) == TPBRT_FALSE) { return TPBRT_ERROR_INVALID_TOKEN; }

		*out_single = token.value;
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_read_float_token(tpbrt_parser_t* const parser, tpbrt_float_t* const out_f) {
		tpbrt_string_t value;
		const tpbrt_error_t err = tpbrt_read_single_token(parser, &value);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (!tpbrt_read_float_value(value, out_f)) { return TPBRT_ERROR_PARSE_FLOAT; }
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_read_point_token(tpbrt_parser_t* const parser, tpbrt_point_t* const out_p) {
			for (tpbrt_uint_t i = 0; i < 3; ++i) {
				const tpbrt_error_t err = tpbrt_read_float_token(parser, &out_p->v[i]);
					if (err != TPBRT_ERROR_NONE) { return err; }
			}
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_read_matrix_token(tpbrt_parser_t* const parser, tpbrt_mat4_t* const out_m) {
			for (tpbrt_uint_t i = 0; i < 16; ++i) {
				const tpbrt_error_t err = tpbrt_read_float_token(parser, &out_m->m[i]);
					if (err != TPBRT_ERROR_NONE) { return err; }
			}
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_skip_brace_token(tpbrt_parser_t* const parser, const tpbrt_bool_t expect_open) {
		tpbrt_token_t token;
		const tpbrt_error_t err = tpbrt_read_token(parser, &token);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (expect_open && !tpbrt_token_is_open_brace(&token)) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }
			if (!expect_open && !tpbrt_token_is_close_brace(&token)) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_read_param_token(tpbrt_parser_t* const parser, tpbrt_param_t* const out_param) {
		tpbrt_string_t type_and_name;
		tpbrt_error_t err = tpbrt_read_string_token(parser, &type_and_name);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_token_t value_tok;
		err = tpbrt_read_token(parser, &value_tok);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_size_t start_offset;
		tpbrt_size_t end_offset;

		tpbrt_param_value_type_t value_type = TPBRT_PARAM_VALUE_TYPE_SINGLE;

			if (tpbrt_token_is_open_brace(&value_tok)) {
				start_offset = tpbrt_tokenizer_offset(&parser->tokenizer);

					while (TPBRT_TRUE) {
						tpbrt_token_t inner;
						err = tpbrt_read_token(parser, &inner);
							if (err != TPBRT_ERROR_NONE) { return err; }

							if (tpbrt_token_is_close_brace(&inner)) {
								end_offset = (tpbrt_size_t)(inner.value.data - parser->tokenizer.str.data);
								value_type = TPBRT_PARAM_VALUE_TYPE_ARRAY;
								break;
							}
							if (tpbrt_token_is_directive(&inner)) { return TPBRT_ERROR_UNEXPECTED_TOKEN; }
					}
			}
			else {
				start_offset = (tpbrt_size_t)(value_tok.value.data - parser->tokenizer.str.data);
				end_offset	 = start_offset + value_tok.value.size;
			}

		tpbrt_string_t value_str = { .data = parser->tokenizer.str.data + start_offset, .size = end_offset - start_offset };
			if (value_type != TPBRT_PARAM_VALUE_TYPE_ARRAY && (value_str.size >= 2u && value_str.data[0] == '\"' && value_str.data[value_str.size - 1] == '\"')) {
				value_type = TPBRT_PARAM_VALUE_TYPE_STRING;
				value_str  = (tpbrt_string_t) { .data = value_str.data + 1u, .size = value_str.size - 2u };
			}

		return tpbrt_param_init(type_and_name, value_type, value_str, out_param);
	}

	static tpbrt_error_t tpbrt_read_param_list_token(tpbrt_parser_t* const parser, tpbrt_params_list_t* const out_list) {
		tpbrt_error_t err = tpbrt_params_list_init(out_list);
			if (err != TPBRT_ERROR_NONE) { return err; }

			while (TPBRT_TRUE) {
				tpbrt_token_t next_tok;
					if (!tpbrt_tokenizer_peek(&parser->tokenizer, &next_tok)) { break; }

					if (!tpbrt_token_is_quoted_string(&next_tok)) { break; }

				tpbrt_param_t param;
				err = tpbrt_read_param_token(parser, &param);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_params_list_free(out_list);
						return err;
					}

				err = tpbrt_params_list_append(out_list, &param);

					if (err != TPBRT_ERROR_NONE) {
						tpbrt_params_list_free(out_list);
						return err;
					}
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_parser_next(tpbrt_parser_t* const parser, tpbrt_element_t* const out_element) {
			if (parser == TPBRT_NULL || out_element == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_token_t token;
		tpbrt_error_t err = tpbrt_read_token(parser, &token);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (tpbrt_token_is_directive(&token) == TPBRT_FALSE) { return TPBRT_ERROR_INVALID_DIRECTIVE; }

		out_element->type = token.directive;

			switch (token.directive) {
			case TPBRT_DIRECTIVE_INCLUDE:
				case TPBRT_DIRECTIVE_IMPORT: {
					return tpbrt_read_string_token(parser, &out_element->as.include_import.path);
				}

				case TPBRT_DIRECTIVE_OPTION: {
					tpbrt_param_t p;
					err = tpbrt_read_param_token(parser, &p);
						if (err == TPBRT_ERROR_NONE) { out_element->as.option.param = p; }
					return err;
				}

			case TPBRT_DIRECTIVE_FILM:
			case TPBRT_DIRECTIVE_CAMERA:
			case TPBRT_DIRECTIVE_SAMPLER:
			case TPBRT_DIRECTIVE_INTEGRATOR:
			case TPBRT_DIRECTIVE_ACCELERATOR:
			case TPBRT_DIRECTIVE_LIGHT_SOURCE:
			case TPBRT_DIRECTIVE_AREA_LIGHT_SOURCE:
			case TPBRT_DIRECTIVE_MATERIAL:
			case TPBRT_DIRECTIVE_PIXEL_FILTER:
				case TPBRT_DIRECTIVE_SHAPE:			{
					err = tpbrt_read_string_token(parser, &out_element->as.generic_with_params.type_name);
						if (err != TPBRT_ERROR_NONE) { return err; }
					return tpbrt_read_param_list_token(parser, &out_element->as.generic_with_params.params);
				}
				case TPBRT_DIRECTIVE_MAKE_NAMED_MEDIUM: {
					err = tpbrt_read_string_token(parser, &out_element->as.named_medium.name);
						if (err != TPBRT_ERROR_NONE) { return err; }

					out_element->as.named_medium.type = TPBRT_EMPTY_STRING;

					tpbrt_token_t peek_token;
						if (tpbrt_tokenizer_peek(&parser->tokenizer, &peek_token) == TPBRT_TRUE &&
							tpbrt_token_is_quoted_string(&peek_token) == TPBRT_TRUE) {
							tpbrt_bool_t has_space = TPBRT_FALSE;
								for (tpbrt_size_t i = 0; i < peek_token.value.size; ++i) {
										if (peek_token.value.data[i] == ' ') {
											has_space = TPBRT_TRUE;
											break;
										}
								}

								if (has_space == TPBRT_FALSE) {
									err = tpbrt_read_string_token(parser, &out_element->as.named_medium.type);
										if (err != TPBRT_ERROR_NONE) { return err; }
								}
						}
					return tpbrt_read_param_list_token(parser, &out_element->as.named_medium.params);
				}

			case TPBRT_DIRECTIVE_COLOR_SPACE:
				case TPBRT_DIRECTIVE_ACTIVE_TRANSFORM: {
					return tpbrt_read_single_token(parser, &out_element->as.single_string.type_name);
				}

			case TPBRT_DIRECTIVE_COORDINATE_SYSTEM:
			case TPBRT_DIRECTIVE_COORD_SYS_TRANSFORM:
			case TPBRT_DIRECTIVE_NAMED_MATERIAL:
			case TPBRT_DIRECTIVE_OBJECT_BEGIN:
				case TPBRT_DIRECTIVE_OBJECT_INSTANCE: {
					return tpbrt_read_string_token(parser, &out_element->as.named_entity.name);
				}

			case TPBRT_DIRECTIVE_TRANSLATE:
				case TPBRT_DIRECTIVE_SCALE: {
					return tpbrt_read_point_token(parser, &out_element->as.vector3.v);
				}

				case TPBRT_DIRECTIVE_ROTATE: {
					err = tpbrt_read_float_token(parser, &out_element->as.rotate.angle);
						if (err != TPBRT_ERROR_NONE) { return err; }
					return tpbrt_read_point_token(parser, &out_element->as.rotate.v);
				}

				case TPBRT_DIRECTIVE_LOOK_AT: {
					err = tpbrt_read_point_token(parser, &out_element->as.look_at.eye);
						if (err != TPBRT_ERROR_NONE) { return err; }
					err = tpbrt_read_point_token(parser, &out_element->as.look_at.look_at);
						if (err != TPBRT_ERROR_NONE) { return err; }
					return tpbrt_read_point_token(parser, &out_element->as.look_at.up);
				}

			case TPBRT_DIRECTIVE_TRANSFORM:
				case TPBRT_DIRECTIVE_CONCAT_TRANSFORM: {
					err = tpbrt_skip_brace_token(parser, TPBRT_TRUE);
						if (err != TPBRT_ERROR_NONE) { return err; }
					err = tpbrt_read_matrix_token(parser, &out_element->as.transform.m);
						if (err != TPBRT_ERROR_NONE) { return err; }
					return tpbrt_skip_brace_token(parser, TPBRT_FALSE);
				}

				case TPBRT_DIRECTIVE_TRANSFORM_TIMES: {
					err = tpbrt_read_float_token(parser, &out_element->as.transform_times.start);
						if (err != TPBRT_ERROR_NONE) { return err; }
					return tpbrt_read_float_token(parser, &out_element->as.transform_times.end);
				}

				case TPBRT_DIRECTIVE_ATTRIBUTE: {
					err = tpbrt_read_string_token(parser, &out_element->as.attribute.target);
						if (err != TPBRT_ERROR_NONE) { return err; }
					return tpbrt_read_param_list_token(parser, &out_element->as.attribute.params);
				}

				case TPBRT_DIRECTIVE_MAKE_NAMED_MATERIAL: {
					err = tpbrt_read_string_token(parser, &out_element->as.named_with_params.name);
						if (err != TPBRT_ERROR_NONE) { return err; }
					return tpbrt_read_param_list_token(parser, &out_element->as.named_with_params.params);
				}

				case TPBRT_DIRECTIVE_TEXTURE: {
					err = tpbrt_read_string_token(parser, &out_element->as.texture.name);
						if (err != TPBRT_ERROR_NONE) { return err; }
					err = tpbrt_read_string_token(parser, &out_element->as.texture.type_name);
						if (err != TPBRT_ERROR_NONE) { return err; }
					err = tpbrt_read_string_token(parser, &out_element->as.texture.class_name);
						if (err != TPBRT_ERROR_NONE) { return err; }
					return tpbrt_read_param_list_token(parser, &out_element->as.texture.params);
				}

				case TPBRT_DIRECTIVE_MEDIUM_INTERFACE: {
					err = tpbrt_read_string_token(parser, &out_element->as.medium_interface.interior);
						if (err != TPBRT_ERROR_NONE) { return err; }
					err = tpbrt_read_string_token(parser, &out_element->as.medium_interface.exterior);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

						if (err == TPBRT_ERROR_NOT_FOUND) {
							out_element->as.medium_interface.exterior = out_element->as.medium_interface.interior;
						}
					break;
				}

			case TPBRT_DIRECTIVE_IDENTITY:
			case TPBRT_DIRECTIVE_REVERSE_ORIENTATION:
			case TPBRT_DIRECTIVE_WORLD_BEGIN:
			case TPBRT_DIRECTIVE_ATTRIBUTE_BEGIN:
			case TPBRT_DIRECTIVE_ATTRIBUTE_END:
				case TPBRT_DIRECTIVE_OBJECT_END:	  {
					return TPBRT_ERROR_NONE;
				}

				default: {
					return TPBRT_ERROR_INVALID_DIRECTIVE;
				}
			}
	}

#ifdef __cplusplus
}
#endif