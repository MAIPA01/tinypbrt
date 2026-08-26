#include "texture_internal.h"


#include <tinypbrt/pch.h>

#include <tinypbrt/detail/material_internal.h>

#include <tinypbrt/detail/common_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region MATERIAL

	static tpbrt_error_t tpbrt_material_type_from_string(const tpbrt_string_t* const type_str,
	  tpbrt_material_type_t* const type) {
		static const tpbrt_string_t TYPES_STRS[TPBRT_MATERIAL_TYPE_MAX_NUM] = {
			TPBRT_STRING("coateddiffuse"),
			TPBRT_STRING("coatedconductor"),
			TPBRT_STRING("conductor"),
			TPBRT_STRING("dielectric"),
			TPBRT_STRING("diffuse"),
			TPBRT_STRING("diffusetransmission"),
			TPBRT_STRING("hair"),
			TPBRT_STRING("interface"),
			TPBRT_STRING("measured"),
			TPBRT_STRING("mix"),
			TPBRT_STRING("subsurface"),
			TPBRT_STRING("thindielectric"),
		};

			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || type == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_material_type_t t = 0; t < TPBRT_MATERIAL_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS + t)) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_UNKNOWN_MATERIAL_TYPE;
	}

	tpbrt_error_t tpbrt_create_material(const tpbrt_string_t* const type_str, const tpbrt_params_list_t* const params,
	  const tpbrt_textures_list_t* const textures, const tpbrt_materials_list_t* const materials,
	  tpbrt_material_t* const material) {
			if (type_str == TPBRT_NULL || type_str->data == TPBRT_NULL || params == TPBRT_NULL || textures == TPBRT_NULL ||
				materials == TPBRT_NULL || material == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (memset(material, 0, sizeof(tpbrt_material_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		material->idx	  = TPBRT_MATERIAL_HANDLE_INVALID;

		tpbrt_error_t err = tpbrt_material_type_from_string(type_str, &material->type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_material(material);
				return err;
			}

		static const tpbrt_string_t DISPLACEMENT_STR	= TPBRT_STRING("displacement");
		static const tpbrt_string_t NORMAL_MAP_STR		= TPBRT_STRING("normalmap");
		static const tpbrt_string_t ROUGHNESS_STR		= TPBRT_STRING("roughness");
		static const tpbrt_string_t U_ROUGHNESS_STR		= TPBRT_STRING("uroughness");
		static const tpbrt_string_t V_ROUGHNESS_STR		= TPBRT_STRING("vroughness");
		static const tpbrt_string_t REMAP_ROUGHNESS_STR = TPBRT_STRING("remaproughness");

			switch (material->type) {
			default:
				case TPBRT_MATERIAL_TYPE_COATED_DIFFUSE: {
					static const tpbrt_string_t ALBEDO_STR			   = TPBRT_STRING("albedo");
					static const tpbrt_string_t G_STR				   = TPBRT_STRING("g");
					static const tpbrt_string_t MAX_DEPTH_STR		   = TPBRT_STRING("maxdepth");
					static const tpbrt_string_t N_SAMPLES_STR		   = TPBRT_STRING("nsamples");
					static const tpbrt_string_t THICKNESS_STR		   = TPBRT_STRING("thickness");
					static const tpbrt_string_t REFLECTANCE_STR		   = TPBRT_STRING("reflectance");

					tpbrt_material_coated_diffuse_params_t* mat_params = &material->as.coated_diffuse;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_texture_of_type(params, &DISPLACEMENT_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->bump_map.as.displacement);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NONE) { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						else if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &mat_params->bump_map.as.normal_map);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NONE) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
								}
								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
						}
#pragma endregion

					err = tpbrt_params_list_get_texture_of_type(params, &ALBEDO_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
					  &mat_params->albedo);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->albedo.value_type	  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->albedo.as.float_value = 0.0f;
						}

					err =
					  tpbrt_params_list_get_texture_of_type(params, &G_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &mat_params->g);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->g.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->g.as.float_value = 0.0f;
						}

					static const tpbrt_uint_t MAX_DEPTH_DEFAULT = 10u;
					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, MAX_DEPTH_DEFAULT, &mat_params->max_depth);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_material(material);
							return err;
						}

					err = tpbrt_params_list_get_uint(params, &N_SAMPLES_STR, 1, &mat_params->n_samples);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_material(material);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &THICKNESS_STR, 0.01f, &mat_params->thickness);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_material(material);
							return err;
						}

					err = tpbrt_params_list_get_texture(params, &REFLECTANCE_STR, textures, &mat_params->reflectance);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->reflectance.value_type	   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->reflectance.as.float_value = 0.5f;
						}

#pragma region ROUGHNESS
					err = tpbrt_params_list_get_texture_of_type(params, &ROUGHNESS_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->roughness_params.roughness);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.roughness.value_type	  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.roughness.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &U_ROUGHNESS_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->roughness_params.u_roughness);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.u_roughness.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.u_roughness.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &V_ROUGHNESS_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->roughness_params.v_roughness);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.v_roughness.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.v_roughness.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_bool(params, &REMAP_ROUGHNESS_STR, TPBRT_TRUE,
					  &mat_params->roughness_params.remap_roughness);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_material(material);
							return err;
						}
#pragma endregion
					break;
				}
				case TPBRT_MATERIAL_TYPE_COATED_CONDUCTOR: {
					static const tpbrt_string_t ALBEDO_STR				 = TPBRT_STRING("albedo");
					static const tpbrt_string_t G_STR					 = TPBRT_STRING("g");
					static const tpbrt_string_t MAX_DEPTH_STR			 = TPBRT_STRING("maxdepth");
					static const tpbrt_string_t N_SAMPLES_STR			 = TPBRT_STRING("nsamples");
					static const tpbrt_string_t THICKNESS_STR			 = TPBRT_STRING("thickness");
					static const tpbrt_string_t CONDUCTOR_ETA_STR		 = TPBRT_STRING("conductor.eta");
					static const tpbrt_string_t CONDUCTOR_K_STR			 = TPBRT_STRING("conductor.k");
					static const tpbrt_string_t REFLECTANCE_STR			 = TPBRT_STRING("reflectance");

					tpbrt_material_coated_conductor_params_t* mat_params = &material->as.coated_conductor;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_texture_of_type(params, &DISPLACEMENT_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->bump_map.as.displacement);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NONE) { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						else if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &mat_params->bump_map.as.normal_map);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NONE) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
								}
								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
						}
#pragma endregion

					err = tpbrt_params_list_get_texture_of_type(params, &ALBEDO_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
					  &mat_params->albedo);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->albedo.value_type	  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->albedo.as.float_value = 0.0f;
						}

					err =
					  tpbrt_params_list_get_texture_of_type(params, &G_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &mat_params->g);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->g.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->g.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_uint(params, &MAX_DEPTH_STR, 10u, &mat_params->max_depth);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_material(material);
							return err;
						}

					err = tpbrt_params_list_get_uint(params, &N_SAMPLES_STR, 1, &mat_params->n_samples);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_material(material);
							return err;
						}

					err = tpbrt_params_list_get_float(params, &THICKNESS_STR, 0.01f, &mat_params->thickness);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_material(material);
							return err;
						}

					err = tpbrt_params_list_get_spectrum(params, &REFLECTANCE_STR, &mat_params->as.reflectance);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_spectrum(params, &CONDUCTOR_ETA_STR, &mat_params->as.conductor.eta);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->as.conductor.eta.type		= TPBRT_SPECTRUM_TYPE_BUILTIN;
									mat_params->as.conductor.eta.as.builtin = TPBRT_SPECTRUM_BUILTIN_METAL_CU_ETA;
								}

							err = tpbrt_params_list_get_spectrum(params, &CONDUCTOR_K_STR, &mat_params->as.conductor.k);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->as.conductor.k.type		  = TPBRT_SPECTRUM_TYPE_BUILTIN;
									mat_params->as.conductor.k.as.builtin = TPBRT_SPECTRUM_BUILTIN_METAL_CU_K;
								}
						}

#pragma region ROUGHNESS
					err = tpbrt_params_list_get_texture_of_type(params, &ROUGHNESS_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->roughness_params.roughness);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.roughness.value_type	  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.roughness.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &U_ROUGHNESS_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->roughness_params.u_roughness);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.u_roughness.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.u_roughness.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &V_ROUGHNESS_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->roughness_params.v_roughness);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.v_roughness.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.v_roughness.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_bool(params, &REMAP_ROUGHNESS_STR, TPBRT_TRUE,
					  &mat_params->roughness_params.remap_roughness);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_material(material);
							return err;
						}
#pragma endregion
					break;
				}
				case TPBRT_MATERIAL_TYPE_CONDUCTOR: {
					static const tpbrt_string_t ETA_STR			  = TPBRT_STRING("eta");
					static const tpbrt_string_t K_STR			  = TPBRT_STRING("k");
					static const tpbrt_string_t REFLECTANCE_STR	  = TPBRT_STRING("reflectance");

					tpbrt_material_conductor_params_t* mat_params = &material->as.conductor;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_texture_of_type(params, &DISPLACEMENT_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->bump_map.as.displacement);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NONE) { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						else if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &mat_params->bump_map.as.normal_map);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NONE) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
								}
								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
						}
#pragma endregion

					err = tpbrt_params_list_get_texture_of_type(params, &REFLECTANCE_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
					  &mat_params->as.reflectance);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NONE) { mat_params->value_type = TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_REFLECTANCE; }
						else if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->value_type = TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_ETA_K;

							err = tpbrt_params_list_get_texture_of_type(params, &ETA_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
							  &mat_params->as.conductor.eta);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->as.conductor.eta.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM_BUILTIN;
									mat_params->as.conductor.eta.as.spectrum_builtin = TPBRT_SPECTRUM_BUILTIN_METAL_CU_ETA;
								}

							err = tpbrt_params_list_get_texture_of_type(params, &K_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
							  &mat_params->as.conductor.k);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->as.conductor.k.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM_BUILTIN;
									mat_params->as.conductor.k.as.spectrum_builtin = TPBRT_SPECTRUM_BUILTIN_METAL_CU_K;
								}
						}

#pragma region ROUGHNESS
					err = tpbrt_params_list_get_texture_of_type(params, &ROUGHNESS_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->roughness_params.roughness);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.roughness.value_type	  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.roughness.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &U_ROUGHNESS_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->roughness_params.u_roughness);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.u_roughness.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.u_roughness.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &V_ROUGHNESS_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->roughness_params.v_roughness);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.v_roughness.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.v_roughness.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_bool(params, &REMAP_ROUGHNESS_STR, TPBRT_TRUE,
					  &mat_params->roughness_params.remap_roughness);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_material(material);
							return err;
						}
#pragma endregion
					break;
				}
				case TPBRT_MATERIAL_TYPE_DIELECTRIC: {
					static const tpbrt_string_t ETA_STR			   = TPBRT_STRING("eta");

					tpbrt_material_dielectric_params_t* mat_params = &material->as.dielectric;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_texture_of_type(params, &DISPLACEMENT_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->bump_map.as.displacement);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NONE) { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						else if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &mat_params->bump_map.as.normal_map);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NONE) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
								}
								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
						}
#pragma endregion

					err = tpbrt_params_list_get_texture(params, &ETA_STR, textures, &mat_params->eta);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->eta.value_type	   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->eta.as.float_value = 1.5f;
						}

#pragma region ROUGHNESS
					err = tpbrt_params_list_get_texture_of_type(params, &ROUGHNESS_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->roughness_params.roughness);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.roughness.value_type	  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.roughness.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &U_ROUGHNESS_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->roughness_params.u_roughness);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.u_roughness.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.u_roughness.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &V_ROUGHNESS_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->roughness_params.v_roughness);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.v_roughness.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.v_roughness.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_bool(params, &REMAP_ROUGHNESS_STR, TPBRT_TRUE,
					  &mat_params->roughness_params.remap_roughness);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_material(material);
							return err;
						}
#pragma endregion
					break;
				}
				case TPBRT_MATERIAL_TYPE_DIFFUSE: {
					static const tpbrt_string_t REFLECTANCE_STR = TPBRT_STRING("reflectance");

					tpbrt_material_diffuse_params_t* mat_params = &material->as.diffuse;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_texture_of_type(params, &DISPLACEMENT_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->bump_map.as.displacement);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NONE) { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						else if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &mat_params->bump_map.as.normal_map);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NONE) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
								}
								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
						}
#pragma endregion

					err = tpbrt_params_list_get_texture_of_type(params, &REFLECTANCE_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
					  &mat_params->reflectance);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->reflectance.value_type	   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->reflectance.as.float_value = 0.5f;
						}
					break;
				}
				case TPBRT_MATERIAL_TYPE_DIFFUSE_TRANSMISSION: {
					static const tpbrt_string_t REFLECTANCE_STR				 = TPBRT_STRING("reflectance");
					static const tpbrt_string_t TRANSMITTANCE_STR			 = TPBRT_STRING("transmittance");
					static const tpbrt_string_t SCALE_STR					 = TPBRT_STRING("scale");

					tpbrt_material_diffuse_transmission_params_t* mat_params = &material->as.diffuse_transmission;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_texture_of_type(params, &DISPLACEMENT_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->bump_map.as.displacement);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NONE) { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						else if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &mat_params->bump_map.as.normal_map);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NONE) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
								}
								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
						}
#pragma endregion

					err = tpbrt_params_list_get_texture_of_type(params, &REFLECTANCE_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
					  &mat_params->reflectance);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->reflectance.value_type	   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->reflectance.as.float_value = 0.25f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &TRANSMITTANCE_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
					  &mat_params->transmittance);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->transmittance.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->transmittance.as.float_value = 0.25f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &SCALE_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->scale);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->scale.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->scale.as.float_value = 1.0f;
						}
					break;
				}
				case TPBRT_MATERIAL_TYPE_HAIR: {
					static const tpbrt_string_t SIGMA_A_STR		= TPBRT_STRING("sigma_a");
					static const tpbrt_string_t REFLECTANCE_STR = TPBRT_STRING("reflectance");
					static const tpbrt_string_t EUMELANIN_STR	= TPBRT_STRING("eumelanin");
					static const tpbrt_string_t PHEOMELANIN_STR = TPBRT_STRING("pheomelanin");
					static const tpbrt_string_t ETA_STR			= TPBRT_STRING("eta");
					static const tpbrt_string_t BETA_M_STR		= TPBRT_STRING("beta_m");
					static const tpbrt_string_t BETA_N_STR		= TPBRT_STRING("beta_n");
					static const tpbrt_string_t ALPHA_STR		= TPBRT_STRING("alpha");

					tpbrt_material_hair_params_t* mat_params	= &material->as.hair;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_texture_of_type(params, &DISPLACEMENT_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->bump_map.as.displacement);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NONE) { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						else if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &mat_params->bump_map.as.normal_map);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NONE) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
								}
								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
						}
#pragma endregion

					err = tpbrt_params_list_get_texture_of_type(params, &SIGMA_A_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
					  &mat_params->sigma_a);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &REFLECTANCE_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
					  &mat_params->reflectance);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &EUMELANIN_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->eumelanin);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->eumelanin.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->eumelanin.as.float_value = 1.3f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &PHEOMELANIN_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->pheomelanin);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &ETA_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->eta);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->eta.value_type	   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->eta.as.float_value = 1.55f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &BETA_M_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->beta_m);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->beta_m.value_type	  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->beta_m.as.float_value = 0.3f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &BETA_N_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->beta_n);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->beta_n.value_type	  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->beta_n.as.float_value = 0.3f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &ALPHA_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->alpha);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->alpha.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->alpha.as.float_value = 2.0f;
						}
					break;
				}
			case TPBRT_MATERIAL_TYPE_INTERFACE:	   break;
				case TPBRT_MATERIAL_TYPE_MEASURED: {
					static const tpbrt_string_t FILE_NAME_STR	 = TPBRT_STRING("filename");

					tpbrt_material_measured_params_t* mat_params = &material->as.measured;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_texture_of_type(params, &DISPLACEMENT_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->bump_map.as.displacement);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NONE) { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						else if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &mat_params->bump_map.as.normal_map);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NONE) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
								}
								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
						}
#pragma endregion

					err = tpbrt_params_list_get_string(params, &FILE_NAME_STR, &mat_params->file_name);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_material(material);
							return err;
						}
					break;
				}
				case TPBRT_MATERIAL_TYPE_MIX: {
					static const tpbrt_string_t MATERIALS_STR = TPBRT_STRING("materials");
					static const tpbrt_string_t AMOUNT_STR	  = TPBRT_STRING("amount");

					tpbrt_material_mix_params_t* mat_params	  = &material->as.mix;

					tpbrt_string_array_t temp_strings;
					err = tpbrt_params_list_get_strings(params, &MATERIALS_STR, &temp_strings);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_material(material);
							return err;
						}

						if (temp_strings.count < 2) {
							tpbrt_free_string_array(&temp_strings);
							tpbrt_free_material(material);
							return TPBRT_ERROR_MISSING_REQUIRED_PARAMETER;
						}

						if (temp_strings.count > 2) {
							tpbrt_free_string_array(&temp_strings);
							tpbrt_free_material(material);
							return TPBRT_ERROR_TOO_MANY_VALUES;
						}

						for (tpbrt_size_t i = 0; i < temp_strings.count; ++i) {
							err = tpbrt_materials_list_get_material_handle(materials, &temp_strings.data[i],
							  &mat_params->materials[i]);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_string_array(&temp_strings);
									tpbrt_free_material(material);
									return err;
								}
						}
					tpbrt_free_string_array(&temp_strings);

					err = tpbrt_params_list_get_texture_of_type(params, &AMOUNT_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->amount);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->amount.value_type	  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->amount.as.float_value = 0.5f;
						}
					break;
				}
				case TPBRT_MATERIAL_TYPE_SUBSURFACE: {
					static const tpbrt_string_t ETA_STR			   = TPBRT_STRING("eta");
					static const tpbrt_string_t G_STR			   = TPBRT_STRING("g");
					static const tpbrt_string_t MFP_STR			   = TPBRT_STRING("mfp");
					static const tpbrt_string_t NAME_STR		   = TPBRT_STRING("name");
					static const tpbrt_string_t REFLECTANCE_STR	   = TPBRT_STRING("reflectance");
					static const tpbrt_string_t SIGMA_A_STR		   = TPBRT_STRING("sigma_a");
					static const tpbrt_string_t SIGMA_S_STR		   = TPBRT_STRING("sigma_s");
					static const tpbrt_string_t SCALE_STR		   = TPBRT_STRING("scale");

					tpbrt_material_subsurface_params_t* mat_params = &material->as.subsurface;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_texture_of_type(params, &DISPLACEMENT_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->bump_map.as.displacement);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NONE) { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						else if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &mat_params->bump_map.as.normal_map);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NONE) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
								}
								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
						}
#pragma endregion

					err = tpbrt_params_list_get_texture_of_type(params, &ETA_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->eta);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->eta.value_type	   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->eta.as.float_value = 1.33f;
						}

					err =
					  tpbrt_params_list_get_texture_of_type(params, &G_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &mat_params->g);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->g.value_type	 = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->g.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &MFP_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->mfp);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					err = tpbrt_params_list_get_string(params, &NAME_STR, &mat_params->name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &REFLECTANCE_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
					  &mat_params->reflectance);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &SIGMA_A_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
					  &mat_params->sigma_a);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->sigma_a.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_RGB;
							mat_params->sigma_a.as.rgb	   = (tpbrt_rgb_t) { .r = 0.0011f, .g = 0.0024f, .b = 0.014f };
						}

					err = tpbrt_params_list_get_texture_of_type(params, &SIGMA_S_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
					  &mat_params->sigma_s);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->sigma_s.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_RGB;
							mat_params->sigma_s.as.rgb	   = (tpbrt_rgb_t) { .r = 2.55f, .g = 3.12f, .b = 3.77f };
						}

					err = tpbrt_params_list_get_float(params, &SCALE_STR, 1.0f, &mat_params->scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_material(material);
							return err;
						}

#pragma region ROUGHNESS
					err = tpbrt_params_list_get_texture_of_type(params, &ROUGHNESS_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->roughness_params.roughness);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.roughness.value_type	  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.roughness.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &U_ROUGHNESS_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->roughness_params.u_roughness);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.u_roughness.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.u_roughness.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_texture_of_type(params, &V_ROUGHNESS_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->roughness_params.v_roughness);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.v_roughness.value_type		= TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.v_roughness.as.float_value = 0.0f;
						}

					err = tpbrt_params_list_get_bool(params, &REMAP_ROUGHNESS_STR, TPBRT_TRUE,
					  &mat_params->roughness_params.remap_roughness);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_material(material);
							return err;
						}
#pragma endregion
					break;
				}
				case TPBRT_MATERIAL_TYPE_THIN_DIELECTRIC: {
					tpbrt_material_thin_dielectric_params_t* mat_params = &material->as.thin_dielectric;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_texture_of_type(params, &DISPLACEMENT_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
					  &mat_params->bump_map.as.displacement);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NONE) { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						else if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &mat_params->bump_map.as.normal_map);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NONE) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
								}
								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
						}
#pragma endregion
					break;
				}
			}

		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_create_named_material(const tpbrt_string_t* const name, const tpbrt_params_list_t* const params,
	  const tpbrt_textures_list_t* const textures, const tpbrt_materials_list_t* const materials,
	  tpbrt_material_t* const material) {
			if (name == TPBRT_NULL || name->data == TPBRT_NULL || params == TPBRT_NULL || textures == TPBRT_NULL ||
				materials == TPBRT_NULL || material == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			if (memset(material, 0, sizeof(tpbrt_material_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t TYPE_STR = TPBRT_STRING("type");

		tpbrt_string_t type_str;
		tpbrt_error_t err = tpbrt_params_list_get_string(params, &TYPE_STR, &type_str);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_create_material(&type_str, params, textures, materials, material);
		tpbrt_free_string(&type_str);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_copy_string(&material->name, name);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_free_material(material);
				return err;
			}

		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_material(tpbrt_material_t* const material) {
			if (material == TPBRT_NULL) { return; }

		tpbrt_free_string(&material->name);

			switch (material->type) {
				case TPBRT_MATERIAL_TYPE_COATED_DIFFUSE: {
					tpbrt_material_coated_diffuse_params_t* params = &material->as.coated_diffuse;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}
					break;
				}
				case TPBRT_MATERIAL_TYPE_COATED_CONDUCTOR: {
					tpbrt_material_coated_conductor_params_t* params = &material->as.coated_conductor;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}
					break;
				}
				case TPBRT_MATERIAL_TYPE_CONDUCTOR: {
					tpbrt_material_conductor_params_t* params = &material->as.conductor;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}
					break;
				}
				case TPBRT_MATERIAL_TYPE_DIELECTRIC: {
					tpbrt_material_dielectric_params_t* params = &material->as.dielectric;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}
					break;
				}
				case TPBRT_MATERIAL_TYPE_DIFFUSE: {
					tpbrt_material_diffuse_params_t* params = &material->as.diffuse;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}
					break;
				}
				case TPBRT_MATERIAL_TYPE_DIFFUSE_TRANSMISSION: {
					tpbrt_material_diffuse_transmission_params_t* params = &material->as.diffuse_transmission;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}
					break;
				}
				case TPBRT_MATERIAL_TYPE_HAIR: {
					tpbrt_material_hair_params_t* params = &material->as.hair;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}
					break;
				}
				case TPBRT_MATERIAL_TYPE_MEASURED: {
					tpbrt_material_measured_params_t* params = &material->as.measured;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}

					tpbrt_free_string(&params->file_name);
					break;
				}
				case TPBRT_MATERIAL_TYPE_SUBSURFACE: {
					tpbrt_material_subsurface_params_t* params = &material->as.subsurface;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}

					tpbrt_free_string(&params->name);
					break;
				}
				case TPBRT_MATERIAL_TYPE_THIN_DIELECTRIC: {
					tpbrt_material_thin_dielectric_params_t* params = &material->as.thin_dielectric;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}
					break;
				}
			default: break;
			}
	}

#pragma endregion

#pragma region MATERIALS_LIST

	tpbrt_error_t tpbrt_init_materials_list(tpbrt_materials_list_t* const materials_list) {
			if (materials_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		materials_list->materials = TPBRT_NULL;
		materials_list->count	  = 0;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_materials_list_add_material(tpbrt_materials_list_t* const materials_list,
	  const tpbrt_material_t* const material, tpbrt_material_handle_t* const handle) {
			if (material == TPBRT_NULL || materials_list == TPBRT_NULL || handle == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_size_t new_count = materials_list->count + 1;

		tpbrt_material_t* new_list	 = realloc(materials_list->materials, sizeof(tpbrt_material_t) * new_count);
			if (new_list == TPBRT_NULL) {
				*handle = TPBRT_MATERIAL_HANDLE_INVALID;
				return TPBRT_ERROR_OUT_OF_MEMORY;
			}

		materials_list->materials							 = new_list;
		materials_list->materials[materials_list->count]	 = *material;
		materials_list->materials[materials_list->count].idx = materials_list->count;
		*handle												 = materials_list->count;
		materials_list->count								 = new_count;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_materials_list_get_material_by_name(const tpbrt_materials_list_t* const materials_list,
	  const tpbrt_string_t* const material_name, const tpbrt_material_t** const material) {
			if (materials_list == TPBRT_NULL || material_name == TPBRT_NULL || material_name->data == TPBRT_NULL ||
				material == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

			for (tpbrt_size_t i = 0; i < materials_list->count; i++) {
					if (tpbrt_string_equals(&materials_list->materials[i].name, material_name)) {
						*material = &materials_list->materials[i];
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_NOT_FOUND;
	}

	tpbrt_error_t tpbrt_materials_list_get_material_handle(const tpbrt_materials_list_t* const materials_list,
	  const tpbrt_string_t* const material_name, tpbrt_material_handle_t* const material_handle) {
			if (materials_list == TPBRT_NULL || material_name == TPBRT_NULL || material_name->data == TPBRT_NULL ||
				material_handle == TPBRT_NULL) {
				return TPBRT_ERROR_INVALID_POINTER;
			}

		const tpbrt_material_t* material;
		const tpbrt_error_t err = tpbrt_materials_list_get_material_by_name(materials_list, material_name, &material);

			if (err != TPBRT_ERROR_NONE) { return err; }

		*material_handle = material->idx;
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_free_materials_list(tpbrt_materials_list_t* const materials_list) {
			if (materials_list == TPBRT_NULL || materials_list->materials == TPBRT_NULL) { return; }

			for (tpbrt_size_t i = 0; i < materials_list->count; ++i) { tpbrt_free_material(materials_list->materials + i); }

		free(materials_list->materials);
		materials_list->materials = TPBRT_NULL;
		materials_list->count	  = 0;
	}

	tpbrt_size_t tpbrt_materials_list_size(const tpbrt_materials_list_t* const materials_list) {
		return materials_list != TPBRT_NULL ? materials_list->count : 0;
	}

	tpbrt_bool_t tpbrt_materials_list_is_empty(const tpbrt_materials_list_t* const materials_list) {
		return tpbrt_materials_list_size(materials_list) == 0;
	}

#pragma endregion

#pragma region API

	tpbrt_error_t tpbrt_get_material_by_name(const tpbrt_materials_list_t* const materials, const tpbrt_string_t* const name,
	  const tpbrt_material_t** const material) {
		return tpbrt_materials_list_get_material_by_name(materials, name, material);
	}

	tpbrt_error_t tpbrt_get_material_by_handle(const tpbrt_materials_list_t* const materials,
	  const tpbrt_material_handle_t handle, const tpbrt_material_t** const material) {
			if (materials == TPBRT_NULL || material == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (handle == TPBRT_MATERIAL_HANDLE_INVALID || handle >= materials->count) {
				*material = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_HANDLE;
			}

		*material = &materials->materials[handle];
		return TPBRT_ERROR_NONE;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif