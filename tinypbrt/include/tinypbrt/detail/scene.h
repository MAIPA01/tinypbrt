#pragma once
#ifndef _TINYPBRT_SCENE_H_
#define _TINYPBRT_SCENE_H_

#include <tinypbrt/detail/common.h>
#include <tinypbrt/detail/coord.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		tpbrt_bool_t disable_pixel_jitter;
		tpbrt_bool_t disable_texture_filtering;
		tpbrt_bool_t disable_wave_length_jitter;
		tpbrt_float_t displacement_edge_scale;
		tpbrt_string_t mse_reference_image;
		tpbrt_string_t mse_reference_out;
		tpbrt_coordinate_system_t render_coord_sys;
		tpbrt_int_t seed;
		tpbrt_bool_t force_diffuse;
		tpbrt_bool_t pixel_stats;
		tpbrt_bool_t wavefront;
	} tpbrt_options_t;

    #define TPBRT_INVALID_INDEX ((tpbrt_size_t)-1)

    // Opcje (Możesz podmienić te deklaracje na własne #include)
    typedef struct { tpbrt_size_t dummy; } tpbrt_options_t;
    typedef struct { tpbrt_size_t dummy; } tpbrt_camera_t;
    typedef struct { tpbrt_size_t dummy; } tpbrt_film_t;
    typedef struct { tpbrt_size_t dummy; } tpbrt_integrator_t;
    typedef struct { tpbrt_size_t dummy; } tpbrt_accelerator_t;
    typedef struct { tpbrt_size_t dummy; } tpbrt_sampler_t;
    typedef struct { tpbrt_size_t dummy; } tpbrt_texture_t;
    typedef struct { tpbrt_size_t dummy; } tpbrt_material_t;
    typedef struct { tpbrt_size_t dummy; } tpbrt_light_t;
    typedef struct { tpbrt_size_t dummy; } tpbrt_area_light_t;
    typedef struct { tpbrt_size_t dummy; } tpbrt_medium_t;
    typedef struct { tpbrt_size_t dummy; } tpbrt_shape_t;

    typedef struct {
        tpbrt_camera_t params;
        float transform[16];
    } tpbrt_camera_entity_t;

    typedef struct {
        tpbrt_shape_t params;
        float transform[16];
        tpbrt_bool_t reverse_orientation;
        tpbrt_size_t material_index;      // TPBRT_INVALID_INDEX jeśli brak
        tpbrt_size_t area_light_index;    // TPBRT_INVALID_INDEX jeśli brak
    } tpbrt_shape_entity_t;

    typedef struct {
        tpbrt_string_t name;
        tpbrt_size_t shape_start;         // TPBRT_INVALID_INDEX jeśli brak
        tpbrt_size_t shape_count;
        float object_to_instance[16];
    } tpbrt_object_t;

    typedef struct {
        float instance_to_world[16];
        tpbrt_size_t object_index;
        tpbrt_size_t area_light_index;    // TPBRT_INVALID_INDEX jeśli brak
        tpbrt_bool_t reverse_orientation;
    } tpbrt_instance_t;

    typedef struct {
        float start_time;
        float end_time;
        tpbrt_options_t options;

        tpbrt_camera_entity_t* camera;        // NULL jeśli brak
        tpbrt_film_t* film;                   // NULL jeśli brak
        tpbrt_integrator_t* integrator;       // NULL jeśli brak
        tpbrt_accelerator_t* accelerator;     // NULL jeśli brak
        tpbrt_sampler_t* sampler;             // NULL jeśli brak

        tpbrt_texture_t* textures;            tpbrt_size_t textures_count;
        tpbrt_material_t* materials;          tpbrt_size_t materials_count;
        tpbrt_light_t* lights;                tpbrt_size_t lights_count;
        tpbrt_area_light_t* area_lights;      tpbrt_size_t area_lights_count;
        tpbrt_medium_t* mediums;              tpbrt_size_t mediums_count;

        tpbrt_shape_entity_t* shapes;         tpbrt_size_t shapes_count;
        tpbrt_object_t* objects;              tpbrt_size_t objects_count;
        tpbrt_instance_t* instances;          tpbrt_size_t instances_count;

        // Bufory pamięci z załadowanych plików Include (zwalniane na końcu)
        char** include_buffers;               tpbrt_size_t include_buffers_count;
    } tpbrt_scene_t;

    // Główne API
    tpbrt_error_t tpbrt_scene_load_from_file(const char* filepath, tpbrt_scene_t* out_scene);
    tpbrt_error_t tpbrt_scene_load_from_memory(const char* data, const char* working_directory, tpbrt_scene_t* out_scene);
    void tpbrt_free_scene(tpbrt_scene_t* scene);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
}
#endif
#endif // _TINYPBRT_SCENE_H_
