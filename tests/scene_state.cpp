#include <pch.hpp>

#include <tinypbrt/detail/scene.h>
#include <tinypbrt/detail/state_internal.h>

#include <test_helpers.hpp>

using namespace tpbrt_test;

TEST(scene_state, attribute_defaults_and_explicit_params) {
	const Scene s("WorldBegin\nAttribute \"shape\" \"float radius\" 5\nShape \"sphere\"\nShape \"sphere\" \"float radius\" 2\n");
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->shapes.count, 2u);
	EXPECT_FLOAT_EQ(s->shapes.shapes[0].as.sphere.radius, 5.0f);
	EXPECT_FLOAT_EQ(s->shapes.shapes[1].as.sphere.radius, 2.0f);
}

TEST(scene_state, later_attribute_overrides_earlier) {
	const Scene s(
	  "WorldBegin\nAttribute \"shape\" \"float radius\" 5\nAttribute \"shape\" \"float radius\" 7\nShape \"sphere\"\n"
	);
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_FLOAT_EQ(s->shapes.shapes[0].as.sphere.radius, 7.0f);
}

TEST(scene_state, attribute_is_scoped) {
	const Scene s("WorldBegin\nAttributeBegin\nAttribute \"shape\" \"float radius\" 5\nAttributeEnd\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_FLOAT_EQ(s->shapes.shapes[0].as.sphere.radius, 1.0f);
}

TEST(scene_state, attribute_end_restores_material) {
	/* clang-format off */
	const Scene s("WorldBegin\nMaterial \"conductor\"\nAttributeBegin\nMaterial \"diffuse\"\nShape \"sphere\"\nAttributeEnd\n"
			"Shape \"disk\"\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->shapes.count, 2u);
	const tpbrt_material_t& inner = s->materials.materials[s->shapes.shapes[0].material.material_idx];
	const tpbrt_material_t& outer = s->materials.materials[s->shapes.shapes[1].material.material_idx];
	EXPECT_EQ(inner.type, TPBRT_MATERIAL_TYPE_DIFFUSE);
	EXPECT_EQ(outer.type, TPBRT_MATERIAL_TYPE_CONDUCTOR);
}

TEST(scene_state, too_many_attribute_end) {
	EXPECT_EQ(Scene("WorldBegin\nAttributeEnd\n").error(), TPBRT_ERROR_TOO_MANY_END_ATTRIBUTES);
}

TEST(scene_state, make_named_material_keeps_current) {
	/* clang-format off */
	const Scene s("WorldBegin\nMaterial \"conductor\"\nMakeNamedMaterial \"m\" \"string type\" \"diffuse\"\nShape \"sphere\"\n"
			"NamedMaterial \"m\"\nShape \"disk\"\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->shapes.count, 2u);
	EXPECT_EQ(s->materials.materials[s->shapes.shapes[0].material.material_idx].type, TPBRT_MATERIAL_TYPE_CONDUCTOR);
	const tpbrt_material_t& named = s->materials.materials[s->shapes.shapes[1].material.material_idx];
	EXPECT_EQ(named.type, TPBRT_MATERIAL_TYPE_DIFFUSE);
	EXPECT_EQ(to_std(named.name), "m");
}

TEST(scene_state, make_named_material_requires_type) {
	EXPECT_EQ(Scene("WorldBegin\nMakeNamedMaterial \"m\"\n").error(), TPBRT_ERROR_NOT_FOUND);
}

TEST(scene_state, unknown_named_material) { EXPECT_NE(Scene("WorldBegin\nNamedMaterial \"nope\"\n").error(), TPBRT_ERROR_NONE); }

TEST(scene_state, reverse_orientation) {
	const Scene s("WorldBegin\nAttributeBegin\nReverseOrientation\nShape \"sphere\"\nAttributeEnd\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(s->shapes.shapes[0].reverse_orientation, TPBRT_TRUE);
	EXPECT_EQ(s->shapes.shapes[1].reverse_orientation, TPBRT_FALSE);
}

TEST(scene_state, object_instancing) {
	/* clang-format off */
	const Scene s("WorldBegin\nObjectBegin \"o\"\nShape \"sphere\"\nShape \"disk\"\nObjectEnd\n"
			"Translate 1 0 0\nObjectInstance \"o\"\nTranslate 1 0 0\nObjectInstance \"o\"\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(s->shapes.count, 0u);
	ASSERT_EQ(s->objects.count, 1u);
	EXPECT_EQ(to_std(s->objects.objects[0].name), "o");
	EXPECT_EQ(s->objects.objects[0].children.count, 2u);
	ASSERT_EQ(s->instances.count, 2u);
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, s->instances.instances[1].transform.start_transform), 2.0f, 0.0f, 0.0f);
}

TEST(scene_state, object_with_attribute_block_inside) {
	const Scene s(
	  "WorldBegin\nObjectBegin \"o\"\nAttributeBegin\nShape \"sphere\"\nAttributeEnd\nObjectEnd\nObjectInstance \"o\"\n"
	);
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->objects.count, 1u);
	EXPECT_EQ(s->objects.objects[0].children.count, 1u);
}

TEST(scene_state, object_end_cannot_close_attribute) {
	EXPECT_EQ(Scene("WorldBegin\nObjectBegin \"o\"\nAttributeBegin\nObjectEnd\nShape \"sphere\"\n").error(),
	  TPBRT_ERROR_UNEXPECTED_TOKEN);
}

TEST(scene_state, attribute_end_cannot_close_object) {
	EXPECT_EQ(Scene("WorldBegin\nObjectBegin \"o\"\nAttributeEnd\n").error(), TPBRT_ERROR_UNEXPECTED_TOKEN);
}

TEST(scene_state, object_end_without_begin) { EXPECT_EQ(Scene("WorldBegin\nObjectEnd\n").error(), TPBRT_ERROR_UNEXPECTED_TOKEN); }

TEST(scene_state, unclosed_object_does_not_leak) {
	const Scene s("WorldBegin\nObjectBegin \"o\"\nShape \"sphere\"\n");
	(void)s.error();
}

TEST(scene_state, unknown_object_instance) {
	EXPECT_NE(Scene("WorldBegin\nObjectInstance \"nope\"\n").error(), TPBRT_ERROR_NONE);
}

TEST(scene_state, medium_interface) {
	/* clang-format off */
	const Scene s("MakeNamedMedium \"in\" \"string type\" \"homogeneous\"\nMakeNamedMedium \"out\" \"string type\" \"homogeneous\"\n"
			"WorldBegin\nMediumInterface \"in\" \"out\"\nShape \"sphere\"\nMediumInterface \"in\"\nShape \"disk\"\n"
			"MediumInterface \"\" \"\"\nShape \"cylinder\"\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->shapes.count, 3u);
	const tpbrt_media_idx_t in	= s->medias.medias[0].idx;
	const tpbrt_media_idx_t out = s->medias.medias[1].idx;
	EXPECT_EQ(s->shapes.shapes[0].interior_media_handle.media_idx, in);
	EXPECT_EQ(s->shapes.shapes[0].exterior_media_handle.media_idx, out);
	EXPECT_EQ(s->shapes.shapes[1].interior_media_handle.media_idx, in);
	EXPECT_EQ(s->shapes.shapes[1].exterior_media_handle.media_idx, in);
	EXPECT_EQ(s->shapes.shapes[2].interior_media_handle.media_idx, TPBRT_MEDIA_IDX_INVALID);
}

TEST(scene_state, unknown_medium) { EXPECT_NE(Scene("WorldBegin\nMediumInterface \"nope\"\n").error(), TPBRT_ERROR_NONE); }
