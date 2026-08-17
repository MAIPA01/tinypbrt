#pragma once
#ifndef _TINYPBRT_MATERIAL_H_
#define _TINYPBRT_MATERIAL_H_

#include "../texture.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_MATERIAL_COATED_DIFFUSE		= 0,
		TPBRT_MATERIAL_COATED_CONDUCTOR		= 1,
		TPBRT_MATERIAL_CONDUCTOR			= 2,
		TPBRT_MATERIAL_DIELECTRIC			= 3,
		TPBRT_MATERIAL_DIFFUSE				= 4,
		TPBRT_MATERIAL_DIFFUSE_TRANSMISSION = 5,
		TPBRT_MATERIAL_HAIR					= 6,
		TPBRT_MATERIAL_INTERFACE			= 7,
		TPBRT_MATERIAL_MEASURED				= 8,
		TPBRT_MATERIAL_MIX					= 9,
		TPBRT_MATERIAL_SUBSURFACE			= 10,
		TPBRT_MATERIAL_THIN_DIELECTRIC		= 11,
	} tpbrt_material_type_t;

	typedef struct {
		tpbrt_float_valued_texture_t roughness;
		tpbrt_float_valued_texture_t u_roughness;
		tpbrt_float_valued_texture_t v_roughness;
		tpbrt_bool_t remap_roughness;
	} tpbrt_material_roughness_params_t;

	typedef struct {
		tpbrt_spectrum_valued_texture_t albedo;
		tpbrt_float_valued_texture_t g;
		uint32_t max_depth;
		uint32_t nsamples;
		float thickness;
		tpbrt_spectrum_valued_texture_t reflectance;
		tpbrt_material_roughness_params_t roughness_params;
	} tpbrt_material_coated_diffuse_params_t;

	typedef struct {
		tpbrt_spectrum_valued_texture_t albedo;
		tpbrt_float_valued_texture_t g;
		uint32_t max_depth;
		uint32_t nsamples;
		float thickness;
		tpbrt_spectrum_t eta;
		tpbrt_spectrum_t k;
		tpbrt_spectrum_t reflectance;
		tpbrt_material_roughness_params_t roughness_params;
	} tpbrt_material_coated_conductor_params_t;

	typedef struct {
		tpbrt_spectrum_valued_texture_t eta;
		tpbrt_spectrum_valued_texture_t k;
		tpbrt_spectrum_valued_texture_t reflectance;
		tpbrt_material_roughness_params_t roughness_params;
	} tpbrt_material_conductor_params_t;

	typedef struct {
		tpbrt_float_valued_texture_t eta_f;
		tpbrt_spectrum_valued_texture_t eta_s;
		tpbrt_material_roughness_params_t roughness_params;
	} tpbrt_material_dielectric_params_t;

	typedef struct {
		tpbrt_spectrum_valued_texture_t reflectance;
	} tpbrt_material_diffuse_params_t;

	typedef struct {
		tpbrt_spectrum_valued_texture_t reflectance;
		tpbrt_spectrum_valued_texture_t transmittance;
		tpbrt_float_valued_texture_t scale;
	} tpbrt_material_diffuse_transmission_params_t;

	typedef struct {
		tpbrt_spectrum_valued_texture_t sigma_a;
		tpbrt_spectrum_valued_texture_t reflectance;
		tpbrt_float_valued_texture_t eumelanin;
		tpbrt_float_valued_texture_t pheomelanin;
		tpbrt_float_valued_texture_t eta;
		tpbrt_float_valued_texture_t beta_m;
		tpbrt_float_valued_texture_t beta_n;
		tpbrt_float_valued_texture_t alpha;
	} tpbrt_material_hair_params_t;

	typedef struct {
		tpbrt_string_t file_name;
	} tpbrt_material_measured_params_t;

	typedef struct {
		tpbrt_string_t materials[2];
		tpbrt_float_valued_texture_t amount;
	} tpbrt_material_mix_params_t;

	typedef struct {
		tpbrt_float_valued_texture_t eta;
		tpbrt_float_valued_texture_t g;
		tpbrt_float_valued_texture_t mfp;
		tpbrt_string_t name;
		tpbrt_spectrum_valued_texture_t reflectance;
		tpbrt_spectrum_valued_texture_t sigma_a;
		tpbrt_spectrum_valued_texture_t sigma_s;
		float scale;
		tpbrt_material_roughness_params_t roughness_params;
	} tpbrt_material_subsurface_params_t;

	typedef struct {
		tpbrt_string_t name;
		tpbrt_material_type_t type;
		tpbrt_float_valued_texture_t displacement;
		tpbrt_string_t normal_map;

		union {
			tpbrt_material_coated_diffuse_params_t coated_diffuse_params;
			tpbrt_material_coated_conductor_params_t coated_conductor_params;
			tpbrt_material_conductor_params_t conductor_params;
			tpbrt_material_dielectric_params_t dielectric_params;
			tpbrt_material_diffuse_params_t diffuse_params;
			tpbrt_material_diffuse_transmission_params_t diffuse_transmission_params;
			tpbrt_material_hair_params_t hair_params;
			tpbrt_material_measured_params_t measured_params;
			tpbrt_material_mix_params_t mix_params;
			tpbrt_material_subsurface_params_t subsurface_params;
		};
	} tpbrt_material_t;

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MATERIAL_H_
