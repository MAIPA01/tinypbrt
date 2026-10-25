#include <pch.hpp>

#include <tinypbrt/detail/scene.h>
#include <tinypbrt/detail/material_internal.h>

#include <test_helpers.hpp>

using namespace tpbrt_test;

namespace {
	const tpbrt_material_t& last_material(const Scene& s) {
		EXPECT_GE(s->materials.count, 1u);
		return s->materials.materials[s->materials.count - 1u];
	}

	float constant(const tpbrt_texture_handle_t& h) {
		EXPECT_EQ(h.value_type, TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM);
		EXPECT_EQ(h.as.spectrum.type, TPBRT_SPECTRUM_TYPE_CONSTANT);
		return h.as.spectrum.as.constant;
	}
} // namespace

TEST(scene_materials, diffuse_default_reflectance) {
	const Scene s("WorldBegin\nMaterial \"diffuse\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_FLOAT_EQ(constant(last_material(s).as.diffuse.reflectance), 0.5f);
}

TEST(scene_materials, reflectance_variants) {
	{
		const Scene s("WorldBegin\nMaterial \"diffuse\" \"rgb reflectance\" [ .1 .2 .3 ]\n");
		ASSERT_TRUE(s.ok()) << s.error();
		EXPECT_EQ(last_material(s).as.diffuse.reflectance.as.spectrum.type, TPBRT_SPECTRUM_TYPE_RGB);
	}
	{
		const Scene s("WorldBegin\nMaterial \"diffuse\" \"spectrum reflectance\" [ 300 .3 400 .6 500 .2 ]\n");
		ASSERT_TRUE(s.ok()) << s.error();
		const tpbrt_spectrum_t& sp = last_material(s).as.diffuse.reflectance.as.spectrum;
		ASSERT_EQ(sp.type, TPBRT_SPECTRUM_TYPE_WAVELENGTHS);
		EXPECT_EQ(sp.as.wavelengths.count, 3u);
	}
	{
		const Scene s("WorldBegin\nMaterial \"diffuse\" \"blackbody reflectance\" 3000\n");
		ASSERT_TRUE(s.ok()) << s.error();
		EXPECT_EQ(last_material(s).as.diffuse.reflectance.as.spectrum.type, TPBRT_SPECTRUM_TYPE_BLACKBODY);
	}
	{
		const Scene s("WorldBegin\nTexture \"t\" \"spectrum\" \"constant\"\nMaterial \"diffuse\" \"texture reflectance\" \"t\"\n");
		ASSERT_TRUE(s.ok()) << s.error();
		const tpbrt_texture_handle_t& h = last_material(s).as.diffuse.reflectance;
		ASSERT_EQ(h.value_type, TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE_IDX);
		EXPECT_EQ(h.as.texture_idx, s->textures.textures[0].idx);
	}
}

TEST(scene_materials, texture_of_wrong_type) {
	EXPECT_EQ(Scene("WorldBegin\nTexture \"t\" \"float\" \"constant\"\nMaterial \"diffuse\" \"texture reflectance\" \"t\"\n").error(),
	  TPBRT_ERROR_WRONG_TEXTURE_TYPE);
	EXPECT_EQ(Scene("WorldBegin\nMaterial \"diffuse\" \"texture reflectance\" \"missing\"\n").error(), TPBRT_ERROR_NOT_FOUND);
}

TEST(scene_materials, roughness_defaults_to_u_and_v) {
	const Scene s("WorldBegin\nMaterial \"conductor\" \"float roughness\" 0.3\n");
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_material_roughness_params_t& r = last_material(s).as.conductor.roughness_params;
	EXPECT_FLOAT_EQ(constant(r.roughness), 0.3f);
	EXPECT_FLOAT_EQ(constant(r.u_roughness), 0.3f);
	EXPECT_FLOAT_EQ(constant(r.v_roughness), 0.3f);
	EXPECT_EQ(r.remap_roughness, TPBRT_TRUE);
}

TEST(scene_materials, explicit_u_v_roughness) {
	const Scene s("WorldBegin\nMaterial \"conductor\" \"float uroughness\" 0.1 \"float vroughness\" 0.2 \"bool remaproughness\" false\n");
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_material_roughness_params_t& r = last_material(s).as.conductor.roughness_params;
	EXPECT_FLOAT_EQ(constant(r.u_roughness), 0.1f);
	EXPECT_FLOAT_EQ(constant(r.v_roughness), 0.2f);
	EXPECT_EQ(r.remap_roughness, TPBRT_FALSE);
}

TEST(scene_materials, roughness_must_be_float) {
	EXPECT_EQ(Scene("WorldBegin\nMaterial \"conductor\" \"rgb roughness\" [ .1 .1 .1 ]\n").error(), TPBRT_ERROR_INVALID_TEXTURE_TYPE);
}

TEST(scene_materials, conductor_defaults_to_copper) {
	const Scene s("WorldBegin\nMaterial \"conductor\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_material_conductor_params_t& c = last_material(s).as.conductor;
	ASSERT_EQ(c.value_type, TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_ETA_K);
	EXPECT_EQ(c.as.conductor.eta.as.spectrum.type, TPBRT_SPECTRUM_TYPE_BUILTIN);
	EXPECT_EQ(c.as.conductor.eta.as.spectrum.as.builtin, TPBRT_SPECTRUM_BUILTIN_METAL_CU_ETA);
	EXPECT_EQ(c.as.conductor.k.as.spectrum.as.builtin, TPBRT_SPECTRUM_BUILTIN_METAL_CU_K);
}

TEST(scene_materials, conductor_named_spectra_and_reflectance) {
	/* clang-format off */
	const Scene s("WorldBegin\nMaterial \"conductor\" \"spectrum eta\" \"metal-Au-eta\" \"spectrum k\" \"metal-Au-k\"\n"
			"Material \"conductor\" \"rgb reflectance\" [ .9 .8 .7 ]\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->materials.count, 2u);
	EXPECT_EQ(s->materials.materials[0].as.conductor.as.conductor.eta.as.spectrum.type, TPBRT_SPECTRUM_TYPE_BUILTIN);
	EXPECT_EQ(s->materials.materials[1].as.conductor.value_type, TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_REFLECTANCE);
}

TEST(scene_materials, coated_conductor_has_two_roughness_sets) {
	const Scene s("WorldBegin\nMaterial \"coatedconductor\" \"float interface.roughness\" 0.3 \"float conductor.roughness\" 0.1\n");
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_material_coated_conductor_params_t& c = last_material(s).as.coated_conductor;
	EXPECT_FLOAT_EQ(constant(c.interface_roughness_params.roughness), 0.3f);
	EXPECT_FLOAT_EQ(constant(c.interface_roughness_params.u_roughness), 0.3f);
	EXPECT_FLOAT_EQ(constant(c.conductor_roughness_params.roughness), 0.1f);
	EXPECT_FLOAT_EQ(constant(c.conductor_roughness_params.v_roughness), 0.1f);
}

TEST(scene_materials, eta_defaults) {
	/* clang-format off */
	const Scene s("WorldBegin\nMaterial \"dielectric\"\nMaterial \"thindielectric\"\nMaterial \"coateddiffuse\"\n"
			"Material \"thindielectric\" \"float eta\" 1.33\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->materials.count, 4u);
	EXPECT_FLOAT_EQ(constant(s->materials.materials[0].as.dielectric.eta), 1.5f);
	EXPECT_FLOAT_EQ(constant(s->materials.materials[1].as.thin_dielectric.eta), 1.5f);
	EXPECT_FLOAT_EQ(constant(s->materials.materials[2].as.coated_diffuse.eta), 1.5f);
	EXPECT_FLOAT_EQ(constant(s->materials.materials[3].as.thin_dielectric.eta), 1.33f);
}

TEST(scene_materials, mix_and_measured) {
	/* clang-format off */
	const Scene s("WorldBegin\nMakeNamedMaterial \"a\" \"string type\" \"diffuse\"\nMakeNamedMaterial \"b\" \"string type\" \"conductor\"\n"
			"Material \"mix\" \"string materials\" [ \"a\" \"b\" ] \"float amount\" 0.25\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(last_material(s).type, TPBRT_MATERIAL_TYPE_MIX);

	EXPECT_EQ(Scene("WorldBegin\nMaterial \"measured\"\n").error(), TPBRT_ERROR_NOT_FOUND);
	EXPECT_TRUE(Scene("WorldBegin\nMaterial \"measured\" \"string filename\" \"brdf.bsdf\"\n").ok());
}

TEST(scene_materials, interface_and_invalid) {
	EXPECT_TRUE(Scene("WorldBegin\nMaterial \"interface\"\n").ok());
	EXPECT_EQ(Scene("WorldBegin\nMaterial \"plastic\"\n").error(), TPBRT_ERROR_INVALID_MATERIAL_TYPE);
}

TEST(scene_materials, error_after_spectrum_does_not_leak) {
	EXPECT_NE(Scene("WorldBegin\nMaterial \"coateddiffuse\" \"spectrum albedo\" [ 400 1 700 1 ] \"integer maxdepth\" [ 1 2 ]\n").error(),
	  TPBRT_ERROR_NONE);
}
