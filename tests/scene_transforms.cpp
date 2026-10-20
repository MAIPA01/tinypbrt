#include <pch.hpp>

#include <tinypbrt/detail/scene.h>
#include <tinypbrt/detail/transform_internal.h>

#include <test_helpers.hpp>

using namespace tpbrt_test;

namespace {
	std::array<float, 3> first_shape_point(const Scene& s, const float x, const float y, const float z) {
		EXPECT_GE(s->shapes.count, 1u);
		return transform_point(matrix(*s, s->shapes.shapes[0].transform.start_transform), x, y, z);
	}
} // namespace

TEST(scene_transforms, translate) {
	const Scene s("WorldBegin\nTranslate 5 6 7\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(first_shape_point(s, 0, 0, 0), 5.0f, 6.0f, 7.0f);
}

TEST(scene_transforms, scale) {
	const Scene s("WorldBegin\nScale 2 3 4\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(first_shape_point(s, 1, 1, 1), 2.0f, 3.0f, 4.0f);
}

TEST(scene_transforms, rotate_in_degrees) {
	const Scene s("WorldBegin\nRotate 90 0 0 1\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(first_shape_point(s, 1, 0, 0), 0.0f, 1.0f, 0.0f);
}

TEST(scene_transforms, composition_order) {
	const Scene s("WorldBegin\nTranslate 5 0 0\nScale 2 2 2\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(first_shape_point(s, 1, 0, 0), 7.0f, 0.0f, 0.0f);
}

TEST(scene_transforms, transform_is_column_major) {
	const Scene s("WorldBegin\nTransform [ 1 0 0 0  0 1 0 0  0 0 1 0  5 6 7 1 ]\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(first_shape_point(s, 0, 0, 0), 5.0f, 6.0f, 7.0f);
}

TEST(scene_transforms, transform_replaces_ctm) {
	const Scene s("WorldBegin\nTranslate 100 0 0\nTransform [ 1 0 0 0  0 1 0 0  0 0 1 0  1 2 3 1 ]\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(first_shape_point(s, 0, 0, 0), 1.0f, 2.0f, 3.0f);
}

TEST(scene_transforms, concat_transform) {
	const Scene s("WorldBegin\nScale 2 2 2\nConcatTransform [ 1 0 0 0  0 1 0 0  0 0 1 0  1 0 0 1 ]\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(first_shape_point(s, 0, 0, 0), 2.0f, 0.0f, 0.0f);
}

TEST(scene_transforms, identity) {
	const Scene s("WorldBegin\nTranslate 1 2 3\nIdentity\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(first_shape_point(s, 0, 0, 0), 0.0f, 0.0f, 0.0f);
}

TEST(scene_transforms, world_begin_resets_ctm) {
	const Scene s("Translate 1 2 3\nWorldBegin\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(first_shape_point(s, 0, 0, 0), 0.0f, 0.0f, 0.0f);
}

TEST(scene_transforms, attribute_end_restores_ctm) {
	const Scene s("WorldBegin\nTranslate 1 0 0\nAttributeBegin\nTranslate 0 5 0\nAttributeEnd\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(first_shape_point(s, 0, 0, 0), 1.0f, 0.0f, 0.0f);
}

TEST(scene_transforms, transform_begin_end_is_supported) {
	const Scene s("WorldBegin\nTransformBegin\nTranslate 0 5 0\nTransformEnd\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(first_shape_point(s, 0, 0, 0), 0.0f, 0.0f, 0.0f);
}

TEST(scene_transforms, coordinate_system) {
	const Scene s("WorldBegin\nTranslate 1 2 3\nCoordinateSystem \"a\"\nIdentity\nCoordSysTransform \"a\"\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(first_shape_point(s, 0, 0, 0), 1.0f, 2.0f, 3.0f);
}

TEST(scene_transforms, coordinate_system_redefinition_overwrites) {
	/* clang-format off */
	const Scene s("WorldBegin\nTranslate 1 0 0\nCoordinateSystem \"a\"\nTranslate 1 0 0\nCoordinateSystem \"a\"\n"
			"Identity\nCoordSysTransform \"a\"\nShape \"sphere\"\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(first_shape_point(s, 0, 0, 0), 2.0f, 0.0f, 0.0f);
}

TEST(scene_transforms, unknown_coordinate_system_is_ignored) {
	const Scene s("WorldBegin\nTranslate 1 0 0\nCoordSysTransform \"nope\"\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(first_shape_point(s, 0, 0, 0), 1.0f, 0.0f, 0.0f);
}

TEST(scene_transforms, camera_coordinate_system) {
	const Scene s(
	  "LookAt 0 0 -5  0 0 0  0 1 0\nCamera \"perspective\"\nWorldBegin\nCoordSysTransform \"camera\"\nShape \"sphere\"\n"
	);
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(first_shape_point(s, 0, 0, 0), 0.0f, 0.0f, -5.0f);
}

TEST(scene_transforms, active_transform_end_time) {
	const Scene s("WorldBegin\nActiveTransform EndTime\nTranslate 0 0 5\nActiveTransform All\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->shapes.count, 1u);
	const tpbrt_transform_animated_t& t = s->shapes.shapes[0].transform;
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, t.start_transform), 0.0f, 0.0f, 0.0f);
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, t.end_transform), 0.0f, 0.0f, 5.0f);
}

TEST(scene_transforms, active_transform_start_time) {
	const Scene s("WorldBegin\nActiveTransform StartTime\nTranslate 0 0 5\nActiveTransform All\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_transform_animated_t& t = s->shapes.shapes[0].transform;
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, t.start_transform), 0.0f, 0.0f, 5.0f);
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, t.end_transform), 0.0f, 0.0f, 0.0f);
}

TEST(scene_transforms, static_ctm_shares_start_and_end) {
	const Scene s("WorldBegin\nTranslate 1 0 0\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_transform_animated_t& t = s->shapes.shapes[0].transform;
	EXPECT_EQ(t.start_transform.transform_idx, t.end_transform.transform_idx);
}

TEST(scene_transforms, lights_media_textures_get_transforms) {
	/* clang-format off */
	const Scene s("Translate 0 1 0\nMakeNamedMedium \"fog\" \"string type\" \"homogeneous\"\nWorldBegin\n"
			"Translate 2 0 0\nLightSource \"point\"\n"
			"Texture \"t\" \"float\" \"fbm\"\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->medias.count, 1u);
	ASSERT_EQ(s->lights.lights_sources.count, 1u);
	ASSERT_EQ(s->textures.count, 1u);
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, s->medias.medias[0].transform), 0.0f, 1.0f, 0.0f);
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, s->lights.lights_sources.lights[0].transform), 2.0f, 0.0f, 0.0f);
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, s->textures.textures[0].as.fbm.transform), 2.0f, 0.0f, 0.0f);
}
