#include <pch.hpp>

#include <tinypbrt/detail/scene.h>
#include <tinypbrt/detail/texture_internal.h>

#include <test_helpers.hpp>

using namespace tpbrt_test;

TEST(scene_textures, checkerboard) {
	/* clang-format off */
	const Scene s("WorldBegin\nTexture \"checks\" \"spectrum\" \"checkerboard\" \"float uscale\" [16] \"rgb tex1\" [ .1 .1 .1 ] \"rgb tex2\" [ .8 .8 .8 ]\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->textures.count, 1u);
	const tpbrt_texture_t& t = s->textures.textures[0];
	EXPECT_EQ(to_std(t.name), "checks");
	EXPECT_EQ(t.type, TPBRT_TEXTURE_TYPE_SPECTRUM);
	EXPECT_EQ(t.tex_class, TPBRT_TEXTURE_CLASS_CHECKER_BOARD);
	EXPECT_EQ(t.as.checkerboard.tex1.as.spectrum.type, TPBRT_SPECTRUM_TYPE_RGB);
}

TEST(scene_textures, defaults) {
	const Scene s("WorldBegin\nTexture \"c\" \"float\" \"checkerboard\"\nTexture \"k\" \"float\" \"constant\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->textures.count, 2u);
	EXPECT_FLOAT_EQ(s->textures.textures[0].as.checkerboard.tex1.as.spectrum.as.constant, 1.0f);
	EXPECT_FLOAT_EQ(s->textures.textures[0].as.checkerboard.tex2.as.spectrum.as.constant, 0.0f);
	EXPECT_FLOAT_EQ(s->textures.textures[1].as.constant.value.as.spectrum.as.constant, 1.0f);
}

TEST(scene_textures, value_must_match_texture_type) {
	EXPECT_NE(Scene("WorldBegin\nTexture \"t\" \"float\" \"constant\" \"rgb value\" [ 1 1 1 ]\n").error(), TPBRT_ERROR_NONE);
	EXPECT_NE(Scene("WorldBegin\nTexture \"t\" \"float\" \"checkerboard\" \"spectrum tex1\" [ 400 1 700 1 ]\n").error(),
	  TPBRT_ERROR_NONE);
	EXPECT_NE(Scene("WorldBegin\nTexture \"t\" \"spectrum\" \"mix\" \"spectrum amount\" [ 400 1 700 1 ]\n").error(),
	  TPBRT_ERROR_NONE);
	EXPECT_TRUE(Scene("WorldBegin\nTexture \"t\" \"spectrum\" \"constant\" \"spectrum value\" [ 400 .5 700 .5 ]\n").ok());
}

TEST(scene_textures, texture_references_texture) {
	/* clang-format off */
	const Scene s("WorldBegin\nTexture \"a\" \"float\" \"constant\" \"float value\" 0.5\n"
			"Texture \"b\" \"float\" \"scale\" \"texture tex\" \"a\" \"float scale\" 2\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->textures.count, 2u);
	const tpbrt_texture_t& b = s->textures.textures[1];
	ASSERT_EQ(b.as.scale.tex.value_type, TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE_IDX);
	EXPECT_EQ(b.as.scale.tex.as.texture_idx, s->textures.textures[0].idx);
	EXPECT_FLOAT_EQ(b.as.scale.scale.as.spectrum.as.constant, 2.0f);
}

TEST(scene_textures, imagemap) {
	EXPECT_EQ(Scene("WorldBegin\nTexture \"t\" \"spectrum\" \"imagemap\"\n").error(), TPBRT_ERROR_NOT_FOUND);

	const Scene s("WorldBegin\nTexture \"t\" \"spectrum\" \"imagemap\" \"string filename\" \"textures\\\\wood \\\"1\\\".png\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(to_std(s->textures.textures[0].as.image_map.file_name), "textures\\wood \"1\".png");
}

TEST(scene_textures, mappings) {
	const Scene s("WorldBegin\nTranslate 1 2 3\nTexture \"s\" \"float\" \"checkerboard\" \"string mapping\" \"spherical\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_texture_t& t = s->textures.textures[0];
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, t.mapping_as.spherical.transform), 1.0f, 2.0f, 3.0f);
	EXPECT_EQ(Scene("WorldBegin\nTexture \"t\" \"float\" \"checkerboard\" \"string mapping\" \"cubic\"\n").error(),
	  TPBRT_ERROR_INVALID_TEXTURE_MAPPING);
}

TEST(scene_textures, invalid_type_and_class) {
	EXPECT_EQ(Scene("WorldBegin\nTexture \"t\" \"color\" \"constant\"\n").error(), TPBRT_ERROR_INVALID_TEXTURE_TYPE);
	EXPECT_EQ(Scene("WorldBegin\nTexture \"t\" \"float\" \"noise\"\n").error(), TPBRT_ERROR_INVALID_TEXTURE_CLASS);
}

TEST(scene_textures, error_after_spectrum_does_not_leak) {
	/* clang-format off */
	EXPECT_NE(Scene("WorldBegin\nTexture \"t\" \"spectrum\" \"checkerboard\" \"spectrum tex1\" [ 400 1 700 1 ] \"integer dimension\" [ 1 2 ]\n").error(),
	  TPBRT_ERROR_NONE);
	/* clang-format on */
}
