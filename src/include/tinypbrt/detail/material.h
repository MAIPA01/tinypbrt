#pragma once
#ifndef _TINYPBRT_MATERIAL_H_
#define _TINYPBRT_MATERIAL_H_

#include <tinypbrt/detail/fwd.h>

#include <tinypbrt/detail/color.h>
#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/error.h>
#include <tinypbrt/detail/scattering.h>
#include <tinypbrt/detail/texture.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @defgroup material-public Material
	 * @ingroup tinypbrt-public
	 */

	/**
	 * @brief Material types.
	 * @ingroup material-public
	 */
	typedef enum tpbrt_material_type_t {
		/// @brief Coated diffuse.
		TPBRT_MATERIAL_TYPE_COATED_DIFFUSE		 = 0,
		/// @brief Coated conductor.
		TPBRT_MATERIAL_TYPE_COATED_CONDUCTOR	 = 1,
		/// @brief Conductor.
		TPBRT_MATERIAL_TYPE_CONDUCTOR			 = 2,
		/// @brief Dielectric.
		TPBRT_MATERIAL_TYPE_DIELECTRIC			 = 3,
		/// @brief Diffuse.
		TPBRT_MATERIAL_TYPE_DIFFUSE				 = 4,
		/// @brief Diffuse transmission.
		TPBRT_MATERIAL_TYPE_DIFFUSE_TRANSMISSION = 5,
		/// @brief Hair.
		TPBRT_MATERIAL_TYPE_HAIR				 = 6,
		/// @brief Interface.
		TPBRT_MATERIAL_TYPE_INTERFACE			 = 7,
		/// @brief Measured.
		TPBRT_MATERIAL_TYPE_MEASURED			 = 8,
		/// @brief Mix.
		TPBRT_MATERIAL_TYPE_MIX					 = 9,
		/// @brief Subsurface.
		TPBRT_MATERIAL_TYPE_SUBSURFACE			 = 10,
		/// @brief Thin Dielectric.
		TPBRT_MATERIAL_TYPE_THIN_DIELECTRIC		 = 11,
		/// @brief Count of available material types.
		TPBRT_MATERIAL_TYPE_MAX_NUM				 = 12,
	} tpbrt_material_type_t;

	/**
	 * @brief Material handle.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_handle_t {
		/// @brief Material index.
		tpbrt_material_idx_t material_idx;
	} tpbrt_material_handle_t;

/**
 * @brief Invalid material handle initialization.
 * @ingroup material-public
 */
#define TPBRT_MATERIAL_HANDLE_INVALID_INIT { .material_idx = TPBRT_MATERIAL_IDX_INVALID }
/**
 * @brief Invalid material handle initialization with type conversion.
 * @ingroup material-public
 */
#define TPBRT_MATERIAL_HANDLE_INVALID (tpbrt_material_handle_t) TPBRT_MATERIAL_HANDLE_INVALID_INIT

	/**
	 * @brief Material bump map types.
	 * @ingroup material-public
	 */
	typedef enum tpbrt_material_bump_map_type_t {
		/// @brief No bump map.
		TPBRT_MATERIAL_BUMP_MAP_TYPE_NONE		  = 0,
		/// @brief Displacement as bump map.
		TPBRT_MATERIAL_BUMP_MAP_TYPE_DISPLACEMENT = 1,
		/// @brief Normal map as bump map.
		TPBRT_MATERIAL_BUMP_MAP_TYPE_NORMAL_MAP	  = 2,
		/// @brief Count of available material bump map types.
		TPBRT_MATERIAL_BUMP_MAP_TYPE_MAX_NUM	  = 3,
	} tpbrt_material_bump_map_type_t;

	/**
	 * @brief Material bump map parameters.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_bump_map_params_t {
		/// @brief Value dependent on type value.
		union {
			/// @brief Float-valued texture specifying surface height offsets that are used for bump mapping.
			tpbrt_texture_handle_t displacement;
			/// @brief Filename for an image to use to specify a normal map.
			tpbrt_string_t normal_map;
		} as;

		/// @brief Bump map value type.
		tpbrt_material_bump_map_type_t type : 3;
	} tpbrt_material_bump_map_params_t;

	/**
	 * @brief Material roughness parameters.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_roughness_params_t {
		/// @brief Overall surface roughness, modeled using the Trowbridge-Reitz (GGX) microfacet distribution. If zero, the surface is perfectly smooth and perfect specular reflection (and possibly transmission) occurs. Larger values correspond to greater roughness.
		tpbrt_texture_handle_t roughness;
		/// @brief For surfaces with anisotropic roughness distributions, microfacet roughness in the u direction.
		tpbrt_texture_handle_t u_roughness;
		/// @brief For surfaces with anisotropic roughness distributions, microfacet roughness in the v direction.
		tpbrt_texture_handle_t v_roughness;
		/// @brief If true, roughness values are expected to be between 0 and 1, and are remapped to microfacet distribution function parameter values that range from near-perfect-specular at 0 to very rough at 1. Otherwise the roughness parameters are used directly for the alpha parameter of the Trowbridge-Reitz microfacet distribution function. Using this remapping gives a more intuitive control of roughness than specifying alpha.
		tpbrt_bool_t remap_roughness : 1;
	} tpbrt_material_roughness_params_t;

	/**
	 * @brief Coated diffuse material parameters.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_coated_diffuse_params_t {
		/// @brief Bump map parameters.
		tpbrt_material_bump_map_params_t bump_map;
		/// @brief Scattering albedo of the medium between the interface and the diffuse base layer. Must be less than or equal to 1.
		tpbrt_texture_handle_t albedo;
		/// @brief Henyey-Greenstein asymmetry parameter ranging from -1 to 1 that describes the distribution of scattered light in the medium.
		tpbrt_texture_handle_t g;
		/// @brief Index of refraction of the medium inside the surface.
		tpbrt_texture_handle_t eta;
		/// @brief Maximum number of bounces of light to model when modeling light scattering among the interface, the base layer, and the medium (if present).
		tpbrt_uint_t max_depth;
		/// @brief Number of independent samples to take when performing Monte Carlo integration of light scattering between the layers.
		tpbrt_uint_t n_samples;
		/// @brief Reflectance of the base diffuse layer.
		tpbrt_texture_handle_t reflectance;
		/// @brief Roughness parameters.
		tpbrt_material_roughness_params_t roughness_params;
		/// @brief Thickness of the medium between the two layers.
		tpbrt_float_t thickness;
	} tpbrt_material_coated_diffuse_params_t;

	/**
	 * @brief Conductor material value type.
	 * @ingroup material-public
	 */
	typedef enum tpbrt_material_conductor_value_type_t {
		/// @brief Conductor material value defined as 'eta' and 'k' values.
		TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_ETA_K		= 0,
		/// @brief Conductor material value defined as 'reflectance' value.
		TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_REFLECTANCE = 1,
		/// @brief Count of available conductor material value types.
		TPBRT_MATERIAL_CONDUCTOR_VALUE_TYPE_MAX_NUM		= 2,
	} tpbrt_material_conductor_value_type_t;

	/**
	 * @brief Coated conductor material parameters.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_coated_conductor_params_t {
		/// @brief Bump map parameters.
		tpbrt_material_bump_map_params_t bump_map;
		/// @brief Scattering albedo of the medium between the interface and the diffuse base layer. Must be less than or equal to 1.
		tpbrt_texture_handle_t albedo;
		/// @brief Henyey-Greenstein asymmetry parameter ranging from -1 to 1 that describes the distribution of scattered light in the medium.
		tpbrt_texture_handle_t g;
		/// @brief Maximum number of bounces of light to model when modeling light scattering among the interface, the base layer, and the medium (if present).
		tpbrt_uint_t max_depth;
		/// @brief Number of independent samples to take when performing Monte Carlo integration of light scattering between the layers.
		tpbrt_uint_t n_samples;

		/// @brief Value dependent on value_type value.
		union {
			/// @brief Holds conductor 'eta' and 'k' values.
			struct {
				/// @brief The conductor's index of refraction.
				tpbrt_spectrum_t eta;
				/// @brief The conductor's exctinction coefficient.
				tpbrt_spectrum_t k;
			} conductor;

			/// @brief As an alternative to specifying the conductor's index of refraction and extinction coefficient, its average spectral reflectance can be specified instead; in this case plausible values for the index of refraction and extinction cofficient are found for rendering.
			tpbrt_spectrum_t reflectance;
		} as;

		/// @brief Conductor roughness parameters.
		tpbrt_material_roughness_params_t conductor_roughness_params;
		/// @brief Interface roughness parameters.
		tpbrt_material_roughness_params_t interface_roughness_params;
		/// @brief Thickness of the medium between the two layers.
		tpbrt_float_t thickness;
		/// @brief Conductor's value type.
		tpbrt_material_conductor_value_type_t value_type : 3;
	} tpbrt_material_coated_conductor_params_t;

	/**
	 * @brief Conductor material parameters.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_conductor_params_t {
		/// @brief Bump map parameters.
		tpbrt_material_bump_map_params_t bump_map;

		/// @brief Value dependent on value_type value.
		union {
			/// @brief Holds conductor 'eta' and 'k' values.
			struct {
				/// @brief The wavelength-dependent index of refraction of the conductor.
				tpbrt_texture_handle_t eta;
				/// @brief The conductor's wavelength-dependent absorption coefficient.
				tpbrt_texture_handle_t k;
			} conductor;

			/// @brief Average reflectance of the conductor. Plausible values of "eta" and "k" are computed based on the reflectance if it is provided.
			tpbrt_texture_handle_t reflectance;
		} as;

		/// @brief Roughness parameters.
		tpbrt_material_roughness_params_t roughness_params;
		/// @brief Conductor's value type.
		tpbrt_material_conductor_value_type_t value_type : 3;
	} tpbrt_material_conductor_params_t;

	/**
	 * @brief Dielectric material parameters.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_dielectric_params_t {
		/// @brief Bump map parameters.
		tpbrt_material_bump_map_params_t bump_map;
		/// @brief Index of refraction of the medium inside the surface or Wavelength-dependent index of refraction of the medium inside the surface. (Corresponding data for a variety of media are built in to pbrt; see also the refractiveindex.info website.
		tpbrt_texture_handle_t eta;
		/// @brief Roughness parameters.
		tpbrt_material_roughness_params_t roughness_params;
	} tpbrt_material_dielectric_params_t;

	/**
	 * @brief Diffuse material parameters.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_diffuse_params_t {
		/// @brief Bump map parameters.
		tpbrt_material_bump_map_params_t bump_map;
		/// @brief The reflectivity of the surface. Must be between 0 and 1.
		tpbrt_texture_handle_t reflectance;
	} tpbrt_material_diffuse_params_t;

	/**
	 * @brief Diffuse transmission material parameters
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_diffuse_transmission_params_t {
		/// @brief Bump map parameters.
		tpbrt_material_bump_map_params_t bump_map;
		/// @brief The reflectivity of the surface.
		tpbrt_texture_handle_t reflectance;
		/// @brief The transmissivity of the surface.
		tpbrt_texture_handle_t transmittance;
		/// @brief A scale factor that is applied to both the reflectance and transmittance values.
		tpbrt_texture_handle_t scale;
	} tpbrt_material_diffuse_transmission_params_t;

	/**
	 * @brief Hair material parameters.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_hair_params_t {
		/// @brief Bump map parameters.
		tpbrt_material_bump_map_params_t bump_map;
		/// @brief Absorption coefficient of the medium inside the hair. This absorption coefficient is normalized such that the value provided should be with respect to the diameter of the hair.
		tpbrt_texture_handle_t sigma_a;
		/// @brief If specified, a value of the absorption coefficient is computed using an approximation that leads to the hair having roughly this reflectance, after multiple scattering in the hair during rendering.
		tpbrt_texture_handle_t reflectance;
		/// @brief Concentration of the eumelanin pigment in the hair. Blonde hair has concentrations around 0.3, brown around 1.3, and black around 8.
		tpbrt_texture_handle_t eumelanin;
		/// @brief Concentration of the pheomelanin pigment in the hair. Pheomelanin makes red hair red.
		tpbrt_texture_handle_t pheomelanin;
		/// @brief Index of refraction of the hair medium.
		tpbrt_texture_handle_t eta;
		/// @brief Longitudinal roughness of the hair. Should be between 0 and 1. This roughness controls the size and shape of the hair highlight.
		tpbrt_texture_handle_t beta_m;
		/// @brief Azimuthal roughness of the hair. Should be between 0 and 1.
		tpbrt_texture_handle_t beta_n;
		/// @brief Angle of scales on the hair surface, in degrees.
		tpbrt_texture_handle_t alpha;
	} tpbrt_material_hair_params_t;

	/**
	 * @brief Measured material parameters.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_measured_params_t {
		/// @brief Bump map parameters.
		tpbrt_material_bump_map_params_t bump_map;
		/// @brief Name of file storing measured BRDF data.
		tpbrt_string_t file_name;
	} tpbrt_material_measured_params_t;

	/**
	 * @brief Mix material parameters.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_mix_params_t {
		/// @brief Handles of two materials to mix between.
		tpbrt_material_handle_t materials[2];
		/// @brief Weighting factor for the blend between materials. A value of zero corresponds to just the first material, a value of one corresponds to just the second, and values in between interpolate linearly.
		tpbrt_texture_handle_t amount;
	} tpbrt_material_mix_params_t;

	/**
	 * @brief Subsurface material scattering value type.
	 * @ingroup material-public
	 */
	typedef enum tpbrt_material_subsurface_scattering_value_t {
		/// @brief Uses sigma_s and sigma_a values.
		TPBRT_MATERIAL_SUBSURFACE_SCATTERING_VALUE_SIGMA   = 0,
		/// @brief Uses builtin as sigma_s and sigma_a values.
		TPBRT_MATERIAL_SUBSURFACE_SCATTERING_VALUE_BUILTIN = 1,
		/// @brief Count of available media scattering values.
		TPBRT_MATERIAL_SUBSURFACE_SCATTERING_VALUE_MAX_NUM = 2,
	} tpbrt_material_subsurface_scattering_value_t;

	/**
	 * @brief Subsurface material scattering.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_subsurface_scattering_t {
		/// @brief Scattering value dependent on type value.
		union {
			struct {
				/// @brief Absorption coefficient of the volume, measured in mm^-1.
				tpbrt_texture_handle_t a;
				/// @brief Scattering coefficient of the volume, measured in mm^-1.
				tpbrt_texture_handle_t s;
			} sigma;

			/// @brief Name of measured subsurface scattering coefficients.
			tpbrt_scattering_builtin_t builtin;
		} as;

		/// @brief Scattering value type.
		tpbrt_material_subsurface_scattering_value_t type : 3;
	} tpbrt_material_subsurface_scattering_t;

	/**
	 * @brief Subsurface material parameters.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_subsurface_params_t {
		/// @brief Bump map parameters.
		tpbrt_material_bump_map_params_t bump_map;
		/// @brief Index of refraction of the scattering volume. Note that wavelength-dependent indices of refraction are not supported in the current implementation.
		tpbrt_texture_handle_t eta;
		/// @brief Henyey-Greenstein asymmetry parameter, which ranges from -1 to 1, going from completely backwards-scattering to completely forward-scattering.
		tpbrt_texture_handle_t g;
		/// @brief Mean free path of light in the volume, specified in meters. (Used only if "reflectance" is specified.)
		tpbrt_texture_handle_t mfp;
		/// @brief Scattering parameters.
		tpbrt_material_subsurface_scattering_t scattering;
		/// @brief Average reflectance of the volume. If specified, absorption and scattering coefficients are computed from this value and the specified mean free path.
		tpbrt_texture_handle_t reflectance;
		/// @brief Roughness parameters.
		tpbrt_material_roughness_params_t roughness_params;
		/// @brief Scale factor that is applied to sigma_a and sigma_s. This is particularly useful when the scene is not measured in mm and the coefficients need to be scaled accordingly. For example, if the scene is modeled in meters, then a scale factor of 0.001 would be appropriate.
		tpbrt_float_t scale;
	} tpbrt_material_subsurface_params_t;

	/**
	 * @brief Thin dielectric material parameters.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_thin_dielectric_params_t {
		/// @brief Bump map parameters.
		tpbrt_material_bump_map_params_t bump_map;
		/// @brief Index of refraction of the medium inside the surface or Wavelength-dependent index of refraction of the medium inside the surface. (Corresponding data for a variety of media are built in to pbrt; see also the refractiveindex.info website.
		tpbrt_texture_handle_t eta;
	} tpbrt_material_thin_dielectric_params_t;

	/**
	 * @brief PBRT material representation.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_material_t {
		/// @brief Material index.
		tpbrt_size_t idx;
		/// @brief Material name (if it is not named material name is empty).
		tpbrt_string_t name;

		/// @brief Value dependent on type value.
		union {
			/// @brief Coated diffuse material parameters.
			tpbrt_material_coated_diffuse_params_t coated_diffuse;
			/// @brief Coated conductor material parameters.
			tpbrt_material_coated_conductor_params_t coated_conductor;
			/// @brief Conductor material parameters.
			tpbrt_material_conductor_params_t conductor;
			/// @brief Dielectric material parameters.
			tpbrt_material_dielectric_params_t dielectric;
			/// @brief Diffuse material parameters.
			tpbrt_material_diffuse_params_t diffuse;
			/// @brief Diffuse transmission material parameters.
			tpbrt_material_diffuse_transmission_params_t diffuse_transmission;
			/// @brief Hair material parameters.
			tpbrt_material_hair_params_t hair;
			/// @brief Measured material parameters.
			tpbrt_material_measured_params_t measured;
			/// @brief Mix material parameters.
			tpbrt_material_mix_params_t mix;
			/// @brief Subsurface material parameters.
			tpbrt_material_subsurface_params_t subsurface;
			/// @brief Thin dielectric material parameters.
			tpbrt_material_thin_dielectric_params_t thin_dielectric;
		} as;

		/// @brief Material value type.
		tpbrt_material_type_t type : 5;
	} tpbrt_material_t;

	/**
	 * @brief List of materials.
	 * @ingroup material-public
	 */
	typedef struct tpbrt_materials_list_t {
		/// @brief Materials list data.
		tpbrt_material_t* materials;
		/// @brief Number of materials in list.
		tpbrt_size_t count;
		/// @brief Names indexes.
		tpbrt_name_index_t names;
	} tpbrt_materials_list_t;

	/**
	 * @brief Returns pointer to material with given name.
	 * @param materials List of materials in which searched material should be.
	 * @param name Name of the searched material.
	 * @param material Pointer in which pointer to material will be stored (if not found this will be TPBRT_NULL).
	 * @return Returns error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup material-public
	 */
	TPBRT_API tpbrt_error_t tpbrt_get_material_by_name(const tpbrt_materials_list_t* materials, tpbrt_string_t name,
	  const tpbrt_material_t** material);
	/**
	 * @brief Returns pointer to material to which handle is provided.
	 * @param materials List of materials in which material given by handle should be.
	 * @param handle Handle to material.
	 * @param material Pointer in which pointer to material will be stored (if not found this will be TPBRT_NULL).
	 * @return Returns error code (most possible TPBRT_ERROR_NOT_FOUND, TPBRT_ERROR_NONE).
	 * @ingroup material-public
	 */
	TPBRT_API tpbrt_error_t tpbrt_get_material_by_handle(const tpbrt_materials_list_t* materials, tpbrt_material_handle_t handle,
	  const tpbrt_material_t** material);

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_MATERIAL_H_
