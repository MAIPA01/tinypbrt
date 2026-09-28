#include <tinypbrt/pch.h>

#include <tinypbrt/detail/shape_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>
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

			if (err == TPBRT_ERROR_NOT_FOUND) {
				shape->alpha.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
				shape->alpha.as.float_value = 1.0f;
			}

		err = tpbrt_shape_type_from_string(type_str, &shape->type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_shape_free(shape);
				return err;
			}

		tpbrt_string_t temp_string;
			switch (shape->type) {
			default:
				case TPBRT_SHAPE_TYPE_BILINEAR_MESH: {
					break;
				}
				case TPBRT_SHAPE_TYPE_CURVE: {
					static const tpbrt_string_t P_STR			= TPBRT_STRING("P");
					static const tpbrt_string_t BASIS_STR		= TPBRT_STRING("basis");
					static const tpbrt_string_t DEGREE_STR		= TPBRT_STRING("degree");
					static const tpbrt_string_t TYPE_STR		= TPBRT_STRING("type");
					static const tpbrt_string_t N_STR			= TPBRT_STRING("N");
					static const tpbrt_string_t WIDTH_STR		= TPBRT_STRING("width");
					static const tpbrt_string_t WIDTH_0_STR		= TPBRT_STRING("width0");
					static const tpbrt_string_t WIDTH_1_STR		= TPBRT_STRING("width1");
					static const tpbrt_string_t SPLIT_DEPTH_STR = TPBRT_STRING("splitdepth");

					tpbrt_shape_curve_params_t* shape_params	= &shape->as.curve;

					tpbrt_point3_array_t temp_P;
					err = tpbrt_params_list_get_point3s(params, P_STR, &temp_P);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

						if (temp_P.count < 4) {
							tpbrt_vec3_array_free(&temp_P);
							tpbrt_shape_free(shape);
							return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
						}

						if (temp_P.count > 4) {
							tpbrt_vec3_array_free(&temp_P);
							tpbrt_shape_free(shape);
							return TPBRT_ERROR_TOO_MANY_VALUES;
						}

						for (tpbrt_uint_t i = 0; i < 4; ++i) { shape_params->P[i] = temp_P.data[i]; }
					tpbrt_vec3_array_free(&temp_P);

					err = tpbrt_params_list_get_string(params, BASIS_STR, &temp_string);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_shape_curve_basis_from_string(temp_string, &shape_params->basis);
					tpbrt_string_free(&temp_string);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					tpbrt_uint_t temp_uint;
					err = tpbrt_params_list_get_uint(params, DEGREE_STR, 3u, &temp_uint);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

						if (temp_uint > 3 || temp_uint < 2) {
							tpbrt_shape_free(shape);
							return TPBRT_ERROR_INVALID_PARAM_VALUE;
						}

					shape_params->degree = (tpbrt_shape_curve_degree_t)temp_uint;

					err					 = tpbrt_params_list_get_string(params, TYPE_STR, &temp_string);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_shape_curve_type_from_string(temp_string, &shape_params->type);
					tpbrt_string_free(&temp_string);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

						if (shape_params->type == TPBRT_SHAPE_CURVE_TYPE_RIBBON) {
							tpbrt_normal_array_t temp_normal;
							err = tpbrt_params_list_get_normals(params, N_STR, &temp_normal);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_shape_free(shape);
									return err;
								}

								if (temp_normal.count < 2) {
									tpbrt_vec3_array_free(&temp_normal);
									tpbrt_shape_free(shape);
									return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
								}

								if (temp_normal.count > 2) {
									tpbrt_vec3_array_free(&temp_normal);
									tpbrt_shape_free(shape);
									return TPBRT_ERROR_TOO_MANY_VALUES;
								}

								for (tpbrt_uint_t i = 0; i < 2; ++i) { shape_params->N[i] = temp_normal.data[i]; }
							tpbrt_vec3_array_free(&temp_normal);
						}

					err = tpbrt_params_list_get_float(params, WIDTH_STR, 1.0f, &shape_params->width);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_opt_float(params, WIDTH_0_STR, &shape_params->width0);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_opt_float(params, WIDTH_1_STR, &shape_params->width1);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_uint(params, SPLIT_DEPTH_STR, 3u, &shape_params->split_depth);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}
					break;
				}
				case TPBRT_SHAPE_TYPE_CYLINDER: {
					static const tpbrt_string_t RADIUS_STR		= TPBRT_STRING("radius");
					static const tpbrt_string_t Z_MIN_STR		= TPBRT_STRING("zmin");
					static const tpbrt_string_t Z_MAX_STR		= TPBRT_STRING("zmax");
					static const tpbrt_string_t PHI_MAX_STR		= TPBRT_STRING("phimax");

					tpbrt_shape_cylinder_params_t* shape_params = &shape->as.cylinder;

					err = tpbrt_params_list_get_float(params, RADIUS_STR, 1.0f, &shape_params->radius);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, Z_MIN_STR, -1.0f, &shape_params->z_min);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, Z_MAX_STR, 1.0f, &shape_params->z_max);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, PHI_MAX_STR, 360.0f, &shape_params->phi_max);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}
					break;
				}
				case TPBRT_SHAPE_TYPE_DISK: {
					static const tpbrt_string_t HEIGHT_STR		 = TPBRT_STRING("height");
					static const tpbrt_string_t RADIUS_STR		 = TPBRT_STRING("radius");
					static const tpbrt_string_t INNER_RADIUS_STR = TPBRT_STRING("innerradius");
					static const tpbrt_string_t PHI_MAX_STR		 = TPBRT_STRING("phimax");

					tpbrt_shape_disk_params_t* shape_params		 = &shape->as.disk;

					err = tpbrt_params_list_get_float(params, HEIGHT_STR, 0.0f, &shape_params->height);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, RADIUS_STR, 1.0f, &shape_params->radius);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, INNER_RADIUS_STR, 0.0f, &shape_params->inner_radius);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, PHI_MAX_STR, 360.0f, &shape_params->phi_max);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}
					break;
				}
				case TPBRT_SHAPE_TYPE_SPHERE: {
					static const tpbrt_string_t RADIUS_STR	  = TPBRT_STRING("radius");
					static const tpbrt_string_t Z_MIN_STR	  = TPBRT_STRING("zmin");
					static const tpbrt_string_t Z_MAX_STR	  = TPBRT_STRING("zmax");
					static const tpbrt_string_t PHI_MAX_STR	  = TPBRT_STRING("phimax");

					tpbrt_shape_sphere_params_t* shape_params = &shape->as.sphere;

					err = tpbrt_params_list_get_float(params, RADIUS_STR, 1.0f, &shape_params->radius);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, Z_MIN_STR, -shape_params->radius, &shape_params->z_min);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, Z_MAX_STR, shape_params->radius, &shape_params->z_max);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, PHI_MAX_STR, 360.0f, &shape_params->phi_max);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}
					break;
				}
				case TPBRT_SHAPE_TYPE_TRIANGLE_MESH: {
					static const tpbrt_string_t INDICES_STR			 = TPBRT_STRING("indices");
					static const tpbrt_string_t P_STR				 = TPBRT_STRING("P");
					static const tpbrt_string_t N_STR				 = TPBRT_STRING("N");
					static const tpbrt_string_t S_STR				 = TPBRT_STRING("S");
					static const tpbrt_string_t UV_STR				 = TPBRT_STRING("uv");

					tpbrt_shape_triangle_mesh_params_t* shape_params = &shape->as.triangle_mesh;

					err = tpbrt_params_list_get_point3s(params, P_STR, &shape_params->P);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_uints(params, INDICES_STR, &shape_params->indices);
						if (err != TPBRT_ERROR_NONE && !(shape_params->indices.count == 3 && err == TPBRT_ERROR_NOT_FOUND)) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_normals(params, N_STR, &shape_params->N);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_vec3s(params, S_STR, &shape_params->S);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_point2s(params, UV_STR, &shape_params->uv);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_shape_free(shape);
							return err;
						}
					break;
				}
				case TPBRT_SHAPE_TYPE_PLY_MESH: {
					static const tpbrt_string_t FILE_NAME_STR	 = TPBRT_STRING("filename");
					static const tpbrt_string_t DISPLACEMENT_STR = TPBRT_STRING("displacement");
					static const tpbrt_string_t EDGE_LENGTH_STR	 = TPBRT_STRING("edgelength");

					tpbrt_shape_ply_mesh_params_t* shape_params	 = &shape->as.ply_mesh;

					err = tpbrt_params_list_get_string(params, FILE_NAME_STR, &shape_params->file_name);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_texture(params, DISPLACEMENT_STR, textures, &shape_params->displacement);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, EDGE_LENGTH_STR, 1.0f, &shape_params->edge_length);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}
					break;
				}
				case TPBRT_SHAPE_TYPE_LOOP_SUBDIV: {
					static const tpbrt_string_t LEVELS_STR		   = TPBRT_STRING("levels");
					static const tpbrt_string_t INDICES_STR		   = TPBRT_STRING("indices");
					static const tpbrt_string_t P_STR			   = TPBRT_STRING("P");

					tpbrt_shape_loop_subdiv_params_t* shape_params = &shape->as.loop_subdiv;

					err = tpbrt_params_list_get_uint(params, LEVELS_STR, 3u, &shape_params->levels);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_uints(params, INDICES_STR, &shape_params->indices);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}

					err = tpbrt_params_list_get_points(params, P_STR, &shape_params->P);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_shape_free(shape);
							return err;
						}
					break;
				}
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