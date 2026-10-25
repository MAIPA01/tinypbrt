#include <tinypbrt/pch.h>

#include <tinypbrt/detail/token_internal.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

	tpbrt_error_t tpbrt_directive_from_string(const tpbrt_string_t directive_str, tpbrt_directive_t* directive_out) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_DIRECTIVE_MAX_NUM] = {
			TPBRT_STRING_INIT("Identity"),
			TPBRT_STRING_INIT("Translate"),
			TPBRT_STRING_INIT("Scale"),
			TPBRT_STRING_INIT("Rotate"),
			TPBRT_STRING_INIT("LookAt"),
			TPBRT_STRING_INIT("CoordinateSystem"),
			TPBRT_STRING_INIT("CoordSysTransform"),
			TPBRT_STRING_INIT("Transform"),
			TPBRT_STRING_INIT("ConcatTransform"),
			TPBRT_STRING_INIT("TransformTimes"),
			TPBRT_STRING_INIT("ActiveTransform"),

			TPBRT_STRING_INIT("Include"),
			TPBRT_STRING_INIT("Import"),

			TPBRT_STRING_INIT("Option"),

			TPBRT_STRING_INIT("Camera"),
			TPBRT_STRING_INIT("Sampler"),
			TPBRT_STRING_INIT("ColorSpace"),
			TPBRT_STRING_INIT("Film"),
			TPBRT_STRING_INIT("PixelFilter"),
			TPBRT_STRING_INIT("Integrator"),
			TPBRT_STRING_INIT("Accelerator"),

			TPBRT_STRING_INIT("WorldBegin"),

			TPBRT_STRING_INIT("AttributeBegin"),
			TPBRT_STRING_INIT("AttributeEnd"),
			TPBRT_STRING_INIT("Attribute"),

			TPBRT_STRING_INIT("ReverseOrientation"),
			TPBRT_STRING_INIT("Shape"),
			TPBRT_STRING_INIT("ObjectBegin"),
			TPBRT_STRING_INIT("ObjectEnd"),
			TPBRT_STRING_INIT("ObjectInstance"),

			TPBRT_STRING_INIT("LightSource"),
			TPBRT_STRING_INIT("AreaLightSource"),

			TPBRT_STRING_INIT("Material"),
			TPBRT_STRING_INIT("MakeNamedMaterial"),
			TPBRT_STRING_INIT("NamedMaterial"),

			TPBRT_STRING_INIT("Texture"),

			TPBRT_STRING_INIT("MakeNamedMedium"),
			TPBRT_STRING_INIT("MediumInterface"),

			TPBRT_STRING_INIT("TransformBegin"),
			TPBRT_STRING_INIT("TransformEnd"),
		};

			if (directive_out == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(directive_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_DIRECTIVE; }

			for (tpbrt_directive_t d = 0; d < TPBRT_DIRECTIVE_MAX_NUM; ++d) {
					if (tpbrt_string_equals(directive_str, TYPES_STRS[d]) == TPBRT_TRUE) {
						*directive_out = d;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_DIRECTIVE;
	}

	tpbrt_error_t tpbrt_token_init(const tpbrt_string_t value, tpbrt_token_t* const token) {
			if (token == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		token->value	 = value;
		token->type		 = TPBRT_TOKEN_TYPE_SINGLE;
		token->directive = TPBRT_DIRECTIVE_MAX_NUM;

			if (value.size == 0u || value.data == TPBRT_NULL) { return TPBRT_ERROR_NONE; }

		const tpbrt_char_t first = value.data[0];

			if (first >= 'A' && first <= 'Z') {
				tpbrt_directive_t directive = TPBRT_DIRECTIVE_MAX_NUM;
					if (tpbrt_directive_from_string(value, &directive) == TPBRT_ERROR_NONE) {
						token->type		 = TPBRT_TOKEN_TYPE_DIRECTIVE;
						token->directive = directive;
					}
				return TPBRT_ERROR_NONE;
			}

			if (value.size >= 2u && first == '\"' && value.data[value.size - 1u] == '\"') {
				token->type = TPBRT_TOKEN_TYPE_QUOTED_STRING;
			}
		return TPBRT_ERROR_NONE;
	}

	tpbrt_size_t tpbrt_token_size(const tpbrt_token_t* const token) { return token != TPBRT_NULL ? token->value.size : 0u; }

	tpbrt_bool_t tpbrt_token_is_quoted_string(const tpbrt_token_t* const token) {
			if (token == TPBRT_NULL) { return TPBRT_FALSE; }
		return token->type == TPBRT_TOKEN_TYPE_QUOTED_STRING ? TPBRT_TRUE : TPBRT_FALSE;
	}

	tpbrt_bool_t tpbrt_token_is_directive(const tpbrt_token_t* const token) {
			if (token == TPBRT_NULL) { return TPBRT_FALSE; }
		return token->type == TPBRT_TOKEN_TYPE_DIRECTIVE ? TPBRT_TRUE : TPBRT_FALSE;
	}

	tpbrt_bool_t tpbrt_token_is_single(const tpbrt_token_t* const token) {
			if (token == TPBRT_NULL) { return TPBRT_FALSE; }
		return token->type == TPBRT_TOKEN_TYPE_SINGLE ? TPBRT_TRUE : TPBRT_FALSE;
	}

	tpbrt_bool_t tpbrt_token_is_open_brace(const tpbrt_token_t* const token) {
			if (!tpbrt_token_is_single(token)) { return TPBRT_FALSE; }
		return token->value.size == 1u && token->value.data[0] == '[';
	}

	tpbrt_bool_t tpbrt_token_is_close_brace(const tpbrt_token_t* const token) {
			if (!tpbrt_token_is_single(token)) { return TPBRT_FALSE; }
		return token->value.size == 1u && token->value.data[0] == ']';
	}

	tpbrt_bool_t tpbrt_token_is_valid(const tpbrt_token_t* const token) {
			if (token == TPBRT_NULL || token->value.size == 0u) { return TPBRT_FALSE; }

			if (token->value.size >= 1u) {
				const tpbrt_bool_t starts_with_quote = token->value.data[0] == '\"' ? TPBRT_TRUE : TPBRT_FALSE;
				const tpbrt_bool_t ends_with_quote = token->value.data[token->value.size - 1] == '\"' ? TPBRT_TRUE : TPBRT_FALSE;

					if (starts_with_quote == TPBRT_TRUE || ends_with_quote == TPBRT_TRUE) {
							if (starts_with_quote != ends_with_quote) { return TPBRT_FALSE; }

							if (token->value.size < 2u) { return TPBRT_FALSE; }
					}
			}

		return TPBRT_TRUE;
	}

#ifdef __cplusplus
}
#endif
