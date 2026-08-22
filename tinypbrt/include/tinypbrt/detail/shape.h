#pragma once
#ifndef _TINYPBRT_SHAPE_H_
#define _TINYPBRT_SHAPE_H_

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/material.h>
#include <tinypbrt/detail/math.h>
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
		TPBRT_SHAPE_CURVE_DEGREE_3 = 3
	} tpbrt_shape_curve_degree_t;

	typedef enum tpbrt_shape_curve_type_t : uint8_t {
		TPBRT_SHAPE_CURVE_TYPE_FLAT		= 0,
		TPBRT_SHAPE_CURVE_TYPE_CYLINDER = 1,
		TPBRT_SHAPE_CURVE_TYPE_RIBBON	= 2,
		TPBRT_SHAPE_CURVE_TYPE_MAX_NUM	= 3,
	} tpbrt_shape_curve_type_t;

	typedef struct tpbrt_shape_curve_params_t {
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
		tpbrt_uint_t levels;
		tpbrt_uint_array_t indices;
		tpbrt_point_array_t P;
	} tpbrt_shape_loop_subdiv_params_t;

	typedef struct tpbrt_shape_t {
		tpbrt_shape_type_t type;
		tpbrt_texture_handle_t alpha;
		tpbrt_mat4_t transform;
		tpbrt_material_handle_t material;

		union {
			tpbrt_shape_curve_params_t curve;
			tpbrt_shape_cylinder_params_t cylinder;
			tpbrt_shape_disk_params_t disk;
			tpbrt_shape_sphere_params_t sphere;
			tpbrt_shape_triangle_mesh_params_t triangle_mesh;
			tpbrt_shape_ply_mesh_params_t ply_mesh;
			tpbrt_shape_loop_subdiv_params_t loop_subdiv;
		} as;
	} tpbrt_shape_t;

	typedef struct tpbrt_shape_array_t {
		tpbrt_shape_t* data;
		tpbrt_size_t count;
	} tpbrt_shape_array_t;

	typedef struct tpbrt_object_t {
		tpbrt_size_t idx;
		tpbrt_string_t name;
		tpbrt_shape_array_t shapes;
	} tpbrt_object_t;

	typedef struct tpbrt_objects_list_t {
		tpbrt_object_t* objects;
		tpbrt_size_t count;
	} tpbrt_objects_list_t;

	typedef tpbrt_uint_t tpbrt_object_handle_t;

	typedef struct tpbrt_instance_t {
		tpbrt_object_handle_t object;
		tpbrt_mat4_t transform;
	} tpbrt_instance_t;

	typedef struct tpbrt_instances_list_t {
		tpbrt_instance_t* instances;
		tpbrt_size_t count;
	} tpbrt_instances_list_t;

	tpbrt_error_t tpbrt_get_object_by_name(const tpbrt_objects_list_t* objects, const tpbrt_string_t* name,
	  const tpbrt_object_t** object);
	tpbrt_error_t tpbrt_get_object_by_handle(const tpbrt_objects_list_t* objects, const tpbrt_object_handle_t* handle,
	  const tpbrt_object_t** object);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SHAPE_H_
