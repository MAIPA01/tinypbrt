#include <stdio.h>

#include <tinypbrt/tinypbrt.h>

#include <example_utils.h>

static int load_and_report(const char* const title, const char* const source, const char* const working_directory) {
	tpbrt_scene_t scene;
	const tpbrt_error_t err = tpbrt_scene_load_from_memory(source, working_directory, &scene);
		if (err != TPBRT_ERROR_NONE) {
			char message[128]; // NOLINT(readability-magic-numbers)
			tpbrt_get_error_message(err, message, sizeof(message));
			printf("%-28s -> error %2d: %s\n", title, (int)err, message);
			return 0;
		}

	printf("%-28s -> ok: %zu shape(s), %zu material(s)\n", title, scene.shapes.count, scene.materials.count);
	tpbrt_scene_free(&scene);
	return 1;
}

int main(const int argc, char** argv) {
	const char* const scenes_directory = argc > 1 ? argv[1] : ".";

	// clang-format off
	const char* const inline_scene = "LookAt 0 0 -5  0 0 0  0 1 0\n"
									 "Camera \"perspective\" \"float fov\" 45\n"
									 "WorldBegin\n"
									 "LightSource \"point\" \"point3 from\" [ 0 4 0 ] \"blackbody I\" 3000\n"
									 "Material \"diffuse\" \"rgb reflectance\" [ .8 .2 .2 ]\n"
									 "Shape \"sphere\" \"float radius\" 0.5\n";

	const char* const scene_with_include = "WorldBegin\n"
										   "MakeNamedMaterial \"white\" \"string type\" \"diffuse\"\n"
										   "MakeNamedMaterial \"red\" \"string type\" \"diffuse\"\n"
										   "MakeNamedMaterial \"green\" \"string type\" \"diffuse\"\n"
										   "MakeNamedMaterial \"floor\" \"string type\" \"diffuse\"\n"
										   "Include \"geometry/walls.pbrt\"\n";
	// clang-format on

	int ok							   = 1;
	ok &= load_and_report("inline scene", inline_scene, ".");
	ok &= load_and_report("scene with Include", scene_with_include, scenes_directory);

	printf("\nexpected errors:\n");
	load_and_report("unknown shape", "WorldBegin\nShape \"torus\"\n", ".");
	load_and_report("unknown directive", "WorldBegin\nSphere 1\n", ".");
	load_and_report("missing file", "WorldBegin\nInclude \"does/not/exist.pbrt\"\n", ".");
	load_and_report("wrong parameter type", "WorldBegin\nShape \"sphere\" \"string radius\" \"big\"\n", ".");
	load_and_report("index out of range",
	  "WorldBegin\nShape \"trianglemesh\" \"point3 P\" [ 0 0 0 1 0 0 0 1 0 ] \"integer indices\" [ 0 1 7 ]\n", ".");
	load_and_report("unbalanced AttributeEnd", "WorldBegin\nAttributeEnd\n", ".");
	load_and_report("unknown named material", "WorldBegin\nNamedMaterial \"nope\"\n", ".");

	return ok ? 0 : 1;
}
