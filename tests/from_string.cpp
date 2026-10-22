#include <pch.hpp>

#include <from_string_helpers.hpp>

using namespace tpbrt_test;

TEST(from_string, accelerator_type) {
	static constexpr FromStringCase<tpbrt_accelerator_type_t> CASES[] = {
		{ .text = "bvh",	 .value = TPBRT_ACCELERATOR_TYPE_BVH	 },
		{ .text = "kdtree", .value = TPBRT_ACCELERATOR_TYPE_KD_TREE },
	};
	check_from_string<tpbrt_accelerator_type_t>(tpbrt_accelerator_type_from_string, CASES, TPBRT_ERROR_INVALID_ACCELERATOR_TYPE,
	  TPBRT_ACCELERATOR_TYPE_MAX_NUM);
}

TEST(from_string, accelerator_bvh_split_method) {
	static constexpr FromStringCase<tpbrt_accelerator_bvh_split_method_t> CASES[] = {
		{ .text = "sah",	 .value = TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_SAH	 },
		{ .text = "middle", .value = TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MIDDLE },
		{ .text = "equal",  .value = TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_EQUAL	 },
		{ .text = "hlbvh",  .value = TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_HLBVH	 },
	};
	check_from_string<tpbrt_accelerator_bvh_split_method_t>(tpbrt_accelerator_bvh_split_method_from_string, CASES,
	  TPBRT_ERROR_INVALID_ACCELERATOR_BVH_SPLIT_METHOD, TPBRT_ACCELERATOR_BVH_SPLIT_METHOD_MAX_NUM);
}

TEST(from_string, camera_type) {
	static constexpr FromStringCase<tpbrt_camera_type_t> CASES[] = {
		{ .text = "orthographic", .value = TPBRT_CAMERA_TYPE_ORTHOGRAPHIC },
		{ .text = "perspective",	 .value = TPBRT_CAMERA_TYPE_PERSPECTIVE	},
		{ .text = "realistic",	   .value = TPBRT_CAMERA_TYPE_REALISTIC	},
		{ .text = "spherical",	   .value = TPBRT_CAMERA_TYPE_SPHERICAL	},
	};
	check_from_string<tpbrt_camera_type_t>(tpbrt_camera_type_from_string, CASES, TPBRT_ERROR_INVALID_CAMERA_TYPE,
	  TPBRT_CAMERA_TYPE_MAX_NUM);
}

TEST(from_string, camera_spherical_mapping) {
	static constexpr FromStringCase<tpbrt_camera_spherical_mapping_t> CASES[] = {
		{ .text = "equalarea",	   .value = TPBRT_CAMERA_SPHERICAL_MAPPING_EQUAL_AREA		  },
		{ .text = "equirectangular", .value = TPBRT_CAMERA_SPHERICAL_MAPPING_EQUI_RECTANGULAR },
	};
	check_from_string<tpbrt_camera_spherical_mapping_t>(tpbrt_camera_spherical_mapping_from_string, CASES,
	  TPBRT_ERROR_INVALID_CAMERA_SPHERICAL_MAPPING, TPBRT_CAMERA_SPHERICAL_MAPPING_MAX_NUM);
}

TEST(from_string, camera_realistic_aperture_builtin) {
	static constexpr FromStringCase<tpbrt_camera_realistic_aperture_builtin_t> CASES[] = {
		{ .text = "circular", .value = TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_CIRCULAR },
		{ .text = "gaussian", .value = TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_GAUSSIAN },
		{ .text = "square",	.value = TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_SQUARE	},
		{ .text = "pentagon", .value = TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_PENTAGON },
		{ .text = "star",	  .value = TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_STAR		},
	};
	check_from_string<tpbrt_camera_realistic_aperture_builtin_t>(tpbrt_camera_realistic_aperture_builtin_from_string, CASES,
	  TPBRT_ERROR_INVALID_CAMERA_REALISTIC_APERTURE_BUILTIN, TPBRT_CAMERA_REALISTIC_APERTURE_BUILTIN_MAX_NUM);
}

TEST(from_string, color_space) {
	static constexpr FromStringCase<tpbrt_color_space_t> CASES[] = {
		{ .text = "aces2065-1", .value = TPBRT_COLOR_SPACE_ACES_2065_1 },
		{ .text = "rec2020",	 .value = TPBRT_COLOR_SPACE_REC_2020	 },
		{ .text = "dci-p3",		.value = TPBRT_COLOR_SPACE_DCI_P3	  },
		{ .text = "srgb",		  .value = TPBRT_COLOR_SPACE_SRGB		  },
	};
	check_from_string<tpbrt_color_space_t>(tpbrt_color_space_from_string, CASES, TPBRT_ERROR_INVALID_COLOR_SPACE,
	  TPBRT_COLOR_SPACE_MAX_NUM);
}

TEST(from_string, spectrum_builtin) {
	static constexpr FromStringCase<tpbrt_spectrum_builtin_t> CASES[] = {
		{ .text = "glass-BK7",	   .value = TPBRT_SPECTRUM_BUILTIN_GLASS_BK7		 },
		{ .text = "glass-BAF10",	 .value = TPBRT_SPECTRUM_BUILTIN_GLASS_BAF10	 },
		{ .text = "glass-FK51A",	 .value = TPBRT_SPECTRUM_BUILTIN_GLASS_FK51A	 },
		{ .text = "glass-LASF9",	 .value = TPBRT_SPECTRUM_BUILTIN_GLASS_LASF9	 },
		{ .text = "glass-F5",		  .value = TPBRT_SPECTRUM_BUILTIN_GLASS_F5		   },
		{ .text = "glass-F10",	   .value = TPBRT_SPECTRUM_BUILTIN_GLASS_F10		 },
		{ .text = "glass-F11",	   .value = TPBRT_SPECTRUM_BUILTIN_GLASS_F11		 },
		{ .text = "metal-Ag-eta",	  .value = TPBRT_SPECTRUM_BUILTIN_METAL_AG_ETA	   },
		{ .text = "metal-Ag-k",		.value = TPBRT_SPECTRUM_BUILTIN_METAL_AG_K	   },
		{ .text = "metal-Al-eta",	  .value = TPBRT_SPECTRUM_BUILTIN_METAL_AL_ETA	   },
		{ .text = "metal-Al-k",		.value = TPBRT_SPECTRUM_BUILTIN_METAL_AL_K	   },
		{ .text = "metal-Au-eta",	  .value = TPBRT_SPECTRUM_BUILTIN_METAL_AU_ETA	   },
		{ .text = "metal-Au-k",		.value = TPBRT_SPECTRUM_BUILTIN_METAL_AU_K	   },
		{ .text = "metal-Cu-eta",	  .value = TPBRT_SPECTRUM_BUILTIN_METAL_CU_ETA	   },
		{ .text = "metal-Cu-k",		.value = TPBRT_SPECTRUM_BUILTIN_METAL_CU_K	   },
		{ .text = "metal-CuZn-eta", .value = TPBRT_SPECTRUM_BUILTIN_METAL_CU_ZN_ETA },
		{ .text = "metal-CuZn-k",	  .value = TPBRT_SPECTRUM_BUILTIN_METAL_CU_ZN_K	},
		{ .text = "metal-MgO-eta",  .value = TPBRT_SPECTRUM_BUILTIN_METAL_MG_O_ETA  },
		{ .text = "metal-MgO-k",	 .value = TPBRT_SPECTRUM_BUILTIN_METAL_MG_O_K	  },
		{ .text = "metal-TiO2-eta", .value = TPBRT_SPECTRUM_BUILTIN_METAL_TI_O2_ETA },
		{ .text = "metal-TiO2-k",	  .value = TPBRT_SPECTRUM_BUILTIN_METAL_TI_O2_K	},
		{ .text = "stdillum-A",		.value = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_A		},
		{ .text = "stdillum-D50",	  .value = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D50	},
		{ .text = "stdillum-D65",	  .value = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_D65	},
		{ .text = "stdillum-F1",	 .value = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F1	  },
		{ .text = "stdillum-F2",	 .value = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F2	  },
		{ .text = "stdillum-F3",	 .value = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F3	  },
		{ .text = "stdillum-F4",	 .value = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F4	  },
		{ .text = "stdillum-F5",	 .value = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F5	  },
		{ .text = "stdillum-F6",	 .value = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F6	  },
		{ .text = "stdillum-F7",	 .value = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F7	  },
		{ .text = "stdillum-F8",	 .value = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F8	  },
		{ .text = "stdillum-F9",	 .value = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F9	  },
		{ .text = "stdillum-F10",	  .value = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F10	},
		{ .text = "stdillum-F11",	  .value = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F11	},
		{ .text = "stdillum-F12",	  .value = TPBRT_SPECTRUM_BUILTIN_STD_ILLUM_F12	},
		{ .text = "illum-acesD60",  .value = TPBRT_SPECTRUM_BUILTIN_ILLUM_ACES_D60  },
	};
	check_from_string<tpbrt_spectrum_builtin_t>(tpbrt_spectrum_builtin_from_string, CASES, TPBRT_ERROR_INVALID_SPECTRUM_BUILTIN,
	  TPBRT_SPECTRUM_BUILTIN_MAX_NUM);
}

TEST(from_string, coord_system_type) {
	static constexpr FromStringCase<tpbrt_coord_system_type_t> CASES[] = {
		{ .text = "cameraworld", .value = TPBRT_COORD_SYSTEM_TYPE_CAMERA_WORLD },
		{ .text = "camera",		.value = TPBRT_COORD_SYSTEM_TYPE_CAMERA		},
		{ .text = "world",	   .value = TPBRT_COORD_SYSTEM_TYPE_WORLD		  },
	};
	check_from_string<tpbrt_coord_system_type_t>(tpbrt_coord_system_type_from_string, CASES,
	  TPBRT_ERROR_INVALID_COORD_SYSTEM_TYPE, TPBRT_COORD_SYSTEM_TYPE_MAX_NUM);
}

TEST(from_string, film_type) {
	static constexpr FromStringCase<tpbrt_film_type_t> CASES[] = {
		{ .text = "rgb",		 .value = TPBRT_FILM_TYPE_RGB	  },
		{ .text = "gbuffer",	 .value = TPBRT_FILM_TYPE_GBUFFER  },
		{ .text = "spectral", .value = TPBRT_FILM_TYPE_SPECTRAL },
	};
	check_from_string<tpbrt_film_type_t>(tpbrt_film_type_from_string, CASES, TPBRT_ERROR_INVALID_FILM_TYPE,
	  TPBRT_FILM_TYPE_MAX_NUM);
}

TEST(from_string, film_sensor) {
	static constexpr FromStringCase<tpbrt_film_sensor_t> CASES[] = {
		{ .text = "cie1931",			 .value = TPBRT_FILM_SENSOR_CIE_1931			 },
		{ .text = "canon_eos_100d",		.value = TPBRT_FILM_SENSOR_CANON_EOS_100D	  },
		{ .text = "canon_eos_1dx_mkii",	.value = TPBRT_FILM_SENSOR_CANON_EOS_1DX_MKII  },
		{ .text = "canon_eos_200d",		.value = TPBRT_FILM_SENSOR_CANON_EOS_200D	  },
		{ .text = "canon_eos_200d_mkii", .value = TPBRT_FILM_SENSOR_CANON_EOS_200D_MKII },
		{ .text = "canon_eos_5d",		  .value = TPBRT_FILM_SENSOR_CANON_EOS_5D		  },
		{ .text = "canon_eos_5d_mkii",   .value = TPBRT_FILM_SENSOR_CANON_EOS_5D_MKII	},
		{ .text = "canon_eos_5d_mkiii",	.value = TPBRT_FILM_SENSOR_CANON_EOS_5D_MKIII  },
		{ .text = "canon_eos_5d_mkiv",   .value = TPBRT_FILM_SENSOR_CANON_EOS_5D_MKIV	},
		{ .text = "canon_eos_5ds",	   .value = TPBRT_FILM_SENSOR_CANON_EOS_5DS		},
		{ .text = "canon_eos_m",		 .value = TPBRT_FILM_SENSOR_CANON_EOS_M			},
		{ .text = "hasselblad_l1d_20c",	.value = TPBRT_FILM_SENSOR_HASSELBLAD_L1D_20C  },
		{ .text = "nikon_d810",			.value = TPBRT_FILM_SENSOR_NIKON_D810		  },
		{ .text = "nikon_d850",			.value = TPBRT_FILM_SENSOR_NIKON_D850		  },
		{ .text = "sony_ilce_6400",		.value = TPBRT_FILM_SENSOR_SONY_ILCE_6400	  },
		{ .text = "sony_ilce_7m3",	   .value = TPBRT_FILM_SENSOR_SONY_ILCE_7M3		},
		{ .text = "sony_ilce_7rm3",		.value = TPBRT_FILM_SENSOR_SONY_ILCE_7RM3	  },
		{ .text = "sony_ilce_9",		 .value = TPBRT_FILM_SENSOR_SONY_ILCE_9			},
	};
	check_from_string<tpbrt_film_sensor_t>(tpbrt_film_sensor_from_string, CASES, TPBRT_ERROR_INVALID_FILM_SENSOR,
	  TPBRT_FILM_SENSOR_MAX_NUM);
}

TEST(from_string, filter_type) {
	static constexpr FromStringCase<tpbrt_filter_type_t> CASES[] = {
		{ .text = "box",		 .value = TPBRT_FILTER_TYPE_BOX		},
		{ .text = "gaussian", .value = TPBRT_FILTER_TYPE_GAUSSIAN },
		{ .text = "mitchell", .value = TPBRT_FILTER_TYPE_MITCHELL },
		{ .text = "sinc",	  .value = TPBRT_FILTER_TYPE_SINC	  },
		{ .text = "triangle", .value = TPBRT_FILTER_TYPE_TRIANGLE },
	};
	check_from_string<tpbrt_filter_type_t>(tpbrt_filter_type_from_string, CASES, TPBRT_ERROR_INVALID_FILTER_TYPE,
	  TPBRT_FILTER_TYPE_MAX_NUM);
}

TEST(from_string, integrator_type) {
	static constexpr FromStringCase<tpbrt_integrator_type_t> CASES[] = {
		{ .text = "ambientocclusion", .value = TPBRT_INTEGRATOR_TYPE_AMBIENT_OCCLUSION },
		{ .text = "bdpt",			  .value = TPBRT_INTEGRATOR_TYPE_BDPT			  },
		{ .text = "lightpath",		   .value = TPBRT_INTEGRATOR_TYPE_LIGHT_PATH		 },
		{ .text = "mlt",				 .value = TPBRT_INTEGRATOR_TYPE_MLT				},
		{ .text = "path",			  .value = TPBRT_INTEGRATOR_TYPE_PATH			  },
		{ .text = "randomwalk",		.value = TPBRT_INTEGRATOR_TYPE_RANDOM_WALK	   },
		{ .text = "simplepath",		.value = TPBRT_INTEGRATOR_TYPE_SIMPLE_PATH	   },
		{ .text = "simplevolpath",	   .value = TPBRT_INTEGRATOR_TYPE_SIMPLE_VOL_PATH	  },
		{ .text = "sppm",			  .value = TPBRT_INTEGRATOR_TYPE_SPPM			  },
		{ .text = "volpath",			 .value = TPBRT_INTEGRATOR_TYPE_VOL_PATH			 },
	};
	check_from_string<tpbrt_integrator_type_t>(tpbrt_integrator_type_from_string, CASES, TPBRT_ERROR_INVALID_INTEGRATOR_TYPE,
	  TPBRT_INTEGRATOR_TYPE_MAX_NUM);
}

TEST(from_string, integrator_light_sampler) {
	static constexpr FromStringCase<tpbrt_integrator_light_sampler_t> CASES[] = {
		{ .text = "bvh",	 .value = TPBRT_INTEGRATOR_LIGHT_SAMPLER_BVH	 },
		{ .text = "uniform", .value = TPBRT_INTEGRATOR_LIGHT_SAMPLER_UNIFORM },
		{ .text = "power",   .value = TPBRT_INTEGRATOR_LIGHT_SAMPLER_POWER	 },
	};
	check_from_string<tpbrt_integrator_light_sampler_t>(tpbrt_integrator_light_sampler_from_string, CASES,
	  TPBRT_ERROR_INVALID_INTEGRATOR_LIGHT_SAMPLER, TPBRT_INTEGRATOR_LIGHT_SAMPLER_MAX_NUM);
}

TEST(from_string, light_source_type) {
	static constexpr FromStringCase<tpbrt_light_source_type_t> CASES[] = {
		{ .text = "distant",	 .value = TPBRT_LIGHT_SOURCE_TYPE_DISTANT	  },
		{ .text = "goniometric", .value = TPBRT_LIGHT_SOURCE_TYPE_GONIOMETRIC },
		{ .text = "infinite",	  .value = TPBRT_LIGHT_SOURCE_TYPE_INFINITE	},
		{ .text = "point",	   .value = TPBRT_LIGHT_SOURCE_TYPE_POINT		  },
		{ .text = "projection",	.value = TPBRT_LIGHT_SOURCE_TYPE_PROJECTION	},
		{ .text = "spot",		  .value = TPBRT_LIGHT_SOURCE_TYPE_SPOT		},
	};
	check_from_string<tpbrt_light_source_type_t>(tpbrt_light_source_type_from_string, CASES,
	  TPBRT_ERROR_INVALID_LIGHT_SOURCE_TYPE, TPBRT_LIGHT_SOURCE_TYPE_MAX_NUM);
}

TEST(from_string, area_light_type) {
	static constexpr FromStringCase<tpbrt_area_light_type_t> CASES[] = {
		{ .text = "diffuse", .value = TPBRT_AREA_LIGHT_TYPE_DIFFUSE },
	};
	check_from_string<tpbrt_area_light_type_t>(tpbrt_area_light_type_from_string, CASES, TPBRT_ERROR_INVALID_AREA_LIGHT_TYPE,
	  TPBRT_AREA_LIGHT_TYPE_MAX_NUM);
}

TEST(from_string, material_type) {
	static constexpr FromStringCase<tpbrt_material_type_t> CASES[] = {
		{ .text = "coateddiffuse",	   .value = TPBRT_MATERIAL_TYPE_COATED_DIFFUSE	   },
		{ .text = "coatedconductor",	 .value = TPBRT_MATERIAL_TYPE_COATED_CONDUCTOR	   },
		{ .text = "conductor",		   .value = TPBRT_MATERIAL_TYPE_CONDUCTOR			  },
		{ .text = "dielectric",			.value = TPBRT_MATERIAL_TYPE_DIELECTRIC			},
		{ .text = "diffuse",			 .value = TPBRT_MATERIAL_TYPE_DIFFUSE			  },
		{ .text = "diffusetransmission", .value = TPBRT_MATERIAL_TYPE_DIFFUSE_TRANSMISSION },
		{ .text = "hair",				  .value = TPBRT_MATERIAL_TYPE_HAIR					},
		{ .text = "interface",		   .value = TPBRT_MATERIAL_TYPE_INTERFACE			  },
		{ .text = "measured",			  .value = TPBRT_MATERIAL_TYPE_MEASURED				},
		{ .text = "mix",				 .value = TPBRT_MATERIAL_TYPE_MIX				  },
		{ .text = "subsurface",			.value = TPBRT_MATERIAL_TYPE_SUBSURFACE			},
		{ .text = "thindielectric",		.value = TPBRT_MATERIAL_TYPE_THIN_DIELECTRIC		 },
	};
	check_from_string<tpbrt_material_type_t>(tpbrt_material_type_from_string, CASES, TPBRT_ERROR_INVALID_MATERIAL_TYPE,
	  TPBRT_MATERIAL_TYPE_MAX_NUM);
}

TEST(from_string, media_type) {
	static constexpr FromStringCase<tpbrt_media_type_t> CASES[] = {
		{ .text = "cloud",	   .value = TPBRT_MEDIA_TYPE_CLOUD		   },
		{ .text = "homogeneous", .value = TPBRT_MEDIA_TYPE_HOMOGENEOUS  },
		{ .text = "nanovdb",	 .value = TPBRT_MEDIA_TYPE_NANO_VDB		},
		{ .text = "rgbgrid",	 .value = TPBRT_MEDIA_TYPE_RGB_GRID		},
		{ .text = "uniformgrid", .value = TPBRT_MEDIA_TYPE_UNIFORM_GRID },
	};
	check_from_string<tpbrt_media_type_t>(tpbrt_media_type_from_string, CASES, TPBRT_ERROR_INVALID_MEDIA_TYPE,
	  TPBRT_MEDIA_TYPE_MAX_NUM);
}

TEST(from_string, param_type) {
	static constexpr FromStringCase<tpbrt_param_type_t> CASES[] = {
		{ .text = "integer",	 .value = TPBRT_PARAM_TYPE_INTEGER   },
		{ .text = "float",	   .value = TPBRT_PARAM_TYPE_FLOAT	   },
		{ .text = "point2",	.value = TPBRT_PARAM_TYPE_POINT2	 },
		{ .text = "vector2",	 .value = TPBRT_PARAM_TYPE_VECTOR2   },
		{ .text = "point3",	.value = TPBRT_PARAM_TYPE_POINT3	 },
		{ .text = "point",	   .value = TPBRT_PARAM_TYPE_POINT	   },
		{ .text = "vector3",	 .value = TPBRT_PARAM_TYPE_VECTOR3   },
		{ .text = "vector",	.value = TPBRT_PARAM_TYPE_VECTOR	 },
		{ .text = "normal3",	 .value = TPBRT_PARAM_TYPE_NORMAL3   },
		{ .text = "normal",	.value = TPBRT_PARAM_TYPE_NORMAL	 },
		{ .text = "spectrum",  .value = TPBRT_PARAM_TYPE_SPECTRUM	 },
		{ .text = "rgb",		 .value = TPBRT_PARAM_TYPE_RGB	   },
		{ .text = "blackbody", .value = TPBRT_PARAM_TYPE_BLACKBODY },
		{ .text = "bool",	  .value = TPBRT_PARAM_TYPE_BOOL		 },
		{ .text = "string",	.value = TPBRT_PARAM_TYPE_STRING	 },
		{ .text = "texture",	 .value = TPBRT_PARAM_TYPE_TEXTURE   },
	};
	check_from_string<tpbrt_param_type_t>(tpbrt_param_type_from_string, CASES, TPBRT_ERROR_INVALID_PARAM_TYPE,
	  TPBRT_PARAM_TYPE_MAX_NUM);
}

TEST(from_string, sampler_type) {
	static constexpr FromStringCase<tpbrt_sampler_type_t> CASES[] = {
		{ .text = "halton",		.value = TPBRT_SAMPLER_TYPE_HALTON	   },
		{ .text = "independent", .value = TPBRT_SAMPLER_TYPE_INDEPENDENT	 },
		{ .text = "paddedsobol", .value = TPBRT_SAMPLER_TYPE_PADDED_SOBOL },
		{ .text = "sobol",	   .value = TPBRT_SAMPLER_TYPE_SOBOL		 },
		{ .text = "stratified",	.value = TPBRT_SAMPLER_TYPE_STRATIFIED   },
		{ .text = "zsobol",		.value = TPBRT_SAMPLER_TYPE_Z_SOBOL		},
	};
	check_from_string<tpbrt_sampler_type_t>(tpbrt_sampler_type_from_string, CASES, TPBRT_ERROR_INVALID_SAMPLER_TYPE,
	  TPBRT_SAMPLER_TYPE_MAX_NUM);
}

TEST(from_string, sampler_random) {
	static constexpr FromStringCase<tpbrt_sampler_random_t> CASES[] = {
		{ .text = "none",		  .value = TPBRT_SAMPLER_RANDOM_NONE			 },
		{ .text = "permutedigits", .value = TPBRT_SAMPLER_RANDOM_PERMUTE_DIGITS },
		{ .text = "owen",		  .value = TPBRT_SAMPLER_RANDOM_OWEN			 },
		{ .text = "fastowen",	  .value = TPBRT_SAMPLER_RANDOM_FAST_OWEN	  },
	};
	check_from_string<tpbrt_sampler_random_t>(tpbrt_sampler_random_from_string, CASES, TPBRT_ERROR_INVALID_SAMPLER_RANDOM,
	  TPBRT_SAMPLER_RANDOM_MAX_NUM);
}

TEST(from_string, shape_type) {
	static constexpr FromStringCase<tpbrt_shape_type_t> CASES[] = {
		{ .text = "bilinearmesh", .value = TPBRT_SHAPE_TYPE_BILINEAR_MESH },
		{ .text = "curve",		   .value = TPBRT_SHAPE_TYPE_CURVE		   },
		{ .text = "cylinder",	  .value = TPBRT_SHAPE_TYPE_CYLINDER		 },
		{ .text = "disk",		  .value = TPBRT_SHAPE_TYPE_DISK			 },
		{ .text = "sphere",		.value = TPBRT_SHAPE_TYPE_SPHERE		 },
		{ .text = "trianglemesh", .value = TPBRT_SHAPE_TYPE_TRIANGLE_MESH },
		{ .text = "plymesh",		 .value = TPBRT_SHAPE_TYPE_PLY_MESH		},
		{ .text = "loopsubdiv",	.value = TPBRT_SHAPE_TYPE_LOOP_SUBDIV	  },
	};
	check_from_string<tpbrt_shape_type_t>(tpbrt_shape_type_from_string, CASES, TPBRT_ERROR_INVALID_SHAPE_TYPE,
	  TPBRT_SHAPE_TYPE_MAX_NUM);
}

TEST(from_string, shape_curve_basis) {
	static constexpr FromStringCase<tpbrt_shape_curve_basis_t> CASES[] = {
		{ .text = "bezier",	.value = TPBRT_SHAPE_CURVE_BASIS_BEZIER	},
		{ .text = "bspline", .value = TPBRT_SHAPE_CURVE_BASIS_BSPLINE },
	};
	check_from_string<tpbrt_shape_curve_basis_t>(tpbrt_shape_curve_basis_from_string, CASES,
	  TPBRT_ERROR_INVALID_SHAPE_CURVE_BASIS, TPBRT_SHAPE_CURVE_BASIS_MAX_NUM);
}

TEST(from_string, shape_curve_type) {
	static constexpr FromStringCase<tpbrt_shape_curve_type_t> CASES[] = {
		{ .text = "flat",	  .value = TPBRT_SHAPE_CURVE_TYPE_FLAT	   },
		{ .text = "cylinder", .value = TPBRT_SHAPE_CURVE_TYPE_CYLINDER },
		{ .text = "ribbon",	.value = TPBRT_SHAPE_CURVE_TYPE_RIBBON   },
	};
	check_from_string<tpbrt_shape_curve_type_t>(tpbrt_shape_curve_type_from_string, CASES, TPBRT_ERROR_INVALID_SHAPE_CURVE_TYPE,
	  TPBRT_SHAPE_CURVE_TYPE_MAX_NUM);
}

TEST(from_string, texture_type) {
	static constexpr FromStringCase<tpbrt_texture_type_t> CASES[] = {
		{ .text = "float",	   .value = TPBRT_TEXTURE_TYPE_FLOAT	 },
		{ .text = "spectrum", .value = TPBRT_TEXTURE_TYPE_SPECTRUM },
	};
	check_from_string<tpbrt_texture_type_t>(tpbrt_texture_type_from_string, CASES, TPBRT_ERROR_INVALID_TEXTURE_TYPE,
	  TPBRT_TEXTURE_TYPE_MAX_NUM);
}

TEST(from_string, texture_class) {
	static constexpr FromStringCase<tpbrt_texture_class_t> CASES[] = {
		{ .text = "bilerp",		.value = TPBRT_TEXTURE_CLASS_BILERP		},
		{ .text = "checkerboard", .value = TPBRT_TEXTURE_CLASS_CHECKER_BOARD },
		{ .text = "constant",	  .value = TPBRT_TEXTURE_CLASS_CONSTANT		},
		{ .text = "directionmix", .value = TPBRT_TEXTURE_CLASS_DIRECTION_MIX },
		{ .text = "dots",		  .value = TPBRT_TEXTURE_CLASS_DOTS			},
		{ .text = "fbm",			 .value = TPBRT_TEXTURE_CLASS_FBM			  },
		{ .text = "imagemap",	  .value = TPBRT_TEXTURE_CLASS_IMAGE_MAP	 },
		{ .text = "marble",		.value = TPBRT_TEXTURE_CLASS_MARBLE		},
		{ .text = "mix",			 .value = TPBRT_TEXTURE_CLASS_MIX			  },
		{ .text = "ptex",		  .value = TPBRT_TEXTURE_CLASS_PTEX			},
		{ .text = "scale",		   .value = TPBRT_TEXTURE_CLASS_SCALE		  },
		{ .text = "windy",		   .value = TPBRT_TEXTURE_CLASS_WINDY		  },
		{ .text = "wrinkled",	  .value = TPBRT_TEXTURE_CLASS_WRINKLED		},
	};
	check_from_string<tpbrt_texture_class_t>(tpbrt_texture_class_from_string, CASES, TPBRT_ERROR_INVALID_TEXTURE_CLASS,
	  TPBRT_TEXTURE_CLASS_MAX_NUM);
}

TEST(from_string, texture_wrap) {
	static constexpr FromStringCase<tpbrt_texture_wrap_t> CASES[] = {
		{ .text = "repeat", .value = TPBRT_TEXTURE_WRAP_REPEAT },
		{ .text = "black",  .value = TPBRT_TEXTURE_WRAP_BLACK	 },
		{ .text = "clamp",  .value = TPBRT_TEXTURE_WRAP_CLAMP	 },
	};
	check_from_string<tpbrt_texture_wrap_t>(tpbrt_texture_wrap_from_string, CASES, TPBRT_ERROR_INVALID_TEXTURE_WRAP,
	  TPBRT_TEXTURE_WRAP_MAX_NUM);
}

TEST(from_string, texture_filter) {
	static constexpr FromStringCase<tpbrt_texture_filter_t> CASES[] = {
		{ .text = "bilinear",  .value = TPBRT_TEXTURE_FILTER_BILINEAR	 },
		{ .text = "ewa",		 .value = TPBRT_TEXTURE_FILTER_EWA	   },
		{ .text = "trilinear", .value = TPBRT_TEXTURE_FILTER_TRILINEAR },
		{ .text = "point",	   .value = TPBRT_TEXTURE_FILTER_POINT	   },
	};
	check_from_string<tpbrt_texture_filter_t>(tpbrt_texture_filter_from_string, CASES, TPBRT_ERROR_INVALID_TEXTURE_FILTER,
	  TPBRT_TEXTURE_FILTER_MAX_NUM);
}

TEST(from_string, texture_mapping) {
	static constexpr FromStringCase<tpbrt_texture_mapping_t> CASES[] = {
		{ .text = "uv",			.value = TPBRT_TEXTURE_MAPPING_UV		  },
		{ .text = "spherical",   .value = TPBRT_TEXTURE_MAPPING_SPHERICAL	},
		{ .text = "cylindrical", .value = TPBRT_TEXTURE_MAPPING_CYLINDRICAL },
		{ .text = "planar",		.value = TPBRT_TEXTURE_MAPPING_PLANAR	  },
	};
	check_from_string<tpbrt_texture_mapping_t>(tpbrt_texture_mapping_from_string, CASES, TPBRT_ERROR_INVALID_TEXTURE_MAPPING,
	  TPBRT_TEXTURE_MAPPING_MAX_NUM);
}

TEST(from_string, directive) {
	static constexpr FromStringCase<tpbrt_directive_t> CASES[] = {
		{ .text = "Identity",			  .value = TPBRT_DIRECTIVE_IDENTITY			},
		{ .text = "Translate",		   .value = TPBRT_DIRECTIVE_TRANSLATE			  },
		{ .text = "Scale",			   .value = TPBRT_DIRECTIVE_SCALE				  },
		{ .text = "Rotate",				.value = TPBRT_DIRECTIVE_ROTATE				},
		{ .text = "LookAt",				.value = TPBRT_DIRECTIVE_LOOK_AT			 },
		{ .text = "CoordinateSystem",	  .value = TPBRT_DIRECTIVE_COORDINATE_SYSTEM	 },
		{ .text = "CoordSysTransform",  .value = TPBRT_DIRECTIVE_COORD_SYS_TRANSFORM },
		{ .text = "Transform",		   .value = TPBRT_DIRECTIVE_TRANSFORM			  },
		{ .text = "ConcatTransform",	 .value = TPBRT_DIRECTIVE_CONCAT_TRANSFORM	   },
		{ .text = "TransformTimes",		.value = TPBRT_DIRECTIVE_TRANSFORM_TIMES	 },
		{ .text = "ActiveTransform",	 .value = TPBRT_DIRECTIVE_ACTIVE_TRANSFORM	   },
		{ .text = "Include",			 .value = TPBRT_DIRECTIVE_INCLUDE			  },
		{ .text = "Import",				.value = TPBRT_DIRECTIVE_IMPORT				},
		{ .text = "Option",				.value = TPBRT_DIRECTIVE_OPTION				},
		{ .text = "Camera",				.value = TPBRT_DIRECTIVE_CAMERA				},
		{ .text = "Sampler",			 .value = TPBRT_DIRECTIVE_SAMPLER			  },
		{ .text = "ColorSpace",			.value = TPBRT_DIRECTIVE_COLOR_SPACE		 },
		{ .text = "Film",				  .value = TPBRT_DIRECTIVE_FILM				},
		{ .text = "PixelFilter",		 .value = TPBRT_DIRECTIVE_PIXEL_FILTER		   },
		{ .text = "Integrator",			.value = TPBRT_DIRECTIVE_INTEGRATOR			},
		{ .text = "Accelerator",		 .value = TPBRT_DIRECTIVE_ACCELERATOR		  },
		{ .text = "WorldBegin",			.value = TPBRT_DIRECTIVE_WORLD_BEGIN		 },
		{ .text = "AttributeBegin",		.value = TPBRT_DIRECTIVE_ATTRIBUTE_BEGIN	 },
		{ .text = "AttributeEnd",		  .value = TPBRT_DIRECTIVE_ATTRIBUTE_END		 },
		{ .text = "Attribute",		   .value = TPBRT_DIRECTIVE_ATTRIBUTE			  },
		{ .text = "ReverseOrientation", .value = TPBRT_DIRECTIVE_REVERSE_ORIENTATION },
		{ .text = "Shape",			   .value = TPBRT_DIRECTIVE_SHAPE				  },
		{ .text = "ObjectBegin",		 .value = TPBRT_DIRECTIVE_OBJECT_BEGIN		   },
		{ .text = "ObjectEnd",		   .value = TPBRT_DIRECTIVE_OBJECT_END		   },
		{ .text = "ObjectInstance",		.value = TPBRT_DIRECTIVE_OBJECT_INSTANCE	 },
		{ .text = "LightSource",		 .value = TPBRT_DIRECTIVE_LIGHT_SOURCE		   },
		{ .text = "AreaLightSource",	 .value = TPBRT_DIRECTIVE_AREA_LIGHT_SOURCE	},
		{ .text = "Material",			  .value = TPBRT_DIRECTIVE_MATERIAL			},
		{ .text = "MakeNamedMaterial",  .value = TPBRT_DIRECTIVE_MAKE_NAMED_MATERIAL },
		{ .text = "NamedMaterial",	   .value = TPBRT_DIRECTIVE_NAMED_MATERIAL	   },
		{ .text = "Texture",			 .value = TPBRT_DIRECTIVE_TEXTURE			  },
		{ .text = "MakeNamedMedium",	 .value = TPBRT_DIRECTIVE_MAKE_NAMED_MEDIUM	},
		{ .text = "MediumInterface",	 .value = TPBRT_DIRECTIVE_MEDIUM_INTERFACE	   },
		{ .text = "TransformBegin",		.value = TPBRT_DIRECTIVE_TRANSFORM_BEGIN	 },
		{ .text = "TransformEnd",		  .value = TPBRT_DIRECTIVE_TRANSFORM_END		 },
	};
	check_from_string<tpbrt_directive_t>(tpbrt_directive_from_string, CASES, TPBRT_ERROR_INVALID_DIRECTIVE,
	  TPBRT_DIRECTIVE_MAX_NUM);
}

namespace {
	tpbrt_error_t encoding(const std::string& text, tpbrt_texture_encoding_t* const out, tpbrt_opt_float_t* const gamma) {
		const UnterminatedString s(text);
		return tpbrt_texture_encoding_from_string(s.get(), out, gamma);
	}
} // namespace

TEST(from_string, texture_encoding_named) {
	tpbrt_texture_encoding_t e = TPBRT_TEXTURE_ENCODING_MAX_NUM;
	tpbrt_opt_float_t gamma;
	gamma.has_value = TPBRT_TRUE;
	gamma.value		= 7.0f;

	ASSERT_EQ(encoding("sRGB", &e, &gamma), TPBRT_ERROR_NONE);
	EXPECT_EQ(e, TPBRT_TEXTURE_ENCODING_SRGB);
	EXPECT_EQ(gamma.has_value, TPBRT_FALSE);

	gamma.has_value = TPBRT_TRUE;
	ASSERT_EQ(encoding("linear", &e, &gamma), TPBRT_ERROR_NONE);
	EXPECT_EQ(e, TPBRT_TEXTURE_ENCODING_LINEAR);
	EXPECT_EQ(gamma.has_value, TPBRT_FALSE);
}

TEST(from_string, texture_encoding_gamma) {
	tpbrt_texture_encoding_t e = TPBRT_TEXTURE_ENCODING_MAX_NUM;
	tpbrt_opt_float_t gamma;

	ASSERT_EQ(encoding("gamma 2.2", &e, &gamma), TPBRT_ERROR_NONE);
	EXPECT_EQ(e, TPBRT_TEXTURE_ENCODING_GAMMA);
	EXPECT_EQ(gamma.has_value, TPBRT_TRUE);
	EXPECT_FLOAT_EQ(gamma.value, 2.2f);

	ASSERT_EQ(encoding("gamma   1.8", &e, &gamma), TPBRT_ERROR_NONE);
	EXPECT_FLOAT_EQ(gamma.value, 1.8f);

	ASSERT_EQ(encoding("  gamma\t2.4 ", &e, &gamma), TPBRT_ERROR_NONE);
	EXPECT_FLOAT_EQ(gamma.value, 2.4f);

	{
		const std::string buffer = "gamma 2.5";
		tpbrt_string_t s;
		s.data = buffer.c_str();
		s.size = 7u;
		ASSERT_EQ(tpbrt_texture_encoding_from_string(s, &e, &gamma), TPBRT_ERROR_NONE);
		EXPECT_FLOAT_EQ(gamma.value, 2.0f);
	}
}

TEST(from_string, texture_encoding_invalid) {
	tpbrt_texture_encoding_t e = TPBRT_TEXTURE_ENCODING_MAX_NUM;
	tpbrt_opt_float_t gamma;
	gamma.has_value = TPBRT_FALSE;
	gamma.value		= 0.0f;

	EXPECT_EQ(encoding("srgb", &e, &gamma), TPBRT_ERROR_INVALID_TEXTURE_ENCODING);
	EXPECT_EQ(encoding("Linear", &e, &gamma), TPBRT_ERROR_INVALID_TEXTURE_ENCODING);
	EXPECT_EQ(encoding("", &e, &gamma), TPBRT_ERROR_INVALID_TEXTURE_ENCODING);
	EXPECT_EQ(encoding("abc", &e, &gamma), TPBRT_ERROR_INVALID_TEXTURE_ENCODING);
	EXPECT_EQ(encoding("xgamma 2.2", &e, &gamma), TPBRT_ERROR_INVALID_TEXTURE_ENCODING);
	EXPECT_EQ(encoding("gamma2.2", &e, &gamma), TPBRT_ERROR_INVALID_TEXTURE_ENCODING);
	EXPECT_EQ(encoding("gamma", &e, &gamma), TPBRT_ERROR_PARSE_FLOAT);
	EXPECT_EQ(encoding("gamma   ", &e, &gamma), TPBRT_ERROR_PARSE_FLOAT);
	EXPECT_EQ(encoding("gamma abc", &e, &gamma), TPBRT_ERROR_PARSE_FLOAT);
	EXPECT_EQ(encoding("gamma 0", &e, &gamma), TPBRT_ERROR_PARSE_FLOAT); // pbrt-v4: "unable to parse gamma value"
	EXPECT_EQ(encoding("gamma 2.2 2.4", &e, &gamma), TPBRT_ERROR_INVALID_TEXTURE_ENCODING);
	EXPECT_EQ(encoding("Gamma 2.2", &e, &gamma), TPBRT_ERROR_INVALID_TEXTURE_ENCODING);
	EXPECT_EQ(e, TPBRT_TEXTURE_ENCODING_MAX_NUM) << "wartosc nadpisana przy bledzie";
	EXPECT_EQ(gamma.has_value, TPBRT_FALSE) << "gamma nadpisana przy bledzie";

	{
		const std::string buffer = "abc gamma 2.2";
		tpbrt_string_t s;
		s.data = buffer.c_str();
		s.size = 3u;
		EXPECT_EQ(tpbrt_texture_encoding_from_string(s, &e, &gamma), TPBRT_ERROR_INVALID_TEXTURE_ENCODING);
	}

	tpbrt_string_t null_str;
	null_str.data = nullptr;
	null_str.size = 0u;
	EXPECT_EQ(tpbrt_texture_encoding_from_string(null_str, &e, &gamma), TPBRT_ERROR_INVALID_TEXTURE_ENCODING);
	EXPECT_EQ(encoding("sRGB", nullptr, &gamma), TPBRT_ERROR_INVALID_POINTER);
	EXPECT_EQ(encoding("sRGB", &e, nullptr), TPBRT_ERROR_INVALID_POINTER);
}

TEST(from_string, texture_encoding_in_scene) {
	const Scene ok(
	  "WorldBegin\nTexture \"t\" \"spectrum\" \"imagemap\" \"string filename\" \"a.png\" \"string encoding\" \"gamma 2.2\"\n"
	);
	ASSERT_TRUE(ok.ok()) << ok.error();
	EXPECT_EQ(
	  Scene("WorldBegin\nTexture \"t\" \"spectrum\" \"imagemap\" \"string filename\" \"a.png\" \"string encoding\" \"gamma\"\n")
		.error(),
	  TPBRT_ERROR_PARSE_FLOAT
	);
	EXPECT_EQ(
	  Scene("WorldBegin\nTexture \"t\" \"spectrum\" \"imagemap\" \"string filename\" \"a.png\" \"string encoding\" \"x\"\n")
		.error(),
	  TPBRT_ERROR_INVALID_TEXTURE_ENCODING
	);
}
