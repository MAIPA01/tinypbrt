#include <pch.hpp>

#include <tinypbrt/detail/scene.h>
#include <tinypbrt/detail/shape_internal.h>

#include <test_helpers.hpp>

using namespace tpbrt_test;

TEST(scene_shapes, sphere_defaults_and_params) {
	/* clang-format off */
	const Scene s("WorldBegin\nShape \"sphere\"\nShape \"sphere\" \"float radius\" 3 \"float zmin\" -1 \"float zmax\" 2 \"float "
				  "phimax\" 180\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->shapes.count, 2u);
	const tpbrt_shape_sphere_params_t& d = s->shapes.shapes[0].as.sphere;
	EXPECT_FLOAT_EQ(d.radius, 1.0f);
	EXPECT_FLOAT_EQ(d.z_min, -1.0f);
	EXPECT_FLOAT_EQ(d.z_max, 1.0f);
	EXPECT_FLOAT_EQ(d.phi_max, 360.0f);
	const tpbrt_shape_sphere_params_t& p = s->shapes.shapes[1].as.sphere;
	EXPECT_FLOAT_EQ(p.radius, 3.0f);
	EXPECT_FLOAT_EQ(p.z_min, -1.0f);
	EXPECT_FLOAT_EQ(p.z_max, 2.0f);
	EXPECT_FLOAT_EQ(p.phi_max, 180.0f);
}

TEST(scene_shapes, cylinder_and_disk) {
	const Scene s("WorldBegin\nShape \"cylinder\" \"float radius\" 2\nShape \"disk\" \"float innerradius\" 0.5\n");
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->shapes.count, 2u);
	EXPECT_EQ(s->shapes.shapes[0].type, TPBRT_SHAPE_TYPE_CYLINDER);
	EXPECT_FLOAT_EQ(s->shapes.shapes[0].as.cylinder.radius, 2.0f);
	EXPECT_EQ(s->shapes.shapes[1].type, TPBRT_SHAPE_TYPE_DISK);
	EXPECT_FLOAT_EQ(s->shapes.shapes[1].as.disk.inner_radius, 0.5f);
	EXPECT_FLOAT_EQ(s->shapes.shapes[1].as.disk.radius, 1.0f);
}

TEST(scene_shapes, trianglemesh) {
	/* clang-format off */
	const Scene s("WorldBegin\nShape \"trianglemesh\" \"point3 P\" [ 0 0 0  1 0 0  0 1 0  1 1 0 ] \"integer indices\" [ 0 1 2  2 "
				  "1 3 ]" " \"point2 uv\" [ 0 0 1 0 0 1 1 1 ] \"normal N\" [ 0 0 1 0 0 1 0 0 1 0 0 1 ]\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_shape_triangle_mesh_params_t& m = s->shapes.shapes[0].as.triangle_mesh;
	EXPECT_EQ(m.p.count, 4u);
	EXPECT_EQ(m.indices.count, 6u);
	EXPECT_EQ(m.uv.count, 4u);
	EXPECT_EQ(m.n.count, 4u);
	EXPECT_EQ(m.indices.data[5], 3u);
}

TEST(scene_shapes, single_triangle_gets_default_indices) {
	const Scene s("WorldBegin\nShape \"trianglemesh\" \"point3 P\" [ 0 0 0 1 0 0 0 1 0 ]\n");
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_shape_triangle_mesh_params_t& m = s->shapes.shapes[0].as.triangle_mesh;
	ASSERT_EQ(m.indices.count, 3u);
	EXPECT_EQ(m.indices.data[0], 0u);
	EXPECT_EQ(m.indices.data[1], 1u);
	EXPECT_EQ(m.indices.data[2], 2u);
}

TEST(scene_shapes, trianglemesh_errors) {
	EXPECT_EQ(Scene("WorldBegin\nShape \"trianglemesh\" \"point3 P\" [ 0 0 0 1 0 0 0 1 0 1 1 0 ]\n").error(),
	  TPBRT_ERROR_MISSING_REQUIRED_PARAMETER);
	EXPECT_EQ(
	  Scene("WorldBegin\nShape \"trianglemesh\" \"point3 P\" [ 0 0 0 1 0 0 0 1 0 ] \"integer indices\" [ 0 1 2 0 ]\n").error(),
	  TPBRT_ERROR_MISSING_REQUIRED_PARAMETER
	);
	EXPECT_EQ(
	  Scene("WorldBegin\nShape \"trianglemesh\" \"point3 P\" [ 0 0 0 1 0 0 0 1 0 ] \"integer indices\" [ 0 1 5 ]\n").error(),
	  TPBRT_ERROR_INDEX_OUT_OF_RANGE
	);
	EXPECT_NE(Scene("WorldBegin\nShape \"trianglemesh\" \"integer indices\" [ 0 1 2 ]\n").error(), TPBRT_ERROR_NONE);
}

TEST(scene_shapes, bilinearmesh) {
	/* clang-format off */
	const Scene s(
	  "WorldBegin\nShape \"bilinearmesh\" \"point3 P\" [ 0 0 0 1 0 0 2 0 0 0 1 0 1 1 0 2 1 0 ]" " \"integer indices\" [ 0 1 3 4  "
																								"1 2 4 5 ]\n"
	);
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_shape_bilinear_mesh_params_t& m = s->shapes.shapes[0].as.bilinear_mesh;
	EXPECT_EQ(m.p.count, 6u);
	EXPECT_EQ(m.indices.count, 8u);
}

TEST(scene_shapes, bilinearmesh_errors) {
	EXPECT_EQ(Scene("WorldBegin\nShape \"bilinearmesh\" \"point3 P\" [ 0 0 0 1 0 0 0 1 0 1 1 0 0 0 1 ]\n").error(),
	  TPBRT_ERROR_MISSING_REQUIRED_PARAMETER);
	EXPECT_EQ(
	  Scene("WorldBegin\nShape \"bilinearmesh\" \"point3 P\" [ 0 0 0 1 0 0 0 1 0 1 1 0 ] \"integer indices\" [ 0 1 2 7 ]\n")
		.error(),
	  TPBRT_ERROR_INDEX_OUT_OF_RANGE
	);
}

TEST(scene_shapes, curve_defaults) {
	const Scene s("WorldBegin\nShape \"curve\" \"point3 P\" [ 0 0 0 1 0 0 2 0 0 3 0 0 ]\n");
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_shape_curve_params_t& c = s->shapes.shapes[0].as.curve;
	EXPECT_EQ(c.basis, TPBRT_SHAPE_CURVE_BASIS_BEZIER);
	EXPECT_EQ(c.type, TPBRT_SHAPE_CURVE_TYPE_FLAT);
	EXPECT_FLOAT_EQ(c.p[3].x, 3.0f);
}

TEST(scene_shapes, curve_explicit_basis_and_type) {
	/* clang-format off */
	const Scene s("WorldBegin\nShape \"curve\" \"point3 P\" [ 0 0 0 1 0 0 2 0 0 3 0 0 ] \"string basis\" \"bspline\" \"string "
				  "type\" \"cylinder\"\n");
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(s->shapes.shapes[0].as.curve.basis, TPBRT_SHAPE_CURVE_BASIS_BSPLINE);
	EXPECT_EQ(s->shapes.shapes[0].as.curve.type, TPBRT_SHAPE_CURVE_TYPE_CYLINDER);
	EXPECT_EQ(Scene("WorldBegin\nShape \"curve\" \"point3 P\" [ 0 0 0 1 0 0 2 0 0 3 0 0 ] \"string basis\" \"x\"\n").error(),
	  TPBRT_ERROR_INVALID_SHAPE_CURVE_BASIS);
}

TEST(scene_shapes, plymesh_requires_filename) {
	EXPECT_EQ(Scene("WorldBegin\nShape \"plymesh\"\n").error(), TPBRT_ERROR_NOT_FOUND);

	const Scene s("WorldBegin\nShape \"plymesh\" \"string filename\" \"mesh.ply\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	EXPECT_EQ(to_std(s->shapes.shapes[0].as.ply_mesh.file_name), "mesh.ply");
}

TEST(scene_shapes, alpha_is_float_texture) {
	const Scene s("WorldBegin\nShape \"sphere\" \"float alpha\" 0.25\nShape \"disk\"\n");
	ASSERT_TRUE(s.ok()) << s.error();
	const tpbrt_texture_handle_t& a = s->shapes.shapes[0].alpha;
	ASSERT_EQ(a.value_type, TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM);
	EXPECT_FLOAT_EQ(a.as.spectrum.as.constant, 0.25f);
	EXPECT_FLOAT_EQ(s->shapes.shapes[1].alpha.as.spectrum.as.constant, 1.0f);

	EXPECT_NE(Scene("WorldBegin\nShape \"sphere\" \"spectrum alpha\" [ 400 1 700 1 ]\n").error(), TPBRT_ERROR_NONE);
}

TEST(scene_shapes, area_light_attached) {
	/* clang-format off */
	const Scene s(
	  "WorldBegin\nAttributeBegin\nAreaLightSource \"diffuse\" \"blackbody L\" 6500\nShape \"sphere\"\nAttributeEnd\nShape \"disk\"\n"
	);
	/* clang-format on */
	ASSERT_TRUE(s.ok()) << s.error();
	ASSERT_EQ(s->lights.area_lights.count, 1u);
	EXPECT_EQ(s->shapes.shapes[0].area_light.area_light_idx, s->lights.area_lights.lights[0].idx);
	EXPECT_EQ(s->shapes.shapes[1].area_light.area_light_idx, TPBRT_AREA_LIGHT_IDX_INVALID);
}

TEST(scene_shapes, invalid_type) { EXPECT_EQ(Scene("WorldBegin\nShape \"torus\"\n").error(), TPBRT_ERROR_INVALID_SHAPE_TYPE); }
