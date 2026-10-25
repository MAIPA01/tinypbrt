#include <pch.hpp>

#include <tinypbrt/detail/param_internal.h>

#include <test_helpers.hpp>

using namespace tpbrt_test;

TEST(params, parse_every_param_type) {
	struct Case {
		const char* name;
		tpbrt_param_type_t type;
	};

	const Case cases[] = {
		{ .name = "integer",	 .type = TPBRT_PARAM_TYPE_INTEGER	  },
		{ .name = "float",	   .type = TPBRT_PARAM_TYPE_FLOAT	  },
		{ .name = "point2",	.type = TPBRT_PARAM_TYPE_POINT2	},
		{ .name = "vector2",	 .type = TPBRT_PARAM_TYPE_VECTOR2	  },
		{ .name = "point3",	.type = TPBRT_PARAM_TYPE_POINT3	},
		{ .name = "point",	   .type = TPBRT_PARAM_TYPE_POINT	  },
		{ .name = "vector3",	 .type = TPBRT_PARAM_TYPE_VECTOR3	  },
		{ .name = "vector",	.type = TPBRT_PARAM_TYPE_VECTOR	},
		{ .name = "normal3",	 .type = TPBRT_PARAM_TYPE_NORMAL3	  },
		{ .name = "normal",	.type = TPBRT_PARAM_TYPE_NORMAL	},
		{ .name = "spectrum",  .type = TPBRT_PARAM_TYPE_SPECTRUM	},
		{ .name = "rgb",		 .type = TPBRT_PARAM_TYPE_RGB		  },
		{ .name = "blackbody", .type = TPBRT_PARAM_TYPE_BLACKBODY },
		{ .name = "bool",	  .type = TPBRT_PARAM_TYPE_BOOL		},
		{ .name = "string",	.type = TPBRT_PARAM_TYPE_STRING	},
		{ .name = "texture",	 .type = TPBRT_PARAM_TYPE_TEXTURE	  },
	};
		for (const Case& c : cases) {
			tpbrt_param_type_t type;
			ASSERT_EQ(tpbrt_param_type_from_string(str(c.name), &type), TPBRT_ERROR_NONE) << c.name;
			EXPECT_EQ(type, c.type) << c.name;
		}

	tpbrt_param_type_t type;
	EXPECT_NE(tpbrt_param_type_from_string(TPBRT_STRING("double"), &type), TPBRT_ERROR_NONE);
}

TEST(params, float_single_array_and_default) {
	const Params p("\"float a\" 2.5 \"float b\" [ -1 ] \"float c\" [ 1 2 ]");
	ASSERT_EQ(p.error(), TPBRT_ERROR_NONE);

	tpbrt_float_t v = 0.0f;
	ASSERT_EQ(tpbrt_params_list_get_float(&p.list(), TPBRT_STRING("a"), 0.0f, &v), TPBRT_ERROR_NONE);
	EXPECT_FLOAT_EQ(v, 2.5f);
	ASSERT_EQ(tpbrt_params_list_get_float(&p.list(), TPBRT_STRING("b"), 0.0f, &v), TPBRT_ERROR_NONE);
	EXPECT_FLOAT_EQ(v, -1.0f);

	ASSERT_EQ(tpbrt_params_list_get_float(&p.list(), TPBRT_STRING("missing"), 7.0f, &v), TPBRT_ERROR_NONE);
	EXPECT_FLOAT_EQ(v, 7.0f);

	EXPECT_EQ(tpbrt_params_list_get_float(&p.list(), TPBRT_STRING("c"), 0.0f, &v), TPBRT_ERROR_TOO_MANY_VALUES);
}

TEST(params, floats_array) {
	const Params p("\"float values\" [ 1 2 3.5 ]");
	ASSERT_EQ(p.error(), TPBRT_ERROR_NONE);

	tpbrt_float_array_t values;
	ASSERT_EQ(tpbrt_params_list_get_floats(&p.list(), TPBRT_STRING("values"), &values), TPBRT_ERROR_NONE);
	ASSERT_EQ(values.count, 3u);
	EXPECT_FLOAT_EQ(values.data[2], 3.5f);
	tpbrt_float_array_free(&values);

	EXPECT_EQ(tpbrt_params_list_get_floats(&p.list(), TPBRT_STRING("missing"), &values), TPBRT_ERROR_NOT_FOUND);
}

TEST(params, int_uint_bool) {
	const Params p("\"integer i\" -3 \"integer u\" 4 \"bool t\" true \"bool f\" [ false ]");
	ASSERT_EQ(p.error(), TPBRT_ERROR_NONE);

	tpbrt_int_t i = 0;
	ASSERT_EQ(tpbrt_params_list_get_int(&p.list(), TPBRT_STRING("i"), 0, &i), TPBRT_ERROR_NONE);
	EXPECT_EQ(i, -3);

	tpbrt_uint_t u = 0u;
	ASSERT_EQ(tpbrt_params_list_get_uint(&p.list(), TPBRT_STRING("u"), 0u, &u), TPBRT_ERROR_NONE);
	EXPECT_EQ(u, 4u);
	EXPECT_EQ(tpbrt_params_list_get_uint(&p.list(), TPBRT_STRING("i"), 0u, &u), TPBRT_ERROR_PARSE_UINT);

	tpbrt_bool_t b = TPBRT_FALSE;
	ASSERT_EQ(tpbrt_params_list_get_bool(&p.list(), TPBRT_STRING("t"), TPBRT_FALSE, &b), TPBRT_ERROR_NONE);
	EXPECT_EQ(b, TPBRT_TRUE);
	ASSERT_EQ(tpbrt_params_list_get_bool(&p.list(), TPBRT_STRING("f"), TPBRT_TRUE, &b), TPBRT_ERROR_NONE);
	EXPECT_EQ(b, TPBRT_FALSE);
}

TEST(params, string_is_unescaped_and_owned) {
	const Params p("\"string filename\" \"dir\\\\a \\\"b\\\".png\"");
	ASSERT_EQ(p.error(), TPBRT_ERROR_NONE);

	tpbrt_string_t s;
	ASSERT_EQ(tpbrt_params_list_get_string(&p.list(), TPBRT_STRING("filename"), TPBRT_EMPTY_STRING, &s), TPBRT_ERROR_NONE);
	EXPECT_EQ(to_std(s), "dir\\a \"b\".png");
	tpbrt_string_free(&s);
}

TEST(params, string_default_is_a_copy) {
	const Params p("");
	ASSERT_EQ(p.error(), TPBRT_ERROR_NONE);

	tpbrt_string_t s;
	ASSERT_EQ(tpbrt_params_list_get_string(&p.list(), TPBRT_STRING("mapping"), TPBRT_STRING("uv"), &s), TPBRT_ERROR_NONE);
	EXPECT_EQ(to_std(s), "uv");
	tpbrt_string_free(&s);
}

TEST(params, string_required) {
	const Params p("\"string type\" \"diffuse\"");
	ASSERT_EQ(p.error(), TPBRT_ERROR_NONE);

	tpbrt_string_t s;
	ASSERT_EQ(tpbrt_params_list_get_string_required(&p.list(), TPBRT_STRING("type"), &s), TPBRT_ERROR_NONE);
	EXPECT_EQ(to_std(s), "diffuse");
	tpbrt_string_free(&s);

	EXPECT_EQ(tpbrt_params_list_get_string_required(&p.list(), TPBRT_STRING("filename"), &s), TPBRT_ERROR_NOT_FOUND);
	EXPECT_EQ(tpbrt_string_empty(s), TPBRT_TRUE);
}

TEST(params, strings_array) {
	const Params p("\"string materials\" [ \"a\" \"b c\" ]");
	ASSERT_EQ(p.error(), TPBRT_ERROR_NONE);

	tpbrt_string_array_t names;
	ASSERT_EQ(tpbrt_params_list_get_strings(&p.list(), TPBRT_STRING("materials"), &names), TPBRT_ERROR_NONE);
	ASSERT_EQ(names.count, 2u);
	EXPECT_EQ(to_std(names.data[0]), "a");
	EXPECT_EQ(to_std(names.data[1]), "b c");
	tpbrt_string_array_free(&names);
}

TEST(params, point3) {
	const Params p("\"point3 from\" [ 1 2 3 ] \"point3 P\" [ 0 0 0 1 0 0 0 1 0 ] \"point3 bad\" [ 1 2 ]");
	ASSERT_EQ(p.error(), TPBRT_ERROR_NONE);

	tpbrt_point3_t def;
	def.x = def.y = def.z = 9.0f;
	tpbrt_point3_t v;
	ASSERT_EQ(tpbrt_params_list_get_point3(&p.list(), TPBRT_STRING("from"), def, &v), TPBRT_ERROR_NONE);
	EXPECT_FLOAT_EQ(v.x, 1.0f);
	EXPECT_FLOAT_EQ(v.y, 2.0f);
	EXPECT_FLOAT_EQ(v.z, 3.0f);

	ASSERT_EQ(tpbrt_params_list_get_point3(&p.list(), TPBRT_STRING("to"), def, &v), TPBRT_ERROR_NONE);
	EXPECT_FLOAT_EQ(v.x, 9.0f);

	tpbrt_point3_array_t points;
	ASSERT_EQ(tpbrt_params_list_get_point3s(&p.list(), TPBRT_STRING("P"), &points), TPBRT_ERROR_NONE);
	EXPECT_EQ(points.count, 3u);
	tpbrt_vec3_array_free(&points);

	EXPECT_NE(tpbrt_params_list_get_point3s(&p.list(), TPBRT_STRING("bad"), &points), TPBRT_ERROR_NONE);
}

TEST(params, spectrum_variants) {
	/* clang-format off */
	Params p(
	  "\"spectrum w\" [ 300 .3 400 .6 500 .2 ] \"spectrum b\" \"metal-Cu-eta\" \"spectrum f\" \"my.spd\" "
	  "\"blackbody t\" 3000 \"rgb c\" [ .1 .2 .3 ] \"float k\" 0.5 \"spectrum odd\" [ 300 .3 400 ]"
	  );
	/* clang-format on */
	ASSERT_EQ(p.error(), TPBRT_ERROR_NONE);

	const tpbrt_spectrum_t def = constant_spectrum(1.0f);
	tpbrt_spectrum_t s;

	ASSERT_EQ(tpbrt_params_list_get_spectrum(&p.list(), TPBRT_STRING("w"), &def, &s), TPBRT_ERROR_NONE);
	ASSERT_EQ(s.type, TPBRT_SPECTRUM_TYPE_WAVELENGTHS);
	ASSERT_EQ(s.as.wavelengths.count, 3u);
	EXPECT_FLOAT_EQ(s.as.wavelengths.data[1].wavelength, 400.0f);
	EXPECT_FLOAT_EQ(s.as.wavelengths.data[1].value, 0.6f);
	tpbrt_spectrum_free(&s);

	ASSERT_EQ(tpbrt_params_list_get_spectrum(&p.list(), TPBRT_STRING("b"), &def, &s), TPBRT_ERROR_NONE);
	ASSERT_EQ(s.type, TPBRT_SPECTRUM_TYPE_BUILTIN);
	EXPECT_EQ(s.as.builtin, TPBRT_SPECTRUM_BUILTIN_METAL_CU_ETA);

	ASSERT_EQ(tpbrt_params_list_get_spectrum(&p.list(), TPBRT_STRING("f"), &def, &s), TPBRT_ERROR_NONE);
	ASSERT_EQ(s.type, TPBRT_SPECTRUM_TYPE_FILE);
	EXPECT_EQ(to_std(s.as.file_name), "my.spd");
	tpbrt_spectrum_free(&s);

	ASSERT_EQ(tpbrt_params_list_get_spectrum(&p.list(), TPBRT_STRING("t"), &def, &s), TPBRT_ERROR_NONE);
	ASSERT_EQ(s.type, TPBRT_SPECTRUM_TYPE_BLACKBODY);
	EXPECT_FLOAT_EQ(s.as.blackbody, 3000.0f);

	ASSERT_EQ(tpbrt_params_list_get_spectrum(&p.list(), TPBRT_STRING("c"), &def, &s), TPBRT_ERROR_NONE);
	ASSERT_EQ(s.type, TPBRT_SPECTRUM_TYPE_RGB);
	EXPECT_FLOAT_EQ(s.as.rgb.b, 0.3f);

	ASSERT_EQ(tpbrt_params_list_get_spectrum(&p.list(), TPBRT_STRING("k"), &def, &s), TPBRT_ERROR_NONE);
	ASSERT_EQ(s.type, TPBRT_SPECTRUM_TYPE_CONSTANT);
	EXPECT_FLOAT_EQ(s.as.constant, 0.5f);

	ASSERT_EQ(tpbrt_params_list_get_spectrum(&p.list(), TPBRT_STRING("missing"), &def, &s), TPBRT_ERROR_NONE);
	ASSERT_EQ(s.type, TPBRT_SPECTRUM_TYPE_CONSTANT);
	EXPECT_FLOAT_EQ(s.as.constant, 1.0f);

	EXPECT_EQ(tpbrt_params_list_get_spectrum_required(&p.list(), TPBRT_STRING("missing"), &s), TPBRT_ERROR_NOT_FOUND);

	EXPECT_NE(tpbrt_params_list_get_spectrum(&p.list(), TPBRT_STRING("odd"), &def, &s), TPBRT_ERROR_NONE);
}

TEST(params, spectrum_copy_keeps_constant) {
	const tpbrt_spectrum_t src = constant_spectrum(0.25f);
	tpbrt_spectrum_t dst;
	ASSERT_EQ(tpbrt_spectrum_copy(&dst, &src), TPBRT_ERROR_NONE);
	EXPECT_EQ(dst.type, TPBRT_SPECTRUM_TYPE_CONSTANT);
	EXPECT_FLOAT_EQ(dst.as.constant, 0.25f);
}

TEST(params, texture_values) {
	Params p("\"float r\" 0.3 \"rgb c\" [ 1 0 0 ] \"spectrum s\" [ 400 1 700 1 ] \"texture t\" \"unknown\"");
	ASSERT_EQ(p.error(), TPBRT_ERROR_NONE);

	constexpr tpbrt_textures_list_t no_textures = {};
	const tpbrt_texture_handle_t def			= constant_handle(1.0f);
	tpbrt_texture_handle_t h;

	ASSERT_EQ(tpbrt_params_list_get_texture_of_type(&p.list(), TPBRT_STRING("r"), &no_textures, TPBRT_TEXTURE_TYPE_FLOAT, &def,
				&h),
	  TPBRT_ERROR_NONE);
	ASSERT_EQ(h.value_type, TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM);
	ASSERT_EQ(h.as.spectrum.type, TPBRT_SPECTRUM_TYPE_CONSTANT);
	EXPECT_FLOAT_EQ(h.as.spectrum.as.constant, 0.3f);

	EXPECT_EQ(tpbrt_params_list_get_texture_of_type(&p.list(), TPBRT_STRING("c"), &no_textures, TPBRT_TEXTURE_TYPE_FLOAT, &def,
				&h),
	  TPBRT_ERROR_INVALID_TEXTURE_TYPE);
	EXPECT_EQ(tpbrt_params_list_get_texture_of_type(&p.list(), TPBRT_STRING("s"), &no_textures, TPBRT_TEXTURE_TYPE_FLOAT, &def,
				&h),
	  TPBRT_ERROR_INVALID_TEXTURE_TYPE);

	ASSERT_EQ(tpbrt_params_list_get_texture_of_type(&p.list(), TPBRT_STRING("s"), &no_textures, TPBRT_TEXTURE_TYPE_SPECTRUM, &def,
				&h),
	  TPBRT_ERROR_NONE);
	ASSERT_EQ(h.value_type, TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM);
	EXPECT_EQ(h.as.spectrum.type, TPBRT_SPECTRUM_TYPE_WAVELENGTHS);
	tpbrt_texture_handle_free(&h);

	EXPECT_EQ(tpbrt_params_list_get_texture(&p.list(), TPBRT_STRING("t"), &no_textures, &def, &h), TPBRT_ERROR_NOT_FOUND);

	ASSERT_EQ(tpbrt_params_list_get_texture(&p.list(), TPBRT_STRING("missing"), &no_textures, &def, &h), TPBRT_ERROR_NONE);
	EXPECT_FLOAT_EQ(h.as.spectrum.as.constant, 1.0f);
}

TEST(params, extend_overwrites_and_append_missing_keeps) {
	const Params a("\"float radius\" 1 \"float zmin\" -1");
	const Params b("\"float radius\" 5 \"float zmax\" 2");
	ASSERT_EQ(a.error(), TPBRT_ERROR_NONE);
	ASSERT_EQ(b.error(), TPBRT_ERROR_NONE);

	tpbrt_float_t v = 0.0f;

	tpbrt_params_list_t extended;
	tpbrt_params_list_init(&extended);
	ASSERT_EQ(tpbrt_params_list_extend(&extended, &a.list()), TPBRT_ERROR_NONE);
	ASSERT_EQ(tpbrt_params_list_extend(&extended, &b.list()), TPBRT_ERROR_NONE);
	EXPECT_EQ(extended.count, 3u);
	ASSERT_EQ(tpbrt_params_list_get_float(&extended, TPBRT_STRING("radius"), 0.0f, &v), TPBRT_ERROR_NONE);
	EXPECT_FLOAT_EQ(v, 5.0f);
	tpbrt_params_list_free(&extended);

	tpbrt_params_list_t missing;
	tpbrt_params_list_init(&missing);
	ASSERT_EQ(tpbrt_params_list_extend(&missing, &a.list()), TPBRT_ERROR_NONE);
	ASSERT_EQ(tpbrt_params_list_append_missing(&missing, &b.list()), TPBRT_ERROR_NONE);
	EXPECT_EQ(missing.count, 3u);
	ASSERT_EQ(tpbrt_params_list_get_float(&missing, TPBRT_STRING("radius"), 0.0f, &v), TPBRT_ERROR_NONE);
	EXPECT_FLOAT_EQ(v, 1.0f);
	tpbrt_params_list_free(&missing);
}
