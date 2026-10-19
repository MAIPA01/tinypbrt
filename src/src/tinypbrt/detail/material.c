#include <tinypbrt/pch.h>

#include <tinypbrt/detail/material_internal.h>

#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/param_internal.h>
#include <tinypbrt/detail/texture_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region MATERIAL

	tpbrt_error_t tpbrt_material_type_from_string(const tpbrt_string_t type_str, tpbrt_material_type_t* const type) {
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

			if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(type_str)) { return TPBRT_ERROR_INVALID_MATERIAL_TYPE; }

			for (tpbrt_material_type_t t = 0; t < TPBRT_MATERIAL_TYPE_MAX_NUM; ++t) {
					if (tpbrt_string_equals(type_str, TYPES_STRS[t])) {
						*type = t;
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_INVALID_MATERIAL_TYPE;
	}

	static tpbrt_error_t tpbrt_material_read_bump_map(tpbrt_material_bump_map_params_t* const bump_map,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures) {
			if (bump_map == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t DISPLACEMENT_STR = TPBRT_STRING("displacement");
		static const tpbrt_string_t NORMAL_MAP_STR	 = TPBRT_STRING("normalmap");

		tpbrt_error_t err = tpbrt_params_list_get_texture_of_type(params, DISPLACEMENT_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
		  &bump_map->as.displacement);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NONE) {
				bump_map->type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT;
				return TPBRT_ERROR_NONE;
			}

		err = tpbrt_params_list_get_string(params, NORMAL_MAP_STR, &bump_map->as.normal_map);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NONE) {
				bump_map->type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
				return TPBRT_ERROR_NONE;
			}

		bump_map->type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_material_read_roughness_named(tpbrt_material_roughness_params_t* const roughness_params,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures, const tpbrt_string_t roughness_str,
	  const tpbrt_string_t u_str, const tpbrt_string_t v_str, const tpbrt_string_t remap_str) {
			if (roughness_params == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		tpbrt_error_t err = tpbrt_params_list_get_texture_of_type(params, roughness_str, textures, TPBRT_TEXTURE_TYPE_FLOAT,
		  &roughness_params->roughness);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { roughness_params->roughness = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(0.0f); }

		err = tpbrt_params_list_get_texture_of_type(params, u_str, textures, TPBRT_TEXTURE_TYPE_FLOAT,
		  &roughness_params->u_roughness);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { roughness_params->u_roughness = roughness_params->roughness; }

		err = tpbrt_params_list_get_texture_of_type(params, v_str, textures, TPBRT_TEXTURE_TYPE_FLOAT,
		  &roughness_params->v_roughness);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { roughness_params->v_roughness = roughness_params->roughness; }

		tpbrt_bool_t remap_roughness;
		err = tpbrt_params_list_get_bool(params, remap_str, TPBRT_TRUE, &remap_roughness);
			if (err != TPBRT_ERROR_NONE) { return err; }
		roughness_params->remap_roughness = remap_roughness;

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_material_read_roughness(tpbrt_material_roughness_params_t* const roughness_params,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures) {
			if (roughness_params == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t ROUGHNESS_STR		= TPBRT_STRING("roughness");
		static const tpbrt_string_t U_ROUGHNESS_STR		= TPBRT_STRING("uroughness");
		static const tpbrt_string_t V_ROUGHNESS_STR		= TPBRT_STRING("vroughness");
		static const tpbrt_string_t REMAP_ROUGHNESS_STR = TPBRT_STRING("remaproughness");

		return tpbrt_material_read_roughness_named(roughness_params, params, textures, ROUGHNESS_STR, U_ROUGHNESS_STR,
		  V_ROUGHNESS_STR, REMAP_ROUGHNESS_STR);
	}

	static tpbrt_error_t tpbrt_material_read_coated_diffuse(tpbrt_material_coated_diffuse_params_t* const coated_diffuse,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures) {
			if (coated_diffuse == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t ALBEDO_STR		= TPBRT_STRING("albedo");
		static const tpbrt_string_t G_STR			= TPBRT_STRING("g");
		static const tpbrt_string_t ETA_STR			= TPBRT_STRING("eta");
		static const tpbrt_string_t MAX_DEPTH_STR	= TPBRT_STRING("maxdepth");
		static const tpbrt_string_t N_SAMPLES_STR	= TPBRT_STRING("nsamples");
		static const tpbrt_string_t THICKNESS_STR	= TPBRT_STRING("thickness");
		static const tpbrt_string_t REFLECTANCE_STR = TPBRT_STRING("reflectance");

		tpbrt_error_t err							= tpbrt_material_read_bump_map(&coated_diffuse->bump_map, params, textures);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture_of_type(params, ALBEDO_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
		  &coated_diffuse->albedo);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { coated_diffuse->albedo = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(0.0f); }

		err = tpbrt_params_list_get_texture_of_type(params, G_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &coated_diffuse->g);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { coated_diffuse->g = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(0.0f); }

		err = tpbrt_params_list_get_texture(params, ETA_STR, textures, &coated_diffuse->eta);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { coated_diffuse->eta = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(1.5f); }

		static const tpbrt_uint_t MAX_DEPTH_DEFAULT = 10u;
		err = tpbrt_params_list_get_uint(params, MAX_DEPTH_STR, MAX_DEPTH_DEFAULT, &coated_diffuse->max_depth);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_uint(params, N_SAMPLES_STR, 1, &coated_diffuse->n_samples);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, THICKNESS_STR, 0.01f, &coated_diffuse->thickness);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture(params, REFLECTANCE_STR, textures, &coated_diffuse->reflectance);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { coated_diffuse->reflectance = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(0.5f); }

		return tpbrt_material_read_roughness(&coated_diffuse->roughness_params, params, textures);
	}

	static tpbrt_error_t tpbrt_material_read_coated_conductor(tpbrt_material_coated_conductor_params_t* const coated_conductor,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures) {
			if (coated_conductor == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t ALBEDO_STR					  = TPBRT_STRING("albedo");
		static const tpbrt_string_t G_STR						  = TPBRT_STRING("g");
		static const tpbrt_string_t MAX_DEPTH_STR				  = TPBRT_STRING("maxdepth");
		static const tpbrt_string_t N_SAMPLES_STR				  = TPBRT_STRING("nsamples");
		static const tpbrt_string_t THICKNESS_STR				  = TPBRT_STRING("thickness");
		static const tpbrt_string_t CONDUCTOR_ETA_STR			  = TPBRT_STRING("conductor.eta");
		static const tpbrt_string_t CONDUCTOR_K_STR				  = TPBRT_STRING("conductor.k");
		static const tpbrt_string_t REFLECTANCE_STR				  = TPBRT_STRING("reflectance");

		static const tpbrt_string_t CONDUCTOR_ROUGHNESS_STR		  = TPBRT_STRING("conductor.roughness");
		static const tpbrt_string_t CONDUCTOR_U_ROUGHNESS_STR	  = TPBRT_STRING("conductor.uroughness");
		static const tpbrt_string_t CONDUCTOR_V_ROUGHNESS_STR	  = TPBRT_STRING("conductor.vroughness");
		static const tpbrt_string_t CONDUCTOR_REMAP_ROUGHNESS_STR = TPBRT_STRING("conductor.remaproughness");

		static const tpbrt_string_t INTERFACE_ROUGHNESS_STR		  = TPBRT_STRING("interface.roughness");
		static const tpbrt_string_t INTERFACE_U_ROUGHNESS_STR	  = TPBRT_STRING("interface.uroughness");
		static const tpbrt_string_t INTERFACE_V_ROUGHNESS_STR	  = TPBRT_STRING("interface.vroughness");
		static const tpbrt_string_t INTERFACE_REMAP_ROUGHNESS_STR = TPBRT_STRING("interface.remaproughness");

		tpbrt_error_t err = tpbrt_material_read_bump_map(&coated_conductor->bump_map, params, textures);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture_of_type(params, ALBEDO_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
		  &coated_conductor->albedo);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { coated_conductor->albedo = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(0.0f); }

		err = tpbrt_params_list_get_texture_of_type(params, G_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &coated_conductor->g);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { coated_conductor->g = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(0.0f); }

		err = tpbrt_params_list_get_uint(params, MAX_DEPTH_STR, 10u, &coated_conductor->max_depth);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_uint(params, N_SAMPLES_STR, 1, &coated_conductor->n_samples);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_float(params, THICKNESS_STR, 0.01f, &coated_conductor->thickness);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_spectrum(params, REFLECTANCE_STR, &coated_conductor->as.reflectance);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) {
				err = tpbrt_params_list_get_spectrum(params, CONDUCTOR_ETA_STR, &coated_conductor->as.conductor.eta);
					if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

					if (err == TPBRT_ERROR_NOT_FOUND) {
						coated_conductor->as.conductor.eta = TPBRT_SPECTRUM_BUILTIN(TPBRT_SPECTRUM_BUILTIN_METAL_CU_ETA);
					}

				err = tpbrt_params_list_get_spectrum(params, CONDUCTOR_K_STR, &coated_conductor->as.conductor.k);
					if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

					if (err == TPBRT_ERROR_NOT_FOUND) {
						coated_conductor->as.conductor.k = TPBRT_SPECTRUM_BUILTIN(TPBRT_SPECTRUM_BUILTIN_METAL_CU_K);
					}
			}

		err = tpbrt_material_read_roughness_named(&coated_conductor->conductor_roughness_params, params, textures,
		  CONDUCTOR_ROUGHNESS_STR, CONDUCTOR_U_ROUGHNESS_STR, CONDUCTOR_V_ROUGHNESS_STR, CONDUCTOR_REMAP_ROUGHNESS_STR);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_material_read_roughness_named(&coated_conductor->interface_roughness_params, params, textures,
		  INTERFACE_ROUGHNESS_STR, INTERFACE_U_ROUGHNESS_STR, INTERFACE_V_ROUGHNESS_STR, INTERFACE_REMAP_ROUGHNESS_STR);
	}

	static tpbrt_error_t tpbrt_material_read_conductor(tpbrt_material_conductor_params_t* const conductor,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures) {
			if (conductor == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t ETA_STR			= TPBRT_STRING("eta");
		static const tpbrt_string_t K_STR			= TPBRT_STRING("k");
		static const tpbrt_string_t REFLECTANCE_STR = TPBRT_STRING("reflectance");

		tpbrt_error_t err							= tpbrt_material_read_bump_map(&conductor->bump_map, params, textures);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture_of_type(params, REFLECTANCE_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
		  &conductor->as.reflectance);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NONE) { conductor->value_type = TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_REFLECTANCE; }
			else if (err == TPBRT_ERROR_NOT_FOUND) {
				conductor->value_type = TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_ETA_K;

				err = tpbrt_params_list_get_texture_of_type(params, ETA_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
				  &conductor->as.conductor.eta);
					if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

					if (err == TPBRT_ERROR_NOT_FOUND) {
						conductor->as.conductor.eta = TPBRT_TEXTURE_HANDLE_SPECTRUM_BUILTIN(TPBRT_SPECTRUM_BUILTIN_METAL_CU_ETA);
					}

				err = tpbrt_params_list_get_texture_of_type(params, K_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
				  &conductor->as.conductor.k);
					if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

					if (err == TPBRT_ERROR_NOT_FOUND) {
						conductor->as.conductor.k = TPBRT_TEXTURE_HANDLE_SPECTRUM_BUILTIN(TPBRT_SPECTRUM_BUILTIN_METAL_CU_K);
					}
			}

		return tpbrt_material_read_roughness(&conductor->roughness_params, params, textures);
	}

	static tpbrt_error_t tpbrt_material_read_dielectric(tpbrt_material_dielectric_params_t* const dielectric,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures) {
			if (dielectric == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t ETA_STR = TPBRT_STRING("eta");

		tpbrt_error_t err					= tpbrt_material_read_bump_map(&dielectric->bump_map, params, textures);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture(params, ETA_STR, textures, &dielectric->eta);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { dielectric->eta = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(1.5f); }

		return tpbrt_material_read_roughness(&dielectric->roughness_params, params, textures);
	}

	static tpbrt_error_t tpbrt_material_read_diffuse(tpbrt_material_diffuse_params_t* const diffuse,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures) {
			if (diffuse == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t REFLECTANCE_STR = TPBRT_STRING("reflectance");

		tpbrt_error_t err							= tpbrt_material_read_bump_map(&diffuse->bump_map, params, textures);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture_of_type(params, REFLECTANCE_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
		  &diffuse->reflectance);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { diffuse->reflectance = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(0.5f); }

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_material_read_diffuse_transmission(
	  tpbrt_material_diffuse_transmission_params_t* const diffuse_transmission, const tpbrt_params_list_t params,
	  const tpbrt_textures_list_t textures
	) {
			if (diffuse_transmission == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t REFLECTANCE_STR	  = TPBRT_STRING("reflectance");
		static const tpbrt_string_t TRANSMITTANCE_STR = TPBRT_STRING("transmittance");
		static const tpbrt_string_t SCALE_STR		  = TPBRT_STRING("scale");

		tpbrt_error_t err = tpbrt_material_read_bump_map(&diffuse_transmission->bump_map, params, textures);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture_of_type(params, REFLECTANCE_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
		  &diffuse_transmission->reflectance);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) {
				diffuse_transmission->reflectance = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(0.25f);
			}

		err = tpbrt_params_list_get_texture_of_type(params, TRANSMITTANCE_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
		  &diffuse_transmission->transmittance);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) {
				diffuse_transmission->transmittance = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(0.25f);
			}

		err = tpbrt_params_list_get_texture_of_type(params, SCALE_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT,
		  &diffuse_transmission->scale);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { diffuse_transmission->scale = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(1.0f); }

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_material_read_hair(tpbrt_material_hair_params_t* const hair, const tpbrt_params_list_t params,
	  const tpbrt_textures_list_t textures) {
			if (hair == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t SIGMA_A_STR		= TPBRT_STRING("sigma_a");
		static const tpbrt_string_t REFLECTANCE_STR = TPBRT_STRING("reflectance");
		static const tpbrt_string_t EUMELANIN_STR	= TPBRT_STRING("eumelanin");
		static const tpbrt_string_t PHEOMELANIN_STR = TPBRT_STRING("pheomelanin");
		static const tpbrt_string_t ETA_STR			= TPBRT_STRING("eta");
		static const tpbrt_string_t BETA_M_STR		= TPBRT_STRING("beta_m");
		static const tpbrt_string_t BETA_N_STR		= TPBRT_STRING("beta_n");
		static const tpbrt_string_t ALPHA_STR		= TPBRT_STRING("alpha");

		tpbrt_error_t err							= tpbrt_material_read_bump_map(&hair->bump_map, params, textures);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture_of_type(params, SIGMA_A_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM, &hair->sigma_a);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_texture_of_type(params, REFLECTANCE_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
		  &hair->reflectance);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_texture_of_type(params, EUMELANIN_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &hair->eumelanin);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { hair->eumelanin = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(1.3f); }

		err =
		  tpbrt_params_list_get_texture_of_type(params, PHEOMELANIN_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &hair->pheomelanin);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_texture_of_type(params, ETA_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &hair->eta);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { hair->eta = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(1.55f); }

		err = tpbrt_params_list_get_texture_of_type(params, BETA_M_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &hair->beta_m);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { hair->beta_m = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(0.3f); }

		err = tpbrt_params_list_get_texture_of_type(params, BETA_N_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &hair->beta_n);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { hair->beta_n = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(0.3f); }

		err = tpbrt_params_list_get_texture_of_type(params, ALPHA_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &hair->alpha);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { hair->alpha = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(2.0f); }

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_material_read_measured(tpbrt_material_measured_params_t* const measured,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures) {
			if (measured == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t FILE_NAME_STR = TPBRT_STRING("filename");

		const tpbrt_error_t err					  = tpbrt_material_read_bump_map(&measured->bump_map, params, textures);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_params_list_get_string(params, FILE_NAME_STR, &measured->file_name);
	}

	static tpbrt_error_t tpbrt_material_read_mix(tpbrt_material_mix_params_t* const mix, const tpbrt_params_list_t params,
	  const tpbrt_textures_list_t textures, const tpbrt_materials_list_t materials) {
			if (mix == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t MATERIALS_STR = TPBRT_STRING("materials");
		static const tpbrt_string_t AMOUNT_STR	  = TPBRT_STRING("amount");

		tpbrt_string_array_t temp_strings;
		tpbrt_error_t err = tpbrt_params_list_get_strings_of_size(params, MATERIALS_STR, 2u, &temp_strings);
			if (err != TPBRT_ERROR_NONE) { return err; }

			for (tpbrt_size_t i = 0; i < temp_strings.count; ++i) {
				err = tpbrt_materials_list_get_handle(materials, temp_strings.data[i], &mix->materials[i]);
					if (err != TPBRT_ERROR_NONE) {
						tpbrt_string_array_free(&temp_strings);
						return err;
					}
			}
		tpbrt_string_array_free(&temp_strings);

		err = tpbrt_params_list_get_texture_of_type(params, AMOUNT_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &mix->amount);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { mix->amount = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(0.5f); }

		return TPBRT_ERROR_NONE;
	}

	static tpbrt_error_t tpbrt_material_read_subsurface(tpbrt_material_subsurface_params_t* const subsurface,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures) {
			if (subsurface == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t ETA_STR			= TPBRT_STRING("eta");
		static const tpbrt_string_t G_STR			= TPBRT_STRING("g");
		static const tpbrt_string_t MFP_STR			= TPBRT_STRING("mfp");
		static const tpbrt_string_t NAME_STR		= TPBRT_STRING("name");
		static const tpbrt_string_t REFLECTANCE_STR = TPBRT_STRING("reflectance");
		static const tpbrt_string_t SIGMA_A_STR		= TPBRT_STRING("sigma_a");
		static const tpbrt_string_t SIGMA_S_STR		= TPBRT_STRING("sigma_s");
		static const tpbrt_string_t SCALE_STR		= TPBRT_STRING("scale");

		tpbrt_error_t err							= tpbrt_material_read_bump_map(&subsurface->bump_map, params, textures);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture_of_type(params, ETA_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &subsurface->eta);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { subsurface->eta = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(1.33f); }

		err = tpbrt_params_list_get_texture_of_type(params, G_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &subsurface->g);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { subsurface->g = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(0.0f); }

		err = tpbrt_params_list_get_texture_of_type(params, MFP_STR, textures, TPBRT_TEXTURE_TYPE_FLOAT, &subsurface->mfp);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_string(params, NAME_STR, &subsurface->name);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err = tpbrt_params_list_get_texture_of_type(params, REFLECTANCE_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
		  &subsurface->reflectance);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		err =
		  tpbrt_params_list_get_texture_of_type(params, SIGMA_A_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM, &subsurface->sigma_a);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) {
				subsurface->sigma_a = TPBRT_TEXTURE_HANDLE_SPECTRUM_RGB(0.0011f, 0.0024f, 0.014f);
			}

		err =
		  tpbrt_params_list_get_texture_of_type(params, SIGMA_S_STR, textures, TPBRT_TEXTURE_TYPE_SPECTRUM, &subsurface->sigma_s);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { subsurface->sigma_s = TPBRT_TEXTURE_HANDLE_SPECTRUM_RGB(2.55f, 3.12f, 3.77f); }

		err = tpbrt_params_list_get_float(params, SCALE_STR, 1.0f, &subsurface->scale);
			if (err != TPBRT_ERROR_NONE) { return err; }

		return tpbrt_material_read_roughness(&subsurface->roughness_params, params, textures);
	}

	static tpbrt_error_t tpbrt_material_read_thin_dielectric(tpbrt_material_thin_dielectric_params_t* const thin_dielectric,
	  const tpbrt_params_list_t params, const tpbrt_textures_list_t textures) {
			if (thin_dielectric == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t ETA_STR = TPBRT_STRING("eta");

		tpbrt_error_t err					= tpbrt_material_read_bump_map(&thin_dielectric->bump_map, params, textures);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_params_list_get_texture(params, ETA_STR, textures, &thin_dielectric->eta);
			if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

			if (err == TPBRT_ERROR_NOT_FOUND) { thin_dielectric->eta = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT(1.5f); }

		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_material_init(const tpbrt_string_t type_str, const tpbrt_params_list_t params,
	  const tpbrt_textures_list_t textures, const tpbrt_materials_list_t materials, tpbrt_material_t* const material) {
			if (material == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (memset(material, 0, sizeof(tpbrt_material_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		material->idx = TPBRT_MATERIAL_IDX_INVALID;

		tpbrt_material_type_t type;
		tpbrt_error_t err = tpbrt_material_type_from_string(type_str, &type);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_material_free(material);
				return err;
			}
		material->type = type;

			switch (material->type) {
			default:
				case TPBRT_MATERIAL_TYPE_COATED_DIFFUSE: {
					err = tpbrt_material_read_coated_diffuse(&material->as.coated_diffuse, params, textures);
					break;
				}
				case TPBRT_MATERIAL_TYPE_COATED_CONDUCTOR: {
					err = tpbrt_material_read_coated_conductor(&material->as.coated_conductor, params, textures);
					break;
				}
				case TPBRT_MATERIAL_TYPE_CONDUCTOR: {
					err = tpbrt_material_read_conductor(&material->as.conductor, params, textures);
					break;
				}
				case TPBRT_MATERIAL_TYPE_DIELECTRIC: {
					err = tpbrt_material_read_dielectric(&material->as.dielectric, params, textures);
					break;
				}
				case TPBRT_MATERIAL_TYPE_DIFFUSE: {
					err = tpbrt_material_read_diffuse(&material->as.diffuse, params, textures);
					break;
				}
				case TPBRT_MATERIAL_TYPE_DIFFUSE_TRANSMISSION: {
					err = tpbrt_material_read_diffuse_transmission(&material->as.diffuse_transmission, params, textures);
					break;
				}
				case TPBRT_MATERIAL_TYPE_HAIR: {
					err = tpbrt_material_read_hair(&material->as.hair, params, textures);
					break;
				}
				case TPBRT_MATERIAL_TYPE_INTERFACE: {
					err = TPBRT_ERROR_NONE;
					break;
				}
				case TPBRT_MATERIAL_TYPE_MEASURED: {
					err = tpbrt_material_read_measured(&material->as.measured, params, textures);
					break;
				}
				case TPBRT_MATERIAL_TYPE_MIX: {
					err = tpbrt_material_read_mix(&material->as.mix, params, textures, materials);
					break;
				}
				case TPBRT_MATERIAL_TYPE_SUBSURFACE: {
					err = tpbrt_material_read_subsurface(&material->as.subsurface, params, textures);
					break;
				}
				case TPBRT_MATERIAL_TYPE_THIN_DIELECTRIC: {
					err = tpbrt_material_read_thin_dielectric(&material->as.thin_dielectric, params, textures);
					break;
				}
			}

			if (err != TPBRT_ERROR_NONE) {
				tpbrt_material_free(material);
				return err;
			}

		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_material_init_named(const tpbrt_string_t name, const tpbrt_params_list_t params,
	  const tpbrt_textures_list_t textures, const tpbrt_materials_list_t materials, tpbrt_material_t* const material) {
			if (material == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (tpbrt_string_empty(name)) { return TPBRT_ERROR_INVALID_STRING; }

			if (memset(material, 0, sizeof(tpbrt_material_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		static const tpbrt_string_t TYPE_STR = TPBRT_STRING("type");

		tpbrt_string_t type_str;
		tpbrt_error_t err = tpbrt_params_list_get_string(params, TYPE_STR, &type_str);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_material_init(type_str, params, textures, materials, material);
		tpbrt_string_free(&type_str);
			if (err != TPBRT_ERROR_NONE) { return err; }

		err = tpbrt_string_copy(&material->name, name);
			if (err != TPBRT_ERROR_NONE) {
				tpbrt_material_free(material);
				return err;
			}

		return TPBRT_ERROR_NONE;
	}

	static void tpbrt_material_free_bump_map(tpbrt_material_bump_map_params_t* const bump_map) {
			if (bump_map->type == TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP) { tpbrt_string_free(&bump_map->as.normal_map); }
	}

	void tpbrt_material_free(tpbrt_material_t* const material) {
			if (material == TPBRT_NULL) { return; }

		tpbrt_string_free(&material->name);

			switch (material->type) {
				case TPBRT_MATERIAL_TYPE_COATED_DIFFUSE: {
					tpbrt_material_coated_diffuse_params_t* const params = &material->as.coated_diffuse;
					tpbrt_material_free_bump_map(&params->bump_map);
					tpbrt_texture_handle_free(&params->reflectance);
					tpbrt_texture_handle_free(&params->albedo);
					tpbrt_texture_handle_free(&params->eta);
					break;
				}
				case TPBRT_MATERIAL_TYPE_COATED_CONDUCTOR: {
					tpbrt_material_coated_conductor_params_t* const params = &material->as.coated_conductor;
					tpbrt_material_free_bump_map(&params->bump_map);
					tpbrt_texture_handle_free(&params->albedo);

						switch (params->value_type) {
							default: {
								break;
							}
							case TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_REFLECTANCE: {
								tpbrt_spectrum_free(&params->as.reflectance);
								break;
							}
							case TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_ETA_K: {
								tpbrt_spectrum_free(&params->as.conductor.eta);
								tpbrt_spectrum_free(&params->as.conductor.k);
							}
						}
					break;
				}
				case TPBRT_MATERIAL_TYPE_CONDUCTOR: {
					tpbrt_material_conductor_params_t* const params = &material->as.conductor;
					tpbrt_material_free_bump_map(&params->bump_map);
						if (params->value_type == TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_ETA_K) {
							tpbrt_texture_handle_free(&params->as.conductor.eta);
							tpbrt_texture_handle_free(&params->as.conductor.k);
						}
						else { tpbrt_texture_handle_free(&params->as.reflectance); }
					break;
				}
				case TPBRT_MATERIAL_TYPE_DIELECTRIC: {
					tpbrt_material_dielectric_params_t* const params = &material->as.dielectric;
					tpbrt_material_free_bump_map(&params->bump_map);
					tpbrt_texture_handle_free(&params->eta);
					break;
				}
				case TPBRT_MATERIAL_TYPE_DIFFUSE: {
					tpbrt_material_diffuse_params_t* const params = &material->as.diffuse;
					tpbrt_material_free_bump_map(&params->bump_map);
					tpbrt_texture_handle_free(&params->reflectance);
					break;
				}
				case TPBRT_MATERIAL_TYPE_DIFFUSE_TRANSMISSION: {
					tpbrt_material_diffuse_transmission_params_t* const params = &material->as.diffuse_transmission;
					tpbrt_material_free_bump_map(&params->bump_map);
					tpbrt_texture_handle_free(&params->reflectance);
					tpbrt_texture_handle_free(&params->transmittance);
					break;
				}
				case TPBRT_MATERIAL_TYPE_HAIR: {
					tpbrt_material_hair_params_t* const params = &material->as.hair;
					tpbrt_material_free_bump_map(&params->bump_map);
					tpbrt_texture_handle_free(&params->sigma_a);
					tpbrt_texture_handle_free(&params->reflectance);
					tpbrt_texture_handle_free(&params->eta);
					break;
				}
				case TPBRT_MATERIAL_TYPE_MEASURED: {
					tpbrt_material_measured_params_t* const params = &material->as.measured;
					tpbrt_material_free_bump_map(&params->bump_map);
					tpbrt_string_free(&params->file_name);
					break;
				}
				case TPBRT_MATERIAL_TYPE_SUBSURFACE: {
					tpbrt_material_subsurface_params_t* const params = &material->as.subsurface;
					tpbrt_material_free_bump_map(&params->bump_map);
					tpbrt_string_free(&params->name);
					tpbrt_texture_handle_free(&params->mfp);
					tpbrt_texture_handle_free(&params->reflectance);
					tpbrt_texture_handle_free(&params->sigma_a);
					tpbrt_texture_handle_free(&params->sigma_s);
					tpbrt_texture_handle_free(&params->eta);
					break;
				}
				case TPBRT_MATERIAL_TYPE_THIN_DIELECTRIC: {
					tpbrt_material_thin_dielectric_params_t* const params = &material->as.thin_dielectric;
					tpbrt_material_free_bump_map(&params->bump_map);
					tpbrt_texture_handle_free(&params->eta);
					break;
				}
				default: {
					break;
				}
			}
	}

#pragma endregion

#pragma region MATERIALS_LIST
#define TPBRT_EMPTY_MATERIALS_LIST (tpbrt_materials_list_t) { .materials = TPBRT_NULL, .count = 0 }

	tpbrt_error_t tpbrt_materials_list_init(tpbrt_materials_list_t* const materials_list) {
			if (materials_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		*materials_list = TPBRT_EMPTY_MATERIALS_LIST;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_materials_list_append(tpbrt_materials_list_t* const materials_list,
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
		handle->material_idx								 = materials_list->count;
		materials_list->count								 = new_count;
		return TPBRT_ERROR_NONE;
	}

	tpbrt_error_t tpbrt_materials_list_get(const tpbrt_materials_list_t materials_list, const tpbrt_string_t material_name,
	  const tpbrt_material_t** const material) {
			if (material == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			for (tpbrt_size_t i = 0; i < materials_list.count; i++) {
					if (tpbrt_string_equals(materials_list.materials[i].name, material_name)) {
						*material = &materials_list.materials[i];
						return TPBRT_ERROR_NONE;
					}
			}

		return TPBRT_ERROR_NOT_FOUND;
	}

	tpbrt_error_t tpbrt_materials_list_get_handle(const tpbrt_materials_list_t materials_list, const tpbrt_string_t material_name,
	  tpbrt_material_handle_t* const material_handle) {
			if (material_handle == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		const tpbrt_material_t* material;
		const tpbrt_error_t err = tpbrt_materials_list_get(materials_list, material_name, &material);

			if (err != TPBRT_ERROR_NONE) { return err; }

		material_handle->material_idx = material->idx;
		return TPBRT_ERROR_NONE;
	}

	void tpbrt_materials_list_free(tpbrt_materials_list_t* const materials_list) {
			if (materials_list == TPBRT_NULL || materials_list->materials == TPBRT_NULL) { return; }

			for (tpbrt_size_t i = 0; i < materials_list->count; ++i) { tpbrt_material_free(materials_list->materials + i); }

		free(materials_list->materials);
		*materials_list = TPBRT_EMPTY_MATERIALS_LIST;
	}

	tpbrt_bool_t tpbrt_materials_list_empty(const tpbrt_materials_list_t materials_list) {
		return materials_list.materials == TPBRT_NULL || materials_list.count == 0;
	}

#pragma endregion

#pragma region API

	tpbrt_error_t tpbrt_get_material_by_name(const tpbrt_materials_list_t materials, const tpbrt_string_t name,
	  const tpbrt_material_t** const material) {
		return tpbrt_materials_list_get(materials, name, material);
	}

	tpbrt_error_t tpbrt_get_material_by_handle(const tpbrt_materials_list_t materials, const tpbrt_material_handle_t handle,
	  const tpbrt_material_t** const material) {
			if (material == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

			if (handle.material_idx == TPBRT_MATERIAL_IDX_INVALID || handle.material_idx >= materials.count) {
				*material = TPBRT_NULL;
				return TPBRT_ERROR_INVALID_HANDLE;
			}

		*material = materials.materials + handle.material_idx;
		return TPBRT_ERROR_NONE;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif