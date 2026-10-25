#include <stdio.h>

#include <tinypbrt/tinypbrt.h>

#include <example_utils.h>

static void print_camera(const tpbrt_scene_t* const scene) {
	const tpbrt_camera_t* const camera = &scene->camera;
	printf("camera:      %s", EXAMPLE_NAME(example_camera_names, camera->type));
		switch (camera->type) {
		case TPBRT_CAMERA_TYPE_PERSPECTIVE:
			printf(" fov=%g lensradius=%g focaldistance=%g", camera->as.perspective.fov, camera->as.perspective.lens_radius,
			  camera->as.perspective.focal_distance);
			break;
		case TPBRT_CAMERA_TYPE_ORTHOGRAPHIC:
			printf(" screenwindow=[%g %g %g %g]", camera->as.orthographic.screen_window[0],
			  camera->as.orthographic.screen_window[1], camera->as.orthographic.screen_window[2],
			  camera->as.orthographic.screen_window[3]);
			break;
		case TPBRT_CAMERA_TYPE_REALISTIC:
			printf(" lensfile=\"" EXAMPLE_STR_FMT "\" aperturediameter=%g", EXAMPLE_STR_ARG(camera->as.realistic.lens_file),
			  camera->as.realistic.aperture_diameter);
			break;
		default: break;
		}
	printf(" shutter=[%g, %g]\n", camera->shutter_open, camera->shutter_close);

	const tpbrt_mat4_t* start = TPBRT_NULL;
	const tpbrt_mat4_t* end	  = TPBRT_NULL;
	tpbrt_get_transform(&scene->transforms, camera->transform.start_transform, &start);
	tpbrt_get_transform(&scene->transforms, camera->transform.end_transform, &end);
		if (start != TPBRT_NULL && end != TPBRT_NULL) {
			// NOLINTBEGIN(readability-magic-numbers)
			printf("             position start=(%g %g %g) end=(%g %g %g)%s\n", start->m[3], start->m[7], start->m[11], end->m[3],
			  end->m[7], end->m[11], start == end ? "" : " (animated)");
			// NOLINTEND(readability-magic-numbers)
		}
}

static void print_film(const tpbrt_film_t* const film) {
	printf("film:        %s %ux%u -> \"" EXAMPLE_STR_FMT "\" iso=%g diagonal=%gmm\n",
	  EXAMPLE_NAME(example_film_names, film->type), film->x_resolution, film->y_resolution, EXAMPLE_STR_ARG(film->file_name),
	  film->iso, film->diagonal);
	printf("             cropwindow=[%g %g %g %g]\n", film->crop_window[0], film->crop_window[1], film->crop_window[2],
	  film->crop_window[3]);
}

static void print_sampler(const tpbrt_sampler_t* const sampler) {
	printf("sampler:     %s", EXAMPLE_NAME(example_sampler_names, sampler->type));
		switch (sampler->type) {
		case TPBRT_SAMPLER_TYPE_HALTON:		  printf(" pixelsamples=%u", sampler->as.halton.pixel_samples); break;
		case TPBRT_SAMPLER_TYPE_INDEPENDENT:  printf(" pixelsamples=%u", sampler->as.independent.pixel_samples); break;
		case TPBRT_SAMPLER_TYPE_PADDED_SOBOL: printf(" pixelsamples=%u", sampler->as.padded_sobol.pixel_samples); break;
		case TPBRT_SAMPLER_TYPE_SOBOL:		  printf(" pixelsamples=%u", sampler->as.sobol.pixel_samples); break;
		case TPBRT_SAMPLER_TYPE_Z_SOBOL:	  printf(" pixelsamples=%u", sampler->as.z_sobol.pixel_samples); break;
		case TPBRT_SAMPLER_TYPE_STRATIFIED:
			printf(" %ux%u jitter=%s", sampler->as.stratified.x_samples, sampler->as.stratified.y_samples,
			  sampler->as.stratified.jitter ? "true" : "false");
			break;
		default: break;
		}
	printf(" seed=%d\n", (int)sampler->seed);
}

static void print_integrator(const tpbrt_integrator_t* const integrator) {
	printf("integrator:  %s", EXAMPLE_NAME(example_integrator_names, integrator->type));
		switch (integrator->type) {
		case TPBRT_INTEGRATOR_TYPE_VOL_PATH: printf(" maxdepth=%u", integrator->as.vol_path.max_depth); break;
		case TPBRT_INTEGRATOR_TYPE_PATH:	 printf(" maxdepth=%u", integrator->as.path.max_depth); break;
		case TPBRT_INTEGRATOR_TYPE_BDPT:	 printf(" maxdepth=%u", integrator->as.bdpt.max_depth); break;
		case TPBRT_INTEGRATOR_TYPE_SPPM:
			printf(" maxdepth=%u radius=%g", integrator->as.sppm.max_depth, integrator->as.sppm.radius);
			break;
		default: break;
		}
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

	print_camera(&scene);
	print_film(&scene.film);
	print_sampler(&scene.sampler);
	print_integrator(&scene.integrator);
	printf("filter:      %s radius=(%g, %g)\n", EXAMPLE_NAME(example_filter_names, scene.filter.type), scene.filter.x_radius,
	  scene.filter.y_radius);
	printf("accelerator: %s\n", EXAMPLE_NAME(example_accelerator_names, scene.accelerator.type));
	printf("time:        [%g, %g]\n", scene.start_time, scene.end_time);

	size_t shapes_by_type[TPBRT_SHAPE_TYPE_MAX_NUM] = { 0 };
		for (size_t i = 0; i < scene.shapes.count; ++i) { ++shapes_by_type[scene.shapes.shapes[i].type]; }
	size_t object_shapes = 0u;
		for (size_t o = 0; o < scene.objects.count; ++o) {
			const tpbrt_shapes_list_t children = scene.objects.objects[o].children;
				for (size_t i = 0; i < children.count; ++i) { ++shapes_by_type[children.shapes[i].type]; }
			object_shapes += children.count;
		}

	printf("\nshapes:      %zu (+%zu inside %zu object(s), %zu instance(s))\n", scene.shapes.count, object_shapes,
	  scene.objects.count, scene.instances.count);
		for (unsigned t = 0; t < TPBRT_SHAPE_TYPE_MAX_NUM; ++t) {
				if (shapes_by_type[t] != 0u) { printf("  %-14s %zu\n", EXAMPLE_NAME(example_shape_names, t), shapes_by_type[t]); }
		}

	printf("materials:   %zu\n", scene.materials.count);
		for (size_t i = 0; i < scene.materials.count; ++i) {
			const tpbrt_material_t* const m = &scene.materials.materials[i];
			printf("  %-14s \"" EXAMPLE_STR_FMT "\"\n", EXAMPLE_NAME(example_material_names, m->type), EXAMPLE_STR_ARG(m->name));
		}

	printf("textures:    %zu\n", scene.textures.count);
		for (size_t i = 0; i < scene.textures.count; ++i) {
			const tpbrt_texture_t* const t = &scene.textures.textures[i];
			printf("  %-14s \"" EXAMPLE_STR_FMT "\" (%s)\n", EXAMPLE_NAME(example_texture_class_names, t->tex_class),
			  EXAMPLE_STR_ARG(t->name), t->type == TPBRT_TEXTURE_TYPE_FLOAT ? "float" : "spectrum");
		}

	printf("media:       %zu\n", scene.medias.count);
		for (size_t i = 0; i < scene.medias.count; ++i) {
			const tpbrt_media_t* const m = &scene.medias.medias[i];
			printf("  %-14s \"" EXAMPLE_STR_FMT "\"\n", EXAMPLE_NAME(example_media_names, m->type), EXAMPLE_STR_ARG(m->name));
		}

	printf("lights:      %zu light source(s), %zu area light(s)\n", scene.lights.lights_sources.count,
	  scene.lights.area_lights.count);

	tpbrt_scene_free(&scene);
	return 0;
}
