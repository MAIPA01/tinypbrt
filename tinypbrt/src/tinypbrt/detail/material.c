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

		material->idx	  = ~(tpbrt_size_t)0;

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

					tpbrt_string_t texture_name;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_string(params, &DISPLACEMENT_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &texture_name);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else {
									mat_params->bump_map.type		   = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
									mat_params->bump_map.as.normal_map = texture_name;
								}
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, &mat_params->bump_map.as.displacement);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						}
#pragma endregion

					err = tpbrt_params_list_get_string(params, &ALBEDO_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->albedo.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->albedo.as.f32	  = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_SPECTRUM, 0.0f, &mat_params->albedo);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &G_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->g.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->g.as.f32	 = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->g);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
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

					err = tpbrt_params_list_get_string(params, &REFLECTANCE_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					static const tpbrt_float_t REFLECTANCE_DEFAULT = 0.5f;
						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->reflectance.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->reflectance.as.f32	   = REFLECTANCE_DEFAULT;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, REFLECTANCE_DEFAULT, &mat_params->reflectance);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

#pragma region ROUGHNESS
					err = tpbrt_params_list_get_string(params, &ROUGHNESS_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.roughness.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.roughness.as.f32	  = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->roughness_params.roughness);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &U_ROUGHNESS_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.u_roughness.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.u_roughness.as.f32		= 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->roughness_params.u_roughness);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &V_ROUGHNESS_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.v_roughness.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.v_roughness.as.f32		= 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->roughness_params.v_roughness);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
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

					tpbrt_string_t texture_name;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_string(params, &DISPLACEMENT_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &texture_name);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else {
									mat_params->bump_map.type		   = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
									mat_params->bump_map.as.normal_map = texture_name;
								}
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, &mat_params->bump_map.as.displacement);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						}
#pragma endregion

					err = tpbrt_params_list_get_string(params, &ALBEDO_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->albedo.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->albedo.as.f32	  = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_SPECTRUM, 0.0f, &mat_params->albedo);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &G_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->g.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->g.as.f32	 = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->g);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
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
					err = tpbrt_params_list_get_string(params, &ROUGHNESS_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.roughness.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.roughness.as.f32	  = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->roughness_params.roughness);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &U_ROUGHNESS_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.u_roughness.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.u_roughness.as.f32		= 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->roughness_params.u_roughness);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &V_ROUGHNESS_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.v_roughness.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.v_roughness.as.f32		= 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->roughness_params.v_roughness);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
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

					tpbrt_string_t texture_name;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_string(params, &DISPLACEMENT_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &texture_name);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else {
									mat_params->bump_map.type		   = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
									mat_params->bump_map.as.normal_map = texture_name;
								}
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, &mat_params->bump_map.as.displacement);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						}
#pragma endregion

					err = tpbrt_params_list_get_string(params, &REFLECTANCE_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &ETA_STR, &texture_name);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->as.conductor.eta.value_type		  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM;
									mat_params->as.conductor.eta.as.spectrum.type = TPBRT_SPECTRUM_TYPE_BUILTIN;
									mat_params->as.conductor.eta.as.spectrum.as.builtin = TPBRT_SPECTRUM_BUILTIN_METAL_CU_ETA;
								}
								else {
									err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
									  TPBRT_TEXTURE_TYPE_SPECTRUM, &mat_params->as.conductor.eta);
									tpbrt_free_string(&texture_name);
										if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
											tpbrt_free_material(material);
											return err;
										}

										if (err == TPBRT_ERROR_NOT_FOUND) {
											mat_params->as.conductor.eta.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM;
											mat_params->as.conductor.eta.as.spectrum.type = TPBRT_SPECTRUM_TYPE_BUILTIN;
											mat_params->as.conductor.eta.as.spectrum.as.builtin =
											  TPBRT_SPECTRUM_BUILTIN_METAL_CU_ETA;
										}
								}

							err = tpbrt_params_list_get_string(params, &K_STR, &texture_name);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->as.conductor.k.value_type			  = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM;
									mat_params->as.conductor.k.as.spectrum.type		  = TPBRT_SPECTRUM_TYPE_BUILTIN;
									mat_params->as.conductor.k.as.spectrum.as.builtin = TPBRT_SPECTRUM_BUILTIN_METAL_CU_K;
								}
								else {
									err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
									  TPBRT_TEXTURE_TYPE_SPECTRUM, &mat_params->as.conductor.k);
									tpbrt_free_string(&texture_name);
										if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
											tpbrt_free_material(material);
											return err;
										}

										if (err == TPBRT_ERROR_NOT_FOUND) {
											mat_params->as.conductor.k.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM;
											mat_params->as.conductor.k.as.spectrum.type		  = TPBRT_SPECTRUM_TYPE_BUILTIN;
											mat_params->as.conductor.k.as.spectrum.as.builtin = TPBRT_SPECTRUM_BUILTIN_METAL_CU_K;
										}
								}
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_SPECTRUM, &mat_params->as.reflectance);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

#pragma region ROUGHNESS
					err = tpbrt_params_list_get_string(params, &ROUGHNESS_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.roughness.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.roughness.as.f32	  = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->roughness_params.roughness);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &U_ROUGHNESS_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.u_roughness.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.u_roughness.as.f32		= 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->roughness_params.u_roughness);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &V_ROUGHNESS_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.v_roughness.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.v_roughness.as.f32		= 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->roughness_params.v_roughness);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
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

					tpbrt_string_t texture_name;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_string(params, &DISPLACEMENT_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &texture_name);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else {
									mat_params->bump_map.type		   = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
									mat_params->bump_map.as.normal_map = texture_name;
								}
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, &mat_params->bump_map.as.displacement);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						}
#pragma endregion

					err = tpbrt_params_list_get_string(params, &ETA_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					static const tpbrt_float_t ETA_DEFAULT = 1.5f;
						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->eta_type				 = TPBRT_MATERIAL_DIELECTRIC_ETA_TYPE_SINGLE_INDEX;
							mat_params->eta_as.single.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->eta_as.single.as.f32	 = ETA_DEFAULT;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, ETA_DEFAULT, &mat_params->eta_as.single);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_INVALID_TEXTURE_TYPE) {
									tpbrt_free_string(&texture_name);
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_INVALID_TEXTURE_TYPE) {
									err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
									  TPBRT_TEXTURE_TYPE_SPECTRUM, &mat_params->eta_as.wavelength);
										if (err != TPBRT_ERROR_NONE) {
											tpbrt_free_string(&texture_name);
											tpbrt_free_material(material);
											return err;
										}
								}
							tpbrt_free_string(&texture_name);
						}

#pragma region ROUGHNESS
					err = tpbrt_params_list_get_string(params, &ROUGHNESS_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.roughness.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.roughness.as.f32	  = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->roughness_params.roughness);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &U_ROUGHNESS_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.u_roughness.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.u_roughness.as.f32		= 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->roughness_params.u_roughness);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &V_ROUGHNESS_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.v_roughness.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.v_roughness.as.f32		= 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->roughness_params.v_roughness);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
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

					tpbrt_string_t texture_name;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_string(params, &DISPLACEMENT_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &texture_name);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else {
									mat_params->bump_map.type		   = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
									mat_params->bump_map.as.normal_map = texture_name;
								}
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, &mat_params->bump_map.as.displacement);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						}
#pragma endregion

					err = tpbrt_params_list_get_string(params, &REFLECTANCE_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					static const tpbrt_float_t REFLECTANCE_DEFAULT = 0.5f;
						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->reflectance.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->reflectance.as.f32	   = REFLECTANCE_DEFAULT;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_SPECTRUM, REFLECTANCE_DEFAULT, &mat_params->reflectance);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}
					break;
				}
				case TPBRT_MATERIAL_TYPE_DIFFUSE_TRANSMISSION: {
					static const tpbrt_string_t REFLECTANCE_STR				 = TPBRT_STRING("reflectance");
					static const tpbrt_string_t TRANSMITTANCE_STR			 = TPBRT_STRING("transmittance");
					static const tpbrt_string_t SCALE_STR					 = TPBRT_STRING("scale");

					tpbrt_material_diffuse_transmission_params_t* mat_params = &material->as.diffuse_transmission;

					tpbrt_string_t texture_name;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_string(params, &DISPLACEMENT_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &texture_name);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else {
									mat_params->bump_map.type		   = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
									mat_params->bump_map.as.normal_map = texture_name;
								}
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, &mat_params->bump_map.as.displacement);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						}
#pragma endregion

					err = tpbrt_params_list_get_string(params, &REFLECTANCE_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					static const tpbrt_float_t REFLECTANCE_DEFAULT = 0.25f;
						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->reflectance.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->reflectance.as.f32	   = REFLECTANCE_DEFAULT;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_SPECTRUM, REFLECTANCE_DEFAULT, &mat_params->reflectance);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &TRANSMITTANCE_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					static const tpbrt_float_t TRANSMITTANCE_DEFAULT = 0.25f;
						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->transmittance.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->transmittance.as.f32	 = TRANSMITTANCE_DEFAULT;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_SPECTRUM, TRANSMITTANCE_DEFAULT, &mat_params->transmittance);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &SCALE_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->scale.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->scale.as.f32	 = 1.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 1.0f, &mat_params->scale);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
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

					tpbrt_string_t texture_name;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_string(params, &DISPLACEMENT_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &texture_name);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else {
									mat_params->bump_map.type		   = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
									mat_params->bump_map.as.normal_map = texture_name;
								}
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, &mat_params->bump_map.as.displacement);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						}
#pragma endregion

					err = tpbrt_params_list_get_string(params, &SIGMA_A_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->sigma_a.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE;
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_SPECTRUM, &mat_params->sigma_a);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &REFLECTANCE_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->reflectance.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE;
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_SPECTRUM, &mat_params->reflectance);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &EUMELANIN_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					static const tpbrt_float_t EUMELANIN_DEFAULT = 1.3f;
						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->eumelanin.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->eumelanin.as.f32	 = EUMELANIN_DEFAULT;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, EUMELANIN_DEFAULT, &mat_params->eumelanin);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &PHEOMELANIN_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->pheomelanin.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE;
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, &mat_params->pheomelanin);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &ETA_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					static const tpbrt_float_t ETA_DEFAULT = 1.55f;
						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->eta.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->eta.as.f32	   = ETA_DEFAULT;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, ETA_DEFAULT, &mat_params->eta);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &BETA_M_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					static const tpbrt_float_t BETA_M_DEFAULT = 0.3f;
						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->beta_m.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->beta_m.as.f32	  = BETA_M_DEFAULT;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, BETA_M_DEFAULT, &mat_params->beta_m);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &BETA_N_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					static const tpbrt_float_t BETA_N_DEFAULT = 0.3f;
						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->beta_n.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->beta_n.as.f32	  = BETA_N_DEFAULT;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, BETA_N_DEFAULT, &mat_params->beta_n);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &ALPHA_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					static const tpbrt_float_t ALPHA_DEFAULT = 0.3f;
						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->alpha.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->alpha.as.f32	 = ALPHA_DEFAULT;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, ALPHA_DEFAULT, &mat_params->alpha);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}
					break;
				}
			case TPBRT_MATERIAL_TYPE_INTERFACE:	   break;
				case TPBRT_MATERIAL_TYPE_MEASURED: {
					static const tpbrt_string_t FILE_NAME_STR	 = TPBRT_STRING("filename");

					tpbrt_material_measured_params_t* mat_params = &material->as.measured;

					tpbrt_string_t texture_name;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_string(params, &DISPLACEMENT_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &texture_name);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else {
									mat_params->bump_map.type		   = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
									mat_params->bump_map.as.normal_map = texture_name;
								}
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, &mat_params->bump_map.as.displacement);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						}
#pragma endregion

					tpbrt_string_t temp_name;
					err = tpbrt_params_list_get_string(params, &FILE_NAME_STR, &temp_name);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_material(material);
							return err;
						}

					mat_params->file_name = temp_name;
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

					tpbrt_string_t texture_name;
					err = tpbrt_params_list_get_string(params, &AMOUNT_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					static const tpbrt_float_t AMOUNT_DEFAULT = 0.5f;
						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->amount.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->amount.as.f32	  = AMOUNT_DEFAULT;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, AMOUNT_DEFAULT, &mat_params->amount);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
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

					tpbrt_string_t texture_name;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_string(params, &DISPLACEMENT_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &texture_name);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else {
									mat_params->bump_map.type		   = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
									mat_params->bump_map.as.normal_map = texture_name;
								}
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, &mat_params->bump_map.as.displacement);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
						}
#pragma endregion

					err = tpbrt_params_list_get_string(params, &ETA_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					static const tpbrt_float_t ETA_DEFAULT = 1.33f;
						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->eta.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->eta.as.f32	   = ETA_DEFAULT;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, ETA_DEFAULT, &mat_params->eta);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &G_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->g.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->g.as.f32	 = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->g);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &MFP_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) { mat_params->mfp.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE; }
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, &mat_params->mfp);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &NAME_STR, &mat_params->name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					err = tpbrt_params_list_get_string(params, &REFLECTANCE_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->reflectance.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_NONE;
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_SPECTRUM, &mat_params->reflectance);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &SIGMA_A_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					static const tpbrt_rgb_t SIGMA_A_DEFAULT = (tpbrt_rgb_t) { .r = 0.0011f, .g = 0.0024f, .b = 0.014f };
						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->sigma_a.value_type		   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM;
							mat_params->sigma_a.as.spectrum.type   = TPBRT_SPECTRUM_TYPE_RGB;
							mat_params->sigma_a.as.spectrum.as.rgb = SIGMA_A_DEFAULT;
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_SPECTRUM, &mat_params->sigma_a);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &SIGMA_S_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

					static const tpbrt_rgb_t SIGMA_S_DEFAULT = (tpbrt_rgb_t) { .r = 2.55f, .g = 3.12f, .b = 3.77f };
						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->sigma_s.value_type		   = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_SPECTRUM;
							mat_params->sigma_s.as.spectrum.type   = TPBRT_SPECTRUM_TYPE_RGB;
							mat_params->sigma_s.as.spectrum.as.rgb = SIGMA_S_DEFAULT;
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_SPECTRUM, &mat_params->sigma_s);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_float(params, &SCALE_STR, 1.0f, &mat_params->scale);
						if (err != TPBRT_ERROR_NONE) {
							tpbrt_free_material(material);
							return err;
						}

#pragma region ROUGHNESS
					err = tpbrt_params_list_get_string(params, &ROUGHNESS_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.roughness.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.roughness.as.f32	  = 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->roughness_params.roughness);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &U_ROUGHNESS_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.u_roughness.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.u_roughness.as.f32		= 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->roughness_params.u_roughness);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
						}

					err = tpbrt_params_list_get_string(params, &V_ROUGHNESS_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							mat_params->roughness_params.v_roughness.value_type = TPBRT_TEXTURE_HANDLE_VALUE_TYPE_FLOAT;
							mat_params->roughness_params.v_roughness.as.f32		= 0.0f;
						}
						else {
							err = tpbrt_textures_list_get_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, 0.0f, &mat_params->roughness_params.v_roughness);
							tpbrt_free_string(&texture_name);
								if (err != TPBRT_ERROR_NONE) {
									tpbrt_free_material(material);
									return err;
								}
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

					tpbrt_string_t texture_name;

#pragma region BUMP_MAP
					err = tpbrt_params_list_get_string(params, &DISPLACEMENT_STR, &texture_name);
						if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
							tpbrt_free_material(material);
							return err;
						}

						if (err == TPBRT_ERROR_NOT_FOUND) {
							err = tpbrt_params_list_get_string(params, &NORMAL_MAP_STR, &texture_name);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else {
									mat_params->bump_map.type		   = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
									mat_params->bump_map.as.normal_map = texture_name;
								}
						}
						else {
							err = tpbrt_textures_list_get_opt_texture_handle_of_type(textures, &texture_name,
							  TPBRT_TEXTURE_TYPE_FLOAT, &mat_params->bump_map.as.displacement);
								if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) {
									tpbrt_free_material(material);
									return err;
								}

								if (err == TPBRT_ERROR_NOT_FOUND) {
									mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
								}
								else { mat_params->bump_map.type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT; }
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
				}
				case TPBRT_MATERIAL_TYPE_COATED_CONDUCTOR: {
					tpbrt_material_coated_conductor_params_t* params = &material->as.coated_conductor;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}
				}
				case TPBRT_MATERIAL_TYPE_CONDUCTOR: {
					tpbrt_material_conductor_params_t* params = &material->as.conductor;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}
				}
				case TPBRT_MATERIAL_TYPE_DIELECTRIC: {
					tpbrt_material_dielectric_params_t* params = &material->as.dielectric;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}
				}
				case TPBRT_MATERIAL_TYPE_DIFFUSE: {
					tpbrt_material_diffuse_params_t* params = &material->as.diffuse;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}
				}
				case TPBRT_MATERIAL_TYPE_DIFFUSE_TRANSMISSION: {
					tpbrt_material_diffuse_transmission_params_t* params = &material->as.diffuse_transmission;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}
				}
				case TPBRT_MATERIAL_TYPE_HAIR: {
					tpbrt_material_hair_params_t* params = &material->as.hair;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}
				}
				case TPBRT_MATERIAL_TYPE_MEASURED: {
					tpbrt_material_measured_params_t* params = &material->as.measured;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}

					tpbrt_free_string(&params->file_name);
				}
				case TPBRT_MATERIAL_TYPE_SUBSURFACE: {
					tpbrt_material_subsurface_params_t* params = &material->as.subsurface;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}

					tpbrt_free_string(&params->name);
				}
				case TPBRT_MATERIAL_TYPE_THIN_DIELECTRIC: {
					tpbrt_material_thin_dielectric_params_t* params = &material->as.thin_dielectric;
						if (params->bump_map.type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) {
							tpbrt_free_string(&params->bump_map.as.normal_map);
						}
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