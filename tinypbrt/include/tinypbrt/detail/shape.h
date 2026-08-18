#pragma once
#ifndef _TINYPBRT_SHAPE_H_
#define _TINYPBRT_SHAPE_H_

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/math.h>
#include <tinypbrt/detail/texture.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_SHAPE_BILINEAR_MESH = 0,
		TPBRT_SHAPE_CURVE		  = 1,
		TPBRT_SHAPE_CYLINDER	  = 2,
		TPBRT_SHAPE_DISK		  = 3,
		TPBRT_SHAPE_SPHERE		  = 4,
		TPBRT_SHAPE_TRIANGLE_MESH = 5,
		TPBRT_SHAPE_PLY_MESH	  = 6,
		TPBRT_SHAPE_LOOP_SUBDIV	  = 7,
	} tpbrt_shape_type_t;

	typedef enum : uint8_t { TPBRT_SHAPE_CURVE_BASIS_BEZIER = 0, TPBRT_SHAPE_CURVE_BASIS_BSPLINE = 1 } tpbrt_shape_curve_basis_t;

	typedef enum : uint8_t { TPBRT_SHAPE_CURVE_DEGREE_2 = 2, TPBRT_SHAPE_CURVE_DEGREE_3 = 3 } tpbrt_shape_curve_degree_t;

	typedef enum : uint8_t {
		TPBRT_SHAPE_CURVE_TYPE_FLAT		= 0,
		TPBRT_SHAPE_CURVE_TYPE_CYLINDER = 1,
		TPBRT_SHAPE_CURVE_TYPE_RIBBON	= 2,
	} tpbrt_shape_curve_type_t;

	typedef struct {
		tpbrt_point3_t P[4];
		tpbrt_shape_curve_basis_t basis;
		tpbrt_shape_curve_degree_t degree;
		tpbrt_shape_curve_type_t type;
		tpbrt_normal_t N[2];
		tpbrt_float_t width;
		tpbrt_opt_float_t width0;
		tpbrt_opt_float_t width1;
		tpbrt_uint_t split_depth;
	} tpbrt_shape_curve_params_t;

	typedef struct {
		tpbrt_float_t radius;
		tpbrt_float_t z_min;
		tpbrt_float_t z_max;
		tpbrt_float_t phi_max;
	} tpbrt_shape_cylinder_params_t;

	typedef struct {
		tpbrt_float_t height;
		tpbrt_float_t radius;
		tpbrt_float_t inner_radius;
		tpbrt_float_t phi_max;
	} tpbrt_shape_disk_params_t;

	typedef struct {
		tpbrt_float_t radius;
		tpbrt_float_t z_min;
		tpbrt_float_t z_max;
		tpbrt_float_t phi_max;
	} tpbrt_shape_sphere_params_t;

	typedef struct {
		tpbrt_uint_array_t indices;
		tpbrt_point3_array_t P;
		tpbrt_normal_array_t N;
		tpbrt_vec3_array_t S;
		tpbrt_point2_array_t uv;
	} tpbrt_shape_triangle_mesh_params_t;

	typedef struct {
		tpbrt_string_t file_name;
		tpbrt_texture_t displacement;
		tpbrt_float_t edge_length;
	} tpbrt_shape_ply_mesh_params_t;

	typedef struct {
		tpbrt_uint_t levels;
		tpbrt_uint_array_t indicies;
		tpbrt_point_array_t P;
	} tpbrt_shape_loop_subdiv_params_t;

	typedef enum : uint8_t {
		TPBRT_FLOAT_TEXTURE_TYPE_FLOAT	 = 0,
		TPBRT_FLOAT_TEXTURE_TYPE_TEXTURE = 1,
	} tpbrt_shape_alpha_type_t;

	typedef struct {
		tpbrt_shape_alpha_type_t type;

		union {
			float value;
			tpbrt_texture_t texture;
		};
	} tpbrt_shape_alpha_t;

	typedef struct {
		tpbrt_shape_type_t type;
		tpbrt_shape_alpha_t alpha;

		union {
			tpbrt_shape_curve_params_t curve_params;
			tpbrt_shape_cylinder_params_t cylinder_params;
			tpbrt_shape_disk_params_t disk_params;
			tpbrt_shape_sphere_params_t sphere_params;
			tpbrt_shape_triangle_mesh_params_t tri_mesh_params;
			tpbrt_shape_ply_mesh_params_t ply_mesh_params;
			tpbrt_shape_loop_subdiv_params_t loop_subdiv_params;
		};
	} tpbrt_shape_t;

	typedef struct {
		tpbrt_shape_t* values;
		size_t count;
	} tpbrt_shape_array_t;

	typedef struct {
		tpbrt_string_t name;
		tpbrt_shape_array_t shapes;
	} tpbrt_object_t;

	typedef struct {
		tpbrt_object_t* object;
		tpbrt_mat4_t transform;
	} tpbrt_object_instance_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SHAPE_H_
