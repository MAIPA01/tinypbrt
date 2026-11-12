// Tagged unions as std::variant: every alternative is a distinct type, visit / as<T>() / holds<T>() pick the right one.
#include <cpp_test_helpers.hpp>

#include <string>
#include <type_traits>

using namespace tpbrt_cpp_test;

namespace {
	template<class... Fs>
	struct overloaded : Fs... {
		using Fs::operator()...;
	};
	template<class... Fs>
	overloaded(Fs...) -> overloaded<Fs...>;

	/// @brief Name of the alternative held by a spectrum (one lambda per alternative, no duplicates allowed).
	std::string spectrum_kind(const tpbrt::spectrum spectrum) {
		return spectrum
		  .visit(overloaded {
			  [](const tpbrt::wavelength_array&) { return std::string("wavelengths"); },
			  [](const tpbrt::rgb&) { return std::string("rgb"); },
			  [](const tpbrt::blackbody&) { return std::string("blackbody"); },
			  [](const tpbrt::string&) { return std::string("file"); },
			  [](const tpbrt::spectrum_builtin) { return std::string("builtin"); },
			  [](const tpbrt::float_t) { return std::string("constant"); },
		  })
		  .value_or("none");
	}

	tpbrt::spectrum diffuse_reflectance(const tpbrt::scene& s, const tpbrt::size_t index = 0) {
		return *s.materials()[index].as_diffuse()->reflectance().as_spectrum();
	}
} // namespace

TEST(cpp_variants, blackbody_is_not_a_constant_spectrum) {
	// in C tpbrt_blackbody_t is tpbrt_float_t, the C++ wrapper keeps them apart
	const tpbrt::scene s = load(
	  "WorldBegin\nMaterial \"diffuse\" \"blackbody reflectance\" 3000\n" "Material \"diffuse\" \"float reflectance\" 0.25\n"
	);
	const tpbrt::spectrum blackbody = diffuse_reflectance(s, 0);
	const tpbrt::spectrum constant	= diffuse_reflectance(s, 1);

	EXPECT_EQ(spectrum_kind(blackbody), "blackbody");
	EXPECT_EQ(spectrum_kind(constant), "constant");
	ASSERT_TRUE(blackbody.as<tpbrt::blackbody>().has_value());
	EXPECT_FLOAT_EQ(blackbody.as<tpbrt::blackbody>()->temperature(), 3000.0f);
	EXPECT_FLOAT_EQ(blackbody.as_blackbody()->temperature(), 3000.0f);
	EXPECT_FALSE(blackbody.holds<tpbrt::float_t>());
	EXPECT_TRUE(constant.holds<tpbrt::float_t>());
	EXPECT_FLOAT_EQ(*constant.as<tpbrt::float_t>(), 0.25f);
}

TEST(cpp_variants, every_spectrum_kind) {
	const tpbrt::scene s = load(
	  "WorldBegin\nMaterial \"diffuse\" \"rgb reflectance\" [ .1 .2 .3 ]\n" "Material \"diffuse\" \"spectrum reflectance\" [ 300 "
																			".3 400 .6 500 .2 ]\n" "Material \"diffuse\" "
																								   "\"spectrum reflectance\" "
																								   "\"stdillum-D65\"\n"
	);
	EXPECT_EQ(spectrum_kind(diffuse_reflectance(s, 0)), "rgb");
	EXPECT_EQ(spectrum_kind(diffuse_reflectance(s, 1)), "wavelengths");
	EXPECT_EQ(spectrum_kind(diffuse_reflectance(s, 2)), "builtin");

	const tpbrt::rgb rgb = *diffuse_reflectance(s, 0).as_rgb();
	EXPECT_FLOAT_EQ(rgb.r(), 0.1f);
	EXPECT_FLOAT_EQ(rgb.g(), 0.2f);
	EXPECT_FLOAT_EQ(rgb.b(), 0.3f);

	const tpbrt::wavelength_array wavelengths = *diffuse_reflectance(s, 1).as_wavelengths();
	ASSERT_EQ(wavelengths.size(), 3u);
	EXPECT_FLOAT_EQ(wavelengths[1].lambda(), 400.0f);
	EXPECT_FLOAT_EQ(wavelengths[1].value(), 0.6f);
	EXPECT_EQ(*diffuse_reflectance(s, 2).as_builtin(), tpbrt::spectrum_builtin::STD_ILLUM_D65);
}

TEST(cpp_variants, noise_textures_are_distinguishable) {
	const tpbrt::scene s = load(
	  "WorldBegin\nTexture \"a\" \"float\" \"fbm\" \"integer octaves\" 3\n" "Texture \"b\" \"float\" \"wrinkled\" \"integer "
																			"octaves\" 4\n" "Texture \"c\" \"float\" \"windy\" "
																							"\"integer octaves\" 5\n"
	);
	ASSERT_EQ(s.textures().size(), 3u);
	const auto kind = [](const tpbrt::texture texture) {
		return texture
		  .visit(overloaded {
			  [](const tpbrt::texture_fbm_params& p) { return "fbm" + std::to_string(p.octaves()); },
			  [](const tpbrt::texture_wrinkled_params& p) { return "wrinkled" + std::to_string(p.octaves()); },
			  [](const tpbrt::texture_windy_params& p) { return "windy" + std::to_string(p.octaves()); },
			  [](const auto&) { return std::string("other"); },
		  })
		  .value_or("none");
	};
	EXPECT_EQ(kind(s.textures()[0]), "fbm3");
	EXPECT_EQ(kind(s.textures()[1]), "wrinkled4");
	EXPECT_EQ(kind(s.textures()[2]), "windy5");
	EXPECT_TRUE(s.textures()[1].holds<tpbrt::texture_wrinkled_params>());
	EXPECT_FALSE(s.textures()[1].holds<tpbrt::texture_fbm_params>());
	static_assert(tpbrt::texture_windy_params::TextureClass == tpbrt::texture_class::WINDY);
}

TEST(cpp_variants, texture_mappings_are_distinguishable) {
	const tpbrt::
	  scene
		s =
		  load(
			"WorldBegin\nTexture \"s\" \"float\" \"checkerboard\" \"string mapping\" \"spherical\"\n" "Texture \"c\" \"float\" "
																									  "\"checkerboard\" \"string "
																									  "mapping\" "
																									  "\"cylindrical\"\n" "Textur"
																														  "e "
																														  "\"u\" "
																														  "\"floa"
																														  "t\" "
																														  "\"chec"
																														  "kerboa"
																														  "rd\" "
																														  "\"floa"
																														  "t "
																														  "uscale"
																														  "\" 4\n"
		  );
	const auto mapping = [](const tpbrt::texture texture) {
		return texture
		  .visit_mapping(overloaded {
			  [](const tpbrt::texture_mapping_spherical_params&) { return std::string("spherical"); },
			  [](const tpbrt::texture_mapping_cylindrical_params&) { return std::string("cylindrical"); },
			  [](const tpbrt::texture_mapping_uv_params& p) { return "uv" + std::to_string(static_cast<int>(p.u_scale())); },
			  [](const tpbrt::texture_mapping_planar_params&) { return std::string("planar"); },
		  })
		  .value_or("none");
	};
	EXPECT_EQ(mapping(s.textures()[0]), "spherical");
	EXPECT_EQ(mapping(s.textures()[1]), "cylindrical");
	EXPECT_EQ(mapping(s.textures()[2]), "uv4");
	EXPECT_EQ(s.textures()[2].as_checkerboard()->tex1().value_type(), tpbrt::texture_handle_value_type::SPECTRUM);
}

TEST(cpp_variants, conductor_eta_k_and_reflectance) {
	const tpbrt::scene s = load("WorldBegin\nMaterial \"conductor\" \"spectrum eta\" \"metal-Au-eta\" \"spectrum k\" "
								"\"metal-Au-k\"\n" "Material \"conductor\" \"rgb reflectance\" [ .9 .5 .1 ]\n");
	const tpbrt::material_conductor_params eta_k	   = *s.materials()[0].as_conductor();
	const tpbrt::material_conductor_params reflectance = *s.materials()[1].as_conductor();

	ASSERT_TRUE(eta_k.holds<tpbrt::material_conductor_eta_k>());
	const tpbrt::material_conductor_eta_k values = *eta_k.as_eta_k();
	EXPECT_EQ(*values.eta().as_spectrum()->as_builtin(), tpbrt::spectrum_builtin::METAL_AU_ETA);
	EXPECT_EQ(*values.k().as_spectrum()->as_builtin(), tpbrt::spectrum_builtin::METAL_AU_K);

	ASSERT_TRUE(reflectance.holds<tpbrt::texture_handle>());
	EXPECT_FLOAT_EQ(reflectance.as_reflectance()->as_spectrum()->as_rgb()->r(), 0.9f);

	const auto kind = [](const tpbrt::material_conductor_params p) {
		return p
		  .visit(overloaded { [](const tpbrt::material_conductor_eta_k&) { return 1; },
			  [](const tpbrt::texture_handle&) { return 2; } })
		  .value_or(0);
	};
	EXPECT_EQ(kind(eta_k), 1);
	EXPECT_EQ(kind(reflectance), 2);
}

TEST(cpp_variants, subsurface_and_media_scattering) {
	const tpbrt::scene s = load(
	  "MakeNamedMedium \"fog\" \"string type\" \"homogeneous\" \"rgb sigma_a\" [ 1 2 3 ]\n" "MakeNamedMedium \"sea\" \"string "
																							"type\" \"homogeneous\" \"string "
																							"preset\" \"Pacific Ocean Surface "
																							"Water\"\n" "WorldBegin\nMaterial "
																										"\"subsurface\" \"string "
																										"name\" "
																										"\"Skin1\"\nMaterial "
																										"\"subsurface\"\n"
	);
	const tpbrt::media_scattering fog = s.medias()[0].as_homogeneous()->scattering();
	ASSERT_TRUE(fog.holds<tpbrt::media_sigma>());
	EXPECT_FLOAT_EQ(fog.as_sigma()->sigma_a().as_rgb()->b(), 3.0f);
	EXPECT_EQ(*s.medias()[1].as_homogeneous()->scattering().as_preset(), tpbrt::scattering_builtin::PACIFIC_OCEAN_SURFACE_WATER);

	const tpbrt::material_subsurface_scattering skin = s.materials()[0].as_subsurface()->scattering();
	EXPECT_EQ(*skin.as_builtin(), tpbrt::scattering_builtin::SKIN_1);
	const tpbrt::material_subsurface_scattering sigma = s.materials()[1].as_subsurface()->scattering();
	ASSERT_TRUE(sigma.holds<tpbrt::material_subsurface_sigma>());
	EXPECT_FLOAT_EQ(sigma.as_sigma()->sigma_s().as_spectrum()->as_rgb()->g(), 3.21f);
}

TEST(cpp_variants, every_shape_type) {
	const tpbrt::scene s = load(
	  "WorldBegin\n" "Shape \"sphere\" \"float radius\" 3\n" "Shape \"cylinder\" \"float radius\" 2\n" "Shape \"disk\" \"float "
																									   "innerradius\" 0.5\n" "Sha"
																															 "pe "
																															 "\"t"
																															 "ria"
																															 "ngl"
																															 "eme"
																															 "sh"
																															 "\" "
																															 "\"p"
																															 "oin"
																															 "t3 "
																															 "P\""
																															 " [ "
																															 "0 "
																															 "0 "
																															 "0  "
																															 "1 "
																															 "0 "
																															 "0  "
																															 "0 "
																															 "1 "
																															 "0 "
																															 "] "
																															 "\"i"
																															 "nte"
																															 "ger"
																															 " in"
																															 "dic"
																															 "es"
																															 "\" "
																															 "[ "
																															 "0 "
																															 "1 "
																															 "2 "
																															 "]\n" "Shape \"bilinearmesh\" \"point3 P\" [ 0 0 0  1 0 0  0 1 0  1 1 0 ] \"integer indices\" [ 0 1 2 3 ]\n" "Shape \"curve\" \"point3 P\" [ 0 0 0 1 0 0 2 0 0 3 0 0 ] \"float width\" 0.5\n" "Shape \"plymesh\" \"string filename\" \"mesh.ply\"\n" "Shape \"loopsubdiv\" \"point3 P\" [ 0 0 0  1 0 0  0 1 0 ] \"integer indices\" [ 0 1 2 ]\n"
	);
	ASSERT_EQ(s.shapes().size(), 8u);
	std::string kinds;
		for (const tpbrt::shape shape : s.shapes()) {
			shape.visit(overloaded {
				[&](const tpbrt::shape_sphere_params& p) {
					kinds += "sphere(" + std::to_string(static_cast<int>(p.radius())) + ")";
				},
				[&](const tpbrt::shape_cylinder_params&) { kinds += "cylinder"; },
				[&](const tpbrt::shape_disk_params& p) { kinds += p.inner_radius() == 0.5f ? "disk" : "disk?"; },
				[&](const tpbrt::shape_triangle_mesh_params& p) { kinds += "mesh" + std::to_string(p.p().size()); },
				[&](const tpbrt::shape_bilinear_mesh_params& p) { kinds += "bilinear" + std::to_string(p.indices().size()); },
				[&](const tpbrt::shape_curve_params& p) { kinds += p.width() == 0.5f ? "curve" : "curve?"; },
				[&](const tpbrt::shape_ply_mesh_params& p) { kinds += p.file_name() == "mesh.ply" ? "ply" : "ply?"; },
				[&](const tpbrt::shape_loop_subdiv_params&) { kinds += "loop"; },
			});
			kinds += ' ';
		}
	EXPECT_EQ(kinds, "sphere(3) cylinder disk mesh3 bilinear4 curve ply loop ");

	const tpbrt::point3_array p = s.shapes()[3].as_triangle_mesh()->p();
	EXPECT_FLOAT_EQ(p[1].x(), 1.0f);
	EXPECT_FLOAT_EQ(p[2].y(), 1.0f);
	EXPECT_EQ(s.shapes()[5].as_curve()->p()[3].x(), 3.0f);
}

TEST(cpp_variants, unknown_type_gives_empty_optional) {
	tpbrt_shape_t native	 = {};
	native.type				 = TPBRT_SHAPE_TYPE_MAX_NUM;
	const tpbrt::shape shape = native;
	bool called				 = false;
	shape.visit([&](const auto&) { called = true; });
	EXPECT_FALSE(called);
	EXPECT_FALSE(shape.visit([](const auto&) { return 1; }).has_value());
	EXPECT_FALSE(shape.as<tpbrt::shape_sphere_params>().has_value());
	EXPECT_FALSE(shape.as_sphere().has_value());
}

TEST(cpp_variants, lights_and_scene_settings) {
	const tpbrt::
	  scene
		s =
		  load(
			"Camera \"orthographic\"\nFilm \"gbuffer\"\nPixelFilter \"gaussian\" \"float sigma\" 0.25\n" "Sampler \"halton\" "
																										 "\"integer "
																										 "pixelsamples\" "
																										 "32\nIntegrator "
																										 "\"bdpt\" \"integer "
																										 "maxdepth\" 7\n" "Accele"
																														  "rator "
																														  "\"kdtr"
																														  "ee\"\n"
																														  "WorldB"
																														  "egin\n" "LightSource \"point\" \"spectrum I\" \"stdillum-D65\" \"point3 from\" [ 1 2 3 ]\n" "AttributeBegin\nAreaLightSource \"diffuse\" \"float scale\" 2 \"float power\" 100\nShape \"sphere\"\n" "AttributeEnd\n"
		  );
	EXPECT_EQ(s.camera().type(), tpbrt::camera_type::ORTHOGRAPHIC);
	EXPECT_TRUE(s.camera().holds<tpbrt::camera_orthographic_params>());
	EXPECT_EQ(s.film().type(), tpbrt::film_type::GBUFFER);
	EXPECT_TRUE(s.film().as_gbuffer().has_value());
	EXPECT_FLOAT_EQ(s.filter().as_gaussian()->sigma(), 0.25f);
	EXPECT_EQ(s.sampler().as_halton()->pixel_samples(), 32u);
	EXPECT_EQ(s.integrator().as_bdpt()->max_depth(), 7u);
	EXPECT_EQ(s.accelerator().type(), tpbrt::accelerator_type::KD_TREE);

	const tpbrt::light_source point = s.lights().lights_sources()[0];
	EXPECT_FLOAT_EQ(point.as_point()->from().z(), 3.0f);
	EXPECT_EQ(*point.as_point()->i().as_builtin(), tpbrt::spectrum_builtin::STD_ILLUM_D65);

	ASSERT_EQ(s.lights().area_lights().size(), 1u);
	const tpbrt::area_light area = s.lights().area_lights()[0];
	EXPECT_FLOAT_EQ(area.scale(), 2.0f);
	ASSERT_TRUE(area.power().has_value());
	EXPECT_FLOAT_EQ(area.power().value(), 100.0f);
	EXPECT_EQ(area.power().to_optional(), std::optional<float>(100.0f));
}
