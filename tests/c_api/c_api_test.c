#include <stdio.h>
#include <string.h>

#include <tinypbrt/tinypbrt.h>

static int failures = 0;

#define CHECK(cond)                                                                          \
		do {                                                                                 \
				if (!(cond)) {                                                               \
					fprintf(stderr, "%s:%d: CHECK failed: %s\n", __FILE__, __LINE__, #cond); \
					++failures;                                                              \
				}                                                                            \
	} while (0)

/* clang-format off */
static const char* const SCENE = "LookAt 0 0 -5  0 0 0  0 1 0\n"
								 "Camera \"perspective\" \"float fov\" 45\n"
								 "Film \"rgb\" \"integer xresolution\" 400 \"integer yresolution\" 400\n"
								 "WorldBegin\n"
								 "LightSource \"point\" \"point3 from\" [ 0 4 0 ] \"blackbody I\" 3000\n"
								 "AttributeBegin\n"
								 "  Material \"diffuse\" \"rgb reflectance\" [ .8 .2 .2 ]\n"
								 "  Translate 1 2 3\n"
								 "  Shape \"sphere\" \"float radius\" 0.5\n"
								 "AttributeEnd\n";
/* clang-format on */

static void print_error(const tpbrt_error_t err) {
	char message[128];
	tpbrt_get_error_message(err, message, sizeof(message));
	fprintf(stderr, "tinypbrt error %d: %s\n", (int)err, message);
}

int main(void) {
	tpbrt_scene_t scene;
	const tpbrt_error_t err = tpbrt_scene_load_from_memory(SCENE, ".", &scene);
		if (err != TPBRT_ERROR_NONE) {
			print_error(err);
			return 1;
		}

	CHECK(scene.camera.type == TPBRT_CAMERA_TYPE_PERSPECTIVE);
	CHECK(scene.camera.as.perspective.fov == 45.0f);
	CHECK(scene.film.x_resolution == 400u);
	CHECK(scene.lights.lights_sources.count == 1u);
	CHECK(scene.shapes.count == 1u);

		if (scene.shapes.count == 1u) {
			const tpbrt_shape_t* const shape = &scene.shapes.shapes[0];
			CHECK(shape->type == TPBRT_SHAPE_TYPE_SPHERE);
			CHECK(shape->as.sphere.radius == 0.5f);

			const tpbrt_mat4_t* object_to_world = NULL;
			CHECK(tpbrt_get_transform(scene.transforms, shape->transform.start_transform, &object_to_world) == TPBRT_ERROR_NONE);
				if (object_to_world != NULL) {
					CHECK(object_to_world->m[3] == 1.0f);
					CHECK(object_to_world->m[7] == 2.0f);
					CHECK(object_to_world->m[11] == 3.0f);
				}

			const tpbrt_material_t* material = NULL;
			CHECK(tpbrt_get_material_by_handle(scene.materials, shape->material, &material) == TPBRT_ERROR_NONE);
				if (material != NULL) { CHECK(material->type == TPBRT_MATERIAL_TYPE_DIFFUSE); }
		}

	tpbrt_scene_free(&scene);

	tpbrt_scene_t broken;
	CHECK(tpbrt_scene_load_from_memory("WorldBegin\nShape \"torus\"\n", ".", &broken) == TPBRT_ERROR_INVALID_SHAPE_TYPE);
	CHECK(tpbrt_scene_load_from_file("this/file/does/not/exist.pbrt", &broken) == TPBRT_ERROR_FILE_NOT_FOUND);

	char message[64];
	CHECK(tpbrt_get_error_message(TPBRT_ERROR_FILE_NOT_FOUND, message, sizeof(message)) > 0u);
	CHECK(strcmp(message, "File could not be opened or found") == 0);

		if (failures != 0) {
			fprintf(stderr, "%d check(s) failed\n", failures);
			return 1;
		}
	printf("c_api_test: OK\n");
	return 0;
}
