#include <pch.hpp>

#include <test_helpers.hpp>

using namespace tpbrt_test;

TEST(param_types, wrong_declared_type_is_reported) {
	EXPECT_EQ(Scene("WorldBegin\nShape \"sphere\" \"string radius\" \"big\"\n").error(), TPBRT_ERROR_PARAM_TYPE_MISMATCH);
	EXPECT_EQ(Scene("WorldBegin\nShape \"sphere\" \"integer radius\" 2\n").error(), TPBRT_ERROR_PARAM_TYPE_MISMATCH);
	EXPECT_EQ(Scene("WorldBegin\nShape \"sphere\" \"bool radius\" true\n").error(), TPBRT_ERROR_PARAM_TYPE_MISMATCH);
	EXPECT_EQ(Scene("Film \"rgb\" \"float xresolution\" 100\nWorldBegin\n").error(), TPBRT_ERROR_PARAM_TYPE_MISMATCH);
	EXPECT_EQ(Scene("Integrator \"path\" \"float maxdepth\" 3\nWorldBegin\n").error(), TPBRT_ERROR_PARAM_TYPE_MISMATCH);
	EXPECT_EQ(Scene("Film \"rgb\" \"float filename\" 1\nWorldBegin\n").error(), TPBRT_ERROR_PARAM_TYPE_MISMATCH);
	EXPECT_EQ(Scene("WorldBegin\nMaterial \"conductor\" \"integer remaproughness\" 1\n").error(), TPBRT_ERROR_PARAM_TYPE_MISMATCH);
}

TEST(param_types, geometric_types_must_match) {
	EXPECT_EQ(Scene("WorldBegin\nShape \"trianglemesh\" \"vector3 P\" [ 0 0 0 1 0 0 0 1 0 ]\n").error(),
	  TPBRT_ERROR_PARAM_TYPE_MISMATCH);
	EXPECT_EQ(Scene("WorldBegin\nShape \"trianglemesh\" \"point3 P\" [ 0 0 0 1 0 0 0 1 0 ] \"point3 N\" [ 0 0 1 0 0 1 0 0 1 ]\n").error(),
	  TPBRT_ERROR_PARAM_TYPE_MISMATCH);
	EXPECT_EQ(Scene("WorldBegin\nShape \"trianglemesh\" \"point3 P\" [ 0 0 0 1 0 0 0 1 0 ] \"vector2 uv\" [ 0 0 1 0 0 1 ]\n").error(),
	  TPBRT_ERROR_PARAM_TYPE_MISMATCH);
	EXPECT_EQ(Scene("WorldBegin\nLightSource \"point\" \"rgb from\" [ 0 1 0 ]\n").error(), TPBRT_ERROR_PARAM_TYPE_MISMATCH);
}

TEST(param_types, synonyms_are_accepted) {
	// "point" = "point3", "normal" = "normal3", "vector" = "vector3"
	const Scene s("WorldBegin\nShape \"trianglemesh\" \"point P\" [ 0 0 0 1 0 0 0 1 0 ] \"normal N\" [ 0 0 1 0 0 1 0 0 1 ]"
				  " \"vector S\" [ 1 0 0 1 0 0 1 0 0 ] \"point2 uv\" [ 0 0 1 0 0 1 ]\n"
				  "Shape \"trianglemesh\" \"point3 P\" [ 0 0 0 1 0 0 0 1 0 ] \"normal3 N\" [ 0 0 1 0 0 1 0 0 1 ]"
				  " \"vector3 S\" [ 1 0 0 1 0 0 1 0 0 ]\n"
				  "LightSource \"spot\" \"point from\" [ 0 1 0 ] \"point3 to\" [ 0 0 0 ]\n");
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->shapes.count, 2u);
	EXPECT_EQ(s->shapes.shapes[0].as.triangle_mesh.n.count, 3u);
	EXPECT_EQ(s->shapes.shapes[1].as.triangle_mesh.s.count, 3u);
}

TEST(param_types, textured_parameters_accept_values_and_references) {
	// clang-format off
	const Scene s("WorldBegin\nTexture \"t\" \"spectrum\" \"constant\"\n"
				  "Material \"diffuse\" \"rgb reflectance\" [ .1 .2 .3 ]\n"
				  "Material \"diffuse\" \"spectrum reflectance\" \"metal-Cu-eta\"\n"
				  "Material \"diffuse\" \"blackbody reflectance\" 3000\n"
				  "Material \"diffuse\" \"float reflectance\" .5\n"
				  "Material \"diffuse\" \"texture reflectance\" \"t\"\n");
	// clang-format on
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(s->materials.count, 5u);

	EXPECT_EQ(Scene("WorldBegin\nMaterial \"diffuse\" \"bool reflectance\" true\n").error(), TPBRT_ERROR_PARAM_TYPE_MISMATCH);
	EXPECT_EQ(Scene("WorldBegin\nMaterial \"diffuse\" \"point3 reflectance\" [ 1 1 1 ]\n").error(),
	  TPBRT_ERROR_PARAM_TYPE_MISMATCH);
}

TEST(param_types, error_message_names_the_problem) {
	// NOLINTNEXTLINE(readability-magic-numbers)
	char message[160] = {};
	ASSERT_GT(tpbrt_get_error_message(TPBRT_ERROR_PARAM_TYPE_MISMATCH, message, sizeof(message)), 0u);
	EXPECT_NE(std::string(message).find("different type"), std::string::npos) << message;
}

TEST(param_types, unknown_parameter_names_are_still_ignored) {
	EXPECT_TRUE(Scene("WorldBegin\nShape \"sphere\" \"string something\" \"x\"\n").ok());
}
