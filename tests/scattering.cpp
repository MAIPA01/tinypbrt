// Subsurface scattering: material "subsurface" ("name" / "sigma_a" + "sigma_s") and media "preset",
// semantics as in pbrt-v4 (SubsurfaceMaterial::Create, HomogeneousMedium::Create).
#include <pch.hpp>

#include <tinypbrt/detail/scene.h>

#include <test_helpers.hpp>

using namespace tpbrt_test;

namespace {
	const tpbrt_material_t& last_material(const Scene& s) {
		EXPECT_GE(s->materials.count, 1u);
		return s->materials.materials[s->materials.count - 1u];
	}

	const tpbrt_spectrum_t& spectrum(const tpbrt_texture_handle_t& h) {
		EXPECT_EQ(h.value_type, TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM);
		return h.as.spectrum;
	}

	void expect_rgb(const tpbrt_spectrum_t& sp, const float r, const float g, const float b) {
		ASSERT_EQ(sp.type, TPBRT_SPECTRUM_TYPE_RGB);
		EXPECT_FLOAT_EQ(sp.as.rgb.r, r);
		EXPECT_FLOAT_EQ(sp.as.rgb.g, g);
		EXPECT_FLOAT_EQ(sp.as.rgb.b, b);
	}
} // namespace

TEST(scattering, subsurface_without_name_uses_default_sigma) {
	const Scene s("WorldBegin\nMaterial \"subsurface\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_material_subsurface_scattering_t& sc = last_material(s).as.subsurface.scattering;
	ASSERT_EQ(sc.type, TPBRT_MATERIAL_SUBSURFACE_SCATTERING_VALUE_SIGMA);
	/// NOLINTBEGIN(readability-magic-numbers)
	// pbrt-v4: RGB(.0011, .0024, .014) and RGB(2.55, 3.21, 3.77)
	expect_rgb(spectrum(sc.as.sigma.a), 0.0011f, 0.0024f, 0.014f);
	expect_rgb(spectrum(sc.as.sigma.s), 2.55f, 3.21f, 3.77f);
	/// NOLINTEND(readability-magic-numbers)
}

TEST(scattering, subsurface_sigma_a_and_sigma_s_are_separate) {
	const Scene s("WorldBegin\nMaterial \"subsurface\" \"rgb sigma_a\" [ 1 2 3 ] \"rgb sigma_s\" [ 4 5 6 ]\n");
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_material_subsurface_scattering_t& sc = last_material(s).as.subsurface.scattering;
	ASSERT_EQ(sc.type, TPBRT_MATERIAL_SUBSURFACE_SCATTERING_VALUE_SIGMA);
	/// NOLINTBEGIN(readability-magic-numbers)
	expect_rgb(spectrum(sc.as.sigma.a), 1.0f, 2.0f, 3.0f);
	expect_rgb(spectrum(sc.as.sigma.s), 4.0f, 5.0f, 6.0f);
	/// NOLINTEND(readability-magic-numbers)
}

TEST(scattering, subsurface_named_builtin) {
	const Scene s("WorldBegin\nMaterial \"subsurface\" \"string name\" \"Head & Shoulders Shampoo\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_material_subsurface_scattering_t& sc = last_material(s).as.subsurface.scattering;
	ASSERT_EQ(sc.type, TPBRT_MATERIAL_SUBSURFACE_SCATTERING_VALUE_BUILTIN);
	EXPECT_EQ(sc.as.builtin, TPBRT_SCATTERING_BUILTIN_HEAD_AND_SHOULDERS_SHAMPOO);
}

TEST(scattering, subsurface_empty_name_is_not_a_builtin) {
	// pbrt-v4: `if (!name.empty())` - an empty name falls back to sigma_a / sigma_s
	const Scene s("WorldBegin\nMaterial \"subsurface\" \"string name\" \"\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(last_material(s).as.subsurface.scattering.type, TPBRT_MATERIAL_SUBSURFACE_SCATTERING_VALUE_SIGMA);
}

TEST(scattering, subsurface_unknown_name_is_an_error) {
	EXPECT_EQ(Scene("WorldBegin\nMaterial \"subsurface\" \"string name\" \"Lava\"\n").error(),
	  TPBRT_ERROR_INVALID_SCATTERING_BUILTIN);
}

TEST(scattering, named_subsurface_material) {
	const Scene s("WorldBegin\nMakeNamedMaterial \"skin\" \"string type\" \"subsurface\" \"string name\" \"Skin1\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(last_material(s).as.subsurface.scattering.as.builtin, TPBRT_SCATTERING_BUILTIN_SKIN_1);
}

TEST(scattering, media_preset) {
	// clang-format off
	const Scene s("MakeNamedMedium \"m\" \"string type\" \"homogeneous\" \"string preset\" \"Pacific Ocean Surface Water\"\n"
				  "MakeNamedMedium \"g\" \"string type\" \"uniformgrid\" \"string preset\" \"Ketchup\"\n"
				  "  \"integer nx\" 1 \"integer ny\" 1 \"integer nz\" 1 \"float density\" [ 1 ]\n"
				  "WorldBegin\n");
	// clang-format on
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->medias.count, 2u);
	const tpbrt_media_scattering_t& homogeneous = s->medias.medias[0].as.homogeneous.scattering;
	ASSERT_EQ(homogeneous.type, TPBRT_MEDIA_SCATTERING_VALUE_PRESET);
	EXPECT_EQ(homogeneous.as.preset, TPBRT_SCATTERING_BUILTIN_PACIFIC_OCEAN_SURFACE_WATER);
	const tpbrt_media_scattering_t& grid = s->medias.medias[1].as.uniform_grid.scattering;
	ASSERT_EQ(grid.type, TPBRT_MEDIA_SCATTERING_VALUE_PRESET);
	EXPECT_EQ(grid.as.preset, TPBRT_SCATTERING_BUILTIN_KETCHUP);
}

TEST(scattering, media_sigma_and_empty_preset) {
	// clang-format off
	const Scene s("MakeNamedMedium \"m\" \"string type\" \"homogeneous\" \"string preset\" \"\"\n"
				  "  \"rgb sigma_a\" [ 1 2 3 ] \"rgb sigma_s\" [ 4 5 6 ]\nWorldBegin\n");
	// clang-format on
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_media_scattering_t& sc = s->medias.medias[0].as.homogeneous.scattering;
	ASSERT_EQ(sc.type, TPBRT_MEDIA_SCATTERING_VALUE_SIGMA);
	/// NOLINTBEGIN(readability-magic-numbers)
	expect_rgb(sc.as.sigma.a, 1.0f, 2.0f, 3.0f);
	expect_rgb(sc.as.sigma.s, 4.0f, 5.0f, 6.0f);
	/// NOLINTEND(readability-magic-numbers)
}

TEST(scattering, every_builtin_name_parses) {
	static const char* const NAMES[] = { "Apple", "Chicken1", "Chicken2", "Cream", "Ketchup", "Marble", "Potato", "Skimmilk",
		"Skin1", "Skin2", "Spectralon", "Wholemilk", "Lowfat Milk", "Reduced Milk", "Regular Milk", "Espresso",
		"Mint Mocha Coffee", "Lowfat Soy Milk", "Regular Soy Milk", "Lowfat Chocolate Milk", "Regular Chocolate Milk", "Coke",
		"Pepsi", "Sprite", "Gatorade", "Chardonnay", "White Zinfandel", "Merlot", "Budweiser Beer", "Coors Light Beer", "Clorox",
		"Apple Juice", "Cranberry Juice", "Grape Juice", "Ruby Grapefruit Juice", "White Grapefruit Juice", "Shampoo",
		"Strawberry Shampoo", "Head & Shoulders Shampoo", "Lemon Tea Powder", "Orange Powder", "Pink Lemonade Powder",
		"Cappuccino Powder", "Salt Powder", "Sugar Powder", "Suisse Mocha Powder", "Pacific Ocean Surface Water" };
	static_assert(std::size(NAMES) == TPBRT_SCATTERING_BUILTIN_MAX_NUM, "pbrt-v4 has 47 measured media");
		for (size_t i = 0; i < TPBRT_SCATTERING_BUILTIN_MAX_NUM; ++i) {
			const Scene s(std::string("WorldBegin\nMaterial \"subsurface\" \"string name\" \"") + NAMES[i] + "\"\n");
			ASSERT_TRUE(s.ok()) << NAMES[i] << ": " << s.error();
			EXPECT_EQ(static_cast<size_t>(last_material(s).as.subsurface.scattering.as.builtin), i) << NAMES[i];
		}
}

TEST(scattering, coord_sys_transform_uses_the_stored_system) {
	// regression: tpbrt_coord_system_map_get returns tpbrt_error_t (TPBRT_ERROR_NONE == 0 == TPBRT_FALSE)
	const Scene s(
	  "WorldBegin\nTranslate 1 2 3\nCoordinateSystem \"a\"\nIdentity\nCoordSysTransform \"a\"\n" "Shape \"sphere\"\n"
	);
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->shapes.count, 1u);
}
