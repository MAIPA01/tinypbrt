#include <stdio.h>

#include <tinypbrt/tinypbrt.h>

int main(const int argc, char** argv) {
		if (argc < 2) {
			fprintf(stderr, "usage: %s scene.pbrt\n", argv[0]);
			return 1;
		}

	tpbrt_scene_t scene;
	const tpbrt_error_t err = tpbrt_scene_load_from_file(argv[1], &scene);
		if (err != TPBRT_ERROR_NONE) {
			char message[128];
			tpbrt_get_error_message(err, message, sizeof(message));
			fprintf(stderr, "failed to load %s: %s (%d)\n", argv[1], message, (int)err);
			return 1; // on error the scene is already freed
		}

	printf("film:   %ux%u -> %.*s\n", scene.film.x_resolution, scene.film.y_resolution, (int)scene.film.file_name.size,
	  scene.film.file_name.data);
	printf("shapes: %zu, lights: %zu, materials: %zu, textures: %zu\n", scene.shapes.count, scene.lights.lights_sources.count,
	  scene.materials.count, scene.textures.count);

		for (size_t i = 0; i < scene.shapes.count; ++i) {
			const tpbrt_shape_t* shape			= &scene.shapes.shapes[i];

			// object -> world matrix (row-major: m[row * 4 + column])
			const tpbrt_mat4_t* object_to_world = TPBRT_NULL;
			tpbrt_get_transform(&scene.transforms, shape->transform.start_transform, &object_to_world);

			const tpbrt_material_t* material = TPBRT_NULL;
			tpbrt_get_material_by_handle(&scene.materials, shape->material, &material);

				if (shape->type == TPBRT_SHAPE_TYPE_SPHERE) {
					printf("sphere r=%g at (%g %g %g)\n", shape->as.sphere.radius, object_to_world->m[3], object_to_world->m[7],
					  object_to_world->m[11]);
				}
				else if (shape->type == TPBRT_SHAPE_TYPE_TRIANGLE_MESH) {
					printf("triangle mesh: %zu vertices, %zu triangles\n", shape->as.triangle_mesh.p.count,
					  shape->as.triangle_mesh.indices.count / 3);
				}
			(void)material;
		}

	tpbrt_scene_free(&scene);
	return 0;
}
