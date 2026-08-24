#include <tinypbrt/pch.h>

#include <tinypbrt/detail/shape_internal.h>

#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/math_internal.h>
#include <tinypbrt/detail/texture_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region SHAPE

	static tpbrt_error_t tpbrt_shape_type_from_string(const tpbrt_string_t* const type_str, tpbrt_shape_type_t* const type) {
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

			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_shape_type_t t = 0; t < TPBRT_SHAPE_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS + t)) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_SHAPE_TYPE;
	}

	static tpbrt_error_t tpbrt_shape_curve_basis_from_string(const tpbrt_string_t* const basis_str,
	  tpbrt_shape_curve_basis_t* const basis) {
		static const tpbrt_string_t BASIS_STRS[TPBRT_SHAPE_CURVE_BASIS_MAX_NUM] = {
			TPBRT_STRING("bezier"),
			TPBRT_STRING("bspline"),
		};

			if (basis_str == TPBRT_NULL || basis_str->data == TPBRT_NULL || basis == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_shape_curve_basis_t b = 0; b < TPBRT_SHAPE_CURVE_BASIS_MAX_NUM; ++b) {
					if (tpbrt_string_equals(basis_str, BASIS_STRS + b)) {
						*basis = b;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_SHAPE_CURVE_BASIS;
	}

	static tpbrt_error_t tpbrt_shape_curve_type_from_string(const tpbrt_string_t* const type_str,
	  tpbrt_shape_curve_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_SHAPE_CURVE_TYPE_MAX_NUM] = {
			TPBRT_STRING("flat"),
			TPBRT_STRING("cylinder"),
			TPBRT_STRING("ribbon"),
		};

			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_shape_curve_type_t t = 0; t < TPBRT_SHAPE_CURVE_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS + t)) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_SHAPE_CURVE_TYPE;
	}

	tpbrt_error_t tpbrt_create_shape(const tpbrt_string_t* const type_str, const tpbrt_params_list_t* const params,
	  const tpbrt_textures_list_t* const textures, const tpbrt_mat4_animated_t* const ctm,
	  const tpbrt_material_handle_t material_handle, const tpbrt_area_light_handle_t area_light_handle,
	  const tpbrt_media_handle_t interior_media_handle, const tpbrt_media_handle_t exterior_media_handle,
	  const tpbrt_bool_t reverse_orientation, tpbrt_shape_t* const shape) {
			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || params == TPBRT_NULL || textures == TPBRT_NULL ||
				shape == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (memset(shape, 0, sizeof(tpbrt_shape_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		shape->transform					  = *ctm;
		shape->material						  = material_handle;
		shape->area_light					  = area_light_handle;
		shape->interior_media_handle		  = interior_media_handle;
		shape->exterior_media_handle		  = exterior_media_handle;
		shape->reverse_orientation			  = reverse_orientation;

		static const tpbrt_string_t ALPHA_STR = TPBRT_STRING("alpha");

		tpbrt_error_t err					  = tpbrt_params_list_get_float(params, &ALPHA_STR, 1.0f, &shape->alpha.as.f32);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_INVALID_OBJECT_TYPE) {
				tpbrt_free_shape(shape);
				return err;
			}

		tpbrt_string_t temp_string;
			if (err == TPBRT_ERROR_INVALID_OBJECT_TYPE) {
				err = tpbrt_params_list_get_string(params, &ALPHA_STR, &temp_string);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_free_shape(shape);
						return err;
					}

				err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &temp_string, TPBRT_TEXTURE_TYPE_FLOAT,
				  &shape->alpha);
				tpbrt_free_string(&temp_string);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_free_shape(shape);
						return err;
					}
			}
			else { shape->alpha.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT; }

		err = tpbrt_shape_type_from_string(type_str, &shape->type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_shape(shape);
				return err;
			}

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
					err = tpbrt_params_list_get_point3s(params, &P_STR, &temp_P);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

						if (temp_P.count < 4) {
							tpbrt_free_vec3_array(&temp_P);
							tpbrt_free_shape(shape);
							return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
						}

						if (temp_P.count > 4) {
							tpbrt_free_vec3_array(&temp_P);
							tpbrt_free_shape(shape);
							return TPBRT_ERROR_TOO_MANY_VALUES;
						}

					shape_params->P[0] = temp_P.data[0];
					shape_params->P[1] = temp_P.data[1];
					shape_params->P[2] = temp_P.data[2];
					shape_params->P[3] = temp_P.data[3];
					tpbrt_free_vec3_array(&temp_P);

					err = tpbrt_params_list_get_string(params, &BASIS_STR, &temp_string);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_shape_curve_basis_from_string(&temp_string, &shape_params->basis);
					tpbrt_free_string(&temp_string);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					tpbrt_uint_t temp_uint;
					err = tpbrt_params_list_get_uint(params, &DEGREE_STR, 3u, &temp_uint);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

						if (temp_uint > 3 || temp_uint < 2) {
							tpbrt_free_shape(shape);
							return TPBRT_ERROR_INVALID_PARAM_VALUE;
						}

					shape_params->degree = (tpbrt_shape_curve_degree_t)temp_uint;

					err					 = tpbrt_params_list_get_string(params, &TYPE_STR, &temp_string);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_shape_curve_type_from_string(&temp_string, &shape_params->type);
					tpbrt_free_string(&temp_string);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

						if (shape_params->type == TPBRT_SHAPE_CURVE_TYPE_RIBBON) {
							tpbrt_normal_array_t temp_normal;
							err = tpbrt_params_list_get_normals(params, &N_STR, &temp_normal);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_shape(shape);
									return err;
								}

								if (temp_normal.count < 2) {
									tpbrt_free_vec3_array(&temp_normal);
									tpbrt_free_shape(shape);
									return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
								}

								if (temp_normal.count > 2) {
									tpbrt_free_vec3_array(&temp_normal);
									tpbrt_free_shape(shape);
									return TPBRT_ERROR_TOO_MANY_VALUES;
								}

							shape_params->N[0] = temp_normal.data[0];
							shape_params->N[1] = temp_normal.data[1];
							tpbrt_free_vec3_array(&temp_normal);
						}

					err = tpbrt_params_list_get_float(params, &WIDTH_STR, 1.0f, &shape_params->width);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_opt_float(params, &WIDTH_0_STR, &shape_params->width0);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_opt_float(params, &WIDTH_1_STR, &shape_params->width1);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_uint(params, &SPLIT_DEPTH_STR, 3u, &shape_params->split_depth);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
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

					err = tpbrt_params_list_get_float(params, &RADIUS_STR, 1.0f, &shape_params->radius);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &Z_MIN_STR, -1.0f, &shape_params->z_min);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &Z_MAX_STR, 1.0f, &shape_params->z_max);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &PHI_MAX_STR, 360.0f, &shape_params->phi_max);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
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

					err = tpbrt_params_list_get_float(params, &HEIGHT_STR, 0.0f, &shape_params->height);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &RADIUS_STR, 1.0f, &shape_params->radius);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &INNER_RADIUS_STR, 0.0f, &shape_params->inner_radius);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &PHI_MAX_STR, 360.0f, &shape_params->phi_max);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
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

					err = tpbrt_params_list_get_float(params, &RADIUS_STR, 1.0f, &shape_params->radius);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &Z_MIN_STR, -shape_params->radius, &shape_params->z_min);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &Z_MAX_STR, shape_params->radius, &shape_params->z_max);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &PHI_MAX_STR, 360.0f, &shape_params->phi_max);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
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

					err = tpbrt_params_list_get_point3s(params, &P_STR, &shape_params->P);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_uints(params, &INDICES_STR, &shape_params->indices);
						if (err != TPBRT_ERROR_NONE && !(shape_params->indices.count == 3 && err == TPBRT_ERROR_NOT_FOUND)) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_normals(params, &N_STR, &shape_params->N);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_vec3s(params, &S_STR, &shape_params->S);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_point2s(params, &UV_STR, &shape_params->uv);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_shape(shape);
							return err;
						}
					break;
				}
				case TPBRT_SHAPE_TYPE_PLY_MESH: {
					static const tpbrt_string_t FILE_NAME_STR	 = TPBRT_STRING("filename");
					static const tpbrt_string_t DISPLACEMENT_STR = TPBRT_STRING("displacement");
					static const tpbrt_string_t EDGE_LENGTH_STR	 = TPBRT_STRING("edgelength");

					tpbrt_shape_ply_mesh_params_t* shape_params	 = &shape->as.ply_mesh;

					err = tpbrt_params_list_get_string(params, &FILE_NAME_STR, &shape_params->file_name);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_string(params, &DISPLACEMENT_STR, &temp_string);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_shape(shape);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							shape_params->displacement.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE;
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &temp_string,
							  TPBRT_TEXTURE_TYPE_FLOAT, &shape_params->displacement);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_shape(shape);
									return err;
								}
						}

					err = tpbrt_params_list_get_float(params, &EDGE_LENGTH_STR, 1.0f, &shape_params->edge_length);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}
					break;
				}
				case TPBRT_SHAPE_TYPE_LOOP_SUBDIV: {
					static const tpbrt_string_t LEVELS_STR		   = TPBRT_STRING("levels");
					static const tpbrt_string_t INDICES_STR		   = TPBRT_STRING("indices");
					static const tpbrt_string_t P_STR			   = TPBRT_STRING("P");

					tpbrt_shape_loop_subdiv_params_t* shape_params = &shape->as.loop_subdiv;

					err = tpbrt_params_list_get_uint(params, &LEVELS_STR, 3u, &shape_params->levels);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_uints(params, &INDICES_STR, &shape_params->indices);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}

					err = tpbrt_params_list_get_points(params, &P_STR, &shape_params->P);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_shape(shape);
							return err;
						}
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_shape(tpbrt_shape_t* const shape) {
			if (shape == TPBRT_NULL) { return; }

			switch (shape->type) {
				case TPBRT_SHAPE_TYPE_TRIANGLE_MESH: {
					tpbrt_shape_triangle_mesh_params_t* const params = &shape->as.triangle_mesh;

					tpbrt_free_uint_array(&params->indices);
					tpbrt_free_vec3_array(&params->P);
					tpbrt_free_vec3_array(&params->N);
					tpbrt_free_vec3_array(&params->S);
					tpbrt_free_vec2_array(&params->uv);
				}
				case TPBRT_SHAPE_TYPE_PLY_MESH: {
					tpbrt_shape_ply_mesh_params_t* const params = &shape->as.ply_mesh;

					tpbrt_free_string(&params->file_name);
				}
				case TPBRT_SHAPE_TYPE_LOOP_SUBDIV: {
					tpbrt_shape_loop_subdiv_params_t* const params = &shape->as.loop_subdiv;

					tpbrt_free_uint_array(&params->indices);
					tpbrt_free_vec3_array(&params->P);
				}
			default: break;
			}
	}

#pragma endregion

#pragma region SHAPE_ARRAY

	tpbrt_error_t tpbrt_init_shape_array(tpbrt_shape_array_t* const shape_array) {
			if (shape_array == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		shape_array->data  = TPBRT_NULL;
		shape_array->count = 0;

		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_shape_array_add_shape(tpbrt_shape_array_t* const shape_array, const tpbrt_shape_t* const shape) {
			if (shape_array == TPBRT_NULL || shape == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_size_t new_count = shape_array->count + 1;

		tpbrt_shape_t* new_list		 = realloc(shape_array->data, sizeof(tpbrt_shape_t) * new_count);
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		shape_array->data					  = new_list;
		shape_array->data[shape_array->count] = *shape;
		shape_array->count					  = new_count;
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_shape_array(tpbrt_shape_array_t* const shape_array) {
			if (shape_array == TPBRT_NULL || shape_array->data == TPBRT_NULL) { return; }

			for (tpbrt_size_t i = 0; i < shape_array->count; ++i) { tpbrt_free_shape(shape_array->data + i); }

		free(shape_array->data);
		shape_array->data  = TPBRT_NULL;
		shape_array->count = 0;
	}

#pragma endregion

#pragma region OBJECT

	tpbrt_error_t tpbrt_create_object(const tpbrt_string_t* const name, tpbrt_object_t* const object) {
			if (name == TPBRT_NULL || object == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(object, 0, sizeof(tpbrt_object_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		object->idx		  = TPBRT_OBJECT_HANDLE_INVALID;

		tpbrt_error_t err = tpbrt_init_shape_array(&object->shapes);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_object(object);
				return err;
			}

		err = tpbrt_copy_string(&object->name, name);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_object(object);
				return err;
			}

		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_object_add_shape(tpbrt_object_t* const object, const tpbrt_shape_t* const shape) {
			if (object == TPBRT_NULL || shape == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		return tpbrt_shape_array_add_shape(&object->shapes, shape);
	}

	void tpbrt_free_object(tpbrt_object_t* const object) {
			if (object == TPBRT_NULL) { return; }

		tpbrt_free_string(&object->name);
		tpbrt_free_shape_array(&object->shapes);
	}

#pragma endregion

#pragma region OBJECTS_LIST

	tpbrt_error_t tpbrt_init_objects_list(tpbrt_objects_list_t* const objects_list) {
			if (objects_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		objects_list->objects = TPBRT_NULL;
		objects_list->count	  = 0;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_objects_list_add_object(tpbrt_objects_list_t* const objects_list, const tpbrt_object_t* const object) {
			if (object == TPBRT_NULL || objects_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_size_t new_count = objects_list->count + 1;

		tpbrt_object_t* new_list	 = realloc(objects_list->objects, sizeof(tpbrt_object_t) * new_count);
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		objects_list->objects						   = new_list;
		objects_list->objects[objects_list->count]	   = *object;
		objects_list->objects[objects_list->count].idx = objects_list->count;
		objects_list->count							   = new_count;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_objects_list_get_object(const tpbrt_objects_list_t* const objects_list,
	  const tpbrt_string_t* const object_name, const tpbrt_object_t** const object) {
			if (objects_list == TPBRT_NULL || object_name == TPBRT_NULL || object_name->data == TPBRT_NULL ||
				object == TPBRT_NULL) {
				*object = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_size_t i = 0; i < objects_list->count; i++) {
					if (tpbrt_string_equals(&objects_list->objects[i].name, object_name)) {
						*object = &objects_list->objects[i];
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_NOT_FOUND;
	}

	tpbrt_error_t tpbrt_objects_list_get_object_handle(const tpbrt_objects_list_t* const objects_list,
	  const tpbrt_string_t* const object_name, tpbrt_object_handle_t* const object_handle) {
			if (objects_list == TPBRT_NULL || object_name == TPBRT_NULL || object_name->data == TPBRT_NULL ||
				object_handle == TPBRT_NULL) {
				*object_handle = ~(tpbrt_object_handle_t)0;
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_object_t* object;
		const tpbrt_error_t err = tpbrt_objects_list_get_object(objects_list, object_name, &object);

			if (err != TPBRT_ERROR_NONE) {
				*object_handle = ~(tpbrt_object_handle_t)0;
				return err;
			}

		*object_handle = object->idx;
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_objects_list(tpbrt_objects_list_t* const objects_list) {
			if (objects_list == TPBRT_NULL || objects_list->objects == TPBRT_NULL) { return; }

			for (tpbrt_size_t i = 0; i < objects_list->count; ++i) { tpbrt_free_object(objects_list->objects + i); }

		free(objects_list->objects);
		objects_list->objects = TPBRT_NULL;
		objects_list->count	  = 0;
	}

	tpbrt_size_t tpbrt_objects_list_size(const tpbrt_objects_list_t* const objects_list) {
		return objects_list != TPBRT_NULL ? objects_list->count : 0;
	}

	tpbrt_bool_t tpbrt_objects_list_is_empty(const tpbrt_objects_list_t* const objects_list) {
		return tpbrt_objects_list_size(objects_list) == 0;
	}

#pragma endregion

#pragma region OBJECT_INSTANCE

	tpbrt_error_t tpbrt_create_instance(const tpbrt_string_t* const object_name, const tpbrt_objects_list_t* const objects,
	  const tpbrt_mat4_animated_t* const ctm, tpbrt_instance_t* const instance) {
			if (object_name == TPBRT_NULL || object_name->data == TPBRT_NULL || objects == TPBRT_NULL || ctm == TPBRT_NULL ||
				instance == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (memset(instance, 0, sizeof(tpbrt_instance_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		instance->transform		= *ctm;

		const tpbrt_error_t err = tpbrt_objects_list_get_object_handle(objects, object_name, &instance->object);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_instance(instance);
				return err;
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_instance(tpbrt_instance_t* const instance) {
			if (instance == TPBRT_NULL) { return; }
		instance->object = TPBRT_OBJECT_HANDLE_INVALID;
	}

#pragma endregion

#pragma region OBJECT_INSTANCES_LIST

	tpbrt_error_t tpbrt_init_instances_list(tpbrt_instances_list_t* const instances_list) {
			if (instances_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		instances_list->instances = TPBRT_NULL;
		instances_list->count	  = 0;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_instances_list_add_instance(tpbrt_instances_list_t* const instances_list,
	  const tpbrt_instance_t* const instance) {
			if (instances_list == TPBRT_NULL || instance == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_size_t new_count = instances_list->count + 1;

		tpbrt_instance_t* new_list	 = realloc(instances_list->instances, sizeof(tpbrt_instance_t) * new_count);
			if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

		instances_list->instances						 = new_list;
		instances_list->instances[instances_list->count] = *instance;
		instances_list->count							 = new_count;
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_instances_list(tpbrt_instances_list_t* const instances_list) {
			if (instances_list == TPBRT_NULL || instances_list->instances == TPBRT_NULL) { return; }

			for (tpbrt_size_t i = 0; i < instances_list->count; ++i) { tpbrt_free_instance(instances_list->instances + i); }

		free(instances_list->instances);
		instances_list->instances = TPBRT_NULL;
		instances_list->count	  = 0;
	}

#pragma endregion

#pragma region API

	tpbrt_error_t tpbrt_get_object_by_name(const tpbrt_objects_list_t* const objects, const tpbrt_string_t* const name,
	  const tpbrt_object_t** const object) {
		return tpbrt_objects_list_get_object(objects, name, object);
	}

	tpbrt_error_t tpbrt_get_object_by_handle(const tpbrt_objects_list_t* const objects, const tpbrt_object_handle_t handle,
	  const tpbrt_object_t** const object) {
			if (objects == TPBRT_NULL || object == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (handle >= objects->count) {
				*object = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_HANDLE;
			}

		*object = &objects->objects[handle];
		return TPBRT_ERROR_NONE;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif