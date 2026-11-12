#include <tinypbrt/pch.h>

#include <tinypbrt/detail/material_internal.h>
#include <tinypbrt/detail/name_index_internal.h>

#include <tinypbrt/detail/color_internal.h>
#include <tinypbrt/detail/common_internal.h>
#include <tinypbrt/detail/param_internal.h>
#include <tinypbrt/detail/scattering_internal.h>
#include <tinypbrt/detail/texture_internal.h>

#pragma region MATERIAL

tpbrt_error_t tpbrt_material_type_from_string(const tpbrt_string_t type_str, tpbrt_material_type_t* const type) {
	static const tpbrt_string_t TypesStrs[TPBRT_MATERIAL_TYPE_MAX_NUM] = {
		TPBRT_STRING_INIT("coateddiffuse"),
		TPBRT_STRING_INIT("coatedconductor"),
		TPBRT_STRING_INIT("conductor"),
		TPBRT_STRING_INIT("dielectric"),
		TPBRT_STRING_INIT("diffuse"),
		TPBRT_STRING_INIT("diffusetransmission"),
		TPBRT_STRING_INIT("hair"),
		TPBRT_STRING_INIT("interface"),
		TPBRT_STRING_INIT("measured"),
		TPBRT_STRING_INIT("mix"),
		TPBRT_STRING_INIT("subsurface"),
		TPBRT_STRING_INIT("thindielectric"),
	};

		if (type == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (tpbrt_string_empty(type_str) == TPBRT_TRUE) { return TPBRT_ERROR_INVALID_MATERIAL_TYPE; }

		for (tpbrt_material_type_t t = 0; t < TPBRT_MATERIAL_TYPE_MAX_NUM; ++t) {
				if (tpbrt_string_equals(type_str, TypesStrs[t]) == TPBRT_TRUE) {
					*type = t;
					return TPBRT_ERROR_NONE;
				}
		}

	return TPBRT_ERROR_INVALID_MATERIAL_TYPE;
}

/**
 * @brief Reads bump map parameters.
 * @param bump_map Bump map data.
 * @param params Parameters list.
 * @param textures Textures list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup material-private
 */
static tpbrt_error_t tpbrt_material_read_bump_map(tpbrt_material_bump_map_params_t* const bump_map,
  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures) {
		if (bump_map == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t DisplacementStr				= TPBRT_STRING_INIT("displacement");
	static const tpbrt_string_t NormalMapStr				= TPBRT_STRING_INIT("normalmap");

	static const tpbrt_texture_handle_t DisplacementDefault = TPBRT_TEXTURE_HANDLE_INVALID_INIT;
	static const tpbrt_string_t NormalMapDefault			= TPBRT_EMPTY_STRING_INIT;

	tpbrt_error_t err = tpbrt_params_list_get_texture_of_type(params, DisplacementStr, textures, TPBRT_TEXTURE_TYPE_FLOAT,
	  &DisplacementDefault, &bump_map->as.displacement);
		if (err != TPBRT_ERROR_NONE) { return err; }

		if (err == TPBRT_ERROR_NONE) {
			bump_map->type = TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT;
			return TPBRT_ERROR_NONE;
		}

	err = tpbrt_params_list_get_string(params, NormalMapStr, NormalMapDefault, &bump_map->as.normal_map);
		if (err != TPBRT_ERROR_NONE) { return err; }

		if (tpbrt_string_empty(bump_map->as.normal_map) == TPBRT_FALSE) {
			bump_map->type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP;
			return TPBRT_ERROR_NONE;
		}

	bump_map->type = TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE;
	return TPBRT_ERROR_NONE;
}

/**
 * @brief Reads roughness named parameters.
 * @param roughness_params Roughness data.
 * @param params Parameters list.
 * @param textures Textures list.
 * @param roughness_str Roughness parameter name.
 * @param u_str U parameter name.
 * @param v_str V parameter name.
 * @param remap_str Remap parameter name.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup material-private
 */
static tpbrt_error_t tpbrt_material_read_roughness_named(tpbrt_material_roughness_params_t* const roughness_params,
  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures, const tpbrt_string_t roughness_str,
  const tpbrt_string_t u_str, const tpbrt_string_t v_str, const tpbrt_string_t remap_str) {
		if (roughness_params == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_texture_handle_t RoughnessDefault = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.0f);
	static const tpbrt_bool_t RemapDefault				 = TPBRT_TRUE;

	tpbrt_error_t err = tpbrt_params_list_get_texture_of_type(params, roughness_str, textures, TPBRT_TEXTURE_TYPE_FLOAT,
	  &RoughnessDefault, &roughness_params->roughness);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, u_str, textures, TPBRT_TEXTURE_TYPE_FLOAT, &roughness_params->roughness,
	  &roughness_params->u_roughness);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, v_str, textures, TPBRT_TEXTURE_TYPE_FLOAT, &roughness_params->roughness,
	  &roughness_params->v_roughness);
		if (err != TPBRT_ERROR_NONE) { return err; }

	tpbrt_bool_t remap_roughness;
	err = tpbrt_params_list_get_bool(params, remap_str, RemapDefault, &remap_roughness);
		if (err != TPBRT_ERROR_NONE) { return err; }
	roughness_params->remap_roughness = remap_roughness;

	return TPBRT_ERROR_NONE;
}

/**
 * @brief Reads roughness parameters.
 * @param roughness_params Roughness data.
 * @param params Parameters list.
 * @param textures Textures list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup material-private
 */
static tpbrt_error_t tpbrt_material_read_roughness(tpbrt_material_roughness_params_t* const roughness_params,
  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures) {
		if (roughness_params == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t RoughnessStr	  = TPBRT_STRING_INIT("roughness");
	static const tpbrt_string_t URoughnessStr	  = TPBRT_STRING_INIT("uroughness");
	static const tpbrt_string_t VRoughnessStr	  = TPBRT_STRING_INIT("vroughness");
	static const tpbrt_string_t RemapRoughnessStr = TPBRT_STRING_INIT("remaproughness");

	return tpbrt_material_read_roughness_named(roughness_params, params, textures, RoughnessStr, URoughnessStr, VRoughnessStr,
	  RemapRoughnessStr);
}

/**
 * @brief Reads coated diffuse parameters.
 * @param coated_diffuse Coated diffuse data.
 * @param params Parameters list.
 * @param textures Textures list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup material-private
 */
static tpbrt_error_t tpbrt_material_read_coated_diffuse(tpbrt_material_coated_diffuse_params_t* const coated_diffuse,
  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures) {
		if (coated_diffuse == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t AlbedoStr				   = TPBRT_STRING_INIT("albedo");
	static const tpbrt_string_t GStr					   = TPBRT_STRING_INIT("g");
	static const tpbrt_string_t EtaStr					   = TPBRT_STRING_INIT("eta");
	static const tpbrt_string_t MaxDepthStr				   = TPBRT_STRING_INIT("maxdepth");
	static const tpbrt_string_t NSamplesStr				   = TPBRT_STRING_INIT("nsamples");
	static const tpbrt_string_t ThicknessStr			   = TPBRT_STRING_INIT("thickness");
	static const tpbrt_string_t ReflectanceStr			   = TPBRT_STRING_INIT("reflectance");

	static const tpbrt_texture_handle_t AlbedoDefault	   = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.0f);
	static const tpbrt_texture_handle_t GDefault		   = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.0f);
	static const tpbrt_texture_handle_t EtaDefault		   = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.5f);
	static const tpbrt_uint_t MaxDepthDefault			   = 10u;
	static const tpbrt_uint_t NSamplesDefault			   = 1u;
	static const tpbrt_float_t ThicknessDefault			   = 0.01f;
	static const tpbrt_texture_handle_t ReflectanceDefault = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.5f);

	tpbrt_error_t err = tpbrt_material_read_bump_map(&coated_diffuse->bump_map, params, textures);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, AlbedoStr, textures, TPBRT_TEXTURE_TYPE_SPECTRUM, &AlbedoDefault,
	  &coated_diffuse->albedo);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, GStr, textures, TPBRT_TEXTURE_TYPE_FLOAT, &GDefault, &coated_diffuse->g);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture(params, EtaStr, textures, &EtaDefault, &coated_diffuse->eta);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_uint(params, MaxDepthStr, MaxDepthDefault, &coated_diffuse->max_depth);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_uint(params, NSamplesStr, NSamplesDefault, &coated_diffuse->n_samples);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, ThicknessStr, ThicknessDefault, &coated_diffuse->thickness);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture(params, ReflectanceStr, textures, &ReflectanceDefault, &coated_diffuse->reflectance);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_material_read_roughness(&coated_diffuse->roughness_params, params, textures);
}

/**
 * @brief Reads coated conductor parameters.
 * @param coated_conductor Coated conductor data.
 * @param params Parameters list.
 * @param textures Textures list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup material-private
 */
static tpbrt_error_t tpbrt_material_read_coated_conductor(tpbrt_material_coated_conductor_params_t* const coated_conductor,
  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures) {
		if (coated_conductor == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t AlbedoStr				   = TPBRT_STRING_INIT("albedo");
	static const tpbrt_string_t GStr					   = TPBRT_STRING_INIT("g");
	static const tpbrt_string_t MaxDepthStr				   = TPBRT_STRING_INIT("maxdepth");
	static const tpbrt_string_t NSamplesStr				   = TPBRT_STRING_INIT("nsamples");
	static const tpbrt_string_t ThicknessStr			   = TPBRT_STRING_INIT("thickness");
	static const tpbrt_string_t ReflectanceStr			   = TPBRT_STRING_INIT("reflectance");

	static const tpbrt_string_t ConductorEtaStr			   = TPBRT_STRING_INIT("conductor.eta");
	static const tpbrt_string_t ConductorKStr			   = TPBRT_STRING_INIT("conductor.k");
	static const tpbrt_string_t ConductorRoughnessStr	   = TPBRT_STRING_INIT("conductor.roughness");
	static const tpbrt_string_t ConductorURoughnessStr	   = TPBRT_STRING_INIT("conductor.uroughness");
	static const tpbrt_string_t ConductorVRoughnessStr	   = TPBRT_STRING_INIT("conductor.vroughness");
	static const tpbrt_string_t ConductorRemapRoughnessStr = TPBRT_STRING_INIT("conductor.remaproughness");

	static const tpbrt_string_t InterfaceRoughnessStr	   = TPBRT_STRING_INIT("interface.roughness");
	static const tpbrt_string_t InterfaceURoughnessStr	   = TPBRT_STRING_INIT("interface.uroughness");
	static const tpbrt_string_t InterfaceVRoughnessStr	   = TPBRT_STRING_INIT("interface.vroughness");
	static const tpbrt_string_t InterfaceRemapRoughnessStr = TPBRT_STRING_INIT("interface.remaproughness");

	static const tpbrt_texture_handle_t AlbedoDefault	   = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.0f);
	static const tpbrt_texture_handle_t GDefault		   = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.0f);
	static const tpbrt_uint_t MaxDepthDefault			   = 10u;
	static const tpbrt_uint_t NSamplesDefault			   = 1u;
	static const tpbrt_float_t ThicknessDefault			   = 0.01f;

	static const tpbrt_spectrum_t ConductorEtaDefault	   = TPBRT_SPECTRUM_BUILTIN_INIT(TPBRT_SPECTRUM_BUILTIN_METAL_CU_ETA);
	static const tpbrt_spectrum_t ConductorKDefault		   = TPBRT_SPECTRUM_BUILTIN_INIT(TPBRT_SPECTRUM_BUILTIN_METAL_CU_K);

	tpbrt_error_t err = tpbrt_material_read_bump_map(&coated_conductor->bump_map, params, textures);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, AlbedoStr, textures, TPBRT_TEXTURE_TYPE_SPECTRUM, &AlbedoDefault,
	  &coated_conductor->albedo);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err =
	  tpbrt_params_list_get_texture_of_type(params, GStr, textures, TPBRT_TEXTURE_TYPE_FLOAT, &GDefault, &coated_conductor->g);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_uint(params, MaxDepthStr, MaxDepthDefault, &coated_conductor->max_depth);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_uint(params, NSamplesStr, NSamplesDefault, &coated_conductor->n_samples);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, ThicknessStr, ThicknessDefault, &coated_conductor->thickness);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_spectrum_required(params, ReflectanceStr, &coated_conductor->as.reflectance);
		if (err != TPBRT_ERROR_NONE && err != TPBRT_ERROR_NOT_FOUND) { return err; }

		if (err == TPBRT_ERROR_NOT_FOUND) {
			coated_conductor->value_type = TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_ETA_K;

			err =
			  tpbrt_params_list_get_spectrum(params, ConductorEtaStr, &ConductorEtaDefault, &coated_conductor->as.conductor.eta);
				if (err != TPBRT_ERROR_NONE) { return err; }

			err = tpbrt_params_list_get_spectrum(params, ConductorKStr, &ConductorKDefault, &coated_conductor->as.conductor.k);
				if (err != TPBRT_ERROR_NONE) { return err; }
		}
		else { coated_conductor->value_type = TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_REFLECTANCE; }

	err = tpbrt_material_read_roughness_named(&coated_conductor->conductor_roughness_params, params, textures,
	  ConductorRoughnessStr, ConductorURoughnessStr, ConductorVRoughnessStr, ConductorRemapRoughnessStr);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_material_read_roughness_named(&coated_conductor->interface_roughness_params, params, textures,
	  InterfaceRoughnessStr, InterfaceURoughnessStr, InterfaceVRoughnessStr, InterfaceRemapRoughnessStr);
}

/**
 * @brief Reads conductor parameters.
 * @param conductor Conductor data.
 * @param params Parameters list.
 * @param textures Textures list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup material-private
 */
static tpbrt_error_t tpbrt_material_read_conductor(tpbrt_material_conductor_params_t* const conductor,
  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures) {
		if (conductor == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t ReflectanceStr			   = TPBRT_STRING_INIT("reflectance");
	static const tpbrt_string_t EtaStr					   = TPBRT_STRING_INIT("eta");
	static const tpbrt_string_t KStr					   = TPBRT_STRING_INIT("k");

	static const tpbrt_texture_handle_t ReflectanceDefault = TPBRT_TEXTURE_HANDLE_INVALID_INIT;
	static const tpbrt_texture_handle_t EtaDefault =
	  TPBRT_TEXTURE_HANDLE_SPECTRUM_BUILTIN_INIT(TPBRT_SPECTRUM_BUILTIN_METAL_CU_ETA);
	static const tpbrt_texture_handle_t KDefault = TPBRT_TEXTURE_HANDLE_SPECTRUM_BUILTIN_INIT(TPBRT_SPECTRUM_BUILTIN_METAL_CU_K);

	tpbrt_error_t err							 = tpbrt_material_read_bump_map(&conductor->bump_map, params, textures);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, ReflectanceStr, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
	  &ReflectanceDefault, &conductor->as.reflectance);
		if (err != TPBRT_ERROR_NONE) { return err; }

		if (conductor->as.reflectance.value_type == TPBRT_TEXTURE_HANDLE_VALUE_TYPE_TEXTURE_IDX &&
			conductor->as.reflectance.as.texture_idx == TPBRT_TEXTURE_IDX_INVALID) {
			conductor->value_type = TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_ETA_K;

			err = tpbrt_params_list_get_texture_of_type(params, EtaStr, textures, TPBRT_TEXTURE_TYPE_SPECTRUM, &EtaDefault,
			  &conductor->as.conductor.eta);
				if (err != TPBRT_ERROR_NONE) { return err; }

			err = tpbrt_params_list_get_texture_of_type(params, KStr, textures, TPBRT_TEXTURE_TYPE_SPECTRUM, &KDefault,
			  &conductor->as.conductor.k);
				if (err != TPBRT_ERROR_NONE) { return err; }
		}
		else { conductor->value_type = TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_REFLECTANCE; }

	return tpbrt_material_read_roughness(&conductor->roughness_params, params, textures);
}

/**
 * @brief Reads dielectric parameters.
 * @param dielectric Dielectric data.
 * @param params Parameters list.
 * @param textures Textures list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup material-private
 */
static tpbrt_error_t tpbrt_material_read_dielectric(tpbrt_material_dielectric_params_t* const dielectric,
  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures) {
		if (dielectric == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t EtaStr			   = TPBRT_STRING_INIT("eta");

	static const tpbrt_texture_handle_t EtaDefault = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.5f);

	tpbrt_error_t err							   = tpbrt_material_read_bump_map(&dielectric->bump_map, params, textures);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture(params, EtaStr, textures, &EtaDefault, &dielectric->eta);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_material_read_roughness(&dielectric->roughness_params, params, textures);
}

/**
 * @brief Reads diffuse parameters.
 * @param diffuse Diffuse data.
 * @param params Parameters list.
 * @param textures Textures list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup material-private
 */
static tpbrt_error_t tpbrt_material_read_diffuse(tpbrt_material_diffuse_params_t* const diffuse,
  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures) {
		if (diffuse == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t ReflectanceStr			   = TPBRT_STRING_INIT("reflectance");

	static const tpbrt_texture_handle_t ReflectanceDefault = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.5f);

	tpbrt_error_t err									   = tpbrt_material_read_bump_map(&diffuse->bump_map, params, textures);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, ReflectanceStr, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
	  &ReflectanceDefault, &diffuse->reflectance);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return TPBRT_ERROR_NONE;
}

/**
 * @brief Reads diffuse transmission parameters.
 * @param diffuse_transmission Diffuse transmission data.
 * @param params Parameters list.
 * @param textures Textures list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup material-private
 */
static tpbrt_error_t tpbrt_material_read_diffuse_transmission(
  tpbrt_material_diffuse_transmission_params_t* const diffuse_transmission, const tpbrt_params_list_t* const params,
  const tpbrt_textures_list_t* const textures
) {
		if (diffuse_transmission == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t ReflectanceStr				 = TPBRT_STRING_INIT("reflectance");
	static const tpbrt_string_t TransmittanceStr			 = TPBRT_STRING_INIT("transmittance");
	static const tpbrt_string_t ScaleStr					 = TPBRT_STRING_INIT("scale");

	static const tpbrt_texture_handle_t ReflectanceDefault	 = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.25f);
	static const tpbrt_texture_handle_t TransmittanceDefault = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.25f);
	static const tpbrt_texture_handle_t ScaleDefault		 = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.0f);

	tpbrt_error_t err = tpbrt_material_read_bump_map(&diffuse_transmission->bump_map, params, textures);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, ReflectanceStr, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
	  &ReflectanceDefault, &diffuse_transmission->reflectance);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, TransmittanceStr, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
	  &TransmittanceDefault, &diffuse_transmission->transmittance);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_texture_of_type(params, ScaleStr, textures, TPBRT_TEXTURE_TYPE_FLOAT, &ScaleDefault,
	  &diffuse_transmission->scale);
}

/**
 * @brief Reads hair parameters.
 * @param hair Hair data.
 * @param params Parameters list.
 * @param textures Textures list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup material-private
 */
static tpbrt_error_t tpbrt_material_read_hair(tpbrt_material_hair_params_t* const hair, const tpbrt_params_list_t* const params,
  const tpbrt_textures_list_t* const textures) {
		if (hair == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t SigmaAStr				   = TPBRT_STRING_INIT("sigma_a");
	static const tpbrt_string_t ReflectanceStr			   = TPBRT_STRING_INIT("reflectance");
	static const tpbrt_string_t EumelaninStr			   = TPBRT_STRING_INIT("eumelanin");
	static const tpbrt_string_t PheomelaninStr			   = TPBRT_STRING_INIT("pheomelanin");
	static const tpbrt_string_t EtaStr					   = TPBRT_STRING_INIT("eta");
	static const tpbrt_string_t BetaMStr				   = TPBRT_STRING_INIT("beta_m");
	static const tpbrt_string_t BetaNStr				   = TPBRT_STRING_INIT("beta_n");
	static const tpbrt_string_t AlphaStr				   = TPBRT_STRING_INIT("alpha");

	static const tpbrt_texture_handle_t SigmaADefault	   = TPBRT_TEXTURE_HANDLE_INVALID_INIT;
	static const tpbrt_texture_handle_t ReflectanceDefault = TPBRT_TEXTURE_HANDLE_INVALID_INIT;
	static const tpbrt_texture_handle_t EumelaninDefault   = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.3f);
	static const tpbrt_texture_handle_t PheomelaninDefault = TPBRT_TEXTURE_HANDLE_INVALID_INIT;
	static const tpbrt_texture_handle_t EtaDefault		   = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.55f);
	static const tpbrt_texture_handle_t BetaMDefault	   = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.3f);
	static const tpbrt_texture_handle_t BetaNDefault	   = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.3f);
	static const tpbrt_texture_handle_t AlphaDefault	   = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(2.0f);

	tpbrt_error_t err									   = tpbrt_material_read_bump_map(&hair->bump_map, params, textures);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, SigmaAStr, textures, TPBRT_TEXTURE_TYPE_SPECTRUM, &SigmaADefault,
	  &hair->sigma_a);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, ReflectanceStr, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
	  &ReflectanceDefault, &hair->reflectance);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, EumelaninStr, textures, TPBRT_TEXTURE_TYPE_FLOAT, &EumelaninDefault,
	  &hair->eumelanin);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, PheomelaninStr, textures, TPBRT_TEXTURE_TYPE_FLOAT, &PheomelaninDefault,
	  &hair->pheomelanin);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, EtaStr, textures, TPBRT_TEXTURE_TYPE_FLOAT, &EtaDefault, &hair->eta);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err =
	  tpbrt_params_list_get_texture_of_type(params, BetaMStr, textures, TPBRT_TEXTURE_TYPE_FLOAT, &BetaMDefault, &hair->beta_m);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err =
	  tpbrt_params_list_get_texture_of_type(params, BetaNStr, textures, TPBRT_TEXTURE_TYPE_FLOAT, &BetaNDefault, &hair->beta_n);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_texture_of_type(params, AlphaStr, textures, TPBRT_TEXTURE_TYPE_FLOAT, &AlphaDefault,
	  &hair->alpha);
}

/**
 * @brief Reads measured parameters.
 * @param measured Measured data.
 * @param params Parameters list.
 * @param textures Textures list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup material-private
 */
static tpbrt_error_t tpbrt_material_read_measured(tpbrt_material_measured_params_t* const measured,
  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures) {
		if (measured == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t FileNameStr = TPBRT_STRING_INIT("filename");

	const tpbrt_error_t err					= tpbrt_material_read_bump_map(&measured->bump_map, params, textures);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_string_required(params, FileNameStr, &measured->file_name);
}

/**
 * @brief Reads mix parameters.
 * @param mix Mix data.
 * @param params Parameters list.
 * @param textures Textures list.
 * @param materials Materials list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup material-private
 */
static tpbrt_error_t tpbrt_material_read_mix(tpbrt_material_mix_params_t* const mix, const tpbrt_params_list_t* const params,
  const tpbrt_textures_list_t* const textures, const tpbrt_materials_list_t* const materials) {
		if (mix == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t MaterialsStr		  = TPBRT_STRING_INIT("materials");
	static const tpbrt_string_t AmountStr			  = TPBRT_STRING_INIT("amount");

	static const tpbrt_texture_handle_t AmountDefault = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.5f);

	tpbrt_string_array_t temp_strings;
	tpbrt_error_t err = tpbrt_params_list_get_strings_of_size(params, MaterialsStr, 2u, &temp_strings);
		if (err != TPBRT_ERROR_NONE) { return err; }

		for (tpbrt_size_t i = 0; i < temp_strings.count; ++i) {
			err = tpbrt_materials_list_get_handle(materials, temp_strings.data[i], &mix->materials[i]);
				if (err != TPBRT_ERROR_NONE) {
					tpbrt_string_array_free(&temp_strings);
					return err;
				}
		}
	tpbrt_string_array_free(&temp_strings);

	return tpbrt_params_list_get_texture_of_type(params, AmountStr, textures, TPBRT_TEXTURE_TYPE_FLOAT, &AmountDefault,
	  &mix->amount);
}

/**
 * @brief Reads scattering parameters.
 * @param scattering Scattering data.
 * @param params Parameters list.
 * @param textures Textures list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_INVALID_SCATTERING_BUILTIN, TPBRT_ERROR_PARAM_TYPE_MISMATCH, TPBRT_ERROR_NONE).
 * @ingroup material-private
 */
static tpbrt_error_t tpbrt_material_read_subsurface_scattering(tpbrt_material_subsurface_scattering_t* const scattering,
  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures) {
		if (scattering == TPBRT_NULL || params == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t NameStr				  = TPBRT_STRING_INIT("name");
	static const tpbrt_string_t SigmaAStr			  = TPBRT_STRING_INIT("sigma_a");
	static const tpbrt_string_t SigmaSStr			  = TPBRT_STRING_INIT("sigma_s");

	static const tpbrt_string_t NameDefault			  = TPBRT_EMPTY_STRING_INIT;
	static const tpbrt_texture_handle_t SigmaADefault = TPBRT_TEXTURE_HANDLE_SPECTRUM_RGB_INIT(0.0011f, 0.0024f, 0.014f);
	static const tpbrt_texture_handle_t SigmaSDefault = TPBRT_TEXTURE_HANDLE_SPECTRUM_RGB_INIT(2.55f, 3.21f, 3.77f);

	// get_string returns the default ("") with TPBRT_ERROR_NONE when "name" is missing - like pbrt-v4,
	// an empty name means "no builtin", so sigma_a / sigma_s are read instead
	tpbrt_string_t name_str;
	tpbrt_error_t err = tpbrt_params_list_get_string(params, NameStr, NameDefault, &name_str);
		if (err != TPBRT_ERROR_NONE) { return err; }

		if (tpbrt_string_empty(name_str) == TPBRT_FALSE) {
			scattering->type = TPBRT_MATERIAL_SUBSURFACE_SCATTERING_VALUE_BUILTIN;

			err				 = tpbrt_scattering_builtin_from_string(name_str, &scattering->as.builtin);
			tpbrt_string_free(&name_str);
			return err;
		}
	tpbrt_string_free(&name_str);

	scattering->type = TPBRT_MATERIAL_SUBSURFACE_SCATTERING_VALUE_SIGMA;

	err = tpbrt_params_list_get_texture_of_type(params, SigmaAStr, textures, TPBRT_TEXTURE_TYPE_SPECTRUM, &SigmaADefault,
	  &scattering->as.sigma.a);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_texture_of_type(params, SigmaSStr, textures, TPBRT_TEXTURE_TYPE_SPECTRUM, &SigmaSDefault,
	  &scattering->as.sigma.s);
}

/**
 * @brief Reads subsurface parameters.
 * @param subsurface Subsurface data.
 * @param params Parameters list.
 * @param textures Textures list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup material-private
 */
static tpbrt_error_t tpbrt_material_read_subsurface(tpbrt_material_subsurface_params_t* const subsurface,
  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures) {
		if (subsurface == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t EtaStr					   = TPBRT_STRING_INIT("eta");
	static const tpbrt_string_t GStr					   = TPBRT_STRING_INIT("g");
	static const tpbrt_string_t MFPStr					   = TPBRT_STRING_INIT("mfp");
	static const tpbrt_string_t ReflectanceStr			   = TPBRT_STRING_INIT("reflectance");
	static const tpbrt_string_t ScaleStr				   = TPBRT_STRING_INIT("scale");

	static const tpbrt_texture_handle_t EtaDefault		   = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.33f);
	static const tpbrt_texture_handle_t GDefault		   = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(0.0f);
	static const tpbrt_texture_handle_t MFPDefault		   = TPBRT_TEXTURE_HANDLE_INVALID_INIT;
	static const tpbrt_texture_handle_t ReflectanceDefault = TPBRT_TEXTURE_HANDLE_INVALID_INIT;
	static const tpbrt_float_t ScaleDefault				   = 1.0f;

	tpbrt_error_t err = tpbrt_material_read_bump_map(&subsurface->bump_map, params, textures);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err =
	  tpbrt_params_list_get_texture_of_type(params, EtaStr, textures, TPBRT_TEXTURE_TYPE_FLOAT, &EtaDefault, &subsurface->eta);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, GStr, textures, TPBRT_TEXTURE_TYPE_FLOAT, &GDefault, &subsurface->g);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err =
	  tpbrt_params_list_get_texture_of_type(params, MFPStr, textures, TPBRT_TEXTURE_TYPE_FLOAT, &MFPDefault, &subsurface->mfp);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_texture_of_type(params, ReflectanceStr, textures, TPBRT_TEXTURE_TYPE_SPECTRUM,
	  &ReflectanceDefault, &subsurface->reflectance);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_material_read_subsurface_scattering(&subsurface->scattering, params, textures);
		if (err != TPBRT_ERROR_NONE) { return err; }

	err = tpbrt_params_list_get_float(params, ScaleStr, ScaleDefault, &subsurface->scale);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_material_read_roughness(&subsurface->roughness_params, params, textures);
}

/**
 * @brief Reads thin dielectric parameters.
 * @param thin_dielectric Thin dielectric data.
 * @param params Parameters list.
 * @param textures Textures list.
 * @return Error code (possible errors TPBRT_ERROR_INVALID_POINTER, TPBRT_ERROR_NONE).
 * @ingroup material-private
 */
static tpbrt_error_t tpbrt_material_read_thin_dielectric(tpbrt_material_thin_dielectric_params_t* const thin_dielectric,
  const tpbrt_params_list_t* const params, const tpbrt_textures_list_t* const textures) {
		if (thin_dielectric == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t EtaStr			   = TPBRT_STRING_INIT("eta");

	static const tpbrt_texture_handle_t EtaDefault = TPBRT_TEXTURE_HANDLE_SPECTRUM_CONSTANT_INIT(1.5f);

	const tpbrt_error_t err						   = tpbrt_material_read_bump_map(&thin_dielectric->bump_map, params, textures);
		if (err != TPBRT_ERROR_NONE) { return err; }

	return tpbrt_params_list_get_texture(params, EtaStr, textures, &EtaDefault, &thin_dielectric->eta);
}

tpbrt_error_t tpbrt_material_init(const tpbrt_string_t type_str, const tpbrt_params_list_t* const params,
  const tpbrt_textures_list_t* const textures, const tpbrt_materials_list_t* const materials, tpbrt_material_t* const material) {
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

tpbrt_error_t tpbrt_material_init_named(const tpbrt_string_t name, const tpbrt_params_list_t* const params,
  const tpbrt_textures_list_t* const textures, const tpbrt_materials_list_t* const materials, tpbrt_material_t* const material) {
		if (material == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (tpbrt_string_empty(name)) { return TPBRT_ERROR_INVALID_STRING; }

		if (memset(material, 0, sizeof(tpbrt_material_t)) == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	static const tpbrt_string_t TypeStr = TPBRT_STRING_INIT("type");

	tpbrt_string_t type_str;
	tpbrt_error_t err = tpbrt_params_list_get_string_required(params, TypeStr, &type_str);
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

/**
 * @brief Frees bump map data.
 * @param bump_map Bump map data to free.
 * @ingroup material-private
 */
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
				tpbrt_texture_handle_free(&params->mfp);
				tpbrt_texture_handle_free(&params->reflectance);
				tpbrt_texture_handle_free(&params->eta);

					if (params->scattering.type == TPBRT_MATERIAL_SUBSURFACE_SCATTERING_VALUE_SIGMA) {
						tpbrt_texture_handle_free(&params->scattering.as.sigma.a);
						tpbrt_texture_handle_free(&params->scattering.as.sigma.s);
					}
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
/**
 * @brief Initializes empty materials list with type conversion.
 * @ingroup material-private
 */
#define TPBRT_EMPTY_MATERIALS_LIST (tpbrt_materials_list_t) { .materials = TPBRT_NULL, .count = 0 }

tpbrt_error_t tpbrt_materials_list_init(tpbrt_materials_list_t* const materials_list) {
		if (materials_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	*materials_list = TPBRT_EMPTY_MATERIALS_LIST;
	return TPBRT_ERROR_NONE;
}

tpbrt_error_t tpbrt_materials_list_append(tpbrt_materials_list_t* const materials_list, const tpbrt_material_t* const material,
  tpbrt_material_handle_t* const handle) {
		if (material == TPBRT_NULL || materials_list == TPBRT_NULL || handle == TPBRT_NULL) {
			return TPBRT_ERROR_INVALID_POINTER;
		}

	*handle = TPBRT_MATERIAL_HANDLE_INVALID;
	tpbrt_material_t* const new_list =
	  tpbrt_array_reserve_append(materials_list->materials, materials_list->count, sizeof(tpbrt_material_t));
		if (new_list == TPBRT_NULL) { return TPBRT_ERROR_OUT_OF_MEMORY; }

	const tpbrt_size_t idx	  = materials_list->count;
	new_list[idx]			  = *material;
	new_list[idx].idx		  = (tpbrt_idx_t)idx;
	materials_list->materials = new_list;
	materials_list->count	  = idx + 1u;
	handle->material_idx	  = (tpbrt_material_idx_t)idx;

	return tpbrt_name_index_insert(&materials_list->names, &(TPBRT_NAME_INDEX_VIEW(new_list, idx + 1u, tpbrt_material_t)),
	  (tpbrt_idx_t)idx);
}

tpbrt_error_t tpbrt_materials_list_get(const tpbrt_materials_list_t* const materials_list, const tpbrt_string_t material_name,
  const tpbrt_material_t** const material) {
		if (material == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	const tpbrt_idx_t idx = tpbrt_name_index_find(&materials_list->names,
	  &(TPBRT_NAME_INDEX_VIEW(materials_list->materials, materials_list->count, tpbrt_material_t)), material_name, TPBRT_NULL,
	  TPBRT_NULL);
		if (idx == TPBRT_IDX_INVALID) {
			*material = TPBRT_NULL;
			return TPBRT_ERROR_NOT_FOUND;
		}

	*material = &materials_list->materials[idx];
	return TPBRT_ERROR_NONE;
}

tpbrt_error_t tpbrt_materials_list_shrink_to_fit(tpbrt_materials_list_t* const materials_list) {
		if (materials_list == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	materials_list->materials =
	  tpbrt_array_shrink_to_fit(materials_list->materials, materials_list->count, sizeof(tpbrt_material_t));
	return tpbrt_name_index_shrink(&materials_list->names,
	  &(TPBRT_NAME_INDEX_VIEW(materials_list->materials, materials_list->count, tpbrt_material_t)));
}

tpbrt_error_t tpbrt_materials_list_get_handle(const tpbrt_materials_list_t* const materials_list,
  const tpbrt_string_t material_name, tpbrt_material_handle_t* const material_handle) {
		if (material_handle == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

	const tpbrt_material_t* material;
	const tpbrt_error_t err = tpbrt_materials_list_get(materials_list, material_name, &material);

		if (err != TPBRT_ERROR_NONE) { return err; }

	material_handle->material_idx = material->idx;
	return TPBRT_ERROR_NONE;
}

void tpbrt_materials_list_free(tpbrt_materials_list_t* const materials_list) {
		if (materials_list == TPBRT_NULL) { return; }

	tpbrt_name_index_free(&materials_list->names);

		if (materials_list->materials == TPBRT_NULL) { return; }

		for (tpbrt_size_t i = 0; i < materials_list->count; ++i) { tpbrt_material_free(materials_list->materials + i); }

	free(materials_list->materials);
	*materials_list = TPBRT_EMPTY_MATERIALS_LIST;
}

tpbrt_bool_t tpbrt_materials_list_empty(const tpbrt_materials_list_t* const materials_list) {
	return materials_list->materials == TPBRT_NULL || materials_list->count == 0;
}

#pragma endregion

#pragma region API

tpbrt_error_t tpbrt_get_material_by_name(const tpbrt_materials_list_t* const materials, const tpbrt_string_t name,
  const tpbrt_material_t** const material) {
	return tpbrt_materials_list_get(materials, name, material);
}

tpbrt_error_t tpbrt_get_material_by_handle(const tpbrt_materials_list_t* const materials, const tpbrt_material_handle_t handle,
  const tpbrt_material_t** const material) {
		if (material == TPBRT_NULL) { return TPBRT_ERROR_INVALID_POINTER; }

		if (handle.material_idx == TPBRT_MATERIAL_IDX_INVALID || handle.material_idx >= materials->count) {
			*material = TPBRT_NULL;
			return TPBRT_ERROR_INVALID_HANDLE;
		}

	*material = materials->materials + handle.material_idx;
	return TPBRT_ERROR_NONE;
}

#pragma endregion
