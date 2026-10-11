#include <tinypbrt/pch.h>

#include <tinypbrt/detail/shape_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/param_internal.h>
#include <tinypbrt/detail/texture_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region SHAPE

	tpbrt_error_t tpbrt_shape_type_from_string(const tpbrt_string_t type_str, tpbrt_shape_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_SHAPE_TYPE_MAX_NUM] = {
			TPBRT_STRING("bilinearmesh"),
			TPBRT_STRING("curve"),
			TPBRT_STRING("cylinder"),
			TPBRT_STRING("disk"),
			TPBRT_STRING("sphere"),
			TPBRT_STRING("trianglemesh"),
			TPBRT_STRING("plymesh"),
			TPBRT_STRING("loopsubdiv"),
		};

			if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(type_str)) { return TPBRT_ERROR_INVALID_SHAPE_TYPE; }

			for (tpbrt_shape_type_t t = 0; t < TPBRT_SHAPE_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS[t])) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_SHAPE_TYPE;
	}

	tpbrt_error_t tpbrt_shape_curve_basis_from_string(const tpbrt_string_t basis_str, tpbrt_shape_curve_basis_t* const basis) {
		static const tpbrt_string_t BASIS_STRS[TPBRT_SHAPE_CURVE_BASIS_MAX_NUM] = {
			TPBRT_STRING("bezier"),
			TPBRT_STRING("bspline"),
		};

			if (basis == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(basis_str)) { return TPBRT_ERROR_INVALID_SHAPE_CURVE_BASIS; }

			for (tpbrt_shape_curve_basis_t b = 0; b < TPBRT_SHAPE_CURVE_BASIS_MAX_NUM; ++b) {
					if (tpbrt_string_equals(basis_str, BASIS_STRS[b])) {
						*basis = b;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_SHAPE_CURVE_BASIS;
	}

	tpbrt_error_t tpbrt_shape_curve_type_from_string(const tpbrt_string_t type_str, tpbrt_shape_curve_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_SHAPE_CURVE_TYPE_MAX_NUM] = {
			TPBRT_STRING("flat"),
			TPBRT_STRING("cylinder"),
			TPBRT_STRING("ribbon"),
		};

			if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(type_str)) { return TPBRT_ERROR_INVALID_SHAPE_CURVE_TYPE; }

			for (tpbrt_shape_curve_type_t t = 0; t < TPBRT_SHAPE_CURVE_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS[t])) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_SHAPE_CURVE_TYPE;
	}

	static tpbrt_error_t tpbrt_shape_read_curve(tpbrt_shape_curve_params_t* const curve, const tpbrt_params_list_t params) {
			if (curve == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t P_STR			= TPBRT_STRING("P");
		static const tpbrt_string_t BASIS_STR		= TPBRT_STRING("basis");
		static const tpbrt_string_t DEGREE_STR		= TPBRT_STRING("degree");
		static const tpbrt_string_t TYPE_STR		= TPBRT_STRING("type");
		static const tpbrt_string_t N_STR			= TPBRT_STRING("N");
		static const tpbrt_string_t WIDTH_STR		= TPBRT_STRING("width");
		static const tpbrt_string_t WIDTH_0_STR		= TPBRT_STRING("width0");
		static const tpbrt_string_t WIDTH_1_STR		= TPBRT_STRING("width1");
		static const tpbrt_string_t SPLIT_DEPTH_STR = TPBRT_STRING("splitdepth");

		tpbrt_point3_array_t temp_P;
		tpbrt_error_t err = tpbrt_params_list_get_point3s_of_size(params, P_STR, 4u, &temp_P);
			if (err != TPBRT_ERROR_NONE) { return err; }

		memcpy(curve->P, temp_P.data, sizeof(tpbrt_point3_t) * 4u);
		tpbrt_vec3_array_free(&temp_P);

		tpbrt_string_t temp_string;
		err = tpbrt_params_list_get_string(params, BASIS_STR, &temp_string);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_shape_curve_basis_t basis;
		err = tpbrt_shape_curve_basis_from_string(temp_string, &basis);
		tpbrt_string_free(&temp_string);
			if (err != TPBRT_ERROR_NONE) { return err; }
		curve->basis = basis;

		tpbrt_uint_t temp_uint;
		err = tpbrt_params_list_get_uint(params, DEGREE_STR, 3u, &temp_uint);
			if (err != TPBRT_ERROR_NONE) { return err; }

			if (temp_uint > 3u || temp_uint < 2u) { return TPBRT_ERROR_INVALID_PARAM_VALUE; }

		curve->degree = (tpbrt_shape_curve_degree_t)temp_uint;

		err			  = tpbrt_params_list_get_string(params, TYPE_STR, &temp_string);
			if (err != TPBRT_ERROR_NONE) { return err; }

		tpbrt_shape_curve_type_t type;
		err = tpbrt_shape_curve_type_from_string(temp_string, &type);
		tpbrt_string_free(&temp_string);
			if (err != TPBRT_ERROR_NONE) { return err; }
		curve->type = type;

			if (curve->type == TPBRT_SHAPE_CURVE_TYPE_RIBBON) {
				tpbrt_normal_array_t temp_normal;
				err = tpbrt_params_list_get_normals_of_size(params, N_STR, 2u, &temp_normal);
					if (err != TPBRT_ERROR_NONE) { return err; }

				memcpy(curve->N, temp_normal.data, sizeof(tpbrt_normal_t) * 2u);
				tpbrt_vec3_array_free(&temp_normal);
			}

		err = tpbrt_params_list_get_float(params, WIDTH_STR, 1.0f, &curve->width);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_opt_float(params, WIDTH_0_STR, &curve->width0);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_opt_float(params, WIDTH_1_STR, &curve->width1);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_uint(params, SPLIT_DEPTH_STR, 3u, &curve->split_depth);
	}

	static tpbrt_error_t tpbrt_shape_read_cylinder(tpbrt_shape_cylinder_params_t* const cylinder,
	  const tpbrt_params_list_t params) {
			if (cylinder == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t RADIUS_STR	= TPBRT_STRING("radius");
		static const tpbrt_string_t Z_MIN_STR	= TPBRT_STRING("zmin");
		static const tpbrt_string_t Z_MAX_STR	= TPBRT_STRING("zmax");
		static const tpbrt_string_t PHI_MAX_STR = TPBRT_STRING("phimax");

		tpbrt_error_t err						= tpbrt_params_list_get_float(params, RADIUS_STR, 1.0f, &cylinder->radius);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, Z_MIN_STR, -1.0f, &cylinder->z_min);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, Z_MAX_STR, 1.0f, &cylinder->z_max);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, PHI_MAX_STR, 360.0f, &cylinder->phi_max);
	}

	static tpbrt_error_t tpbrt_shape_read_disk(tpbrt_shape_disk_params_t* const disk, const tpbrt_params_list_t params) {
			if (disk == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t HEIGHT_STR		 = TPBRT_STRING("height");
		static const tpbrt_string_t RADIUS_STR		 = TPBRT_STRING("radius");
		static const tpbrt_string_t INNER_RADIUS_STR = TPBRT_STRING("innerradius");
		static const tpbrt_string_t PHI_MAX_STR		 = TPBRT_STRING("phimax");

		tpbrt_error_t err							 = tpbrt_params_list_get_float(params, HEIGHT_STR, 0.0f, &disk->height);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, RADIUS_STR, 1.0f, &disk->radius);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, INNER_RADIUS_STR, 0.0f, &disk->inner_radius);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, PHI_MAX_STR, 360.0f, &disk->phi_max);
	}

	static tpbrt_error_t tpbrt_shape_read_sphere(tpbrt_shape_sphere_params_t* const sphere, const tpbrt_params_list_t params) {
			if (sphere == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t RADIUS_STR	= TPBRT_STRING("radius");
		static const tpbrt_string_t Z_MIN_STR	= TPBRT_STRING("zmin");
		static const tpbrt_string_t Z_MAX_STR	= TPBRT_STRING("zmax");
		static const tpbrt_string_t PHI_MAX_STR = TPBRT_STRING("phimax");

		tpbrt_error_t err						= tpbrt_params_list_get_float(params, RADIUS_STR, 1.0f, &sphere->radius);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, Z_MIN_STR, -sphere->radius, &sphere->z_min);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, Z_MAX_STR, sphere->radius, &sphere->z_max);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_float(params, PHI_MAX_STR, 360.0f, &sphere->phi_max);
	}

	static tpbrt_error_t tpbrt_shape_read_triangle_mesh(tpbrt_shape_triangle_mesh_params_t* const triangle_mesh,
	  const tpbrt_params_list_t params) {
			if (triangle_mesh == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t INDICES_STR = TPBRT_STRING("indices");
		static const tpbrt_string_t P_STR		= TPBRT_STRING("P");
		static const tpbrt_string_t N_STR		= TPBRT_STRING("N");
		static const tpbrt_string_t S_STR		= TPBRT_STRING("S");
		static const tpbrt_string_t UV_STR		= TPBRT_STRING("uv");

		tpbrt_error_t err						= tpbrt_params_list_get_point3s(params, P_STR, &triangle_mesh->P);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_uints(params, INDICES_STR, &triangle_mesh->indices);
			if (err != TPBRT_ERROR_NONE && !(triangle_mesh->P.count == 3 && err == TPBRT_ERROR_NOT_FOUND)) { return err; }

		err = tpbrt_params_list_get_normals(params, N_STR, &triangle_mesh->N);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_vec3s(params, S_STR, &triangle_mesh->S);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_point2s(params, UV_STR, &triangle_mesh->uv);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_shape_read_ply_mesh(tpbrt_shape_ply_mesh_params_t* const ply_mesh,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures) {
			if (ply_mesh == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t FILE_NAME_STR	 = TPBRT_STRING("filename");
		static const tpbrt_string_t DISPLACEMENT_STR = TPBRT_STRING("displacement");
		static const tpbrt_string_t EDGE_LENGTH_STR	 = TPBRT_STRING("edgelength");

		tpbrt_error_t err							 = tpbrt_params_list_get_string(params, FILE_NAME_STR, &ply_mesh->file_name);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture(params, DISPLACEMENT_STR, textures, &ply_mesh->displacement);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		return tpbrt_params_list_get_float(params, EDGE_LENGTH_STR, 1.0f, &ply_mesh->edge_length);
	}

	static tpbrt_error_t tpbrt_shape_read_loop_subdiv(tpbrt_shape_loop_subdiv_params_t* const loop_subdiv,
	  const tpbrt_params_list_t params) {
			if (loop_subdiv == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t LEVELS_STR	= TPBRT_STRING("levels");
		static const tpbrt_string_t INDICES_STR = TPBRT_STRING("indices");
		static const tpbrt_string_t P_STR		= TPBRT_STRING("P");

		tpbrt_error_t err						= tpbrt_params_list_get_uint(params, LEVELS_STR, 3u, &loop_subdiv->levels);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_uints(params, INDICES_STR, &loop_subdiv->indices);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_points(params, P_STR, &loop_subdiv->P);
	}

	tpbrt_error_t tpbrt_shape_init(const tpbrt_string_t type_str, const tpbrt_params_list_t params,
	  const tpbrt_textures_list_t textures, const tpbrt_mat4_animated_t* const ctm, const tpbrt_material_handle_t material_handle,
	  const tpbrt_area_light_handle_t area_light_handle, const tpbrt_media_handle_t interior_media_handle,
	  const tpbrt_media_handle_t exterior_media_handle, const tpbrt_bool_t reverse_orientation, tpbrt_shape_t* const shape) {
			if (shape == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(shape, 0, sizeof(tpbrt_shape_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		shape->transform					  = *ctm;
		shape->material						  = material_handle;
		shape->area_light					  = area_light_handle;
		shape->interior_media_handle		  = interior_media_handle;
		shape->exterior_media_handle		  = exterior_media_handle;
		shape->reverse_orientation			  = reverse_orientation;

		static const tpbrt_string_t ALPHA_STR = TPBRT_STRING("alpha");

		tpbrt_error_t err					  = tpbrt_params_list_get_texture(params, ALPHA_STR, textures, &shape->alpha);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
				tpbrt_shape_free(shape);
				return err;
			}

			if (err == TPBRT_ERROR_NOT_FOUND) { shape->alpha = TPBRT_TEXTURE_HANDLE_FLOAT(1.0f); }

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
					err = TPBRT_ERROR_NONE;
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

			switch (shape->type) {
				case TPBRT_SHAPE_TYPE_TRIANGLE_MESH: {
					tpbrt_shape_triangle_mesh_params_t* const params = &shape->as.triangle_mesh;

					tpbrt_uint_array_free(&params->indices);
					tpbrt_vec3_array_free(&params->P);
					tpbrt_vec3_array_free(&params->N);
					tpbrt_vec3_array_free(&params->S);
					tpbrt_vec2_array_free(&params->uv);
					break;
				}
				case TPBRT_SHAPE_TYPE_PLY_MESH: {
					tpbrt_shape_ply_mesh_params_t* const params = &shape->as.ply_mesh;

					tpbrt_string_free(&params->file_name);
					break;
				}
				case TPBRT_SHAPE_TYPE_LOOP_SUBDIV: {
					tpbrt_shape_loop_subdiv_params_t* const params = &shape->as.loop_subdiv;

					tpbrt_uint_array_free(&params->indices);
					tpbrt_vec3_array_free(&params->P);
					break;
				}
			default: break;
			}
	}

#pragma endregion

#pragma region SHAPES_LIST
#define TPBRT_EMPTY_SHAPES_LIST (tpbrt_shapes_list_t) { .shapes = TPBRT_NULL, .count = 0 }

	tpbrt_error_t tpbrt_shapes_list_init(tpbrt_shapes_list_t* const shapes_list) {
			if (shapes_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*shapes_list = TPBRT_EMPTY_SHAPES_LIST;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_shapes_list_append(tpbrt_shapes_list_t* const shapes_list, const tpbrt_shape_t* const shape) {
			if (shapes_list == TPBRT_NULL || shape == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_size_t new_count = shapes_list->count + 1;

		tpbrt_shape_t* new_list		 = realloc(shapes_list->shapes, sizeof(tpbrt_shape_t) * new_count);
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		new_list[shapes_list->count] = *shape;
		*shapes_list				 = (tpbrt_shapes_list_t) { .shapes = new_list, .count = new_count };
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_shapes_list_free(tpbrt_shapes_list_t* const shapes_list) {
			if (shapes_list == TPBRT_NULL || shapes_list->shapes == TPBRT_NULL) { return; }

			for (tpbrt_size_t i = 0; i < shapes_list->count; ++i) { tpbrt_shape_free(shapes_list->shapes + i); }

		free(shapes_list->shapes);
		*shapes_list = TPBRT_EMPTY_SHAPES_LIST;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif