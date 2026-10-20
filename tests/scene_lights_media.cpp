#include <pch.hpp>

#include <tinypbrt/detail/light_internal.h>
#include <tinypbrt/detail/media_internal.h>
#include <tinypbrt/detail/scene.h>

#include <test_helpers.hpp>

using namespace tpbrt_test;

TEST(scene_lights, point_light_builtin_spectrum) {
	const Scene s("WorldBegin\nLightSource \"point\" \"spectrum I\" \"stdillum-D65\" \"point3 from\" [ 1 2 3 ]\n");
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->lights.lights_sources.count, 1u);
	const tpbrt_light_source_t& l = s->lights.lights_sources.lights[0];
	EXPECT_EQ(l.type, TPBRT_LIGHT_SOURCE_TYPE_POINT);
	EXPECT_EQ(l.as.point.i.type, TPBRT_SPECTRUM_TYPE_BUILTIN);
	EXPECT_FLOAT_EQ(l.as.point.from.z, 3.0f);
	EXPECT_FLOAT_EQ(l.scale, 1.0f);
}

TEST(scene_lights, wavelengths_and_power) {
	const Scene s("WorldBegin\nLightSource \"spot\" \"spectrum I\" [ 400 1 700 1 ] \"float power\" 100 \"float coneangle\" 20\n");
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_light_source_t& l = s->lights.lights_sources.lights[0];
	EXPECT_EQ(l.as.spot.i.type, TPBRT_SPECTRUM_TYPE_WAVELENGTHS);
	EXPECT_FLOAT_EQ(l.as.spot.cone_angle, 20.0f);
	ASSERT_EQ(static_cast<tpbrt_bool_t>(l.power_illuminance.has_value), TPBRT_TRUE);
	EXPECT_FLOAT_EQ(l.power_illuminance.value, 100.0f);
}

TEST(scene_lights, area_light_scale_and_power) {
	const Scene s("WorldBegin\nAttributeBegin\nAreaLightSource \"diffuse\" \"float scale\" 2 \"float power\" 100 \"bool "
				  "twosided\" true\n" "Shape \"sphere\"\nAttributeEnd\n");
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->lights.area_lights.count, 1u);
	const tpbrt_area_light_t& a = s->lights.area_lights.lights[0];
	EXPECT_FLOAT_EQ(a.scale, 2.0f);
	ASSERT_EQ(static_cast<tpbrt_bool_t>(a.power.has_value), TPBRT_TRUE);
	EXPECT_FLOAT_EQ(a.power.value, 100.0f);
	EXPECT_EQ(a.as.diffuse.two_sided, TPBRT_TRUE);
}

TEST(scene_lights, area_light_without_power) {
	const Scene s("WorldBegin\nAreaLightSource \"diffuse\"\nShape \"sphere\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(static_cast<tpbrt_bool_t>(s->lights.area_lights.lights[0].power.has_value), TPBRT_FALSE);
	EXPECT_FLOAT_EQ(s->lights.area_lights.lights[0].scale, 1.0f);
}

TEST(scene_lights, filename_required) {
	EXPECT_EQ(Scene("WorldBegin\nLightSource \"goniometric\"\n").error(), TPBRT_ERROR_NOT_FOUND);
	EXPECT_EQ(Scene("WorldBegin\nLightSource \"projection\"\n").error(), TPBRT_ERROR_NOT_FOUND);
	EXPECT_TRUE(Scene("WorldBegin\nLightSource \"goniometric\" \"string filename\" \"g.exr\"\n").ok());
	EXPECT_TRUE(Scene("WorldBegin\nLightSource \"infinite\"\n").ok()); // infinite: filename opcjonalny
}

TEST(scene_lights, invalid_type) {
	EXPECT_EQ(Scene("WorldBegin\nLightSource \"laser\"\n").error(), TPBRT_ERROR_INVALID_LIGHT_SOURCE_TYPE);
	EXPECT_EQ(Scene("WorldBegin\nAreaLightSource \"laser\"\n").error(), TPBRT_ERROR_INVALID_AREA_LIGHT_TYPE);
}

TEST(scene_media, homogeneous_defaults) {
	const Scene s("MakeNamedMedium \"fog\" \"string type\" \"homogeneous\"\nWorldBegin\n");
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->medias.count, 1u);
	const tpbrt_media_t& m = s->medias.medias[0];
	EXPECT_EQ(to_std(m.name), "fog");
	EXPECT_EQ(m.type, TPBRT_MEDIA_TYPE_HOMOGENEOUS);
	ASSERT_EQ(m.as.homogeneous.sigma_a.type, TPBRT_SPECTRUM_TYPE_CONSTANT);
	EXPECT_FLOAT_EQ(m.as.homogeneous.sigma_a.as.constant, 1.0f);
	EXPECT_FLOAT_EQ(m.as.homogeneous.sigma_s.as.constant, 1.0f);
	ASSERT_EQ(m.as.homogeneous.Le.type, TPBRT_SPECTRUM_TYPE_CONSTANT);
	EXPECT_FLOAT_EQ(m.as.homogeneous.Le.as.constant, 0.0f);
}

TEST(scene_media, legacy_type_argument) {
	const Scene s("MakeNamedMedium \"fog\" \"homogeneous\"\nWorldBegin\n");
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(s->medias.medias[0].type, TPBRT_MEDIA_TYPE_HOMOGENEOUS);
}

TEST(scene_media, requires_type) {
	EXPECT_NE(Scene("MakeNamedMedium \"fog\"\nWorldBegin\n").error(), TPBRT_ERROR_NONE);
	EXPECT_EQ(Scene("MakeNamedMedium \"fog\" \"string type\" \"smoke\"\nWorldBegin\n").error(), TPBRT_ERROR_INVALID_MEDIA_TYPE);
}

TEST(scene_media, uniformgrid_bounds) {
	const Scene s(
	  "MakeNamedMedium \"u\" \"string type\" \"uniformgrid\" \"point3 p0\" [ -1 -2 -3 ] \"point3 p1\" [ 4 5 6 ]\nWorldBegin\n"
	);
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_media_uniform_grid_params_t& g = s->medias.medias[0].as.uniform_grid;
	EXPECT_FLOAT_EQ(g.p0.x, -1.0f);
	EXPECT_FLOAT_EQ(g.p0.z, -3.0f);
	EXPECT_FLOAT_EQ(g.p1.x, 4.0f);
	EXPECT_FLOAT_EQ(g.p1.z, 6.0f);
}

TEST(scene_media, uniformgrid_defaults_and_density_count) {
	const Scene s(
	  "MakeNamedMedium \"u\" \"string type\" \"uniformgrid\" \"integer nx\" 2 \"float density\" [ 1 2 ]\nWorldBegin\n"
	);
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_media_uniform_grid_params_t& g = s->medias.medias[0].as.uniform_grid;
	EXPECT_EQ(g.nx, 2u);
	EXPECT_EQ(g.ny, 1u);
	EXPECT_EQ(g.nz, 1u);
	EXPECT_FLOAT_EQ(g.p1.y, 1.0f);
	EXPECT_EQ(g.density.count, 2u);

	EXPECT_EQ(
	  Scene("MakeNamedMedium \"u\" \"string type\" \"uniformgrid\" \"integer nx\" 2 \"float density\" [ 1 2 3 ]\nWorldBegin\n")
		.error(),
	  TPBRT_ERROR_INDEX_OUT_OF_RANGE
	);
}

TEST(scene_media, rgbgrid) {
	/* clang-format off */
	const Scene s("MakeNamedMedium \"g\" \"string type\" \"rgbgrid\" \"integer nx\" 2 \"integer ny\" 1 \"integer nz\" 1"
			" \"rgb sigma_a\" [ 1 1 1  2 2 2 ]\nWorldBegin\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_media_rgb_grid_params_t& g = s->medias.medias[0].as.rgb_grid;
	EXPECT_EQ(g.nx, 2u);
	EXPECT_EQ(g.sigma_a.count, 2u);

	EXPECT_EQ(
	  Scene("MakeNamedMedium \"g\" \"string type\" \"rgbgrid\" \"integer nx\" 2 \"rgb sigma_a\" [ 1 1 1 ]\nWorldBegin\n").error(),
	  TPBRT_ERROR_INDEX_OUT_OF_RANGE
	);
}

TEST(scene_media, medium_in_world_uses_ctm) {
	const Scene s(
	  "WorldBegin\nAttributeBegin\nTranslate 0 0 7\nMakeNamedMedium \"fog\" \"string type\" \"homogeneous\"\nAttributeEnd\n"
	);
	ASSERT_TRUE(s.ok()) << s.error();
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, s->medias.medias[0].transform), 0.0f, 0.0f, 7.0f);
}
