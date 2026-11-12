#include <tinypbrt/pch.h>

#include <tinypbrt/detail/name_index_internal.h>
#include <tinypbrt/detail/shape_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/param_internal.h>
#include <tinypbrt/detail/texture_internal.h>

#pragma region SHAPE

tpbrt_error_t tpbrt_shape_type_from_string(const tpbrt_string_t type_str, tpbrt_shape_type_t* const type) {
	static const tpbrt_string_t TypesStrs[TPBRT_SHAPE_TYPE_MAX_NUM] = {
		TPBRT_STRING_INIT("bilinearmesh"),
		TPBRT_STRING_INIT("curve"),
		TPBRT_STRING_INIT("cylinder"),
		TPBRT_STRING_INIT("disk"),
		TPBRT_STRING_INIT("sphere"),
		TPBRT_STRING_INIT("trianglemesh"),
		TPBRT_STRING_INIT("plymesh"),
		TPBRT_STRING_INIT("loopsubdiv"),
	};

		if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (tpbrt_string_empty(type_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_SHAPE_TYPE; }

		for (tpbrt_shape_type_t t = 0; t < TPBRT_SHAPE_TYPE_MAX_NUM; ++t) {
				if (tpbrt_string_equals(type_str, TypesStrs[t]) == TPBRT_TRUE) {
					*type = t;
					return TPBRT_ERROR_NONE;
				}
		}

	return TPBRT_ERROR_INVALID_SHAPE_TYPE;
}

tpbrt_error_t tpbrt_shape_curve_basis_from_string(const tpbrt_string_t basis_str, tpbrt_shape_curve_basis_t* const basis) {
	static const tpbrt_string_t BasisStrs[TPBRT_SHAPE_CURVE_BASIS_MAX_NUM] = {
		TPBRT_STRING_INIT("bezier"),
		TPBRT_STRING_INIT("bspline"),
	};

		if (basis == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (tpbrt_string_empty(basis_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_SHAPE_CURVE_BASIS; }

		for (tpbrt_shape_curve_basis_t b = 0; b < TPBRT_SHAPE_CURVE_BASIS_MAX_NUM; ++b) {
				if (tpbrt_string_equals(basis_str, BasisStrs[b]) == TPBRT_TRUE) {
					*basis = b;
					return TPBRT_ERROR_NONE;
				}
		}

	return TPBRT_ERROR_INVALID_SHAPE_CURVE_BASIS;
}

tpbrt_error_t tpbrt_shape_curve_type_from_string(const tpbrt_string_t type_str, tpbrt_shape_curve_type_t* const type) {
	static const tpbrt_string_t TypesStrs[TPBRT_SHAPE_CURVE_TYPE_MAX_NUM] = {
		TPBRT_STRING_INIT("flat"),
		TPBRT_STRING_INIT("cylinder"),
		TPBRT_STRING_INIT("ribbon"),
	};

		if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (tpbrt_string_empty(type_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_SHAPE_CURVE_TYPE; }

		for (tpbrt_shape_curve_type_t t = 0; t < TPBRT_SHAPE_CURVE_TYPE_MAX_NUM; ++t) {
				if (tpbrt_string_equals(type_str, TypesStrs[t]) == TPBRT_TRUE) {
					*type = t;
					return TPBRT_ERROR_NONE;
				}
		}

	return TPBRT_ERROR_INVALID_SHAPE_CURVE_TYPE;
}

/**
 * @brief Reads bilinear mesh parameters.
 * @param bilinear_mesh Bilinear mesh data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup shape-private
 */
static tpbrt_error_t tpbrt_shape_read_bilinear_mesh(tpbrt_shape_bilinear_mesh_params_t* const bilinear_mesh,
  const tpbrt_params_list_t* const params) {
		if (bilinear_mesh == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t IndicesStr = TPBRT_STRING_INIT("indices");
	static const tpbrt_string_t PStr	   = TPBRT_STRING_INIT("P");
	static const tpbrt_string_t NStr	   = TPBRT_STRING_INIT("N");
	static const tpbrt_string_t UVStr	   = TPBRT_STRING_INIT("uv");

	tpbrt_error_t err					   = tpbrt_params_list_get_point3s(params, PStr, &bilinear_mesh->p);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_uints(params, IndicesStr, &bilinear_mesh->indices);
		if (err == TPBRT_ERROR_NOT_FOUND) {
				if (bilinear_mesh->p.count != 4u) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }

			bilinear_mesh->indices.data = malloc(4u * sizeof(tpbrt_uint_t));
				if (bilinear_mesh->indices.data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

			bilinear_mesh->indices.data[0] = 0u;
			bilinear_mesh->indices.data[1] = 1u;
			bilinear_mesh->indices.data[2] = 2u;
			bilinear_mesh->indices.data[3] = 3u;
			bilinear_mesh->indices.count   = 4u;
		}
		else if (err != TPBRT_ERROR_NONE) { return err; }

		if (bilinear_mesh->indices.count % 4u != 0u) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }
		for (tpbrt_size_t i = 0; i < bilinear_mesh->indices.count; ++i) {
				if (bilinear_mesh->indices.data[i] >= bilinear_mesh->p.count) { return TPBRT_ERROR_INDEX_OUT_OF_RANGE; }
		}

	err = tpbrt_params_list_get_normals(params, NStr, &bilinear_mesh->n);
		if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

	err = tpbrt_params_list_get_point2s(params, UVStr, &bilinear_mesh->uv);
		if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

	return TPBRT_ERROR_NONE;
}

/**
 * @brief Reads curve parameters.
 * @param curve Curve data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup shape-private
 */
static tpbrt_error_t tpbrt_shape_read_curve(tpbrt_shape_curve_params_t* const curve, const tpbrt_params_list_t* const params) {
		if (curve == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t PStr			= TPBRT_STRING_INIT("P");
	static const tpbrt_string_t BasisStr		= TPBRT_STRING_INIT("basis");
	static const tpbrt_string_t DegreeStr		= TPBRT_STRING_INIT("degree");
	static const tpbrt_string_t TypeStr			= TPBRT_STRING_INIT("type");
	static const tpbrt_string_t NStr			= TPBRT_STRING_INIT("N");
	static const tpbrt_string_t WidthStr		= TPBRT_STRING_INIT("width");
	static const tpbrt_string_t Width0Str		= TPBRT_STRING_INIT("width0");
	static const tpbrt_string_t Width1Str		= TPBRT_STRING_INIT("width1");
	static const tpbrt_string_t SplitDepthStr	= TPBRT_STRING_INIT("splitdepth");

	static const tpbrt_uint_t PArraySize		= 4u;
	static const tpbrt_string_t BasisDefault	= TPBRT_EMPTY_STRING_INIT;
	static const tpbrt_uint_t DegreeDefault		= 3u;
	static const tpbrt_string_t TypeDefault		= TPBRT_EMPTY_STRING_INIT;
	static const tpbrt_uint_t NArraySize		= 2u;
	static const tpbrt_float_t WidthDefault		= 1.0f;
	static const tpbrt_uint_t SplitDepthDefault = 3u;

	tpbrt_point3_array_t temp_p;
	tpbrt_error_t err = tpbrt_params_list_get_point3s_of_size(params, PStr, PArraySize, &temp_p);
		if (err != TPBRT_ERROR_NONE) { return err; }

	memcpy(curve->p, temp_p.data, sizeof(tpbrt_point3_t) * PArraySize);
	tpbrt_vec3_array_free(&temp_p);

	tpbrt_string_t temp_string;
	err = tpbrt_params_list_get_string(params, BasisStr, BasisDefault, &temp_string);

		if (tpbrt_string_empty(temp_string) == TPBRT_TRUE) { curve->basis = TPBRT_SHAPE_CURVE_BASIS_BEZIER; }
		else {
			tpbrt_shape_curve_basis_t basis = TPBRT_SHAPE_CURVE_BASIS_MAX_NUM;
			err								= tpbrt_shape_curve_basis_from_string(temp_string, &basis);
			tpbrt_string_free(&temp_string);
			curve->basis = basis;
		}

		if (err != TPBRT_ERROR_NONE) { return err; }

	tpbrt_uint_t temp_uint;
	err = tpbrt_params_list_get_uint(params, DegreeStr, DegreeDefault, &temp_uint);
		if (err != TPBRT_ERROR_NONE) { return err; }

		if (temp_uint > 3u || temp_uint < 2u) { return TPBRT_ERROR_INVALID_PARAM_VALUE; }

	curve->degree = (tpbrt_shape_curve_degree_t)temp_uint;

	err			  = tpbrt_params_list_get_string(params, TypeStr, TypeDefault, &temp_string);

		if (tpbrt_string_empty(temp_string) == TPBRT_TRUE) { curve->type = TPBRT_SHAPE_CURVE_TYPE_FLAT; }
		else {
			tpbrt_shape_curve_type_t type = TPBRT_SHAPE_CURVE_TYPE_MAX_NUM;
			err							  = tpbrt_shape_curve_type_from_string(temp_string, &type);
			tpbrt_string_free(&temp_string);
			curve->type = type;
		}

		if (err != TPBRT_ERROR_NONE) { return err; }

		if (curve->type == TPBRT_SHAPE_CURVE_TYPE_RIBBON) {
			tpbrt_normal_array_t temp_normal;
			err = tpbrt_params_list_get_normals_of_size(params, NStr, NArraySize, &temp_normal);
				if (err != TPBRT_ERROR_NONE) { return err; }

			memcpy(curve->n, temp_normal.data, sizeof(tpbrt_normal_t) * NArraySize);
			tpbrt_vec3_array_free(&temp_normal);
		}

	err = tpbrt_params_list_get_float(params, WidthStr, WidthDefault, &curve->width);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_opt_float(params, Width0Str, &curve->width0);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_opt_float(params, Width1Str, &curve->width1);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_uint(params, SplitDepthStr, SplitDepthDefault, &curve->split_depth);
}

/**
 * @brief Reads cylinder parameters.
 * @param cylinder Cylinder data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup shape-private
 */
static tpbrt_error_t tpbrt_shape_read_cylinder(tpbrt_shape_cylinder_params_t* const cylinder,
  const tpbrt_params_list_t* const params) {
		if (cylinder == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t RadiusStr	 = TPBRT_STRING_INIT("radius");
	static const tpbrt_string_t ZMinStr		 = TPBRT_STRING_INIT("zmin");
	static const tpbrt_string_t ZMaxStr		 = TPBRT_STRING_INIT("zmax");
	static const tpbrt_string_t PhiMaxStr	 = TPBRT_STRING_INIT("phimax");

	static const tpbrt_float_t RadiusDefault = 1.0f;
	static const tpbrt_float_t ZMinDefault	 = -1.0f;
	static const tpbrt_float_t ZMaxDefault	 = 1.0f;
	static const tpbrt_float_t PhiMaxDefault = 360.0f;

	tpbrt_error_t err						 = tpbrt_params_list_get_float(params, RadiusStr, RadiusDefault, &cylinder->radius);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, ZMinStr, ZMinDefault, &cylinder->z_min);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, ZMaxStr, ZMaxDefault, &cylinder->z_max);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_float(params, PhiMaxStr, PhiMaxDefault, &cylinder->phi_max);
}

/**
 * @brief Reads disk parameters.
 * @param disk Disk data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup shape-private
 */
static tpbrt_error_t tpbrt_shape_read_disk(tpbrt_shape_disk_params_t* const disk, const tpbrt_params_list_t* const params) {
		if (disk == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t HeightStr		  = TPBRT_STRING_INIT("height");
	static const tpbrt_string_t RadiusStr		  = TPBRT_STRING_INIT("radius");
	static const tpbrt_string_t InnerRadiusStr	  = TPBRT_STRING_INIT("innerradius");
	static const tpbrt_string_t PhiMaxStr		  = TPBRT_STRING_INIT("phimax");

	static const tpbrt_float_t HeightDefault	  = 0.0f;
	static const tpbrt_float_t RadiusDefault	  = 1.0f;
	static const tpbrt_float_t InnerRadiusDefault = 0.0f;
	static const tpbrt_float_t PhiMaxDefault	  = 360.0f;

	tpbrt_error_t err							  = tpbrt_params_list_get_float(params, HeightStr, HeightDefault, &disk->height);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, RadiusStr, RadiusDefault, &disk->radius);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, InnerRadiusStr, InnerRadiusDefault, &disk->inner_radius);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_float(params, PhiMaxStr, PhiMaxDefault, &disk->phi_max);
}

/**
 * @brief Reads sphere parameters.
 * @param sphere Sphere data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup shape-private
 */
static tpbrt_error_t tpbrt_shape_read_sphere(tpbrt_shape_sphere_params_t* const sphere, const tpbrt_params_list_t* const params) {
		if (sphere == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t RadiusStr	 = TPBRT_STRING_INIT("radius");
	static const tpbrt_string_t ZMinStr		 = TPBRT_STRING_INIT("zmin");
	static const tpbrt_string_t ZMaxStr		 = TPBRT_STRING_INIT("zmax");
	static const tpbrt_string_t PhiMaxStr	 = TPBRT_STRING_INIT("phimax");

	static const tpbrt_float_t RadiusDefault = 1.0f;
	static const tpbrt_float_t PhiMaxDefault = 360.0f;

	tpbrt_error_t err						 = tpbrt_params_list_get_float(params, RadiusStr, RadiusDefault, &sphere->radius);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, ZMinStr, -sphere->radius, &sphere->z_min);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, ZMaxStr, sphere->radius, &sphere->z_max);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_float(params, PhiMaxStr, PhiMaxDefault, &sphere->phi_max);
}

/**
 * @brief Reads triangle mesh parameters.
 * @param triangle_mesh Triangle mesh data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup shape-private
 */
static tpbrt_error_t tpbrt_shape_read_triangle_mesh(tpbrt_shape_triangle_mesh_params_t* const triangle_mesh,
  const tpbrt_params_list_t* const params) {
		if (triangle_mesh == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t IndicesStr = TPBRT_STRING_INIT("indices");
	static const tpbrt_string_t PStr	   = TPBRT_STRING_INIT("P");
	static const tpbrt_string_t NStr	   = TPBRT_STRING_INIT("N");
	static const tpbrt_string_t SStr	   = TPBRT_STRING_INIT("S");
	static const tpbrt_string_t UVStr	   = TPBRT_STRING_INIT("uv");

	tpbrt_error_t err					   = tpbrt_params_list_get_point3s(params, PStr, &triangle_mesh->p);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_uints(params, IndicesStr, &triangle_mesh->indices);
		if (err == TPBRT_ERROR_NOT_FOUND) {
				if (triangle_mesh->p.count != 3u) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }

			triangle_mesh->indices.data = malloc(3u * sizeof(tpbrt_uint_t));
				if (triangle_mesh->indices.data == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

			triangle_mesh->indices.data[0] = 0u;
			triangle_mesh->indices.data[1] = 1u;
			triangle_mesh->indices.data[2] = 2u;
			triangle_mesh->indices.count   = 3u;
		}
		else if (err != TPBRT_ERROR_NONE) { return err; }

		if (triangle_mesh->indices.count % 3u != 0u) { return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER; }
		for (tpbrt_size_t i = 0u; i < triangle_mesh->indices.count; ++i) {
				if (triangle_mesh->indices.data[i] >= triangle_mesh->p.count) { return TPBRT_ERROR_INDEX_OUT_OF_RANGE; }
		}

	err = tpbrt_params_list_get_normals(params, NStr, &triangle_mesh->n);
		if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

	err = tpbrt_params_list_get_vec3s(params, SStr, &triangle_mesh->s);
		if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

	err = tpbrt_params_list_get_point2s(params, UVStr, &triangle_mesh->uv);
		if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

	return TPBRT_ERROR_NONE;
}

/**
 * @brief Reads ply mesh parameters.
 * @param ply_mesh Ply mesh data.
 * @param params Parameters list.
 * @param textures Textures list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup shape-private
 */
static tpbrt_error_t tpbrt_shape_read_ply_mesh(tpbrt_shape_ply_mesh_params_t* const ply_mesh,
  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures) {
		if (ply_mesh == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t FileNameStr					= TPBRT_STRING_INIT("filename");
	static const tpbrt_string_t DisplacementStr				= TPBRT_STRING_INIT("displacement");
	static const tpbrt_string_t EdgeLengthStr				= TPBRT_STRING_INIT("edgelength");

	static const tpbrt_texture_handle_t DisplacementDefault = TPBRT_TEXTURE_HANDLE_INVALID_INIT;
	static const tpbrt_float_t EdgeLengthDefault			= 1.0f;

	tpbrt_error_t err = tpbrt_params_list_get_string_required(params, FileNameStr, &ply_mesh->file_name);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, DisplacementStr, textures, TPBRT_TEXTURE_TYPE_FLOAT, &DisplacementDefault,
	  &ply_mesh->displacement);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_float(params, EdgeLengthStr, EdgeLengthDefault, &ply_mesh->edge_length);
}

/**
 * @brief Reads loop subdiv parameters.
 * @param loop_subdiv Loop subdiv data.
 * @param params Parameters list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup shape-private
 */
static tpbrt_error_t tpbrt_shape_read_loop_subdiv(tpbrt_shape_loop_subdiv_params_t* const loop_subdiv,
  const tpbrt_params_list_t* const params) {
		if (loop_subdiv == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t LevelsStr	= TPBRT_STRING_INIT("levels");
	static const tpbrt_string_t IndicesStr	= TPBRT_STRING_INIT("indices");
	static const tpbrt_string_t PStr		= TPBRT_STRING_INIT("P");

	static const tpbrt_uint_t LevelsDefault = 3u;

	tpbrt_error_t err						= tpbrt_params_list_get_uint(params, LevelsStr, LevelsDefault, &loop_subdiv->levels);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_uints(params, IndicesStr, &loop_subdiv->indices);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_points(params, PStr, &loop_subdiv->p);
}

tpbrt_error_t tpbrt_shape_init(const tpbrt_string_t type_str, const tpbrt_params_list_t* const params,
  const tpbrt_textures_list_t* const textures, const tpbrt_transform_animated_t transform,
  const tpbrt_material_handle_t material_handle, const tpbrt_area_light_handle_t area_light_handle,
  const tpbrt_media_handle_t interior_media_handle, const tpbrt_media_handle_t exterior_media_handle,
  const tpbrt_bool_t reverse_orientation, tpbrt_shape_t* const shape) {
		if (shape == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (memset(shape, 0, sizeof(tpbrt_shape_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	shape->transform								 = transform;
	shape->material									 = material_handle;
	shape->area_light								 = area_light_handle;
	shape->interior_media_handle					 = interior_media_handle;
	shape->exterior_media_handle					 = exterior_media_handle;
	shape->reverse_orientation						 = reverse_orientation;

	static const tpbrt_string_t AlphaStr			 = TPBRT_STRING_INIT("alpha");

	static const tpbrt_texture_handle_t AlphaDefault = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.0f);

	tpbrt_error_t err =
	  tpbrt_params_list_get_texture_of_type(params, AlphaStr, textures, TPBRT_TEXTURE_TYPE_FLOAT, &AlphaDefault, &shape->alpha);
		if (err != TPBRT_ERROR_NONE) {
			tpbrt_shape_free(shape);
			return err;
		}

	tpbrt_shape_type_t type;
	err = tpbrt_shape_type_from_string(type_str, &type);
		if (err != TPBRT_ERROR_NONE) {
			tpbrt_shape_free(shape);
			return err;
		}
	shape->type = type;

		switch (shape->type) {
		default:
			case TPBRT_SHAPE_TYPE_BILINEAR_MESH: {
				err = tpbrt_shape_read_bilinear_mesh(&shape->as.bilinear_mesh, params);
				break;
			}
			case TPBRT_SHAPE_TYPE_CURVE: {
				err = tpbrt_shape_read_curve(&shape->as.curve, params);
				break;
			}
			case TPBRT_SHAPE_TYPE_CYLINDER: {
				err = tpbrt_shape_read_cylinder(&shape->as.cylinder, params);
				break;
			}
			case TPBRT_SHAPE_TYPE_DISK: {
				err = tpbrt_shape_read_disk(&shape->as.disk, params);
				break;
			}
			case TPBRT_SHAPE_TYPE_SPHERE: {
				err = tpbrt_shape_read_sphere(&shape->as.sphere, params);
				break;
			}
			case TPBRT_SHAPE_TYPE_TRIANGLE_MESH: {
				err = tpbrt_shape_read_triangle_mesh(&shape->as.triangle_mesh, params);
				break;
			}
			case TPBRT_SHAPE_TYPE_PLY_MESH: {
				err = tpbrt_shape_read_ply_mesh(&shape->as.ply_mesh, params, textures);
				break;
			}
			case TPBRT_SHAPE_TYPE_LOOP_SUBDIV: {
				err = tpbrt_shape_read_loop_subdiv(&shape->as.loop_subdiv, params);
				break;
			}
		}

		if (err != TPBRT_ERROR_NONE) {
			tpbrt_shape_free(shape);
			return err;
		}

	return TPBRT_ERROR_NONE;
}

void tpbrt_shape_free(tpbrt_shape_t* const shape) {
		if (shape == TPBRT_NULL) { return; }

	tpbrt_texture_handle_free(&shape->alpha);

		switch (shape->type) {
			case TPBRT_SHAPE_TYPE_BILINEAR_MESH: {
				tpbrt_shape_bilinear_mesh_params_t* const params = &shape->as.bilinear_mesh;

				tpbrt_uint_array_free(&params->indices);
				tpbrt_vec3_array_free(&params->p);
				tpbrt_vec3_array_free(&params->n);
				tpbrt_vec2_array_free(&params->uv);
				break;
			}
			case TPBRT_SHAPE_TYPE_TRIANGLE_MESH: {
				tpbrt_shape_triangle_mesh_params_t* const params = &shape->as.triangle_mesh;

				tpbrt_uint_array_free(&params->indices);
				tpbrt_vec3_array_free(&params->p);
				tpbrt_vec3_array_free(&params->n);
				tpbrt_vec3_array_free(&params->s);
				tpbrt_vec2_array_free(&params->uv);
				break;
			}
			case TPBRT_SHAPE_TYPE_PLY_MESH: {
				tpbrt_shape_ply_mesh_params_t* const params = &shape->as.ply_mesh;

				tpbrt_string_free(&params->file_name);
				tpbrt_texture_handle_free(&params->displacement);
				break;
			}
			case TPBRT_SHAPE_TYPE_LOOP_SUBDIV: {
				tpbrt_shape_loop_subdiv_params_t* const params = &shape->as.loop_subdiv;

				tpbrt_uint_array_free(&params->indices);
				tpbrt_vec3_array_free(&params->p);
				break;
			}
		default: break;
		}
}

#pragma endregion

#pragma region SHAPES_LIST
/**
 * @brief Initializes empty shapes list with type conversion.
 * @ingroup shape-private
 */
#define TPBRT_EMPTY_SHAPES_LIST (tpbrt_shapes_list_t) { .shapes = TPBRT_NULL, .count = 0 }

tpbrt_error_t tpbrt_shapes_list_init(tpbrt_shapes_list_t* const shapes_list) {
		if (shapes_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	*shapes_list = TPBRT_EMPTY_SHAPES_LIST;
	return TPBRT_ERROR_NONE;
}

tpbrt_error_t tpbrt_shapes_list_append(tpbrt_shapes_list_t* const shapes_list, const tpbrt_shape_t* const shape) {
		if (shapes_list == TPBRT_NULL || shape == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	tpbrt_shape_t* const new_list = tpbrt_array_reserve_append(shapes_list->shapes, shapes_list->count, sizeof(tpbrt_shape_t));
		if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

	new_list[shapes_list->count] = *shape;
	shapes_list->shapes			 = new_list;
	++shapes_list->count;
	return TPBRT_ERROR_NONE;
}

void tpbrt_shapes_list_shrink_to_fit(tpbrt_shapes_list_t* const shapes_list) {
		if (shapes_list == TPBRT_NULL) { return; }
	shapes_list->shapes = tpbrt_array_shrink_to_fit(shapes_list->shapes, shapes_list->count, sizeof(tpbrt_shape_t));
}

void tpbrt_shapes_list_free(tpbrt_shapes_list_t* const shapes_list) {
		if (shapes_list == TPBRT_NULL || shapes_list->shapes == TPBRT_NULL) { return; }

		for (tpbrt_size_t i = 0; i < shapes_list->count; ++i) { tpbrt_shape_free(shapes_list->shapes + i); }

	free(shapes_list->shapes);
	*shapes_list = TPBRT_EMPTY_SHAPES_LIST;
}

#pragma endregion
