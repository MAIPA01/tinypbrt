#include <pch.hpp>

#include <tinypbrt/detail/scene.h>

#include <test_helpers.hpp>

using namespace tpbrt_test;

TEST(scene_include, load_from_file) {
	const TempDir dir;
	const std::string main = dir.write("main.pbrt", "WorldBegin\nShape \"sphere\" \"float radius\" 2\n");

	const Scene s		   = Scene::from_file(main);
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->shapes.count, 1u);
	EXPECT_FLOAT_EQ(s->shapes.shapes[0].as.sphere.radius, 2.0f);
}

TEST(scene_include, missing_file) {
	const TempDir dir;
	EXPECT_EQ(Scene::from_file(dir.path() + "/nope.pbrt").error(), TPBRT_ERROR_FILE_NOT_FOUND);

	const std::string main = dir.write("main.pbrt", "WorldBegin\nInclude \"nope.pbrt\"\n");
	EXPECT_EQ(Scene::from_file(main).error(), TPBRT_ERROR_FILE_NOT_FOUND);
}

TEST(scene_include, nested_paths_relative_to_main_file) {
	const TempDir dir;
	/* clang-format off */
	const std::string main = dir.write("scenes/main.pbrt", "Film \"rgb\" \"integer xresolution\" 400 \"integer yresolution\" "
														   "800\n" "WorldBegin\nInclude \"geometry/a.pbrt\"\nShape \"disk\"\n");
	/* clang-format on */
	std::ignore = dir.write("scenes/geometry/a.pbrt", "Material \"conductor\"\nShape \"sphere\"\nInclude \"geometry/b.pbrt\"\n");
	std::ignore = dir.write("scenes/geometry/b.pbrt", "Shape \"cylinder\"\n");

	const Scene s = Scene::from_file(main);
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->shapes.count, 3u);
	EXPECT_EQ(s->shapes.shapes[0].type, TPBRT_SHAPE_TYPE_SPHERE);
	EXPECT_EQ(s->shapes.shapes[1].type, TPBRT_SHAPE_TYPE_CYLINDER);
	EXPECT_EQ(s->shapes.shapes[2].type, TPBRT_SHAPE_TYPE_DISK);

	EXPECT_EQ(s->materials.materials[s->shapes.shapes[2].material.material_idx].type, TPBRT_MATERIAL_TYPE_CONDUCTOR);
}

TEST(scene_include, import_behaves_like_include) {
	const TempDir dir;
	const std::string main = dir.write("main.pbrt", "WorldBegin\nImport \"geo.pbrt\"\n");
	std::ignore =
	  dir.write("geo.pbrt", "MakeNamedMaterial \"m\" \"string type\" \"diffuse\"\nNamedMaterial \"m\"\nShape \"sphere\"\n");

	const Scene s = Scene::from_file(main);
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(s->shapes.count, 1u);
}

TEST(scene_include, camera_in_included_file) {
	const TempDir dir;
	/* clang-format off */
	const std::string main = dir.write("main.pbrt",
	  "Include \"cam.pbrt\"\nFilm \"rgb\" \"integer xresolution\" 400 \"integer yresolution\" 800\n" "WorldBegin\n");
	std::ignore			   = dir.write("cam.pbrt", "Translate 0 0 5\nCamera \"perspective\" \"float fov\" 30\n");
	/* clang-format on */

	const Scene s = Scene::from_file(main);
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_FLOAT_EQ(s->camera.as.perspective.fov, 30.0f);
	EXPECT_NEAR(s->camera.as.perspective.screen_window[3], 2.0f, EPS);
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, s->camera.transform.start_transform), 0.0f, 0.0f, -5.0f);
}

TEST(scene_include, names_from_included_file_survive) {
	const TempDir dir;
	const std::string main =
	  dir.write("main.pbrt", "WorldBegin\nInclude \"defs.pbrt\"\nMaterial \"diffuse\" \"texture reflectance\" \"t\"\n");
	std::ignore	  = dir.write("defs.pbrt", "Texture \"t\" \"spectrum\" \"imagemap\" \"string filename\" \"wood.png\"\n");

	const Scene s = Scene::from_file(main);
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->textures.count, 1u);
	EXPECT_EQ(to_std(s->textures.textures[0].name), "t");
	EXPECT_EQ(to_std(s->textures.textures[0].as.image_map.file_name), "wood.png");
}

TEST(scene_include, self_include_is_detected) {
	const TempDir dir;
	const std::string loop = dir.write("loop.pbrt", "Include \"loop.pbrt\"\n");
	EXPECT_EQ(Scene::from_file(loop).error(), TPBRT_ERROR_INCLUDES_STACK_OVERFLOW);

	const std::string world_loop = dir.write("world_loop.pbrt", "WorldBegin\nInclude \"world_loop.pbrt\"\n");
	EXPECT_EQ(Scene::from_file(world_loop).error(), TPBRT_ERROR_WORLD_ALREADY_STARTED);
}

TEST(scene_include, from_memory_uses_working_directory) {
	const TempDir dir;
	std::ignore = dir.write("part.pbrt", "Shape \"sphere\"\n");
	const Scene s("WorldBegin\nInclude \"part.pbrt\"\n", dir.path().c_str());
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(s->shapes.count, 1u);
}
