#include <pch.hpp>

#include <tinypbrt/detail/scene.h>

#include <test_helpers.hpp>

using namespace tpbrt_test;

// Przykład z https://pbrt.org/fileformat-v4
static constexpr auto SPEC_EXAMPLE = R"(
LookAt 3 4 1.5  # eye
       .5 .5 0  # look at point
       0 0 1    # up vector
Camera "perspective" "float fov" 45

Sampler "halton" "integer pixelsamples" 128
Integrator "volpath"
Film "rgb" "string filename" "simple.png"
     "integer xresolution" [400] "integer yresolution" [400]

WorldBegin

LightSource "infinite" "rgb L" [ .4 .45 .5 ]
LightSource "distant"  "point3 from" [ -30 40  100 ]
   "blackbody L" 3000 "float scale" 1.5

AttributeBegin
  Material "dielectric"
  Shape "sphere" "float radius" 1
AttributeEnd

AttributeBegin
  Texture "checks" "spectrum" "checkerboard"
          "float uscale" [16] "float vscale" [16]
          "rgb tex1" [.1 .1 .1] "rgb tex2" [.8 .8 .8]
  Material "diffuse" "texture reflectance" "checks"
  Translate 0 0 -1
  Shape "bilinearmesh"
      "point3 P" [ -20 -20 0   20 -20 0   -20 20 0   20 20 0 ]
      "point2 uv" [ 0 0   1 0    1 1   0 1 ]
AttributeEnd
)";

TEST(scene_basic, spec_example) {
	Scene s(SPEC_EXAMPLE);
	ASSERT_TRUE(s.ok()) << s.error();

	EXPECT_EQ(s->shapes.count, 2u);
	EXPECT_EQ(s->lights.lights_sources.count, 2u);
	EXPECT_EQ(s->textures.count, 1u);
	EXPECT_EQ(s->materials.count, 2u);

	EXPECT_EQ(s->camera.type, TPBRT_CAMERA_TYPE_PERSPECTIVE);
	EXPECT_FLOAT_EQ(s->camera.as.perspective.fov, 45.0f);
	EXPECT_EQ(s->sampler.type, TPBRT_SAMPLER_TYPE_HALTON);
	EXPECT_EQ(s->sampler.as.halton.pixel_samples, 128u);
	EXPECT_EQ(s->integrator.type, TPBRT_INTEGRATOR_TYPE_VOL_PATH);
	EXPECT_EQ(s->film.x_resolution, 400u);
	EXPECT_EQ(s->film.y_resolution, 400u);
	EXPECT_EQ(to_std(s->film.file_name), "simple.png");

	const tpbrt_light_source_t& distant = s->lights.lights_sources.lights[1];
	EXPECT_EQ(distant.type, TPBRT_LIGHT_SOURCE_TYPE_DISTANT);
	EXPECT_FLOAT_EQ(distant.scale, 1.5f);
	EXPECT_EQ(distant.as.distant.l.type, TPBRT_SPECTRUM_TYPE_BLACKBODY);

	const tpbrt_shape_t& mesh = s->shapes.shapes[1];
	ASSERT_EQ(mesh.type, TPBRT_SHAPE_TYPE_BILINEAR_MESH);
	EXPECT_EQ(mesh.as.bilinear_mesh.p.count, 4u);
	EXPECT_EQ(mesh.as.bilinear_mesh.indices.count, 4u);
	EXPECT_EQ(mesh.as.bilinear_mesh.uv.count, 4u);
	TPBRT_EXPECT_VEC3_NEAR(origin(*s, mesh.transform.start_transform), 0.0f, 0.0f, -1.0f);
}

TEST(scene_basic, defaults) {
	const Scene s("WorldBegin\n");
	ASSERT_TRUE(s.ok()) << s.error();

	EXPECT_EQ(s->camera.type, TPBRT_CAMERA_TYPE_PERSPECTIVE);
	EXPECT_FLOAT_EQ(s->camera.as.perspective.fov, 90.0f);
	EXPECT_EQ(s->sampler.type, TPBRT_SAMPLER_TYPE_Z_SOBOL);
	EXPECT_EQ(s->integrator.type, TPBRT_INTEGRATOR_TYPE_VOL_PATH);
	EXPECT_EQ(s->accelerator.type, TPBRT_ACCELERATOR_TYPE_BVH);
	EXPECT_EQ(s->accelerator.as.bvh.max_node_prims, 4u);
	EXPECT_EQ(s->film.type, TPBRT_FILM_TYPE_RGB);
	EXPECT_EQ(s->film.x_resolution, 1280u);
	EXPECT_EQ(s->film.y_resolution, 720u);
	EXPECT_EQ(to_std(s->film.file_name), "pbrt.exr");
	EXPECT_FLOAT_EQ(s->start_time, 0.0f);
	EXPECT_FLOAT_EQ(s->end_time, 1.0f);
	EXPECT_EQ(s->shapes.count, 0u);
}

TEST(scene_basic, empty_input_and_null_arguments) {
	const Scene empty("");
	EXPECT_TRUE(empty.ok()) << empty.error();

	tpbrt_scene_t scene;
	EXPECT_EQ(tpbrt_scene_load_from_memory(nullptr, ".", &scene), TPBRT_ERROR_INVALID_POINTER);
	EXPECT_EQ(tpbrt_scene_load_from_memory("WorldBegin", ".", nullptr), TPBRT_ERROR_INVALID_POINTER);
	EXPECT_EQ(tpbrt_scene_load_from_file(nullptr, &scene), TPBRT_ERROR_INVALID_POINTER);
}

TEST(scene_basic, comments_and_whitespace) {
	const Scene s("# komentarz\n\tWorldBegin # po dyrektywie\n\n  Shape \"sphere\" # \"float radius\" 5\n");
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->shapes.count, 1u);
	EXPECT_FLOAT_EQ(s->shapes.shapes[0].as.sphere.radius, 1.0f);
}

TEST(scene_basic, unknown_directive) {
	const Scene s("WorldBegin\nFooBar 1 2 3\n");
	EXPECT_EQ(s.error(), TPBRT_ERROR_INVALID_DIRECTIVE);
}

TEST(scene_basic, world_directives_before_world_begin) {
	EXPECT_EQ(Scene("Shape \"sphere\"\n").error(), TPBRT_ERROR_UNEXPECTED_TOKEN);
	EXPECT_EQ(Scene("Material \"diffuse\"\n").error(), TPBRT_ERROR_UNEXPECTED_TOKEN);
	EXPECT_EQ(Scene("LightSource \"point\"\n").error(), TPBRT_ERROR_UNEXPECTED_TOKEN);
	EXPECT_EQ(Scene("ObjectBegin \"o\"\n").error(), TPBRT_ERROR_UNEXPECTED_TOKEN);
}

TEST(scene_basic, options_inside_world) {
	EXPECT_EQ(Scene("WorldBegin\nCamera \"perspective\"\n").error(), TPBRT_ERROR_UNEXPECTED_TOKEN);
	EXPECT_EQ(Scene("WorldBegin\nFilm \"rgb\"\n").error(), TPBRT_ERROR_UNEXPECTED_TOKEN);
	EXPECT_EQ(Scene("WorldBegin\nSampler \"halton\"\n").error(), TPBRT_ERROR_UNEXPECTED_TOKEN);
	EXPECT_EQ(Scene("WorldBegin\nIntegrator \"path\"\n").error(), TPBRT_ERROR_UNEXPECTED_TOKEN);
	EXPECT_EQ(Scene("WorldBegin\nAccelerator \"bvh\"\n").error(), TPBRT_ERROR_UNEXPECTED_TOKEN);
	EXPECT_EQ(Scene("WorldBegin\nOption \"bool disablepixeljitter\" true\n").error(), TPBRT_ERROR_UNEXPECTED_TOKEN);
	EXPECT_EQ(Scene("WorldBegin\nTransformTimes 0 1\n").error(), TPBRT_ERROR_UNEXPECTED_TOKEN);
}

TEST(scene_basic, world_begin_twice) { EXPECT_EQ(Scene("WorldBegin\nWorldBegin\n").error(), TPBRT_ERROR_WORLD_ALREADY_STARTED); }

TEST(scene_basic, color_space_anywhere) {
	EXPECT_TRUE(Scene("ColorSpace \"rec2020\"\nWorldBegin\n").ok());
	EXPECT_TRUE(Scene("WorldBegin\nColorSpace \"aces2065-1\"\n").ok());
	EXPECT_EQ(Scene("ColorSpace \"xyz\"\nWorldBegin\n").error(), TPBRT_ERROR_INVALID_COLOR_SPACE);
}

TEST(scene_basic, transform_times) {
	const Scene s("TransformTimes 0.25 0.75\nWorldBegin\n");
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_FLOAT_EQ(s->start_time, 0.25f);
	EXPECT_FLOAT_EQ(s->end_time, 0.75f);
}

TEST(scene_basic, escaped_strings) {
	const Scene s("Film \"rgb\" \"string filename\" \"out \\\"1\\\".png\"\nWorldBegin\n");
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(to_std(s->film.file_name), "out \"1\".png");
}

TEST(scene_basic, error_in_the_middle_frees_everything) {
	/* clang-format off */
	const Scene s("Camera \"perspective\" \"float fov\" 30\nWorldBegin\n"
			"Texture \"t\" \"spectrum\" \"constant\" \"spectrum value\" [ 400 1 700 1 ]\n"
			"Material \"diffuse\" \"spectrum reflectance\" [ 400 .5 700 .5 ]\n"
			"Shape \"trianglemesh\" \"point3 P\" [ 0 0 0 1 0 0 0 1 0 ]\n"
			"Shape \"nope\"\n");
	/* clang-format on */
	EXPECT_EQ(s.error(), TPBRT_ERROR_INVALID_SHAPE_TYPE);
}
