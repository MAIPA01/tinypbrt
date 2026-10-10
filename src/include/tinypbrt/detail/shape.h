#pragma once
#ifndef _TINYPBRT_SHAPE_H_
#define _TINYPBRT_SHAPE_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/light.h>
#include <tinypbrt/detail/material.h>
#include <tinypbrt/detail/math.h>
#include <tinypbrt/detail/media.h>
#include <tinypbrt/detail/texture.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum tpbrt_shape_type_t : uint8_t {
		TPBRT_SHAPE_TYPE_BILINEAR_MESH = 0,
		TPBRT_SHAPE_TYPE_CURVE		   = 1,
		TPBRT_SHAPE_TYPE_CYLINDER	   = 2,
		TPBRT_SHAPE_TYPE_DISK		   = 3,
		TPBRT_SHAPE_TYPE_SPHERE		   = 4,
		TPBRT_SHAPE_TYPE_TRIANGLE_MESH = 5,
		TPBRT_SHAPE_TYPE_PLY_MESH	   = 6,
		TPBRT_SHAPE_TYPE_LOOP_SUBDIV   = 7,
		TPBRT_SHAPE_TYPE_MAX_NUM	   = 8,
	} tpbrt_shape_type_t;

	typedef enum tpbrt_shape_curve_basis_t : uint8_t {
		TPBRT_SHAPE_CURVE_BASIS_BEZIER	= 0,
		TPBRT_SHAPE_CURVE_BASIS_BSPLINE = 1,
		TPBRT_SHAPE_CURVE_BASIS_MAX_NUM = 2,
	} tpbrt_shape_curve_basis_t;

	typedef enum tpbrt_shape_curve_degree_t : uint8_t {
		TPBRT_SHAPE_CURVE_DEGREE_2 = 2,
		TPBRT_SHAPE_CURVE_DEGREE_3 = 3,
	} tpbrt_shape_curve_degree_t;

	typedef enum tpbrt_shape_curve_type_t : uint8_t {
		TPBRT_SHAPE_CURVE_TYPE_FLAT		= 0,
		TPBRT_SHAPE_CURVE_TYPE_CYLINDER = 1,
		TPBRT_SHAPE_CURVE_TYPE_RIBBON	= 2,
		TPBRT_SHAPE_CURVE_TYPE_MAX_NUM	= 3,
	} tpbrt_shape_curve_type_t;

	typedef struct tpbrt_shape_curve_params_t {
		tpbrt_point3_t P[4];
		tpbrt_normal_t N[2];
		tpbrt_float_t width;
		tpbrt_opt_float_t width0;
		tpbrt_opt_float_t width1;
		tpbrt_uint_t split_depth;
		tpbrt_shape_curve_basis_t basis	  : 1;
		tpbrt_shape_curve_degree_t degree : 2;
		tpbrt_shape_curve_type_t type	  : 2;
	} tpbrt_shape_curve_params_t;

	typedef struct tpbrt_shape_cylinder_params_t {
		tpbrt_float_t radius;
		tpbrt_float_t z_min;
		tpbrt_float_t z_max;
		tpbrt_float_t phi_max;
	} tpbrt_shape_cylinder_params_t;

	typedef struct tpbrt_shape_disk_params_t {
		tpbrt_float_t height;
		tpbrt_float_t radius;
		tpbrt_float_t inner_radius;
		tpbrt_float_t phi_max;
	} tpbrt_shape_disk_params_t;

	typedef struct tpbrt_shape_sphere_params_t {
		tpbrt_float_t radius;
		tpbrt_float_t z_min;
		tpbrt_float_t z_max;
		tpbrt_float_t phi_max;
	} tpbrt_shape_sphere_params_t;

	typedef struct tpbrt_shape_triangle_mesh_params_t {
		tpbrt_uint_array_t indices;
		tpbrt_point3_array_t P;
		tpbrt_normal_array_t N;
		tpbrt_vec3_array_t S;
		tpbrt_point2_array_t uv;
	} tpbrt_shape_triangle_mesh_params_t;

	typedef struct tpbrt_shape_ply_mesh_params_t {
		tpbrt_string_t file_name;
		tpbrt_texture_handle_t displacement;
		tpbrt_float_t edge_length;
	} tpbrt_shape_ply_mesh_params_t;

	typedef struct tpbrt_shape_loop_subdiv_params_t {
		tpbrt_uint_array_t indices;
		tpbrt_point_array_t P;
		tpbrt_uint_t levels;
	} tpbrt_shape_loop_subdiv_params_t;

	typedef struct tpbrt_shape_t {
		tpbrt_texture_handle_t alpha;
		tpbrt_mat4_animated_t transform;
		tpbrt_material_handle_t material;
		tpbrt_area_light_handle_t area_light;
		tpbrt_media_handle_t interior_media_handle;
		tpbrt_media_handle_t exterior_media_handle;

		union {
			tpbrt_shape_curve_params_t curve;
			tpbrt_shape_cylinder_params_t cylinder;
			tpbrt_shape_disk_params_t disk;
			tpbrt_shape_sphere_params_t sphere;
			tpbrt_shape_triangle_mesh_params_t triangle_mesh;
			tpbrt_shape_ply_mesh_params_t ply_mesh;
			tpbrt_shape_loop_subdiv_params_t loop_subdiv;
		} as;

		tpbrt_shape_type_t type			 : 3;
		tpbrt_bool_t reverse_orientation : 1;
	} tpbrt_shape_t;

	typedef struct tpbrt_shapes_list_t {
		tpbrt_shape_t* shapes;
		tpbrt_size_t count;
	} tpbrt_shapes_list_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SHAPE_H_