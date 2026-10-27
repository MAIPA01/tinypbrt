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

	/**
	 * @defgroup shape-public Shape
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Shape types.
	 * @ingroup shape-public
	 */
	typedef enum tpbrt_shape_type_t {
		/// @brief Bilinear mesh.
		TPBRT_SHAPE_TYPE_BILINEAR_MESH = 0,
		/// @brief Curve.
		TPBRT_SHAPE_TYPE_CURVE		   = 1,
		/// @brief Cylinder.
		TPBRT_SHAPE_TYPE_CYLINDER	   = 2,
		/// @brief Disk.
		TPBRT_SHAPE_TYPE_DISK		   = 3,
		/// @brief Sphere.
		TPBRT_SHAPE_TYPE_SPHERE		   = 4,
		/// @brief Triangle mesh.
		TPBRT_SHAPE_TYPE_TRIANGLE_MESH = 5,
		/// @brief Ply file mesh.
		TPBRT_SHAPE_TYPE_PLY_MESH	   = 6,
		/// @brief Loop subdivision.
		TPBRT_SHAPE_TYPE_LOOP_SUBDIV   = 7,
		/// @brief Count of available shape types.
		TPBRT_SHAPE_TYPE_MAX_NUM	   = 8,
	} tpbrt_shape_type_t;

	/**
	 * @brief Curve shape basis.
	 * @ingroup shape-public
	 */
	typedef enum tpbrt_shape_curve_basis_t {
		/// @brief Bezier curve.
		TPBRT_SHAPE_CURVE_BASIS_BEZIER	= 0,
		/// @brief BSpline curve.
		TPBRT_SHAPE_CURVE_BASIS_BSPLINE = 1,
		/// @brief Count of available curve shape basis.
		TPBRT_SHAPE_CURVE_BASIS_MAX_NUM = 2,
	} tpbrt_shape_curve_basis_t;

	/**
	 * @brief Curve shape degree.
	 * @ingroup shape-public
	 */
	typedef enum tpbrt_shape_curve_degree_t {
		/// @brief 2 Degree.
		TPBRT_SHAPE_CURVE_DEGREE_2 = 2,
		/// @brief 3 Degree.
		TPBRT_SHAPE_CURVE_DEGREE_3 = 3,
	} tpbrt_shape_curve_degree_t;

	/**
	 * @brief Curve shape type.
	 * @ingroup shape-public
	 */
	typedef enum tpbrt_shape_curve_type_t {
		/// @brief Curve is always oriented to face the incident ray.
		TPBRT_SHAPE_CURVE_TYPE_FLAT		= 0,
		/// @brief Alternative includes a shading normal that causes the curve to appear to have a cylindrical cross section.
		TPBRT_SHAPE_CURVE_TYPE_CYLINDER = 1,
		/// @brief Curve has a fixed orientation.
		TPBRT_SHAPE_CURVE_TYPE_RIBBON	= 2,
		/// @brief Count of available curve types.
		TPBRT_SHAPE_CURVE_TYPE_MAX_NUM	= 3,
	} tpbrt_shape_curve_type_t;

	/**
	 * @brief Bilinear mesh parameters.
	 * @ingroup shape-public
	 */
	typedef struct tpbrt_shape_bilinear_mesh_params_t {
		/// @brief The array of integer offsets into the per-vertex data arrays (P, and any of N, S, or uv that are present.) Required, unless exactly four vertices are specified.
		tpbrt_uint_array_t indices;
		/// @brief The vertex positions of the bilinear mesh.
		tpbrt_point3_array_t p;
		/// @brief Per-vertex normals. If present, shading normals will be computed from these values.
		tpbrt_normal_array_t n;
		/// @brief Per-vertex texture coordinates.
		tpbrt_point2_array_t uv;
	} tpbrt_shape_bilinear_mesh_params_t;

	/**
	 * @brief Curve shape parameters.
	 * @ingroup shape-public
	 */
	typedef struct tpbrt_shape_curve_params_t {
		/// @brief Control points for the cubic Bezier spline that goes along the center of the curve shape.
		tpbrt_point3_t p[4];
		/// @brief For "ribbon" curves, these normals are respectively used at endpoints of the curve to orient the surface there. Intermediate normals are interpolated using spherical linear interpolation.
		tpbrt_normal_t n[2];
		/// @brief Width of the curve.
		tpbrt_float_t width;
		/// @brief Width of the curve at the start point. If specified, these override the "width" parameter.
		tpbrt_opt_float_t width0;
		/// @brief Width of the curve at the end point. If specified, these override the "width" parameter.
		tpbrt_opt_float_t width1;
		/// @brief Number of times the curve is split in half into sub-curves at startup time. Splitting curves increases memory use but can improve ray intersection performance, as the sub-curves generally have tighter bounding boxes than the entire curve extent.
		tpbrt_uint_t split_depth;
		/// @brief Curve spline basis.
		tpbrt_shape_curve_basis_t basis	  : 3;
		/// @brief Degree of the curve's spline.
		tpbrt_shape_curve_degree_t degree : 3;
		/// @brief Which curve variant is used.
		tpbrt_shape_curve_type_t type	  : 3;
	} tpbrt_shape_curve_params_t;

	/**
	 * @brief Cylinder shape parameters.
	 * @ingroup shape-public
	 */
	typedef struct tpbrt_shape_cylinder_params_t {
		/// @brief The cylinder's radius.
		tpbrt_float_t radius;
		/// @brief The height of the cylinder's bottom along the z axis.
		tpbrt_float_t z_min;
		/// @brief The height of the cylinder's top along the z axis.
		tpbrt_float_t z_max;
		/// @brief The maximum extent of the cylinder in phi (in spherical coordinates).
		tpbrt_float_t phi_max;
	} tpbrt_shape_cylinder_params_t;

	/**
	 * @brief Disk shape parameters.
	 * @ingroup shape-public
	 */
	typedef struct tpbrt_shape_disk_params_t {
		/// @brief The position of the disk along the z axis.
		tpbrt_float_t height;
		/// @brief The outer radius of the disk.
		tpbrt_float_t radius;
		/// @brief The inner radius of the disk (if nonzero, the disk is an annulus).
		tpbrt_float_t inner_radius;
		/// @brief The maximum extent of the disk in phi (in spherical coordinates).
		tpbrt_float_t phi_max;
	} tpbrt_shape_disk_params_t;

	/**
	 * @brief Sphere shape parameters.
	 * @ingroup shape-public
	 */
	typedef struct tpbrt_shape_sphere_params_t {
		/// @brief The sphere's radius.
		tpbrt_float_t radius;
		/// @brief The height of the lower clipping plane along the z axis.
		tpbrt_float_t z_min;
		/// @brief The height of the upper clipping plane along the z axis.
		tpbrt_float_t z_max;
		/// @brief The maximum extent of the sphere in phi (in spherical coordinates).
		tpbrt_float_t phi_max;
	} tpbrt_shape_sphere_params_t;

	/**
	 * @brief Triangle mesh parameters.
	 * @ingroup shape-public
	 */
	typedef struct tpbrt_shape_triangle_mesh_params_t {
		/// @brief The array of integer offsets into the per-vertex data arrays (P, and any of N, S, or uv that are present.) Required, unless exactly three vertices are specified.
		tpbrt_uint_array_t indices;
		/// @brief The vertex positions of the triangle mesh.
		tpbrt_point3_array_t p;
		/// @brief Per-vertex normals. If present, shading normals will be computed from these values.
		tpbrt_normal_array_t n;
		/// @brief Per-vertex tangents.
		tpbrt_vec3_array_t s;
		/// @brief Per-vertex texture coordinates.
		tpbrt_point2_array_t uv;
	} tpbrt_shape_triangle_mesh_params_t;

	/**
	 * @brief Ply file mesh parameters.
	 * @ingroup shape-public
	 */
	typedef struct tpbrt_shape_ply_mesh_params_t {
		/// @brief File from which the PLY-format mesh is loaded. If the file name's extension is ".gz", the file will be decompressed from gzip format as it is read.
		tpbrt_string_t file_name;
		/// @brief Specifies a texture used to displace the vertices of the mesh. The mesh will be adaptively tessellated so that all edges are below a specified length.
		tpbrt_texture_handle_t displacement;
		/// @brief Maximum length (in world space) of an edge of a triangle mesh that is being displaced. Edges are recursively split until shorter than this length.
		tpbrt_float_t edge_length;
	} tpbrt_shape_ply_mesh_params_t;

	/**
	 * @brief Loop subdiv parameters.
	 * @ingroup shape-public
	 */
	typedef struct tpbrt_shape_loop_subdiv_params_t {
		/// @brief Indices for the base mesh. Indexing is the same as for the triangle mesh primitive.
		tpbrt_uint_array_t indices;
		/// @brief Vertex positions for the base mesh. This is the same as for the triangle mesh primitive.
		tpbrt_point_array_t p;
		/// @brief The number of levels of refinement to compute in the subdivision algorithm.
		tpbrt_uint_t levels;
	} tpbrt_shape_loop_subdiv_params_t;

	/**
	 * @brief PBRT shape representation.
	 * @ingroup shape-public
	 */
	typedef struct tpbrt_shape_t {
		/// @brief Handle to texture.
		tpbrt_texture_handle_t alpha;
		/// @brief Animated transform.
		tpbrt_transform_animated_t transform;
		/// @brief Material handle.
		tpbrt_material_handle_t material;
		/// @brief Area light handle.
		tpbrt_area_light_handle_t area_light;
		/// @brief Interior media handle.
		tpbrt_media_handle_t interior_media_handle;
		/// @brief Exterior media handle.
		tpbrt_media_handle_t exterior_media_handle;
		/// @brief Shape value type.
		tpbrt_shape_type_t type			 : 5;
		/// @brief If true shape orientation is reversed (inside becomes outside)
		tpbrt_bool_t reverse_orientation : 1;

		/// @brief Value dependent on type value.
		union {
			/// @brief Bilinear mesh parameters.
			tpbrt_shape_bilinear_mesh_params_t bilinear_mesh;
			/// @brief Curve shape parameters.
			tpbrt_shape_curve_params_t curve;
			/// @brief Cylinder shape parameters.
			tpbrt_shape_cylinder_params_t cylinder;
			/// @brief Disk shape parameters.
			tpbrt_shape_disk_params_t disk;
			/// @brief Sphere shape parameters.
			tpbrt_shape_sphere_params_t sphere;
			/// @brief Triangle mesh parameters.
			tpbrt_shape_triangle_mesh_params_t triangle_mesh;
			/// @brief Ply mesh file parameters.
			tpbrt_shape_ply_mesh_params_t ply_mesh;
			/// @brief Loop subdiv parameters.
			tpbrt_shape_loop_subdiv_params_t loop_subdiv;
		} as;
	} tpbrt_shape_t;

	/**
	 * @brief List of shapes.
	 * @ingroup shape-public
	 */
	typedef struct tpbrt_shapes_list_t {
		/// @brief Shapes list data.
		tpbrt_shape_t* shapes;
		/// @brief Number of shapes in list.
		tpbrt_size_t count;
	} tpbrt_shapes_list_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SHAPE_H_
