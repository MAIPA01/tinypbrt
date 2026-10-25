#include <pch.hpp>

#include <test_helpers.hpp>

#include <sstream>

using namespace tpbrt_test;

namespace {
	std::string many_named_objects(const int count) {
		std::ostringstream s;
		s << "WorldBegin\n";
			for (int i = 0; i < count; ++i) {
				s << "Texture \"tex" << i << "\" \"spectrum\" \"constant\" \"rgb value\" [ " << i << " 0 0 ]\n";
				s << "Texture \"tex" << i << "\" \"float\" \"constant\" \"float value\" " << i << "\n"; // osobna przestrzeń nazw
				s << "MakeNamedMaterial \"mat" << i << "\" \"string type\" \"diffuse\" \"texture reflectance\" \"tex" << i
				  << "\"\n";
				s << "MakeNamedMedium \"med" << i << "\" \"string type\" \"homogeneous\"\n";
				s << "ObjectBegin \"obj" << i << "\"\nShape \"sphere\" \"float radius\" " << (i + 1) << "\nObjectEnd\n";
				s << "Material \"diffuse\"\n"; // nienazwane materiały przeplatają się z nazwanymi
			}
			for (int i = 0; i < count; ++i) {
				s << "AttributeBegin\nNamedMaterial \"mat" << (count - 1 - i) << "\"\nMediumInterface \"med" << i << "\" \"\"\n"
				  << "ObjectInstance \"obj" << i << "\"\nShape \"disk\"\nAttributeEnd\n";
			}
		return s.str();
	}

	tpbrt_string_t key(const std::string& s) { return tpbrt_test::str(s.c_str()); }
} // namespace

class name_lookup : public ::testing::TestWithParam<int> {};

INSTANTIATE_TEST_SUITE_P(sizes, name_lookup, ::testing::Values(3, 9, 300));

TEST_P(name_lookup, every_name_resolves_to_its_own_element) {
	const int count = GetParam();
	const Scene s(many_named_objects(count));
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->textures.count, static_cast<size_t>(2 * count));
	ASSERT_EQ(s->materials.count, static_cast<size_t>(2 * count));
	ASSERT_EQ(s->medias.count, static_cast<size_t>(count));
	ASSERT_EQ(s->objects.count, static_cast<size_t>(count));

		for (int i = 0; i < count; ++i) {
			const std::string id			 = std::to_string(i);

			const tpbrt_material_t* material = nullptr;
			ASSERT_EQ(tpbrt_get_material_by_name(&s->materials, key("mat" + id), &material), TPBRT_ERROR_NONE) << id;
			EXPECT_EQ(to_std(material->name), "mat" + id);

			const tpbrt_media_t* media = nullptr;
			ASSERT_EQ(tpbrt_get_media_by_name(&s->medias, key("med" + id), &media), TPBRT_ERROR_NONE) << id;
			EXPECT_EQ(to_std(media->name), "med" + id);

			const tpbrt_object_t* object = nullptr;
			ASSERT_EQ(tpbrt_get_object_by_name(&s->objects, key("obj" + id), &object), TPBRT_ERROR_NONE) << id;
			ASSERT_EQ(object->children.count, 1u);
			EXPECT_FLOAT_EQ(object->children.shapes[0].as.sphere.radius, static_cast<float>(i + 1));

			const tpbrt_texture_t* texture = nullptr;
			ASSERT_EQ(tpbrt_get_texture_by_name(&s->textures, key("tex" + id), &texture), TPBRT_ERROR_NONE) << id;
			EXPECT_EQ(texture->type, TPBRT_TEXTURE_TYPE_SPECTRUM);

			const tpbrt_texture_t* reflectance = nullptr;
			ASSERT_EQ(tpbrt_get_texture_by_handle(&s->textures, &material->as.diffuse.reflectance, &reflectance),
			  TPBRT_ERROR_NONE);
			EXPECT_EQ(to_std(reflectance->name), "tex" + id);
			EXPECT_EQ(reflectance->type, TPBRT_TEXTURE_TYPE_SPECTRUM);
		}

	ASSERT_EQ(s->shapes.count, static_cast<size_t>(count));
		for (int i = 0; i < count; ++i) {
			const tpbrt_shape_t& shape		 = s->shapes.shapes[i];
			const tpbrt_material_t* material = nullptr;
			ASSERT_EQ(tpbrt_get_material_by_handle(&s->materials, shape.material, &material), TPBRT_ERROR_NONE);
			EXPECT_EQ(to_std(material->name), "mat" + std::to_string(count - 1 - i));
			const tpbrt_media_t* media = nullptr;
			ASSERT_EQ(tpbrt_get_media_by_handle(&s->medias, shape.interior_media_handle, &media), TPBRT_ERROR_NONE);
			EXPECT_EQ(to_std(media->name), "med" + std::to_string(i));
		}

	const tpbrt_material_t* missing = nullptr;
	EXPECT_EQ(tpbrt_get_material_by_name(&s->materials, key("nope"), &missing), TPBRT_ERROR_NOT_FOUND);
	EXPECT_EQ(tpbrt_get_material_by_name(&s->materials, key(""), &missing), TPBRT_ERROR_NOT_FOUND);
	EXPECT_EQ(tpbrt_get_material_by_name(&s->materials, key("mat"), &missing), TPBRT_ERROR_NOT_FOUND);
}

TEST(name_lookup, redefinitions_are_errors_like_in_pbrt_v4) {
	EXPECT_EQ(
	  Scene(
		"WorldBegin\nMakeNamedMaterial \"m\" \"string type\" \"diffuse\"\n" "MakeNamedMaterial \"m\" \"string type\" " "\"conduct" "or\"\n"
	  )
		.error(),
	  TPBRT_ERROR_DUPLICATE_MATERIAL_NAME
	);
	EXPECT_EQ(Scene("WorldBegin\nTexture \"t\" \"float\" \"constant\"\nTexture \"t\" \"float\" \"fbm\"\n").error(),
	  TPBRT_ERROR_DUPLICATE_TEXTURE_NAME);
	EXPECT_EQ(
	  Scene(
		"MakeNamedMedium \"m\" \"string type\" \"homogeneous\"\n" "MakeNamedMedium \"m\" \"string type\" " "\"homogeneous" "\"\n" "WorldB" "egin\n"
	  )
		.error(),
	  TPBRT_ERROR_DUPLICATE_MEDIUM_NAME
	);
	EXPECT_EQ(Scene("WorldBegin\nObjectBegin \"o\"\nObjectEnd\nObjectBegin \"o\"\nObjectEnd\n").error(),
	  TPBRT_ERROR_DUPLICATE_OBJECT_NAME);

	// clang-format off
	EXPECT_TRUE(Scene(
	  "WorldBegin\nTexture \"t\" \"float\" \"constant\"\nTexture \"t\" \"spectrum\" \"constant\"\n" "Material " "\"diffuse\"\nMat"
																												"erial " "\"diffu"
																														 "se\"\n"
	)
		.ok());
	// clang-format on
}

TEST(name_lookup, texture_of_other_type_is_reported) {
	EXPECT_EQ(
	  Scene("WorldBegin\nTexture \"t\" \"float\" \"constant\"\nMaterial \"diffuse\" \"texture reflectance\" \"t\"\n").error(),
	  TPBRT_ERROR_WRONG_TEXTURE_TYPE
	);
	EXPECT_EQ(Scene("WorldBegin\nMaterial \"diffuse\" \"texture reflectance\" \"t\"\n").error(), TPBRT_ERROR_NOT_FOUND);
}

TEST(name_lookup, lists_are_shrunk_to_their_size) {
	const Scene s(many_named_objects(37));
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(s->materials.names.count, 37u);
	EXPECT_GE(s->materials.names.capacity, 37u);
	EXPECT_LE(s->materials.names.capacity, 37u + 37u / 3u + 1u); // ~1,33 slotu na nazwę
	EXPECT_EQ(s->textures.names.count, 74u);

	const Scene small("WorldBegin\nMakeNamedMaterial \"a\" \"string type\" \"diffuse\"\n");
	ASSERT_TRUE(small.ok());
	EXPECT_EQ(small->materials.names.capacity, 0u);
	EXPECT_EQ(small->materials.names.slots, nullptr);
}
