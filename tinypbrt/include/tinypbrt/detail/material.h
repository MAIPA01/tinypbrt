#pragma once
#ifndef _TINYPBRT_MATERIAL_H_
#define _TINYPBRT_MATERIAL_H_

#include <tinypbrt/detail/color.h>
#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/texture.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum : uint8_t {
		TPBRT_MATERIAL_TYPE_COATED_DIFFUSE		 = 0,
		TPBRT_MATERIAL_TYPE_COATED_CONDUCTOR	 = 1,
		TPBRT_MATERIAL_TYPE_CONDUCTOR			 = 2,
		TPBRT_MATERIAL_TYPE_DIELECTRIC			 = 3,
		TPBRT_MATERIAL_TYPE_DIFFUSE				 = 4,
		TPBRT_MATERIAL_TYPE_DIFFUSE_TRANSMISSION = 5,
		TPBRT_MATERIAL_TYPE_HAIR				 = 6,
		TPBRT_MATERIAL_TYPE_INTERFACE			 = 7,
		TPBRT_MATERIAL_TYPE_MEASURED			 = 8,
		TPBRT_MATERIAL_TYPE_MIX					 = 9,
		TPBRT_MATERIAL_TYPE_SUBSURFACE			 = 10,
		TPBRT_MATERIAL_TYPE_THIN_DIELECTRIC		 = 11,
		TPBRT_MATERIAL_TYPE_MAX_NUM				 = 12,
	} tpbrt_material_type_t;

	typedef tpbrt_size_t tpbrt_material_handle_t;

	typedef enum {
		TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE		  = 0,
		TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT = 1,
		TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP	  = 2,
	} tpbrt_material_bump_map_type_t;

	typedef struct {
		tpbrt_material_bump_map_type_t type;

		union {
			tpbrt_texture_handle_t displacement;
			tpbrt_string_t normal_map;
		} as;
	} tpbrt_material_bump_map_params_t;

	typedef struct {
		tpbrt_texture_handle_t roughness;
		tpbrt_texture_handle_t u_roughness;
		tpbrt_texture_handle_t v_roughness;
		tpbrt_bool_t remap_roughness;
	} tpbrt_material_roughness_params_t;

	typedef struct {
		tpbrt_material_bump_map_params_t bump_map;
		tpbrt_texture_handle_t albedo;
		tpbrt_texture_handle_t g;
		tpbrt_uint_t max_depth;
		tpbrt_uint_t n_samples;
		tpbrt_float_t thickness;
		tpbrt_texture_handle_t reflectance;
		tpbrt_material_roughness_params_t roughness_params;
	} tpbrt_material_coated_diffuse_params_t;

	typedef enum {
		TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_ETA_K		= 0,
		TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_REFLECTANCE = 1,
	} tpbrt_material_conductor_value_type_t;

	typedef struct {
		tpbrt_material_bump_map_params_t bump_map;
		tpbrt_texture_handle_t albedo;
		tpbrt_texture_handle_t g;
		tpbrt_uint_t max_depth;
		tpbrt_uint_t n_samples;
		tpbrt_float_t thickness;
		tpbrt_material_conductor_value_type_t value_type;

		union {
			struct {
				tpbrt_spectrum_t eta;
				tpbrt_spectrum_t k;
			} conductor;

			tpbrt_spectrum_t reflectance;
		} as;

		tpbrt_material_roughness_params_t roughness_params;
	} tpbrt_material_coated_conductor_params_t;

	typedef struct {
		tpbrt_material_bump_map_params_t bump_map;
		tpbrt_material_conductor_value_type_t value_type;

		union {
			struct {
				tpbrt_texture_handle_t eta;
				tpbrt_texture_handle_t k;
			} conductor;

			tpbrt_texture_handle_t reflectance;
		} as;

		tpbrt_material_roughness_params_t roughness_params;
	} tpbrt_material_conductor_params_t;

	typedef enum {
		TPBRT_MATERIAL_DIELECTRIC_ETA_TYPE_SINGLE_INDEX		= 0,
		TPBRT_MATERIAL_DIELECTRIC_ETA_TYPE_WAVELENGTH_INDEX = 1,
	} tpbrt_material_dielectric_eta_type_t;

	typedef struct {
		tpbrt_material_bump_map_params_t bump_map;
		tpbrt_material_dielectric_eta_type_t eta_type;

		union {
			tpbrt_texture_handle_t single;
			tpbrt_texture_handle_t wavelength;
		} eta_as;

		tpbrt_material_roughness_params_t roughness_params;
	} tpbrt_material_dielectric_params_t;

	typedef struct {
		tpbrt_material_bump_map_params_t bump_map;
		tpbrt_texture_handle_t reflectance;
	} tpbrt_material_diffuse_params_t;

	typedef struct {
		tpbrt_material_bump_map_params_t bump_map;
		tpbrt_texture_handle_t reflectance;
		tpbrt_texture_handle_t transmittance;
		tpbrt_texture_handle_t scale;
	} tpbrt_material_diffuse_transmission_params_t;

	typedef struct {
		tpbrt_material_bump_map_params_t bump_map;
		tpbrt_texture_handle_t sigma_a;
		tpbrt_texture_handle_t reflectance;
		tpbrt_texture_handle_t eumelanin;
		tpbrt_texture_handle_t pheomelanin;
		tpbrt_texture_handle_t eta;
		tpbrt_texture_handle_t beta_m;
		tpbrt_texture_handle_t beta_n;
		tpbrt_texture_handle_t alpha;
	} tpbrt_material_hair_params_t;

	typedef struct {
		tpbrt_material_bump_map_params_t bump_map;
		tpbrt_string_t file_name;
	} tpbrt_material_measured_params_t;

	typedef struct {
		tpbrt_material_handle_t materials[2];
		tpbrt_texture_handle_t amount;
	} tpbrt_material_mix_params_t;

	typedef struct {
		tpbrt_material_bump_map_params_t bump_map;
		tpbrt_texture_handle_t eta;
		tpbrt_texture_handle_t g;
		tpbrt_texture_handle_t mfp;
		tpbrt_string_t name;
		tpbrt_texture_handle_t reflectance;
		tpbrt_texture_handle_t sigma_a;
		tpbrt_texture_handle_t sigma_s;
		tpbrt_float_t scale;
		tpbrt_material_roughness_params_t roughness_params;
	} tpbrt_material_subsurface_params_t;

	typedef struct {
		tpbrt_material_bump_map_params_t bump_map;
	} tpbrt_material_thin_dielectric_params_t;

	typedef struct {
		tpbrt_size_t idx;
		tpbrt_string_t name;
		tpbrt_material_type_t type;

		union {
			tpbrt_material_coated_diffuse_params_t coated_diffuse;
			tpbrt_material_coated_conductor_params_t coated_conductor;
			tpbrt_material_conductor_params_t conductor;
			tpbrt_material_dielectric_params_t dielectric;
			tpbrt_material_diffuse_params_t diffuse;
			tpbrt_material_diffuse_transmission_params_t diffuse_transmission;
			tpbrt_material_hair_params_t hair;
			tpbrt_material_measured_params_t measured;
			tpbrt_material_mix_params_t mix;
			tpbrt_material_subsurface_params_t subsurface;
			tpbrt_material_thin_dielectric_params_t thin_dielectric;
		} as;
	} tpbrt_material_t;

	typedef struct {
		tpbrt_material_t* materials;
		tpbrt_size_t count;
	} tpbrt_materials_list_t;

	tpbrt_error_t tpbrt_get_material_by_name(const tpbrt_materials_list_t* materials, const tpbrt_string_t* name,
	  const tpbrt_material_t** material);
	tpbrt_error_t tpbrt_get_material_by_handle(const tpbrt_materials_list_t* materials, const tpbrt_material_handle_t* handle,
	  const tpbrt_material_t** material);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MATERIAL_H_
