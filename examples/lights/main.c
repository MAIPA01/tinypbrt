#include <stdio.h>

#include <tinypbrt/tinypbrt.h>

#include <example_utils.h>

static void print_world_point(const char* const label, const tpbrt_mat4_t* const m, const tpbrt_point_t p) {
	const tpbrt_vec3_t w = example_transform_point(m, p);
	printf(" %s=(%g %g %g)", label, w.x, w.y, w.z);
}

static void print_light(const tpbrt_scene_t* const scene, const tpbrt_light_source_t* const light) {
	const tpbrt_mat4_t* light_to_world = TPBRT_NULL;
		if (tpbrt_get_transform(&scene->transforms, light->transform, &light_to_world) != TPBRT_ERROR_NONE) { return; }

	printf("  %-11s", EXAMPLE_NAME(example_light_names, light->type));
	const tpbrt_spectrum_t* spectrum = TPBRT_NULL;
		switch (light->type) {
		case TPBRT_LIGHT_SOURCE_TYPE_POINT:
			print_world_point("position", light_to_world, light->as.point.from);
			spectrum = &light->as.point.i;
			break;
		case TPBRT_LIGHT_SOURCE_TYPE_SPOT:
			print_world_point("from", light_to_world, light->as.spot.from);
			print_world_point("to", light_to_world, light->as.spot.to);
			printf(" cone=%g/%g deg", light->as.spot.cone_angle, light->as.spot.cone_delta_angle);
			spectrum = &light->as.spot.i;
			break;
			case TPBRT_LIGHT_SOURCE_TYPE_DISTANT: {
				const tpbrt_vec3_t from = example_transform_point(light_to_world, light->as.distant.from);
				const tpbrt_vec3_t to	= example_transform_point(light_to_world, light->as.distant.to);
				printf(" direction=(%g %g %g)", to.x - from.x, to.y - from.y, to.z - from.z);
				spectrum = &light->as.distant.l;
				break;
			}
		case TPBRT_LIGHT_SOURCE_TYPE_INFINITE:
				if (light->as.infinite.file_name.size > 0u) {
					printf(" environment map \"" EXAMPLE_STR_FMT "\"", EXAMPLE_STR_ARG(light->as.infinite.file_name));
				}
				else { spectrum = &light->as.infinite.l; }
			break;
		case TPBRT_LIGHT_SOURCE_TYPE_GONIOMETRIC:
			printf(" profile \"" EXAMPLE_STR_FMT "\"", EXAMPLE_STR_ARG(light->as.goniometric.file_name));
			spectrum = &light->as.goniometric.i;
			break;
		case TPBRT_LIGHT_SOURCE_TYPE_PROJECTION:
			printf(" image \"" EXAMPLE_STR_FMT "\" fov=%g", EXAMPLE_STR_ARG(light->as.projection.file_name),
			  light->as.projection.fov);
			spectrum = &light->as.projection.i;
			break;
		default: break;
		}

		if (spectrum != TPBRT_NULL) {
			printf(" spectrum=");
			example_print_spectrum(stdout, spectrum);
		}
	printf(" scale=%g", light->scale);
		if (light->power_illuminance.has_value) { printf(" power=%g", light->power_illuminance.value); }
	printf("\n");
}

int main(const int argc, char** argv) {
		if (argc < 2) {
			fprintf(stderr, "usage: %s scene.pbrt\n", argv[0]);
			return 1;
		}

	tpbrt_scene_t scene;
	const tpbrt_error_t err = tpbrt_scene_load_from_file(argv[1], &scene);
		if (err != TPBRT_ERROR_NONE) {
			example_print_error(argv[1], err);
			return 1;
		}

	printf("light sources (%zu):\n", scene.lights.lights_sources.count);
		for (size_t i = 0; i < scene.lights.lights_sources.count; ++i) {
			print_light(&scene, &scene.lights.lights_sources.lights[i]);
		}

	printf("area lights (%zu):\n", scene.lights.area_lights.count);
		for (size_t i = 0; i < scene.shapes.count; ++i) {
			const tpbrt_shape_t* const shape = &scene.shapes.shapes[i];
			const tpbrt_area_light_t* area	 = TPBRT_NULL;
				if (tpbrt_get_area_light_by_handle(&scene.lights.area_lights, shape->area_light, &area) != TPBRT_ERROR_NONE) {
					continue;
				}

			const tpbrt_mat4_t* object_to_world = TPBRT_NULL;
			tpbrt_get_transform(&scene.transforms, shape->transform.start_transform, &object_to_world);
			printf("  shape %zu (%s) emits ", i, EXAMPLE_NAME(example_shape_names, shape->type));
			example_print_spectrum(stdout, &area->as.diffuse.l);
			printf(" scale=%g twosided=%s", area->scale, area->as.diffuse.two_sided ? "true" : "false");
				if (area->power.has_value) { printf(" power=%g", area->power.value); }
				if (object_to_world != TPBRT_NULL) {
					printf(" origin=(%g %g %g)", object_to_world->m[3], object_to_world->m[7], object_to_world->m[11]);
				}
			printf("\n");
		}

	tpbrt_scene_free(&scene);
	return 0;
}
