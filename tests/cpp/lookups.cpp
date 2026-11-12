// Lookups by handle and by name through the C++ lists.
#include <cpp_test_helpers.hpp>

#include <string>
#include <string_view>

using namespace tpbrt_cpp_test;

namespace {
	const char* const NAMED_SCENE =
	  "MakeNamedMedium \"fog\" \"string type\" \"homogeneous\"\n" "WorldBegin\n" "Texture \"checks\" \"spectrum\" "
																				 "\"checkerboard\" \"rgb tex1\" [ .1 .1 .1 ] "
																				 "\"rgb tex2\" [ .8 .8 .8 ]\n" "MakeNamedMaterial"
																											   " \"gold\" "
																											   "\"string type\" "
																											   "\"conductor\"\n" "MakeNamedMaterial \"wall\" \"string type\" \"diffuse\" \"texture reflectance\" \"checks\"\n" "ObjectBegin \"tree\"\nShape \"sphere\" \"float radius\" 2\nObjectEnd\n" "AttributeBegin\nNamedMaterial \"wall\"\nMediumInterface \"fog\" \"\"\nAreaLightSource \"diffuse\"\nTranslate 1 2 3\n" "Shape \"disk\"\nAttributeEnd\n" "ObjectInstance \"tree\"\n";
} // namespace

TEST(cpp_lookups, by_handle) {
	const tpbrt::scene s = load(NAMED_SCENE);
	ASSERT_EQ(s.shapes().size(), 1u);
	const tpbrt::shape shape = s.shapes()[0];

	const auto material		 = s.materials().at(shape.material());
	ASSERT_TRUE(material) << tpbrt::to_string(material.get_error());
	EXPECT_EQ(material->name(), "wall");
	EXPECT_EQ(tpbrt::get_material(s.materials(), shape.material())->name(), "wall");

	const auto reflectance = s.textures().at(material->as_diffuse()->reflectance());
	ASSERT_TRUE(reflectance);
	EXPECT_EQ(reflectance->name(), "checks");
	EXPECT_EQ(reflectance->tex_class(), tpbrt::texture_class::CHECKER_BOARD);

	const auto interior = s.medias().at(shape.interior_media_handle());
	ASSERT_TRUE(interior);
	EXPECT_EQ(interior->name(), "fog");
	EXPECT_EQ(s.medias().at(shape.exterior_media_handle()).get_error(), tpbrt::error::INVALID_HANDLE); // no exterior medium

	const auto light = s.lights().area_lights().at(shape.area_light());
	ASSERT_TRUE(light);
	EXPECT_EQ(light->type(), tpbrt::area_light_type::DIFFUSE);

	const auto transform = s.transforms().at(shape.transform().start_transform());
	ASSERT_TRUE(transform);
	EXPECT_FLOAT_EQ(transform->value_at(3), 1.0f); // row-major: m03 is the x translation
	EXPECT_FLOAT_EQ(transform->value_at(7), 2.0f);
	EXPECT_FLOAT_EQ((*transform)[2].w(), 3.0f);

	ASSERT_EQ(s.instances().size(), 1u);
	const auto tree = s.objects().at(s.instances()[0].object());
	ASSERT_TRUE(tree);
	EXPECT_EQ(tree->name(), "tree");
	ASSERT_EQ(tree->children().size(), 1u);
	EXPECT_FLOAT_EQ(tree->children()[0].as_sphere()->radius(), 2.0f);
}

TEST(cpp_lookups, by_name) {
	const tpbrt::scene s = load(NAMED_SCENE);
	EXPECT_EQ(s.materials().at("gold")->type(), tpbrt::material_type::CONDUCTOR);
	EXPECT_EQ(s.materials().at(std::string("wall"))->type(), tpbrt::material_type::DIFFUSE);
	EXPECT_EQ(s.materials().at(std::string_view("glass")).get_error(), tpbrt::error::NOT_FOUND);
	EXPECT_EQ(tpbrt::get_material(s.materials(), "gold")->name(), "gold");
	EXPECT_EQ(s.textures().at("checks")->type(), tpbrt::texture_type::SPECTRUM);
	EXPECT_EQ(s.medias().at("fog")->type(), tpbrt::media_type::HOMOGENEOUS);
	EXPECT_EQ(s.objects().at("tree")->children().size(), 1u);
	EXPECT_EQ(s.objects().at("forest").get_error(), tpbrt::error::NOT_FOUND);

	// a name taken from the scene (tpbrt::string converts to std::string_view)
	const tpbrt::string name = s.materials()[0].name();
	EXPECT_EQ(s.materials().at(name)->name(), name);
}

TEST(cpp_lookups, index_access_is_still_available) {
	const tpbrt::scene s = load(NAMED_SCENE);
	ASSERT_GE(s.materials().size(), 2u);
	EXPECT_EQ(s.materials().at(0)->name(), s.materials()[0].name());
	EXPECT_EQ(s.materials().at(1000).get_error(), tpbrt::error::INDEX_OUT_OF_RANGE);
	EXPECT_EQ(s.transforms().at(0)->size(), 4u);
}
